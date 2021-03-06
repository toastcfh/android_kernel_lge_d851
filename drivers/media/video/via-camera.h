/*
                                   
 */
#define VCR_INTCTRL	0x300	/*                           */
#define   VCR_IC_EAV	  0x0001   /*                            */
#define	  VCR_IC_EVBI	  0x0002   /*                   */
#define   VCR_IC_FBOTFLD  0x0004   /*                                   */
#define   VCR_IC_ACTBUF	  0x0018   /*                      */
#define   VCR_IC_VSYNC	  0x0020   /*                          */
#define   VCR_IC_BOTFLD	  0x0040   /*                        */
#define   VCR_IC_FFULL	  0x0080   /*           */
#define   VCR_IC_INTEN	  0x0100   /*                                 */
#define   VCR_IC_VBIINT	  0x0200   /*                       */
#define   VCR_IC_VBIBUF	  0x0400   /*                    */

#define VCR_TSC		0x308	/*                          */
#define VCR_TSC_ENABLE    0x000001   /*                               */
#define VCR_TSC_DROPERR   0x000002   /*                    */
#define VCR_TSC_METHOD    0x00000c   /*                             */
#define VCR_TSC_COUNT     0x07fff0   /*                       */
#define VCR_TSC_CBMODE	  0x080000   /*                             */
#define VCR_TSC_PSSIG	  0x100000   /*                                */
#define VCR_TSC_BE	  0x200000   /*                         */
#define VCR_TSC_SERIAL	  0x400000   /*                             */

#define VCR_CAPINTC	0x310	/*                           */
#define   VCR_CI_ENABLE   0x00000001  /*                */
#define   VCR_CI_BSS	  0x00000002  /*                            */
#define   VCR_CI_3BUFS	  0x00000004  /*                              */
#define   VCR_CI_VIPEN	  0x00000008  /*            */
#define   VCR_CI_CCIR601_8  0	        /*                             */
#define   VCR_CI_CCIR656_8  0x00000010  /*                    */
#define   VCR_CI_CCIR601_16 0x00000020  /*                     */
#define   VCR_CI_CCIR656_16 0x00000030  /*                     */
#define   VCR_CI_HDMODE   0x00000040  /*                                   */
#define   VCR_CI_BSWAP    0x00000080  /*                     */
#define   VCR_CI_YUYV	  0	      /*                 */
#define   VCR_CI_UYVY	  0x00000100  /*                 */
#define   VCR_CI_YVYU	  0x00000200  /*                 */
#define   VCR_CI_VYUY	  0x00000300  /*                 */
#define   VCR_CI_VIPTYPE  0x00000400  /*          */
#define   VCR_CI_IFSEN    0x00000800  /*                           */
#define   VCR_CI_DIODD	  0	      /*                         */
#define   VCR_CI_DIEVEN   0x00001000  /*                         */
#define   VCR_CI_DIBOTH   0x00002000  /*                          */
#define   VCR_CI_DIBOTH30 0x00003000  /*                                    */
#define   VCR_CI_CONVTYPE 0x00004000  /*                                 */
#define   VCR_CI_CFC	  0x00008000  /*                          */
#define   VCR_CI_FILTER   0x00070000  /*                         
                
                 
                 
                 
                  */
#define   VCR_CI_CLKINV   0x00080000  /*                    */
#define   VCR_CI_VREFINV  0x00100000  /*               */
#define   VCR_CI_HREFINV  0x00200000  /*               */
#define   VCR_CI_FLDINV   0x00400000  /*                */
#define   VCR_CI_CLKPIN	  0x00800000  /*                   */
#define   VCR_CI_THRESH   0x0f000000  /*                        */
#define   VCR_CI_HRLE     0x10000000  /*                       */
#define   VCR_CI_VRLE     0x20000000  /*                       */
#define   VCR_CI_OFLDINV  0x40000000  /*                       */
#define   VCR_CI_CLKEN    0x80000000  /*                      */

#define VCR_HORRANGE	0x314	/*                               */
#define VCR_VERTRANGE	0x318	/*                             */
#define VCR_AVSCALE	0x31c	/*                              */
#define   VCR_AVS_HEN	  0x00000800   /*                         */
#define   VCR_AVS_VEN	  0x04000000   /*                 */
#define VCR_VBIHOR	0x320	/*                           */
#define VCR_VBIVERT	0x324	/*                         */
#define VCR_VBIBUF1	0x328	/*                  */
#define VCR_VBISTRIDE	0x32c	/*            */
#define VCR_ANCDATACNT	0x330	/*                              */
#define VCR_MAXDATA	0x334	/*                                   */
#define VCR_MAXVBI	0x338	/*                           */
#define VCR_CAPDATA	0x33c	/*                    */
#define VCR_VBUF1	0x340	/*                    */
#define VCR_VBUF2	0x344	/*                     */
#define VCR_VBUF3	0x348	/*                    */
#define VCR_VBUF_MASK	0x1ffffff0	/*           */
#define VCR_VBIBUF2	0x34c	/*                   */
#define VCR_VSTRIDE	0x350	/*                                  */
#define   VCR_VS_STRIDE_SHIFT 4
#define   VCR_VS_STRIDE   0x00001ff0  /*                       */
#define   VCR_VS_CCD	  0x007f0000  /*                     */
#define   VCR_VS_COREEN   0x00800000  /*               */
#define VCR_TS0ERR	0x354	/*                             */
#define VCR_TS1ERR	0x358	/*                             */
#define VCR_TS2ERR	0x35c	/*                             */

/*                                                    */
