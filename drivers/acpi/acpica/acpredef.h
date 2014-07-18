/*                                                                             
  
                                                                             
                                
  
                                                                             */

/*
 * Copyright (C) 2000 - 2012, Intel Corp.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    substantially similar to the "NO WARRANTY" disclaimer below
 *    ("Disclaimer") and any redistribution must be conditioned upon
 *    including a substantially similar Disclaimer requirement for further
 *    binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 */

#ifndef __ACPREDEF_H__
#define __ACPREDEF_H__

/*                                                                             
  
                       
  
                                                  
  
                                                        
                   
             
                   
             
  
                                    
                                 
  
                                                                           
                       
  
  
                                                                            
                                                                               
  
                                                             
                   
             
                   
             
                                           
  
                                                                   
                   
                                 
  
                                                                             
                   
             
                   
             
                       
  
                                                        
                       
  
                                                                     
                       
  
                                                                            
                             
  
                                                                              
                                       
                   
             
                   
                            
                       
  
                                                                             */

enum acpi_return_package_types {
	ACPI_PTYPE1_FIXED = 1,
	ACPI_PTYPE1_VAR = 2,
	ACPI_PTYPE1_OPTION = 3,
	ACPI_PTYPE2 = 4,
	ACPI_PTYPE2_COUNT = 5,
	ACPI_PTYPE2_PKG_COUNT = 6,
	ACPI_PTYPE2_FIXED = 7,
	ACPI_PTYPE2_MIN = 8,
	ACPI_PTYPE2_REV_FIXED = 9,
	ACPI_PTYPE2_FIX_VAR = 10
};

#ifdef ACPI_CREATE_PREDEFINED_TABLE
/*
                                              
  
                                                                               
                                               
  
                                                                                       
                                     
                           
                                             
  
                                                                          
                                                      
  
                                                               
                
  
                                                                              
                      
  
                                                                  
  
                                                
                                                           
                                                               
                                                                   
  
                                                                         
                                                                            
                                                                           
                                   
 */
static const union acpi_predefined_info predefined_names[] =
{
	{{"_AC0", 0, ACPI_RTYPE_INTEGER}},
	{{"_AC1", 0, ACPI_RTYPE_INTEGER}},
	{{"_AC2", 0, ACPI_RTYPE_INTEGER}},
	{{"_AC3", 0, ACPI_RTYPE_INTEGER}},
	{{"_AC4", 0, ACPI_RTYPE_INTEGER}},
	{{"_AC5", 0, ACPI_RTYPE_INTEGER}},
	{{"_AC6", 0, ACPI_RTYPE_INTEGER}},
	{{"_AC7", 0, ACPI_RTYPE_INTEGER}},
	{{"_AC8", 0, ACPI_RTYPE_INTEGER}},
	{{"_AC9", 0, ACPI_RTYPE_INTEGER}},
	{{"_ADR", 0, ACPI_RTYPE_INTEGER}},
	{{"_AEI", 0, ACPI_RTYPE_BUFFER}},
	{{"_AL0", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_AL1", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_AL2", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_AL3", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_AL4", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_AL5", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_AL6", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_AL7", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_AL8", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_AL9", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_ALC", 0, ACPI_RTYPE_INTEGER}},
	{{"_ALI", 0, ACPI_RTYPE_INTEGER}},
	{{"_ALP", 0, ACPI_RTYPE_INTEGER}},
	{{"_ALR", 0, ACPI_RTYPE_PACKAGE}}, /*                                      */
			  {{{ACPI_PTYPE2, ACPI_RTYPE_INTEGER, 2,0}, 0,0}},

	{{"_ALT", 0, ACPI_RTYPE_INTEGER}},
	{{"_ART", 0, ACPI_RTYPE_PACKAGE}},	/*                                                   */
	{{{ACPI_PTYPE2_REV_FIXED, ACPI_RTYPE_REFERENCE, 2, ACPI_RTYPE_INTEGER},
	  11, 0}},

	{{"_BBN", 0, ACPI_RTYPE_INTEGER}},
	{{"_BCL", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_INTEGER, 0,0}, 0,0}},

	{{"_BCM", 1, 0}},
	{{"_BCT", 1, ACPI_RTYPE_INTEGER}},
	{{"_BDN", 0, ACPI_RTYPE_INTEGER}},
	{{"_BFS", 1, 0}},
	{{"_BIF", 0, ACPI_RTYPE_PACKAGE} }, /*                                  */
			  {{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_INTEGER, 9,
			     ACPI_RTYPE_STRING | ACPI_RTYPE_BUFFER}, 4, 0} },

	{{"_BIX", 0, ACPI_RTYPE_PACKAGE}},	/*                               */
	{{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_INTEGER, 16, ACPI_RTYPE_STRING}, 4,
	  0}},

	{{"_BLT", 3, 0}},
	{{"_BMA", 1, ACPI_RTYPE_INTEGER}},
	{{"_BMC", 1, 0}},
	{{"_BMD", 0, ACPI_RTYPE_PACKAGE}}, /*                      */
			  {{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_INTEGER, 5,0}, 0,0}},

	{{"_BMS", 1, ACPI_RTYPE_INTEGER}},
	{{"_BQC", 0, ACPI_RTYPE_INTEGER}},
	{{"_BST", 0, ACPI_RTYPE_PACKAGE}}, /*                      */
			  {{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_INTEGER, 4,0}, 0,0}},

	{{"_BTM", 1, ACPI_RTYPE_INTEGER}},
	{{"_BTP", 1, 0}},
	{{"_CBA", 0, ACPI_RTYPE_INTEGER}}, /*                           */
	{{"_CDM", 0, ACPI_RTYPE_INTEGER}},
	{{"_CID", 0, ACPI_RTYPE_INTEGER | ACPI_RTYPE_STRING | ACPI_RTYPE_PACKAGE}}, /*                             */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_INTEGER | ACPI_RTYPE_STRING, 0,0}, 0,0}},

	{{"_CLS", 0, ACPI_RTYPE_PACKAGE}},	/*                      */
	{{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_INTEGER, 3, 0}, 0, 0}},

	{{"_CPC", 0, ACPI_RTYPE_PACKAGE}},	/*                             */
	{{{ACPI_PTYPE1_VAR, ACPI_RTYPE_INTEGER | ACPI_RTYPE_BUFFER, 0, 0}, 0,
	  0}},

	{{"_CRS", 0, ACPI_RTYPE_BUFFER}},
	{{"_CRT", 0, ACPI_RTYPE_INTEGER}},
	{{"_CSD", 0, ACPI_RTYPE_PACKAGE}}, /*                                     */
			  {{{ACPI_PTYPE2_COUNT, ACPI_RTYPE_INTEGER, 0,0}, 0,0}},

	{{"_CST", 0, ACPI_RTYPE_PACKAGE}}, /*                                                */
			  {{{ACPI_PTYPE2_PKG_COUNT,ACPI_RTYPE_BUFFER, 1, ACPI_RTYPE_INTEGER}, 3,0}},

	{{"_CWS", 1, ACPI_RTYPE_INTEGER}},
	{{"_DCK", 1, ACPI_RTYPE_INTEGER}},
	{{"_DCS", 0, ACPI_RTYPE_INTEGER}},
	{{"_DDC", 1, ACPI_RTYPE_INTEGER | ACPI_RTYPE_BUFFER}},
	{{"_DDN", 0, ACPI_RTYPE_STRING}},
	{{"_DEP", 0, ACPI_RTYPE_PACKAGE}},	/*                        */
	{{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0, 0}, 0, 0}},

	{{"_DGS", 0, ACPI_RTYPE_INTEGER}},
	{{"_DIS", 0, 0}},

	{{"_DLM", 0, ACPI_RTYPE_PACKAGE}},	/*                                                           */
	{{{ACPI_PTYPE2_FIX_VAR, ACPI_RTYPE_REFERENCE, 1,
	   ACPI_RTYPE_REFERENCE | ACPI_RTYPE_BUFFER}, 0, 0}},

	{{"_DMA", 0, ACPI_RTYPE_BUFFER}},
	{{"_DOD", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_INTEGER, 0,0}, 0,0}},

	{{"_DOS", 1, 0}},
	{{"_DSM", 4, ACPI_RTYPE_ALL}},     /*                                               */
	{{"_DSS", 1, 0}},
	{{"_DSW", 3, 0}},
	{{"_DTI", 1, 0}},
	{{"_EC_", 0, ACPI_RTYPE_INTEGER}},
	{{"_EDL", 0, ACPI_RTYPE_PACKAGE}}, /*                       */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_EJ0", 1, 0}},
	{{"_EJ1", 1, 0}},
	{{"_EJ2", 1, 0}},
	{{"_EJ3", 1, 0}},
	{{"_EJ4", 1, 0}},
	{{"_EJD", 0, ACPI_RTYPE_STRING}},
	{{"_EVT", 1, 0}},
	{{"_FDE", 0, ACPI_RTYPE_BUFFER}},
	{{"_FDI", 0, ACPI_RTYPE_PACKAGE}}, /*                       */
			  {{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_INTEGER, 16,0}, 0,0}},

	{{"_FDM", 1, 0}},
	{{"_FIF", 0, ACPI_RTYPE_PACKAGE}},	/*                      */
			  {{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_INTEGER, 4, 0}, 0, 0}},

	{{"_FIX", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_INTEGER, 0,0}, 0,0}},

	{{"_FPS", 0, ACPI_RTYPE_PACKAGE}},	/*                                            */
	{{{ACPI_PTYPE2_REV_FIXED, ACPI_RTYPE_INTEGER, 5, 0}, 0, 0}},

	{{"_FSL", 1, 0}},
	{{"_FST", 0, ACPI_RTYPE_PACKAGE}},	/*                      */
	{{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_INTEGER, 3, 0}, 0, 0}},

	{{"_GAI", 0, ACPI_RTYPE_INTEGER}},
	{{"_GCP", 0, ACPI_RTYPE_INTEGER}},
	{{"_GHL", 0, ACPI_RTYPE_INTEGER}},
	{{"_GLK", 0, ACPI_RTYPE_INTEGER}},
	{{"_GPD", 0, ACPI_RTYPE_INTEGER}},
	{{"_GPE", 0, ACPI_RTYPE_INTEGER}}, /*                             */
	{{"_GRT", 0, ACPI_RTYPE_BUFFER}},
	{{"_GSB", 0, ACPI_RTYPE_INTEGER}},
	{{"_GTF", 0, ACPI_RTYPE_BUFFER}},
	{{"_GTM", 0, ACPI_RTYPE_BUFFER}},
	{{"_GTS", 1, 0}},
	{{"_GWS", 1, ACPI_RTYPE_INTEGER}},
	{{"_HID", 0, ACPI_RTYPE_INTEGER | ACPI_RTYPE_STRING}},
	{{"_HOT", 0, ACPI_RTYPE_INTEGER}},
	{{"_HPP", 0, ACPI_RTYPE_PACKAGE}}, /*                      */
			  {{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_INTEGER, 4,0}, 0,0}},

	/*
                                                                               
                                                                    
                                                                     
                                                           
  */
	{{"_HPX", 0, ACPI_RTYPE_PACKAGE}}, /*                                        */
			  {{{ACPI_PTYPE2_MIN, ACPI_RTYPE_INTEGER, 5,0}, 0,0}},

	{{"_HRV", 0, ACPI_RTYPE_INTEGER}},
	{{"_IFT", 0, ACPI_RTYPE_INTEGER}}, /*               */
	{{"_INI", 0, 0}},
	{{"_IRC", 0, 0}},
	{{"_LCK", 1, 0}},
	{{"_LID", 0, ACPI_RTYPE_INTEGER}},
	{{"_MAT", 0, ACPI_RTYPE_BUFFER}},
	{{"_MBM", 0, ACPI_RTYPE_PACKAGE}},	/*                      */
	{{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_INTEGER, 8, 0}, 0, 0}},

	{{"_MLS", 0, ACPI_RTYPE_PACKAGE}}, /*                                     */
			  {{{ACPI_PTYPE2, ACPI_RTYPE_STRING, 2,0}, 0,0}},

	{{"_MSG", 1, 0}},
	{{"_MSM", 4, ACPI_RTYPE_INTEGER}},
	{{"_NTT", 0, ACPI_RTYPE_INTEGER}},
	{{"_OFF", 0, 0}},
	{{"_ON_", 0, 0}},
	{{"_OS_", 0, ACPI_RTYPE_STRING}},
	{{"_OSC", 4, ACPI_RTYPE_BUFFER}},
	{{"_OST", 3, 0}},
	{{"_PAI", 1, ACPI_RTYPE_INTEGER}},
	{{"_PCL", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_PCT", 0, ACPI_RTYPE_PACKAGE}}, /*                      */
			  {{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_BUFFER, 2,0}, 0,0}},

	{{"_PDC", 1, 0}},
	{{"_PDL", 0, ACPI_RTYPE_INTEGER}},
	{{"_PIC", 1, 0}},
	{{"_PIF", 0, ACPI_RTYPE_PACKAGE}},	/*                              */
	{{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_INTEGER, 3, ACPI_RTYPE_STRING}, 3, 0}},

	{{"_PLD", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_BUFFER, 0,0}, 0,0}},

	{{"_PMC", 0, ACPI_RTYPE_PACKAGE}},	/*                               */
	{{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_INTEGER, 11, ACPI_RTYPE_STRING}, 3,
	  0}},

	{{"_PMD", 0, ACPI_RTYPE_PACKAGE}},	/*                        */
	{{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0, 0}, 0, 0}},

	{{"_PMM", 0, ACPI_RTYPE_INTEGER}},
	{{"_PPC", 0, ACPI_RTYPE_INTEGER}},
	{{"_PPE", 0, ACPI_RTYPE_INTEGER}}, /*                */
	{{"_PR0", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_PR1", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_PR2", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_PR3", 0, ACPI_RTYPE_PACKAGE}},	/*                        */
	{{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0, 0}, 0, 0}},

	{{"_PRE", 0, ACPI_RTYPE_PACKAGE}},	/*                        */
	{{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0, 0}, 0, 0}},

	{{"_PRL", 0, ACPI_RTYPE_PACKAGE}},	/*                        */
	{{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0, 0}, 0, 0}},

	{{"_PRS", 0, ACPI_RTYPE_BUFFER}},

	/*
                                                                         
                                                                         
                                                                         
                                                                     
                                                                           
                           
  */
	{{"_PRT", 0, ACPI_RTYPE_PACKAGE}}, /*                                                      */
			  {{{ACPI_PTYPE2_FIXED, 4, ACPI_RTYPE_INTEGER,ACPI_RTYPE_INTEGER},
			  ACPI_RTYPE_INTEGER | ACPI_RTYPE_REFERENCE,
			  ACPI_RTYPE_INTEGER}},

	{{"_PRW", 0, ACPI_RTYPE_PACKAGE}}, /*                                                                                  */
			  {{{ACPI_PTYPE1_OPTION, 2, ACPI_RTYPE_INTEGER | ACPI_RTYPE_PACKAGE,
			  ACPI_RTYPE_INTEGER}, ACPI_RTYPE_REFERENCE,0}},

	{{"_PS0", 0, 0}},
	{{"_PS1", 0, 0}},
	{{"_PS2", 0, 0}},
	{{"_PS3", 0, 0}},
	{{"_PSC", 0, ACPI_RTYPE_INTEGER}},
	{{"_PSD", 0, ACPI_RTYPE_PACKAGE}}, /*                                                */
			  {{{ACPI_PTYPE2_COUNT, ACPI_RTYPE_INTEGER,0,0}, 0,0}},

	{{"_PSE", 1, 0}},
	{{"_PSL", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_PSR", 0, ACPI_RTYPE_INTEGER}},
	{{"_PSS", 0, ACPI_RTYPE_PACKAGE}}, /*                                     */
			  {{{ACPI_PTYPE2, ACPI_RTYPE_INTEGER, 6,0}, 0,0}},

	{{"_PSV", 0, ACPI_RTYPE_INTEGER}},
	{{"_PSW", 1, 0}},
	{{"_PTC", 0, ACPI_RTYPE_PACKAGE}}, /*                      */
			  {{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_BUFFER, 2,0}, 0,0}},

	{{"_PTP", 2, ACPI_RTYPE_INTEGER}},
	{{"_PTS", 1, 0}},
	{{"_PUR", 0, ACPI_RTYPE_PACKAGE}},	/*                      */
	{{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_INTEGER, 2, 0}, 0, 0}},

	{{"_PXM", 0, ACPI_RTYPE_INTEGER}},
	{{"_REG", 2, 0}},
	{{"_REV", 0, ACPI_RTYPE_INTEGER}},
	{{"_RMV", 0, ACPI_RTYPE_INTEGER}},
	{{"_ROM", 2, ACPI_RTYPE_BUFFER}},
	{{"_RTV", 0, ACPI_RTYPE_INTEGER}},

	/*
                                                                 
                                                                            
                                                                               
                     
  */
	{{"_S0_", 0, ACPI_RTYPE_PACKAGE}}, /*                      */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_INTEGER, 1,0}, 0,0}},

	{{"_S1_", 0, ACPI_RTYPE_PACKAGE}}, /*                      */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_INTEGER, 1,0}, 0,0}},

	{{"_S2_", 0, ACPI_RTYPE_PACKAGE}}, /*                      */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_INTEGER, 1,0}, 0,0}},

	{{"_S3_", 0, ACPI_RTYPE_PACKAGE}}, /*                      */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_INTEGER, 1,0}, 0,0}},

	{{"_S4_", 0, ACPI_RTYPE_PACKAGE}}, /*                      */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_INTEGER, 1,0}, 0,0}},

	{{"_S5_", 0, ACPI_RTYPE_PACKAGE}}, /*                      */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_INTEGER, 1,0}, 0,0}},

	{{"_S1D", 0, ACPI_RTYPE_INTEGER}},
	{{"_S2D", 0, ACPI_RTYPE_INTEGER}},
	{{"_S3D", 0, ACPI_RTYPE_INTEGER}},
	{{"_S4D", 0, ACPI_RTYPE_INTEGER}},
	{{"_S0W", 0, ACPI_RTYPE_INTEGER}},
	{{"_S1W", 0, ACPI_RTYPE_INTEGER}},
	{{"_S2W", 0, ACPI_RTYPE_INTEGER}},
	{{"_S3W", 0, ACPI_RTYPE_INTEGER}},
	{{"_S4W", 0, ACPI_RTYPE_INTEGER}},
	{{"_SBS", 0, ACPI_RTYPE_INTEGER}},
	{{"_SCP", 0x13, 0}},               /*                                                                  */
			   /*                                                        */
	{{"_SDD", 1, 0}},
	{{"_SEG", 0, ACPI_RTYPE_INTEGER}},
	{{"_SHL", 1, ACPI_RTYPE_INTEGER}},
	{{"_SLI", 0, ACPI_RTYPE_BUFFER}},
	{{"_SPD", 1, ACPI_RTYPE_INTEGER}},
	{{"_SRS", 1, 0}},
	{{"_SRT", 1, ACPI_RTYPE_INTEGER}},
	{{"_SRV", 0, ACPI_RTYPE_INTEGER}}, /*               */
	{{"_SST", 1, 0}},
	{{"_STA", 0, ACPI_RTYPE_INTEGER}},
	{{"_STM", 3, 0}},
	{{"_STP", 2, ACPI_RTYPE_INTEGER}},
	{{"_STR", 0, ACPI_RTYPE_BUFFER}},
	{{"_STV", 2, ACPI_RTYPE_INTEGER}},
	{{"_SUB", 0, ACPI_RTYPE_STRING}},
	{{"_SUN", 0, ACPI_RTYPE_INTEGER}},
	{{"_SWS", 0, ACPI_RTYPE_INTEGER}},
	{{"_TC1", 0, ACPI_RTYPE_INTEGER}},
	{{"_TC2", 0, ACPI_RTYPE_INTEGER}},
	{{"_TDL", 0, ACPI_RTYPE_INTEGER}},
	{{"_TIP", 1, ACPI_RTYPE_INTEGER}},
	{{"_TIV", 1, ACPI_RTYPE_INTEGER}},
	{{"_TMP", 0, ACPI_RTYPE_INTEGER}},
	{{"_TPC", 0, ACPI_RTYPE_INTEGER}},
	{{"_TPT", 1, 0}},
	{{"_TRT", 0, ACPI_RTYPE_PACKAGE}}, /*                                         */
			  {{{ACPI_PTYPE2, ACPI_RTYPE_REFERENCE, 2, ACPI_RTYPE_INTEGER}, 6, 0}},

	{{"_TSD", 0, ACPI_RTYPE_PACKAGE}}, /*                                              */
			  {{{ACPI_PTYPE2_COUNT,ACPI_RTYPE_INTEGER, 5,0}, 0,0}},

	{{"_TSP", 0, ACPI_RTYPE_INTEGER}},
	{{"_TSS", 0, ACPI_RTYPE_PACKAGE}}, /*                                   */
			  {{{ACPI_PTYPE2, ACPI_RTYPE_INTEGER, 5,0}, 0,0}},

	{{"_TST", 0, ACPI_RTYPE_INTEGER}},
	{{"_TTS", 1, 0}},
	{{"_TZD", 0, ACPI_RTYPE_PACKAGE}}, /*                        */
			  {{{ACPI_PTYPE1_VAR, ACPI_RTYPE_REFERENCE, 0,0}, 0,0}},

	{{"_TZM", 0, ACPI_RTYPE_REFERENCE}},
	{{"_TZP", 0, ACPI_RTYPE_INTEGER}},
	{{"_UID", 0, ACPI_RTYPE_INTEGER | ACPI_RTYPE_STRING}},
	{{"_UPC", 0, ACPI_RTYPE_PACKAGE}}, /*                      */
			  {{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_INTEGER, 4,0}, 0,0}},

	{{"_UPD", 0, ACPI_RTYPE_INTEGER}},
	{{"_UPP", 0, ACPI_RTYPE_INTEGER}},
	{{"_VPO", 0, ACPI_RTYPE_INTEGER}},

	/*                                                                                       */

	{{"_WAK", 1, ACPI_RTYPE_NONE | ACPI_RTYPE_INTEGER | ACPI_RTYPE_PACKAGE}},
			  {{{ACPI_PTYPE1_FIXED, ACPI_RTYPE_INTEGER, 2,0}, 0,0}}, /*                                       */

	/*                                                                  */

	{{"_WDG", 0, ACPI_RTYPE_BUFFER}},
	{{"_WED", 1,
	  ACPI_RTYPE_INTEGER | ACPI_RTYPE_STRING | ACPI_RTYPE_BUFFER}},

	{{{0, 0, 0, 0}, 0, 0}}  /*                  */
};

#if 0
	/*                                                                    */
	{{"_OSI", 1, ACPI_RTYPE_INTEGER}},

	/*      */

	_PRT - currently ignore reversed entries. attempt to fix here?
	think about possibly fixing package elements like _BIF, etc.
#endif

#endif
#endif
