/*
 * Copyright 2004-2007 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2008 Juergen Beisert, kernel@pengutronix.de
 * Copyright 2009 Holger Schurig, hs4233@mail.mn-solutions.de
 *
 * This contains i.MX21-specific hardware definitions. For those
 * hardware pieces that are common between i.MX21 and i.MX27, have a
 * look at mx2x.h.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#ifndef __MACH_MX21_H__
#define __MACH_MX21_H__

#define MX21_AIPI_BASE_ADDR		0x10000000
#define MX21_AIPI_SIZE			SZ_1M
#define MX21_DMA_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x01000)
#define MX21_WDOG_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x02000)
#define MX21_GPT1_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x03000)
#define MX21_GPT2_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x04000)
#define MX21_GPT3_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x05000)
#define MX21_PWM_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x06000)
#define MX21_RTC_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x07000)
#define MX21_KPP_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x08000)
#define MX21_OWIRE_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x09000)
#define MX21_UART1_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x0a000)
#define MX21_UART2_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x0b000)
#define MX21_UART3_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x0c000)
#define MX21_UART4_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x0d000)
#define MX21_CSPI1_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x0e000)
#define MX21_CSPI2_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x0f000)
#define MX21_SSI1_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x10000)
#define MX21_SSI2_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x11000)
#define MX21_I2C_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x12000)
#define MX21_SDHC1_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x13000)
#define MX21_SDHC2_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x14000)
#define MX21_GPIO_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x15000)
#define MX21_GPIO1_BASE_ADDR			(MX21_GPIO_BASE_ADDR + 0x000)
#define MX21_GPIO2_BASE_ADDR			(MX21_GPIO_BASE_ADDR + 0x100)
#define MX21_GPIO3_BASE_ADDR			(MX21_GPIO_BASE_ADDR + 0x200)
#define MX21_GPIO4_BASE_ADDR			(MX21_GPIO_BASE_ADDR + 0x300)
#define MX21_GPIO5_BASE_ADDR			(MX21_GPIO_BASE_ADDR + 0x400)
#define MX21_GPIO6_BASE_ADDR			(MX21_GPIO_BASE_ADDR + 0x500)
#define MX21_AUDMUX_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x16000)
#define MX21_CSPI3_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x17000)
#define MX21_LCDC_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x21000)
#define MX21_SLCDC_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x22000)
#define MX21_USBOTG_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x24000)
#define MX21_EMMA_PP_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x26000)
#define MX21_EMMA_PRP_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x26400)
#define MX21_CCM_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x27000)
#define MX21_SYSCTRL_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x27800)
#define MX21_JAM_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x3e000)
#define MX21_MAX_BASE_ADDR			(MX21_AIPI_BASE_ADDR + 0x3f000)

#define MX21_AVIC_BASE_ADDR		0x10040000

#define MX21_SAHB1_BASE_ADDR		0x80000000
#define MX21_SAHB1_SIZE			SZ_1M
#define MX21_CSI_BASE_ADDR			(MX2x_SAHB1_BASE_ADDR + 0x0000)

/*                       */
#define MX21_SDRAM_BASE_ADDR		0xc0000000
#define MX21_CSD1_BASE_ADDR		0xc4000000

#define MX21_CS0_BASE_ADDR		0xc8000000
#define MX21_CS1_BASE_ADDR		0xcc000000
#define MX21_CS2_BASE_ADDR		0xd0000000
#define MX21_CS3_BASE_ADDR		0xd1000000
#define MX21_CS4_BASE_ADDR		0xd2000000
#define MX21_PCMCIA_MEM_BASE_ADDR	0xd4000000
#define MX21_CS5_BASE_ADDR		0xdd000000

/*                                   */
#define MX21_X_MEMC_BASE_ADDR		0xdf000000
#define MX21_X_MEMC_SIZE		SZ_256K

#define MX21_SDRAMC_BASE_ADDR		(MX21_X_MEMC_BASE_ADDR + 0x0000)
#define MX21_EIM_BASE_ADDR		(MX21_X_MEMC_BASE_ADDR + 0x1000)
#define MX21_PCMCIA_CTL_BASE_ADDR	(MX21_X_MEMC_BASE_ADDR + 0x2000)
#define MX21_NFC_BASE_ADDR		(MX21_X_MEMC_BASE_ADDR + 0x3000)

#define MX21_IRAM_BASE_ADDR		0xffffe800	/*              */

#define MX21_IO_P2V(x)			IMX_IO_P2V(x)
#define MX21_IO_ADDRESS(x)		IOMEM(MX21_IO_P2V(x))

/*                         */
#define MX21_INT_CSPI3		6
#define MX21_INT_GPIO		8
#define MX21_INT_FIRI		9
#define MX21_INT_SDHC2		10
#define MX21_INT_SDHC1		11
#define MX21_INT_I2C		12
#define MX21_INT_SSI2		13
#define MX21_INT_SSI1		14
#define MX21_INT_CSPI2		15
#define MX21_INT_CSPI1		16
#define MX21_INT_UART4		17
#define MX21_INT_UART3		18
#define MX21_INT_UART2		19
#define MX21_INT_UART1		20
#define MX21_INT_KPP		21
#define MX21_INT_RTC		22
#define MX21_INT_PWM		23
#define MX21_INT_GPT3		24
#define MX21_INT_GPT2		25
#define MX21_INT_GPT1		26
#define MX21_INT_WDOG		27
#define MX21_INT_PCMCIA		28
#define MX21_INT_NFC		29
#define MX21_INT_BMI		30
#define MX21_INT_CSI		31
#define MX21_INT_DMACH0		32
#define MX21_INT_DMACH1		33
#define MX21_INT_DMACH2		34
#define MX21_INT_DMACH3		35
#define MX21_INT_DMACH4		36
#define MX21_INT_DMACH5		37
#define MX21_INT_DMACH6		38
#define MX21_INT_DMACH7		39
#define MX21_INT_DMACH8		40
#define MX21_INT_DMACH9		41
#define MX21_INT_DMACH10	42
#define MX21_INT_DMACH11	43
#define MX21_INT_DMACH12	44
#define MX21_INT_DMACH13	45
#define MX21_INT_DMACH14	46
#define MX21_INT_DMACH15	47
#define MX21_INT_EMMAENC	49
#define MX21_INT_EMMADEC	50
#define MX21_INT_EMMAPRP	51
#define MX21_INT_EMMAPP		52
#define MX21_INT_USBWKUP	53
#define MX21_INT_USBDMA		54
#define MX21_INT_USBHOST	55
#define MX21_INT_USBFUNC	56
#define MX21_INT_USBMNP		57
#define MX21_INT_USBCTRL	58
#define MX21_INT_SLCDC		60
#define MX21_INT_LCDC		61

/*                           */
#define MX21_DMA_REQ_CSPI3_RX	1
#define MX21_DMA_REQ_CSPI3_TX	2
#define MX21_DMA_REQ_EXT	3
#define MX21_DMA_REQ_FIRI_RX	4
#define MX21_DMA_REQ_SDHC2	6
#define MX21_DMA_REQ_SDHC1	7
#define MX21_DMA_REQ_SSI2_RX0	8
#define MX21_DMA_REQ_SSI2_TX0	9
#define MX21_DMA_REQ_SSI2_RX1	10
#define MX21_DMA_REQ_SSI2_TX1	11
#define MX21_DMA_REQ_SSI1_RX0	12
#define MX21_DMA_REQ_SSI1_TX0	13
#define MX21_DMA_REQ_SSI1_RX1	14
#define MX21_DMA_REQ_SSI1_TX1	15
#define MX21_DMA_REQ_CSPI2_RX	16
#define MX21_DMA_REQ_CSPI2_TX	17
#define MX21_DMA_REQ_CSPI1_RX	18
#define MX21_DMA_REQ_CSPI1_TX	19
#define MX21_DMA_REQ_UART4_RX	20
#define MX21_DMA_REQ_UART4_TX	21
#define MX21_DMA_REQ_UART3_RX	22
#define MX21_DMA_REQ_UART3_TX	23
#define MX21_DMA_REQ_UART2_RX	24
#define MX21_DMA_REQ_UART2_TX	25
#define MX21_DMA_REQ_UART1_RX	26
#define MX21_DMA_REQ_UART1_TX	27
#define MX21_DMA_REQ_BMI_TX	28
#define MX21_DMA_REQ_BMI_RX	29
#define MX21_DMA_REQ_CSI_STAT	30
#define MX21_DMA_REQ_CSI_RX	31

#endif /*                        */
