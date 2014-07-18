/*
 * Xilinx SPI controller driver (master mode only)
 *
 * Author: MontaVista Software, Inc.
 *	source@mvista.com
 *
 * Copyright (c) 2010 Secret Lab Technologies, Ltd.
 * Copyright (c) 2009 Intel Corporation
 * 2002-2007 (c) MontaVista Software, Inc.

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/spi/spi_bitbang.h>
#include <linux/spi/xilinx_spi.h>
#include <linux/io.h>

#define XILINX_SPI_NAME "xilinx_spi"

/*                                                                            
                                
 */
#define XSPI_CR_OFFSET		0x60	/*                  */

#define XSPI_CR_ENABLE		0x02
#define XSPI_CR_MASTER_MODE	0x04
#define XSPI_CR_CPOL		0x08
#define XSPI_CR_CPHA		0x10
#define XSPI_CR_MODE_MASK	(XSPI_CR_CPHA | XSPI_CR_CPOL)
#define XSPI_CR_TXFIFO_RESET	0x20
#define XSPI_CR_RXFIFO_RESET	0x40
#define XSPI_CR_MANUAL_SSELECT	0x80
#define XSPI_CR_TRANS_INHIBIT	0x100
#define XSPI_CR_LSB_FIRST	0x200

#define XSPI_SR_OFFSET		0x64	/*                 */

#define XSPI_SR_RX_EMPTY_MASK	0x01	/*                       */
#define XSPI_SR_RX_FULL_MASK	0x02	/*                      */
#define XSPI_SR_TX_EMPTY_MASK	0x04	/*                        */
#define XSPI_SR_TX_FULL_MASK	0x08	/*                       */
#define XSPI_SR_MODE_FAULT_MASK	0x10	/*                  */

#define XSPI_TXD_OFFSET		0x68	/*                        */
#define XSPI_RXD_OFFSET		0x6c	/*                       */

#define XSPI_SSR_OFFSET		0x70	/*                              */

/*                                                                             
                            
 */
#define XIPIF_V123B_DGIER_OFFSET	0x1c	/*                            */
#define XIPIF_V123B_GINTR_ENABLE	0x80000000

#define XIPIF_V123B_IISR_OFFSET		0x20	/*                           */
#define XIPIF_V123B_IIER_OFFSET		0x28	/*                           */

#define XSPI_INTR_MODE_FAULT		0x01	/*                  */
#define XSPI_INTR_SLAVE_MODE_FAULT	0x02	/*                        
                  */
#define XSPI_INTR_TX_EMPTY		0x04	/*                 */
#define XSPI_INTR_TX_UNDERRUN		0x08	/*                     */
#define XSPI_INTR_RX_FULL		0x10	/*                */
#define XSPI_INTR_RX_OVERRUN		0x20	/*                    */
#define XSPI_INTR_TX_HALF_EMPTY		0x40	/*                      */

#define XIPIF_V123B_RESETR_OFFSET	0x40	/*                     */
#define XIPIF_V123B_RESET_MASK		0x0a	/*                    */

struct xilinx_spi {
	/*                         */
	struct spi_bitbang bitbang;
	struct completion done;
	struct resource mem; /*          */
	void __iomem	*regs;	/*                                        */

	u32		irq;

	u8 *rx_ptr;		/*                          */
	const u8 *tx_ptr;	/*                          */
	int remaining_bytes;	/*                                      */
	u8 bits_per_word;
	unsigned int (*read_fn) (void __iomem *);
	void (*write_fn) (u32, void __iomem *);
	void (*tx_fn) (struct xilinx_spi *);
	void (*rx_fn) (struct xilinx_spi *);
};

static void xspi_write32(u32 val, void __iomem *addr)
{
	iowrite32(val, addr);
}

static unsigned int xspi_read32(void __iomem *addr)
{
	return ioread32(addr);
}

static void xspi_write32_be(u32 val, void __iomem *addr)
{
	iowrite32be(val, addr);
}

static unsigned int xspi_read32_be(void __iomem *addr)
{
	return ioread32be(addr);
}

static void xspi_tx8(struct xilinx_spi *xspi)
{
	xspi->write_fn(*xspi->tx_ptr, xspi->regs + XSPI_TXD_OFFSET);
	xspi->tx_ptr++;
}

static void xspi_tx16(struct xilinx_spi *xspi)
{
	xspi->write_fn(*(u16 *)(xspi->tx_ptr), xspi->regs + XSPI_TXD_OFFSET);
	xspi->tx_ptr += 2;
}

static void xspi_tx32(struct xilinx_spi *xspi)
{
	xspi->write_fn(*(u32 *)(xspi->tx_ptr), xspi->regs + XSPI_TXD_OFFSET);
	xspi->tx_ptr += 4;
}

static void xspi_rx8(struct xilinx_spi *xspi)
{
	u32 data = xspi->read_fn(xspi->regs + XSPI_RXD_OFFSET);
	if (xspi->rx_ptr) {
		*xspi->rx_ptr = data & 0xff;
		xspi->rx_ptr++;
	}
}

static void xspi_rx16(struct xilinx_spi *xspi)
{
	u32 data = xspi->read_fn(xspi->regs + XSPI_RXD_OFFSET);
	if (xspi->rx_ptr) {
		*(u16 *)(xspi->rx_ptr) = data & 0xffff;
		xspi->rx_ptr += 2;
	}
}

static void xspi_rx32(struct xilinx_spi *xspi)
{
	u32 data = xspi->read_fn(xspi->regs + XSPI_RXD_OFFSET);
	if (xspi->rx_ptr) {
		*(u32 *)(xspi->rx_ptr) = data;
		xspi->rx_ptr += 4;
	}
}

static void xspi_init_hw(struct xilinx_spi *xspi)
{
	void __iomem *regs_base = xspi->regs;

	/*                      */
	xspi->write_fn(XIPIF_V123B_RESET_MASK,
		regs_base + XIPIF_V123B_RESETR_OFFSET);
	/*                                         */
	xspi->write_fn(0, regs_base + XIPIF_V123B_IIER_OFFSET);
	/*                                  */
	xspi->write_fn(XIPIF_V123B_GINTR_ENABLE,
		regs_base + XIPIF_V123B_DGIER_OFFSET);
	/*                                   */
	xspi->write_fn(0xffff, regs_base + XSPI_SSR_OFFSET);
	/*                                                               
                                                       */
	xspi->write_fn(XSPI_CR_TRANS_INHIBIT | XSPI_CR_MANUAL_SSELECT |
		XSPI_CR_MASTER_MODE | XSPI_CR_ENABLE | XSPI_CR_TXFIFO_RESET |
		XSPI_CR_RXFIFO_RESET, regs_base + XSPI_CR_OFFSET);
}

static void xilinx_spi_chipselect(struct spi_device *spi, int is_on)
{
	struct xilinx_spi *xspi = spi_master_get_devdata(spi->master);

	if (is_on == BITBANG_CS_INACTIVE) {
		/*                                   */
		xspi->write_fn(0xffff, xspi->regs + XSPI_SSR_OFFSET);
	} else if (is_on == BITBANG_CS_ACTIVE) {
		/*                                      */
		u16 cr = xspi->read_fn(xspi->regs + XSPI_CR_OFFSET)
			 & ~XSPI_CR_MODE_MASK;
		if (spi->mode & SPI_CPHA)
			cr |= XSPI_CR_CPHA;
		if (spi->mode & SPI_CPOL)
			cr |= XSPI_CR_CPOL;
		xspi->write_fn(cr, xspi->regs + XSPI_CR_OFFSET);

		/*                                                        
                                                                
               
   */

		/*                          */
		xspi->write_fn(~(0x0001 << spi->chip_select),
			xspi->regs + XSPI_SSR_OFFSET);
	}
}

/*                                                                         
                                                                        
                                                                      
                                                 
                                                                           
                                                                             
 */
static int xilinx_spi_setup_transfer(struct spi_device *spi,
		struct spi_transfer *t)
{
	struct xilinx_spi *xspi = spi_master_get_devdata(spi->master);
	u8 bits_per_word;

	bits_per_word = (t && t->bits_per_word)
			 ? t->bits_per_word : spi->bits_per_word;
	if (bits_per_word != xspi->bits_per_word) {
		dev_err(&spi->dev, "%s, unsupported bits_per_word=%d\n",
			__func__, bits_per_word);
		return -EINVAL;
	}

	return 0;
}

static int xilinx_spi_setup(struct spi_device *spi)
{
	/*                                                      
                                                                 
                                                                 
                                                                
                                                                       
                                                                   
                                        
                                                                   
                             
  */
	return 0;
}

static void xilinx_spi_fill_tx_fifo(struct xilinx_spi *xspi)
{
	u8 sr;

	/*                                                 */
	sr = xspi->read_fn(xspi->regs + XSPI_SR_OFFSET);
	while ((sr & XSPI_SR_TX_FULL_MASK) == 0 && xspi->remaining_bytes > 0) {
		if (xspi->tx_ptr)
			xspi->tx_fn(xspi);
		else
			xspi->write_fn(0, xspi->regs + XSPI_TXD_OFFSET);
		xspi->remaining_bytes -= xspi->bits_per_word / 8;
		sr = xspi->read_fn(xspi->regs + XSPI_SR_OFFSET);
	}
}

static int xilinx_spi_txrx_bufs(struct spi_device *spi, struct spi_transfer *t)
{
	struct xilinx_spi *xspi = spi_master_get_devdata(spi->master);
	u32 ipif_ier;
	u16 cr;

	/*                                        */

	xspi->tx_ptr = t->tx_buf;
	xspi->rx_ptr = t->rx_buf;
	xspi->remaining_bytes = t->len;
	INIT_COMPLETION(xspi->done);

	xilinx_spi_fill_tx_fifo(xspi);

	/*                                                               
                                 
  */
	ipif_ier = xspi->read_fn(xspi->regs + XIPIF_V123B_IIER_OFFSET);
	xspi->write_fn(ipif_ier | XSPI_INTR_TX_EMPTY,
		xspi->regs + XIPIF_V123B_IIER_OFFSET);

	/*                                                                 */
	cr = xspi->read_fn(xspi->regs + XSPI_CR_OFFSET) &
		~XSPI_CR_TRANS_INHIBIT;
	xspi->write_fn(cr, xspi->regs + XSPI_CR_OFFSET);

	wait_for_completion(&xspi->done);

	/*                                      */
	xspi->write_fn(ipif_ier, xspi->regs + XIPIF_V123B_IIER_OFFSET);

	return t->len - xspi->remaining_bytes;
}


/*                                                                  
                                       
                                                                           
                           
 */
static irqreturn_t xilinx_spi_irq(int irq, void *dev_id)
{
	struct xilinx_spi *xspi = dev_id;
	u32 ipif_isr;

	/*                                                     */
	ipif_isr = xspi->read_fn(xspi->regs + XIPIF_V123B_IISR_OFFSET);
	xspi->write_fn(ipif_isr, xspi->regs + XIPIF_V123B_IISR_OFFSET);

	if (ipif_isr & XSPI_INTR_TX_EMPTY) {	/*                        */
		u16 cr;
		u8 sr;

		/*                                                         
                                                        
                                                                  
                                              
   */
		cr = xspi->read_fn(xspi->regs + XSPI_CR_OFFSET);
		xspi->write_fn(cr | XSPI_CR_TRANS_INHIBIT,
			xspi->regs + XSPI_CR_OFFSET);

		/*                                        */
		sr = xspi->read_fn(xspi->regs + XSPI_SR_OFFSET);
		while ((sr & XSPI_SR_RX_EMPTY_MASK) == 0) {
			xspi->rx_fn(xspi);
			sr = xspi->read_fn(xspi->regs + XSPI_SR_OFFSET);
		}

		/*                                   */
		if (xspi->remaining_bytes > 0) {
			xilinx_spi_fill_tx_fifo(xspi);
			/*                                         
                            
    */
			xspi->write_fn(cr, xspi->regs + XSPI_CR_OFFSET);
		} else {
			/*                      
                                         
    */
			complete(&xspi->done);
		}
	}

	return IRQ_HANDLED;
}

static const struct of_device_id xilinx_spi_of_match[] = {
	{ .compatible = "xlnx,xps-spi-2.00.a", },
	{ .compatible = "xlnx,xps-spi-2.00.b", },
	{}
};
MODULE_DEVICE_TABLE(of, xilinx_spi_of_match);

struct spi_master *xilinx_spi_init(struct device *dev, struct resource *mem,
	u32 irq, s16 bus_num, int num_cs, int little_endian, int bits_per_word)
{
	struct spi_master *master;
	struct xilinx_spi *xspi;
	int ret;

	master = spi_alloc_master(dev, sizeof(struct xilinx_spi));
	if (!master)
		return NULL;

	/*                                               */
	master->mode_bits = SPI_CPOL | SPI_CPHA;

	xspi = spi_master_get_devdata(master);
	xspi->bitbang.master = spi_master_get(master);
	xspi->bitbang.chipselect = xilinx_spi_chipselect;
	xspi->bitbang.setup_transfer = xilinx_spi_setup_transfer;
	xspi->bitbang.txrx_bufs = xilinx_spi_txrx_bufs;
	xspi->bitbang.master->setup = xilinx_spi_setup;
	init_completion(&xspi->done);

	if (!request_mem_region(mem->start, resource_size(mem),
		XILINX_SPI_NAME))
		goto put_master;

	xspi->regs = ioremap(mem->start, resource_size(mem));
	if (xspi->regs == NULL) {
		dev_warn(dev, "ioremap failure\n");
		goto map_failed;
	}

	master->bus_num = bus_num;
	master->num_chipselect = num_cs;
	master->dev.of_node = dev->of_node;

	xspi->mem = *mem;
	xspi->irq = irq;
	if (little_endian) {
		xspi->read_fn = xspi_read32;
		xspi->write_fn = xspi_write32;
	} else {
		xspi->read_fn = xspi_read32_be;
		xspi->write_fn = xspi_write32_be;
	}
	xspi->bits_per_word = bits_per_word;
	if (xspi->bits_per_word == 8) {
		xspi->tx_fn = xspi_tx8;
		xspi->rx_fn = xspi_rx8;
	} else if (xspi->bits_per_word == 16) {
		xspi->tx_fn = xspi_tx16;
		xspi->rx_fn = xspi_rx16;
	} else if (xspi->bits_per_word == 32) {
		xspi->tx_fn = xspi_tx32;
		xspi->rx_fn = xspi_rx32;
	} else
		goto unmap_io;


	/*                                */
	xspi_init_hw(xspi);

	/*                            */
	ret = request_irq(xspi->irq, xilinx_spi_irq, 0, XILINX_SPI_NAME, xspi);
	if (ret)
		goto unmap_io;

	ret = spi_bitbang_start(&xspi->bitbang);
	if (ret) {
		dev_err(dev, "spi_bitbang_start FAILED\n");
		goto free_irq;
	}

	dev_info(dev, "at 0x%08llX mapped to 0x%p, irq=%d\n",
		(unsigned long long)mem->start, xspi->regs, xspi->irq);
	return master;

free_irq:
	free_irq(xspi->irq, xspi);
unmap_io:
	iounmap(xspi->regs);
map_failed:
	release_mem_region(mem->start, resource_size(mem));
put_master:
	spi_master_put(master);
	return NULL;
}
EXPORT_SYMBOL(xilinx_spi_init);

void xilinx_spi_deinit(struct spi_master *master)
{
	struct xilinx_spi *xspi;

	xspi = spi_master_get_devdata(master);

	spi_bitbang_stop(&xspi->bitbang);
	free_irq(xspi->irq, xspi);
	iounmap(xspi->regs);

	release_mem_region(xspi->mem.start, resource_size(&xspi->mem));
	spi_master_put(xspi->bitbang.master);
}
EXPORT_SYMBOL(xilinx_spi_deinit);

static int __devinit xilinx_spi_probe(struct platform_device *dev)
{
	struct xspi_platform_data *pdata;
	struct resource *r;
	int irq, num_cs = 0, little_endian = 0, bits_per_word = 8;
	struct spi_master *master;
	u8 i;

	pdata = dev->dev.platform_data;
	if (pdata) {
		num_cs = pdata->num_chipselect;
		little_endian = pdata->little_endian;
		bits_per_word = pdata->bits_per_word;
	}

#ifdef CONFIG_OF
	if (dev->dev.of_node) {
		const __be32 *prop;
		int len;

		/*                                         */
		prop = of_get_property(dev->dev.of_node, "xlnx,num-ss-bits",
				       &len);
		if (prop && len >= sizeof(*prop))
			num_cs = __be32_to_cpup(prop);
	}
#endif

	if (!num_cs) {
		dev_err(&dev->dev, "Missing slave select configuration data\n");
		return -EINVAL;
	}


	r = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (!r)
		return -ENODEV;

	irq = platform_get_irq(dev, 0);
	if (irq < 0)
		return -ENXIO;

	master = xilinx_spi_init(&dev->dev, r, irq, dev->id, num_cs,
				 little_endian, bits_per_word);
	if (!master)
		return -ENODEV;

	if (pdata) {
		for (i = 0; i < pdata->num_devices; i++)
			spi_new_device(master, pdata->devices + i);
	}

	platform_set_drvdata(dev, master);
	return 0;
}

static int __devexit xilinx_spi_remove(struct platform_device *dev)
{
	xilinx_spi_deinit(platform_get_drvdata(dev));
	platform_set_drvdata(dev, 0);

	return 0;
}

/*                                */
MODULE_ALIAS("platform:" XILINX_SPI_NAME);

static struct platform_driver xilinx_spi_driver = {
	.probe = xilinx_spi_probe,
	.remove = __devexit_p(xilinx_spi_remove),
	.driver = {
		.name = XILINX_SPI_NAME,
		.owner = THIS_MODULE,
		.of_match_table = xilinx_spi_of_match,
	},
};
module_platform_driver(xilinx_spi_driver);

MODULE_AUTHOR("MontaVista Software, Inc. <source@mvista.com>");
MODULE_DESCRIPTION("Xilinx SPI driver");
MODULE_LICENSE("GPL");
