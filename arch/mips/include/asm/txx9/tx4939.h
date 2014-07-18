/*
 * Definitions for TX4939
 *
 * Copyright (C) 2000-2001,2005-2006 Toshiba Corporation
 * 2003-2005 (c) MontaVista Software, Inc. This file is licensed under the
 * terms of the GNU General Public License version 2. This program is
 * licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */
#ifndef __ASM_TXX9_TX4939_H
#define __ASM_TXX9_TX4939_H

/*                                                */
#include <asm/txx9/tx4938.h>

#ifdef CONFIG_64BIT
#define TX4939_REG_BASE	0xffffffffff1f0000UL /*                    */
#else
#define TX4939_REG_BASE	0xff1f0000UL /*                    */
#endif
#define TX4939_REG_SIZE	0x00010000 /*                    */

#define TX4939_ATA_REG(ch)	(TX4939_REG_BASE + 0x3000 + (ch) * 0x1000)
#define TX4939_NDFMC_REG	(TX4939_REG_BASE + 0x5000)
#define TX4939_SRAMC_REG	(TX4939_REG_BASE + 0x6000)
#define TX4939_CRYPTO_REG	(TX4939_REG_BASE + 0x6800)
#define TX4939_PCIC1_REG	(TX4939_REG_BASE + 0x7000)
#define TX4939_DDRC_REG		(TX4939_REG_BASE + 0x8000)
#define TX4939_EBUSC_REG	(TX4939_REG_BASE + 0x9000)
#define TX4939_VPC_REG		(TX4939_REG_BASE + 0xa000)
#define TX4939_DMA_REG(ch)	(TX4939_REG_BASE + 0xb000 + (ch) * 0x800)
#define TX4939_PCIC_REG		(TX4939_REG_BASE + 0xd000)
#define TX4939_CCFG_REG		(TX4939_REG_BASE + 0xe000)
#define TX4939_IRC_REG		(TX4939_REG_BASE + 0xe800)
#define TX4939_NR_TMR	6	/*                                           */
#define TX4939_TMR_REG(ch)	\
	(TX4939_REG_BASE + 0xf000 + ((ch) + ((ch) >= 3) * 10) * 0x100)
#define TX4939_NR_SIO	4	/*                                */
#define TX4939_SIO_REG(ch)	\
	(TX4939_REG_BASE + 0xf300 + (((ch) & 1) << 8) + (((ch) & 2) << 6))
#define TX4939_ACLC_REG		(TX4939_REG_BASE + 0xf700)
#define TX4939_SPI_REG		(TX4939_REG_BASE + 0xf800)
#define TX4939_I2C_REG		(TX4939_REG_BASE + 0xf900)
#define TX4939_I2S_REG		(TX4939_REG_BASE + 0xfa00)
#define TX4939_RTC_REG		(TX4939_REG_BASE + 0xfb00)
#define TX4939_CIR_REG		(TX4939_REG_BASE + 0xfc00)

#define TX4939_RNG_REG		(TX4939_CRYPTO_REG + 0xb0)

struct tx4939_le_reg {
	__u32 r;
	__u32 unused;
};

struct tx4939_ddrc_reg {
	struct tx4939_le_reg ctl[47];
	__u64 unused0[17];
	__u64 winen;
	__u64 win[4];
};

struct tx4939_ccfg_reg {
	__u64 ccfg;
	__u64 crir;
	__u64 pcfg;
	__u64 toea;
	__u64 clkctr;
	__u64 unused0;
	__u64 garbc;
	__u64 unused1[2];
	__u64 ramp;
	__u64 unused2[2];
	__u64 dskwctrl;
	__u64 mclkosc;
	__u64 mclkctl;
	__u64 unused3[17];
	struct {
		__u64 mr;
		__u64 dr;
	} gpio[2];
};

struct tx4939_irc_reg {
	struct tx4939_le_reg den;
	struct tx4939_le_reg scipb;
	struct tx4939_le_reg dm[2];
	struct tx4939_le_reg lvl[16];
	struct tx4939_le_reg msk;
	struct tx4939_le_reg edc;
	struct tx4939_le_reg pnd0;
	struct tx4939_le_reg cs;
	struct tx4939_le_reg pnd1;
	struct tx4939_le_reg dm2[2];
	struct tx4939_le_reg dbr[2];
	struct tx4939_le_reg dben;
	struct tx4939_le_reg unused0[2];
	struct tx4939_le_reg flag[2];
	struct tx4939_le_reg pol;
	struct tx4939_le_reg cnt;
	struct tx4939_le_reg maskint;
	struct tx4939_le_reg maskext;
};

struct tx4939_rtc_reg {
	__u32 ctl;
	__u32 adr;
	__u32 dat;
	__u32 tbc;
};

struct tx4939_crypto_reg {
	struct tx4939_le_reg csr;
	struct tx4939_le_reg idesptr;
	struct tx4939_le_reg cdesptr;
	struct tx4939_le_reg buserr;
	struct tx4939_le_reg cip_tout;
	struct tx4939_le_reg cir;
	union {
		struct {
			struct tx4939_le_reg data[8];
			struct tx4939_le_reg ctrl;
		} gen;
		struct {
			struct {
				struct tx4939_le_reg l;
				struct tx4939_le_reg u;
			} key[3], ini;
			struct tx4939_le_reg ctrl;
		} des;
		struct {
			struct tx4939_le_reg key[4];
			struct tx4939_le_reg ini[4];
			struct tx4939_le_reg ctrl;
		} aes;
		struct {
			struct {
				struct tx4939_le_reg l;
				struct tx4939_le_reg u;
			} cnt;
			struct tx4939_le_reg ini[5];
			struct tx4939_le_reg unused;
			struct tx4939_le_reg ctrl;
		} hash;
	} cdr;
	struct tx4939_le_reg unused0[7];
	struct tx4939_le_reg rcsr;
	struct tx4939_le_reg rpr;
	__u64 rdr;
	__u64 ror[3];
	struct tx4939_le_reg unused1[2];
	struct tx4939_le_reg xorslr;
	struct tx4939_le_reg xorsur;
};

struct tx4939_crypto_desc {
	__u32 src;
	__u32 dst;
	__u32 next;
	__u32 ctrl;
	__u32 index;
	__u32 xor;
};

struct tx4939_vpc_reg {
	struct tx4939_le_reg csr;
	struct {
		struct tx4939_le_reg ctrlA;
		struct tx4939_le_reg ctrlB;
		struct tx4939_le_reg idesptr;
		struct tx4939_le_reg cdesptr;
	} port[3];
	struct tx4939_le_reg buserr;
};

struct tx4939_vpc_desc {
	__u32 src;
	__u32 next;
	__u32 ctrl1;
	__u32 ctrl2;
};

/*
      
 */
#define TX4939_IR_NONE	0
#define TX4939_IR_DDR	1
#define TX4939_IR_WTOERR	2
#define TX4939_NUM_IR_INT	3
#define TX4939_IR_INT(n)	(3 + (n))
#define TX4939_NUM_IR_ETH	2
#define TX4939_IR_ETH(n)	((n) ? 43 : 6)
#define TX4939_IR_VIDEO	7
#define TX4939_IR_CIR	8
#define TX4939_NUM_IR_SIO	4
#define TX4939_IR_SIO(n)	((n) ? 43 + (n) : 9)	/*         */
#define TX4939_NUM_IR_DMA	4
#define TX4939_IR_DMA(ch, n)	(((ch) ? 22 : 10) + (n)) /*             */
#define TX4939_IR_IRC	14
#define TX4939_IR_PDMAC	15
#define TX4939_NUM_IR_TMR	6
#define TX4939_IR_TMR(n)	(((n) >= 3 ? 45 : 16) + (n)) /*             */
#define TX4939_NUM_IR_ATA	2
#define TX4939_IR_ATA(n)	(19 + (n))
#define TX4939_IR_ACLC	21
#define TX4939_IR_CIPHER	26
#define TX4939_IR_INTA	27
#define TX4939_IR_INTB	28
#define TX4939_IR_INTC	29
#define TX4939_IR_INTD	30
#define TX4939_IR_I2C	33
#define TX4939_IR_SPI	34
#define TX4939_IR_PCIC	35
#define TX4939_IR_PCIC1	36
#define TX4939_IR_PCIERR	37
#define TX4939_IR_PCIPME	38
#define TX4939_IR_NDFMC	39
#define TX4939_IR_ACLCPME	40
#define TX4939_IR_RTC	41
#define TX4939_IR_RND	42
#define TX4939_IR_I2S	47
#define TX4939_NUM_IR	64

#define TX4939_IRC_INT	2	/*                          */

/*
       
 */
/*                           */
#define TX4939_CCFG_PCIBOOT	0x0000040000000000ULL
#define TX4939_CCFG_WDRST	0x0000020000000000ULL
#define TX4939_CCFG_WDREXEN	0x0000010000000000ULL
#define TX4939_CCFG_BCFG_MASK	0x000000ff00000000ULL
#define TX4939_CCFG_GTOT_MASK	0x06000000
#define TX4939_CCFG_GTOT_4096	0x06000000
#define TX4939_CCFG_GTOT_2048	0x04000000
#define TX4939_CCFG_GTOT_1024	0x02000000
#define TX4939_CCFG_GTOT_512	0x00000000
#define TX4939_CCFG_TINTDIS	0x01000000
#define TX4939_CCFG_PCI66	0x00800000
#define TX4939_CCFG_PCIMODE	0x00400000
#define TX4939_CCFG_SSCG	0x00100000
#define TX4939_CCFG_MULCLK_MASK	0x000e0000
#define TX4939_CCFG_MULCLK_8	(0x7 << 17)
#define TX4939_CCFG_MULCLK_9	(0x0 << 17)
#define TX4939_CCFG_MULCLK_10	(0x1 << 17)
#define TX4939_CCFG_MULCLK_11	(0x2 << 17)
#define TX4939_CCFG_MULCLK_12	(0x3 << 17)
#define TX4939_CCFG_MULCLK_13	(0x4 << 17)
#define TX4939_CCFG_MULCLK_14	(0x5 << 17)
#define TX4939_CCFG_MULCLK_15	(0x6 << 17)
#define TX4939_CCFG_BEOW	0x00010000
#define TX4939_CCFG_WR	0x00008000
#define TX4939_CCFG_TOE	0x00004000
#define TX4939_CCFG_PCIARB	0x00002000
#define TX4939_CCFG_YDIVMODE_MASK	0x00001c00
#define TX4939_CCFG_YDIVMODE_2	(0x0 << 10)
#define TX4939_CCFG_YDIVMODE_3	(0x1 << 10)
#define TX4939_CCFG_YDIVMODE_5	(0x6 << 10)
#define TX4939_CCFG_YDIVMODE_6	(0x7 << 10)
#define TX4939_CCFG_PTSEL	0x00000200
#define TX4939_CCFG_BESEL	0x00000100
#define TX4939_CCFG_SYSSP_MASK	0x000000c0
#define TX4939_CCFG_ACKSEL	0x00000020
#define TX4939_CCFG_ROMW	0x00000010
#define TX4939_CCFG_ENDIAN	0x00000004
#define TX4939_CCFG_ARMODE	0x00000002
#define TX4939_CCFG_ACEHOLD	0x00000001

/*                          */
#define TX4939_PCFG_SIO2MODE_MASK	0xc000000000000000ULL
#define TX4939_PCFG_SIO2MODE_GPIO	0x8000000000000000ULL
#define TX4939_PCFG_SIO2MODE_SIO2	0x4000000000000000ULL
#define TX4939_PCFG_SIO2MODE_SIO0	0x0000000000000000ULL
#define TX4939_PCFG_SPIMODE	0x2000000000000000ULL
#define TX4939_PCFG_I2CMODE	0x1000000000000000ULL
#define TX4939_PCFG_I2SMODE_MASK	0x0c00000000000000ULL
#define TX4939_PCFG_I2SMODE_GPIO	0x0c00000000000000ULL
#define TX4939_PCFG_I2SMODE_I2S	0x0800000000000000ULL
#define TX4939_PCFG_I2SMODE_I2S_ALT	0x0400000000000000ULL
#define TX4939_PCFG_I2SMODE_ACLC	0x0000000000000000ULL
#define TX4939_PCFG_SIO3MODE	0x0200000000000000ULL
#define TX4939_PCFG_DMASEL3	0x0004000000000000ULL
#define TX4939_PCFG_DMASEL3_SIO0	0x0004000000000000ULL
#define TX4939_PCFG_DMASEL3_NDFC	0x0000000000000000ULL
#define TX4939_PCFG_VSSMODE	0x0000200000000000ULL
#define TX4939_PCFG_VPSMODE	0x0000100000000000ULL
#define TX4939_PCFG_ET1MODE	0x0000080000000000ULL
#define TX4939_PCFG_ET0MODE	0x0000040000000000ULL
#define TX4939_PCFG_ATA1MODE	0x0000020000000000ULL
#define TX4939_PCFG_ATA0MODE	0x0000010000000000ULL
#define TX4939_PCFG_BP_PLL	0x0000000100000000ULL

#define TX4939_PCFG_SYSCLKEN	0x08000000
#define TX4939_PCFG_PCICLKEN_ALL	0x000f0000
#define TX4939_PCFG_PCICLKEN(ch)	(0x00010000<<(ch))
#define TX4939_PCFG_SPEED1	0x00002000
#define TX4939_PCFG_SPEED0	0x00001000
#define TX4939_PCFG_ITMODE	0x00000300
#define TX4939_PCFG_DMASEL_ALL	(0x00000007 | TX4939_PCFG_DMASEL3)
#define TX4939_PCFG_DMASEL2	0x00000004
#define TX4939_PCFG_DMASEL2_DRQ2	0x00000000
#define TX4939_PCFG_DMASEL2_SIO0	0x00000004
#define TX4939_PCFG_DMASEL1	0x00000002
#define TX4939_PCFG_DMASEL1_DRQ1	0x00000000
#define TX4939_PCFG_DMASEL0	0x00000001
#define TX4939_PCFG_DMASEL0_DRQ0	0x00000000

/*                        */
#define TX4939_CLKCTR_IOSCKD	0x8000000000000000ULL
#define TX4939_CLKCTR_SYSCKD	0x4000000000000000ULL
#define TX4939_CLKCTR_TM5CKD	0x2000000000000000ULL
#define TX4939_CLKCTR_TM4CKD	0x1000000000000000ULL
#define TX4939_CLKCTR_TM3CKD	0x0800000000000000ULL
#define TX4939_CLKCTR_CIRCKD	0x0400000000000000ULL
#define TX4939_CLKCTR_SIO3CKD	0x0200000000000000ULL
#define TX4939_CLKCTR_SIO2CKD	0x0100000000000000ULL
#define TX4939_CLKCTR_SIO1CKD	0x0080000000000000ULL
#define TX4939_CLKCTR_VPCCKD	0x0040000000000000ULL
#define TX4939_CLKCTR_EPCICKD	0x0020000000000000ULL
#define TX4939_CLKCTR_ETH1CKD	0x0008000000000000ULL
#define TX4939_CLKCTR_ATA1CKD	0x0004000000000000ULL
#define TX4939_CLKCTR_BROMCKD	0x0002000000000000ULL
#define TX4939_CLKCTR_NDCCKD	0x0001000000000000ULL
#define TX4939_CLKCTR_I2CCKD	0x0000800000000000ULL
#define TX4939_CLKCTR_ETH0CKD	0x0000400000000000ULL
#define TX4939_CLKCTR_SPICKD	0x0000200000000000ULL
#define TX4939_CLKCTR_SRAMCKD	0x0000100000000000ULL
#define TX4939_CLKCTR_PCI1CKD	0x0000080000000000ULL
#define TX4939_CLKCTR_DMA1CKD	0x0000040000000000ULL
#define TX4939_CLKCTR_ACLCKD	0x0000020000000000ULL
#define TX4939_CLKCTR_ATA0CKD	0x0000010000000000ULL
#define TX4939_CLKCTR_DMA0CKD	0x0000008000000000ULL
#define TX4939_CLKCTR_PCICCKD	0x0000004000000000ULL
#define TX4939_CLKCTR_I2SCKD	0x0000002000000000ULL
#define TX4939_CLKCTR_TM0CKD	0x0000001000000000ULL
#define TX4939_CLKCTR_TM1CKD	0x0000000800000000ULL
#define TX4939_CLKCTR_TM2CKD	0x0000000400000000ULL
#define TX4939_CLKCTR_SIO0CKD	0x0000000200000000ULL
#define TX4939_CLKCTR_CYPCKD	0x0000000100000000ULL
#define TX4939_CLKCTR_IOSRST	0x80000000
#define TX4939_CLKCTR_SYSRST	0x40000000
#define TX4939_CLKCTR_TM5RST	0x20000000
#define TX4939_CLKCTR_TM4RST	0x10000000
#define TX4939_CLKCTR_TM3RST	0x08000000
#define TX4939_CLKCTR_CIRRST	0x04000000
#define TX4939_CLKCTR_SIO3RST	0x02000000
#define TX4939_CLKCTR_SIO2RST	0x01000000
#define TX4939_CLKCTR_SIO1RST	0x00800000
#define TX4939_CLKCTR_VPCRST	0x00400000
#define TX4939_CLKCTR_EPCIRST	0x00200000
#define TX4939_CLKCTR_ETH1RST	0x00080000
#define TX4939_CLKCTR_ATA1RST	0x00040000
#define TX4939_CLKCTR_BROMRST	0x00020000
#define TX4939_CLKCTR_NDCRST	0x00010000
#define TX4939_CLKCTR_I2CRST	0x00008000
#define TX4939_CLKCTR_ETH0RST	0x00004000
#define TX4939_CLKCTR_SPIRST	0x00002000
#define TX4939_CLKCTR_SRAMRST	0x00001000
#define TX4939_CLKCTR_PCI1RST	0x00000800
#define TX4939_CLKCTR_DMA1RST	0x00000400
#define TX4939_CLKCTR_ACLRST	0x00000200
#define TX4939_CLKCTR_ATA0RST	0x00000100
#define TX4939_CLKCTR_DMA0RST	0x00000080
#define TX4939_CLKCTR_PCICRST	0x00000040
#define TX4939_CLKCTR_I2SRST	0x00000020
#define TX4939_CLKCTR_TM0RST	0x00000010
#define TX4939_CLKCTR_TM1RST	0x00000008
#define TX4939_CLKCTR_TM2RST	0x00000004
#define TX4939_CLKCTR_SIO0RST	0x00000002
#define TX4939_CLKCTR_CYPRST	0x00000001

/*
      
 */
#define TX4939_RTCCTL_ALME	0x00000080
#define TX4939_RTCCTL_ALMD	0x00000040
#define TX4939_RTCCTL_BUSY	0x00000020

#define TX4939_RTCCTL_COMMAND	0x00000007
#define TX4939_RTCCTL_COMMAND_NOP	0x00000000
#define TX4939_RTCCTL_COMMAND_GETTIME	0x00000001
#define TX4939_RTCCTL_COMMAND_SETTIME	0x00000002
#define TX4939_RTCCTL_COMMAND_GETALARM	0x00000003
#define TX4939_RTCCTL_COMMAND_SETALARM	0x00000004

#define TX4939_RTCTBC_PM	0x00000080
#define TX4939_RTCTBC_COMP	0x0000007f

#define TX4939_RTC_REG_RAMSIZE	0x00000100
#define TX4939_RTC_REG_RWBSIZE	0x00000006

/*
         
 */
#define TX4939_CRYPTO_CSR_SAESO	0x08000000
#define TX4939_CRYPTO_CSR_SAESI	0x04000000
#define TX4939_CRYPTO_CSR_SDESO	0x02000000
#define TX4939_CRYPTO_CSR_SDESI	0x01000000
#define TX4939_CRYPTO_CSR_INDXBST_MASK	0x00700000
#define TX4939_CRYPTO_CSR_INDXBST(n)	((n) << 20)
#define TX4939_CRYPTO_CSR_TOINT	0x00080000
#define TX4939_CRYPTO_CSR_DCINT	0x00040000
#define TX4939_CRYPTO_CSR_GBINT	0x00010000
#define TX4939_CRYPTO_CSR_INDXAST_MASK	0x0000e000
#define TX4939_CRYPTO_CSR_INDXAST(n)	((n) << 13)
#define TX4939_CRYPTO_CSR_CSWAP_MASK	0x00001800
#define TX4939_CRYPTO_CSR_CSWAP_NONE	0x00000000
#define TX4939_CRYPTO_CSR_CSWAP_IN	0x00000800
#define TX4939_CRYPTO_CSR_CSWAP_OUT	0x00001000
#define TX4939_CRYPTO_CSR_CSWAP_BOTH	0x00001800
#define TX4939_CRYPTO_CSR_CDIV_MASK	0x00000600
#define TX4939_CRYPTO_CSR_CDIV_DIV2	0x00000000
#define TX4939_CRYPTO_CSR_CDIV_DIV1	0x00000200
#define TX4939_CRYPTO_CSR_CDIV_DIV2ALT	0x00000400
#define TX4939_CRYPTO_CSR_CDIV_DIV1ALT	0x00000600
#define TX4939_CRYPTO_CSR_PDINT_MASK	0x000000c0
#define TX4939_CRYPTO_CSR_PDINT_ALL	0x00000000
#define TX4939_CRYPTO_CSR_PDINT_END	0x00000040
#define TX4939_CRYPTO_CSR_PDINT_NEXT	0x00000080
#define TX4939_CRYPTO_CSR_PDINT_NONE	0x000000c0
#define TX4939_CRYPTO_CSR_GINTE	0x00000008
#define TX4939_CRYPTO_CSR_RSTD	0x00000004
#define TX4939_CRYPTO_CSR_RSTC	0x00000002
#define TX4939_CRYPTO_CSR_ENCR	0x00000001

/*                                         */
#define TX4939_CRYPTO_CTX_ENGINE_MASK	0x00000003
#define TX4939_CRYPTO_CTX_ENGINE_DES	0x00000000
#define TX4939_CRYPTO_CTX_ENGINE_AES	0x00000001
#define TX4939_CRYPTO_CTX_ENGINE_MD5	0x00000002
#define TX4939_CRYPTO_CTX_ENGINE_SHA1	0x00000003
#define TX4939_CRYPTO_CTX_TDMS	0x00000010
#define TX4939_CRYPTO_CTX_CMS	0x00000020
#define TX4939_CRYPTO_CTX_DMS	0x00000040
#define TX4939_CRYPTO_CTX_UPDATE	0x00000080

/*                                  */
#define TX4939_CRYPTO_DESC_OB_CNT_MASK	0xffe00000
#define TX4939_CRYPTO_DESC_OB_CNT(cnt)	((cnt) << 21)
#define TX4939_CRYPTO_DESC_IB_CNT_MASK	0x001ffc00
#define TX4939_CRYPTO_DESC_IB_CNT(cnt)	((cnt) << 10)
#define TX4939_CRYPTO_DESC_START	0x00000200
#define TX4939_CRYPTO_DESC_END	0x00000100
#define TX4939_CRYPTO_DESC_XOR	0x00000010
#define TX4939_CRYPTO_DESC_LAST	0x00000008
#define TX4939_CRYPTO_DESC_ERR_MASK	0x00000006
#define TX4939_CRYPTO_DESC_ERR_NONE	0x00000000
#define TX4939_CRYPTO_DESC_ERR_TOUT	0x00000002
#define TX4939_CRYPTO_DESC_ERR_DIGEST	0x00000004
#define TX4939_CRYPTO_DESC_OWN	0x00000001

/*                                   */
#define TX4939_CRYPTO_DESC_HASH_IDX_MASK	0x00000070
#define TX4939_CRYPTO_DESC_HASH_IDX(idx)	((idx) << 4)
#define TX4939_CRYPTO_DESC_ENCRYPT_IDX_MASK	0x00000007
#define TX4939_CRYPTO_DESC_ENCRYPT_IDX(idx)	((idx) << 0)

#define TX4939_CRYPTO_NR_SET	6

#define TX4939_CRYPTO_RCSR_INTE	0x00000008
#define TX4939_CRYPTO_RCSR_RST	0x00000004
#define TX4939_CRYPTO_RCSR_FIN	0x00000002
#define TX4939_CRYPTO_RCSR_ST	0x00000001

/*
      
 */
#define TX4939_VPC_CSR_GBINT	0x00010000
#define TX4939_VPC_CSR_SWAPO	0x00000020
#define TX4939_VPC_CSR_SWAPI	0x00000010
#define TX4939_VPC_CSR_GINTE	0x00000008
#define TX4939_VPC_CSR_RSTD	0x00000004
#define TX4939_VPC_CSR_RSTVPC	0x00000002

#define TX4939_VPC_CTRLA_VDPSN	0x00000200
#define TX4939_VPC_CTRLA_PBUSY	0x00000100
#define TX4939_VPC_CTRLA_DCINT	0x00000080
#define TX4939_VPC_CTRLA_UOINT	0x00000040
#define TX4939_VPC_CTRLA_PDINT_MASK	0x00000030
#define TX4939_VPC_CTRLA_PDINT_ALL	0x00000000
#define TX4939_VPC_CTRLA_PDINT_NEXT	0x00000010
#define TX4939_VPC_CTRLA_PDINT_NONE	0x00000030
#define TX4939_VPC_CTRLA_VDVLDP	0x00000008
#define TX4939_VPC_CTRLA_VDMODE	0x00000004
#define TX4939_VPC_CTRLA_VDFOR	0x00000002
#define TX4939_VPC_CTRLA_ENVPC	0x00000001

/*                                */
#define TX4939_VPC_DESC_CTRL1_ERR_MASK	0x00000006
#define TX4939_VPC_DESC_CTRL1_OWN	0x00000001

#define tx4939_ddrcptr	((struct tx4939_ddrc_reg __iomem *)TX4939_DDRC_REG)
#define tx4939_ebuscptr		tx4938_ebuscptr
#define tx4939_ircptr \
		((struct tx4939_irc_reg __iomem *)TX4939_IRC_REG)
#define tx4939_pcicptr		tx4938_pcicptr
#define tx4939_pcic1ptr		tx4938_pcic1ptr
#define tx4939_ccfgptr \
		((struct tx4939_ccfg_reg __iomem *)TX4939_CCFG_REG)
#define tx4939_sramcptr		tx4938_sramcptr
#define tx4939_rtcptr \
		((struct tx4939_rtc_reg __iomem *)TX4939_RTC_REG)
#define tx4939_cryptoptr \
		((struct tx4939_crypto_reg __iomem *)TX4939_CRYPTO_REG)
#define tx4939_vpcptr	((struct tx4939_vpc_reg __iomem *)TX4939_VPC_REG)

#define TX4939_REV_MAJ_MIN()	\
	((__u32)__raw_readq(&tx4939_ccfgptr->crir) & 0x00ff)
#define TX4939_REV_PCODE()	\
	((__u32)__raw_readq(&tx4939_ccfgptr->crir) >> 16)
#define TX4939_CCFG_BCFG()	\
	((__u32)((__raw_readq(&tx4939_ccfgptr->ccfg) & TX4939_CCFG_BCFG_MASK) \
		 >> 32))

#define tx4939_ccfg_clear(bits)	tx4938_ccfg_clear(bits)
#define tx4939_ccfg_set(bits)	tx4938_ccfg_set(bits)
#define tx4939_ccfg_change(change, new)	tx4938_ccfg_change(change, new)

#define TX4939_EBUSC_CR(ch)	TX4927_EBUSC_CR(ch)
#define TX4939_EBUSC_BA(ch)	TX4927_EBUSC_BA(ch)
#define TX4939_EBUSC_SIZE(ch)	TX4927_EBUSC_SIZE(ch)
#define TX4939_EBUSC_WIDTH(ch)	\
	(16 >> ((__u32)(TX4939_EBUSC_CR(ch) >> 20) & 0x1))

/*                                                                 */
#define TX4939_SCLK0(mst)	\
	((((mst) + 245/2) / 245UL * 429 * 16 + 19) / 19 / 2)

void tx4939_wdt_init(void);
void tx4939_add_memory_regions(void);
void tx4939_setup(void);
void tx4939_time_init(unsigned int tmrnr);
void tx4939_sio_init(unsigned int sclk, unsigned int cts_mask);
void tx4939_spi_init(int busid);
void tx4939_ethaddr_init(unsigned char *addr0, unsigned char *addr1);
int tx4939_report_pciclk(void);
void tx4939_report_pci1clk(void);
struct pci_dev;
int tx4939_pcic1_map_irq(const struct pci_dev *dev, u8 slot);
int tx4939_pci_map_irq(const struct pci_dev *dev, u8 slot, u8 pin);
void tx4939_setup_pcierr_irq(void);
void tx4939_irq_init(void);
int tx4939_irq(void);
void tx4939_mtd_init(int ch);
void tx4939_ata_init(void);
void tx4939_rtc_init(void);
void tx4939_ndfmc_init(unsigned int hold, unsigned int spw,
		       unsigned char ch_mask, unsigned char wide_mask);
void tx4939_dmac_init(int memcpy_chan0, int memcpy_chan1);
void tx4939_aclc_init(void);
void tx4939_sramc_init(void);
void tx4939_rng_init(void);

#endif /*                     */
