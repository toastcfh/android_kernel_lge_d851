/*
                                                                    
 */
#ifndef _ASM_POWERPC_REG_8xx_H
#define _ASM_POWERPC_REG_8xx_H

/*                                                                
                             
 */
#define SPRN_IC_CST	560	/*                                  */
#define SPRN_IC_ADR	561	/*                                  */
#define SPRN_IC_DAT	562	/*                         */
#define SPRN_DC_CST	568	/*                           */
#define SPRN_DC_ADR	569	/*                                  */
#define SPRN_DC_DAT	570	/*                         */

/*                                                                      
*/
#define	IDC_ENABLE	0x02000000	/*              */
#define IDC_DISABLE	0x04000000	/*               */
#define IDC_LDLCK	0x06000000	/*               */
#define IDC_UNLINE	0x08000000	/*             */
#define IDC_UNALL	0x0a000000	/*            */
#define IDC_INVALL	0x0c000000	/*                */

#define DC_FLINE	0x0e000000	/*                       */
#define DC_SFWT		0x01000000	/*                              */
#define DC_CFWT		0x03000000	/*                                */
#define DC_SLES		0x05000000	/*                             */
#define DC_CLES		0x07000000	/*                               */

/*        
*/
#define IDC_ENABLED	0x80000000	/*                  */
#define IDC_CERR1	0x00200000	/*               */
#define IDC_CERR2	0x00100000	/*               */
#define IDC_CERR3	0x00080000	/*               */

#define DC_DFWT		0x40000000	/*                                    */
#define DC_LES		0x20000000	/*                               */

#endif /*                        */
