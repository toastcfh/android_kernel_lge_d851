/******************************************************************************
 *
 *	(C)Copyright 1998,1999 SysKonnect,
 *	a business unit of Schneider & Koch & Co. Datensysteme GmbH.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	The information in this file is provided "AS IS" without warranty.
 *
 ******************************************************************************/

#ifndef _SKFP_H_SMTSTATE_H_
#define _SKFP_H_SMTSTATE_H_

/*
                        
 */

#ifndef	KERNEL
/*
             
 */
#define PC0_OFF			0
#define PC1_BREAK		1
#define PC2_TRACE		2
#define PC3_CONNECT		3
#define PC4_NEXT		4
#define PC5_SIGNAL		5
#define PC6_JOIN		6
#define PC7_VERIFY		7
#define PC8_ACTIVE		8
#define PC9_MAINT		9

/*
            
 */
#define PM_NONE			0
#define PM_PEER			1
#define PM_TREE			2

/*
           
 */
#define TA			0
#define TB			1
#define TS			2
#define TM			3
#define TNONE			4

/*
             
 */
#define SC0_ISOLATED	0		/*          */
#define SC1_WRAP_A	5		/*        */
#define SC2_WRAP_B	6		/*        */
#define SC4_THRU_A	12		/*           */
#define SC5_THRU_B	7		/*                     */
#define SC7_WRAP_S	8		/*     */

/*
             
 */
#define EC0_OUT		0
#define EC1_IN		1
#define EC2_TRACE	2
#define EC3_LEAVE	3
#define EC4_PATH_TEST	4
#define EC5_INSERT	5
#define EC6_CHECK	6
#define EC7_DEINSERT	7

/*
             
 */
#define RM0_ISOLATED	0
#define RM1_NON_OP	1		/*                 */
#define RM2_RING_OP	2		/*                  */
#define RM3_DETECT	3		/*                       */
#define RM4_NON_OP_DUP	4		/*                     */
#define RM5_RING_OP_DUP	5		/*                            */
#define RM6_DIRECTED	6		/*                          */
#define RM7_TRACE	7		/*                 */
#endif

struct pcm_state {
	unsigned char	pcm_type ;		/*             */
	unsigned char	pcm_state ;		/*                 */
	unsigned char	pcm_mode ;		/*                     */
	unsigned char	pcm_neighbor ;		/*             */
	unsigned char	pcm_bsf ;		/*                      */
	unsigned char	pcm_lsf ;		/*                      */
	unsigned char	pcm_lct_fail ;		/*                  */
	unsigned char	pcm_ls_rx ;		/*               */
	short		pcm_r_val ;		/*                */
	short		pcm_t_val ;		/*                */
} ;

struct smt_state {
	struct pcm_state pcm_state[NUMPHYS] ;	/*                 */
} ;

#endif

