#ifndef _SPARC64_INTR_QUEUE_H
#define _SPARC64_INTR_QUEUE_H

/*                                                           */

#define INTRQ_CPU_MONDO_HEAD	  0x3c0 /*                          */
#define INTRQ_CPU_MONDO_TAIL	  0x3c8 /*                          */
#define INTRQ_DEVICE_MONDO_HEAD	  0x3d0 /*                             */
#define INTRQ_DEVICE_MONDO_TAIL	  0x3d8 /*                             */
#define INTRQ_RESUM_MONDO_HEAD	  0x3e0 /*                                */
#define INTRQ_RESUM_MONDO_TAIL	  0x3e8 /*                                */
#define INTRQ_NONRESUM_MONDO_HEAD 0x3f0 /*                                */
#define INTRQ_NONRESUM_MONDO_TAIL 0x3f8 /*                                */

#endif /*                          */
