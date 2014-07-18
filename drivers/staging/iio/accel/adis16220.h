#ifndef SPI_ADIS16220_H_
#define SPI_ADIS16220_H_

#define ADIS16220_STARTUP_DELAY	220 /*    */

#define ADIS16220_READ_REG(a)    a
#define ADIS16220_WRITE_REG(a) ((a) | 0x80)

/*                          */
#define ADIS16220_FLASH_CNT     0x00
/*                                                 */
#define ADIS16220_ACCL_NULL     0x02
/*                                         */
#define ADIS16220_AIN1_NULL     0x04
/*                                         */
#define ADIS16220_AIN2_NULL     0x06
/*                                     */
#define ADIS16220_CAPT_SUPPLY   0x0A
/*                                    */
#define ADIS16220_CAPT_TEMP     0x0C
/*                                          */
#define ADIS16220_CAPT_PEAKA    0x0E
/*                                        */
#define ADIS16220_CAPT_PEAK1    0x10
/*                                        */
#define ADIS16220_CAPT_PEAK2    0x12
/*                                         */
#define ADIS16220_CAPT_BUFA     0x14
/*                                 */
#define ADIS16220_CAPT_BUF1     0x16
/*                                 */
#define ADIS16220_CAPT_BUF2     0x18
/*                                         */
#define ADIS16220_CAPT_PNTR     0x1A
/*                                   */
#define ADIS16220_CAPT_CTRL     0x1C
/*                                          */
#define ADIS16220_CAPT_PRD      0x1E
/*                                               */
#define ADIS16220_ALM_MAGA      0x20
/*                                       */
#define ADIS16220_ALM_MAG1      0x22
/*                                       */
#define ADIS16220_ALM_MAG2      0x24
/*                                  */
#define ADIS16220_ALM_MAGS      0x26
/*                                       */
#define ADIS16220_ALM_CTRL      0x28
/*                                    */
#define ADIS16220_GPIO_CTRL     0x32
/*                                               */
#define ADIS16220_MSC_CTRL      0x34
/*                                    */
#define ADIS16220_DIO_CTRL      0x36
/*                               */
#define ADIS16220_AVG_CNT       0x38
/*                       */
#define ADIS16220_DIAG_STAT     0x3C
/*                          */
#define ADIS16220_GLOB_CMD      0x3E
/*                            */
#define ADIS16220_ST_DELTA      0x40
/*                           */
#define ADIS16220_LOT_ID1       0x52
/*                           */
#define ADIS16220_LOT_ID2       0x54
/*                                                */
#define ADIS16220_PROD_ID       0x56
/*               */
#define ADIS16220_SERIAL_NUM    0x58

#define ADIS16220_CAPTURE_SIZE  2048

/*          */
#define ADIS16220_MSC_CTRL_SELF_TEST_EN	        (1 << 8)
#define ADIS16220_MSC_CTRL_POWER_SUP_COM_AIN1	(1 << 1)
#define ADIS16220_MSC_CTRL_POWER_SUP_COM_AIN2	(1 << 0)

/*          */
#define ADIS16220_MSC_CTRL_DIO2_BUSY_IND     (3<<4)
#define ADIS16220_MSC_CTRL_DIO1_BUSY_IND     (3<<2)
#define ADIS16220_MSC_CTRL_DIO2_ACT_HIGH     (1<<1)
#define ADIS16220_MSC_CTRL_DIO1_ACT_HIGH     (1<<0)

/*           */
/*                        */
#define ADIS16220_DIAG_STAT_ALM_MAG2    (1<<14)
/*                        */
#define ADIS16220_DIAG_STAT_ALM_MAG1    (1<<13)
/*                                */
#define ADIS16220_DIAG_STAT_ALM_MAGA    (1<<12)
/*                                                                          */
#define ADIS16220_DIAG_STAT_ALM_MAGS    (1<<11)
/*                                              */
#define ADIS16220_DIAG_STAT_PEAK_AIN2   (1<<10)
/*                                              */
#define ADIS16220_DIAG_STAT_PEAK_AIN1   (1<<9)
/*                                                      */
#define ADIS16220_DIAG_STAT_PEAK_ACCEL  (1<<8)
/*                              */
#define ADIS16220_DIAG_STAT_DATA_RDY    (1<<7)
#define ADIS16220_DIAG_STAT_FLASH_CHK	(1<<6)
#define ADIS16220_DIAG_STAT_SELF_TEST	(1<<5)
/*                                       */
#define ADIS16220_DIAG_STAT_VIOLATION	(1<<4)
/*                            */
#define ADIS16220_DIAG_STAT_SPI_FAIL	(1<<3)
/*                      */
#define ADIS16220_DIAG_STAT_FLASH_UPT	(1<<2)
/*                            */
#define ADIS16220_DIAG_STAT_POWER_HIGH	(1<<1)
/*                           */
#define ADIS16220_DIAG_STAT_POWER_LOW	(1<<0)

/*          */
#define ADIS16220_GLOB_CMD_SW_RESET	(1<<7)
#define ADIS16220_GLOB_CMD_SELF_TEST	(1<<2)
#define ADIS16220_GLOB_CMD_PWR_DOWN	(1<<1)

#define ADIS16220_MAX_TX 2048
#define ADIS16220_MAX_RX 2048

#define ADIS16220_SPI_BURST	(u32)(1000 * 1000)
#define ADIS16220_SPI_FAST	(u32)(2000 * 1000)

/* 
                                                         
                           
                         
                        
                                         
  */
struct adis16220_state {
	struct spi_device	*us;
	struct mutex		buf_lock;
	u8			tx[ADIS16220_MAX_TX] ____cacheline_aligned;
	u8			rx[ADIS16220_MAX_RX];
};

#endif /*                  */
