/*
 * Memory pre-allocations for Cronus boxes.
 *
 * Copyright (C) 2005-2009 Scientific-Atlanta, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Author:       Ken Eppinett
 *               David Schleef <ds@schleef.org>
 */

#include <linux/init.h>
#include <linux/ioport.h>
#include <asm/mach-powertv/asic.h>
#include "prealloc.h"

/*
                               
 */
struct resource dvr_cronus_resources[] __initdata =
{
	/*
                
  */
	/*                         */
	PREALLOC_NORMAL("ST231aImage", 0x24000000, 0x24200000-1,
		IORESOURCE_MEM)
	/*                           */
	PREALLOC_NORMAL("ST231aMonitor", 0x24200000, 0x24202000-1,
		IORESOURCE_MEM)
	/*                                                   */
	PREALLOC_NORMAL("MediaMemory1", 0x24202000, 0x26000000-1,
		IORESOURCE_MEM)

	/*
                
  */
	/*                         */
	PREALLOC_NORMAL("ST231bImage", 0x60000000, 0x60200000-1,
		IORESOURCE_MEM)
	/*                           */
	PREALLOC_NORMAL("ST231bMonitor", 0x60200000, 0x60202000-1,
		IORESOURCE_MEM)
	/*                                                   */
	PREALLOC_NORMAL("MediaMemory2", 0x60202000, 0x62000000-1,
		IORESOURCE_MEM)

	/*
                   
  */
	/*                                 */
	PREALLOC_NORMAL("DSP_Image_Buff", 0x00000000, 0x00100000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))
	/*                             */
	PREALLOC_NORMAL("ADSC_CPU_PCM_Buff", 0x00000000, 0x0000A000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))
	/*                          */
	PREALLOC_NORMAL("ADSC_AUX_Buff", 0x00000000, 0x00020000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))
	/*                           */
	PREALLOC_NORMAL("ADSC_Main_Buff", 0x00000000, 0x00020000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
                       
   
                                                                       
                                                                      
                                                                 
                                                                   
                                                             
  */
	/*       */
	PREALLOC_NORMAL("AVMEMPartition0", 0x00000000, 0x00c00000-1,
		IORESOURCE_MEM)

	/*
                    
  */
	/*      */
	PREALLOC_DOCSIS("Docsis", 0x67500000, 0x67c00000-1, IORESOURCE_MEM)

	/*
                  
  */
	/*                               */
	PREALLOC_NORMAL("GraphicsHeap", 0x62700000, 0x63500000-1,
		IORESOURCE_MEM)

	/*
                         
  */
	/*        */
	PREALLOC_NORMAL("MulticomSHM", 0x26000000, 0x26020000-1,
		IORESOURCE_MEM)

	/*
                   
  */
	PREALLOC_NORMAL("BMM_Buffer", 0x00000000, 0x002EA000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
                                 
  */
	/*      */
	PREALLOC_NORMAL("DisplayBins0", 0x00000000, 0x00001000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
                                 
  */
	/*      */
	PREALLOC_NORMAL("DisplayBins1", 0x00000000, 0x00001000-1,
		IORESOURCE_MEM)

	/*
        
  */
	/*                                           */
	PREALLOC_NORMAL("ITFS", 0x00000000, 0x0018E000-1, IORESOURCE_MEM)

	/*
        
  */
	/*                                                */
	PREALLOC_NORMAL("AvfsDmaMem", 0x00000000, 0x007c2000-1,
		IORESOURCE_MEM)

	/*      */
	PREALLOC_NORMAL("AvfsFileSys", 0x00000000, 0x00001000-1,
		IORESOURCE_MEM)

	/*
        
  */
	/*                                           */
	PREALLOC_PMEM("DiagPersistentMemory", 0x00000000, 0x10000-1,
	     (IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
             
  */
	/*                                                            */
	PREALLOC_NORMAL("SmartCardInfo", 0x00000000, 0x2800-1,
		IORESOURCE_MEM)

	/*
          
  */
	/*                                                       */
	PREALLOC_NORMAL("NP_Reset_Vector", 0x27c00000, 0x27c01000-1,
		IORESOURCE_MEM)
	/*                                          */
	PREALLOC_NORMAL("NP_Image", 0x27020000, 0x27070000-1, IORESOURCE_MEM)
	/*                                        */
	PREALLOC_NORMAL("NP_IPC", 0x63500000, 0x63580000-1, IORESOURCE_MEM)

	/*
              
   
                                                                     
                                        
  */
	PREALLOC_TFTP("TFTPBuffer", 0x00000000, MEBIBYTE(80)-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
                            
  */

	/*
                          
  */
	{
		.flags  = 0,
	},
};

/*
                                   
 */
struct resource non_dvr_cronus_resources[] __initdata =
{
	/*
                
  */
	/*                         */
	PREALLOC_NORMAL("ST231aImage", 0x24000000, 0x24200000-1,
		IORESOURCE_MEM)
	/*                           */
	PREALLOC_NORMAL("ST231aMonitor", 0x24200000, 0x24202000-1,
		IORESOURCE_MEM)
	/*                                                   */
	PREALLOC_NORMAL("MediaMemory1", 0x24202000, 0x26000000-1,
		IORESOURCE_MEM)

	/*
                
  */
	/*                         */
	PREALLOC_NORMAL("ST231bImage", 0x60000000, 0x60200000-1,
		IORESOURCE_MEM)
	/*                           */
	PREALLOC_NORMAL("ST231bMonitor", 0x60200000, 0x60202000-1,
		IORESOURCE_MEM)
	/*                                                   */
	PREALLOC_NORMAL("MediaMemory2", 0x60202000, 0x62000000-1,
		IORESOURCE_MEM)

	/*
                   
  */
	/*                                 */
	PREALLOC_NORMAL("DSP_Image_Buff", 0x00000000, 0x00100000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))
	/*                             */
	PREALLOC_NORMAL("ADSC_CPU_PCM_Buff", 0x00000000, 0x0000A000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))
	/*                          */
	PREALLOC_NORMAL("ADSC_AUX_Buff", 0x00000000, 0x00020000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))
	/*                           */
	PREALLOC_NORMAL("ADSC_Main_Buff", 0x00000000, 0x00020000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
                       
   
                                                                       
                                                                      
                                                                 
                                                                   
                                                             
  */
	/*       */
	PREALLOC_NORMAL("AVMEMPartition0", 0x00000000, 0x00c00000-1,
		IORESOURCE_MEM)

	/*
                    
  */
	/*      */
	PREALLOC_DOCSIS("Docsis", 0x67500000, 0x67c00000-1, IORESOURCE_MEM)

	/*
                  
  */
	/*                               */
	PREALLOC_NORMAL("GraphicsHeap", 0x62700000, 0x63500000-1,
		IORESOURCE_MEM)

	/*
                         
  */
	/*        */
	PREALLOC_NORMAL("MulticomSHM", 0x26000000, 0x26020000-1,
		IORESOURCE_MEM)

	/*
                                                  
  */
	/*        */
	PREALLOC_NORMAL("BMM_Buffer", 0x00000000, 0x000AA000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
                                 
  */
	/*      */
	PREALLOC_NORMAL("DisplayBins0", 0x00000000, 0x00001000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
                                 
  */
	/*      */
	PREALLOC_NORMAL("DisplayBins1", 0x00000000, 0x00001000-1,
		IORESOURCE_MEM)

	/*
                           
  */
	/*                       */
	PREALLOC_NORMAL("AvfsDmaMem", 0x00000000, 0x002c4c00-1, IORESOURCE_MEM)

	/*
        
  */
	/*                                           */
	PREALLOC_PMEM("DiagPersistentMemory", 0x00000000, 0x10000-1,
	     (IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
             
  */
	/*                                                            */
	PREALLOC_NORMAL("SmartCardInfo", 0x00000000, 0x2800-1, IORESOURCE_MEM)

	/*
          
  */
	/*                                                       */
	PREALLOC_NORMAL("NP_Reset_Vector", 0x27c00000, 0x27c01000-1,
		IORESOURCE_MEM)
	/*                                          */
	PREALLOC_NORMAL("NP_Image", 0x27020000, 0x27070000-1, IORESOURCE_MEM)
	/*                                        */
	PREALLOC_NORMAL("NP_IPC", 0x63500000, 0x63580000-1, IORESOURCE_MEM)

	/*
              
  */
	/*       */
	PREALLOC_NORMAL("NandFlash", NAND_FLASH_BASE, NAND_FLASH_BASE+0x400-1,
		IORESOURCE_MEM)

	/*
                            
  */

	/*
                          
  */
	{
		.flags  = 0,
	},
};
