#ifndef _H8300_PGTABLE_H
#define _H8300_PGTABLE_H

#include <asm-generic/4level-fixup.h>

#include <linux/slab.h>
#include <asm/processor.h>
#include <asm/page.h>
#include <asm/io.h>

#define pgd_present(pgd)     (1)       /*                                   */
#define pgd_none(pgd)		(0)
#define pgd_bad(pgd)		(0)
#define pgd_clear(pgdp)
#define kern_addr_valid(addr)	(1)
#define	pmd_offset(a, b)	((void *)0)
#define pmd_none(pmd)           (1)
#define pgd_offset_k(adrdress)  ((pgd_t *)0)
#define pte_offset_kernel(dir, address) ((pte_t *)0)

#define PAGE_NONE		__pgprot(0)    /*                             */
#define PAGE_SHARED		__pgprot(0)    /*                             */
#define PAGE_COPY		__pgprot(0)    /*                             */
#define PAGE_READONLY	__pgprot(0)    /*                             */
#define PAGE_KERNEL		__pgprot(0)    /*                             */

extern void paging_init(void);
#define swapper_pg_dir ((pgd_t *) 0)

#define __swp_type(x)		(0)
#define __swp_offset(x)		(0)
#define __swp_entry(typ,off)	((swp_entry_t) { ((typ) | ((off) << 7)) })
#define __pte_to_swp_entry(pte)	((swp_entry_t) { pte_val(pte) })
#define __swp_entry_to_pte(x)	((pte_t) { (x).val })

static inline int pte_file(pte_t pte) { return 0; }

/*
                                                              
                                     
 */
#define ZERO_PAGE(vaddr)	(virt_to_page(0))

/*
                                                                         
 */
extern unsigned int kobjsize(const void *objp);
extern int is_in_rom(unsigned long);

/*
                                     
 */
#define pgtable_cache_init()   do { } while (0)

#define io_remap_pfn_range(vma, vaddr, pfn, size, prot)		\
		remap_pfn_range(vma, vaddr, pfn, size, prot)

/*
                                                           
                                          
 */
#define	VMALLOC_START	0
#define	VMALLOC_END	0xffffffff

/*
                                                           
                                          
 */
#define	VMALLOC_START	0
#define	VMALLOC_END	0xffffffff

#define arch_enter_lazy_cpu_mode()    do {} while (0)
#endif /*                  */
