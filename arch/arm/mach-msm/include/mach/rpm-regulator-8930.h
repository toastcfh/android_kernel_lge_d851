/*
 * Copyright (c) 2012, The Linux Foundation. All rights reserved.
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

#ifndef __ARCH_ARM_MACH_MSM_INCLUDE_MACH_RPM_REGULATOR_8930_H
#define __ARCH_ARM_MACH_MSM_INCLUDE_MACH_RPM_REGULATOR_8930_H

/*                            */
#define RPM_VREG_PIN_CTRL_PM8038_D1	0x01
#define RPM_VREG_PIN_CTRL_PM8038_A0	0x02
#define RPM_VREG_PIN_CTRL_PM8038_A1	0x04
#define RPM_VREG_PIN_CTRL_PM8038_A2	0x08

/* 
                                                                 
                                                                          
                                          
                                       
                                                                         
             
                                                                        
                                                                   
                                 
                                                                        
                                         
                          
  
                                                                              
                                                                           
                             
 */
enum rpm_vreg_pin_fn_8930 {
	RPM_VREG_PIN_FN_8930_DONT_CARE,
	RPM_VREG_PIN_FN_8930_ENABLE,
	RPM_VREG_PIN_FN_8930_MODE,
	RPM_VREG_PIN_FN_8930_SLEEP_B,
	RPM_VREG_PIN_FN_8930_NONE,
};

/* 
                                                                   
                                                              
                                                        
                                                           
                                                                          
                                             
                                    
                  
                                                            
                                                                           
                                          
                                          
  
                                                                           
                           
 */
enum rpm_vreg_force_mode_8930 {
	RPM_VREG_FORCE_MODE_8930_PIN_CTRL = 0,
	RPM_VREG_FORCE_MODE_8930_NONE = 0,
	RPM_VREG_FORCE_MODE_8930_LPM,
	RPM_VREG_FORCE_MODE_8930_AUTO,		/*           */
	RPM_VREG_FORCE_MODE_8930_HPM,
	RPM_VREG_FORCE_MODE_8930_BYPASS,	/*          */
};

/* 
                                                                 
                                                                             
                               
                                                                      
                         
 */
enum rpm_vreg_power_mode_8930 {
	RPM_VREG_POWER_MODE_8930_HYSTERETIC,
	RPM_VREG_POWER_MODE_8930_PWM,
};

/* 
                                                                         
                                           
 */
enum rpm_vreg_id_8930_pm8038 {
	RPM_VREG_ID_PM8038_L1,
	RPM_VREG_ID_PM8038_L2,
	RPM_VREG_ID_PM8038_L3,
	RPM_VREG_ID_PM8038_L4,
	RPM_VREG_ID_PM8038_L5,
	RPM_VREG_ID_PM8038_L6,
	RPM_VREG_ID_PM8038_L7,
	RPM_VREG_ID_PM8038_L8,
	RPM_VREG_ID_PM8038_L9,
	RPM_VREG_ID_PM8038_L10,
	RPM_VREG_ID_PM8038_L11,
	RPM_VREG_ID_PM8038_L12,
	RPM_VREG_ID_PM8038_L13,
	RPM_VREG_ID_PM8038_L14,
	RPM_VREG_ID_PM8038_L15,
	RPM_VREG_ID_PM8038_L16,
	RPM_VREG_ID_PM8038_L17,
	RPM_VREG_ID_PM8038_L18,
	RPM_VREG_ID_PM8038_L19,
	RPM_VREG_ID_PM8038_L20,
	RPM_VREG_ID_PM8038_L21,
	RPM_VREG_ID_PM8038_L22,
	RPM_VREG_ID_PM8038_L23,
	RPM_VREG_ID_PM8038_L24,
	RPM_VREG_ID_PM8038_L25,
	RPM_VREG_ID_PM8038_L26,
	RPM_VREG_ID_PM8038_L27,
	RPM_VREG_ID_PM8038_S1,
	RPM_VREG_ID_PM8038_S2,
	RPM_VREG_ID_PM8038_S3,
	RPM_VREG_ID_PM8038_S4,
	RPM_VREG_ID_PM8038_S5,
	RPM_VREG_ID_PM8038_S6,
	RPM_VREG_ID_PM8038_LVS1,
	RPM_VREG_ID_PM8038_LVS2,
	RPM_VREG_ID_PM8038_VDD_DIG_CORNER,
	RPM_VREG_ID_PM8038_MAX_REAL = RPM_VREG_ID_PM8038_VDD_DIG_CORNER,

	/*                                                                    */
	RPM_VREG_ID_PM8038_L2_PC,
	RPM_VREG_ID_PM8038_L3_PC,
	RPM_VREG_ID_PM8038_L4_PC,
	RPM_VREG_ID_PM8038_L5_PC,
	RPM_VREG_ID_PM8038_L6_PC,
	RPM_VREG_ID_PM8038_L7_PC,
	RPM_VREG_ID_PM8038_L8_PC,
	RPM_VREG_ID_PM8038_L9_PC,
	RPM_VREG_ID_PM8038_L10_PC,
	RPM_VREG_ID_PM8038_L11_PC,
	RPM_VREG_ID_PM8038_L12_PC,
	RPM_VREG_ID_PM8038_L14_PC,
	RPM_VREG_ID_PM8038_L15_PC,
	RPM_VREG_ID_PM8038_L17_PC,
	RPM_VREG_ID_PM8038_L18_PC,
	RPM_VREG_ID_PM8038_L21_PC,
	RPM_VREG_ID_PM8038_L22_PC,
	RPM_VREG_ID_PM8038_L23_PC,
	RPM_VREG_ID_PM8038_L26_PC,
	RPM_VREG_ID_PM8038_S1_PC,
	RPM_VREG_ID_PM8038_S2_PC,
	RPM_VREG_ID_PM8038_S3_PC,
	RPM_VREG_ID_PM8038_S4_PC,
	RPM_VREG_ID_PM8038_LVS1_PC,
	RPM_VREG_ID_PM8038_LVS2_PC,
	RPM_VREG_ID_PM8038_MAX = RPM_VREG_ID_PM8038_LVS2_PC,
};

/* 
                                                                         
                                           
 */
enum rpm_vreg_id_8930_pm8917 {
	RPM_VREG_ID_PM8917_L1,
	RPM_VREG_ID_PM8917_L2,
	RPM_VREG_ID_PM8917_L3,
	RPM_VREG_ID_PM8917_L4,
	RPM_VREG_ID_PM8917_L5,
	RPM_VREG_ID_PM8917_L6,
	RPM_VREG_ID_PM8917_L7,
	RPM_VREG_ID_PM8917_L8,
	RPM_VREG_ID_PM8917_L9,
	RPM_VREG_ID_PM8917_L10,
	RPM_VREG_ID_PM8917_L11,
	RPM_VREG_ID_PM8917_L12,
	RPM_VREG_ID_PM8917_L14,
	RPM_VREG_ID_PM8917_L15,
	RPM_VREG_ID_PM8917_L16,
	RPM_VREG_ID_PM8917_L17,
	RPM_VREG_ID_PM8917_L18,
	RPM_VREG_ID_PM8917_L21,
	RPM_VREG_ID_PM8917_L22,
	RPM_VREG_ID_PM8917_L23,
	RPM_VREG_ID_PM8917_L24,
	RPM_VREG_ID_PM8917_L25,
	RPM_VREG_ID_PM8917_L26,
	RPM_VREG_ID_PM8917_L27,
	RPM_VREG_ID_PM8917_L28,
	RPM_VREG_ID_PM8917_L29,
	RPM_VREG_ID_PM8917_L30,
	RPM_VREG_ID_PM8917_L31,
	RPM_VREG_ID_PM8917_L32,
	RPM_VREG_ID_PM8917_L33,
	RPM_VREG_ID_PM8917_L34,
	RPM_VREG_ID_PM8917_L35,
	RPM_VREG_ID_PM8917_L36,
	RPM_VREG_ID_PM8917_S1,
	RPM_VREG_ID_PM8917_S2,
	RPM_VREG_ID_PM8917_S3,
	RPM_VREG_ID_PM8917_S4,
	RPM_VREG_ID_PM8917_S5,
	RPM_VREG_ID_PM8917_S6,
	RPM_VREG_ID_PM8917_S7,
	RPM_VREG_ID_PM8917_S8,
	RPM_VREG_ID_PM8917_LVS1,
	RPM_VREG_ID_PM8917_LVS3,
	RPM_VREG_ID_PM8917_LVS4,
	RPM_VREG_ID_PM8917_LVS5,
	RPM_VREG_ID_PM8917_LVS6,
	RPM_VREG_ID_PM8917_LVS7,
	RPM_VREG_ID_PM8917_USB_OTG,
	RPM_VREG_ID_PM8917_VDD_DIG_CORNER,
	RPM_VREG_ID_PM8917_MAX_REAL = RPM_VREG_ID_PM8917_VDD_DIG_CORNER,

	/*                                                                    */
	RPM_VREG_ID_PM8917_L1_PC,
	RPM_VREG_ID_PM8917_L2_PC,
	RPM_VREG_ID_PM8917_L3_PC,
	RPM_VREG_ID_PM8917_L4_PC,
	RPM_VREG_ID_PM8917_L5_PC,
	RPM_VREG_ID_PM8917_L6_PC,
	RPM_VREG_ID_PM8917_L7_PC,
	RPM_VREG_ID_PM8917_L8_PC,
	RPM_VREG_ID_PM8917_L9_PC,
	RPM_VREG_ID_PM8917_L10_PC,
	RPM_VREG_ID_PM8917_L11_PC,
	RPM_VREG_ID_PM8917_L12_PC,
	RPM_VREG_ID_PM8917_L14_PC,
	RPM_VREG_ID_PM8917_L15_PC,
	RPM_VREG_ID_PM8917_L16_PC,
	RPM_VREG_ID_PM8917_L17_PC,
	RPM_VREG_ID_PM8917_L18_PC,
	RPM_VREG_ID_PM8917_L21_PC,
	RPM_VREG_ID_PM8917_L22_PC,
	RPM_VREG_ID_PM8917_L23_PC,

	RPM_VREG_ID_PM8917_L29_PC,
	RPM_VREG_ID_PM8917_L30_PC,
	RPM_VREG_ID_PM8917_L31_PC,
	RPM_VREG_ID_PM8917_L32_PC,
	RPM_VREG_ID_PM8917_L33_PC,
	RPM_VREG_ID_PM8917_L34_PC,
	RPM_VREG_ID_PM8917_L35_PC,
	RPM_VREG_ID_PM8917_L36_PC,
	RPM_VREG_ID_PM8917_S1_PC,
	RPM_VREG_ID_PM8917_S2_PC,
	RPM_VREG_ID_PM8917_S3_PC,
	RPM_VREG_ID_PM8917_S4_PC,

	RPM_VREG_ID_PM8917_S7_PC,
	RPM_VREG_ID_PM8917_S8_PC,
	RPM_VREG_ID_PM8917_LVS1_PC,
	RPM_VREG_ID_PM8917_LVS3_PC,
	RPM_VREG_ID_PM8917_LVS4_PC,
	RPM_VREG_ID_PM8917_LVS5_PC,
	RPM_VREG_ID_PM8917_LVS6_PC,
	RPM_VREG_ID_PM8917_LVS7_PC,

	RPM_VREG_ID_PM8917_MAX = RPM_VREG_ID_PM8917_LVS7_PC,
};

/*                                      */
#define RPM_VREG_8930_LDO_5_HPM_MIN_LOAD		0
#define RPM_VREG_8930_LDO_50_HPM_MIN_LOAD		5000
#define RPM_VREG_8930_LDO_150_HPM_MIN_LOAD		10000
#define RPM_VREG_8930_LDO_300_HPM_MIN_LOAD		10000
#define RPM_VREG_8930_LDO_600_HPM_MIN_LOAD		10000
#define RPM_VREG_8930_LDO_1200_HPM_MIN_LOAD		10000
#define RPM_VREG_8930_SMPS_1500_HPM_MIN_LOAD		100000
#define RPM_VREG_8930_SMPS_2000_HPM_MIN_LOAD		100000

#endif
