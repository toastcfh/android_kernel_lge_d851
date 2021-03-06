/*
 * Compressed RAM block device
 *
 * Copyright (C) 2008, 2009, 2010  Nitin Gupta
 *
 * This code is released using a dual license strategy: BSD/GPL
 * You can choose the licence that better fits your requirements.
 *
 * Released under the terms of 3-clause BSD License
 * Released under the terms of GNU General Public License Version 2.0
 *
 * Project home: http://compcache.googlecode.com
 */

#ifndef _ZRAM_DRV_H_
#define _ZRAM_DRV_H_

#include <linux/spinlock.h>
#include <linux/mutex.h>

#include "../zsmalloc/zsmalloc.h"

/*
                                              
                                                  
 */
static const unsigned max_num_devices = 32;

/*                           */

/*
                                                           
                          
 */
static const size_t max_zpage_size = PAGE_SIZE / 10 * 9;

/*
                                                      
                                                    
                         
 */

/*                              */

#define SECTOR_SHIFT		9
#define SECTOR_SIZE		(1 << SECTOR_SHIFT)
#define SECTORS_PER_PAGE_SHIFT	(PAGE_SHIFT - SECTOR_SHIFT)
#define SECTORS_PER_PAGE	(1 << SECTORS_PER_PAGE_SHIFT)
#define ZRAM_LOGICAL_BLOCK_SHIFT 12
#define ZRAM_LOGICAL_BLOCK_SIZE	(1 << ZRAM_LOGICAL_BLOCK_SHIFT)
#define ZRAM_SECTOR_PER_LOGICAL_BLOCK	\
	(1 << (ZRAM_LOGICAL_BLOCK_SHIFT - SECTOR_SHIFT))

/*                                             */
enum zram_pageflags {
	/*                                 */
	ZRAM_ZERO,

	__NR_ZRAM_PAGEFLAGS,
};

/*                   */

/*                              */
struct table {
	unsigned long handle;
	u16 size;	/*                                */
	u8 count;	/*                                 */
	u8 flags;
} __aligned(4);

/*
                                                                         
                                                                        
 */
struct zram_stats {
	atomic64_t compr_size;	/*                                 */
	atomic64_t num_reads;	/*                     */
	atomic64_t num_writes;	/*        */
	atomic64_t failed_reads;	/*                      */
	atomic64_t failed_writes;	/*                                   */
	atomic64_t invalid_io;	/*                               */
	atomic64_t notify_free;	/*                                     */
	u32 pages_zero;		/*                          */
	u32 pages_stored;	/*                               */
	u32 good_compress;	/*                                        */
	u32 bad_compress;	/*                                        */
};

struct zram_meta {
	void *compress_workmem;
	void *compress_buffer;
	struct table *table;
	struct zs_pool *mem_pool;
};

struct zram_slot_free {
	unsigned long index;
	struct zram_slot_free *next;
};

struct zram {
	struct zram_meta *meta;
	struct rw_semaphore lock; /*                                    
                                               
                                         */

	struct work_struct free_work;  /*                             */
	struct zram_slot_free *slot_free_rq; /*                           */

	struct request_queue *queue;
	struct gendisk *disk;
	int init_done;
	/*                                                                    */
	struct rw_semaphore init_lock;
	/*
                                                               
                           
  */
	u64 disksize;	/*       */
	spinlock_t slot_free_lock;

	struct zram_stats stats;
#ifdef CONFIG_LZ4_COMPRESS
  /*                                                              */
	bool lz4;
#endif
};
#endif
