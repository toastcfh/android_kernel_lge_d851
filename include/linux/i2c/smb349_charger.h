/* Copyright (c) 2012 The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful;
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __SMB349_H__
#define __SMB349_H__

#define SMB349_NAME		"smb349"
/* 
                              
                                                                             
                                                         
                                                          
                                                                               
                                                                
                                                               
                                                                     
                                                                     
 */
struct smb349_platform_data {
	int en_n_gpio;
	int chg_susp_gpio;
	int chg_current_ma;
	int stat_gpio;
	int term_current_ma;
#if defined(CONFIG_MACH_MSM8974_G3_LGU_EVB)
	int otg_en_gpio;
#endif
};

typedef enum {
	MAX17048_TYPE,
	BMS_TYPE,
	GAUGE_IC_TYPE_MAX
} gauge_ic_type;

extern int32_t smb349_is_ready(void);
extern int32_t external_smb349_enable_charging(bool enable);
#ifdef CONFIG_WIRELESS_CHARGER
int set_wireless_power_supply_control(int value);
#ifdef CONFIG_BQ51053B_CHARGER
void set_usb_present(int value);
int get_usb_present(void);
bool external_smb349_is_charger_present(void);
#endif
#endif
#ifdef CONFIG_SENSORS_QPNP_ADC_VOLTAGE
extern int smb349_get_batt_temp_origin(void);
#endif

#endif
