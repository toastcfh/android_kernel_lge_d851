/*
            
  
                                 
                               
                                        
                                  
  
                                                                   
  
                                                               
                         
  
                                                   
  
                                                                    
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/gfp.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/in.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>

#include <asm/io.h>
#include <asm/pgtable.h>
#include <asm/dma.h>

static char xtsonic_string[] = "xtsonic";

extern unsigned xtboard_nvram_valid(void);
extern void xtboard_get_ether_addr(unsigned char *buf);

#include "sonic.h"

/*
                                                                    
                                                                      
                                                                   
                                            
  
                                                                          
                     
 */
#undef SONIC_RBSIZE
#define SONIC_RBSIZE	1524

/*
                                             
 */
#define SONIC_MEM_SIZE	0x100

/*
                                   
 */
#define SONIC_READ(reg) \
	(0xffff & *((volatile unsigned int *)dev->base_addr+reg))

#define SONIC_WRITE(reg,val) \
	*((volatile unsigned int *)dev->base_addr+reg) = val


/*                                                            */
#ifdef SONIC_DEBUG
static unsigned int sonic_debug = SONIC_DEBUG;
#else
static unsigned int sonic_debug = 1;
#endif

/*
                                                                  
                                                                
                                                      
 */
static unsigned short known_revisions[] =
{
	0x101,			/*             */
	0xffff			/*             */
};

static int xtsonic_open(struct net_device *dev)
{
	int retval;

	retval = request_irq(dev->irq, sonic_interrupt, IRQF_DISABLED,
				"sonic", dev);
	if (retval) {
		printk(KERN_ERR "%s: unable to get IRQ %d.\n",
		       dev->name, dev->irq);
		return -EAGAIN;
	}

	retval = sonic_open(dev);
	if (retval)
		free_irq(dev->irq, dev);
	return retval;
}

static int xtsonic_close(struct net_device *dev)
{
	int err;
	err = sonic_close(dev);
	free_irq(dev->irq, dev);
	return err;
}

static const struct net_device_ops xtsonic_netdev_ops = {
	.ndo_open		= xtsonic_open,
	.ndo_stop		= xtsonic_close,
	.ndo_start_xmit		= sonic_send_packet,
	.ndo_get_stats		= sonic_get_stats,
	.ndo_set_rx_mode	= sonic_multicast_list,
	.ndo_tx_timeout		= sonic_tx_timeout,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_change_mtu		= eth_change_mtu,
	.ndo_set_mac_address	= eth_mac_addr,
};

static int __init sonic_probe1(struct net_device *dev)
{
	static unsigned version_printed = 0;
	unsigned int silicon_revision;
	struct sonic_local *lp = netdev_priv(dev);
	unsigned int base_addr = dev->base_addr;
	int i;
	int err = 0;

	if (!request_mem_region(base_addr, 0x100, xtsonic_string))
		return -EBUSY;

	/*
                                                            
                                                           
                          
  */
	silicon_revision = SONIC_READ(SONIC_SR);
	if (sonic_debug > 1)
		printk("SONIC Silicon Revision = 0x%04x\n",silicon_revision);

	i = 0;
	while ((known_revisions[i] != 0xffff) &&
			(known_revisions[i] != silicon_revision))
		i++;

	if (known_revisions[i] == 0xffff) {
		printk("SONIC ethernet controller not found (0x%4x)\n",
				silicon_revision);
		return -ENODEV;
	}

	if (sonic_debug  &&  version_printed++ == 0)
		printk(version);

	/*
                                                                      
                                                                       
  */
	SONIC_WRITE(SONIC_CMD,SONIC_CR_RST);
	SONIC_WRITE(SONIC_DCR,
		    SONIC_DCR_WC0|SONIC_DCR_DW|SONIC_DCR_LBR|SONIC_DCR_SBUS);
	SONIC_WRITE(SONIC_CEP,0);
	SONIC_WRITE(SONIC_IMR,0);

	SONIC_WRITE(SONIC_CMD,SONIC_CR_RST);
	SONIC_WRITE(SONIC_CEP,0);

	for (i=0; i<3; i++) {
		unsigned int val = SONIC_READ(SONIC_CAP0-i);
		dev->dev_addr[i*2] = val;
		dev->dev_addr[i*2+1] = val >> 8;
	}

	/*                                  */

	lp->dma_bitmode = SONIC_BITMODE32;

	/*
                                                               
                                                                       
                                                         
                                                         
                                                        
                                                            
                                                                
                                                           
  */
	lp->descriptors =
		dma_alloc_coherent(lp->device,
			SIZEOF_SONIC_DESC * SONIC_BUS_SCALE(lp->dma_bitmode),
			&lp->descriptors_laddr, GFP_KERNEL);

	if (lp->descriptors == NULL) {
		printk(KERN_ERR "%s: couldn't alloc DMA memory for "
				" descriptors.\n", dev_name(lp->device));
		goto out;
	}

	lp->cda = lp->descriptors;
	lp->tda = lp->cda + (SIZEOF_SONIC_CDA
			     * SONIC_BUS_SCALE(lp->dma_bitmode));
	lp->rda = lp->tda + (SIZEOF_SONIC_TD * SONIC_NUM_TDS
			     * SONIC_BUS_SCALE(lp->dma_bitmode));
	lp->rra = lp->rda + (SIZEOF_SONIC_RD * SONIC_NUM_RDS
			     * SONIC_BUS_SCALE(lp->dma_bitmode));

	/*                             */

	lp->cda_laddr = lp->descriptors_laddr;
	lp->tda_laddr = lp->cda_laddr + (SIZEOF_SONIC_CDA
				         * SONIC_BUS_SCALE(lp->dma_bitmode));
	lp->rda_laddr = lp->tda_laddr + (SIZEOF_SONIC_TD * SONIC_NUM_TDS
					 * SONIC_BUS_SCALE(lp->dma_bitmode));
	lp->rra_laddr = lp->rda_laddr + (SIZEOF_SONIC_RD * SONIC_NUM_RDS
					 * SONIC_BUS_SCALE(lp->dma_bitmode));

	dev->netdev_ops		= &xtsonic_netdev_ops;
	dev->watchdog_timeo	= TX_TIMEOUT;

	/*
                       
  */
	SONIC_WRITE(SONIC_CRCT,0xffff);
	SONIC_WRITE(SONIC_FAET,0xffff);
	SONIC_WRITE(SONIC_MPT,0xffff);

	return 0;
out:
	release_region(dev->base_addr, SONIC_MEM_SIZE);
	return err;
}


/*
                                                            
                                                      
 */

int __devinit xtsonic_probe(struct platform_device *pdev)
{
	struct net_device *dev;
	struct sonic_local *lp;
	struct resource *resmem, *resirq;
	int err = 0;

	if ((resmem = platform_get_resource(pdev, IORESOURCE_MEM, 0)) == NULL)
		return -ENODEV;

	if ((resirq = platform_get_resource(pdev, IORESOURCE_IRQ, 0)) == NULL)
		return -ENODEV;

	if ((dev = alloc_etherdev(sizeof(struct sonic_local))) == NULL)
		return -ENOMEM;

	lp = netdev_priv(dev);
	lp->device = &pdev->dev;
	SET_NETDEV_DEV(dev, &pdev->dev);
	netdev_boot_setup_check(dev);

	dev->base_addr = resmem->start;
	dev->irq = resirq->start;

	if ((err = sonic_probe1(dev)))
		goto out;
	if ((err = register_netdev(dev)))
		goto out1;

	printk("%s: SONIC ethernet @%08lx, MAC %pM, IRQ %d\n", dev->name,
	       dev->base_addr, dev->dev_addr, dev->irq);

	return 0;

out1:
	release_region(dev->base_addr, SONIC_MEM_SIZE);
out:
	free_netdev(dev);

	return err;
}

MODULE_DESCRIPTION("Xtensa XT2000 SONIC ethernet driver");
module_param(sonic_debug, int, 0);
MODULE_PARM_DESC(sonic_debug, "xtsonic debug level (1-4)");

#include "sonic.c"

static int __devexit xtsonic_device_remove (struct platform_device *pdev)
{
	struct net_device *dev = platform_get_drvdata(pdev);
	struct sonic_local *lp = netdev_priv(dev);

	unregister_netdev(dev);
	dma_free_coherent(lp->device,
			  SIZEOF_SONIC_DESC * SONIC_BUS_SCALE(lp->dma_bitmode),
			  lp->descriptors, lp->descriptors_laddr);
	release_region (dev->base_addr, SONIC_MEM_SIZE);
	free_netdev(dev);

	return 0;
}

static struct platform_driver xtsonic_driver = {
	.probe = xtsonic_probe,
	.remove = __devexit_p(xtsonic_device_remove),
	.driver = {
		.name = xtsonic_string,
	},
};

module_platform_driver(xtsonic_driver);
