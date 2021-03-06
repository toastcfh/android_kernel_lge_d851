/* AM33v2 on-board bus controller registers
 *
 * Copyright (C) 2002 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#ifndef _ASM_BUSCTL_REGS_H
#define _ASM_BUSCTL_REGS_H

#include <asm/cpu-regs.h>

#ifdef __KERNEL__

/*                          */
#define BCCR			__SYSREG(0xc0002000, u32)	/*                            */
#define BCCR_B0AD		0x00000003	/*                                            */
#define BCCR_B1AD		0x0000000c	/*                                            */
#define BCCR_B2AD		0x00000030	/*                                            */
#define BCCR_B3AD		0x000000c0	/*                                            */
#define BCCR_B4AD		0x00000300	/*                                            */
#define BCCR_B5AD		0x00000c00	/*                                            */
#define BCCR_B6AD		0x00003000	/*                                            */
#define BCCR_B7AD		0x0000c000	/*                                            */
#define BCCR_BxAD_EXBUS		0x0		/*                                   */
#define BCCR_BxAD_OPEXBUS	0x1		/*                                   */
#define BCCR_BxAD_OCMBUS	0x2		/*                            */
#define BCCR_API		0x00070000	/*                          */
#define BCCR_API_DMACICD	0x00000000	/*                 */
#define BCCR_API_DMACDCI	0x00010000	/*                 */
#define BCCR_API_CICDDMA	0x00020000	/*                 */
#define BCCR_API_CDCIDMA	0x00030000	/*                 */
#define BCCR_API_ROUNDROBIN	0x00040000	/*               */
#define BCCR_BEPRI_DMACICD	0x00c00000	/*                            */
#define BCCR_BEPRI_DMACDCI	0x00000000	/*                 */
#define BCCR_BEPRI_CICDDMA	0x00400000	/*                 */
#define BCCR_BEPRI_CDCIDMA	0x00800000	/*                 */
#define BCCR_BEPRI		0x00c00000	/*                 */
#define BCCR_TMON		0x03000000	/*                        */
#define BCCR_TMON_16IOCLK	0x00000000	/*                   */
#define BCCR_TMON_256IOCLK	0x01000000	/*                    */
#define BCCR_TMON_4096IOCLK	0x02000000	/*                     */
#define BCCR_TMON_65536IOCLK	0x03000000	/*                      */
#define BCCR_TMOE		0x10000000	/*                          */

#define BCBERR			__SYSREG(0xc0002010, u32)	/*                      */
#define BCBERR_BESB		0x0000001f	/*                                    */
#define BCBERR_BESB_MON		0x00000001	/*                 */
#define BCBERR_BESB_IO		0x00000002	/*          */
#define BCBERR_BESB_EX		0x00000004	/*          */
#define BCBERR_BESB_OPEX	0x00000008	/*            */
#define BCBERR_BESB_OCM		0x00000010	/*                  */
#define BCBERR_BERW		0x00000100	/*                */
#define BCBERR_BERW_WRITE	0x00000000	/*         */
#define BCBERR_BERW_READ	0x00000100	/*        */
#define BCBERR_BESD		0x00000200	/*                */
#define BCBERR_BESD_BCU		0x00000000	/*                      */
#define BCBERR_BESD_SLAVE_BUS	0x00000200	/*                            */
#define BCBERR_BEBST		0x00000400	/*                */
#define BCBERR_BEBST_SINGLE	0x00000000	/*          */
#define BCBERR_BEBST_BURST	0x00000400	/*         */
#define BCBERR_BEME		0x00000800	/*                         */
#define BCBERR_BEMR		0x00007000	/*                                  */
#define BCBERR_BEMR_NOERROR	0x00000000	/*            */
#define BCBERR_BEMR_CI		0x00001000	/*                                          */
#define BCBERR_BEMR_CD		0x00002000	/*                             */
#define BCBERR_BEMR_DMA		0x00004000	/*                        */

#define BCBEAR			__SYSREGC(0xc0002020, u32)	/*                       */

/*                                 */
#define SBBASE(X)		__SYSREG(0xd8c00100 + (X) * 0x10, u32)	/*                    */
#define SBBASE_BE		0x00000001	/*             */
#define SBBASE_BAM		0x0000fffe	/*                           */
#define SBBASE_BBA		0xfffe0000	/*                           */

#define SBCNTRL0(X)		__SYSREG(0xd8c00200 + (X) * 0x10, u32)	/*                     */
#define SBCNTRL0_WEH		0x00000f00	/*                   */
#define SBCNTRL0_REH		0x0000f000	/*                  */
#define SBCNTRL0_RWH		0x000f0000	/*                 */
#define SBCNTRL0_CSH		0x00f00000	/*                  */
#define SBCNTRL0_DAH		0x0f000000	/*           */
#define SBCNTRL0_ADH		0xf0000000	/*              */

#define SBCNTRL1(X)		__SYSREG(0xd8c00204 + (X) * 0x10, u32)	/*                     */
#define SBCNTRL1_WED		0x00000f00	/*                    */
#define SBCNTRL1_RED		0x0000f000	/*                   */
#define SBCNTRL1_RWD		0x000f0000	/*                  */
#define SBCNTRL1_ASW		0x00f00000	/*                      */
#define SBCNTRL1_CSD		0x0f000000	/*                   */
#define SBCNTRL1_ASD		0xf0000000	/*                      */

#define SBCNTRL2(X)		__SYSREG(0xd8c00208 + (X) * 0x10, u32)	/*                     */
#define SBCNTRL2_WC		0x000000ff	/*            */
#define SBCNTRL2_BWC		0x00000f00	/*                  */
#define SBCNTRL2_WM		0x01000000	/*                   */
#define SBCNTRL2_WM_FIXEDWAIT	0x00000000	/*                     */
#define SBCNTRL2_WM_HANDSHAKE	0x01000000	/*                    */
#define SBCNTRL2_BM		0x02000000	/*                          */
#define SBCNTRL2_BM_SYNC	0x00000000	/*                    */
#define SBCNTRL2_BM_ASYNC	0x02000000	/*                     */
#define SBCNTRL2_BW		0x04000000	/*           */
#define SBCNTRL2_BW_32		0x00000000	/*           */
#define SBCNTRL2_BW_16		0x04000000	/*           */
#define SBCNTRL2_RWINV		0x08000000	/*                            */
#define SBCNTRL2_RWINV_NORM	0x00000000	/*                      */
#define SBCNTRL2_RWINV_INV	0x08000000	/*                       */
#define SBCNTRL2_BT		0x70000000	/*                  */
#define SBCNTRL2_BT_SRAM	0x00000000	/*                  */
#define SBCNTRL2_BT_ADMUX	0x00000000	/*                                   */
#define SBCNTRL2_BT_BROM	0x00000000	/*                       */
#define SBCNTRL2_BTSE		0x80000000	/*              */

/*                       */
#define SDBASE(X)		__SYSREG(0xda000008 + (X) * 0x4, u32)	/*                    */
#define SDBASE_CE		0x00000001	/*             */
#define SDBASE_CBAM		0x0000fff0	/*                                */
#define SDBASE_CBAM_SHIFT	16
#define SDBASE_CBA		0xfff00000	/*                           */

#define SDRAMBUS		__SYSREG(0xda000000, u32)	/*                      */
#define SDRAMBUS_REFEN		0x00000004	/*                */
#define SDRAMBUS_TRC		0x00000018	/*                            */
#define SDRAMBUS_BSTPT		0x00000020	/*                           */
#define SDRAMBUS_PONSEQ		0x00000040	/*                   */
#define SDRAMBUS_SELFREQ	0x00000080	/*                           */
#define SDRAMBUS_SELFON		0x00000100	/*                      */
#define SDRAMBUS_SIZE		0x00030000	/*            */
#define SDRAMBUS_SIZE_64Mbit	0x00010000	/*                    */
#define SDRAMBUS_SIZE_128Mbit	0x00020000	/*                     */
#define SDRAMBUS_SIZE_256Mbit	0x00030000	/*                     */
#define SDRAMBUS_TRASWAIT	0x000c0000	/*                                            */
#define SDRAMBUS_REFNUM		0x00300000	/*                        */
#define SDRAMBUS_BSTWAIT	0x00c00000	/*                          */
#define SDRAMBUS_SETWAIT	0x03000000	/*                                     */
#define SDRAMBUS_PREWAIT	0x0c000000	/*                         */
#define SDRAMBUS_RASLATE	0x30000000	/*             */
#define SDRAMBUS_CASLATE	0xc0000000	/*             */

#define SDREFCNT		__SYSREG(0xda000004, u32)	/*                    */
#define SDREFCNT_PERI		0x00000fff	/*                */

#define SDSHDW			__SYSREG(0xda000010, u32)	/*          */

#endif /*            */

#endif /*                    */
