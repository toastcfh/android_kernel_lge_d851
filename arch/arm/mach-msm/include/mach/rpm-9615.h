/* Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __ARCH_ARM_MACH_MSM_RPM_9615_H
#define __ARCH_ARM_MACH_MSM_RPM_9615_H

/*                               */
enum {
	MSM_RPM_9615_CTRL_VERSION_MAJOR,
	MSM_RPM_9615_CTRL_VERSION_MINOR,
	MSM_RPM_9615_CTRL_VERSION_BUILD,

	MSM_RPM_9615_CTRL_REQ_CTX_0,
	MSM_RPM_9615_CTRL_REQ_CTX_7 = MSM_RPM_9615_CTRL_REQ_CTX_0 + 7,
	MSM_RPM_9615_CTRL_REQ_SEL_0,
	MSM_RPM_9615_CTRL_REQ_SEL_3 = MSM_RPM_9615_CTRL_REQ_SEL_0 + 3,
	MSM_RPM_9615_CTRL_ACK_CTX_0,
	MSM_RPM_9615_CTRL_ACK_CTX_7 = MSM_RPM_9615_CTRL_ACK_CTX_0 + 7,
	MSM_RPM_9615_CTRL_ACK_SEL_0,
	MSM_RPM_9615_CTRL_ACK_SEL_7 = MSM_RPM_9615_CTRL_ACK_SEL_0 + 7,
};

enum {
	MSM_RPM_9615_SEL_NOTIFICATION				= 0,
	MSM_RPM_9615_SEL_INVALIDATE				= 1,
	MSM_RPM_9615_SEL_TRIGGER_TIMED				= 2,
	MSM_RPM_9615_SEL_RPM_CTL				= 3,

	MSM_RPM_9615_SEL_CXO_CLK				= 5,
	MSM_RPM_9615_SEL_SYSTEM_FABRIC_CLK			= 9,
	MSM_RPM_9615_SEL_DAYTONA_FABRIC_CLK			= 11,
	MSM_RPM_9615_SEL_SFPB_CLK				= 12,
	MSM_RPM_9615_SEL_CFPB_CLK				= 13,
	MSM_RPM_9615_SEL_EBI1_CLK				= 16,

	MSM_RPM_9615_SEL_SYS_FABRIC_CFG_HALT			= 22,
	MSM_RPM_9615_SEL_SYS_FABRIC_CFG_CLKMOD			= 23,
	MSM_RPM_9615_SEL_SYS_FABRIC_CFG_IOCTL			= 24,
	MSM_RPM_9615_SEL_SYSTEM_FABRIC_ARB			= 25,

	MSM_RPM_9615_SEL_PM8018_S1				= 30,
	MSM_RPM_9615_SEL_PM8018_S2				= 31,
	MSM_RPM_9615_SEL_PM8018_S3				= 32,
	MSM_RPM_9615_SEL_PM8018_S4				= 33,
	MSM_RPM_9615_SEL_PM8018_S5				= 34,
	MSM_RPM_9615_SEL_PM8018_L1				= 35,
	MSM_RPM_9615_SEL_PM8018_L2				= 36,
	MSM_RPM_9615_SEL_PM8018_L3				= 37,
	MSM_RPM_9615_SEL_PM8018_L4				= 38,
	MSM_RPM_9615_SEL_PM8018_L5				= 39,
	MSM_RPM_9615_SEL_PM8018_L6				= 40,
	MSM_RPM_9615_SEL_PM8018_L7				= 41,
	MSM_RPM_9615_SEL_PM8018_L8				= 42,
	MSM_RPM_9615_SEL_PM8018_L9				= 43,
	MSM_RPM_9615_SEL_PM8018_L10				= 44,
	MSM_RPM_9615_SEL_PM8018_L11				= 45,
	MSM_RPM_9615_SEL_PM8018_L12				= 46,
	MSM_RPM_9615_SEL_PM8018_L13				= 47,
	MSM_RPM_9615_SEL_PM8018_L14				= 48,
	MSM_RPM_9615_SEL_PM8018_LVS1				= 49,

	MSM_RPM_9615_SEL_NCP					= 80,
	MSM_RPM_9615_SEL_CXO_BUFFERS				= 81,
	MSM_RPM_9615_SEL_USB_OTG_SWITCH				= 82,
	MSM_RPM_9615_SEL_HDMI_SWITCH				= 83,
	MSM_RPM_9615_SEL_VOLTAGE_CORNER				= 87,

	MSM_RPM_9615_SEL_LAST = MSM_RPM_9615_SEL_VOLTAGE_CORNER,
};

/*                                    */
enum {
	MSM_RPM_9615_ID_NOTIFICATION_CONFIGURED_0		= 0,
	MSM_RPM_9615_ID_NOTIFICATION_CONFIGURED_3 =
		MSM_RPM_9615_ID_NOTIFICATION_CONFIGURED_0 + 3,

	MSM_RPM_9615_ID_NOTIFICATION_REGISTERED_0		= 4,
	MSM_RPM_9615_ID_NOTIFICATION_REGISTERED_3 =
		MSM_RPM_9615_ID_NOTIFICATION_REGISTERED_0 + 3,

	MSM_RPM_9615_ID_INVALIDATE_0				= 8,
	MSM_RPM_9615_ID_INVALIDATE_7 =
		MSM_RPM_9615_ID_INVALIDATE_0 + 7,

	MSM_RPM_9615_ID_TRIGGER_TIMED_TO			= 16,
	MSM_RPM_9615_ID_TRIGGER_TIMED_SCLK_COUNT		= 17,

	MSM_RPM_9615_ID_RPM_CTL					= 18,

	/*                                                         */
	MSM_RPM_9615_ID_RESERVED_0				= 19,
	MSM_RPM_9615_ID_RESERVED_5 =
		MSM_RPM_9615_ID_RESERVED_0 + 5,

	MSM_RPM_9615_ID_CXO_CLK					= 25,
	MSM_RPM_9615_ID_SYSTEM_FABRIC_CLK			= 26,
	MSM_RPM_9615_ID_DAYTONA_FABRIC_CLK			= 27,
	MSM_RPM_9615_ID_SFPB_CLK				= 28,
	MSM_RPM_9615_ID_CFPB_CLK				= 29,
	MSM_RPM_9615_ID_EBI1_CLK				= 30,

	MSM_RPM_9615_ID_SYS_FABRIC_CFG_HALT_0			= 31,
	MSM_RPM_9615_ID_SYS_FABRIC_CFG_HALT_1			= 32,
	MSM_RPM_9615_ID_SYS_FABRIC_CFG_CLKMOD_0			= 33,
	MSM_RPM_9615_ID_SYS_FABRIC_CFG_CLKMOD_1			= 34,
	MSM_RPM_9615_ID_SYS_FABRIC_CFG_CLKMOD_2			= 35,
	MSM_RPM_9615_ID_SYS_FABRIC_CFG_IOCTL			= 36,
	MSM_RPM_9615_ID_SYSTEM_FABRIC_ARB_0			= 37,
	MSM_RPM_9615_ID_SYSTEM_FABRIC_ARB_26 =
		MSM_RPM_9615_ID_SYSTEM_FABRIC_ARB_0 + 26,

	MSM_RPM_9615_ID_PM8018_S1_0				= 64,
	MSM_RPM_9615_ID_PM8018_S1_1				= 65,
	MSM_RPM_9615_ID_PM8018_S2_0				= 66,
	MSM_RPM_9615_ID_PM8018_S2_1				= 67,
	MSM_RPM_9615_ID_PM8018_S3_0				= 68,
	MSM_RPM_9615_ID_PM8018_S3_1				= 69,
	MSM_RPM_9615_ID_PM8018_S4_0				= 70,
	MSM_RPM_9615_ID_PM8018_S4_1				= 71,
	MSM_RPM_9615_ID_PM8018_S5_0				= 72,
	MSM_RPM_9615_ID_PM8018_S5_1				= 73,
	MSM_RPM_9615_ID_PM8018_L1_0				= 74,
	MSM_RPM_9615_ID_PM8018_L1_1				= 75,
	MSM_RPM_9615_ID_PM8018_L2_0				= 76,
	MSM_RPM_9615_ID_PM8018_L2_1				= 77,
	MSM_RPM_9615_ID_PM8018_L3_0				= 78,
	MSM_RPM_9615_ID_PM8018_L3_1				= 79,
	MSM_RPM_9615_ID_PM8018_L4_0				= 80,
	MSM_RPM_9615_ID_PM8018_L4_1				= 81,
	MSM_RPM_9615_ID_PM8018_L5_0				= 82,
	MSM_RPM_9615_ID_PM8018_L5_1				= 83,
	MSM_RPM_9615_ID_PM8018_L6_0				= 84,
	MSM_RPM_9615_ID_PM8018_L6_1				= 85,
	MSM_RPM_9615_ID_PM8018_L7_0				= 86,
	MSM_RPM_9615_ID_PM8018_L7_1				= 87,
	MSM_RPM_9615_ID_PM8018_L8_0				= 88,
	MSM_RPM_9615_ID_PM8018_L8_1				= 89,
	MSM_RPM_9615_ID_PM8018_L9_0				= 90,
	MSM_RPM_9615_ID_PM8018_L9_1				= 91,
	MSM_RPM_9615_ID_PM8018_L10_0				= 92,
	MSM_RPM_9615_ID_PM8018_L10_1				= 93,
	MSM_RPM_9615_ID_PM8018_L11_0				= 94,
	MSM_RPM_9615_ID_PM8018_L11_1				= 95,
	MSM_RPM_9615_ID_PM8018_L12_0				= 96,
	MSM_RPM_9615_ID_PM8018_L12_1				= 97,
	MSM_RPM_9615_ID_PM8018_L13_0				= 98,
	MSM_RPM_9615_ID_PM8018_L13_1				= 99,
	MSM_RPM_9615_ID_PM8018_L14_0				= 100,
	MSM_RPM_9615_ID_PM8018_L14_1				= 101,
	MSM_RPM_9615_ID_PM8018_LVS1				= 102,

	MSM_RPM_9615_ID_NCP_0					= 103,
	MSM_RPM_9615_ID_NCP_1					= 104,
	MSM_RPM_9615_ID_CXO_BUFFERS				= 105,
	MSM_RPM_9615_ID_USB_OTG_SWITCH				= 106,
	MSM_RPM_9615_ID_HDMI_SWITCH				= 107,
	MSM_RPM_9615_ID_VOLTAGE_CORNER				= 109,

	MSM_RPM_9615_ID_LAST = MSM_RPM_9615_ID_VOLTAGE_CORNER,
};

/*                    */
enum {
	MSM_RPM_9615_STATUS_ID_VERSION_MAJOR			= 0,
	MSM_RPM_9615_STATUS_ID_VERSION_MINOR			= 1,
	MSM_RPM_9615_STATUS_ID_VERSION_BUILD			= 2,
	MSM_RPM_9615_STATUS_ID_SUPPORTED_RESOURCES_0		= 3,
	MSM_RPM_9615_STATUS_ID_SUPPORTED_RESOURCES_1		= 4,
	MSM_RPM_9615_STATUS_ID_SUPPORTED_RESOURCES_2		= 5,
	MSM_RPM_9615_STATUS_ID_RESERVED_SUPPORTED_RESOURCES_0	= 6,
	MSM_RPM_9615_STATUS_ID_SEQUENCE				= 7,
	MSM_RPM_9615_STATUS_ID_RPM_CTL				= 8,
	MSM_RPM_9615_STATUS_ID_CXO_CLK				= 9,
	MSM_RPM_9615_STATUS_ID_SYSTEM_FABRIC_CLK		= 10,
	MSM_RPM_9615_STATUS_ID_DAYTONA_FABRIC_CLK		= 11,
	MSM_RPM_9615_STATUS_ID_SFPB_CLK				= 12,
	MSM_RPM_9615_STATUS_ID_CFPB_CLK				= 13,
	MSM_RPM_9615_STATUS_ID_EBI1_CLK				= 14,
	MSM_RPM_9615_STATUS_ID_SYS_FABRIC_CFG_HALT		= 15,
	MSM_RPM_9615_STATUS_ID_SYS_FABRIC_CFG_CLKMOD		= 16,
	MSM_RPM_9615_STATUS_ID_SYS_FABRIC_CFG_IOCTL		= 17,
	MSM_RPM_9615_STATUS_ID_SYSTEM_FABRIC_ARB		= 18,
	MSM_RPM_9615_STATUS_ID_PM8018_S1_0			= 19,
	MSM_RPM_9615_STATUS_ID_PM8018_S1_1			= 20,
	MSM_RPM_9615_STATUS_ID_PM8018_S2_0			= 21,
	MSM_RPM_9615_STATUS_ID_PM8018_S2_1			= 22,
	MSM_RPM_9615_STATUS_ID_PM8018_S3_0			= 23,
	MSM_RPM_9615_STATUS_ID_PM8018_S3_1			= 24,
	MSM_RPM_9615_STATUS_ID_PM8018_S4_0			= 25,
	MSM_RPM_9615_STATUS_ID_PM8018_S4_1			= 26,
	MSM_RPM_9615_STATUS_ID_PM8018_S5_0			= 27,
	MSM_RPM_9615_STATUS_ID_PM8018_S5_1			= 28,
	MSM_RPM_9615_STATUS_ID_PM8018_L1_0			= 29,
	MSM_RPM_9615_STATUS_ID_PM8018_L1_1			= 30,
	MSM_RPM_9615_STATUS_ID_PM8018_L2_0			= 31,
	MSM_RPM_9615_STATUS_ID_PM8018_L2_1			= 32,
	MSM_RPM_9615_STATUS_ID_PM8018_L3_0			= 33,
	MSM_RPM_9615_STATUS_ID_PM8018_L3_1			= 34,
	MSM_RPM_9615_STATUS_ID_PM8018_L4_0			= 35,
	MSM_RPM_9615_STATUS_ID_PM8018_L4_1			= 36,
	MSM_RPM_9615_STATUS_ID_PM8018_L5_0			= 37,
	MSM_RPM_9615_STATUS_ID_PM8018_L5_1			= 38,
	MSM_RPM_9615_STATUS_ID_PM8018_L6_0			= 39,
	MSM_RPM_9615_STATUS_ID_PM8018_L6_1			= 40,
	MSM_RPM_9615_STATUS_ID_PM8018_L7_0			= 41,
	MSM_RPM_9615_STATUS_ID_PM8018_L7_1			= 42,
	MSM_RPM_9615_STATUS_ID_PM8018_L8_0			= 43,
	MSM_RPM_9615_STATUS_ID_PM8018_L8_1			= 44,
	MSM_RPM_9615_STATUS_ID_PM8018_L9_0			= 45,
	MSM_RPM_9615_STATUS_ID_PM8018_L9_1			= 46,
	MSM_RPM_9615_STATUS_ID_PM8018_L10_0			= 47,
	MSM_RPM_9615_STATUS_ID_PM8018_L10_1			= 48,
	MSM_RPM_9615_STATUS_ID_PM8018_L11_0			= 49,
	MSM_RPM_9615_STATUS_ID_PM8018_L11_1			= 50,
	MSM_RPM_9615_STATUS_ID_PM8018_L12_0			= 51,
	MSM_RPM_9615_STATUS_ID_PM8018_L12_1			= 52,
	MSM_RPM_9615_STATUS_ID_PM8018_L13_0			= 53,
	MSM_RPM_9615_STATUS_ID_PM8018_L13_1			= 54,
	MSM_RPM_9615_STATUS_ID_PM8018_L14_0			= 55,
	MSM_RPM_9615_STATUS_ID_PM8018_L14_1			= 56,
	MSM_RPM_9615_STATUS_ID_PM8018_LVS1			= 57,
	MSM_RPM_9615_STATUS_ID_NCP_0				= 58,
	MSM_RPM_9615_STATUS_ID_NCP_1				= 59,
	MSM_RPM_9615_STATUS_ID_CXO_BUFFERS			= 60,
	MSM_RPM_9615_STATUS_ID_USB_OTG_SWITCH			= 61,
	MSM_RPM_9615_STATUS_ID_HDMI_SWITCH			= 62,
	MSM_RPM_9615_STATUS_ID_VOLTAGE_CORNER			= 64,

	MSM_RPM_9615_STATUS_ID_LAST = MSM_RPM_9615_STATUS_ID_VOLTAGE_CORNER,
};

#endif /*                                */
