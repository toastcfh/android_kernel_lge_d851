#ifndef __marb_foo_defs_h
#define __marb_foo_defs_h

/*
                                  
                               
   
                                                                           
                                 
  
                              
 */
/*                    */
#ifndef REG_RD
#define REG_RD( scope, inst, reg ) \
  REG_READ( reg_##scope##_##reg, \
            (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_WR
#define REG_WR( scope, inst, reg, val ) \
  REG_WRITE( reg_##scope##_##reg, \
             (inst) + REG_WR_ADDR_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_VECT
#define REG_RD_VECT( scope, inst, reg, index ) \
  REG_READ( reg_##scope##_##reg, \
            (inst) + REG_RD_ADDR_##scope##_##reg + \
	    (index) * STRIDE_##scope##_##reg )
#endif

#ifndef REG_WR_VECT
#define REG_WR_VECT( scope, inst, reg, index, val ) \
  REG_WRITE( reg_##scope##_##reg, \
             (inst) + REG_WR_ADDR_##scope##_##reg + \
	     (index) * STRIDE_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_INT
#define REG_RD_INT( scope, inst, reg ) \
  REG_READ( int, (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_WR_INT
#define REG_WR_INT( scope, inst, reg, val ) \
  REG_WRITE( int, (inst) + REG_WR_ADDR_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_INT_VECT
#define REG_RD_INT_VECT( scope, inst, reg, index ) \
  REG_READ( int, (inst) + REG_RD_ADDR_##scope##_##reg + \
	    (index) * STRIDE_##scope##_##reg )
#endif

#ifndef REG_WR_INT_VECT
#define REG_WR_INT_VECT( scope, inst, reg, index, val ) \
  REG_WRITE( int, (inst) + REG_WR_ADDR_##scope##_##reg + \
	     (index) * STRIDE_##scope##_##reg, (val) )
#endif

#ifndef REG_TYPE_CONV
#define REG_TYPE_CONV( type, orgtype, val ) \
  ( { union { orgtype o; type n; } r; r.o = val; r.n; } )
#endif

#ifndef reg_page_size
#define reg_page_size 8192
#endif

#ifndef REG_ADDR
#define REG_ADDR( scope, inst, reg ) \
  ( (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_ADDR_VECT
#define REG_ADDR_VECT( scope, inst, reg, index ) \
  ( (inst) + REG_RD_ADDR_##scope##_##reg + \
    (index) * STRIDE_##scope##_##reg )
#endif

/*                                    */

#define STRIDE_marb_foo_rw_intm_slots 4
/*                                                 */
typedef struct {
  unsigned int owner : 4;
  unsigned int dummy1 : 28;
} reg_marb_foo_rw_intm_slots;
#define REG_RD_ADDR_marb_foo_rw_intm_slots 0
#define REG_WR_ADDR_marb_foo_rw_intm_slots 0

#define STRIDE_marb_foo_rw_l2_slots 4
/*                                               */
typedef struct {
  unsigned int owner : 4;
  unsigned int dummy1 : 28;
} reg_marb_foo_rw_l2_slots;
#define REG_RD_ADDR_marb_foo_rw_l2_slots 256
#define REG_WR_ADDR_marb_foo_rw_l2_slots 256

#define STRIDE_marb_foo_rw_regs_slots 4
/*                                                 */
typedef struct {
  unsigned int owner : 4;
  unsigned int dummy1 : 28;
} reg_marb_foo_rw_regs_slots;
#define REG_RD_ADDR_marb_foo_rw_regs_slots 512
#define REG_WR_ADDR_marb_foo_rw_regs_slots 512

/*                                                 */
typedef struct {
  unsigned int intm_bsize : 2;
  unsigned int l2_bsize   : 2;
  unsigned int dummy1     : 28;
} reg_marb_foo_rw_sclr_burst;
#define REG_RD_ADDR_marb_foo_rw_sclr_burst 528
#define REG_WR_ADDR_marb_foo_rw_sclr_burst 528

/*                                                 */
typedef struct {
  unsigned int intm_bsize : 2;
  unsigned int l2_bsize   : 2;
  unsigned int dummy1     : 28;
} reg_marb_foo_rw_dma0_burst;
#define REG_RD_ADDR_marb_foo_rw_dma0_burst 532
#define REG_WR_ADDR_marb_foo_rw_dma0_burst 532

/*                                                 */
typedef struct {
  unsigned int intm_bsize : 2;
  unsigned int l2_bsize   : 2;
  unsigned int dummy1     : 28;
} reg_marb_foo_rw_dma1_burst;
#define REG_RD_ADDR_marb_foo_rw_dma1_burst 536
#define REG_WR_ADDR_marb_foo_rw_dma1_burst 536

/*                                                 */
typedef struct {
  unsigned int intm_bsize : 2;
  unsigned int l2_bsize   : 2;
  unsigned int dummy1     : 28;
} reg_marb_foo_rw_dma2_burst;
#define REG_RD_ADDR_marb_foo_rw_dma2_burst 540
#define REG_WR_ADDR_marb_foo_rw_dma2_burst 540

/*                                                 */
typedef struct {
  unsigned int intm_bsize : 2;
  unsigned int l2_bsize   : 2;
  unsigned int dummy1     : 28;
} reg_marb_foo_rw_dma3_burst;
#define REG_RD_ADDR_marb_foo_rw_dma3_burst 544
#define REG_WR_ADDR_marb_foo_rw_dma3_burst 544

/*                                                 */
typedef struct {
  unsigned int intm_bsize : 2;
  unsigned int l2_bsize   : 2;
  unsigned int dummy1     : 28;
} reg_marb_foo_rw_dma4_burst;
#define REG_RD_ADDR_marb_foo_rw_dma4_burst 548
#define REG_WR_ADDR_marb_foo_rw_dma4_burst 548

/*                                                 */
typedef struct {
  unsigned int intm_bsize : 2;
  unsigned int l2_bsize   : 2;
  unsigned int dummy1     : 28;
} reg_marb_foo_rw_dma5_burst;
#define REG_RD_ADDR_marb_foo_rw_dma5_burst 552
#define REG_WR_ADDR_marb_foo_rw_dma5_burst 552

/*                                                 */
typedef struct {
  unsigned int intm_bsize : 2;
  unsigned int l2_bsize   : 2;
  unsigned int dummy1     : 28;
} reg_marb_foo_rw_dma6_burst;
#define REG_RD_ADDR_marb_foo_rw_dma6_burst 556
#define REG_WR_ADDR_marb_foo_rw_dma6_burst 556

/*                                                 */
typedef struct {
  unsigned int intm_bsize : 2;
  unsigned int l2_bsize   : 2;
  unsigned int dummy1     : 28;
} reg_marb_foo_rw_dma7_burst;
#define REG_RD_ADDR_marb_foo_rw_dma7_burst 560
#define REG_WR_ADDR_marb_foo_rw_dma7_burst 560

/*                                                 */
typedef struct {
  unsigned int intm_bsize : 2;
  unsigned int l2_bsize   : 2;
  unsigned int dummy1     : 28;
} reg_marb_foo_rw_dma9_burst;
#define REG_RD_ADDR_marb_foo_rw_dma9_burst 564
#define REG_WR_ADDR_marb_foo_rw_dma9_burst 564

/*                                                  */
typedef struct {
  unsigned int intm_bsize : 2;
  unsigned int l2_bsize   : 2;
  unsigned int dummy1     : 28;
} reg_marb_foo_rw_dma11_burst;
#define REG_RD_ADDR_marb_foo_rw_dma11_burst 568
#define REG_WR_ADDR_marb_foo_rw_dma11_burst 568

/*                                                 */
typedef struct {
  unsigned int intm_bsize : 2;
  unsigned int l2_bsize   : 2;
  unsigned int dummy1     : 28;
} reg_marb_foo_rw_cpui_burst;
#define REG_RD_ADDR_marb_foo_rw_cpui_burst 572
#define REG_WR_ADDR_marb_foo_rw_cpui_burst 572

/*                                                 */
typedef struct {
  unsigned int intm_bsize : 2;
  unsigned int l2_bsize   : 2;
  unsigned int dummy1     : 28;
} reg_marb_foo_rw_cpud_burst;
#define REG_RD_ADDR_marb_foo_rw_cpud_burst 576
#define REG_WR_ADDR_marb_foo_rw_cpud_burst 576

/*                                                */
typedef struct {
  unsigned int intm_bsize : 2;
  unsigned int l2_bsize   : 2;
  unsigned int dummy1     : 28;
} reg_marb_foo_rw_iop_burst;
#define REG_RD_ADDR_marb_foo_rw_iop_burst 580
#define REG_WR_ADDR_marb_foo_rw_iop_burst 580

/*                                                    */
typedef struct {
  unsigned int intm_bsize : 2;
  unsigned int l2_bsize   : 2;
  unsigned int dummy1     : 28;
} reg_marb_foo_rw_ccdstat_burst;
#define REG_RD_ADDR_marb_foo_rw_ccdstat_burst 584
#define REG_WR_ADDR_marb_foo_rw_ccdstat_burst 584

/*                                                */
typedef struct {
  unsigned int bp0 : 1;
  unsigned int bp1 : 1;
  unsigned int bp2 : 1;
  unsigned int bp3 : 1;
  unsigned int dummy1 : 28;
} reg_marb_foo_rw_intr_mask;
#define REG_RD_ADDR_marb_foo_rw_intr_mask 588
#define REG_WR_ADDR_marb_foo_rw_intr_mask 588

/*                                               */
typedef struct {
  unsigned int bp0 : 1;
  unsigned int bp1 : 1;
  unsigned int bp2 : 1;
  unsigned int bp3 : 1;
  unsigned int dummy1 : 28;
} reg_marb_foo_rw_ack_intr;
#define REG_RD_ADDR_marb_foo_rw_ack_intr 592
#define REG_WR_ADDR_marb_foo_rw_ack_intr 592

/*                                         */
typedef struct {
  unsigned int bp0 : 1;
  unsigned int bp1 : 1;
  unsigned int bp2 : 1;
  unsigned int bp3 : 1;
  unsigned int dummy1 : 28;
} reg_marb_foo_r_intr;
#define REG_RD_ADDR_marb_foo_r_intr 596

/*                                                */
typedef struct {
  unsigned int bp0 : 1;
  unsigned int bp1 : 1;
  unsigned int bp2 : 1;
  unsigned int bp3 : 1;
  unsigned int dummy1 : 28;
} reg_marb_foo_r_masked_intr;
#define REG_RD_ADDR_marb_foo_r_masked_intr 600

/*                                                */
typedef struct {
  unsigned int sclr    : 1;
  unsigned int dma0    : 1;
  unsigned int dma1    : 1;
  unsigned int dma2    : 1;
  unsigned int dma3    : 1;
  unsigned int dma4    : 1;
  unsigned int dma5    : 1;
  unsigned int dma6    : 1;
  unsigned int dma7    : 1;
  unsigned int dma9    : 1;
  unsigned int dma11   : 1;
  unsigned int cpui    : 1;
  unsigned int cpud    : 1;
  unsigned int iop     : 1;
  unsigned int ccdstat : 1;
  unsigned int dummy1  : 17;
} reg_marb_foo_rw_stop_mask;
#define REG_RD_ADDR_marb_foo_rw_stop_mask 604
#define REG_WR_ADDR_marb_foo_rw_stop_mask 604

/*                                            */
typedef struct {
  unsigned int sclr    : 1;
  unsigned int dma0    : 1;
  unsigned int dma1    : 1;
  unsigned int dma2    : 1;
  unsigned int dma3    : 1;
  unsigned int dma4    : 1;
  unsigned int dma5    : 1;
  unsigned int dma6    : 1;
  unsigned int dma7    : 1;
  unsigned int dma9    : 1;
  unsigned int dma11   : 1;
  unsigned int cpui    : 1;
  unsigned int cpud    : 1;
  unsigned int iop     : 1;
  unsigned int ccdstat : 1;
  unsigned int dummy1  : 17;
} reg_marb_foo_r_stopped;
#define REG_RD_ADDR_marb_foo_r_stopped 608

/*                                               */
typedef struct {
  unsigned int sclr    : 1;
  unsigned int dma0    : 1;
  unsigned int dma1    : 1;
  unsigned int dma2    : 1;
  unsigned int dma3    : 1;
  unsigned int dma4    : 1;
  unsigned int dma5    : 1;
  unsigned int dma6    : 1;
  unsigned int dma7    : 1;
  unsigned int dma9    : 1;
  unsigned int dma11   : 1;
  unsigned int cpui    : 1;
  unsigned int cpud    : 1;
  unsigned int iop     : 1;
  unsigned int ccdstat : 1;
  unsigned int dummy1  : 17;
} reg_marb_foo_rw_no_snoop;
#define REG_RD_ADDR_marb_foo_rw_no_snoop 896
#define REG_WR_ADDR_marb_foo_rw_no_snoop 896

/*                                                  */
typedef struct {
  unsigned int dummy1 : 11;
  unsigned int cpui : 1;
  unsigned int cpud : 1;
  unsigned int dummy2 : 19;
} reg_marb_foo_rw_no_snoop_rq;
#define REG_RD_ADDR_marb_foo_rw_no_snoop_rq 900
#define REG_WR_ADDR_marb_foo_rw_no_snoop_rq 900


/*           */
enum {
  regk_marb_foo_ccdstat                    = 0x0000000e,
  regk_marb_foo_cpud                       = 0x0000000c,
  regk_marb_foo_cpui                       = 0x0000000b,
  regk_marb_foo_dma0                       = 0x00000001,
  regk_marb_foo_dma1                       = 0x00000002,
  regk_marb_foo_dma11                      = 0x0000000a,
  regk_marb_foo_dma2                       = 0x00000003,
  regk_marb_foo_dma3                       = 0x00000004,
  regk_marb_foo_dma4                       = 0x00000005,
  regk_marb_foo_dma5                       = 0x00000006,
  regk_marb_foo_dma6                       = 0x00000007,
  regk_marb_foo_dma7                       = 0x00000008,
  regk_marb_foo_dma9                       = 0x00000009,
  regk_marb_foo_iop                        = 0x0000000d,
  regk_marb_foo_no                         = 0x00000000,
  regk_marb_foo_r_stopped_default          = 0x00000000,
  regk_marb_foo_rw_ccdstat_burst_default   = 0x00000000,
  regk_marb_foo_rw_cpud_burst_default      = 0x00000000,
  regk_marb_foo_rw_cpui_burst_default      = 0x00000000,
  regk_marb_foo_rw_dma0_burst_default      = 0x00000000,
  regk_marb_foo_rw_dma11_burst_default     = 0x00000000,
  regk_marb_foo_rw_dma1_burst_default      = 0x00000000,
  regk_marb_foo_rw_dma2_burst_default      = 0x00000000,
  regk_marb_foo_rw_dma3_burst_default      = 0x00000000,
  regk_marb_foo_rw_dma4_burst_default      = 0x00000000,
  regk_marb_foo_rw_dma5_burst_default      = 0x00000000,
  regk_marb_foo_rw_dma6_burst_default      = 0x00000000,
  regk_marb_foo_rw_dma7_burst_default      = 0x00000000,
  regk_marb_foo_rw_dma9_burst_default      = 0x00000000,
  regk_marb_foo_rw_intm_slots_default      = 0x00000000,
  regk_marb_foo_rw_intm_slots_size         = 0x00000040,
  regk_marb_foo_rw_intr_mask_default       = 0x00000000,
  regk_marb_foo_rw_iop_burst_default       = 0x00000000,
  regk_marb_foo_rw_l2_slots_default        = 0x00000000,
  regk_marb_foo_rw_l2_slots_size           = 0x00000040,
  regk_marb_foo_rw_no_snoop_default        = 0x00000000,
  regk_marb_foo_rw_no_snoop_rq_default     = 0x00000000,
  regk_marb_foo_rw_regs_slots_default      = 0x00000000,
  regk_marb_foo_rw_regs_slots_size         = 0x00000004,
  regk_marb_foo_rw_sclr_burst_default      = 0x00000000,
  regk_marb_foo_rw_stop_mask_default       = 0x00000000,
  regk_marb_foo_sclr                       = 0x00000000,
  regk_marb_foo_yes                        = 0x00000001
};
#endif /*                   */
#ifndef __marb_foo_bp_defs_h
#define __marb_foo_bp_defs_h

/*
                                  
                               
   
                                                                           
                                 
  
                              
 */
/*                    */
#ifndef REG_RD
#define REG_RD( scope, inst, reg ) \
  REG_READ( reg_##scope##_##reg, \
            (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_WR
#define REG_WR( scope, inst, reg, val ) \
  REG_WRITE( reg_##scope##_##reg, \
             (inst) + REG_WR_ADDR_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_VECT
#define REG_RD_VECT( scope, inst, reg, index ) \
  REG_READ( reg_##scope##_##reg, \
            (inst) + REG_RD_ADDR_##scope##_##reg + \
	    (index) * STRIDE_##scope##_##reg )
#endif

#ifndef REG_WR_VECT
#define REG_WR_VECT( scope, inst, reg, index, val ) \
  REG_WRITE( reg_##scope##_##reg, \
             (inst) + REG_WR_ADDR_##scope##_##reg + \
	     (index) * STRIDE_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_INT
#define REG_RD_INT( scope, inst, reg ) \
  REG_READ( int, (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_WR_INT
#define REG_WR_INT( scope, inst, reg, val ) \
  REG_WRITE( int, (inst) + REG_WR_ADDR_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_INT_VECT
#define REG_RD_INT_VECT( scope, inst, reg, index ) \
  REG_READ( int, (inst) + REG_RD_ADDR_##scope##_##reg + \
	    (index) * STRIDE_##scope##_##reg )
#endif

#ifndef REG_WR_INT_VECT
#define REG_WR_INT_VECT( scope, inst, reg, index, val ) \
  REG_WRITE( int, (inst) + REG_WR_ADDR_##scope##_##reg + \
	     (index) * STRIDE_##scope##_##reg, (val) )
#endif

#ifndef REG_TYPE_CONV
#define REG_TYPE_CONV( type, orgtype, val ) \
  ( { union { orgtype o; type n; } r; r.o = val; r.n; } )
#endif

#ifndef reg_page_size
#define reg_page_size 8192
#endif

#ifndef REG_ADDR
#define REG_ADDR( scope, inst, reg ) \
  ( (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_ADDR_VECT
#define REG_ADDR_VECT( scope, inst, reg, index ) \
  ( (inst) + REG_RD_ADDR_##scope##_##reg + \
    (index) * STRIDE_##scope##_##reg )
#endif

/*                                       */

/*                                                    */
typedef unsigned int reg_marb_foo_bp_rw_first_addr;
#define REG_RD_ADDR_marb_foo_bp_rw_first_addr 0
#define REG_WR_ADDR_marb_foo_bp_rw_first_addr 0

/*                                                   */
typedef unsigned int reg_marb_foo_bp_rw_last_addr;
#define REG_RD_ADDR_marb_foo_bp_rw_last_addr 4
#define REG_WR_ADDR_marb_foo_bp_rw_last_addr 4

/*                                            */
typedef struct {
  unsigned int rd         : 1;
  unsigned int wr         : 1;
  unsigned int rd_excl    : 1;
  unsigned int pri_wr     : 1;
  unsigned int us_rd      : 1;
  unsigned int us_wr      : 1;
  unsigned int us_rd_excl : 1;
  unsigned int us_pri_wr  : 1;
  unsigned int dummy1     : 24;
} reg_marb_foo_bp_rw_op;
#define REG_RD_ADDR_marb_foo_bp_rw_op 8
#define REG_WR_ADDR_marb_foo_bp_rw_op 8

/*                                                 */
typedef struct {
  unsigned int sclr    : 1;
  unsigned int dma0    : 1;
  unsigned int dma1    : 1;
  unsigned int dma2    : 1;
  unsigned int dma3    : 1;
  unsigned int dma4    : 1;
  unsigned int dma5    : 1;
  unsigned int dma6    : 1;
  unsigned int dma7    : 1;
  unsigned int dma9    : 1;
  unsigned int dma11   : 1;
  unsigned int cpui    : 1;
  unsigned int cpud    : 1;
  unsigned int iop     : 1;
  unsigned int ccdstat : 1;
  unsigned int dummy1  : 17;
} reg_marb_foo_bp_rw_clients;
#define REG_RD_ADDR_marb_foo_bp_rw_clients 12
#define REG_WR_ADDR_marb_foo_bp_rw_clients 12

/*                                                 */
typedef struct {
  unsigned int wrap : 1;
  unsigned int dummy1 : 31;
} reg_marb_foo_bp_rw_options;
#define REG_RD_ADDR_marb_foo_bp_rw_options 16
#define REG_WR_ADDR_marb_foo_bp_rw_options 16

/*                                                */
typedef unsigned int reg_marb_foo_bp_r_brk_addr;
#define REG_RD_ADDR_marb_foo_bp_r_brk_addr 20

/*                                              */
typedef struct {
  unsigned int rd         : 1;
  unsigned int wr         : 1;
  unsigned int rd_excl    : 1;
  unsigned int pri_wr     : 1;
  unsigned int us_rd      : 1;
  unsigned int us_wr      : 1;
  unsigned int us_rd_excl : 1;
  unsigned int us_pri_wr  : 1;
  unsigned int dummy1     : 24;
} reg_marb_foo_bp_r_brk_op;
#define REG_RD_ADDR_marb_foo_bp_r_brk_op 24

/*                                                   */
typedef struct {
  unsigned int sclr    : 1;
  unsigned int dma0    : 1;
  unsigned int dma1    : 1;
  unsigned int dma2    : 1;
  unsigned int dma3    : 1;
  unsigned int dma4    : 1;
  unsigned int dma5    : 1;
  unsigned int dma6    : 1;
  unsigned int dma7    : 1;
  unsigned int dma9    : 1;
  unsigned int dma11   : 1;
  unsigned int cpui    : 1;
  unsigned int cpud    : 1;
  unsigned int iop     : 1;
  unsigned int ccdstat : 1;
  unsigned int dummy1  : 17;
} reg_marb_foo_bp_r_brk_clients;
#define REG_RD_ADDR_marb_foo_bp_r_brk_clients 28

/*                                                        */
typedef struct {
  unsigned int sclr    : 1;
  unsigned int dma0    : 1;
  unsigned int dma1    : 1;
  unsigned int dma2    : 1;
  unsigned int dma3    : 1;
  unsigned int dma4    : 1;
  unsigned int dma5    : 1;
  unsigned int dma6    : 1;
  unsigned int dma7    : 1;
  unsigned int dma9    : 1;
  unsigned int dma11   : 1;
  unsigned int cpui    : 1;
  unsigned int cpud    : 1;
  unsigned int iop     : 1;
  unsigned int ccdstat : 1;
  unsigned int dummy1  : 17;
} reg_marb_foo_bp_r_brk_first_client;
#define REG_RD_ADDR_marb_foo_bp_r_brk_first_client 32

/*                                                */
typedef unsigned int reg_marb_foo_bp_r_brk_size;
#define REG_RD_ADDR_marb_foo_bp_r_brk_size 36

/*                                             */
typedef unsigned int reg_marb_foo_bp_rw_ack;
#define REG_RD_ADDR_marb_foo_bp_rw_ack 40
#define REG_WR_ADDR_marb_foo_bp_rw_ack 40


/*           */
enum {
  regk_marb_foo_bp_no                      = 0x00000000,
  regk_marb_foo_bp_rw_op_default           = 0x00000000,
  regk_marb_foo_bp_rw_options_default      = 0x00000000,
  regk_marb_foo_bp_yes                     = 0x00000001
};
#endif /*                      */
