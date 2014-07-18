/*
                
  
 */

#ifndef __FELICA_GPIO_H__
#define __FELICA_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
                            
 */

#include <linux/list.h>
#include <linux/gpio.h>

#include "felica_common.h"
/*
          
 */

/*        */
enum{
  GPIO_DIRECTION_IN = 0,
  GPIO_DIRECTION_OUT,
};

enum{
  GPIO_LOW_VALUE = 0,
  GPIO_HIGH_VALUE,
};

enum{
  GPIO_CONFIG_ENABLE = 0,
  GPIO_CONFIG_DISABLE,
};


#define GPIO_FELICA_PON   74
#define GPIO_FELICA_RFS_REV_A   102
#define GPIO_FELICA_RFS   82
#define GPIO_FELICA_INT   46
#define GPIO_FELICA_LOCKCONT   45


/*
                      
 */
int felica_gpio_open(int gpionum, int direction, int value);
void felica_gpio_write(int gpionum, int value);
int felica_gpio_read(int gpionum);
int felica_get_rfs_gpio_num(void);
int felica_get_int_gpio_num(void);

#ifdef __cplusplus
}
#endif

#endif //                 
