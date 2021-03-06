/*
 * Network Checksum & Copy routine
 *
 * Copyright (C) 1999, 2003-2004 Hewlett-Packard Co
 *	Stephane Eranian <eranian@hpl.hp.com>
 *
 * Most of the code has been imported from Linux/Alpha
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/string.h>

#include <asm/uaccess.h>

/*
                                                                      
 */
static inline unsigned
short from64to16(unsigned long x)
{
	/*                                 */
	x = (x & 0xffffffff) + (x >> 32);
	/*                                              */
	x = (x & 0xffff) + (x >> 16);
	/*                                      */
	x = (x & 0xffff) + (x >> 16);
	/*                */
	x = (x & 0xffff) + (x >> 16);
	return x;
}

static inline
unsigned long do_csum_c(const unsigned char * buff, int len, unsigned int psum)
{
	int odd, count;
	unsigned long result = (unsigned long)psum;

	if (len <= 0)
		goto out;
	odd = 1 & (unsigned long) buff;
	if (odd) {
		result = *buff << 8;
		len--;
		buff++;
	}
	count = len >> 1;		/*                      */
	if (count) {
		if (2 & (unsigned long) buff) {
			result += *(unsigned short *) buff;
			count--;
			len -= 2;
			buff += 2;
		}
		count >>= 1;		/*                      */
		if (count) {
			if (4 & (unsigned long) buff) {
				result += *(unsigned int *) buff;
				count--;
				len -= 4;
				buff += 4;
			}
			count >>= 1;	/*                      */
			if (count) {
				unsigned long carry = 0;
				do {
					unsigned long w = *(unsigned long *) buff;
					count--;
					buff += 8;
					result += carry;
					result += w;
					carry = (w > result);
				} while (count);
				result += carry;
				result = (result & 0xffffffff) + (result >> 32);
			}
			if (len & 4) {
				result += *(unsigned int *) buff;
				buff += 4;
			}
		}
		if (len & 2) {
			result += *(unsigned short *) buff;
			buff += 2;
		}
	}
	if (len & 1)
		result += *buff;

	result = from64to16(result);

	if (odd)
		result = ((result >> 8) & 0xff) | ((result & 0xff) << 8);

out:
	return result;
}

/*
            
  
                                                                    
                                               
 */
extern unsigned long do_csum(const unsigned char *, long);

__wsum
csum_partial_copy_from_user(const void __user *src, void *dst,
				int len, __wsum psum, int *errp)
{
	unsigned long result;

	/*          
                                                          
                                                                
           
  */

	if (__copy_from_user(dst, src, len) != 0 && errp)
		*errp = -EFAULT;

	result = do_csum(dst, len);

	/*                             */
	result += (__force u32)psum;
	/*                      */
	result = (result & 0xffffffff) + (result >> 32);
	return (__force __wsum)result;
}

EXPORT_SYMBOL(csum_partial_copy_from_user);

__wsum
csum_partial_copy_nocheck(const void *src, void *dst, int len, __wsum sum)
{
	return csum_partial_copy_from_user((__force const void __user *)src,
					   dst, len, sum, NULL);
}

EXPORT_SYMBOL(csum_partial_copy_nocheck);
