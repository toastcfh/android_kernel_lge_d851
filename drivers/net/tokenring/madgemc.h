/* 
                                                    
  
           
                  
 */

#ifndef __LINUX_MADGEMC_H
#define __LINUX_MADGEMC_H

#ifdef __KERNEL__

#define MADGEMC16_CARDNAME "Madge Smart 16/4 MC16 Ringnode"
#define MADGEMC32_CARDNAME "Madge Smart 16/4 MC32 Ringnode"

/* 
                                               
 */
#define MC16_POS0_ADDR1 0x20
#define MC16_POS2_ADDR2 0x04
#define MC16_POS3_ADDR3 0x20

#define MC_CONTROL_REG0		((long)-8) /*      */
#define MC_CONTROL_REG1		((long)-7) /*      */
#define MC_ADAPTER_POS_REG0	((long)-6) /*      */
#define MC_ADAPTER_POS_REG1	((long)-5) /*      */
#define MC_ADAPTER_POS_REG2	((long)-4) /*      */
#define MC_ADAPTER_REG5_UNUSED	((long)-3) /*      */
#define MC_ADAPTER_REG6_UNUSED	((long)-2) /*      */
#define MC_CONTROL_REG7		((long)-1) /*      */

#define MC_CONTROL_REG0_UNKNOWN1	0x01
#define MC_CONTROL_REG0_UNKNOWN2	0x02
#define MC_CONTROL_REG0_SIFSEL		0x04
#define MC_CONTROL_REG0_PAGE		0x08
#define MC_CONTROL_REG0_TESTINTERRUPT	0x10
#define MC_CONTROL_REG0_UNKNOWN20	0x20
#define MC_CONTROL_REG0_SINTR		0x40
#define MC_CONTROL_REG0_UNKNOWN80	0x80

#define MC_CONTROL_REG1_SINTEN		0x01
#define MC_CONTROL_REG1_BITOFDEATH	0x02
#define MC_CONTROL_REG1_NSRESET		0x04
#define MC_CONTROL_REG1_UNKNOWN8	0x08
#define MC_CONTROL_REG1_UNKNOWN10	0x10
#define MC_CONTROL_REG1_UNKNOWN20	0x20
#define MC_CONTROL_REG1_SRSX		0x40
#define MC_CONTROL_REG1_SPEED_SEL	0x80

#define MC_CONTROL_REG7_CABLESTP	0x00
#define MC_CONTROL_REG7_CABLEUTP	0x01

/*
                
 */
#define MC_ROM_MANUFACTURERID		0x00
#define MC_ROM_ADAPTERID		0x01
#define MC_ROM_REVISION			0x02
#define MC_ROM_CONFIG0			0x03
#define MC_ROM_CONFIG1			0x04
#define MC_ROM_CONFIG2			0x05

/*
               
 */
#define MC_ROM_UNUSED_BYTE		0x00
#define MC_ROM_BIA_START		0x01

#endif /*            */
#endif /*                   */
