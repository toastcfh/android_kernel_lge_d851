/*
                                                             
 */
#ifndef _SKGE_H
#define _SKGE_H
#include <linux/interrupt.h>

/*                      */
#define PCI_DEV_REG1	0x40
#define  PCI_PHY_COMA	0x8000000
#define  PCI_VIO	0x2000000

#define PCI_DEV_REG2	0x44
#define  PCI_VPD_ROM_SZ	7L<<14	/*                                */
#define  PCI_REV_DESC	1<<2	/*                          */

#define PCI_STATUS_ERROR_BITS (PCI_STATUS_DETECTED_PARITY | \
			       PCI_STATUS_SIG_SYSTEM_ERROR | \
			       PCI_STATUS_REC_MASTER_ABORT | \
			       PCI_STATUS_REC_TARGET_ABORT | \
			       PCI_STATUS_PARITY)

enum csr_regs {
	B0_RAP	= 0x0000,
	B0_CTST	= 0x0004,
	B0_LED	= 0x0006,
	B0_POWER_CTRL	= 0x0007,
	B0_ISRC	= 0x0008,
	B0_IMSK	= 0x000c,
	B0_HWE_ISRC	= 0x0010,
	B0_HWE_IMSK	= 0x0014,
	B0_SP_ISRC	= 0x0018,
	B0_XM1_IMSK	= 0x0020,
	B0_XM1_ISRC	= 0x0028,
	B0_XM1_PHY_ADDR	= 0x0030,
	B0_XM1_PHY_DATA	= 0x0034,
	B0_XM2_IMSK	= 0x0040,
	B0_XM2_ISRC	= 0x0048,
	B0_XM2_PHY_ADDR	= 0x0050,
	B0_XM2_PHY_DATA	= 0x0054,
	B0_R1_CSR	= 0x0060,
	B0_R2_CSR	= 0x0064,
	B0_XS1_CSR	= 0x0068,
	B0_XA1_CSR	= 0x006c,
	B0_XS2_CSR	= 0x0070,
	B0_XA2_CSR	= 0x0074,

	B2_MAC_1	= 0x0100,
	B2_MAC_2	= 0x0108,
	B2_MAC_3	= 0x0110,
	B2_CONN_TYP	= 0x0118,
	B2_PMD_TYP	= 0x0119,
	B2_MAC_CFG	= 0x011a,
	B2_CHIP_ID	= 0x011b,
	B2_E_0		= 0x011c,
	B2_E_1		= 0x011d,
	B2_E_2		= 0x011e,
	B2_E_3		= 0x011f,
	B2_FAR		= 0x0120,
	B2_FDP		= 0x0124,
	B2_LD_CTRL	= 0x0128,
	B2_LD_TEST	= 0x0129,
	B2_TI_INI	= 0x0130,
	B2_TI_VAL	= 0x0134,
	B2_TI_CTRL	= 0x0138,
	B2_TI_TEST	= 0x0139,
	B2_IRQM_INI	= 0x0140,
	B2_IRQM_VAL	= 0x0144,
	B2_IRQM_CTRL	= 0x0148,
	B2_IRQM_TEST	= 0x0149,
	B2_IRQM_MSK	= 0x014c,
	B2_IRQM_HWE_MSK	= 0x0150,
	B2_TST_CTRL1	= 0x0158,
	B2_TST_CTRL2	= 0x0159,
	B2_GP_IO	= 0x015c,
	B2_I2C_CTRL	= 0x0160,
	B2_I2C_DATA	= 0x0164,
	B2_I2C_IRQ	= 0x0168,
	B2_I2C_SW	= 0x016c,
	B2_BSC_INI	= 0x0170,
	B2_BSC_VAL	= 0x0174,
	B2_BSC_CTRL	= 0x0178,
	B2_BSC_STAT	= 0x0179,
	B2_BSC_TST	= 0x017a,

	B3_RAM_ADDR	= 0x0180,
	B3_RAM_DATA_LO	= 0x0184,
	B3_RAM_DATA_HI	= 0x0188,
	B3_RI_WTO_R1	= 0x0190,
	B3_RI_WTO_XA1	= 0x0191,
	B3_RI_WTO_XS1	= 0x0192,
	B3_RI_RTO_R1	= 0x0193,
	B3_RI_RTO_XA1	= 0x0194,
	B3_RI_RTO_XS1	= 0x0195,
	B3_RI_WTO_R2	= 0x0196,
	B3_RI_WTO_XA2	= 0x0197,
	B3_RI_WTO_XS2	= 0x0198,
	B3_RI_RTO_R2	= 0x0199,
	B3_RI_RTO_XA2	= 0x019a,
	B3_RI_RTO_XS2	= 0x019b,
	B3_RI_TO_VAL	= 0x019c,
	B3_RI_CTRL	= 0x01a0,
	B3_RI_TEST	= 0x01a2,
	B3_MA_TOINI_RX1	= 0x01b0,
	B3_MA_TOINI_RX2	= 0x01b1,
	B3_MA_TOINI_TX1	= 0x01b2,
	B3_MA_TOINI_TX2	= 0x01b3,
	B3_MA_TOVAL_RX1	= 0x01b4,
	B3_MA_TOVAL_RX2	= 0x01b5,
	B3_MA_TOVAL_TX1	= 0x01b6,
	B3_MA_TOVAL_TX2	= 0x01b7,
	B3_MA_TO_CTRL	= 0x01b8,
	B3_MA_TO_TEST	= 0x01ba,
	B3_MA_RCINI_RX1	= 0x01c0,
	B3_MA_RCINI_RX2	= 0x01c1,
	B3_MA_RCINI_TX1	= 0x01c2,
	B3_MA_RCINI_TX2	= 0x01c3,
	B3_MA_RCVAL_RX1	= 0x01c4,
	B3_MA_RCVAL_RX2	= 0x01c5,
	B3_MA_RCVAL_TX1	= 0x01c6,
	B3_MA_RCVAL_TX2	= 0x01c7,
	B3_MA_RC_CTRL	= 0x01c8,
	B3_MA_RC_TEST	= 0x01ca,
	B3_PA_TOINI_RX1	= 0x01d0,
	B3_PA_TOINI_RX2	= 0x01d4,
	B3_PA_TOINI_TX1	= 0x01d8,
	B3_PA_TOINI_TX2	= 0x01dc,
	B3_PA_TOVAL_RX1	= 0x01e0,
	B3_PA_TOVAL_RX2	= 0x01e4,
	B3_PA_TOVAL_TX1	= 0x01e8,
	B3_PA_TOVAL_TX2	= 0x01ec,
	B3_PA_CTRL	= 0x01f0,
	B3_PA_TEST	= 0x01f2,
};

/*                                          */
enum {
	CS_CLK_RUN_HOT	= 1<<13,/*                                  */
	CS_CLK_RUN_RST	= 1<<12,/*                                  */
	CS_CLK_RUN_ENA	= 1<<11,/*                                  */
	CS_VAUX_AVAIL	= 1<<10,/*                             */
	CS_BUS_CLOCK	= 1<<9,	/*                           */
	CS_BUS_SLOT_SZ	= 1<<8,	/*                                */
	CS_ST_SW_IRQ	= 1<<7,	/*                    */
	CS_CL_SW_IRQ	= 1<<6,	/*                      */
	CS_STOP_DONE	= 1<<5,	/*                         */
	CS_STOP_MAST	= 1<<4,	/*                                */
	CS_MRST_CLR	= 1<<3,	/*                    */
	CS_MRST_SET	= 1<<2,	/*                  */
	CS_RST_CLR	= 1<<1,	/*                      */
	CS_RST_SET	= 1,	/*                      */

/*                              */
/*                      */
	LED_STAT_ON	= 1<<1,	/*               */
	LED_STAT_OFF	= 1,		/*                */

/*                                                     */
	PC_VAUX_ENA	= 1<<7,	/*                     */
	PC_VAUX_DIS	= 1<<6,	/*                     */
	PC_VCC_ENA	= 1<<5,	/*                    */
	PC_VCC_DIS	= 1<<4,	/*                    */
	PC_VAUX_ON	= 1<<3,	/*                 */
	PC_VAUX_OFF	= 1<<2,	/*                 */
	PC_VCC_ON	= 1<<1,	/*                */
	PC_VCC_OFF	= 1<<0,	/*                */
};

/*                                         */
enum {
	IS_ALL_MSK	= 0xbffffffful,	/*                    */
	IS_HW_ERR	= 1<<31,	/*                    */
					/*                  */
	IS_PA_TO_RX1	= 1<<29,	/*                        */
	IS_PA_TO_RX2	= 1<<28,	/*                        */
	IS_PA_TO_TX1	= 1<<27,	/*                        */
	IS_PA_TO_TX2	= 1<<26,	/*                        */
	IS_I2C_READY	= 1<<25,	/*                      */
	IS_IRQ_SW	= 1<<24,	/*               */
	IS_EXT_REG	= 1<<23,	/*                                     */
					/*                           */
	IS_TIMINT	= 1<<22,	/*                */
	IS_MAC1		= 1<<21,	/*                */
	IS_LNK_SYNC_M1	= 1<<20,	/*                          */
	IS_MAC2		= 1<<19,	/*                */
	IS_LNK_SYNC_M2	= 1<<18,	/*                          */
/*                 */
	IS_R1_B		= 1<<17,	/*                    */
	IS_R1_F		= 1<<16,	/*                   */
	IS_R1_C		= 1<<15,	/*                     */
/*                 */
	IS_R2_B		= 1<<14,	/*                    */
	IS_R2_F		= 1<<13,	/*                   */
	IS_R2_C		= 1<<12,	/*                     */
/*                              */
	IS_XS1_B	= 1<<11,	/*                     */
	IS_XS1_F	= 1<<10,	/*                    */
	IS_XS1_C	= 1<<9,		/*                      */
/*                               */
	IS_XA1_B	= 1<<8,		/*                     */
	IS_XA1_F	= 1<<7,		/*                    */
	IS_XA1_C	= 1<<6,		/*                      */
/*                              */
	IS_XS2_B	= 1<<5,		/*                     */
	IS_XS2_F	= 1<<4,		/*                    */
	IS_XS2_C	= 1<<3,		/*                      */
/*                               */
	IS_XA2_B	= 1<<2,		/*                     */
	IS_XA2_F	= 1<<1,		/*                    */
	IS_XA2_C	= 1<<0,		/*                      */

	IS_TO_PORT1	= IS_PA_TO_RX1 | IS_PA_TO_TX1,
	IS_TO_PORT2	= IS_PA_TO_RX2 | IS_PA_TO_TX2,

	IS_PORT_1	= IS_XA1_F| IS_R1_F | IS_TO_PORT1 | IS_MAC1,
	IS_PORT_2	= IS_XA2_F| IS_R2_F | IS_TO_PORT2 | IS_MAC2,
};


/*                                                     */
enum {
	IS_IRQ_TIST_OV	= 1<<13, /*                                        */
	IS_IRQ_SENSOR	= 1<<12, /*                              */
	IS_IRQ_MST_ERR	= 1<<11, /*                           */
	IS_IRQ_STAT	= 1<<10, /*                      */
	IS_NO_STAT_M1	= 1<<9,	/*                         */
	IS_NO_STAT_M2	= 1<<8,	/*                         */
	IS_NO_TIST_M1	= 1<<7,	/*                          */
	IS_NO_TIST_M2	= 1<<6,	/*                          */
	IS_RAM_RD_PAR	= 1<<5,	/*                        */
	IS_RAM_WR_PAR	= 1<<4,	/*                        */
	IS_M1_PAR_ERR	= 1<<3,	/*                    */
	IS_M2_PAR_ERR	= 1<<2,	/*                    */
	IS_R1_PAR_ERR	= 1<<1,	/*                       */
	IS_R2_PAR_ERR	= 1<<0,	/*                       */

	IS_ERR_MSK	= IS_IRQ_MST_ERR | IS_IRQ_STAT
			| IS_RAM_RD_PAR | IS_RAM_WR_PAR
			| IS_M1_PAR_ERR | IS_M2_PAR_ERR
			| IS_R1_PAR_ERR | IS_R2_PAR_ERR,
};

/*                                             */
enum {
	TST_FRC_DPERR_MR = 1<<7, /*                          */
	TST_FRC_DPERR_MW = 1<<6, /*                          */
	TST_FRC_DPERR_TR = 1<<5, /*                          */
	TST_FRC_DPERR_TW = 1<<4, /*                          */
	TST_FRC_APERR_M	 = 1<<3, /*                       */
	TST_FRC_APERR_T	 = 1<<2, /*                       */
	TST_CFG_WRITE_ON = 1<<1, /*                       */
	TST_CFG_WRITE_OFF= 1<<0, /*                       */
};

/*                                                      */
enum {
	CFG_CHIP_R_MSK	  = 0xf<<4,	/*                          */
					/*                     */
	CFG_DIS_M2_CLK	  = 1<<1,	/*                           */
	CFG_SNG_MAC	  = 1<<0,	/*                               */
};

/*                                                */
enum {
	CHIP_ID_GENESIS	   = 0x0a, /*                     */
	CHIP_ID_YUKON	   = 0xb0, /*                   */
	CHIP_ID_YUKON_LITE = 0xb1, /*                                     */
	CHIP_ID_YUKON_LP   = 0xb2, /*                      */
	CHIP_ID_YUKON_XL   = 0xb3, /*                        */
	CHIP_ID_YUKON_EC   = 0xb6, /*                        */
 	CHIP_ID_YUKON_FE   = 0xb7, /*                        */

	CHIP_REV_YU_LITE_A1  = 3,	/*                                */
	CHIP_REV_YU_LITE_A3  = 7,	/*                             */
};

/*                                  */
/*                                                  */
enum {
	TIM_START	= 1<<2,	/*             */
	TIM_STOP	= 1<<1,	/*             */
	TIM_CLR_IRQ	= 1<<0,	/*                         */
};

/*                               */
/*                                               */
/*                                                    */
enum {
	TIM_T_ON	= 1<<2,	/*              */
	TIM_T_OFF	= 1<<1,	/*               */
	TIM_T_STEP	= 1<<0,	/*           */
};

/*                                               */
enum {
	GP_DIR_9 = 1<<25, /*                         */
	GP_DIR_8 = 1<<24, /*                         */
	GP_DIR_7 = 1<<23, /*                         */
	GP_DIR_6 = 1<<22, /*                         */
	GP_DIR_5 = 1<<21, /*                         */
	GP_DIR_4 = 1<<20, /*                         */
	GP_DIR_3 = 1<<19, /*                         */
	GP_DIR_2 = 1<<18, /*                         */
	GP_DIR_1 = 1<<17, /*                         */
	GP_DIR_0 = 1<<16, /*                         */

	GP_IO_9	= 1<<9,	/*          */
	GP_IO_8	= 1<<8,	/*          */
	GP_IO_7	= 1<<7,	/*          */
	GP_IO_6	= 1<<6,	/*          */
	GP_IO_5	= 1<<5,	/*          */
	GP_IO_4	= 1<<4,	/*          */
	GP_IO_3	= 1<<3,	/*          */
	GP_IO_2	= 1<<2,	/*          */
	GP_IO_1	= 1<<1,	/*          */
	GP_IO_0	= 1<<0,	/*          */
};

/*                           */
/*                                       */
/*                                       */
enum {
	BMU_OWN		= 1<<31, /*                       */
	BMU_STF		= 1<<30, /*                */
	BMU_EOF		= 1<<29, /*              */
	BMU_IRQ_EOB	= 1<<28, /*                         */
	BMU_IRQ_EOF	= 1<<27, /*                        */
				/*                      */
	BMU_STFWD	= 1<<26, /*                            */
	BMU_NO_FCS	= 1<<25, /*                                       */
	BMU_SW	= 1<<24, /*                            */
				/*                      */
	BMU_DEV_0	= 1<<26, /*                            */
	BMU_STAT_VAL	= 1<<25, /*                      */
	BMU_TIST_VAL	= 1<<24, /*                         */
			/*                               */
	BMU_CHECK	= 0x55<<16, /*                   */
	BMU_TCP_CHECK	= 0x56<<16, /*                    */
	BMU_UDP_CHECK	= 0x57<<16, /*                                 */
	BMU_BBC		= 0xffffL, /*                                 */
};

/*                                                  */
enum {
	 BSC_START	= 1<<1,	/*                            */
	 BSC_STOP	= 1<<0,	/*                            */
};

/*                                                 */
enum {
	BSC_SRC		= 1<<0,	/*                            */
};

/*                                                  */
enum {
	BSC_T_ON	= 1<<2,	/*              */
	BSC_T_OFF	= 1<<1,	/*               */
	BSC_T_STEP	= 1<<0,	/*           */
};

/*                                                   */
					/*                      */
#define RAM_ADR_RAN	0x0007ffffL	/*                               */
/*                         */

/*                                               */
enum {
	RI_CLR_RD_PERR	= 1<<9,	/*                               */
	RI_CLR_WR_PERR	= 1<<8,	/*                               */

	RI_RST_CLR	= 1<<1,	/*                           */
	RI_RST_SET	= 1<<0,	/*                           */
};

/*                       */
/*                                                   */
enum {
	MA_FOE_ON	= 1<<3,	/*                            */
	MA_FOE_OFF	= 1<<2,	/*                             */
	MA_RST_CLR	= 1<<1,	/*                         */
	MA_RST_SET	= 1<<0,	/*                         */

};

/*                */
#define SK_MAC_TO_53	72		/*                     */
#define SK_PKT_TO_53	0x2000		/*                        */
#define SK_PKT_TO_MAX	0xffff		/*               */
#define SK_RI_TO_53	36		/*                       */

/*                          */
/*                                                 */
enum {
	PA_CLR_TO_TX2	= 1<<13,/*                              */
	PA_CLR_TO_TX1	= 1<<12,/*                              */
	PA_CLR_TO_RX2	= 1<<11,/*                              */
	PA_CLR_TO_RX1	= 1<<10,/*                              */
	PA_ENA_TO_TX2	= 1<<9,	/*                           */
	PA_DIS_TO_TX2	= 1<<8,	/*                           */
	PA_ENA_TO_TX1	= 1<<7,	/*                           */
	PA_DIS_TO_TX1	= 1<<6,	/*                           */
	PA_ENA_TO_RX2	= 1<<5,	/*                           */
	PA_DIS_TO_RX2	= 1<<4,	/*                           */
	PA_ENA_TO_RX1	= 1<<3,	/*                           */
	PA_DIS_TO_RX1	= 1<<2,	/*                           */
	PA_RST_CLR	= 1<<1,	/*                         */
	PA_RST_SET	= 1<<0,	/*                         */
};

#define PA_ENA_TO_ALL	(PA_ENA_TO_RX1 | PA_ENA_TO_RX2 |\
						PA_ENA_TO_TX1 | PA_ENA_TO_TX2)


/*                                                                */
/*                                                    */
/*                                                 */
/*                                                   */
/*                                                */

#define TXA_MAX_VAL	0x00ffffffUL	/*                                   */

/*                                              */
enum {
	TXA_ENA_FSYNC	= 1<<7,	/*                                */
	TXA_DIS_FSYNC	= 1<<6,	/*                                */
	TXA_ENA_ALLOC	= 1<<5,	/*                                 */
	TXA_DIS_ALLOC	= 1<<4,	/*                                 */
	TXA_START_RC	= 1<<3,	/*                         */
	TXA_STOP_RC	= 1<<2,	/*                         */
	TXA_ENA_ARB	= 1<<1,	/*                    */
	TXA_DIS_ARB	= 1<<0,	/*                    */
};

/*
             
 */
/*                                                                */
enum {
	TXA_ITI_INI	= 0x0200,/*                                      */
	TXA_ITI_VAL	= 0x0204,/*                                    */
	TXA_LIM_INI	= 0x0208,/*                                      */
	TXA_LIM_VAL	= 0x020c,/*                                   */
	TXA_CTRL	= 0x0210,/*                                    */
	TXA_TEST	= 0x0211,/*                                 */
	TXA_STAT	= 0x0212,/*                                   */
};


enum {
	B6_EXT_REG	= 0x0300,/*                                   */
	B7_CFG_SPC	= 0x0380,/*                                    */
	B8_RQ1_REGS	= 0x0400,/*                 */
	B8_RQ2_REGS	= 0x0480,/*                 */
	B8_TS1_REGS	= 0x0600,/*                       */
	B8_TA1_REGS	= 0x0680,/*                        */
	B8_TS2_REGS	= 0x0700,/*                       */
	B8_TA2_REGS	= 0x0780,/*                       */
	B16_RAM_REGS	= 0x0800,/*                      */
};

/*                                                */
enum {
	B8_Q_REGS = 0x0400, /*                         */
	Q_D	= 0x00,	/*                             */
	Q_DA_L	= 0x20,	/*                                             */
	Q_DA_H	= 0x24,	/*                                              */
	Q_AC_L	= 0x28,	/*                                          */
	Q_AC_H	= 0x2c,	/*                                           */
	Q_BC	= 0x30,	/*                             */
	Q_CSR	= 0x34,	/*                                    */
	Q_F	= 0x38,	/*                      */
	Q_T1	= 0x3c,	/*                        */
	Q_T1_TR	= 0x3c,	/*                                    */
	Q_T1_WR	= 0x3d,	/*                                            */
	Q_T1_RD	= 0x3e,	/*                                           */
	Q_T1_SV	= 0x3f,	/*                                      */
	Q_T2	= 0x40,	/*                        */
	Q_T3	= 0x44,	/*                        */

};
#define Q_ADDR(reg, offs) (B8_Q_REGS + (reg) + (offs))

/*                             */
enum {

	RB_START= 0x00,/*                                 */
	RB_END	= 0x04,/*                               */
	RB_WP	= 0x08,/*                                 */
	RB_RP	= 0x0c,/*                                */
	RB_RX_UTPP= 0x10,/*                                         */
	RB_RX_LTPP= 0x14,/*                                         */
	RB_RX_UTHP= 0x18,/*                                      */
	RB_RX_LTHP= 0x1c,/*                                      */
	/*                                                  */
	RB_PC	= 0x20,/*                                  */
	RB_LEV	= 0x24,/*                                  */
	RB_CTRL	= 0x28,/*                                    */
	RB_TST1	= 0x29,/*                                   */
	RB_TST2	= 0x2a,/*                                   */
};

/*                             */
enum {
	Q_R1	= 0x0000,	/*                 */
	Q_R2	= 0x0080,	/*                 */
	Q_XS1	= 0x0200,	/*                              */
	Q_XA1	= 0x0280,	/*                               */
	Q_XS2	= 0x0300,	/*                              */
	Q_XA2	= 0x0380,	/*                               */
};

/*                     */
enum {
	SK_MAC_XMAC =	0,	/*               */
	SK_MAC_GMAC =	1,	/*              */
};

/*                     */
enum {
	SK_PHY_XMAC	= 0,/*                       */
	SK_PHY_BCOM	= 1,/*                  */
	SK_PHY_LONE	= 2,/*                                   */
	SK_PHY_NAT	= 3,/*                                   */
	SK_PHY_MARV_COPPER= 4,/*                  */
	SK_PHY_MARV_FIBER = 5,/*                                   */
};

/*                                               */
enum {
	PHY_ADDR_XMAC	= 0<<8,
	PHY_ADDR_BCOM	= 1<<8,

/*                                               */
	PHY_ADDR_MARV	= 0,
};

#define RB_ADDR(offs, queue) ((u16)B16_RAM_REGS + (u16)(queue) + (offs))

/*                                                                  */
enum {
	RX_MFF_EA	= 0x0c00,/*                                     */
	RX_MFF_WP	= 0x0c04,/*                                       */

	RX_MFF_RP	= 0x0c0c,/*                                      */
	RX_MFF_PC	= 0x0c10,/*                                    */
	RX_MFF_LEV	= 0x0c14,/*                               */
	RX_MFF_CTRL1	= 0x0c18,/*                                      */
	RX_MFF_STAT_TO	= 0x0c1a,/*                                   */
	RX_MFF_TIST_TO	= 0x0c1b,/*                                       */
	RX_MFF_CTRL2	= 0x0c1c,/*                                      */
	RX_MFF_TST1	= 0x0c1d,/*                                    */
	RX_MFF_TST2	= 0x0c1e,/*                                    */

	RX_LED_INI	= 0x0c20,/*                                   */
	RX_LED_VAL	= 0x0c24,/*                                      */
	RX_LED_CTRL	= 0x0c28,/*                                    */
	RX_LED_TST	= 0x0c29,/*                                      */

	LNK_SYNC_INI	= 0x0c30,/*                                 */
	LNK_SYNC_VAL	= 0x0c34,/*                                    */
	LNK_SYNC_CTRL	= 0x0c38,/*                                       */
	LNK_SYNC_TST	= 0x0c39,/*                                    */
	LNK_LED_REG	= 0x0c3c,/*                          */
};

/*                                                        */
/*                                                    */
enum {
	MFF_ENA_RDY_PAT	= 1<<13,	/*                     */
	MFF_DIS_RDY_PAT	= 1<<12,	/*                     */
	MFF_ENA_TIM_PAT	= 1<<11,	/*                      */
	MFF_DIS_TIM_PAT	= 1<<10,	/*                      */
	MFF_ENA_ALM_FUL	= 1<<9,	/*                         */
	MFF_DIS_ALM_FUL	= 1<<8,	/*                         */
	MFF_ENA_PAUSE	= 1<<7,	/*                         */
	MFF_DIS_PAUSE	= 1<<6,	/*                         */
	MFF_ENA_FLUSH	= 1<<5,	/*                        */
	MFF_DIS_FLUSH	= 1<<4,	/*                        */
	MFF_ENA_TIST	= 1<<3,	/*                          */
	MFF_DIS_TIST	= 1<<2,	/*                          */
	MFF_CLR_INTIST	= 1<<1,	/*                         */
	MFF_CLR_INSTAT	= 1<<0,	/*                     */
	MFF_RX_CTRL_DEF = MFF_ENA_TIM_PAT,
};

/*                                                     */
enum {
	MFF_CLR_PERR	= 1<<15, /*                        */

	MFF_ENA_PKT_REC	= 1<<13, /*                         */
	MFF_DIS_PKT_REC	= 1<<12, /*                         */

	MFF_ENA_W4E	= 1<<7,	/*                        */
	MFF_DIS_W4E	= 1<<6,	/*                        */

	MFF_ENA_LOOPB	= 1<<3,	/*                  */
	MFF_DIS_LOOPB	= 1<<2,	/*                  */
	MFF_CLR_MAC_RST	= 1<<1,	/*                  */
	MFF_SET_MAC_RST	= 1<<0,	/*                  */

	MFF_TX_CTRL_DEF	 = MFF_ENA_PKT_REC | (u16) MFF_ENA_TIM_PAT | MFF_ENA_FLUSH,
};


/*                                                       */
/*                                                        */
enum {
	MFF_WSP_T_ON	= 1<<6,	/*                             */
	MFF_WSP_T_OFF	= 1<<5,	/*                             */
	MFF_WSP_INC	= 1<<4,	/*                                */
	MFF_PC_DEC	= 1<<3,	/*                          */
	MFF_PC_T_ON	= 1<<2,	/*                        */
	MFF_PC_T_OFF	= 1<<1,	/*                         */
	MFF_PC_INC	= 1<<0,	/*                          */
};

/*                                                       */
/*                                                        */
enum {
	MFF_WP_T_ON	= 1<<6,	/*                       */
	MFF_WP_T_OFF	= 1<<5,	/*                        */
	MFF_WP_INC	= 1<<4,	/*                      */

	MFF_RP_T_ON	= 1<<2,	/*                      */
	MFF_RP_T_OFF	= 1<<1,	/*                       */
	MFF_RP_DEC	= 1<<0,	/*                        */
};

/*                                                    */
/*                                                     */
enum {
	MFF_ENA_OP_MD	= 1<<3,	/*                        */
	MFF_DIS_OP_MD	= 1<<2,	/*                        */
	MFF_RST_CLR	= 1<<1,	/*                      */
	MFF_RST_SET	= 1<<0,	/*                      */
};


/*                                           */

/*                                                 */
/*                                                  */
/*                                                     */
enum {
	LED_START	= 1<<2,	/*             */
	LED_STOP	= 1<<1,	/*            */
	LED_STATE	= 1<<0,	/*                            */
};

/*                                                  */
/*                                                   */
/*                                                 */
enum {
	LED_T_ON	= 1<<2,	/*                          */
	LED_T_OFF	= 1<<1,	/*                           */
	LED_T_STEP	= 1<<0,	/*                  */
};

/*                                        */
enum {
	LED_BLK_ON	= 1<<5,	/*                      */
	LED_BLK_OFF	= 1<<4,	/*                       */
	LED_SYNC_ON	= 1<<3,	/*                             */
	LED_SYNC_OFF	= 1<<2,	/*                         */
	LED_ON	= 1<<1,	/*               */
	LED_OFF	= 1<<0,	/*                */
};

/*                           */
enum {
	RX_GMF_EA	= 0x0c40,/*                                 */
	RX_GMF_AF_THR	= 0x0c44,/*                                         */
	RX_GMF_CTRL_T	= 0x0c48,/*                                  */
	RX_GMF_FL_MSK	= 0x0c4c,/*                                */
	RX_GMF_FL_THR	= 0x0c50,/*                                     */
	RX_GMF_WP	= 0x0c60,/*                                   */
	RX_GMF_WLEV	= 0x0c68,/*                                 */
	RX_GMF_RP	= 0x0c70,/*                                  */
	RX_GMF_RLEV	= 0x0c78,/*                                */
};


/*                                           */
enum {
	TXA_INT_T_ON	= 1<<5,	/*                               */
	TXA_INT_T_OFF	= 1<<4,	/*                                */
	TXA_INT_T_STEP	= 1<<3,	/*                            */
	TXA_LIM_T_ON	= 1<<2,	/*                            */
	TXA_LIM_T_OFF	= 1<<1,	/*                             */
	TXA_LIM_T_STEP	= 1<<0,	/*                         */
};

/*                                             */
enum {
	TXA_PRIO_XS	= 1<<0,	/*                             */
};


/*                                    */

/*                              */
/*                                            */
/*                                            */
/*                                                  */
/*                                                   */
/*                                                  */
/*                                                   */
/*                                            */

enum {
	CSR_SV_IDLE	= 1<<24,	/*             */

	CSR_DESC_CLR	= 1<<21,	/*                       */
	CSR_DESC_SET	= 1<<20,	/*                       */
	CSR_FIFO_CLR	= 1<<19,	/*                      */
	CSR_FIFO_SET	= 1<<18,	/*                      */
	CSR_HPI_RUN	= 1<<17,	/*                */
	CSR_HPI_RST	= 1<<16,	/*                        */
	CSR_SV_RUN	= 1<<15,	/*                       */
	CSR_SV_RST	= 1<<14,	/*                       */
	CSR_DREAD_RUN	= 1<<13,	/*                       */
	CSR_DREAD_RST	= 1<<12,	/*                       */
	CSR_DWRITE_RUN	= 1<<11,	/*                        */
	CSR_DWRITE_RST	= 1<<10,	/*                        */
	CSR_TRANS_RUN	= 1<<9,		/*                     */
	CSR_TRANS_RST	= 1<<8,		/*                     */
	CSR_ENA_POL	= 1<<7,		/*                       */
	CSR_DIS_POL	= 1<<6,		/*                       */
	CSR_STOP	= 1<<5,		/*                   */
	CSR_START	= 1<<4,		/*                   */
	CSR_IRQ_CL_P	= 1<<3,		/*                       */
	CSR_IRQ_CL_B	= 1<<2,		/*               */
	CSR_IRQ_CL_F	= 1<<1,		/*               */
	CSR_IRQ_CL_C	= 1<<0,		/*               */
};

#define CSR_SET_RESET	(CSR_DESC_SET | CSR_FIFO_SET | CSR_HPI_RST |\
			CSR_SV_RST | CSR_DREAD_RST | CSR_DWRITE_RST |\
			CSR_TRANS_RST)
#define CSR_CLR_RESET	(CSR_DESC_CLR | CSR_FIFO_CLR | CSR_HPI_RUN |\
			CSR_SV_RUN | CSR_DREAD_RUN | CSR_DWRITE_RUN |\
			CSR_TRANS_RUN)

/*                             */
enum {
	F_ALM_FULL	= 1<<27,	/*                      */
	F_EMPTY		= 1<<27,	/*                     */
	F_FIFO_EOF	= 1<<26,	/*                            */
	F_WM_REACHED	= 1<<25,	/*                   */

	F_FIFO_LEVEL	= 0x1fL<<16,	/*                                 */
	F_WATER_MARK	= 0x0007ffL,	/*                       */
};

/*                                                                 */
/*                                           */
/*                                        */
/*                                         */
/*                                        */
/*                                                   */
/*                                                   */
/*                                                  */
/*                                                  */
/*                                          */
/*                                           */

#define RB_MSK	0x0007ffff	/*                                     */
/*                                             */
/*                                             */

/*                                              */
enum {
	RB_ENA_STFWD	= 1<<5,	/*                         */
	RB_DIS_STFWD	= 1<<4,	/*                         */
	RB_ENA_OP_MD	= 1<<3,	/*                        */
	RB_DIS_OP_MD	= 1<<2,	/*                        */
	RB_RST_CLR	= 1<<1,	/*                         */
	RB_RST_SET	= 1<<0,	/*                         */
};

/*                                                              */
enum {
	TX_MFF_EA	= 0x0d00,/*                                      */
	TX_MFF_WP	= 0x0d04,/*                                     */
	TX_MFF_WSP	= 0x0d08,/*                                        */
	TX_MFF_RP	= 0x0d0c,/*                                     */
	TX_MFF_PC	= 0x0d10,/*                                     */
	TX_MFF_LEV	= 0x0d14,/*                                */
	TX_MFF_CTRL1	= 0x0d18,/*                                     */
	TX_MFF_WAF	= 0x0d1a,/*                                      */

	TX_MFF_CTRL2	= 0x0d1c,/*                                     */
	TX_MFF_TST1	= 0x0d1d,/*                                     */
	TX_MFF_TST2	= 0x0d1e,/*                                     */

	TX_LED_INI	= 0x0d20,/*                                    */
	TX_LED_VAL	= 0x0d24,/*                                     */
	TX_LED_CTRL	= 0x0d28,/*                                     */
	TX_LED_TST	= 0x0d29,/*                                  */
};

/*                                                           */
#define SK_XMIT_DUR		0x002faf08UL	/*        */
#define SK_BLK_DUR		0x01dcd650UL	/*        */

#define SK_DPOLL_DEF	0x00ee6b28UL	/*                    */

#define SK_DPOLL_MAX	0x00ffffffUL	/*                    */
					/*                     */

#define SK_FACT_62		100	/*                     */
#define SK_FACT_53		 85     /*                       */
#define SK_FACT_78		125	/*                     */


/*                                 */
enum {
	TX_GMF_EA	= 0x0d40,/*                                 */
	TX_GMF_AE_THR	= 0x0d44,/*                                         */
	TX_GMF_CTRL_T	= 0x0d48,/*                                  */

	TX_GMF_WP	= 0x0d60,/*                                    */
	TX_GMF_WSP	= 0x0d64,/*                                        */
	TX_GMF_WLEV	= 0x0d68,/*                                  */

	TX_GMF_RP	= 0x0d70,/*                                   */
	TX_GMF_RSTP	= 0x0d74,/*                                      */
	TX_GMF_RLEV	= 0x0d78,/*                                 */

	/*                                 */
	B28_DPT_INI	= 0x0e00,/*                                       */
	B28_DPT_VAL	= 0x0e04,/*                                       */
	B28_DPT_CTRL	= 0x0e08,/*                                       */

	B28_DPT_TST	= 0x0e0a,/*                                       */

	/*                                         */
	GMAC_TI_ST_VAL	= 0x0e14,/*                                  */
	GMAC_TI_ST_CTRL	= 0x0e18,/*                                  */
	GMAC_TI_ST_TST	= 0x0e1a,/*                                  */
};


enum {
	LINKLED_OFF 	     = 0x01,
	LINKLED_ON  	     = 0x02,
	LINKLED_LINKSYNC_OFF = 0x04,
	LINKLED_LINKSYNC_ON  = 0x08,
	LINKLED_BLINK_OFF    = 0x10,
	LINKLED_BLINK_ON     = 0x20,
};

/*                                              */
enum {
	GMAC_CTRL	= 0x0f00,/*                         */
	GPHY_CTRL	= 0x0f04,/*                         */
	GMAC_IRQ_SRC	= 0x0f08,/*                                  */
	GMAC_IRQ_MSK	= 0x0f0c,/*                                */
	GMAC_LINK_CTRL	= 0x0f10,/*                         */

/*                                                            */

	WOL_REG_OFFS	= 0x20,/*                                         */

	WOL_CTRL_STAT	= 0x0f20,/*                               */
	WOL_MATCH_CTL	= 0x0f22,/*                              */
	WOL_MATCH_RES	= 0x0f23,/*                             */
	WOL_MAC_ADDR	= 0x0f24,/*                        */
	WOL_PATT_RPTR	= 0x0f2c,/*                                 */

/*                                           */

	WOL_PATT_LEN_LO	= 0x0f30,/*                                */
	WOL_PATT_LEN_HI	= 0x0f34,/*                                */

/*                                            */

	WOL_PATT_CNT_0	= 0x0f38,/*                                 */
	WOL_PATT_CNT_4	= 0x0f3c,/*                                 */
};
#define WOL_REGS(port, x)	(x + (port)*0x80)

enum {
	WOL_PATT_RAM_1	= 0x1000,/*                         */
	WOL_PATT_RAM_2	= 0x1400,/*                         */
};
#define WOL_PATT_RAM_BASE(port)	(WOL_PATT_RAM_1 + (port)*0x400)

enum {
	BASE_XMAC_1	= 0x2000,/*                  */
	BASE_GMAC_1	= 0x2800,/*                  */
	BASE_XMAC_2	= 0x3000,/*                  */
	BASE_GMAC_2	= 0x3800,/*                  */
};

/*
                                
 */
enum {
	XMR_FS_LEN	= 0x3fff<<18,	/*                             */
	XMR_FS_LEN_SHIFT = 18,
	XMR_FS_2L_VLAN	= 1<<17, /*                               */
	XMR_FS_1_VLAN	= 1<<16, /*                              */
	XMR_FS_BC	= 1<<15, /*                         */
	XMR_FS_MC	= 1<<14, /*                         */
	XMR_FS_UC	= 1<<13, /*                       */

	XMR_FS_BURST	= 1<<11, /*                    */
	XMR_FS_CEX_ERR	= 1<<10, /*                            */
	XMR_FS_802_3	= 1<<9, /*                     */
	XMR_FS_COL_ERR	= 1<<8, /*                         */
	XMR_FS_CAR_ERR	= 1<<7, /*                             */
	XMR_FS_LEN_ERR	= 1<<6, /*                               */
	XMR_FS_FRA_ERR	= 1<<5, /*                       */
	XMR_FS_RUNT	= 1<<4, /*                    */
	XMR_FS_LNG_ERR	= 1<<3, /*                             */
	XMR_FS_FCS_ERR	= 1<<2, /*                              */
	XMR_FS_ERR	= 1<<1, /*                     */
	XMR_FS_MCTRL	= 1<<0, /*                            */

/*
                            
                                               
                                                    
                                                       
                                                                 
                   
 */
};

/*
                                                       
 */
enum {
	PHY_XMAC_CTRL		= 0x00,/*                                 */
	PHY_XMAC_STAT		= 0x01,/*                                */
	PHY_XMAC_ID0		= 0x02,/*                             */
	PHY_XMAC_ID1		= 0x03,/*                             */
	PHY_XMAC_AUNE_ADV	= 0x04,/*                                    */
	PHY_XMAC_AUNE_LP	= 0x05,/*                                 */
	PHY_XMAC_AUNE_EXP	= 0x06,/*                                    */
	PHY_XMAC_NEPG		= 0x07,/*                               */
	PHY_XMAC_NEPG_LP	= 0x08,/*                                   */

	PHY_XMAC_EXT_STAT	= 0x0f,/*                                */
	PHY_XMAC_RES_ABI	= 0x10,/*                                 */
};
/*
                                                       
 */
enum {
	PHY_BCOM_CTRL		= 0x00,/*                                 */
	PHY_BCOM_STAT		= 0x01,/*                                */
	PHY_BCOM_ID0		= 0x02,/*                             */
	PHY_BCOM_ID1		= 0x03,/*                             */
	PHY_BCOM_AUNE_ADV	= 0x04,/*                                    */
	PHY_BCOM_AUNE_LP	= 0x05,/*                                  */
	PHY_BCOM_AUNE_EXP	= 0x06,/*                                    */
	PHY_BCOM_NEPG		= 0x07,/*                               */
	PHY_BCOM_NEPG_LP	= 0x08,/*                                   */
	/*                             */
	PHY_BCOM_1000T_CTRL	= 0x09,/*                                   */
	PHY_BCOM_1000T_STAT	= 0x0a,/*                                  */
	PHY_BCOM_EXT_STAT	= 0x0f,/*                                */
	PHY_BCOM_P_EXT_CTRL	= 0x10,/*                                  */
	PHY_BCOM_P_EXT_STAT	= 0x11,/*                                  */
	PHY_BCOM_RE_CTR		= 0x12,/*                                  */
	PHY_BCOM_FC_CTR		= 0x13,/*                                    */
	PHY_BCOM_RNO_CTR	= 0x14,/*                                */

	PHY_BCOM_AUX_CTRL	= 0x18,/*                                  */
	PHY_BCOM_AUX_STAT	= 0x19,/*                                   */
	PHY_BCOM_INT_STAT	= 0x1a,/*                                 */
	PHY_BCOM_INT_MASK	= 0x1b,/*                               */
};

/*
                                                     
 */
enum {
	PHY_MARV_CTRL		= 0x00,/*                                 */
	PHY_MARV_STAT		= 0x01,/*                                */
	PHY_MARV_ID0		= 0x02,/*                             */
	PHY_MARV_ID1		= 0x03,/*                             */
	PHY_MARV_AUNE_ADV	= 0x04,/*                                    */
	PHY_MARV_AUNE_LP	= 0x05,/*                                  */
	PHY_MARV_AUNE_EXP	= 0x06,/*                                    */
	PHY_MARV_NEPG		= 0x07,/*                               */
	PHY_MARV_NEPG_LP	= 0x08,/*                                   */
	/*                           */
	PHY_MARV_1000T_CTRL	= 0x09,/*                                   */
	PHY_MARV_1000T_STAT	= 0x0a,/*                                  */
	PHY_MARV_EXT_STAT	= 0x0f,/*                                */
	PHY_MARV_PHY_CTRL	= 0x10,/*                                  */
	PHY_MARV_PHY_STAT	= 0x11,/*                                  */
	PHY_MARV_INT_MASK	= 0x12,/*                               */
	PHY_MARV_INT_STAT	= 0x13,/*                                 */
	PHY_MARV_EXT_CTRL	= 0x14,/*                                   */
	PHY_MARV_RXE_CNT	= 0x15,/*                                  */
	PHY_MARV_EXT_ADR	= 0x16,/*                                     */
	PHY_MARV_PORT_IRQ	= 0x17,/*                                      */
	PHY_MARV_LED_CTRL	= 0x18,/*                            */
	PHY_MARV_LED_OVER	= 0x19,/*                                    */
	PHY_MARV_EXT_CTRL_2	= 0x1a,/*                                     */
	PHY_MARV_EXT_P_STAT	= 0x1b,/*                                    */
	PHY_MARV_CABLE_DIAG	= 0x1c,/*                                 */
	PHY_MARV_PAGE_ADDR	= 0x1d,/*                                      */
	PHY_MARV_PAGE_DATA	= 0x1e,/*                                   */

/*                                             */
	PHY_MARV_FE_LED_PAR	= 0x16,/*                                     */
	PHY_MARV_FE_LED_SER	= 0x17,/*                                     */
	PHY_MARV_FE_VCT_TX	= 0x1a,/*                                    */
	PHY_MARV_FE_VCT_RX	= 0x1b,/*                                    */
	PHY_MARV_FE_SPEC_2	= 0x1c,/*                                    */
};

enum {
	PHY_CT_RESET	= 1<<15, /*                                         */
	PHY_CT_LOOP	= 1<<14, /*                                  */
	PHY_CT_SPS_LSB	= 1<<13, /*                                 */
	PHY_CT_ANE	= 1<<12, /*                                  */
	PHY_CT_PDOWN	= 1<<11, /*                         */
	PHY_CT_ISOL	= 1<<10, /*                      */
	PHY_CT_RE_CFG	= 1<<9, /*                                       */
	PHY_CT_DUP_MD	= 1<<8, /*                     */
	PHY_CT_COL_TST	= 1<<7, /*                                */
	PHY_CT_SPS_MSB	= 1<<6, /*                                 */
};

enum {
	PHY_CT_SP1000	= PHY_CT_SPS_MSB, /*                           */
	PHY_CT_SP100	= PHY_CT_SPS_LSB, /*                           */
	PHY_CT_SP10	= 0,		  /*                           */
};

enum {
	PHY_ST_EXT_ST	= 1<<8, /*                                 */

	PHY_ST_PRE_SUP	= 1<<6, /*                              */
	PHY_ST_AN_OVER	= 1<<5, /*                               */
	PHY_ST_REM_FLT	= 1<<4, /*                                         */
	PHY_ST_AN_CAP	= 1<<3, /*                                     */
	PHY_ST_LSYNC	= 1<<2, /*                           */
	PHY_ST_JAB_DET	= 1<<1, /*                         */
	PHY_ST_EXT_REG	= 1<<0, /*                                     */
};

enum {
	PHY_I1_OUI_MSK	= 0x3f<<10, /*                                    */
	PHY_I1_MOD_NUM	= 0x3f<<4, /*                          */
	PHY_I1_REV_MSK	= 0xf, /*                             */
};

/*                            */
enum {
	PHY_BCOM_ID1_A1	= 0x6041,
	PHY_BCOM_ID1_B2 = 0x6043,
	PHY_BCOM_ID1_C0	= 0x6044,
	PHY_BCOM_ID1_C5	= 0x6047,
};

/*                           */
enum {
	PHY_MARV_ID0_VAL= 0x0141, /*                           */
	PHY_MARV_ID1_B0	= 0x0C23, /*                     */
	PHY_MARV_ID1_B2	= 0x0C25, /*                          */
	PHY_MARV_ID1_C2	= 0x0CC2, /*                        */
	PHY_MARV_ID1_Y2	= 0x0C91, /*                       */
};

/*                             */
enum {
	PHY_AN_NXT_PG	= 1<<15, /*                           */
	PHY_AN_ACK	= 1<<14, /*                                   */
	PHY_AN_RF	= 1<<13, /*                           */

	PHY_AN_PAUSE_ASYM = 1<<11,/*                            */
	PHY_AN_PAUSE_CAP = 1<<10, /*                       */
	PHY_AN_100BASE4	= 1<<9, /*                                   */
	PHY_AN_100FULL	= 1<<8, /*                                    */
	PHY_AN_100HALF	= 1<<7, /*                                    */
	PHY_AN_10FULL	= 1<<6, /*                                   */
	PHY_AN_10HALF	= 1<<5, /*                                   */
	PHY_AN_CSMA	= 1<<0, /*                                */
	PHY_AN_SEL	= 0x1f, /*                                         */
	PHY_AN_FULL	= PHY_AN_100FULL | PHY_AN_10FULL | PHY_AN_CSMA,
	PHY_AN_ALL	= PHY_AN_10HALF | PHY_AN_10FULL |
		  	  PHY_AN_100HALF | PHY_AN_100FULL,
};

/*               */
enum {
	PHY_X_AN_NXT_PG	= 1<<15, /*                           */
	PHY_X_AN_ACK	= 1<<14, /*                                   */
	PHY_X_AN_RFB	= 3<<12,/*                               */

	PHY_X_AN_PAUSE	= 3<<7,/*                        */
	PHY_X_AN_HD	= 1<<6, /*                     */
	PHY_X_AN_FD	= 1<<5, /*                     */
};

/*                                                         */
enum {
	PHY_X_P_NO_PAUSE= 0<<7,/*                          */
	PHY_X_P_SYM_MD	= 1<<7, /*                                 */
	PHY_X_P_ASYM_MD	= 2<<7,/*                                  */
	PHY_X_P_BOTH_MD	= 3<<7,/*                            */
};


/*                                                                */
enum {
	PHY_X_EX_FD	= 1<<15, /*                                     */
	PHY_X_EX_HD	= 1<<14, /*                                     */
};

/*                                                           */
enum {
	PHY_X_RS_PAUSE	= 3<<7,	/*                                */
	PHY_X_RS_HD	= 1<<6,	/*                                   */
	PHY_X_RS_FD	= 1<<5,	/*                                   */
	PHY_X_RS_ABLMIS = 1<<4,	/*                                      */
	PHY_X_RS_PAUMIS = 1<<3,	/*                                   */
};

/*                                           */
enum {
	X_RFB_OK	= 0<<12,/*                               */
	X_RFB_LF	= 1<<12,/*                         */
	X_RFB_OFF	= 2<<12,/*                    */
	X_RFB_AN_ERR	= 3<<12,/*                                   */
};

/*                   */
/*                                                                */
enum {
	PHY_B_1000C_TEST	= 7<<13,/*                        */
	PHY_B_1000C_MSE	= 1<<12, /*                             */
	PHY_B_1000C_MSC	= 1<<11, /*                           */
	PHY_B_1000C_RD	= 1<<10, /*                      */
	PHY_B_1000C_AFD	= 1<<9, /*                               */
	PHY_B_1000C_AHD	= 1<<8, /*                               */
};

/*                                                               */
/*                                                               */
enum {
	PHY_B_1000S_MSF	= 1<<15, /*                            */
	PHY_B_1000S_MSR	= 1<<14, /*                             */
	PHY_B_1000S_LRS	= 1<<13, /*                               */
	PHY_B_1000S_RRS	= 1<<12, /*                                */
	PHY_B_1000S_LP_FD	= 1<<11, /*                             */
	PHY_B_1000S_LP_HD	= 1<<10, /*                             */
									/*                     */
	PHY_B_1000S_IEC	= 0xff, /*                             */
};

/*                                                                */
enum {
	PHY_B_ES_X_FD_CAP	= 1<<15, /*                               */
	PHY_B_ES_X_HD_CAP	= 1<<14, /*                               */
	PHY_B_ES_T_FD_CAP	= 1<<13, /*                               */
	PHY_B_ES_T_HD_CAP	= 1<<12, /*                               */
};

/*                                                                  */
enum {
	PHY_B_PEC_MAC_PHY	= 1<<15, /*                             */
	PHY_B_PEC_DIS_CROSS	= 1<<14, /*                               */
	PHY_B_PEC_TX_DIS	= 1<<13, /*                            */
	PHY_B_PEC_INT_DIS	= 1<<12, /*                             */
	PHY_B_PEC_F_INT	= 1<<11, /*                         */
	PHY_B_PEC_BY_45	= 1<<10, /*                             */
	PHY_B_PEC_BY_SCR	= 1<<9, /*                          */
	PHY_B_PEC_BY_MLT3	= 1<<8, /*                             */
	PHY_B_PEC_BY_RXA	= 1<<7, /*                           */
	PHY_B_PEC_RES_SCR	= 1<<6, /*                         */
	PHY_B_PEC_EN_LTR	= 1<<5, /*                              */
	PHY_B_PEC_LED_ON	= 1<<4, /*                        */
	PHY_B_PEC_LED_OFF	= 1<<3, /*                         */
	PHY_B_PEC_EX_IPG	= 1<<2, /*                            */
	PHY_B_PEC_3_LED	= 1<<1, /*                             */
	PHY_B_PEC_HIGH_LA	= 1<<0, /*                            */
};

/*                                                                 */
enum {
	PHY_B_PES_CROSS_STAT	= 1<<13, /*                              */
	PHY_B_PES_INT_STAT	= 1<<12, /*                          */
	PHY_B_PES_RRS	= 1<<11, /*                               */
	PHY_B_PES_LRS	= 1<<10, /*                              */
	PHY_B_PES_LOCKED	= 1<<9, /*                */
	PHY_B_PES_LS	= 1<<8, /*                     */
	PHY_B_PES_RF	= 1<<7, /*                      */
	PHY_B_PES_CE_ER	= 1<<6, /*                           */
	PHY_B_PES_BAD_SSD	= 1<<5, /*                 */
	PHY_B_PES_BAD_ESD	= 1<<4, /*                 */
	PHY_B_PES_RX_ER	= 1<<3, /*                       */
	PHY_B_PES_TX_ER	= 1<<2, /*                        */
	PHY_B_PES_LOCK_ER	= 1<<1, /*                    */
	PHY_B_PES_MLT3_ER	= 1<<0, /*                         */
};

/*                                                                  */
/*                                                           */
enum {
	PHY_B_AN_RF	= 1<<13, /*                      */

	PHY_B_AN_ASP	= 1<<11, /*                          */
	PHY_B_AN_PC	= 1<<10, /*                       */
};


/*                                                            */
enum {
	PHY_B_FC_CTR	= 0xff, /*                                  */

/*                                                             */
	PHY_B_RC_LOC_MSK	= 0xff00, /*                                */
	PHY_B_RC_REM_MSK	= 0x00ff, /*                                 */

/*                                                             */
	PHY_B_AC_L_SQE		= 1<<15, /*                     */
	PHY_B_AC_LONG_PACK	= 1<<14, /*                         */
	PHY_B_AC_ER_CTRL	= 3<<12,/*                              */
									/*                  */
	PHY_B_AC_TX_TST	= 1<<10, /*                               */
									/*                      */
	PHY_B_AC_DIS_PRF	= 1<<7, /*                              */
									/*                  */
	PHY_B_AC_DIS_PM	= 1<<5, /*                              */
									/*                  */
	PHY_B_AC_DIAG	= 1<<3, /*                         */
};

/*                                                            */
enum {
	PHY_B_AS_AN_C	= 1<<15, /*                          */
	PHY_B_AS_AN_CA	= 1<<14, /*                         */
	PHY_B_AS_ANACK_D	= 1<<13, /*                       */
	PHY_B_AS_ANAB_D	= 1<<12, /*                           */
	PHY_B_AS_NPW	= 1<<11, /*                           */
	PHY_B_AS_AN_RES_MSK	= 7<<8,/*                   */
	PHY_B_AS_PDF	= 1<<7, /*                                */
	PHY_B_AS_RF	= 1<<6, /*                      */
	PHY_B_AS_ANP_R	= 1<<5, /*                          */
	PHY_B_AS_LP_ANAB	= 1<<4, /*                       */
	PHY_B_AS_LP_NPAB	= 1<<3, /*                              */
	PHY_B_AS_LS	= 1<<2, /*                     */
	PHY_B_AS_PRR	= 1<<1, /*                             */
	PHY_B_AS_PRT	= 1<<0, /*                             */
};
#define PHY_B_AS_PAUSE_MSK	(PHY_B_AS_PRR | PHY_B_AS_PRT)

/*                                                            */
/*                                                          */
enum {
	PHY_B_IS_PSE	= 1<<14, /*                         */
	PHY_B_IS_MDXI_SC	= 1<<13, /*                            */
	PHY_B_IS_HCT	= 1<<12, /*                           */
	PHY_B_IS_LCT	= 1<<11, /*                           */
	PHY_B_IS_AN_PR	= 1<<10, /*                       */
	PHY_B_IS_NO_HDCL	= 1<<9, /*                     */
	PHY_B_IS_NO_HDC	= 1<<8, /*                */
	PHY_B_IS_NEG_USHDC	= 1<<7, /*                               */
	PHY_B_IS_SCR_S_ER	= 1<<6, /*                              */
	PHY_B_IS_RRS_CHANGE	= 1<<5, /*                               */
	PHY_B_IS_LRS_CHANGE	= 1<<4, /*                              */
	PHY_B_IS_DUP_CHANGE	= 1<<3, /*                            */
	PHY_B_IS_LSP_CHANGE	= 1<<2, /*                           */
	PHY_B_IS_LST_CHANGE	= 1<<1, /*                             */
	PHY_B_IS_CRC_ER	= 1<<0, /*                   */
};
#define PHY_B_DEF_MSK	\
	(~(PHY_B_IS_PSE | PHY_B_IS_AN_PR | PHY_B_IS_DUP_CHANGE | \
	    PHY_B_IS_LSP_CHANGE | PHY_B_IS_LST_CHANGE))

/*                                                    */
enum {
	PHY_B_P_NO_PAUSE	= 0<<10,/*                           */
	PHY_B_P_SYM_MD	= 1<<10, /*                                  */
	PHY_B_P_ASYM_MD	= 2<<10,/*                                   */
	PHY_B_P_BOTH_MD	= 3<<10,/*                             */
};
/*
                                                                 
 */
enum {
	PHY_B_RES_1000FD	= 7<<8,/*                                 */
	PHY_B_RES_1000HD	= 6<<8,/*                                 */
};

/*                   */
enum {
	PHY_M_AN_NXT_PG	= 1<<15, /*                   */
	PHY_M_AN_ACK	= 1<<14, /*                           */
	PHY_M_AN_RF	= 1<<13, /*              */

	PHY_M_AN_ASP	= 1<<11, /*                  */
	PHY_M_AN_PC	= 1<<10, /*                       */
	PHY_M_AN_100_T4	= 1<<9, /*                                */
	PHY_M_AN_100_FD	= 1<<8, /*                                  */
	PHY_M_AN_100_HD	= 1<<7, /*                                  */
	PHY_M_AN_10_FD	= 1<<6, /*                                 */
	PHY_M_AN_10_HD	= 1<<5, /*                                 */
	PHY_M_AN_SEL_MSK =0x1f<<4,	/*                                 */
};

/*                                           */
enum {
	PHY_M_AN_ASP_X		= 1<<8, /*                  */
	PHY_M_AN_PC_X		= 1<<7, /*                       */
	PHY_M_AN_1000X_AHD	= 1<<6, /*                                   */
	PHY_M_AN_1000X_AFD	= 1<<5, /*                                   */
};

/*                                                        */
enum {
	PHY_M_P_NO_PAUSE_X	= 0<<7,/*                           */
	PHY_M_P_SYM_MD_X	= 1<<7, /*                                  */
	PHY_M_P_ASYM_MD_X	= 2<<7,/*                                   */
	PHY_M_P_BOTH_MD_X	= 3<<7,/*                             */
};

/*                                                                */
enum {
	PHY_M_1000C_TEST= 7<<13,/*                        */
	PHY_M_1000C_MSE	= 1<<12, /*                            */
	PHY_M_1000C_MSC	= 1<<11, /*                              */
	PHY_M_1000C_MPD	= 1<<10, /*                   */
	PHY_M_1000C_AFD	= 1<<9, /*                       */
	PHY_M_1000C_AHD	= 1<<8, /*                       */
};

/*                                                             */
enum {
	PHY_M_PC_TX_FFD_MSK	= 3<<14,/*                                */
	PHY_M_PC_RX_FFD_MSK	= 3<<12,/*                                */
	PHY_M_PC_ASS_CRS_TX	= 1<<11, /*                        */
	PHY_M_PC_FL_GOOD	= 1<<10, /*                 */
	PHY_M_PC_EN_DET_MSK	= 3<<8,/*                                */
	PHY_M_PC_ENA_EXT_D	= 1<<7, /*                             */
	PHY_M_PC_MDIX_MSK	= 3<<5,/*                                   */
	PHY_M_PC_DIS_125CLK	= 1<<4, /*                 */
	PHY_M_PC_MAC_POW_UP	= 1<<3, /*              */
	PHY_M_PC_SQE_T_ENA	= 1<<2, /*                  */
	PHY_M_PC_POL_R_DIS	= 1<<1, /*                            */
	PHY_M_PC_DIS_JABBER	= 1<<0, /*                */
};

enum {
	PHY_M_PC_EN_DET		= 2<<8,	/*                        */
	PHY_M_PC_EN_DET_PLUS	= 3<<8, /*                             */
};

enum {
	PHY_M_PC_MAN_MDI	= 0, /*                               */
	PHY_M_PC_MAN_MDIX	= 1, /*                                */
	PHY_M_PC_ENA_AUTO	= 3, /*                                 */
};

/*                                             */
enum {
	PHY_M_PC_ENA_DTE_DT	= 1<<15, /*                                        */
	PHY_M_PC_ENA_ENE_DT	= 1<<14, /*                                      */
	PHY_M_PC_DIS_NLP_CK	= 1<<13, /*                                      */
	PHY_M_PC_ENA_LIP_NP	= 1<<12, /*                                    */
	PHY_M_PC_DIS_NLP_GN	= 1<<11, /*                                     */

	PHY_M_PC_DIS_SCRAMB	= 1<<9, /*                   */
	PHY_M_PC_DIS_FEFI	= 1<<8, /*                                     */

	PHY_M_PC_SH_TP_SEL	= 1<<6, /*                              */
	PHY_M_PC_RX_FD_MSK	= 3<<2,/*                                */
};

/*                                                               */
enum {
	PHY_M_PS_SPEED_MSK	= 3<<14, /*                        */
	PHY_M_PS_SPEED_1000	= 1<<15, /*                 */
	PHY_M_PS_SPEED_100	= 1<<14, /*                 */
	PHY_M_PS_SPEED_10	= 0,	 /*                 */
	PHY_M_PS_FULL_DUP	= 1<<13, /*             */
	PHY_M_PS_PAGE_REC	= 1<<12, /*               */
	PHY_M_PS_SPDUP_RES	= 1<<11, /*                         */
	PHY_M_PS_LINK_UP	= 1<<10, /*         */
	PHY_M_PS_CABLE_MSK	= 7<<7,  /*                               */
	PHY_M_PS_MDI_X_STAT	= 1<<6,  /*                             */
	PHY_M_PS_DOWNS_STAT	= 1<<5,  /*                              */
	PHY_M_PS_ENDET_STAT	= 1<<4,  /*                              */
	PHY_M_PS_TX_P_EN	= 1<<3,  /*                  */
	PHY_M_PS_RX_P_EN	= 1<<2,  /*                  */
	PHY_M_PS_POL_REV	= 1<<1,  /*                   */
	PHY_M_PS_JABBER		= 1<<0,  /*        */
};

#define PHY_M_PS_PAUSE_MSK	(PHY_M_PS_TX_P_EN | PHY_M_PS_RX_P_EN)

/*                                             */
enum {
	PHY_M_PS_DTE_DETECT	= 1<<15, /*                                        */
	PHY_M_PS_RES_SPEED	= 1<<14, /*                                       */
};

enum {
	PHY_M_IS_AN_ERROR	= 1<<15, /*                        */
	PHY_M_IS_LSP_CHANGE	= 1<<14, /*                    */
	PHY_M_IS_DUP_CHANGE	= 1<<13, /*                     */
	PHY_M_IS_AN_PR		= 1<<12, /*               */
	PHY_M_IS_AN_COMPL	= 1<<11, /*                            */
	PHY_M_IS_LST_CHANGE	= 1<<10, /*                     */
	PHY_M_IS_SYMB_ERROR	= 1<<9, /*              */
	PHY_M_IS_FALSE_CARR	= 1<<8, /*               */
	PHY_M_IS_FIFO_ERROR	= 1<<7, /*                              */
	PHY_M_IS_MDI_CHANGE	= 1<<6, /*                       */
	PHY_M_IS_DOWNSH_DET	= 1<<5, /*                    */
	PHY_M_IS_END_CHANGE	= 1<<4, /*                       */

	PHY_M_IS_DTE_CHANGE	= 1<<2, /*                               */
	PHY_M_IS_POL_CHANGE	= 1<<1, /*                  */
	PHY_M_IS_JABBER		= 1<<0, /*        */

	PHY_M_IS_DEF_MSK	= PHY_M_IS_AN_ERROR | PHY_M_IS_LSP_CHANGE |
				  PHY_M_IS_LST_CHANGE | PHY_M_IS_FIFO_ERROR,

	PHY_M_IS_AN_MSK		= PHY_M_IS_AN_ERROR | PHY_M_IS_AN_COMPL,
};

/*                                                              */
enum {
	PHY_M_EC_ENA_BC_EXT = 1<<15, /*                                        */
	PHY_M_EC_ENA_LIN_LB = 1<<14, /*                                     */

	PHY_M_EC_DIS_LINK_P = 1<<12, /*                                    */
	PHY_M_EC_M_DSC_MSK  = 3<<10, /*                                      */
					/*                */
	PHY_M_EC_S_DSC_MSK  = 3<<8,  /*                                      */
				       /*                */
	PHY_M_EC_M_DSC_MSK2  = 7<<9, /*                                      */
					/*                */
	PHY_M_EC_DOWN_S_ENA  = 1<<8, /*                                 */
					/*                                   */
	PHY_M_EC_RX_TIM_CT   = 1<<7, /*                        */
	PHY_M_EC_MAC_S_MSK   = 7<<4, /*                                      */
	PHY_M_EC_FIB_AN_ENA  = 1<<3, /*                                        */
	PHY_M_EC_DTE_D_ENA   = 1<<2, /*                                  */
	PHY_M_EC_TX_TIM_CT   = 1<<1, /*                         */
	PHY_M_EC_TRANS_DIS   = 1<<0, /*                                    */};

#define PHY_M_EC_M_DSC(x)	((u16)(x)<<10) /*                            */
#define PHY_M_EC_S_DSC(x)	((u16)(x)<<8) /*                             */
#define PHY_M_EC_MAC_S(x)	((u16)(x)<<4) /*                              */

#define PHY_M_EC_M_DSC_2(x)	((u16)(x)<<9) /*                                */
											/*                                */
enum {
	MAC_TX_CLK_0_MHZ	= 2,
	MAC_TX_CLK_2_5_MHZ	= 6,
	MAC_TX_CLK_25_MHZ 	= 7,
};

/*                                                       */
enum {
	PHY_M_LEDC_DIS_LED	= 1<<15, /*             */
	PHY_M_LEDC_PULS_MSK	= 7<<12,/*                                */
	PHY_M_LEDC_F_INT	= 1<<11, /*                 */
	PHY_M_LEDC_BL_R_MSK	= 7<<8,/*                             */
	PHY_M_LEDC_DP_C_LSB	= 1<<7, /*                                    */
	PHY_M_LEDC_TX_C_LSB	= 1<<6, /*                                */
	PHY_M_LEDC_LK_C_MSK	= 7<<3,/*                               */
					/*                */
};
#define PHY_M_LED_PULS_DUR(x)	(((u16)(x)<<12) & PHY_M_LEDC_PULS_MSK)
#define PHY_M_LED_BLINK_RT(x)	(((u16)(x)<<8) & PHY_M_LEDC_BL_R_MSK)

enum {
	PHY_M_LEDC_LINK_MSK	= 3<<3, /*                               */
					/*                */
	PHY_M_LEDC_DP_CTRL	= 1<<2, /*                */
	PHY_M_LEDC_DP_C_MSB	= 1<<2, /*                                    */
	PHY_M_LEDC_RX_CTRL	= 1<<1, /*                    */
	PHY_M_LEDC_TX_CTRL	= 1<<0, /*                    */
	PHY_M_LEDC_TX_C_MSB	= 1<<0, /*                                */
};

enum {
	PULS_NO_STR	= 0, /*                     */
	PULS_21MS	= 1, /*                */
	PULS_42MS	= 2, /*                */
	PULS_84MS	= 3, /*                 */
	PULS_170MS	= 4, /*                  */
	PULS_340MS	= 5, /*                  */
	PULS_670MS	= 6, /*                 */
	PULS_1300MS	= 7, /*                */
};


enum {
	BLINK_42MS	= 0, /*       */
	BLINK_84MS	= 1, /*       */
	BLINK_170MS	= 2, /*        */
	BLINK_340MS	= 3, /*        */
	BLINK_670MS	= 4, /*        */
};

/*                                                               */
#define PHY_M_LED_MO_SGMII(x)	((x)<<14) /*                             */
										/*                      */
#define PHY_M_LED_MO_DUP(x)	((x)<<10) /*                     */
#define PHY_M_LED_MO_10(x)	((x)<<8) /*                      */
#define PHY_M_LED_MO_100(x)	((x)<<6) /*                       */
#define PHY_M_LED_MO_1000(x)	((x)<<4) /*                        */
#define PHY_M_LED_MO_RX(x)	((x)<<2) /*                 */
#define PHY_M_LED_MO_TX(x)	((x)<<0) /*                 */

enum {
	MO_LED_NORM	= 0,
	MO_LED_BLINK	= 1,
	MO_LED_OFF	= 2,
	MO_LED_ON	= 3,
};

/*                                                                  */
enum {
	PHY_M_EC2_FI_IMPED	= 1<<6, /*                        */
	PHY_M_EC2_FO_IMPED	= 1<<5, /*                        */
	PHY_M_EC2_FO_M_CLK	= 1<<4, /*                         */
	PHY_M_EC2_FO_BOOST	= 1<<3, /*                    */
	PHY_M_EC2_FO_AM_MSK	= 7, /*                                    */
};

/*                                                                  */
enum {
	PHY_M_FC_AUTO_SEL	= 1<<15, /*                             */
	PHY_M_FC_AN_REG_ACC	= 1<<14, /*                             */
	PHY_M_FC_RESOLUTION	= 1<<13, /*                         */
	PHY_M_SER_IF_AN_BP	= 1<<12, /*                          */
	PHY_M_SER_IF_BP_ST	= 1<<11, /*                          */
	PHY_M_IRQ_POLARITY	= 1<<10, /*              */
	PHY_M_DIS_AUT_MED	= 1<<9, /*                                    */
									/*                */
								/*                                     */
	PHY_M_UNDOC1	= 1<<7, /*                     */
	PHY_M_DTE_POW_STAT	= 1<<4, /*                                 */
	PHY_M_MODE_MASK	= 0xf, /*                                     */
};

/*                                                              */
enum {
	PHY_M_CABD_ENA_TEST	= 1<<15, /*                      */
	PHY_M_CABD_DIS_WAIT	= 1<<15, /*                                 */
					/*                */
	PHY_M_CABD_STAT_MSK	= 3<<13, /*                         */
	PHY_M_CABD_AMPL_MSK	= 0x1f<<8, /*                            */
					/*                */
	PHY_M_CABD_DIST_MSK	= 0xff, /*                           */
};

/*                                                                        */
enum {
	CABD_STAT_NORMAL= 0,
	CABD_STAT_SHORT	= 1,
	CABD_STAT_OPEN	= 2,
	CABD_STAT_FAIL	= 3,
};

/*                                             */
/*                                                                   */
									/*                                        */
enum {
	PHY_M_FELP_LED2_MSK = 0xf<<8,	/*                              */
	PHY_M_FELP_LED1_MSK = 0xf<<4,	/*                             */
	PHY_M_FELP_LED0_MSK = 0xf, /*                               */
};

#define PHY_M_FELP_LED2_CTRL(x)	(((x)<<8) & PHY_M_FELP_LED2_MSK)
#define PHY_M_FELP_LED1_CTRL(x)	(((x)<<4) & PHY_M_FELP_LED1_MSK)
#define PHY_M_FELP_LED0_CTRL(x)	(((x)<<0) & PHY_M_FELP_LED0_MSK)

enum {
	LED_PAR_CTRL_COLX	= 0x00,
	LED_PAR_CTRL_ERROR	= 0x01,
	LED_PAR_CTRL_DUPLEX	= 0x02,
	LED_PAR_CTRL_DP_COL	= 0x03,
	LED_PAR_CTRL_SPEED	= 0x04,
	LED_PAR_CTRL_LINK	= 0x05,
	LED_PAR_CTRL_TX		= 0x06,
	LED_PAR_CTRL_RX		= 0x07,
	LED_PAR_CTRL_ACT	= 0x08,
	LED_PAR_CTRL_LNK_RX	= 0x09,
	LED_PAR_CTRL_LNK_AC	= 0x0a,
	LED_PAR_CTRL_ACT_BL	= 0x0b,
	LED_PAR_CTRL_TX_BL	= 0x0c,
	LED_PAR_CTRL_RX_BL	= 0x0d,
	LED_PAR_CTRL_COL_BL	= 0x0e,
	LED_PAR_CTRL_INACT	= 0x0f
};

/*                                                                */
enum {
	PHY_M_FESC_DIS_WAIT	= 1<<2, /*                            */
	PHY_M_FESC_ENA_MCLK	= 1<<1, /*                                   */
	PHY_M_FESC_SEL_CL_A	= 1<<0, /*                                 */
};


/*                                                                  */
enum {
	PHY_M_LEDC_LOS_MSK	= 0xf<<12, /*                                */
	PHY_M_LEDC_INIT_MSK	= 0xf<<8, /*                                 */
	PHY_M_LEDC_STA1_MSK	= 0xf<<4, /*                                  */
	PHY_M_LEDC_STA0_MSK	= 0xf, /*                                  */
};

#define PHY_M_LEDC_LOS_CTRL(x)	(((x)<<12) & PHY_M_LEDC_LOS_MSK)
#define PHY_M_LEDC_INIT_CTRL(x)	(((x)<<8) & PHY_M_LEDC_INIT_MSK)
#define PHY_M_LEDC_STA1_CTRL(x)	(((x)<<4) & PHY_M_LEDC_STA1_MSK)
#define PHY_M_LEDC_STA0_CTRL(x)	(((x)<<0) & PHY_M_LEDC_STA0_MSK)

/*                 */
/*                */
enum {
	GM_GP_STAT	= 0x0000,	/*                                   */
	GM_GP_CTRL	= 0x0004,	/*                                    */
	GM_TX_CTRL	= 0x0008,	/*                                  */
	GM_RX_CTRL	= 0x000c,	/*                                 */
	GM_TX_FLOW_CTRL	= 0x0010,	/*                                  */
	GM_TX_PARAM	= 0x0014,	/*                                    */
	GM_SERIAL_MODE	= 0x0018,	/*                                 */
/*                          */
	GM_SRC_ADDR_1L	= 0x001c,	/*                                   */
	GM_SRC_ADDR_1M	= 0x0020,	/*                                      */
	GM_SRC_ADDR_1H	= 0x0024,	/*                                    */
	GM_SRC_ADDR_2L	= 0x0028,	/*                                   */
	GM_SRC_ADDR_2M	= 0x002c,	/*                                      */
	GM_SRC_ADDR_2H	= 0x0030,	/*                                    */

/*                                  */
	GM_MC_ADDR_H1	= 0x0034,	/*                                     */
	GM_MC_ADDR_H2	= 0x0038,	/*                                     */
	GM_MC_ADDR_H3	= 0x003c,	/*                                     */
	GM_MC_ADDR_H4	= 0x0040,	/*                                     */

/*                            */
	GM_TX_IRQ_SRC	= 0x0044,	/*                                   */
	GM_RX_IRQ_SRC	= 0x0048,	/*                                   */
	GM_TR_IRQ_SRC	= 0x004c,	/*                                   */

/*                          */
	GM_TX_IRQ_MSK	= 0x0050,	/*                                 */
	GM_RX_IRQ_MSK	= 0x0054,	/*                                 */
	GM_TR_IRQ_MSK	= 0x0058,	/*                                 */

/*                                             */
	GM_SMI_CTRL	= 0x0080,	/*                                 */
	GM_SMI_DATA	= 0x0084,	/*                              */
	GM_PHY_ADDR	= 0x0088,	/*                                  */
};

/*              */
#define GM_MIB_CNT_BASE	0x0100		/*                              */
#define GM_MIB_CNT_SIZE	44		/*                        */

/*
                                                     
                                                    
 */
enum {
	GM_RXF_UC_OK  = GM_MIB_CNT_BASE + 0,	/*                            */
	GM_RXF_BC_OK	= GM_MIB_CNT_BASE + 8,	/*                              */
	GM_RXF_MPAUSE	= GM_MIB_CNT_BASE + 16,	/*                                */
	GM_RXF_MC_OK	= GM_MIB_CNT_BASE + 24,	/*                              */
	GM_RXF_FCS_ERR	= GM_MIB_CNT_BASE + 32,	/*                           */
	/*                                */
	GM_RXO_OK_LO	= GM_MIB_CNT_BASE + 48,	/*                        */
	GM_RXO_OK_HI	= GM_MIB_CNT_BASE + 56,	/*                         */
	GM_RXO_ERR_LO	= GM_MIB_CNT_BASE + 64,	/*                             */
	GM_RXO_ERR_HI	= GM_MIB_CNT_BASE + 72,	/*                              */
	GM_RXF_SHT	= GM_MIB_CNT_BASE + 80,	/*                             */
	GM_RXE_FRAG	= GM_MIB_CNT_BASE + 88,	/*                                       */
	GM_RXF_64B	= GM_MIB_CNT_BASE + 96,	/*                  */
	GM_RXF_127B	= GM_MIB_CNT_BASE + 104,	/*                      */
	GM_RXF_255B	= GM_MIB_CNT_BASE + 112,	/*                       */
	GM_RXF_511B	= GM_MIB_CNT_BASE + 120,	/*                       */
	GM_RXF_1023B	= GM_MIB_CNT_BASE + 128,	/*                        */
	GM_RXF_1518B	= GM_MIB_CNT_BASE + 136,	/*                         */
	GM_RXF_MAX_SZ	= GM_MIB_CNT_BASE + 144,	/*                            */
	GM_RXF_LNG_ERR	= GM_MIB_CNT_BASE + 152,	/*                         */
	GM_RXF_JAB_PKT	= GM_MIB_CNT_BASE + 160,	/*                        */
	/*                                 */
	GM_RXE_FIFO_OV	= GM_MIB_CNT_BASE + 176,	/*                        */
	/*                                 */
	GM_TXF_UC_OK	= GM_MIB_CNT_BASE + 192,	/*                           */
	GM_TXF_BC_OK	= GM_MIB_CNT_BASE + 200,	/*                             */
	GM_TXF_MPAUSE	= GM_MIB_CNT_BASE + 208,	/*                               */
	GM_TXF_MC_OK	= GM_MIB_CNT_BASE + 216,	/*                             */
	GM_TXO_OK_LO	= GM_MIB_CNT_BASE + 224,	/*                           */
	GM_TXO_OK_HI	= GM_MIB_CNT_BASE + 232,	/*                            */
	GM_TXF_64B	= GM_MIB_CNT_BASE + 240,	/*                  */
	GM_TXF_127B	= GM_MIB_CNT_BASE + 248,	/*                      */
	GM_TXF_255B	= GM_MIB_CNT_BASE + 256,	/*                       */
	GM_TXF_511B	= GM_MIB_CNT_BASE + 264,	/*                       */
	GM_TXF_1023B	= GM_MIB_CNT_BASE + 272,	/*                        */
	GM_TXF_1518B	= GM_MIB_CNT_BASE + 280,	/*                         */
	GM_TXF_MAX_SZ	= GM_MIB_CNT_BASE + 288,	/*                            */

	GM_TXF_COL	= GM_MIB_CNT_BASE + 304,	/*              */
	GM_TXF_LAT_COL	= GM_MIB_CNT_BASE + 312,	/*                   */
	GM_TXF_ABO_COL	= GM_MIB_CNT_BASE + 320,	/*                               */
	GM_TXF_MUL_COL	= GM_MIB_CNT_BASE + 328,	/*                       */
	GM_TXF_SNG_COL	= GM_MIB_CNT_BASE + 336,	/*                     */
	GM_TXE_FIFO_UR	= GM_MIB_CNT_BASE + 344,	/*                        */
};

/*                      */
/*                                                       */
enum {
	GM_GPSR_SPEED		= 1<<15, /*                                   */
	GM_GPSR_DUPLEX		= 1<<14, /*                                */
	GM_GPSR_FC_TX_DIS	= 1<<13, /*                                       */
	GM_GPSR_LINK_UP		= 1<<12, /*                        */
	GM_GPSR_PAUSE		= 1<<11, /*                     */
	GM_GPSR_TX_ACTIVE	= 1<<10, /*                        */
	GM_GPSR_EXC_COL		= 1<<9,	/*                                       */
	GM_GPSR_LAT_COL		= 1<<8,	/*                                  */

	GM_GPSR_PHY_ST_CH	= 1<<5,	/*                           */
	GM_GPSR_GIG_SPEED	= 1<<4,	/*                                       */
	GM_GPSR_PART_MODE	= 1<<3,	/*                        */
	GM_GPSR_FC_RX_DIS	= 1<<2,	/*                                       */
	GM_GPSR_PROM_EN		= 1<<1,	/*                                  */
};

/*                                                        */
enum {
	GM_GPCR_PROM_ENA	= 1<<14,	/*                                 */
	GM_GPCR_FC_TX_DIS	= 1<<13, /*                                      */
	GM_GPCR_TX_ENA		= 1<<12, /*                         */
	GM_GPCR_RX_ENA		= 1<<11, /*                        */
	GM_GPCR_BURST_ENA	= 1<<10, /*                           */
	GM_GPCR_LOOP_ENA	= 1<<9,	/*                                  */
	GM_GPCR_PART_ENA	= 1<<8,	/*                               */
	GM_GPCR_GIGS_ENA	= 1<<7,	/*                                   */
	GM_GPCR_FL_PASS		= 1<<6,	/*                         */
	GM_GPCR_DUP_FULL	= 1<<5,	/*                          */
	GM_GPCR_FC_RX_DIS	= 1<<4,	/*                                      */
	GM_GPCR_SPEED_100	= 1<<3,   /*                             */
	GM_GPCR_AU_DUP_DIS	= 1<<2,	/*                                    */
	GM_GPCR_AU_FCT_DIS	= 1<<1,	/*                                     */
	GM_GPCR_AU_SPD_DIS	= 1<<0,	/*                                   */
};

#define GM_GPCR_SPEED_1000	(GM_GPCR_GIGS_ENA | GM_GPCR_SPEED_100)
#define GM_GPCR_AU_ALL_DIS	(GM_GPCR_AU_DUP_DIS | GM_GPCR_AU_FCT_DIS|GM_GPCR_AU_SPD_DIS)

/*                                                   */
enum {
	GM_TXCR_FORCE_JAM	= 1<<15, /*                                  */
	GM_TXCR_CRC_DIS		= 1<<14, /*                                  */
	GM_TXCR_PAD_DIS		= 1<<13, /*                                    */
	GM_TXCR_COL_THR_MSK	= 7<<10, /*                                 */
};

#define TX_COL_THR(x)		(((x)<<10) & GM_TXCR_COL_THR_MSK)
#define TX_COL_DEF		0x04	/*                              */

/*                                                  */
enum {
	GM_RXCR_UCF_ENA	= 1<<15, /*                                  */
	GM_RXCR_MCF_ENA	= 1<<14, /*                                    */
	GM_RXCR_CRC_DIS	= 1<<13, /*                           */
	GM_RXCR_PASS_FC	= 1<<12, /*                                 */
};

/*                                                     */
enum {
	GM_TXPA_JAMLEN_MSK	= 0x03<<14,	/*                        */
	GM_TXPA_JAMIPG_MSK	= 0x1f<<9,	/*                    */
	GM_TXPA_JAMDAT_MSK	= 0x1f<<4,	/*                            */

	TX_JAM_LEN_DEF		= 0x03,
	TX_JAM_IPG_DEF		= 0x0b,
	TX_IPG_JAM_DEF		= 0x1c,
};

#define TX_JAM_LEN_VAL(x)	(((x)<<14) & GM_TXPA_JAMLEN_MSK)
#define TX_JAM_IPG_VAL(x)	(((x)<<9)  & GM_TXPA_JAMIPG_MSK)
#define TX_IPG_JAM_DATA(x)	(((x)<<4)  & GM_TXPA_JAMDAT_MSK)


/*                                                  */
enum {
	GM_SMOD_DATABL_MSK	= 0x1f<<11, /*                                */
	GM_SMOD_LIMIT_4		= 1<<10, /*                                 */
	GM_SMOD_VLAN_ENA	= 1<<9,	/*                                       */
	GM_SMOD_JUMBO_ENA	= 1<<8,	/*                                       */
	 GM_SMOD_IPG_MSK	= 0x1f	/*                                  */
};

#define DATA_BLIND_VAL(x)	(((x)<<11) & GM_SMOD_DATABL_MSK)
#define DATA_BLIND_DEF		0x04

#define IPG_DATA_VAL(x)		(x & GM_SMOD_IPG_MSK)
#define IPG_DATA_DEF		0x1e

/*                                               */
enum {
	GM_SMI_CT_PHY_A_MSK	= 0x1f<<11, /*                                */
	GM_SMI_CT_REG_A_MSK	= 0x1f<<6, /*                                  */
	GM_SMI_CT_OP_RD		= 1<<5,	/*                              */
	GM_SMI_CT_RD_VAL	= 1<<4,	/*                                     */
	GM_SMI_CT_BUSY		= 1<<3,	/*                                      */
};

#define GM_SMI_CT_PHY_AD(x)	(((x)<<11) & GM_SMI_CT_PHY_A_MSK)
#define GM_SMI_CT_REG_AD(x)	(((x)<<6) & GM_SMI_CT_REG_A_MSK)

/*                                                 */
enum {
	GM_PAR_MIB_CLR	= 1<<5,	/*                                    */
	GM_PAR_MIB_TST	= 1<<4,	/*                                      */
};

/*                               */
enum {
	GMR_FS_LEN	= 0xffff<<16, /*                             */
	GMR_FS_LEN_SHIFT = 16,
	GMR_FS_VLAN	= 1<<13, /*                     */
	GMR_FS_JABBER	= 1<<12, /*                       */
	GMR_FS_UN_SIZE	= 1<<11, /*                          */
	GMR_FS_MC	= 1<<10, /*                          */
	GMR_FS_BC	= 1<<9, /*                          */
	GMR_FS_RX_OK	= 1<<8, /*                                  */
	GMR_FS_GOOD_FC	= 1<<7, /*                                  */
	GMR_FS_BAD_FC	= 1<<6, /*                                  */
	GMR_FS_MII_ERR	= 1<<5, /*                   */
	GMR_FS_LONG_ERR	= 1<<4, /*                         */
	GMR_FS_FRAGMENT	= 1<<3, /*                  */

	GMR_FS_CRC_ERR	= 1<<1, /*                   */
	GMR_FS_RX_FF_OV	= 1<<0, /*                          */

/*
                                               
 */
	GMR_FS_ANY_ERR	= GMR_FS_CRC_ERR | GMR_FS_LONG_ERR |
		  	  GMR_FS_MII_ERR | GMR_FS_BAD_FC | GMR_FS_GOOD_FC |
			  GMR_FS_JABBER,
/*                                   */
	RX_FF_FL_DEF_MSK = GMR_FS_CRC_ERR | GMR_FS_RX_FF_OV |GMR_FS_MII_ERR |
			   GMR_FS_BAD_FC |  GMR_FS_UN_SIZE | GMR_FS_JABBER,
};

/*                                                */
enum {
	GMF_WP_TST_ON	= 1<<14,	/*                       */
	GMF_WP_TST_OFF	= 1<<13,	/*                        */
	GMF_WP_STEP	= 1<<12,	/*                              */

	GMF_RP_TST_ON	= 1<<10,	/*                      */
	GMF_RP_TST_OFF	= 1<<9,		/*                       */
	GMF_RP_STEP	= 1<<8,		/*                             */
	GMF_RX_F_FL_ON	= 1<<7,		/*                       */
	GMF_RX_F_FL_OFF	= 1<<6,		/*                        */
	GMF_CLI_RX_FO	= 1<<5,		/*                           */
	GMF_CLI_RX_FC	= 1<<4,		/*                             */
	GMF_OPER_ON	= 1<<3,		/*                     */
	GMF_OPER_OFF	= 1<<2,		/*                      */
	GMF_RST_CLR	= 1<<1,		/*                       */
	GMF_RST_SET	= 1<<0,		/*                       */

	RX_GMF_FL_THR_DEF = 0xa,	/*                           */
};


/*                                                */
enum {
	GMF_WSP_TST_ON	= 1<<18, /*                              */
	GMF_WSP_TST_OFF	= 1<<17, /*                               */
	GMF_WSP_STEP	= 1<<16, /*                                     */

	GMF_CLI_TX_FU	= 1<<6,	/*                            */
	GMF_CLI_TX_FC	= 1<<5,	/*                             */
	GMF_CLI_TX_PE	= 1<<4,	/*                           */
};

/*                                                               */
enum {
	GMT_ST_START	= 1<<2,	/*                        */
	GMT_ST_STOP	= 1<<1,	/*                        */
	GMT_ST_CLR_IRQ	= 1<<0,	/*                            */
};

/*                                                 */
enum {
	GMC_H_BURST_ON	= 1<<7,	/*                           */
	GMC_H_BURST_OFF	= 1<<6,	/*                            */
	GMC_F_LOOPB_ON	= 1<<5,	/*                  */
	GMC_F_LOOPB_OFF	= 1<<4,	/*                   */
	GMC_PAUSE_ON	= 1<<3,	/*          */
	GMC_PAUSE_OFF	= 1<<2,	/*           */
	GMC_RST_CLR	= 1<<1,	/*                  */
	GMC_RST_SET	= 1<<0,	/*                  */
};

/*                                                 */
enum {
	GPC_SEL_BDT	= 1<<28, /*                                      */
	GPC_INT_POL_HI	= 1<<27, /*                             */
	GPC_75_OHM	= 1<<26, /*                                      */
	GPC_DIS_FC	= 1<<25, /*                                          */
	GPC_DIS_SLEEP	= 1<<24, /*                       */
	GPC_HWCFG_M_3	= 1<<23, /*               */
	GPC_HWCFG_M_2	= 1<<22, /*               */
	GPC_HWCFG_M_1	= 1<<21, /*               */
	GPC_HWCFG_M_0	= 1<<20, /*               */
	GPC_ANEG_0	= 1<<19, /*         */
	GPC_ENA_XC	= 1<<18, /*                      */
	GPC_DIS_125	= 1<<17, /*                       */
	GPC_ANEG_3	= 1<<16, /*         */
	GPC_ANEG_2	= 1<<15, /*         */
	GPC_ANEG_1	= 1<<14, /*         */
	GPC_ENA_PAUSE	= 1<<13, /*                           */
	GPC_PHYADDR_4	= 1<<12, /*                   */
	GPC_PHYADDR_3	= 1<<11, /*                   */
	GPC_PHYADDR_2	= 1<<10, /*                   */
	GPC_PHYADDR_1	= 1<<9,	 /*                   */
	GPC_PHYADDR_0	= 1<<8,	 /*                   */
						/*                      */
	GPC_RST_CLR	= 1<<1,	/*                  */
	GPC_RST_SET	= 1<<0,	/*                  */
};

#define GPC_HWCFG_GMII_COP (GPC_HWCFG_M_3|GPC_HWCFG_M_2 | GPC_HWCFG_M_1 | GPC_HWCFG_M_0)
#define GPC_HWCFG_GMII_FIB (GPC_HWCFG_M_2 | GPC_HWCFG_M_1 | GPC_HWCFG_M_0)
#define GPC_ANEG_ADV_ALL_M  (GPC_ANEG_3 | GPC_ANEG_2 | GPC_ANEG_1 | GPC_ANEG_0)

/*                                                               */
#define GPC_FRC10MBIT_HALF	0
#define GPC_FRC10MBIT_FULL	GPC_ANEG_0
#define GPC_FRC100MBIT_HALF	GPC_ANEG_1
#define GPC_FRC100MBIT_FULL	(GPC_ANEG_0 | GPC_ANEG_1)

/*                                                 */
/*                                                  */
#define GPC_ADV_1000_HALF	GPC_ANEG_2
#define GPC_ADV_1000_FULL	GPC_ANEG_3
#define GPC_ADV_ALL		(GPC_ANEG_2 | GPC_ANEG_3)

/*                       */
/*                                */
#define GPC_FORCE_MASTER	0
#define GPC_FORCE_SLAVE		GPC_ANEG_0
#define GPC_PREF_MASTER		GPC_ANEG_1
#define GPC_PREF_SLAVE		(GPC_ANEG_1 | GPC_ANEG_0)

/*                                                            */
/*                                                            */
enum {
	GM_IS_TX_CO_OV	= 1<<5,	/*                               */
	GM_IS_RX_CO_OV	= 1<<4,	/*                              */
	GM_IS_TX_FF_UR	= 1<<3,	/*                        */
	GM_IS_TX_COMPL	= 1<<2,	/*                             */
	GM_IS_RX_FF_OR	= 1<<1,	/*                      */
	GM_IS_RX_COMPL	= 1<<0,	/*                          */

#define GMAC_DEF_MSK	(GM_IS_RX_FF_OR | GM_IS_TX_FF_UR)

/*                                                          */
						/*                       */
	GMLC_RST_CLR	= 1<<1,	/*                       */
	GMLC_RST_SET	= 1<<0,	/*                       */


/*                                             */
	WOL_CTL_LINK_CHG_OCC		= 1<<15,
	WOL_CTL_MAGIC_PKT_OCC		= 1<<14,
	WOL_CTL_PATTERN_OCC		= 1<<13,
	WOL_CTL_CLEAR_RESULT		= 1<<12,
	WOL_CTL_ENA_PME_ON_LINK_CHG	= 1<<11,
	WOL_CTL_DIS_PME_ON_LINK_CHG	= 1<<10,
	WOL_CTL_ENA_PME_ON_MAGIC_PKT	= 1<<9,
	WOL_CTL_DIS_PME_ON_MAGIC_PKT	= 1<<8,
	WOL_CTL_ENA_PME_ON_PATTERN	= 1<<7,
	WOL_CTL_DIS_PME_ON_PATTERN	= 1<<6,
	WOL_CTL_ENA_LINK_CHG_UNIT	= 1<<5,
	WOL_CTL_DIS_LINK_CHG_UNIT	= 1<<4,
	WOL_CTL_ENA_MAGIC_PKT_UNIT	= 1<<3,
	WOL_CTL_DIS_MAGIC_PKT_UNIT	= 1<<2,
	WOL_CTL_ENA_PATTERN_UNIT	= 1<<1,
	WOL_CTL_DIS_PATTERN_UNIT	= 1<<0,
};

#define WOL_CTL_DEFAULT				\
	(WOL_CTL_DIS_PME_ON_LINK_CHG |	\
	WOL_CTL_DIS_PME_ON_PATTERN |	\
	WOL_CTL_DIS_PME_ON_MAGIC_PKT |	\
	WOL_CTL_DIS_LINK_CHG_UNIT |		\
	WOL_CTL_DIS_PATTERN_UNIT |		\
	WOL_CTL_DIS_MAGIC_PKT_UNIT)

/*                                            */
#define WOL_CTL_PATT_ENA(x)	(1 << (x))


/*                            */
enum {
	XM_MMU_CMD	= 0x0000, /*                                 */
	XM_POFF		= 0x0008, /*                                   */
	XM_BURST	= 0x000c, /*                                          */
	XM_1L_VLAN_TAG	= 0x0010, /*                                  */
	XM_2L_VLAN_TAG	= 0x0014, /*                                  */
	XM_TX_CMD	= 0x0020, /*                                      */
	XM_TX_RT_LIM	= 0x0024, /*                                          */
	XM_TX_STIME	= 0x0028, /*                                       */
	XM_TX_IPG	= 0x002c, /*                                      */
	XM_RX_CMD	= 0x0030, /*                                     */
	XM_PHY_ADDR	= 0x0034, /*                                 */
	XM_PHY_DATA	= 0x0038, /*                              */
	XM_GP_PORT	= 0x0040, /*                                          */
	XM_IMSK		= 0x0044, /*                                    */
	XM_ISRC		= 0x0048, /*                                      */
	XM_HW_CFG	= 0x004c, /*                                     */
	XM_TX_LO_WM	= 0x0060, /*                                   */
	XM_TX_HI_WM	= 0x0062, /*                                    */
	XM_TX_THR	= 0x0064, /*                                 */
	XM_HT_THR	= 0x0066, /*                                   */
	XM_PAUSE_DA	= 0x0068, /*                                      */
	XM_CTL_PARA	= 0x0070, /*                                       */
	XM_MAC_OPCODE	= 0x0074, /*                                          */
	XM_MAC_PTIME	= 0x0076, /*                                          */
	XM_TX_STAT	= 0x0078, /*                                    */

	XM_EXM_START	= 0x0080, /*                                   */
#define XM_EXM(reg)	(XM_EXM_START + ((reg) << 3))
};

enum {
	XM_SRC_CHK	= 0x0100, /*                                          */
	XM_SA		= 0x0108, /*                                     */
	XM_HSM		= 0x0110, /*                                         */
	XM_RX_LO_WM	= 0x0118, /*                                   */
	XM_RX_HI_WM	= 0x011a, /*                                    */
	XM_RX_THR	= 0x011c, /*                                      */
	XM_DEV_ID	= 0x0120, /*                               */
	XM_MODE		= 0x0124, /*                          */
	XM_LSA		= 0x0128, /*                                 */
	XM_TS_READ	= 0x0130, /*                                     */
	XM_TS_LOAD	= 0x0134, /*                                  */
	XM_STAT_CMD	= 0x0200, /*                                        */
	XM_RX_CNT_EV	= 0x0204, /*                                      */
	XM_TX_CNT_EV	= 0x0208, /*                                      */
	XM_RX_EV_MSK	= 0x020c, /*                                  */
	XM_TX_EV_MSK	= 0x0210, /*                                  */
	XM_TXF_OK	= 0x0280, /*                                          */
	XM_TXO_OK_HI	= 0x0284, /*                                          */
	XM_TXO_OK_LO	= 0x0288, /*                                          */
	XM_TXF_BC_OK	= 0x028c, /*                                        */
	XM_TXF_MC_OK	= 0x0290, /*                                        */
	XM_TXF_UC_OK	= 0x0294, /*                                      */
	XM_TXF_LONG	= 0x0298, /*                                  */
	XM_TXE_BURST	= 0x029c, /*                                   */
	XM_TXF_MPAUSE	= 0x02a0, /*                                        */
	XM_TXF_MCTRL	= 0x02a4, /*                                      */
	XM_TXF_SNG_COL	= 0x02a8, /*                                        */
	XM_TXF_MUL_COL	= 0x02ac, /*                                          */
	XM_TXF_ABO_COL	= 0x02b0, /*                                          */
	XM_TXF_LAT_COL	= 0x02b4, /*                                      */
	XM_TXF_DEF	= 0x02b8, /*                                      */
	XM_TXF_EX_DEF	= 0x02bc, /*                                          */
	XM_TXE_FIFO_UR	= 0x02c0, /*                                       */
	XM_TXE_CS_ERR	= 0x02c4, /*                                       */
	XM_TXP_UTIL	= 0x02c8, /*                                */
	XM_TXF_64B	= 0x02d0, /*                                     */
	XM_TXF_127B	= 0x02d4, /*                                         */
	XM_TXF_255B	= 0x02d8, /*                                          */
	XM_TXF_511B	= 0x02dc, /*                                          */
	XM_TXF_1023B	= 0x02e0, /*                                          */
	XM_TXF_MAX_SZ	= 0x02e4, /*                                          */
	XM_RXF_OK	= 0x0300, /*                               */
	XM_RXO_OK_HI	= 0x0304, /*                                        */
	XM_RXO_OK_LO	= 0x0308, /*                                          */
	XM_RXF_BC_OK	= 0x030c, /*                                         */
	XM_RXF_MC_OK	= 0x0310, /*                                         */
	XM_RXF_UC_OK	= 0x0314, /*                                       */
	XM_RXF_MPAUSE	= 0x0318, /*                                        */
	XM_RXF_MCTRL	= 0x031c, /*                                      */
	XM_RXF_INV_MP	= 0x0320, /*                                       */
	XM_RXF_INV_MOC	= 0x0324, /*                                          */
	XM_RXE_BURST	= 0x0328, /*                                   */
	XM_RXE_FMISS	= 0x032c, /*                                       */
	XM_RXF_FRA_ERR	= 0x0330, /*                                     */
	XM_RXE_FIFO_OV	= 0x0334, /*                                       */
	XM_RXF_JAB_PKT	= 0x0338, /*                                       */
	XM_RXE_CAR_ERR	= 0x033c, /*                                       */
	XM_RXF_LEN_ERR	= 0x0340, /*                                     */
	XM_RXE_SYM_ERR	= 0x0344, /*                                    */
	XM_RXE_SHT_ERR	= 0x0348, /*                                     */
	XM_RXE_RUNT	= 0x034c, /*                                  */
	XM_RXF_LNG_ERR	= 0x0350, /*                                        */
	XM_RXF_FCS_ERR	= 0x0354, /*                                          */
	XM_RXF_CEX_ERR	= 0x035c, /*                                          */
	XM_RXP_UTIL	= 0x0360, /*                                */
	XM_RXF_64B	= 0x0368, /*                                     */
	XM_RXF_127B	= 0x036c, /*                                         */
	XM_RXF_255B	= 0x0370, /*                                          */
	XM_RXF_511B	= 0x0374, /*                                          */
	XM_RXF_1023B	= 0x0378, /*                                          */
	XM_RXF_MAX_SZ	= 0x037c, /*                                          */
};

/*                                            */
enum {
	XM_MMU_PHY_RDY	= 1<<12, /*                        */
	XM_MMU_PHY_BUSY	= 1<<11, /*                  */
	XM_MMU_IGN_PF	= 1<<10, /*                            */
	XM_MMU_MAC_LB	= 1<<9,	 /*                             */
	XM_MMU_FRC_COL	= 1<<7,	 /*                         */
	XM_MMU_SIM_COL	= 1<<6,	 /*                            */
	XM_MMU_NO_PRE	= 1<<5,	 /*                          */
	XM_MMU_GMII_FD	= 1<<4,	 /*                               */
	XM_MMU_RAT_CTRL	= 1<<3,	 /*                             */
	XM_MMU_GMII_LOOP= 1<<2,	 /*                                 */
	XM_MMU_ENA_RX	= 1<<1,	 /*                         */
	XM_MMU_ENA_TX	= 1<<0,	 /*                            */
};


/*                                                */
enum {
	XM_TX_BK2BK	= 1<<6,	/*                                       */
	XM_TX_ENC_BYP	= 1<<5,	/*                                    */
	XM_TX_SAM_LINE	= 1<<4,	/*                                            */
	XM_TX_NO_GIG_MD	= 1<<3,	/*                                   */
	XM_TX_NO_PRE	= 1<<2,	/*                                     */
	XM_TX_NO_CRC	= 1<<1,	/*                                */
	XM_TX_AUTO_PAD	= 1<<0,	/*                                  */
};

/*                                                       */
#define XM_RT_LIM_MSK	0x1f	/*                           */


/*                                                   */
#define XM_STIME_MSK	0x7f	/*                             */


/*                                                */
#define XM_IPG_MSK		0xff	/*                           */


/*                                               */
enum {
	XM_RX_LENERR_OK	= 1<<8,	/*                                 */
				/*                        */
	XM_RX_BIG_PK_OK	= 1<<7,	/*                                 */
				/*                */
	XM_RX_IPG_CAP	= 1<<6,	/*                                  */
	XM_RX_TP_MD	= 1<<5,	/*                                 */
	XM_RX_STRIP_FCS	= 1<<4,	/*                              */
	XM_RX_SELF_RX	= 1<<3,	/*                                   */
	XM_RX_SAM_LINE	= 1<<2,	/*                                            */
	XM_RX_STRIP_PAD	= 1<<1,	/*                                      */
	XM_RX_DIS_CEXT	= 1<<0,	/*                                    */
};


/*                                                     */
enum {
	XM_GP_ANIP	= 1<<6,	/*                                    */
	XM_GP_FRC_INT	= 1<<5,	/*                              */
	XM_GP_RES_MAC	= 1<<3,	/*                                  */
	XM_GP_RES_STAT	= 1<<2,	/*                                          */
	XM_GP_INP_ASS	= 1<<0,	/*                                    */
};


/*                                             */
/*                                               */
enum {
	XM_IS_LNK_AE	= 1<<14, /*                                 */
	XM_IS_TX_ABORT	= 1<<13, /*                                       */
	XM_IS_FRC_INT	= 1<<12, /*                                 */
	XM_IS_INP_ASS	= 1<<11, /*                                      */
	XM_IS_LIPA_RC	= 1<<10, /*                                      */
	XM_IS_RX_PAGE	= 1<<9,	/*                       */
	XM_IS_TX_PAGE	= 1<<8,	/*                                       */
	XM_IS_AND	= 1<<7,	/*                               */
	XM_IS_TSC_OV	= 1<<6,	/*                                     */
	XM_IS_RXC_OV	= 1<<5,	/*                                   */
	XM_IS_TXC_OV	= 1<<4,	/*                                   */
	XM_IS_RXF_OV	= 1<<3,	/*                               */
	XM_IS_TXF_UR	= 1<<2,	/*                                */
	XM_IS_TX_COMP	= 1<<1,	/*                           */
	XM_IS_RX_COMP	= 1<<0,	/*                           */

	XM_IMSK_DISABLE	= 0xffff,
};

/*                                               */
enum {
	XM_HW_GEN_EOP	= 1<<3,	/*                                      */
	XM_HW_COM4SIG	= 1<<2,	/*                                       */
	XM_HW_GMII_MD	= 1<<0,	/*                                 */
};


/*                                               */
/*                                                */
#define XM_TX_WM_MSK	0x01ff	/*                                   */

/*                                           */
/*                                             */
/*                                           */
#define XM_THR_MSK		0x03ff	/*                                         */


/*                                               */
enum {
	XM_ST_VALID	= (1UL<<31),	/*                      */
	XM_ST_BYTE_CNT	= (0x3fffL<<17),	/*                             */
	XM_ST_RETRY_CNT	= (0x1fL<<12),	/*                         */
	XM_ST_EX_COL	= 1<<11,	/*                              */
	XM_ST_EX_DEF	= 1<<10,	/*                            */
	XM_ST_BURST	= 1<<9,		/*                               */
	XM_ST_DEFER	= 1<<8,		/*                            */
	XM_ST_BC	= 1<<7,		/*                          */
	XM_ST_MC	= 1<<6,		/*                          */
	XM_ST_UC	= 1<<5,		/*                        */
	XM_ST_TX_UR	= 1<<4,		/*                                */
	XM_ST_CS_ERR	= 1<<3,		/*                             */
	XM_ST_LAT_COL	= 1<<2,		/*                              */
	XM_ST_MUL_COL	= 1<<1,		/*                             */
	XM_ST_SGN_COL	= 1<<0,		/*                          */
};

/*                                               */
/*                                                */
#define XM_RX_WM_MSK	0x03ff		/*                                    */


/*                                         */
#define XM_DEV_OUI	(0x00ffffffUL<<8)	/*                       */
#define XM_DEV_REV	(0x07L << 5)		/*                         */


/*                                   */
enum {
	XM_MD_ENA_REJ	= 1<<26, /*                             */
	XM_MD_SPOE_E	= 1<<25, /*                            */
									/*                    */
	XM_MD_TX_REP	= 1<<24, /*                                */
	XM_MD_SPOFF_I	= 1<<23, /*                                 */
									/*                   */
	XM_MD_LE_STW	= 1<<22, /*                                       */
	XM_MD_TX_CONT	= 1<<21, /*                         */
	XM_MD_TX_PAUSE	= 1<<20, /*                               */
	XM_MD_ATS	= 1<<19, /*                           */
	XM_MD_SPOL_I	= 1<<18, /*                           */
									/*                   */
	XM_MD_SPOH_I	= 1<<17, /*                            */
									/*                   */
	XM_MD_CAP	= 1<<16, /*                            */
	XM_MD_ENA_HASH	= 1<<15, /*                        */
	XM_MD_CSA	= 1<<14, /*                               */
	XM_MD_CAA	= 1<<13, /*                             */
	XM_MD_RX_MCTRL	= 1<<12, /*                              */
	XM_MD_RX_RUNT	= 1<<11, /*                        */
	XM_MD_RX_IRLE	= 1<<10, /*                                   */
	XM_MD_RX_LONG	= 1<<9,  /*                       */
	XM_MD_RX_CRCE	= 1<<8,  /*                            */
	XM_MD_RX_ERR	= 1<<7,  /*                        */
	XM_MD_DIS_UC	= 1<<6,  /*                            */
	XM_MD_DIS_MC	= 1<<5,  /*                              */
	XM_MD_DIS_BC	= 1<<4,  /*                              */
	XM_MD_ENA_PROM	= 1<<3,  /*                            */
	XM_MD_ENA_BE	= 1<<2,  /*                           */
	XM_MD_FTF	= 1<<1,  /*                            */
	XM_MD_FRF	= 1<<0,  /*                            */
};

#define XM_PAUSE_MODE	(XM_MD_SPOE_E | XM_MD_SPOL_I | XM_MD_SPOH_I)
#define XM_DEF_MODE	(XM_MD_RX_RUNT | XM_MD_RX_IRLE | XM_MD_RX_LONG |\
			 XM_MD_RX_CRCE | XM_MD_RX_ERR | XM_MD_CSA)

/*                                                    */
enum {
	XM_SC_SNP_RXC	= 1<<5,	/*                               */
	XM_SC_SNP_TXC	= 1<<4,	/*                               */
	XM_SC_CP_RXC	= 1<<3,	/*                                        */
	XM_SC_CP_TXC	= 1<<2,	/*                                       */
	XM_SC_CLR_RXC	= 1<<1,	/*                                */
	XM_SC_CLR_TXC	= 1<<0,	/*                                */
};


/*                                                   */
/*                                               */
enum {
	XMR_MAX_SZ_OV	= 1<<31, /*                               */
	XMR_1023B_OV	= 1<<30, /*                               */
	XMR_511B_OV	= 1<<29, /*                               */
	XMR_255B_OV	= 1<<28, /*                               */
	XMR_127B_OV	= 1<<27, /*                               */
	XMR_64B_OV	= 1<<26, /*                           */
	XMR_UTIL_OV	= 1<<25, /*                              */
	XMR_UTIL_UR	= 1<<24, /*                              */
	XMR_CEX_ERR_OV	= 1<<23, /*                         */
	XMR_FCS_ERR_OV	= 1<<21, /*                             */
	XMR_LNG_ERR_OV	= 1<<20, /*                               */
	XMR_RUNT_OV	= 1<<19, /*                           */
	XMR_SHT_ERR_OV	= 1<<18, /*                               */
	XMR_SYM_ERR_OV	= 1<<17, /*                           */
	XMR_CAR_ERR_OV	= 1<<15, /*                               */
	XMR_JAB_PKT_OV	= 1<<14, /*                               */
	XMR_FIFO_OV	= 1<<13, /*                              */
	XMR_FRA_ERR_OV	= 1<<12, /*                               */
	XMR_FMISS_OV	= 1<<11, /*                             */
	XMR_BURST	= 1<<10, /*                               */
	XMR_INV_MOC	= 1<<9,  /*                               */
	XMR_INV_MP	= 1<<8,  /*                               */
	XMR_MCTRL_OV	= 1<<7,  /*                              */
	XMR_MPAUSE_OV	= 1<<6,  /*                               */
	XMR_UC_OK_OV	= 1<<5,  /*                               */
	XMR_MC_OK_OV	= 1<<4,  /*                             */
	XMR_BC_OK_OV	= 1<<3,  /*                             */
	XMR_OK_LO_OV	= 1<<2,  /*                               */
	XMR_OK_HI_OV	= 1<<1,  /*                               */
	XMR_OK_OV	= 1<<0,  /*                               */
};

#define XMR_DEF_MSK		(XMR_OK_LO_OV | XMR_OK_HI_OV)

/*                                                   */
/*                                               */
enum {
	XMT_MAX_SZ_OV	= 1<<25,	/*                               */
	XMT_1023B_OV	= 1<<24,	/*                               */
	XMT_511B_OV	= 1<<23,	/*                               */
	XMT_255B_OV	= 1<<22,	/*                               */
	XMT_127B_OV	= 1<<21,	/*                               */
	XMT_64B_OV	= 1<<20,	/*                           */
	XMT_UTIL_OV	= 1<<19,	/*                              */
	XMT_UTIL_UR	= 1<<18,	/*                              */
	XMT_CS_ERR_OV	= 1<<17,	/*                               */
	XMT_FIFO_UR_OV	= 1<<16,	/*                              */
	XMT_EX_DEF_OV	= 1<<15,	/*                               */
	XMT_DEF	= 1<<14,	/*                            */
	XMT_LAT_COL_OV	= 1<<13,	/*                            */
	XMT_ABO_COL_OV	= 1<<12,	/*                               */
	XMT_MUL_COL_OV	= 1<<11,	/*                            */
	XMT_SNG_COL	= 1<<10,	/*                              */
	XMT_MCTRL_OV	= 1<<9,		/*                               */
	XMT_MPAUSE	= 1<<8,		/*                               */
	XMT_BURST	= 1<<7,		/*                               */
	XMT_LONG	= 1<<6,		/*                              */
	XMT_UC_OK_OV	= 1<<5,		/*                           */
	XMT_MC_OK_OV	= 1<<4,		/*                             */
	XMT_BC_OK_OV	= 1<<3,		/*                             */
	XMT_OK_LO_OV	= 1<<2,		/*                               */
	XMT_OK_HI_OV	= 1<<1,		/*                               */
	XMT_OK_OV	= 1<<0,		/*                         */
};

#define XMT_DEF_MSK		(XMT_OK_LO_OV | XMT_OK_HI_OV)

struct skge_rx_desc {
	u32		control;
	u32		next_offset;
	u32		dma_lo;
	u32		dma_hi;
	u32		status;
	u32		timestamp;
	u16		csum2;
	u16		csum1;
	u16		csum2_start;
	u16		csum1_start;
};

struct skge_tx_desc {
	u32		control;
	u32		next_offset;
	u32		dma_lo;
	u32		dma_hi;
	u32		status;
	u32		csum_offs;
	u16		csum_write;
	u16		csum_start;
	u32		rsvd;
};

struct skge_element {
	struct skge_element	*next;
	void			*desc;
	struct sk_buff  	*skb;
	DEFINE_DMA_UNMAP_ADDR(mapaddr);
	DEFINE_DMA_UNMAP_LEN(maplen);
};

struct skge_ring {
	struct skge_element *to_clean;
	struct skge_element *to_use;
	struct skge_element *start;
	unsigned long	    count;
};


struct skge_hw {
	void __iomem  	     *regs;
	struct pci_dev	     *pdev;
	spinlock_t	     hw_lock;
	u32		     intr_mask;
	struct net_device    *dev[2];

	u8	     	     chip_id;
	u8		     chip_rev;
	u8		     copper;
	u8		     ports;
	u8		     phy_type;

	u32	     	     ram_size;
	u32	     	     ram_offset;
	u16		     phy_addr;
	spinlock_t	     phy_lock;
	struct tasklet_struct phy_task;

	char		     irq_name[0]; /*                      */
};

enum pause_control {
	FLOW_MODE_NONE 		= 1, /*                 */
	FLOW_MODE_LOC_SEND	= 2, /*                           */
	FLOW_MODE_SYMMETRIC	= 3, /*                              */
	FLOW_MODE_SYM_OR_REM	= 4, /*                                
                                                  
          */
};

enum pause_status {
	FLOW_STAT_INDETERMINATED=0,	/*                */
	FLOW_STAT_NONE,			/*                 */
	FLOW_STAT_REM_SEND,		/*                            */
	FLOW_STAT_LOC_SEND,		/*                           */
	FLOW_STAT_SYMMETRIC,		/*                             */
};


struct skge_port {
	struct skge_hw	     *hw;
	struct net_device    *netdev;
	struct napi_struct   napi;
	int		     port;
	u32		     msg_enable;

	struct skge_ring     tx_ring;

	struct skge_ring     rx_ring ____cacheline_aligned_in_smp;
	unsigned int	     rx_buf_size;

	struct timer_list    link_timer;
	enum pause_control   flow_control;
	enum pause_status    flow_status;
	u8		     blink_on;
	u8		     wol;
	u8		     autoneg;	/*                                 */
	u8		     duplex;	/*                          */
	u16		     speed;	/*                            */
	u32		     advertising;

	void		     *mem;	/*                      */
	dma_addr_t	     dma;
	unsigned long	     mem_size;
#ifdef CONFIG_SKGE_DEBUG
	struct dentry	     *debugfs;
#endif
};


/*                                            */
static inline u32 skge_read32(const struct skge_hw *hw, int reg)
{
	return readl(hw->regs + reg);
}

static inline u16 skge_read16(const struct skge_hw *hw, int reg)
{
	return readw(hw->regs + reg);
}

static inline u8 skge_read8(const struct skge_hw *hw, int reg)
{
	return readb(hw->regs + reg);
}

static inline void skge_write32(const struct skge_hw *hw, int reg, u32 val)
{
	writel(val, hw->regs + reg);
}

static inline void skge_write16(const struct skge_hw *hw, int reg, u16 val)
{
	writew(val, hw->regs + reg);
}

static inline void skge_write8(const struct skge_hw *hw, int reg, u8 val)
{
	writeb(val, hw->regs + reg);
}

/*                                          */
#define SK_REG(port,reg)	(((port)<<7)+(u16)(reg))
#define SK_XMAC_REG(port, reg) \
	((BASE_XMAC_1 + (port) * (BASE_XMAC_2 - BASE_XMAC_1)) | (reg) << 1)

static inline u32 xm_read32(const struct skge_hw *hw, int port, int reg)
{
	u32 v;
	v = skge_read16(hw, SK_XMAC_REG(port, reg));
	v |= (u32)skge_read16(hw, SK_XMAC_REG(port, reg+2)) << 16;
	return v;
}

static inline u16 xm_read16(const struct skge_hw *hw, int port, int reg)
{
	return skge_read16(hw, SK_XMAC_REG(port,reg));
}

static inline void xm_write32(const struct skge_hw *hw, int port, int r, u32 v)
{
	skge_write16(hw, SK_XMAC_REG(port,r), v & 0xffff);
	skge_write16(hw, SK_XMAC_REG(port,r+2), v >> 16);
}

static inline void xm_write16(const struct skge_hw *hw, int port, int r, u16 v)
{
	skge_write16(hw, SK_XMAC_REG(port,r), v);
}

static inline void xm_outhash(const struct skge_hw *hw, int port, int reg,
				   const u8 *hash)
{
	xm_write16(hw, port, reg,   (u16)hash[0] | ((u16)hash[1] << 8));
	xm_write16(hw, port, reg+2, (u16)hash[2] | ((u16)hash[3] << 8));
	xm_write16(hw, port, reg+4, (u16)hash[4] | ((u16)hash[5] << 8));
	xm_write16(hw, port, reg+6, (u16)hash[6] | ((u16)hash[7] << 8));
}

static inline void xm_outaddr(const struct skge_hw *hw, int port, int reg,
				   const u8 *addr)
{
	xm_write16(hw, port, reg,   (u16)addr[0] | ((u16)addr[1] << 8));
	xm_write16(hw, port, reg+2, (u16)addr[2] | ((u16)addr[3] << 8));
	xm_write16(hw, port, reg+4, (u16)addr[4] | ((u16)addr[5] << 8));
}

#define SK_GMAC_REG(port,reg) \
	(BASE_GMAC_1 + (port) * (BASE_GMAC_2-BASE_GMAC_1) + (reg))

static inline u16 gma_read16(const struct skge_hw *hw, int port, int reg)
{
	return skge_read16(hw, SK_GMAC_REG(port,reg));
}

static inline u32 gma_read32(const struct skge_hw *hw, int port, int reg)
{
	return (u32) skge_read16(hw, SK_GMAC_REG(port,reg))
		| ((u32)skge_read16(hw, SK_GMAC_REG(port,reg+4)) << 16);
}

static inline void gma_write16(const struct skge_hw *hw, int port, int r, u16 v)
{
	skge_write16(hw, SK_GMAC_REG(port,r), v);
}

static inline void gma_set_addr(struct skge_hw *hw, int port, int reg,
				    const u8 *addr)
{
	gma_write16(hw, port, reg,  (u16) addr[0] | ((u16) addr[1] << 8));
	gma_write16(hw, port, reg+4,(u16) addr[2] | ((u16) addr[3] << 8));
	gma_write16(hw, port, reg+8,(u16) addr[4] | ((u16) addr[5] << 8));
}

#endif
