/*
 * Driver for Intel MSIC
 *
 * Copyright (C) 2011, Intel Corporation
 * Author: Mika Westerberg <mika.westerberg@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/mfd/core.h>
#include <linux/mfd/intel_msic.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#include <asm/intel_scu_ipc.h>

#define MSIC_VENDOR(id)		((id >> 6) & 3)
#define MSIC_VERSION(id)	(id & 0x3f)
#define MSIC_MAJOR(id)		('A' + ((id >> 3) & 7))
#define MSIC_MINOR(id)		(id & 7)

/*
                                                                         
                                                                           
                                          
 */
#define MSIC_IRQ_STATUS(x)	(INTEL_MSIC_IRQ_PHYS_BASE + ((x) - 2))
#define MSIC_IRQ_STATUS_ACCDET	MSIC_IRQ_STATUS(INTEL_MSIC_ACCDET)

/*
                                                                       
            
 */
#define SCU_IPC_RWBUF_LIMIT	16

/* 
                                           
                                        
                     
                         
                                                                 
 */
struct intel_msic {
	struct platform_device		*pdev;
	unsigned			vendor;
	unsigned			version;
	void __iomem			*irq_base;
};

static struct resource msic_touch_resources[] = {
	{
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource msic_adc_resources[] = {
	{
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource msic_battery_resources[] = {
	{
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource msic_gpio_resources[] = {
	{
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource msic_audio_resources[] = {
	{
		.name		= "IRQ",
		.flags		= IORESOURCE_IRQ,
	},
	/*
                                                                   
                                                           
  */
	{
		.name		= "IRQ_BASE",
		.flags		= IORESOURCE_MEM,
		.start		= MSIC_IRQ_STATUS_ACCDET,
		.end		= MSIC_IRQ_STATUS_ACCDET,
	},
};

static struct resource msic_hdmi_resources[] = {
	{
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource msic_thermal_resources[] = {
	{
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource msic_power_btn_resources[] = {
	{
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource msic_ocd_resources[] = {
	{
		.flags		= IORESOURCE_IRQ,
	},
};

/*
                                                                   
                            
 */
static struct mfd_cell msic_devs[] = {
	[INTEL_MSIC_BLOCK_TOUCH]	= {
		.name			= "msic_touch",
		.num_resources		= ARRAY_SIZE(msic_touch_resources),
		.resources		= msic_touch_resources,
	},
	[INTEL_MSIC_BLOCK_ADC]		= {
		.name			= "msic_adc",
		.num_resources		= ARRAY_SIZE(msic_adc_resources),
		.resources		= msic_adc_resources,
	},
	[INTEL_MSIC_BLOCK_BATTERY]	= {
		.name			= "msic_battery",
		.num_resources		= ARRAY_SIZE(msic_battery_resources),
		.resources		= msic_battery_resources,
	},
	[INTEL_MSIC_BLOCK_GPIO]		= {
		.name			= "msic_gpio",
		.num_resources		= ARRAY_SIZE(msic_gpio_resources),
		.resources		= msic_gpio_resources,
	},
	[INTEL_MSIC_BLOCK_AUDIO]	= {
		.name			= "msic_audio",
		.num_resources		= ARRAY_SIZE(msic_audio_resources),
		.resources		= msic_audio_resources,
	},
	[INTEL_MSIC_BLOCK_HDMI]		= {
		.name			= "msic_hdmi",
		.num_resources		= ARRAY_SIZE(msic_hdmi_resources),
		.resources		= msic_hdmi_resources,
	},
	[INTEL_MSIC_BLOCK_THERMAL]	= {
		.name			= "msic_thermal",
		.num_resources		= ARRAY_SIZE(msic_thermal_resources),
		.resources		= msic_thermal_resources,
	},
	[INTEL_MSIC_BLOCK_POWER_BTN]	= {
		.name			= "msic_power_btn",
		.num_resources		= ARRAY_SIZE(msic_power_btn_resources),
		.resources		= msic_power_btn_resources,
	},
	[INTEL_MSIC_BLOCK_OCD]		= {
		.name			= "msic_ocd",
		.num_resources		= ARRAY_SIZE(msic_ocd_resources),
		.resources		= msic_ocd_resources,
	},
};

/*
                                                                           
                                                                           
                      
  
                                                                           
                                                        
 */
static struct mfd_cell msic_other_devs[] = {
	/*                         */
	{
		.id			= -1,
		.name			= "sn95031",
	},
};

/* 
                                                    
                         
                                      
  
                                                                       
                            
  
                      
 */
int intel_msic_reg_read(unsigned short reg, u8 *val)
{
	return intel_scu_ipc_ioread8(reg, val);
}
EXPORT_SYMBOL_GPL(intel_msic_reg_read);

/* 
                                                      
                          
                                        
  
                                                                  
                            
  
                      
 */
int intel_msic_reg_write(unsigned short reg, u8 val)
{
	return intel_scu_ipc_iowrite8(reg, val);
}
EXPORT_SYMBOL_GPL(intel_msic_reg_write);

/* 
                                                        
                           
                                       
                                                                     
                      
  
                                                                       
                                                                        
                   
  
                      
 */
int intel_msic_reg_update(unsigned short reg, u8 val, u8 mask)
{
	return intel_scu_ipc_update_register(reg, val, mask);
}
EXPORT_SYMBOL_GPL(intel_msic_reg_update);

/* 
                                                    
                                            
                                               
                                      
  
                                                                          
                                                                       
             
  
                                                                       
  
                      
 */
int intel_msic_bulk_read(unsigned short *reg, u8 *buf, size_t count)
{
	if (WARN_ON(count > SCU_IPC_RWBUF_LIMIT))
		return -EINVAL;

	return intel_scu_ipc_readv(reg, buf, count);
}
EXPORT_SYMBOL_GPL(intel_msic_bulk_read);

/* 
                                                                         
                                    
                                         
                                       
  
                                                                         
                                                                           
                   
  
                      
 */
int intel_msic_bulk_write(unsigned short *reg, u8 *buf, size_t count)
{
	if (WARN_ON(count > SCU_IPC_RWBUF_LIMIT))
		return -EINVAL;

	return intel_scu_ipc_writev(reg, buf, count);
}
EXPORT_SYMBOL_GPL(intel_msic_bulk_write);

/* 
                                                                    
                       
                                                            
                          
                                             
  
                                                                           
                                                                       
                                                                          
                              
  
                                                                 
  
                                                                      
 */
int intel_msic_irq_read(struct intel_msic *msic, unsigned short reg, u8 *val)
{
	if (WARN_ON(reg < INTEL_MSIC_IRQLVL1 || reg > INTEL_MSIC_RESETIRQ2))
		return -EINVAL;

	*val = readb(msic->irq_base + (reg - INTEL_MSIC_IRQLVL1));
	return 0;
}
EXPORT_SYMBOL_GPL(intel_msic_irq_read);

static int __devinit intel_msic_init_devices(struct intel_msic *msic)
{
	struct platform_device *pdev = msic->pdev;
	struct intel_msic_platform_data *pdata = pdev->dev.platform_data;
	int ret, i;

	if (pdata->gpio) {
		struct mfd_cell *cell = &msic_devs[INTEL_MSIC_BLOCK_GPIO];

		cell->platform_data = pdata->gpio;
		cell->pdata_size = sizeof(*pdata->gpio);
	}

	if (pdata->ocd) {
		unsigned gpio = pdata->ocd->gpio;

		ret = gpio_request_one(gpio, GPIOF_IN, "ocd_gpio");
		if (ret) {
			dev_err(&pdev->dev, "failed to register OCD GPIO\n");
			return ret;
		}

		ret = gpio_to_irq(gpio);
		if (ret < 0) {
			dev_err(&pdev->dev, "no IRQ number for OCD GPIO\n");
			gpio_free(gpio);
			return ret;
		}

		/*                                   */
		pdata->irq[INTEL_MSIC_BLOCK_OCD] = ret;
	}

	for (i = 0; i < ARRAY_SIZE(msic_devs); i++) {
		if (!pdata->irq[i])
			continue;

		ret = mfd_add_devices(&pdev->dev, -1, &msic_devs[i], 1, NULL,
				      pdata->irq[i]);
		if (ret)
			goto fail;
	}

	ret = mfd_add_devices(&pdev->dev, 0, msic_other_devs,
			      ARRAY_SIZE(msic_other_devs), NULL, 0);
	if (ret)
		goto fail;

	return 0;

fail:
	mfd_remove_devices(&pdev->dev);
	if (pdata->ocd)
		gpio_free(pdata->ocd->gpio);

	return ret;
}

static void __devexit intel_msic_remove_devices(struct intel_msic *msic)
{
	struct platform_device *pdev = msic->pdev;
	struct intel_msic_platform_data *pdata = pdev->dev.platform_data;

	mfd_remove_devices(&pdev->dev);

	if (pdata->ocd)
		gpio_free(pdata->ocd->gpio);
}

static int __devinit intel_msic_probe(struct platform_device *pdev)
{
	struct intel_msic_platform_data *pdata = pdev->dev.platform_data;
	struct intel_msic *msic;
	struct resource *res;
	u8 id0, id1;
	int ret;

	if (!pdata) {
		dev_err(&pdev->dev, "no platform data passed\n");
		return -EINVAL;
	}

	/*                                              */
	ret = intel_scu_ipc_ioread8(INTEL_MSIC_ID0, &id0);
	if (ret) {
		dev_err(&pdev->dev, "failed to identify the MSIC chip (ID0)\n");
		return -ENXIO;
	}

	ret = intel_scu_ipc_ioread8(INTEL_MSIC_ID1, &id1);
	if (ret) {
		dev_err(&pdev->dev, "failed to identify the MSIC chip (ID1)\n");
		return -ENXIO;
	}

	if (MSIC_VENDOR(id0) != MSIC_VENDOR(id1)) {
		dev_err(&pdev->dev, "invalid vendor ID: %x, %x\n", id0, id1);
		return -ENXIO;
	}

	msic = kzalloc(sizeof(*msic), GFP_KERNEL);
	if (!msic)
		return -ENOMEM;

	msic->vendor = MSIC_VENDOR(id0);
	msic->version = MSIC_VERSION(id0);
	msic->pdev = pdev;

	/*
                                                                   
                                          
  */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "failed to get SRAM iomem resource\n");
		ret = -ENODEV;
		goto fail_free_msic;
	}

	res = request_mem_region(res->start, resource_size(res), pdev->name);
	if (!res) {
		ret = -EBUSY;
		goto fail_free_msic;
	}

	msic->irq_base = ioremap_nocache(res->start, resource_size(res));
	if (!msic->irq_base) {
		dev_err(&pdev->dev, "failed to map SRAM memory\n");
		ret = -ENOMEM;
		goto fail_release_region;
	}

	platform_set_drvdata(pdev, msic);

	ret = intel_msic_init_devices(msic);
	if (ret) {
		dev_err(&pdev->dev, "failed to initialize MSIC devices\n");
		goto fail_unmap_mem;
	}

	dev_info(&pdev->dev, "Intel MSIC version %c%d (vendor %#x)\n",
		 MSIC_MAJOR(msic->version), MSIC_MINOR(msic->version),
		 msic->vendor);

	return 0;

fail_unmap_mem:
	iounmap(msic->irq_base);
fail_release_region:
	release_mem_region(res->start, resource_size(res));
fail_free_msic:
	kfree(msic);

	return ret;
}

static int __devexit intel_msic_remove(struct platform_device *pdev)
{
	struct intel_msic *msic = platform_get_drvdata(pdev);
	struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	intel_msic_remove_devices(msic);
	platform_set_drvdata(pdev, NULL);
	iounmap(msic->irq_base);
	release_mem_region(res->start, resource_size(res));
	kfree(msic);

	return 0;
}

static struct platform_driver intel_msic_driver = {
	.probe		= intel_msic_probe,
	.remove		= __devexit_p(intel_msic_remove),
	.driver		= {
		.name	= "intel_msic",
		.owner	= THIS_MODULE,
	},
};

module_platform_driver(intel_msic_driver);

MODULE_DESCRIPTION("Driver for Intel MSIC");
MODULE_AUTHOR("Mika Westerberg <mika.westerberg@linux.intel.com>");
MODULE_LICENSE("GPL");