/*
 * bitmap.c two-level bitmap (C) Peter T. Breuer (ptb@ot.uc3m.es) 2003
 *
 * bitmap_create  - sets up the bitmap structure
 * bitmap_destroy - destroys the bitmap structure
 *
 * additions, Copyright (C) 2003-2004, Paul Clements, SteelEye Technology, Inc.:
 * - added disk storage for bitmap
 * - changes to allow various bitmap chunk sizes
 */

/*
               
  
                                                                     
 */

#include <linux/blkdev.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/file.h>
#include <linux/mount.h>
#include <linux/buffer_head.h>
#include <linux/seq_file.h>
#include "md.h"
#include "bitmap.h"

static inline char *bmname(struct bitmap *bitmap)
{
	return bitmap->mddev ? mdname(bitmap->mddev) : "mdX";
}

/*
                                                                                
  
                                                                           
                                                                       
                                        
  
                                                                         
                                 
 */
static int bitmap_checkpage(struct bitmap *bitmap,
			    unsigned long page, int create)
__releases(bitmap->lock)
__acquires(bitmap->lock)
{
	unsigned char *mappage;

	if (page >= bitmap->pages) {
		/*                                                 
                                                  
                    
   */
		return -EINVAL;
	}

	if (bitmap->bp[page].hijacked) /*                                   */
		return 0;

	if (bitmap->bp[page].map) /*                                        */
		return 0;

	if (!create)
		return -ENOENT;

	/*                                      */

	spin_unlock_irq(&bitmap->lock);
	mappage = kzalloc(PAGE_SIZE, GFP_NOIO);
	spin_lock_irq(&bitmap->lock);

	if (mappage == NULL) {
		pr_debug("%s: bitmap map page allocation failed, hijacking\n",
			 bmname(bitmap));
		/*                                                      
                          */
		if (!bitmap->bp[page].map)
			bitmap->bp[page].hijacked = 1;
	} else if (bitmap->bp[page].map ||
		   bitmap->bp[page].hijacked) {
		/*                                      */
		kfree(mappage);
		return 0;
	} else {

		/*                                                     */

		bitmap->bp[page].map = mappage;
		bitmap->missing_pages--;
	}
	return 0;
}

/*                                                                          */
/*                                                                         */
/*                                             */
static void bitmap_checkfree(struct bitmap *bitmap, unsigned long page)
{
	char *ptr;

	if (bitmap->bp[page].count) /*                    */
		return;

	/*                                              */

	if (bitmap->bp[page].hijacked) { /*                                  */
		bitmap->bp[page].hijacked = 0;
		bitmap->bp[page].map = NULL;
	} else {
		/*                            */
		ptr = bitmap->bp[page].map;
		bitmap->bp[page].map = NULL;
		bitmap->missing_pages++;
		kfree(ptr);
	}
}

/*
                                                                           
 */

/*
                            
 */

/*                                                          */
static struct page *read_sb_page(struct mddev *mddev, loff_t offset,
				 struct page *page,
				 unsigned long index, int size)
{
	/*                                                 */

	struct md_rdev *rdev;
	sector_t target;
	int did_alloc = 0;

	if (!page) {
		page = alloc_page(GFP_KERNEL);
		if (!page)
			return ERR_PTR(-ENOMEM);
		did_alloc = 1;
	}

	rdev_for_each(rdev, mddev) {
		if (! test_bit(In_sync, &rdev->flags)
		    || test_bit(Faulty, &rdev->flags))
			continue;

		target = offset + index * (PAGE_SIZE/512);

		if (sync_page_io(rdev, target,
				 roundup(size, bdev_logical_block_size(rdev->bdev)),
				 page, READ, true)) {
			page->index = index;
			attach_page_buffers(page, NULL); /*                         
                         */
			return page;
		}
	}
	if (did_alloc)
		put_page(page);
	return ERR_PTR(-EIO);

}

static struct md_rdev *next_active_rdev(struct md_rdev *rdev, struct mddev *mddev)
{
	/*                                                                  
                                                                        
                                      
                                                                  
                                                                    
                                                              
                               
  */
	struct list_head *pos;
	rcu_read_lock();
	if (rdev == NULL)
		/*                        */
		pos = &mddev->disks;
	else {
		/*                                                 */
		rdev_dec_pending(rdev, mddev);
		pos = &rdev->same_set;
	}
	list_for_each_continue_rcu(pos, &mddev->disks) {
		rdev = list_entry(pos, struct md_rdev, same_set);
		if (rdev->raid_disk >= 0 &&
		    !test_bit(Faulty, &rdev->flags)) {
			/*                          */
			atomic_inc(&rdev->nr_pending);
			rcu_read_unlock();
			return rdev;
		}
	}
	rcu_read_unlock();
	return NULL;
}

static int write_sb_page(struct bitmap *bitmap, struct page *page, int wait)
{
	struct md_rdev *rdev = NULL;
	struct block_device *bdev;
	struct mddev *mddev = bitmap->mddev;

	while ((rdev = next_active_rdev(rdev, mddev)) != NULL) {
		int size = PAGE_SIZE;
		loff_t offset = mddev->bitmap_info.offset;

		bdev = (rdev->meta_bdev) ? rdev->meta_bdev : rdev->bdev;

		if (page->index == bitmap->file_pages-1)
			size = roundup(bitmap->last_page_size,
				       bdev_logical_block_size(bdev));
		/*                                            
             
   */
		if (mddev->external) {
			/*                           */
			if (rdev->sb_start + offset + (page->index
						       * (PAGE_SIZE/512))
			    > rdev->data_offset
			    &&
			    rdev->sb_start + offset
			    < (rdev->data_offset + mddev->dev_sectors
			     + (PAGE_SIZE/512)))
				goto bad_alignment;
		} else if (offset < 0) {
			/*                        */
			if (offset
			    + (long)(page->index * (PAGE_SIZE/512))
			    + size/512 > 0)
				/*                            */
				goto bad_alignment;
			if (rdev->data_offset + mddev->dev_sectors
			    > rdev->sb_start + offset)
				/*                        */
				goto bad_alignment;
		} else if (rdev->sb_start < rdev->data_offset) {
			/*                      */
			if (rdev->sb_start
			    + offset
			    + page->index*(PAGE_SIZE/512) + size/512
			    > rdev->data_offset)
				/*                        */
				goto bad_alignment;
		} else {
			/*                                    */
		}
		md_super_write(mddev, rdev,
			       rdev->sb_start + offset
			       + page->index * (PAGE_SIZE/512),
			       size,
			       page);
	}

	if (wait)
		md_super_wait(mddev);
	return 0;

 bad_alignment:
	return -EINVAL;
}

static void bitmap_file_kick(struct bitmap *bitmap);
/*
                             
 */
static void write_page(struct bitmap *bitmap, struct page *page, int wait)
{
	struct buffer_head *bh;

	if (bitmap->file == NULL) {
		switch (write_sb_page(bitmap, page, wait)) {
		case -EINVAL:
			bitmap->flags |= BITMAP_WRITE_ERROR;
		}
	} else {

		bh = page_buffers(page);

		while (bh && bh->b_blocknr) {
			atomic_inc(&bitmap->pending_writes);
			set_buffer_locked(bh);
			set_buffer_mapped(bh);
			submit_bh(WRITE | REQ_SYNC, bh);
			bh = bh->b_this_page;
		}

		if (wait)
			wait_event(bitmap->write_wait,
				   atomic_read(&bitmap->pending_writes)==0);
	}
	if (bitmap->flags & BITMAP_WRITE_ERROR)
		bitmap_file_kick(bitmap);
}

static void end_bitmap_write(struct buffer_head *bh, int uptodate)
{
	struct bitmap *bitmap = bh->b_private;
	unsigned long flags;

	if (!uptodate) {
		spin_lock_irqsave(&bitmap->lock, flags);
		bitmap->flags |= BITMAP_WRITE_ERROR;
		spin_unlock_irqrestore(&bitmap->lock, flags);
	}
	if (atomic_dec_and_test(&bitmap->pending_writes))
		wake_up(&bitmap->write_wait);
}

/*                      */
static void
__clear_page_buffers(struct page *page)
{
	ClearPagePrivate(page);
	set_page_private(page, 0);
	page_cache_release(page);
}
static void free_buffers(struct page *page)
{
	struct buffer_head *bh = page_buffers(page);

	while (bh) {
		struct buffer_head *next = bh->b_this_page;
		free_buffer_head(bh);
		bh = next;
	}
	__clear_page_buffers(page);
	put_page(page);
}

/*                         
                                                                      
                                                                    
                                                                 
                                                                     
                                                                    
 */
static struct page *read_page(struct file *file, unsigned long index,
			      struct bitmap *bitmap,
			      unsigned long count)
{
	struct page *page = NULL;
	struct inode *inode = file->f_path.dentry->d_inode;
	struct buffer_head *bh;
	sector_t block;

	pr_debug("read bitmap file (%dB @ %llu)\n", (int)PAGE_SIZE,
		 (unsigned long long)index << PAGE_SHIFT);

	page = alloc_page(GFP_KERNEL);
	if (!page)
		page = ERR_PTR(-ENOMEM);
	if (IS_ERR(page))
		goto out;

	bh = alloc_page_buffers(page, 1<<inode->i_blkbits, 0);
	if (!bh) {
		put_page(page);
		page = ERR_PTR(-ENOMEM);
		goto out;
	}
	attach_page_buffers(page, bh);
	block = index << (PAGE_SHIFT - inode->i_blkbits);
	while (bh) {
		if (count == 0)
			bh->b_blocknr = 0;
		else {
			bh->b_blocknr = bmap(inode, block);
			if (bh->b_blocknr == 0) {
				/*                       */
				free_buffers(page);
				page = ERR_PTR(-EINVAL);
				goto out;
			}
			bh->b_bdev = inode->i_sb->s_bdev;
			if (count < (1<<inode->i_blkbits))
				count = 0;
			else
				count -= (1<<inode->i_blkbits);

			bh->b_end_io = end_bitmap_write;
			bh->b_private = bitmap;
			atomic_inc(&bitmap->pending_writes);
			set_buffer_locked(bh);
			set_buffer_mapped(bh);
			submit_bh(READ, bh);
		}
		block++;
		bh = bh->b_this_page;
	}
	page->index = index;

	wait_event(bitmap->write_wait,
		   atomic_read(&bitmap->pending_writes)==0);
	if (bitmap->flags & BITMAP_WRITE_ERROR) {
		free_buffers(page);
		page = ERR_PTR(-EIO);
	}
out:
	if (IS_ERR(page))
		printk(KERN_ALERT "md: bitmap read error: (%dB @ %llu): %ld\n",
			(int)PAGE_SIZE,
			(unsigned long long)index << PAGE_SHIFT,
			PTR_ERR(page));
	return page;
}

/*
                                    
 */

/*                                                          */
void bitmap_update_sb(struct bitmap *bitmap)
{
	bitmap_super_t *sb;

	if (!bitmap || !bitmap->mddev) /*                          */
		return;
	if (bitmap->mddev->bitmap_info.external)
		return;
	if (!bitmap->sb_page) /*               */
		return;
	sb = kmap_atomic(bitmap->sb_page);
	sb->events = cpu_to_le64(bitmap->mddev->events);
	if (bitmap->mddev->events < bitmap->events_cleared)
		/*                           */
		bitmap->events_cleared = bitmap->mddev->events;
	sb->events_cleared = cpu_to_le64(bitmap->events_cleared);
	sb->state = cpu_to_le32(bitmap->flags);
	/*                                                 */
	sb->daemon_sleep = cpu_to_le32(bitmap->mddev->bitmap_info.daemon_sleep/HZ);
	sb->write_behind = cpu_to_le32(bitmap->mddev->bitmap_info.max_write_behind);
	kunmap_atomic(sb);
	write_page(bitmap, bitmap->sb_page, 1);
}

/*                                      */
void bitmap_print_sb(struct bitmap *bitmap)
{
	bitmap_super_t *sb;

	if (!bitmap || !bitmap->sb_page)
		return;
	sb = kmap_atomic(bitmap->sb_page);
	printk(KERN_DEBUG "%s: bitmap file superblock:\n", bmname(bitmap));
	printk(KERN_DEBUG "         magic: %08x\n", le32_to_cpu(sb->magic));
	printk(KERN_DEBUG "       version: %d\n", le32_to_cpu(sb->version));
	printk(KERN_DEBUG "          uuid: %08x.%08x.%08x.%08x\n",
					*(__u32 *)(sb->uuid+0),
					*(__u32 *)(sb->uuid+4),
					*(__u32 *)(sb->uuid+8),
					*(__u32 *)(sb->uuid+12));
	printk(KERN_DEBUG "        events: %llu\n",
			(unsigned long long) le64_to_cpu(sb->events));
	printk(KERN_DEBUG "events cleared: %llu\n",
			(unsigned long long) le64_to_cpu(sb->events_cleared));
	printk(KERN_DEBUG "         state: %08x\n", le32_to_cpu(sb->state));
	printk(KERN_DEBUG "     chunksize: %d B\n", le32_to_cpu(sb->chunksize));
	printk(KERN_DEBUG "  daemon sleep: %ds\n", le32_to_cpu(sb->daemon_sleep));
	printk(KERN_DEBUG "     sync size: %llu KB\n",
			(unsigned long long)le64_to_cpu(sb->sync_size)/2);
	printk(KERN_DEBUG "max write behind: %d\n", le32_to_cpu(sb->write_behind));
	kunmap_atomic(sb);
}

/*
                     
          
  
                                                                           
                                                                              
                                                                        
                                             
  
                                        
 */
static int bitmap_new_disk_sb(struct bitmap *bitmap)
{
	bitmap_super_t *sb;
	unsigned long chunksize, daemon_sleep, write_behind;
	int err = -EINVAL;

	bitmap->sb_page = alloc_page(GFP_KERNEL);
	if (IS_ERR(bitmap->sb_page)) {
		err = PTR_ERR(bitmap->sb_page);
		bitmap->sb_page = NULL;
		return err;
	}
	bitmap->sb_page->index = 0;

	sb = kmap_atomic(bitmap->sb_page);

	sb->magic = cpu_to_le32(BITMAP_MAGIC);
	sb->version = cpu_to_le32(BITMAP_MAJOR_HI);

	chunksize = bitmap->mddev->bitmap_info.chunksize;
	BUG_ON(!chunksize);
	if (!is_power_of_2(chunksize)) {
		kunmap_atomic(sb);
		printk(KERN_ERR "bitmap chunksize not a power of 2\n");
		return -EINVAL;
	}
	sb->chunksize = cpu_to_le32(chunksize);

	daemon_sleep = bitmap->mddev->bitmap_info.daemon_sleep;
	if (!daemon_sleep ||
	    (daemon_sleep < 1) || (daemon_sleep > MAX_SCHEDULE_TIMEOUT)) {
		printk(KERN_INFO "Choosing daemon_sleep default (5 sec)\n");
		daemon_sleep = 5 * HZ;
	}
	sb->daemon_sleep = cpu_to_le32(daemon_sleep);
	bitmap->mddev->bitmap_info.daemon_sleep = daemon_sleep;

	/*
                                                          
                                                             
  */
	write_behind = bitmap->mddev->bitmap_info.max_write_behind;
	if (write_behind > COUNTER_MAX)
		write_behind = COUNTER_MAX / 2;
	sb->write_behind = cpu_to_le32(write_behind);
	bitmap->mddev->bitmap_info.max_write_behind = write_behind;

	/*                                                               */
	sb->sync_size = cpu_to_le64(bitmap->mddev->resync_max_sectors);

	memcpy(sb->uuid, bitmap->mddev->uuid, 16);

	bitmap->flags |= BITMAP_STALE;
	sb->state |= cpu_to_le32(BITMAP_STALE);
	bitmap->events_cleared = bitmap->mddev->events;
	sb->events_cleared = cpu_to_le64(bitmap->mddev->events);

	kunmap_atomic(sb);

	return 0;
}

/*                                                                            */
static int bitmap_read_sb(struct bitmap *bitmap)
{
	char *reason = NULL;
	bitmap_super_t *sb;
	unsigned long chunksize, daemon_sleep, write_behind;
	unsigned long long events;
	int err = -EINVAL;

	/*                                      */
	if (bitmap->file) {
		loff_t isize = i_size_read(bitmap->file->f_mapping->host);
		int bytes = isize > PAGE_SIZE ? PAGE_SIZE : isize;

		bitmap->sb_page = read_page(bitmap->file, 0, bitmap, bytes);
	} else {
		bitmap->sb_page = read_sb_page(bitmap->mddev,
					       bitmap->mddev->bitmap_info.offset,
					       NULL,
					       0, sizeof(bitmap_super_t));
	}
	if (IS_ERR(bitmap->sb_page)) {
		err = PTR_ERR(bitmap->sb_page);
		bitmap->sb_page = NULL;
		return err;
	}

	sb = kmap_atomic(bitmap->sb_page);

	chunksize = le32_to_cpu(sb->chunksize);
	daemon_sleep = le32_to_cpu(sb->daemon_sleep) * HZ;
	write_behind = le32_to_cpu(sb->write_behind);

	/*                                                  */
	if (sb->magic != cpu_to_le32(BITMAP_MAGIC))
		reason = "bad magic";
	else if (le32_to_cpu(sb->version) < BITMAP_MAJOR_LO ||
		 le32_to_cpu(sb->version) > BITMAP_MAJOR_HI)
		reason = "unrecognized superblock version";
	else if (chunksize < 512)
		reason = "bitmap chunksize too small";
	else if (!is_power_of_2(chunksize))
		reason = "bitmap chunksize not a power of 2";
	else if (daemon_sleep < 1 || daemon_sleep > MAX_SCHEDULE_TIMEOUT)
		reason = "daemon sleep period out of range";
	else if (write_behind > COUNTER_MAX)
		reason = "write-behind limit out of range (0 - 16383)";
	if (reason) {
		printk(KERN_INFO "%s: invalid bitmap file superblock: %s\n",
			bmname(bitmap), reason);
		goto out;
	}

	/*                                                               */
	sb->sync_size = cpu_to_le64(bitmap->mddev->resync_max_sectors);

	if (bitmap->mddev->persistent) {
		/*
                                                          
                                                   
   */
		if (memcmp(sb->uuid, bitmap->mddev->uuid, 16)) {
			printk(KERN_INFO
			       "%s: bitmap superblock UUID mismatch\n",
			       bmname(bitmap));
			goto out;
		}
		events = le64_to_cpu(sb->events);
		if (events < bitmap->mddev->events) {
			printk(KERN_INFO
			       "%s: bitmap file is out of date (%llu < %llu) "
			       "-- forcing full recovery\n",
			       bmname(bitmap), events,
			       (unsigned long long) bitmap->mddev->events);
			sb->state |= cpu_to_le32(BITMAP_STALE);
		}
	}

	/*                                            */
	bitmap->mddev->bitmap_info.chunksize = chunksize;
	bitmap->mddev->bitmap_info.daemon_sleep = daemon_sleep;
	bitmap->mddev->bitmap_info.max_write_behind = write_behind;
	bitmap->flags |= le32_to_cpu(sb->state);
	if (le32_to_cpu(sb->version) == BITMAP_MAJOR_HOSTENDIAN)
		bitmap->flags |= BITMAP_HOSTENDIAN;
	bitmap->events_cleared = le64_to_cpu(sb->events_cleared);
	if (bitmap->flags & BITMAP_STALE)
		bitmap->events_cleared = bitmap->mddev->events;
	err = 0;
out:
	kunmap_atomic(sb);
	if (err)
		bitmap_print_sb(bitmap);
	return err;
}

enum bitmap_mask_op {
	MASK_SET,
	MASK_UNSET
};

/*                                                                         */
static int bitmap_mask_state(struct bitmap *bitmap, enum bitmap_state bits,
			     enum bitmap_mask_op op)
{
	bitmap_super_t *sb;
	int old;

	if (!bitmap->sb_page) /*                     */
		return 0;
	sb = kmap_atomic(bitmap->sb_page);
	old = le32_to_cpu(sb->state) & bits;
	switch (op) {
	case MASK_SET:
		sb->state |= cpu_to_le32(bits);
		bitmap->flags |= bits;
		break;
	case MASK_UNSET:
		sb->state &= cpu_to_le32(~bits);
		bitmap->flags &= ~bits;
		break;
	default:
		BUG();
	}
	kunmap_atomic(sb);
	return old;
}

/*
                                 
 */

/*
                  
  
                                                                        
                                                                        
 */
/*                                                        */
static inline unsigned long file_page_index(struct bitmap *bitmap, unsigned long chunk)
{
	if (!bitmap->mddev->bitmap_info.external)
		chunk += sizeof(bitmap_super_t) << 3;
	return chunk >> PAGE_BIT_SHIFT;
}

/*                                                      */
static inline unsigned long file_page_offset(struct bitmap *bitmap, unsigned long chunk)
{
	if (!bitmap->mddev->bitmap_info.external)
		chunk += sizeof(bitmap_super_t) << 3;
	return chunk & (PAGE_BITS - 1);
}

/*
                                                                          
  
                                                                             
                                                                              
              
 */
static inline struct page *filemap_get_page(struct bitmap *bitmap,
					    unsigned long chunk)
{
	if (file_page_index(bitmap, chunk) >= bitmap->file_pages)
		return NULL;
	return bitmap->filemap[file_page_index(bitmap, chunk)
			       - file_page_index(bitmap, 0)];
}

static void bitmap_file_unmap(struct bitmap *bitmap)
{
	struct page **map, *sb_page;
	unsigned long *attr;
	int pages;
	unsigned long flags;

	spin_lock_irqsave(&bitmap->lock, flags);
	map = bitmap->filemap;
	bitmap->filemap = NULL;
	attr = bitmap->filemap_attr;
	bitmap->filemap_attr = NULL;
	pages = bitmap->file_pages;
	bitmap->file_pages = 0;
	sb_page = bitmap->sb_page;
	bitmap->sb_page = NULL;
	spin_unlock_irqrestore(&bitmap->lock, flags);

	while (pages--)
		if (map[pages] != sb_page) /*                                */
			free_buffers(map[pages]);
	kfree(map);
	kfree(attr);

	if (sb_page)
		free_buffers(sb_page);
}

static void bitmap_file_put(struct bitmap *bitmap)
{
	struct file *file;
	unsigned long flags;

	spin_lock_irqsave(&bitmap->lock, flags);
	file = bitmap->file;
	bitmap->file = NULL;
	spin_unlock_irqrestore(&bitmap->lock, flags);

	if (file)
		wait_event(bitmap->write_wait,
			   atomic_read(&bitmap->pending_writes)==0);
	bitmap_file_unmap(bitmap);

	if (file) {
		struct inode *inode = file->f_path.dentry->d_inode;
		invalidate_mapping_pages(inode->i_mapping, 0, -1);
		fput(file);
	}
}

/*
                                                                           
                                                                          
                              
 */
static void bitmap_file_kick(struct bitmap *bitmap)
{
	char *path, *ptr = NULL;

	if (bitmap_mask_state(bitmap, BITMAP_STALE, MASK_SET) == 0) {
		bitmap_update_sb(bitmap);

		if (bitmap->file) {
			path = kmalloc(PAGE_SIZE, GFP_KERNEL);
			if (path)
				ptr = d_path(&bitmap->file->f_path, path,
					     PAGE_SIZE);

			printk(KERN_ALERT
			      "%s: kicking failed bitmap file %s from array!\n",
			      bmname(bitmap), IS_ERR(ptr) ? "" : ptr);

			kfree(path);
		} else
			printk(KERN_ALERT
			       "%s: disabling internal bitmap due to errors\n",
			       bmname(bitmap));
	}

	bitmap_file_put(bitmap);

	return;
}

enum bitmap_page_attr {
	BITMAP_PAGE_DIRTY = 0,     /*                                           */
	BITMAP_PAGE_PENDING = 1,   /*                                       
                                  */
	BITMAP_PAGE_NEEDWRITE = 2, /*                                               */
};

static inline void set_page_attr(struct bitmap *bitmap, struct page *page,
				enum bitmap_page_attr attr)
{
	__set_bit((page->index<<2) + attr, bitmap->filemap_attr);
}

static inline void clear_page_attr(struct bitmap *bitmap, struct page *page,
				enum bitmap_page_attr attr)
{
	__clear_bit((page->index<<2) + attr, bitmap->filemap_attr);
}

static inline unsigned long test_page_attr(struct bitmap *bitmap, struct page *page,
					   enum bitmap_page_attr attr)
{
	return test_bit((page->index<<2) + attr, bitmap->filemap_attr);
}

/*
                                                                           
                                                                   
  
                                                                     
                                                                  
 */
static void bitmap_file_set_bit(struct bitmap *bitmap, sector_t block)
{
	unsigned long bit;
	struct page *page;
	void *kaddr;
	unsigned long chunk = block >> bitmap->chunkshift;

	if (!bitmap->filemap)
		return;

	page = filemap_get_page(bitmap, chunk);
	if (!page)
		return;
	bit = file_page_offset(bitmap, chunk);

	/*             */
	kaddr = kmap_atomic(page);
	if (bitmap->flags & BITMAP_HOSTENDIAN)
		set_bit(bit, kaddr);
	else
		__set_bit_le(bit, kaddr);
	kunmap_atomic(kaddr);
	pr_debug("set file bit %lu page %lu\n", bit, page->index);
	/*                                                                  */
	set_page_attr(bitmap, page, BITMAP_PAGE_DIRTY);
}

/*                                                                      
                                                                        
                                                   */
void bitmap_unplug(struct bitmap *bitmap)
{
	unsigned long i, flags;
	int dirty, need_write;
	struct page *page;
	int wait = 0;

	if (!bitmap)
		return;

	/*                                                                   
                        */
	for (i = 0; i < bitmap->file_pages; i++) {
		spin_lock_irqsave(&bitmap->lock, flags);
		if (!bitmap->filemap) {
			spin_unlock_irqrestore(&bitmap->lock, flags);
			return;
		}
		page = bitmap->filemap[i];
		dirty = test_page_attr(bitmap, page, BITMAP_PAGE_DIRTY);
		need_write = test_page_attr(bitmap, page, BITMAP_PAGE_NEEDWRITE);
		clear_page_attr(bitmap, page, BITMAP_PAGE_DIRTY);
		clear_page_attr(bitmap, page, BITMAP_PAGE_NEEDWRITE);
		if (dirty)
			wait = 1;
		spin_unlock_irqrestore(&bitmap->lock, flags);

		if (dirty || need_write)
			write_page(bitmap, page, 0);
	}
	if (wait) { /*                                                       */
		if (bitmap->file)
			wait_event(bitmap->write_wait,
				   atomic_read(&bitmap->pending_writes)==0);
		else
			md_super_wait(bitmap->mddev);
	}
	if (bitmap->flags & BITMAP_WRITE_ERROR)
		bitmap_file_kick(bitmap);
}
EXPORT_SYMBOL(bitmap_unplug);

static void bitmap_set_memory_bits(struct bitmap *bitmap, sector_t offset, int needed);
/*                                                                      
                                                                    
                                    
                 
                                                              
                                                              
                                         
  
                                                                
                                                     
 */
static int bitmap_init_from_disk(struct bitmap *bitmap, sector_t start)
{
	unsigned long i, chunks, index, oldindex, bit;
	struct page *page = NULL, *oldpage = NULL;
	unsigned long num_pages, bit_cnt = 0;
	struct file *file;
	unsigned long bytes, offset;
	int outofdate;
	int ret = -ENOSPC;
	void *paddr;

	chunks = bitmap->chunks;
	file = bitmap->file;

	BUG_ON(!file && !bitmap->mddev->bitmap_info.offset);

	outofdate = bitmap->flags & BITMAP_STALE;
	if (outofdate)
		printk(KERN_INFO "%s: bitmap file is out of date, doing full "
			"recovery\n", bmname(bitmap));

	bytes = DIV_ROUND_UP(bitmap->chunks, 8);
	if (!bitmap->mddev->bitmap_info.external)
		bytes += sizeof(bitmap_super_t);

	num_pages = DIV_ROUND_UP(bytes, PAGE_SIZE);

	if (file && i_size_read(file->f_mapping->host) < bytes) {
		printk(KERN_INFO "%s: bitmap file too short %lu < %lu\n",
			bmname(bitmap),
			(unsigned long) i_size_read(file->f_mapping->host),
			bytes);
		goto err;
	}

	ret = -ENOMEM;

	bitmap->filemap = kmalloc(sizeof(struct page *) * num_pages, GFP_KERNEL);
	if (!bitmap->filemap)
		goto err;

	/*                                                                            */
	bitmap->filemap_attr = kzalloc(
		roundup(DIV_ROUND_UP(num_pages*4, 8), sizeof(unsigned long)),
		GFP_KERNEL);
	if (!bitmap->filemap_attr)
		goto err;

	oldindex = ~0L;

	for (i = 0; i < chunks; i++) {
		int b;
		index = file_page_index(bitmap, i);
		bit = file_page_offset(bitmap, i);
		if (index != oldindex) { /*                                */
			int count;
			/*                                        */
			if (index == num_pages-1)
				count = bytes - index * PAGE_SIZE;
			else
				count = PAGE_SIZE;
			if (index == 0 && bitmap->sb_page) {
				/*
                                             
                                                  
                                               
     */
				page = bitmap->sb_page;
				offset = sizeof(bitmap_super_t);
				if (!file)
					page = read_sb_page(
						bitmap->mddev,
						bitmap->mddev->bitmap_info.offset,
						page,
						index, count);
			} else if (file) {
				page = read_page(file, index, bitmap, count);
				offset = 0;
			} else {
				page = read_sb_page(bitmap->mddev,
						    bitmap->mddev->bitmap_info.offset,
						    NULL,
						    index, count);
				offset = 0;
			}
			if (IS_ERR(page)) { /*            */
				ret = PTR_ERR(page);
				goto err;
			}

			oldindex = index;
			oldpage = page;

			bitmap->filemap[bitmap->file_pages++] = page;
			bitmap->last_page_size = count;

			if (outofdate) {
				/*
                                          
                                  
     */
				paddr = kmap_atomic(page);
				memset(paddr + offset, 0xff,
				       PAGE_SIZE - offset);
				kunmap_atomic(paddr);
				write_page(bitmap, page, 1);

				ret = -EIO;
				if (bitmap->flags & BITMAP_WRITE_ERROR)
					goto err;
			}
		}
		paddr = kmap_atomic(page);
		if (bitmap->flags & BITMAP_HOSTENDIAN)
			b = test_bit(bit, paddr);
		else
			b = test_bit_le(bit, paddr);
		kunmap_atomic(paddr);
		if (b) {
			/*                                            */
			int needed = ((sector_t)(i+1) << bitmap->chunkshift
				      >= start);
			bitmap_set_memory_bits(bitmap,
					       (sector_t)i << bitmap->chunkshift,
					       needed);
			bit_cnt++;
		}
	}

	/*                    */
	ret = 0;
	bitmap_mask_state(bitmap, BITMAP_STALE, MASK_UNSET);

	if (bit_cnt) { /*                                    */
		set_bit(MD_RECOVERY_NEEDED, &bitmap->mddev->recovery);
		md_wakeup_thread(bitmap->mddev->thread);
	}

	printk(KERN_INFO "%s: bitmap initialized from disk: "
	       "read %lu/%lu pages, set %lu of %lu bits\n",
	       bmname(bitmap), bitmap->file_pages, num_pages, bit_cnt, chunks);

	return 0;

 err:
	printk(KERN_INFO "%s: bitmap initialisation failed: %d\n",
	       bmname(bitmap), ret);
	return ret;
}

void bitmap_write_all(struct bitmap *bitmap)
{
	/*                                                
                                           
  */
	int i;

	spin_lock_irq(&bitmap->lock);
	for (i = 0; i < bitmap->file_pages; i++)
		set_page_attr(bitmap, bitmap->filemap[i],
			      BITMAP_PAGE_NEEDWRITE);
	bitmap->allclean = 0;
	spin_unlock_irq(&bitmap->lock);
}

static void bitmap_count_page(struct bitmap *bitmap, sector_t offset, int inc)
{
	sector_t chunk = offset >> bitmap->chunkshift;
	unsigned long page = chunk >> PAGE_COUNTER_SHIFT;
	bitmap->bp[page].count += inc;
	bitmap_checkfree(bitmap, page);
}
static bitmap_counter_t *bitmap_get_counter(struct bitmap *bitmap,
					    sector_t offset, sector_t *blocks,
					    int create);

/*
                                                                       
                
 */

void bitmap_daemon_work(struct mddev *mddev)
{
	struct bitmap *bitmap;
	unsigned long j;
	unsigned long flags;
	struct page *page = NULL, *lastpage = NULL;
	sector_t blocks;
	void *paddr;

	/*                                         
                   
  */
	mutex_lock(&mddev->bitmap_info.mutex);
	bitmap = mddev->bitmap;
	if (bitmap == NULL) {
		mutex_unlock(&mddev->bitmap_info.mutex);
		return;
	}
	if (time_before(jiffies, bitmap->daemon_lastrun
			+ mddev->bitmap_info.daemon_sleep))
		goto done;

	bitmap->daemon_lastrun = jiffies;
	if (bitmap->allclean) {
		mddev->thread->timeout = MAX_SCHEDULE_TIMEOUT;
		goto done;
	}
	bitmap->allclean = 1;

	spin_lock_irqsave(&bitmap->lock, flags);
	for (j = 0; j < bitmap->chunks; j++) {
		bitmap_counter_t *bmc;
		if (!bitmap->filemap)
			/*                   */
			break;

		page = filemap_get_page(bitmap, j);

		if (page != lastpage) {
			/*                                                       */
			if (!test_page_attr(bitmap, page, BITMAP_PAGE_PENDING)) {
				int need_write = test_page_attr(bitmap, page,
								BITMAP_PAGE_NEEDWRITE);
				if (need_write)
					clear_page_attr(bitmap, page, BITMAP_PAGE_NEEDWRITE);

				spin_unlock_irqrestore(&bitmap->lock, flags);
				if (need_write)
					write_page(bitmap, page, 0);
				spin_lock_irqsave(&bitmap->lock, flags);
				j |= (PAGE_BITS - 1);
				continue;
			}

			/*                                             */
			if (lastpage != NULL) {
				if (test_page_attr(bitmap, lastpage,
						   BITMAP_PAGE_NEEDWRITE)) {
					clear_page_attr(bitmap, lastpage,
							BITMAP_PAGE_NEEDWRITE);
					spin_unlock_irqrestore(&bitmap->lock, flags);
					write_page(bitmap, lastpage, 0);
				} else {
					set_page_attr(bitmap, lastpage,
						      BITMAP_PAGE_NEEDWRITE);
					bitmap->allclean = 0;
					spin_unlock_irqrestore(&bitmap->lock, flags);
				}
			} else
				spin_unlock_irqrestore(&bitmap->lock, flags);
			lastpage = page;

			/*                                                  
                                             
    */
			if (bitmap->need_sync &&
			    mddev->bitmap_info.external == 0) {
				bitmap_super_t *sb;
				bitmap->need_sync = 0;
				sb = kmap_atomic(bitmap->sb_page);
				sb->events_cleared =
					cpu_to_le64(bitmap->events_cleared);
				kunmap_atomic(sb);
				write_page(bitmap, bitmap->sb_page, 1);
			}
			spin_lock_irqsave(&bitmap->lock, flags);
			if (!bitmap->need_sync)
				clear_page_attr(bitmap, page, BITMAP_PAGE_PENDING);
			else
				bitmap->allclean = 0;
		}
		bmc = bitmap_get_counter(bitmap,
					 (sector_t)j << bitmap->chunkshift,
					 &blocks, 0);
		if (!bmc)
			j |= PAGE_COUNTER_MASK;
		else if (*bmc) {
			if (*bmc == 1 && !bitmap->need_sync) {
				/*                      */
				*bmc = 0;
				bitmap_count_page(bitmap,
						  (sector_t)j << bitmap->chunkshift,
						  -1);

				/*               */
				paddr = kmap_atomic(page);
				if (bitmap->flags & BITMAP_HOSTENDIAN)
					clear_bit(file_page_offset(bitmap, j),
						  paddr);
				else
					__clear_bit_le(
						file_page_offset(bitmap,
								 j),
						paddr);
				kunmap_atomic(paddr);
			} else if (*bmc <= 2) {
				*bmc = 1; /*                               */
				set_page_attr(bitmap, page, BITMAP_PAGE_PENDING);
				bitmap->allclean = 0;
			}
		}
	}
	spin_unlock_irqrestore(&bitmap->lock, flags);

	/*                         */
	if (lastpage != NULL) {
		spin_lock_irqsave(&bitmap->lock, flags);
		if (test_page_attr(bitmap, lastpage, BITMAP_PAGE_NEEDWRITE)) {
			clear_page_attr(bitmap, lastpage, BITMAP_PAGE_NEEDWRITE);
			spin_unlock_irqrestore(&bitmap->lock, flags);
			write_page(bitmap, lastpage, 0);
		} else {
			set_page_attr(bitmap, lastpage, BITMAP_PAGE_NEEDWRITE);
			bitmap->allclean = 0;
			spin_unlock_irqrestore(&bitmap->lock, flags);
		}
	}

 done:
	if (bitmap->allclean == 0)
		mddev->thread->timeout =
			mddev->bitmap_info.daemon_sleep;
	mutex_unlock(&mddev->bitmap_info.mutex);
}

static bitmap_counter_t *bitmap_get_counter(struct bitmap *bitmap,
					    sector_t offset, sector_t *blocks,
					    int create)
__releases(bitmap->lock)
__acquires(bitmap->lock)
{
	/*                                                       
                                                          
                                          
  */
	sector_t chunk = offset >> bitmap->chunkshift;
	unsigned long page = chunk >> PAGE_COUNTER_SHIFT;
	unsigned long pageoff = (chunk & PAGE_COUNTER_MASK) << COUNTER_BYTE_SHIFT;
	sector_t csize;
	int err;

	err = bitmap_checkpage(bitmap, page, create);

	if (bitmap->bp[page].hijacked ||
	    bitmap->bp[page].map == NULL)
		csize = ((sector_t)1) << (bitmap->chunkshift +
					  PAGE_COUNTER_SHIFT - 1);
	else
		csize = ((sector_t)1) << bitmap->chunkshift;
	*blocks = csize - (offset & (csize - 1));

	if (err < 0)
		return NULL;

	/*                */

	if (bitmap->bp[page].hijacked) { /*                  */
		/*                                                
                              */
		int hi = (pageoff > PAGE_COUNTER_MASK);
		return  &((bitmap_counter_t *)
			  &bitmap->bp[page].map)[hi];
	} else /*                   */
		return (bitmap_counter_t *)
			&(bitmap->bp[page].map[pageoff]);
}

int bitmap_startwrite(struct bitmap *bitmap, sector_t offset, unsigned long sectors, int behind)
{
	if (!bitmap)
		return 0;

	if (behind) {
		int bw;
		atomic_inc(&bitmap->behind_writes);
		bw = atomic_read(&bitmap->behind_writes);
		if (bw > bitmap->behind_writes_used)
			bitmap->behind_writes_used = bw;

		pr_debug("inc write-behind count %d/%lu\n",
			 bw, bitmap->mddev->bitmap_info.max_write_behind);
	}

	while (sectors) {
		sector_t blocks;
		bitmap_counter_t *bmc;

		spin_lock_irq(&bitmap->lock);
		bmc = bitmap_get_counter(bitmap, offset, &blocks, 1);
		if (!bmc) {
			spin_unlock_irq(&bitmap->lock);
			return 0;
		}

		if (unlikely(COUNTER(*bmc) == COUNTER_MAX)) {
			DEFINE_WAIT(__wait);
			/*                                               
                                                        
                   
    */
			prepare_to_wait(&bitmap->overflow_wait, &__wait,
					TASK_UNINTERRUPTIBLE);
			spin_unlock_irq(&bitmap->lock);
			io_schedule();
			finish_wait(&bitmap->overflow_wait, &__wait);
			continue;
		}

		switch (*bmc) {
		case 0:
			bitmap_file_set_bit(bitmap, offset);
			bitmap_count_page(bitmap, offset, 1);
			/*              */
		case 1:
			*bmc = 2;
		}

		(*bmc)++;

		spin_unlock_irq(&bitmap->lock);

		offset += blocks;
		if (sectors > blocks)
			sectors -= blocks;
		else
			sectors = 0;
	}
	return 0;
}
EXPORT_SYMBOL(bitmap_startwrite);

void bitmap_endwrite(struct bitmap *bitmap, sector_t offset, unsigned long sectors,
		     int success, int behind)
{
	if (!bitmap)
		return;
	if (behind) {
		if (atomic_dec_and_test(&bitmap->behind_writes))
			wake_up(&bitmap->behind_wait);
		pr_debug("dec write-behind count %d/%lu\n",
			 atomic_read(&bitmap->behind_writes),
			 bitmap->mddev->bitmap_info.max_write_behind);
	}

	while (sectors) {
		sector_t blocks;
		unsigned long flags;
		bitmap_counter_t *bmc;

		spin_lock_irqsave(&bitmap->lock, flags);
		bmc = bitmap_get_counter(bitmap, offset, &blocks, 0);
		if (!bmc) {
			spin_unlock_irqrestore(&bitmap->lock, flags);
			return;
		}

		if (success && !bitmap->mddev->degraded &&
		    bitmap->events_cleared < bitmap->mddev->events) {
			bitmap->events_cleared = bitmap->mddev->events;
			bitmap->need_sync = 1;
			sysfs_notify_dirent_safe(bitmap->sysfs_can_clear);
		}

		if (!success && !NEEDED(*bmc))
			*bmc |= NEEDED_MASK;

		if (COUNTER(*bmc) == COUNTER_MAX)
			wake_up(&bitmap->overflow_wait);

		(*bmc)--;
		if (*bmc <= 2) {
			set_page_attr(bitmap,
				      filemap_get_page(
					      bitmap,
					      offset >> bitmap->chunkshift),
				      BITMAP_PAGE_PENDING);
			bitmap->allclean = 0;
		}
		spin_unlock_irqrestore(&bitmap->lock, flags);
		offset += blocks;
		if (sectors > blocks)
			sectors -= blocks;
		else
			sectors = 0;
	}
}
EXPORT_SYMBOL(bitmap_endwrite);

static int __bitmap_start_sync(struct bitmap *bitmap, sector_t offset, sector_t *blocks,
			       int degraded)
{
	bitmap_counter_t *bmc;
	int rv;
	if (bitmap == NULL) {/*                                 */
		*blocks = 1024;
		return 1; /*                            */
	}
	spin_lock_irq(&bitmap->lock);
	bmc = bitmap_get_counter(bitmap, offset, blocks, 0);
	rv = 0;
	if (bmc) {
		/*        */
		if (RESYNC(*bmc))
			rv = 1;
		else if (NEEDED(*bmc)) {
			rv = 1;
			if (!degraded) { /*                                  */
				*bmc |= RESYNC_MASK;
				*bmc &= ~NEEDED_MASK;
			}
		}
	}
	spin_unlock_irq(&bitmap->lock);
	return rv;
}

int bitmap_start_sync(struct bitmap *bitmap, sector_t offset, sector_t *blocks,
		      int degraded)
{
	/*                                                           
                                                                
                 
                                                            
                                             
                                  
  */
	int rv = 0;
	sector_t blocks1;

	*blocks = 0;
	while (*blocks < (PAGE_SIZE>>9)) {
		rv |= __bitmap_start_sync(bitmap, offset,
					  &blocks1, degraded);
		offset += blocks1;
		*blocks += blocks1;
	}
	return rv;
}
EXPORT_SYMBOL(bitmap_start_sync);

void bitmap_end_sync(struct bitmap *bitmap, sector_t offset, sector_t *blocks, int aborted)
{
	bitmap_counter_t *bmc;
	unsigned long flags;

	if (bitmap == NULL) {
		*blocks = 1024;
		return;
	}
	spin_lock_irqsave(&bitmap->lock, flags);
	bmc = bitmap_get_counter(bitmap, offset, blocks, 0);
	if (bmc == NULL)
		goto unlock;
	/*        */
	if (RESYNC(*bmc)) {
		*bmc &= ~RESYNC_MASK;

		if (!NEEDED(*bmc) && aborted)
			*bmc |= NEEDED_MASK;
		else {
			if (*bmc <= 2) {
				set_page_attr(bitmap,
					      filemap_get_page(bitmap, offset >> bitmap->chunkshift),
					      BITMAP_PAGE_PENDING);
				bitmap->allclean = 0;
			}
		}
	}
 unlock:
	spin_unlock_irqrestore(&bitmap->lock, flags);
}
EXPORT_SYMBOL(bitmap_end_sync);

void bitmap_close_sync(struct bitmap *bitmap)
{
	/*                                                             
                                                              
                                      
  */
	sector_t sector = 0;
	sector_t blocks;
	if (!bitmap)
		return;
	while (sector < bitmap->mddev->resync_max_sectors) {
		bitmap_end_sync(bitmap, sector, &blocks, 0);
		sector += blocks;
	}
}
EXPORT_SYMBOL(bitmap_close_sync);

void bitmap_cond_end_sync(struct bitmap *bitmap, sector_t sector)
{
	sector_t s = 0;
	sector_t blocks;

	if (!bitmap)
		return;
	if (sector == 0) {
		bitmap->last_end_sync = jiffies;
		return;
	}
	if (time_before(jiffies, (bitmap->last_end_sync
				  + bitmap->mddev->bitmap_info.daemon_sleep)))
		return;
	wait_event(bitmap->mddev->recovery_wait,
		   atomic_read(&bitmap->mddev->recovery_active) == 0);

	bitmap->mddev->curr_resync_completed = sector;
	set_bit(MD_CHANGE_CLEAN, &bitmap->mddev->flags);
	sector &= ~((1ULL << bitmap->chunkshift) - 1);
	s = 0;
	while (s < sector && s < bitmap->mddev->resync_max_sectors) {
		bitmap_end_sync(bitmap, s, &blocks, 0);
		s += blocks;
	}
	bitmap->last_end_sync = jiffies;
	sysfs_notify(&bitmap->mddev->kobj, NULL, "sync_completed");
}
EXPORT_SYMBOL(bitmap_cond_end_sync);

static void bitmap_set_memory_bits(struct bitmap *bitmap, sector_t offset, int needed)
{
	/*                                                        
                                                        
                      
  */

	sector_t secs;
	bitmap_counter_t *bmc;
	spin_lock_irq(&bitmap->lock);
	bmc = bitmap_get_counter(bitmap, offset, &secs, 1);
	if (!bmc) {
		spin_unlock_irq(&bitmap->lock);
		return;
	}
	if (!*bmc) {
		struct page *page;
		*bmc = 2 | (needed ? NEEDED_MASK : 0);
		bitmap_count_page(bitmap, offset, 1);
		page = filemap_get_page(bitmap, offset >> bitmap->chunkshift);
		set_page_attr(bitmap, page, BITMAP_PAGE_PENDING);
		bitmap->allclean = 0;
	}
	spin_unlock_irq(&bitmap->lock);
}

/*                                                             */
void bitmap_dirty_bits(struct bitmap *bitmap, unsigned long s, unsigned long e)
{
	unsigned long chunk;

	for (chunk = s; chunk <= e; chunk++) {
		sector_t sec = (sector_t)chunk << bitmap->chunkshift;
		bitmap_set_memory_bits(bitmap, sec, 1);
		spin_lock_irq(&bitmap->lock);
		bitmap_file_set_bit(bitmap, sec);
		spin_unlock_irq(&bitmap->lock);
		if (sec < bitmap->mddev->recovery_cp)
			/*                                          
                                             
                                         
    */
			bitmap->mddev->recovery_cp = sec;
	}
}

/*
                                
 */
void bitmap_flush(struct mddev *mddev)
{
	struct bitmap *bitmap = mddev->bitmap;
	long sleep;

	if (!bitmap) /*                     */
		return;

	/*                                                               
               
  */
	sleep = mddev->bitmap_info.daemon_sleep * 2;
	bitmap->daemon_lastrun -= sleep;
	bitmap_daemon_work(mddev);
	bitmap->daemon_lastrun -= sleep;
	bitmap_daemon_work(mddev);
	bitmap->daemon_lastrun -= sleep;
	bitmap_daemon_work(mddev);
	bitmap_update_sb(bitmap);
}

/*
                                 
 */
static void bitmap_free(struct bitmap *bitmap)
{
	unsigned long k, pages;
	struct bitmap_page *bp;

	if (!bitmap) /*                     */
		return;

	/*                                             */
	bitmap_file_put(bitmap);

	bp = bitmap->bp;
	pages = bitmap->pages;

	/*                           */

	if (bp) /*                            */
		for (k = 0; k < pages; k++)
			if (bp[k].map && !bp[k].hijacked)
				kfree(bp[k].map);
	kfree(bp);
	kfree(bitmap);
}

void bitmap_destroy(struct mddev *mddev)
{
	struct bitmap *bitmap = mddev->bitmap;

	if (!bitmap) /*                     */
		return;

	mutex_lock(&mddev->bitmap_info.mutex);
	mddev->bitmap = NULL; /*                               */
	mutex_unlock(&mddev->bitmap_info.mutex);
	if (mddev->thread)
		mddev->thread->timeout = MAX_SCHEDULE_TIMEOUT;

	if (bitmap->sysfs_can_clear)
		sysfs_put(bitmap->sysfs_can_clear);

	bitmap_free(bitmap);
}

/*
                                  
                                                                         
 */
int bitmap_create(struct mddev *mddev)
{
	struct bitmap *bitmap;
	sector_t blocks = mddev->resync_max_sectors;
	unsigned long chunks;
	unsigned long pages;
	struct file *file = mddev->bitmap_info.file;
	int err;
	struct sysfs_dirent *bm = NULL;

	BUILD_BUG_ON(sizeof(bitmap_super_t) != 256);

	if (!file
	    && !mddev->bitmap_info.offset) /*                                */
		return 0;

	BUG_ON(file && mddev->bitmap_info.offset);

	bitmap = kzalloc(sizeof(*bitmap), GFP_KERNEL);
	if (!bitmap)
		return -ENOMEM;

	spin_lock_init(&bitmap->lock);
	atomic_set(&bitmap->pending_writes, 0);
	init_waitqueue_head(&bitmap->write_wait);
	init_waitqueue_head(&bitmap->overflow_wait);
	init_waitqueue_head(&bitmap->behind_wait);

	bitmap->mddev = mddev;

	if (mddev->kobj.sd)
		bm = sysfs_get_dirent(mddev->kobj.sd, NULL, "bitmap");
	if (bm) {
		bitmap->sysfs_can_clear = sysfs_get_dirent(bm, NULL, "can_clear");
		sysfs_put(bm);
	} else
		bitmap->sysfs_can_clear = NULL;

	bitmap->file = file;
	if (file) {
		get_file(file);
		/*                                               
                                                     
           
   */
		vfs_fsync(file, 1);
	}
	/*                                                                           */
	if (!mddev->bitmap_info.external) {
		/*
                                                          
                                                            
   */
		if (test_and_clear_bit(MD_ARRAY_FIRST_USE, &mddev->flags))
			err = bitmap_new_disk_sb(bitmap);
		else
			err = bitmap_read_sb(bitmap);
	} else {
		err = 0;
		if (mddev->bitmap_info.chunksize == 0 ||
		    mddev->bitmap_info.daemon_sleep == 0)
			/*                                   
                 */
			err = -EINVAL;
	}
	if (err)
		goto error;

	bitmap->daemon_lastrun = jiffies;
	bitmap->chunkshift = (ffz(~mddev->bitmap_info.chunksize)
			      - BITMAP_BLOCK_SHIFT);

	chunks = (blocks + (1 << bitmap->chunkshift) - 1) >>
			bitmap->chunkshift;
	pages = (chunks + PAGE_COUNTER_RATIO - 1) / PAGE_COUNTER_RATIO;

	BUG_ON(!pages);

	bitmap->chunks = chunks;
	bitmap->pages = pages;
	bitmap->missing_pages = pages;

	bitmap->bp = kzalloc(pages * sizeof(*bitmap->bp), GFP_KERNEL);

	err = -ENOMEM;
	if (!bitmap->bp)
		goto error;

	printk(KERN_INFO "created bitmap (%lu pages) for device %s\n",
		pages, bmname(bitmap));

	mddev->bitmap = bitmap;


	return (bitmap->flags & BITMAP_WRITE_ERROR) ? -EIO : 0;

 error:
	bitmap_free(bitmap);
	return err;
}

int bitmap_load(struct mddev *mddev)
{
	int err = 0;
	sector_t start = 0;
	sector_t sector = 0;
	struct bitmap *bitmap = mddev->bitmap;

	if (!bitmap)
		goto out;

	/*                                                              
                                                                
                                        
                                                         
  */
	while (sector < mddev->resync_max_sectors) {
		sector_t blocks;
		bitmap_start_sync(bitmap, sector, &blocks, 0);
		sector += blocks;
	}
	bitmap_close_sync(bitmap);

	if (mddev->degraded == 0
	    || bitmap->events_cleared == mddev->events)
		/*                                         
                                */
		start = mddev->recovery_cp;

	mutex_lock(&mddev->bitmap_info.mutex);
	err = bitmap_init_from_disk(bitmap, start);
	mutex_unlock(&mddev->bitmap_info.mutex);

	if (err)
		goto out;

	mddev->thread->timeout = mddev->bitmap_info.daemon_sleep;
	md_wakeup_thread(mddev->thread);

	bitmap_update_sb(bitmap);

	if (bitmap->flags & BITMAP_WRITE_ERROR)
		err = -EIO;
out:
	return err;
}
EXPORT_SYMBOL_GPL(bitmap_load);

void bitmap_status(struct seq_file *seq, struct bitmap *bitmap)
{
	unsigned long chunk_kb;
	unsigned long flags;

	if (!bitmap)
		return;

	spin_lock_irqsave(&bitmap->lock, flags);
	chunk_kb = bitmap->mddev->bitmap_info.chunksize >> 10;
	seq_printf(seq, "bitmap: %lu/%lu pages [%luKB], "
		   "%lu%s chunk",
		   bitmap->pages - bitmap->missing_pages,
		   bitmap->pages,
		   (bitmap->pages - bitmap->missing_pages)
		   << (PAGE_SHIFT - 10),
		   chunk_kb ? chunk_kb : bitmap->mddev->bitmap_info.chunksize,
		   chunk_kb ? "KB" : "B");
	if (bitmap->file) {
		seq_printf(seq, ", file: ");
		seq_path(seq, &bitmap->file->f_path, " \t\n");
	}

	seq_printf(seq, "\n");
	spin_unlock_irqrestore(&bitmap->lock, flags);
}

static ssize_t
location_show(struct mddev *mddev, char *page)
{
	ssize_t len;
	if (mddev->bitmap_info.file)
		len = sprintf(page, "file");
	else if (mddev->bitmap_info.offset)
		len = sprintf(page, "%+lld", (long long)mddev->bitmap_info.offset);
	else
		len = sprintf(page, "none");
	len += sprintf(page+len, "\n");
	return len;
}

static ssize_t
location_store(struct mddev *mddev, const char *buf, size_t len)
{

	if (mddev->pers) {
		if (!mddev->pers->quiesce)
			return -EBUSY;
		if (mddev->recovery || mddev->sync_thread)
			return -EBUSY;
	}

	if (mddev->bitmap || mddev->bitmap_info.file ||
	    mddev->bitmap_info.offset) {
		/*                                                        */
		if (strncmp(buf, "none", 4) != 0)
			return -EBUSY;
		if (mddev->pers) {
			mddev->pers->quiesce(mddev, 1);
			bitmap_destroy(mddev);
			mddev->pers->quiesce(mddev, 0);
		}
		mddev->bitmap_info.offset = 0;
		if (mddev->bitmap_info.file) {
			struct file *f = mddev->bitmap_info.file;
			mddev->bitmap_info.file = NULL;
			restore_bitmap_write_access(f);
			fput(f);
		}
	} else {
		/*                                 */
		long long offset;
		if (strncmp(buf, "none", 4) == 0)
			/*                    */;
		else if (strncmp(buf, "file:", 5) == 0) {
			/*                   */
			return -EINVAL;
		} else {
			int rv;
			if (buf[0] == '+')
				rv = strict_strtoll(buf+1, 10, &offset);
			else
				rv = strict_strtoll(buf, 10, &offset);
			if (rv)
				return rv;
			if (offset == 0)
				return -EINVAL;
			if (mddev->bitmap_info.external == 0 &&
			    mddev->major_version == 0 &&
			    offset != mddev->bitmap_info.default_offset)
				return -EINVAL;
			mddev->bitmap_info.offset = offset;
			if (mddev->pers) {
				mddev->pers->quiesce(mddev, 1);
				rv = bitmap_create(mddev);
				if (!rv)
					rv = bitmap_load(mddev);
				if (rv) {
					bitmap_destroy(mddev);
					mddev->bitmap_info.offset = 0;
				}
				mddev->pers->quiesce(mddev, 0);
				if (rv)
					return rv;
			}
		}
	}
	if (!mddev->external) {
		/*                                    
                       
   */
		set_bit(MD_CHANGE_DEVS, &mddev->flags);
		md_wakeup_thread(mddev->thread);
	}
	return len;
}

static struct md_sysfs_entry bitmap_location =
__ATTR(location, S_IRUGO|S_IWUSR, location_show, location_store);

static ssize_t
timeout_show(struct mddev *mddev, char *page)
{
	ssize_t len;
	unsigned long secs = mddev->bitmap_info.daemon_sleep / HZ;
	unsigned long jifs = mddev->bitmap_info.daemon_sleep % HZ;

	len = sprintf(page, "%lu", secs);
	if (jifs)
		len += sprintf(page+len, ".%03u", jiffies_to_msecs(jifs));
	len += sprintf(page+len, "\n");
	return len;
}

static ssize_t
timeout_store(struct mddev *mddev, const char *buf, size_t len)
{
	/*                                */
	unsigned long timeout;
	int rv = strict_strtoul_scaled(buf, &timeout, 4);
	if (rv)
		return rv;

	/*                                        */
	if (timeout >= LONG_MAX / HZ)
		return -EINVAL;

	timeout = timeout * HZ / 10000;

	if (timeout >= MAX_SCHEDULE_TIMEOUT)
		timeout = MAX_SCHEDULE_TIMEOUT-1;
	if (timeout < 1)
		timeout = 1;
	mddev->bitmap_info.daemon_sleep = timeout;
	if (mddev->thread) {
		/*                                                 
                                                 
                                 
   */
		if (mddev->thread->timeout < MAX_SCHEDULE_TIMEOUT) {
			mddev->thread->timeout = timeout;
			md_wakeup_thread(mddev->thread);
		}
	}
	return len;
}

static struct md_sysfs_entry bitmap_timeout =
__ATTR(time_base, S_IRUGO|S_IWUSR, timeout_show, timeout_store);

static ssize_t
backlog_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%lu\n", mddev->bitmap_info.max_write_behind);
}

static ssize_t
backlog_store(struct mddev *mddev, const char *buf, size_t len)
{
	unsigned long backlog;
	int rv = strict_strtoul(buf, 10, &backlog);
	if (rv)
		return rv;
	if (backlog > COUNTER_MAX)
		return -EINVAL;
	mddev->bitmap_info.max_write_behind = backlog;
	return len;
}

static struct md_sysfs_entry bitmap_backlog =
__ATTR(backlog, S_IRUGO|S_IWUSR, backlog_show, backlog_store);

static ssize_t
chunksize_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%lu\n", mddev->bitmap_info.chunksize);
}

static ssize_t
chunksize_store(struct mddev *mddev, const char *buf, size_t len)
{
	/*                                              */
	int rv;
	unsigned long csize;
	if (mddev->bitmap)
		return -EBUSY;
	rv = strict_strtoul(buf, 10, &csize);
	if (rv)
		return rv;
	if (csize < 512 ||
	    !is_power_of_2(csize))
		return -EINVAL;
	mddev->bitmap_info.chunksize = csize;
	return len;
}

static struct md_sysfs_entry bitmap_chunksize =
__ATTR(chunksize, S_IRUGO|S_IWUSR, chunksize_show, chunksize_store);

static ssize_t metadata_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%s\n", (mddev->bitmap_info.external
				      ? "external" : "internal"));
}

static ssize_t metadata_store(struct mddev *mddev, const char *buf, size_t len)
{
	if (mddev->bitmap ||
	    mddev->bitmap_info.file ||
	    mddev->bitmap_info.offset)
		return -EBUSY;
	if (strncmp(buf, "external", 8) == 0)
		mddev->bitmap_info.external = 1;
	else if (strncmp(buf, "internal", 8) == 0)
		mddev->bitmap_info.external = 0;
	else
		return -EINVAL;
	return len;
}

static struct md_sysfs_entry bitmap_metadata =
__ATTR(metadata, S_IRUGO|S_IWUSR, metadata_show, metadata_store);

static ssize_t can_clear_show(struct mddev *mddev, char *page)
{
	int len;
	if (mddev->bitmap)
		len = sprintf(page, "%s\n", (mddev->bitmap->need_sync ?
					     "false" : "true"));
	else
		len = sprintf(page, "\n");
	return len;
}

static ssize_t can_clear_store(struct mddev *mddev, const char *buf, size_t len)
{
	if (mddev->bitmap == NULL)
		return -ENOENT;
	if (strncmp(buf, "false", 5) == 0)
		mddev->bitmap->need_sync = 1;
	else if (strncmp(buf, "true", 4) == 0) {
		if (mddev->degraded)
			return -EBUSY;
		mddev->bitmap->need_sync = 0;
	} else
		return -EINVAL;
	return len;
}

static struct md_sysfs_entry bitmap_can_clear =
__ATTR(can_clear, S_IRUGO|S_IWUSR, can_clear_show, can_clear_store);

static ssize_t
behind_writes_used_show(struct mddev *mddev, char *page)
{
	if (mddev->bitmap == NULL)
		return sprintf(page, "0\n");
	return sprintf(page, "%lu\n",
		       mddev->bitmap->behind_writes_used);
}

static ssize_t
behind_writes_used_reset(struct mddev *mddev, const char *buf, size_t len)
{
	if (mddev->bitmap)
		mddev->bitmap->behind_writes_used = 0;
	return len;
}

static struct md_sysfs_entry max_backlog_used =
__ATTR(max_backlog_used, S_IRUGO | S_IWUSR,
       behind_writes_used_show, behind_writes_used_reset);

static struct attribute *md_bitmap_attrs[] = {
	&bitmap_location.attr,
	&bitmap_timeout.attr,
	&bitmap_backlog.attr,
	&bitmap_chunksize.attr,
	&bitmap_metadata.attr,
	&bitmap_can_clear.attr,
	&max_backlog_used.attr,
	NULL
};
struct attribute_group md_bitmap_group = {
	.name = "bitmap",
	.attrs = md_bitmap_attrs,
};

