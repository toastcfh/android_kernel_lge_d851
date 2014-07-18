/*
 * Copyright (c) 2003 VIA Networking, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *
 * File: tether.c
 *
 * Purpose:
 *
 * Author: Tevin Chen
 *
 * Date: May 21, 1996
 *
 * Functions:
 *      ETHbyGetHashIndexByCrc32 - Caculate multicast hash value by CRC32
 *      ETHbIsBufferCrc32Ok - Check CRC value of the buffer if Ok or not
 *
 * Revision History:
 *
 */

#include "device.h"
#include "tmacro.h"
#include "tcrc.h"
#include "tether.h"

/*                                                                   */

/*                                                                   */

/*                                                                   */

/*                                                                   */

/*                                                                   */



/*
                                                      
  
              
       
                                       
        
            
  
                           
  
 */
BYTE ETHbyGetHashIndexByCrc32(PBYTE pbyMultiAddr)
{
	int     ii;
	BYTE    byTmpHash;
	BYTE    byHash = 0;

	/*                                         */
	byTmpHash = (BYTE)(CRCdwCrc32(pbyMultiAddr, ETH_ALEN,
			0xFFFFFFFFL) & 0x3F);
	/*                               */
	for (ii = 0; ii < (sizeof(byTmpHash) * 8); ii++) {
		byHash <<= 1;
		if (byTmpHash & 0x01)
			byHash |= 1;
		byTmpHash >>= 1;
	}

	/*                                 */
	return byHash >> 2;
}


/*
                                                          
  
              
       
                                                             
                                                           
        
            
  
                                            
  
 */
BOOL ETHbIsBufferCrc32Ok(PBYTE pbyBuffer, unsigned int cbFrameLength)
{
	DWORD dwCRC;

	dwCRC = CRCdwGetCrc32(pbyBuffer, cbFrameLength - 4);
	if (cpu_to_le32(*((PDWORD)(pbyBuffer + cbFrameLength - 4))) != dwCRC)
		return FALSE;
	return TRUE;
}

