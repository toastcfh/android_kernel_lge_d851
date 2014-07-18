/*
 * SuperH on-chip serial module support.  (SCI with no FIFO / with FIFO)
 *
 *  Copyright (C) 2002 - 2011  Paul Mundt
 *  Modified to support SH7720 SCIF. Markus Brunner, Mark Jonas (Jul 2007).
 *
 * based off of the old drivers/char/sh-sci.c by:
 *
 *   Copyright (C) 1999, 2000  Niibe Yutaka
 *   Copyright (C) 2000  Sugioka Toshinobu
 *   Modified to support multiple serial ports. Stuart Menefy (May 2000).
 *   Modified to support SecureEdge. David McCullough (2002)
 *   Modified to support SH7300 SCIF. Takashi Kusuda (Jun 2003).
 *   Removed SH7300 support (Jul 2007).
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#if defined(CONFIG_SERIAL_SH_SCI_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
#define SUPPORT_SYSRQ
#endif

#undef DEBUG

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial.h>
#include <linux/major.h>
#include <linux/string.h>
#include <linux/sysrq.h>
#include <linux/ioport.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/console.h>
#include <linux/platform_device.h>
#include <linux/serial_sci.h>
#include <linux/notifier.h>
#include <linux/pm_runtime.h>
#include <linux/cpufreq.h>
#include <linux/clk.h>
#include <linux/ctype.h>
#include <linux/err.h>
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/gpio.h>

#ifdef CONFIG_SUPERH
#include <asm/sh_bios.h>
#endif

#include "sh-sci.h"

struct sci_port {
	struct uart_port	port;

	/*                        */
	struct plat_sci_port	*cfg;

	/*             */
	struct timer_list	break_timer;
	int			break_flag;

	/*                 */
	struct clk		*iclk;
	/*                */
	struct clk		*fclk;

	char			*irqstr[SCIx_NR_IRQS];
	char			*gpiostr[SCIx_NR_FNS];

	struct dma_chan			*chan_tx;
	struct dma_chan			*chan_rx;

#ifdef CONFIG_SERIAL_SH_SCI_DMA
	struct dma_async_tx_descriptor	*desc_tx;
	struct dma_async_tx_descriptor	*desc_rx[2];
	dma_cookie_t			cookie_tx;
	dma_cookie_t			cookie_rx[2];
	dma_cookie_t			active_rx;
	struct scatterlist		sg_tx;
	unsigned int			sg_len_tx;
	struct scatterlist		sg_rx[2];
	size_t				buf_len_rx;
	struct sh_dmae_slave		param_tx;
	struct sh_dmae_slave		param_rx;
	struct work_struct		work_tx;
	struct work_struct		work_rx;
	struct timer_list		rx_timer;
	unsigned int			rx_timeout;
#endif

	struct notifier_block		freq_transition;

#ifdef CONFIG_SERIAL_SH_SCI_CONSOLE
	unsigned short saved_smr;
	unsigned short saved_fcr;
	unsigned char saved_brr;
#endif
};

/*                     */
static void sci_start_tx(struct uart_port *port);
static void sci_stop_tx(struct uart_port *port);
static void sci_start_rx(struct uart_port *port);

#define SCI_NPORTS CONFIG_SERIAL_SH_SCI_NR_UARTS

static struct sci_port sci_ports[SCI_NPORTS];
static struct uart_driver sci_uart_driver;

static inline struct sci_port *
to_sci_port(struct uart_port *uart)
{
	return container_of(uart, struct sci_port, port);
}

struct plat_sci_reg {
	u8 offset, size;
};

/*                                                               */
#define sci_reg_invalid	{ .offset = 0, .size = 0 }

static struct plat_sci_reg sci_regmap[SCIx_NR_REGTYPES][SCIx_NR_REGS] = {
	[SCIx_PROBE_REGTYPE] = {
		[0 ... SCIx_NR_REGS - 1] = sci_reg_invalid,
	},

	/*
                                                            
          
  */
	[SCIx_SCI_REGTYPE] = {
		[SCSMR]		= { 0x00,  8 },
		[SCBRR]		= { 0x01,  8 },
		[SCSCR]		= { 0x02,  8 },
		[SCxTDR]	= { 0x03,  8 },
		[SCxSR]		= { 0x04,  8 },
		[SCxRDR]	= { 0x05,  8 },
		[SCFCR]		= sci_reg_invalid,
		[SCFDR]		= sci_reg_invalid,
		[SCTFDR]	= sci_reg_invalid,
		[SCRFDR]	= sci_reg_invalid,
		[SCSPTR]	= sci_reg_invalid,
		[SCLSR]		= sci_reg_invalid,
	},

	/*
                                                          
                   
  */
	[SCIx_IRDA_REGTYPE] = {
		[SCSMR]		= { 0x00,  8 },
		[SCBRR]		= { 0x01,  8 },
		[SCSCR]		= { 0x02,  8 },
		[SCxTDR]	= { 0x03,  8 },
		[SCxSR]		= { 0x04,  8 },
		[SCxRDR]	= { 0x05,  8 },
		[SCFCR]		= { 0x06,  8 },
		[SCFDR]		= { 0x07, 16 },
		[SCTFDR]	= sci_reg_invalid,
		[SCRFDR]	= sci_reg_invalid,
		[SCSPTR]	= sci_reg_invalid,
		[SCLSR]		= sci_reg_invalid,
	},

	/*
                             
  */
	[SCIx_SCIFA_REGTYPE] = {
		[SCSMR]		= { 0x00, 16 },
		[SCBRR]		= { 0x04,  8 },
		[SCSCR]		= { 0x08, 16 },
		[SCxTDR]	= { 0x20,  8 },
		[SCxSR]		= { 0x14, 16 },
		[SCxRDR]	= { 0x24,  8 },
		[SCFCR]		= { 0x18, 16 },
		[SCFDR]		= { 0x1c, 16 },
		[SCTFDR]	= sci_reg_invalid,
		[SCRFDR]	= sci_reg_invalid,
		[SCSPTR]	= sci_reg_invalid,
		[SCLSR]		= sci_reg_invalid,
	},

	/*
                             
  */
	[SCIx_SCIFB_REGTYPE] = {
		[SCSMR]		= { 0x00, 16 },
		[SCBRR]		= { 0x04,  8 },
		[SCSCR]		= { 0x08, 16 },
		[SCxTDR]	= { 0x40,  8 },
		[SCxSR]		= { 0x14, 16 },
		[SCxRDR]	= { 0x60,  8 },
		[SCFCR]		= { 0x18, 16 },
		[SCFDR]		= { 0x1c, 16 },
		[SCTFDR]	= sci_reg_invalid,
		[SCRFDR]	= sci_reg_invalid,
		[SCSPTR]	= sci_reg_invalid,
		[SCLSR]		= sci_reg_invalid,
	},

	/*
                                                            
                    
  */
	[SCIx_SH2_SCIF_FIFODATA_REGTYPE] = {
		[SCSMR]		= { 0x00, 16 },
		[SCBRR]		= { 0x04,  8 },
		[SCSCR]		= { 0x08, 16 },
		[SCxTDR]	= { 0x0c,  8 },
		[SCxSR]		= { 0x10, 16 },
		[SCxRDR]	= { 0x14,  8 },
		[SCFCR]		= { 0x18, 16 },
		[SCFDR]		= { 0x1c, 16 },
		[SCTFDR]	= sci_reg_invalid,
		[SCRFDR]	= sci_reg_invalid,
		[SCSPTR]	= { 0x20, 16 },
		[SCLSR]		= { 0x24, 16 },
	},

	/*
                                 
  */
	[SCIx_SH3_SCIF_REGTYPE] = {
		[SCSMR]		= { 0x00,  8 },
		[SCBRR]		= { 0x02,  8 },
		[SCSCR]		= { 0x04,  8 },
		[SCxTDR]	= { 0x06,  8 },
		[SCxSR]		= { 0x08, 16 },
		[SCxRDR]	= { 0x0a,  8 },
		[SCFCR]		= { 0x0c,  8 },
		[SCFDR]		= { 0x0e, 16 },
		[SCTFDR]	= sci_reg_invalid,
		[SCRFDR]	= sci_reg_invalid,
		[SCSPTR]	= sci_reg_invalid,
		[SCLSR]		= sci_reg_invalid,
	},

	/*
                                       
  */
	[SCIx_SH4_SCIF_REGTYPE] = {
		[SCSMR]		= { 0x00, 16 },
		[SCBRR]		= { 0x04,  8 },
		[SCSCR]		= { 0x08, 16 },
		[SCxTDR]	= { 0x0c,  8 },
		[SCxSR]		= { 0x10, 16 },
		[SCxRDR]	= { 0x14,  8 },
		[SCFCR]		= { 0x18, 16 },
		[SCFDR]		= { 0x1c, 16 },
		[SCTFDR]	= sci_reg_invalid,
		[SCRFDR]	= sci_reg_invalid,
		[SCSPTR]	= { 0x20, 16 },
		[SCLSR]		= { 0x24, 16 },
	},

	/*
                                                                  
             
  */
	[SCIx_SH4_SCIF_NO_SCSPTR_REGTYPE] = {
		[SCSMR]		= { 0x00, 16 },
		[SCBRR]		= { 0x04,  8 },
		[SCSCR]		= { 0x08, 16 },
		[SCxTDR]	= { 0x0c,  8 },
		[SCxSR]		= { 0x10, 16 },
		[SCxRDR]	= { 0x14,  8 },
		[SCFCR]		= { 0x18, 16 },
		[SCFDR]		= { 0x1c, 16 },
		[SCTFDR]	= sci_reg_invalid,
		[SCRFDR]	= sci_reg_invalid,
		[SCSPTR]	= sci_reg_invalid,
		[SCLSR]		= { 0x24, 16 },
	},

	/*
                                                               
                    
  */
	[SCIx_SH4_SCIF_FIFODATA_REGTYPE] = {
		[SCSMR]		= { 0x00, 16 },
		[SCBRR]		= { 0x04,  8 },
		[SCSCR]		= { 0x08, 16 },
		[SCxTDR]	= { 0x0c,  8 },
		[SCxSR]		= { 0x10, 16 },
		[SCxRDR]	= { 0x14,  8 },
		[SCFCR]		= { 0x18, 16 },
		[SCFDR]		= { 0x1c, 16 },
		[SCTFDR]	= { 0x1c, 16 },	/*                  */
		[SCRFDR]	= { 0x20, 16 },
		[SCSPTR]	= { 0x24, 16 },
		[SCLSR]		= { 0x28, 16 },
	},

	/*
                                                             
              
  */
	[SCIx_SH7705_SCIF_REGTYPE] = {
		[SCSMR]		= { 0x00, 16 },
		[SCBRR]		= { 0x04,  8 },
		[SCSCR]		= { 0x08, 16 },
		[SCxTDR]	= { 0x20,  8 },
		[SCxSR]		= { 0x14, 16 },
		[SCxRDR]	= { 0x24,  8 },
		[SCFCR]		= { 0x18, 16 },
		[SCFDR]		= { 0x1c, 16 },
		[SCTFDR]	= sci_reg_invalid,
		[SCRFDR]	= sci_reg_invalid,
		[SCSPTR]	= sci_reg_invalid,
		[SCLSR]		= sci_reg_invalid,
	},
};

#define sci_getreg(up, offset)		(sci_regmap[to_sci_port(up)->cfg->regtype] + offset)

/*
                                                                       
                                                                  
                                                                   
                                   
 */
static unsigned int sci_serial_in(struct uart_port *p, int offset)
{
	struct plat_sci_reg *reg = sci_getreg(p, offset);

	if (reg->size == 8)
		return ioread8(p->membase + (reg->offset << p->regshift));
	else if (reg->size == 16)
		return ioread16(p->membase + (reg->offset << p->regshift));
	else
		WARN(1, "Invalid register access\n");

	return 0;
}

static void sci_serial_out(struct uart_port *p, int offset, int value)
{
	struct plat_sci_reg *reg = sci_getreg(p, offset);

	if (reg->size == 8)
		iowrite8(value, p->membase + (reg->offset << p->regshift));
	else if (reg->size == 16)
		iowrite16(value, p->membase + (reg->offset << p->regshift));
	else
		WARN(1, "Invalid register access\n");
}

static int sci_probe_regmap(struct plat_sci_port *cfg)
{
	switch (cfg->type) {
	case PORT_SCI:
		cfg->regtype = SCIx_SCI_REGTYPE;
		break;
	case PORT_IRDA:
		cfg->regtype = SCIx_IRDA_REGTYPE;
		break;
	case PORT_SCIFA:
		cfg->regtype = SCIx_SCIFA_REGTYPE;
		break;
	case PORT_SCIFB:
		cfg->regtype = SCIx_SCIFB_REGTYPE;
		break;
	case PORT_SCIF:
		/*
                                                          
                                                       
                                                     
                                              
   */
		cfg->regtype = SCIx_SH4_SCIF_REGTYPE;
		break;
	default:
		printk(KERN_ERR "Can't probe register map for given port\n");
		return -EINVAL;
	}

	return 0;
}

static void sci_port_enable(struct sci_port *sci_port)
{
	if (!sci_port->port.dev)
		return;

	pm_runtime_get_sync(sci_port->port.dev);

	clk_enable(sci_port->iclk);
	sci_port->port.uartclk = clk_get_rate(sci_port->iclk);
	clk_enable(sci_port->fclk);
}

static void sci_port_disable(struct sci_port *sci_port)
{
	if (!sci_port->port.dev)
		return;

	clk_disable(sci_port->fclk);
	clk_disable(sci_port->iclk);

	pm_runtime_put_sync(sci_port->port.dev);
}

#if defined(CONFIG_CONSOLE_POLL) || defined(CONFIG_SERIAL_SH_SCI_CONSOLE)

#ifdef CONFIG_CONSOLE_POLL
static int sci_poll_get_char(struct uart_port *port)
{
	unsigned short status;
	int c;

	do {
		status = serial_port_in(port, SCxSR);
		if (status & SCxSR_ERRORS(port)) {
			serial_port_out(port, SCxSR, SCxSR_ERROR_CLEAR(port));
			continue;
		}
		break;
	} while (1);

	if (!(status & SCxSR_RDxF(port)))
		return NO_POLL_CHAR;

	c = serial_port_in(port, SCxRDR);

	/*            */
	serial_port_in(port, SCxSR);
	serial_port_out(port, SCxSR, SCxSR_RDxF_CLEAR(port));

	return c;
}
#endif

static void sci_poll_put_char(struct uart_port *port, unsigned char c)
{
	unsigned short status;

	do {
		status = serial_port_in(port, SCxSR);
	} while (!(status & SCxSR_TDxE(port)));

	serial_port_out(port, SCxTDR, c);
	serial_port_out(port, SCxSR, SCxSR_TDxE_CLEAR(port) & ~SCxSR_TEND(port));
}
#endif /*                                                     */

static void sci_init_pins(struct uart_port *port, unsigned int cflag)
{
	struct sci_port *s = to_sci_port(port);
	struct plat_sci_reg *reg = sci_regmap[s->cfg->regtype] + SCSPTR;

	/*
                                          
  */
	if (s->cfg->ops && s->cfg->ops->init_pins) {
		s->cfg->ops->init_pins(port, cflag);
		return;
	}

	/*
                                                                
                     
  */
	if (!reg->size)
		return;

	if ((s->cfg->capabilities & SCIx_HAVE_RTSCTS) &&
	    ((!(cflag & CRTSCTS)))) {
		unsigned short status;

		status = serial_port_in(port, SCSPTR);
		status &= ~SCSPTR_CTSIO;
		status |= SCSPTR_RTSIO;
		serial_port_out(port, SCSPTR, status); /*             */
	}
}

static int sci_txfill(struct uart_port *port)
{
	struct plat_sci_reg *reg;

	reg = sci_getreg(port, SCTFDR);
	if (reg->size)
		return serial_port_in(port, SCTFDR) & 0xff;

	reg = sci_getreg(port, SCFDR);
	if (reg->size)
		return serial_port_in(port, SCFDR) >> 8;

	return !(serial_port_in(port, SCxSR) & SCI_TDRE);
}

static int sci_txroom(struct uart_port *port)
{
	return port->fifosize - sci_txfill(port);
}

static int sci_rxfill(struct uart_port *port)
{
	struct plat_sci_reg *reg;

	reg = sci_getreg(port, SCRFDR);
	if (reg->size)
		return serial_port_in(port, SCRFDR) & 0xff;

	reg = sci_getreg(port, SCFDR);
	if (reg->size)
		return serial_port_in(port, SCFDR) & ((port->fifosize << 1) - 1);

	return (serial_port_in(port, SCxSR) & SCxSR_RDxF(port)) != 0;
}

/*
                                                                
 */
static inline int sci_rxd_in(struct uart_port *port)
{
	struct sci_port *s = to_sci_port(port);

	if (s->cfg->port_reg <= 0)
		return 1;

	return !!__raw_readb(s->cfg->port_reg);
}

/*                                                                         
                                                                           
                                                                          */

static void sci_transmit_chars(struct uart_port *port)
{
	struct circ_buf *xmit = &port->state->xmit;
	unsigned int stopped = uart_tx_stopped(port);
	unsigned short status;
	unsigned short ctrl;
	int count;

	status = serial_port_in(port, SCxSR);
	if (!(status & SCxSR_TDxE(port))) {
		ctrl = serial_port_in(port, SCSCR);
		if (uart_circ_empty(xmit))
			ctrl &= ~SCSCR_TIE;
		else
			ctrl |= SCSCR_TIE;
		serial_port_out(port, SCSCR, ctrl);
		return;
	}

	count = sci_txroom(port);

	do {
		unsigned char c;

		if (port->x_char) {
			c = port->x_char;
			port->x_char = 0;
		} else if (!uart_circ_empty(xmit) && !stopped) {
			c = xmit->buf[xmit->tail];
			xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		} else {
			break;
		}

		serial_port_out(port, SCxTDR, c);

		port->icount.tx++;
	} while (--count > 0);

	serial_port_out(port, SCxSR, SCxSR_TDxE_CLEAR(port));

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);
	if (uart_circ_empty(xmit)) {
		sci_stop_tx(port);
	} else {
		ctrl = serial_port_in(port, SCSCR);

		if (port->type != PORT_SCI) {
			serial_port_in(port, SCxSR); /*            */
			serial_port_out(port, SCxSR, SCxSR_TDxE_CLEAR(port));
		}

		ctrl |= SCSCR_TIE;
		serial_port_out(port, SCSCR, ctrl);
	}
}

/*                                                          */
#define STEPFN(c)  ({int __c = (c); (((__c-1)|(__c)) == -1); })

static void sci_receive_chars(struct uart_port *port)
{
	struct sci_port *sci_port = to_sci_port(port);
	struct tty_struct *tty = port->state->port.tty;
	int i, count, copied = 0;
	unsigned short status;
	unsigned char flag;

	status = serial_port_in(port, SCxSR);
	if (!(status & SCxSR_RDxF(port)))
		return;

	while (1) {
		/*                                                            */
		count = tty_buffer_request_room(tty, sci_rxfill(port));

		/*                                                        */
		if (count == 0)
			break;

		if (port->type == PORT_SCI) {
			char c = serial_port_in(port, SCxRDR);
			if (uart_handle_sysrq_char(port, c) ||
			    sci_port->break_flag)
				count = 0;
			else
				tty_insert_flip_char(tty, c, TTY_NORMAL);
		} else {
			for (i = 0; i < count; i++) {
				char c = serial_port_in(port, SCxRDR);

				status = serial_port_in(port, SCxSR);
#if defined(CONFIG_CPU_SH3)
				/*                           */
				if (sci_port->break_flag) {
					if ((c == 0) &&
					    (status & SCxSR_FER(port))) {
						count--; i--;
						continue;
					}

					/*                         */
					dev_dbg(port->dev, "debounce<%02x>\n", c);
					sci_port->break_flag = 0;

					if (STEPFN(c)) {
						count--; i--;
						continue;
					}
				}
#endif /*                */
				if (uart_handle_sysrq_char(port, c)) {
					count--; i--;
					continue;
				}

				/*                       */
				if (status & SCxSR_FER(port)) {
					flag = TTY_FRAME;
					port->icount.frame++;
					dev_notice(port->dev, "frame error\n");
				} else if (status & SCxSR_PER(port)) {
					flag = TTY_PARITY;
					port->icount.parity++;
					dev_notice(port->dev, "parity error\n");
				} else
					flag = TTY_NORMAL;

				tty_insert_flip_char(tty, c, flag);
			}
		}

		serial_port_in(port, SCxSR); /*            */
		serial_port_out(port, SCxSR, SCxSR_RDxF_CLEAR(port));

		copied += count;
		port->icount.rx += count;
	}

	if (copied) {
		/*                                                       */
		tty_flip_buffer_push(tty);
	} else {
		serial_port_in(port, SCxSR); /*            */
		serial_port_out(port, SCxSR, SCxSR_RDxF_CLEAR(port));
	}
}

#define SCI_BREAK_JIFFIES (HZ/20)

/*
                                                 
                                                                  
                                       
                                         
                                                            
                         
 */
static inline void sci_schedule_break_timer(struct sci_port *port)
{
	mod_timer(&port->break_timer, jiffies + SCI_BREAK_JIFFIES);
}

/*                                                          */
static void sci_break_timer(unsigned long data)
{
	struct sci_port *port = (struct sci_port *)data;

	sci_port_enable(port);

	if (sci_rxd_in(&port->port) == 0) {
		port->break_flag = 1;
		sci_schedule_break_timer(port);
	} else if (port->break_flag == 1) {
		/*                */
		port->break_flag = 2;
		sci_schedule_break_timer(port);
	} else
		port->break_flag = 0;

	sci_port_disable(port);
}

static int sci_handle_errors(struct uart_port *port)
{
	int copied = 0;
	unsigned short status = serial_port_in(port, SCxSR);
	struct tty_struct *tty = port->state->port.tty;
	struct sci_port *s = to_sci_port(port);

	/*
                                  
  */
	if (s->cfg->overrun_bit != SCIx_NOT_SUPPORTED) {
		if (status & (1 << s->cfg->overrun_bit)) {
			port->icount.overrun++;

			/*               */
			if (tty_insert_flip_char(tty, 0, TTY_OVERRUN))
				copied++;

			dev_notice(port->dev, "overrun error");
		}
	}

	if (status & SCxSR_FER(port)) {
		if (sci_rxd_in(port) == 0) {
			/*                 */
			struct sci_port *sci_port = to_sci_port(port);

			if (!sci_port->break_flag) {
				port->icount.brk++;

				sci_port->break_flag = 1;
				sci_schedule_break_timer(sci_port);

				/*                    */
				if (uart_handle_break(port))
					return 0;

				dev_dbg(port->dev, "BREAK detected\n");

				if (tty_insert_flip_char(tty, 0, TTY_BREAK))
					copied++;
			}

		} else {
			/*             */
			port->icount.frame++;

			if (tty_insert_flip_char(tty, 0, TTY_FRAME))
				copied++;

			dev_notice(port->dev, "frame error\n");
		}
	}

	if (status & SCxSR_PER(port)) {
		/*              */
		port->icount.parity++;

		if (tty_insert_flip_char(tty, 0, TTY_PARITY))
			copied++;

		dev_notice(port->dev, "parity error");
	}

	if (copied)
		tty_flip_buffer_push(tty);

	return copied;
}

static int sci_handle_fifo_overrun(struct uart_port *port)
{
	struct tty_struct *tty = port->state->port.tty;
	struct sci_port *s = to_sci_port(port);
	struct plat_sci_reg *reg;
	int copied = 0;

	reg = sci_getreg(port, SCLSR);
	if (!reg->size)
		return 0;

	if ((serial_port_in(port, SCLSR) & (1 << s->cfg->overrun_bit))) {
		serial_port_out(port, SCLSR, 0);

		port->icount.overrun++;

		tty_insert_flip_char(tty, 0, TTY_OVERRUN);
		tty_flip_buffer_push(tty);

		dev_notice(port->dev, "overrun error\n");
		copied++;
	}

	return copied;
}

static int sci_handle_breaks(struct uart_port *port)
{
	int copied = 0;
	unsigned short status = serial_port_in(port, SCxSR);
	struct tty_struct *tty = port->state->port.tty;
	struct sci_port *s = to_sci_port(port);

	if (uart_handle_break(port))
		return 0;

	if (!s->break_flag && status & SCxSR_BRK(port)) {
#if defined(CONFIG_CPU_SH3)
		/*                */
		s->break_flag = 1;
#endif

		port->icount.brk++;

		/*                 */
		if (tty_insert_flip_char(tty, 0, TTY_BREAK))
			copied++;

		dev_dbg(port->dev, "BREAK detected\n");
	}

	if (copied)
		tty_flip_buffer_push(tty);

	copied += sci_handle_fifo_overrun(port);

	return copied;
}

static irqreturn_t sci_rx_interrupt(int irq, void *ptr)
{
#ifdef CONFIG_SERIAL_SH_SCI_DMA
	struct uart_port *port = ptr;
	struct sci_port *s = to_sci_port(port);

	if (s->chan_rx) {
		u16 scr = serial_port_in(port, SCSCR);
		u16 ssr = serial_port_in(port, SCxSR);

		/*                              */
		if (port->type == PORT_SCIFA || port->type == PORT_SCIFB) {
			disable_irq_nosync(irq);
			scr |= 0x4000;
		} else {
			scr &= ~SCSCR_RIE;
		}
		serial_port_out(port, SCSCR, scr);
		/*                         */
		serial_port_out(port, SCxSR, ssr & ~(1 | SCxSR_RDxF(port)));
		dev_dbg(port->dev, "Rx IRQ %lu: setup t-out in %u jiffies\n",
			jiffies, s->rx_timeout);
		mod_timer(&s->rx_timer, jiffies + s->rx_timeout);

		return IRQ_HANDLED;
	}
#endif

	/*                                                        
                                                                  
                   
  */
	sci_receive_chars(ptr);

	return IRQ_HANDLED;
}

static irqreturn_t sci_tx_interrupt(int irq, void *ptr)
{
	struct uart_port *port = ptr;
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);
	sci_transmit_chars(port);
	spin_unlock_irqrestore(&port->lock, flags);

	return IRQ_HANDLED;
}

static irqreturn_t sci_er_interrupt(int irq, void *ptr)
{
	struct uart_port *port = ptr;

	/*               */
	if (port->type == PORT_SCI) {
		if (sci_handle_errors(port)) {
			/*                                */
			serial_port_in(port, SCxSR);
			serial_port_out(port, SCxSR, SCxSR_RDxF_CLEAR(port));
		}
	} else {
		sci_handle_fifo_overrun(port);
		sci_rx_interrupt(irq, ptr);
	}

	serial_port_out(port, SCxSR, SCxSR_ERROR_CLEAR(port));

	/*                       */
	sci_tx_interrupt(irq, ptr);

	return IRQ_HANDLED;
}

static irqreturn_t sci_br_interrupt(int irq, void *ptr)
{
	struct uart_port *port = ptr;

	/*               */
	sci_handle_breaks(port);
	serial_port_out(port, SCxSR, SCxSR_BREAK_CLEAR(port));

	return IRQ_HANDLED;
}

static inline unsigned long port_rx_irq_mask(struct uart_port *port)
{
	/*
                                                                
                                                                  
                                                                
                                                                
                   
  */
	return SCSCR_RIE | (to_sci_port(port)->cfg->scscr & SCSCR_REIE);
}

static irqreturn_t sci_mpxed_interrupt(int irq, void *ptr)
{
	unsigned short ssr_status, scr_status, err_enabled;
	struct uart_port *port = ptr;
	struct sci_port *s = to_sci_port(port);
	irqreturn_t ret = IRQ_NONE;

	ssr_status = serial_port_in(port, SCxSR);
	scr_status = serial_port_in(port, SCSCR);
	err_enabled = scr_status & port_rx_irq_mask(port);

	/*              */
	if ((ssr_status & SCxSR_TDxE(port)) && (scr_status & SCSCR_TIE) &&
	    !s->chan_tx)
		ret = sci_tx_interrupt(irq, ptr);

	/*
                                                                     
            
  */
	if (((ssr_status & SCxSR_RDxF(port)) || s->chan_rx) &&
	    (scr_status & SCSCR_RIE))
		ret = sci_rx_interrupt(irq, ptr);

	/*                 */
	if ((ssr_status & SCxSR_ERRORS(port)) && err_enabled)
		ret = sci_er_interrupt(irq, ptr);

	/*                 */
	if ((ssr_status & SCxSR_BRK(port)) && err_enabled)
		ret = sci_br_interrupt(irq, ptr);

	return ret;
}

/*
                                                                        
                                                      
 */
static int sci_notifier(struct notifier_block *self,
			unsigned long phase, void *p)
{
	struct sci_port *sci_port;
	unsigned long flags;

	sci_port = container_of(self, struct sci_port, freq_transition);

	if ((phase == CPUFREQ_POSTCHANGE) ||
	    (phase == CPUFREQ_RESUMECHANGE)) {
		struct uart_port *port = &sci_port->port;

		spin_lock_irqsave(&port->lock, flags);
		port->uartclk = clk_get_rate(sci_port->iclk);
		spin_unlock_irqrestore(&port->lock, flags);
	}

	return NOTIFY_OK;
}

static struct sci_irq_desc {
	const char	*desc;
	irq_handler_t	handler;
} sci_irq_desc[] = {
	/*
                                         
  */
	[SCIx_ERI_IRQ] = {
		.desc = "rx err",
		.handler = sci_er_interrupt,
	},

	[SCIx_RXI_IRQ] = {
		.desc = "rx full",
		.handler = sci_rx_interrupt,
	},

	[SCIx_TXI_IRQ] = {
		.desc = "tx empty",
		.handler = sci_tx_interrupt,
	},

	[SCIx_BRI_IRQ] = {
		.desc = "break",
		.handler = sci_br_interrupt,
	},

	/*
                          
  */
	[SCIx_MUX_IRQ] = {
		.desc = "mux",
		.handler = sci_mpxed_interrupt,
	},
};

static int sci_request_irq(struct sci_port *port)
{
	struct uart_port *up = &port->port;
	int i, j, ret = 0;

	for (i = j = 0; i < SCIx_NR_IRQS; i++, j++) {
		struct sci_irq_desc *desc;
		unsigned int irq;

		if (SCIx_IRQ_IS_MUXED(port)) {
			i = SCIx_MUX_IRQ;
			irq = up->irq;
		} else
			irq = port->cfg->irqs[i];

		desc = sci_irq_desc + i;
		port->irqstr[j] = kasprintf(GFP_KERNEL, "%s:%s",
					    dev_name(up->dev), desc->desc);
		if (!port->irqstr[j]) {
			dev_err(up->dev, "Failed to allocate %s IRQ string\n",
				desc->desc);
			goto out_nomem;
		}

		ret = request_irq(irq, desc->handler, up->irqflags,
				  port->irqstr[j], port);
		if (unlikely(ret)) {
			dev_err(up->dev, "Can't allocate %s IRQ\n", desc->desc);
			goto out_noirq;
		}
	}

	return 0;

out_noirq:
	while (--i >= 0)
		free_irq(port->cfg->irqs[i], port);

out_nomem:
	while (--j >= 0)
		kfree(port->irqstr[j]);

	return ret;
}

static void sci_free_irq(struct sci_port *port)
{
	int i;

	/*
                                                               
              
  */
	for (i = 0; i < SCIx_NR_IRQS; i++) {
		free_irq(port->cfg->irqs[i], port);
		kfree(port->irqstr[i]);

		if (SCIx_IRQ_IS_MUXED(port)) {
			/*                                      */
			return;
		}
	}
}

static const char *sci_gpio_names[SCIx_NR_FNS] = {
	"sck", "rxd", "txd", "cts", "rts",
};

static const char *sci_gpio_str(unsigned int index)
{
	return sci_gpio_names[index];
}

static void __devinit sci_init_gpios(struct sci_port *port)
{
	struct uart_port *up = &port->port;
	int i;

	if (!port->cfg)
		return;

	for (i = 0; i < SCIx_NR_FNS; i++) {
		const char *desc;
		int ret;

		if (!port->cfg->gpios[i])
			continue;

		desc = sci_gpio_str(i);

		port->gpiostr[i] = kasprintf(GFP_KERNEL, "%s:%s",
					     dev_name(up->dev), desc);

		/*
                                                          
                           
   */
		if (!port->gpiostr[i])
			dev_notice(up->dev, "%s string allocation failure\n",
				   desc);

		ret = gpio_request(port->cfg->gpios[i], port->gpiostr[i]);
		if (unlikely(ret != 0)) {
			dev_notice(up->dev, "failed %s gpio request\n", desc);

			/*
                                                   
                                                    
    */
			kfree(port->gpiostr[i]);
		}
	}
}

static void sci_free_gpios(struct sci_port *port)
{
	int i;

	for (i = 0; i < SCIx_NR_FNS; i++)
		if (port->cfg->gpios[i]) {
			gpio_free(port->cfg->gpios[i]);
			kfree(port->gpiostr[i]);
		}
}

static unsigned int sci_tx_empty(struct uart_port *port)
{
	unsigned short status = serial_port_in(port, SCxSR);
	unsigned short in_tx_fifo = sci_txfill(port);

	return (status & SCxSR_TEND(port)) && !in_tx_fifo ? TIOCSER_TEMT : 0;
}

/*
                                                                    
                                                                       
                                                                     
                                                                        
                                                                 
                                         
  
                                                                      
                                                                        
                                                                     
 */
static void sci_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	if (mctrl & TIOCM_LOOP) {
		struct plat_sci_reg *reg;

		/*
                                            
   */
		reg = sci_getreg(port, SCFCR);
		if (reg->size)
			serial_port_out(port, SCFCR, serial_port_in(port, SCFCR) | 1);
	}
}

static unsigned int sci_get_mctrl(struct uart_port *port)
{
	/*
                                                                
                                                               
  */
	return TIOCM_DSR | TIOCM_CAR;
}

#ifdef CONFIG_SERIAL_SH_SCI_DMA
static void sci_dma_tx_complete(void *arg)
{
	struct sci_port *s = arg;
	struct uart_port *port = &s->port;
	struct circ_buf *xmit = &port->state->xmit;
	unsigned long flags;

	dev_dbg(port->dev, "%s(%d)\n", __func__, port->line);

	spin_lock_irqsave(&port->lock, flags);

	xmit->tail += sg_dma_len(&s->sg_tx);
	xmit->tail &= UART_XMIT_SIZE - 1;

	port->icount.tx += sg_dma_len(&s->sg_tx);

	async_tx_ack(s->desc_tx);
	s->desc_tx = NULL;

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);

	if (!uart_circ_empty(xmit)) {
		s->cookie_tx = 0;
		schedule_work(&s->work_tx);
	} else {
		s->cookie_tx = -EINVAL;
		if (port->type == PORT_SCIFA || port->type == PORT_SCIFB) {
			u16 ctrl = serial_port_in(port, SCSCR);
			serial_port_out(port, SCSCR, ctrl & ~SCSCR_TIE);
		}
	}

	spin_unlock_irqrestore(&port->lock, flags);
}

/*                                     */
static int sci_dma_rx_push(struct sci_port *s, struct tty_struct *tty,
			   size_t count)
{
	struct uart_port *port = &s->port;
	int i, active, room;

	room = tty_buffer_request_room(tty, count);

	if (s->active_rx == s->cookie_rx[0]) {
		active = 0;
	} else if (s->active_rx == s->cookie_rx[1]) {
		active = 1;
	} else {
		dev_err(port->dev, "cookie %d not found!\n", s->active_rx);
		return 0;
	}

	if (room < count)
		dev_warn(port->dev, "Rx overrun: dropping %u bytes\n",
			 count - room);
	if (!room)
		return room;

	for (i = 0; i < room; i++)
		tty_insert_flip_char(tty, ((u8 *)sg_virt(&s->sg_rx[active]))[i],
				     TTY_NORMAL);

	port->icount.rx += room;

	return room;
}

static void sci_dma_rx_complete(void *arg)
{
	struct sci_port *s = arg;
	struct uart_port *port = &s->port;
	struct tty_struct *tty = port->state->port.tty;
	unsigned long flags;
	int count;

	dev_dbg(port->dev, "%s(%d) active #%d\n", __func__, port->line, s->active_rx);

	spin_lock_irqsave(&port->lock, flags);

	count = sci_dma_rx_push(s, tty, s->buf_len_rx);

	mod_timer(&s->rx_timer, jiffies + s->rx_timeout);

	spin_unlock_irqrestore(&port->lock, flags);

	if (count)
		tty_flip_buffer_push(tty);

	schedule_work(&s->work_rx);
}

static void sci_rx_dma_release(struct sci_port *s, bool enable_pio)
{
	struct dma_chan *chan = s->chan_rx;
	struct uart_port *port = &s->port;

	s->chan_rx = NULL;
	s->cookie_rx[0] = s->cookie_rx[1] = -EINVAL;
	dma_release_channel(chan);
	if (sg_dma_address(&s->sg_rx[0]))
		dma_free_coherent(port->dev, s->buf_len_rx * 2,
				  sg_virt(&s->sg_rx[0]), sg_dma_address(&s->sg_rx[0]));
	if (enable_pio)
		sci_start_rx(port);
}

static void sci_tx_dma_release(struct sci_port *s, bool enable_pio)
{
	struct dma_chan *chan = s->chan_tx;
	struct uart_port *port = &s->port;

	s->chan_tx = NULL;
	s->cookie_tx = -EINVAL;
	dma_release_channel(chan);
	if (enable_pio)
		sci_start_tx(port);
}

static void sci_submit_rx(struct sci_port *s)
{
	struct dma_chan *chan = s->chan_rx;
	int i;

	for (i = 0; i < 2; i++) {
		struct scatterlist *sg = &s->sg_rx[i];
		struct dma_async_tx_descriptor *desc;

		desc = dmaengine_prep_slave_sg(chan,
			sg, 1, DMA_DEV_TO_MEM, DMA_PREP_INTERRUPT);

		if (desc) {
			s->desc_rx[i] = desc;
			desc->callback = sci_dma_rx_complete;
			desc->callback_param = s;
			s->cookie_rx[i] = desc->tx_submit(desc);
		}

		if (!desc || s->cookie_rx[i] < 0) {
			if (i) {
				async_tx_ack(s->desc_rx[0]);
				s->cookie_rx[0] = -EINVAL;
			}
			if (desc) {
				async_tx_ack(desc);
				s->cookie_rx[i] = -EINVAL;
			}
			dev_warn(s->port.dev,
				 "failed to re-start DMA, using PIO\n");
			sci_rx_dma_release(s, true);
			return;
		}
		dev_dbg(s->port.dev, "%s(): cookie %d to #%d\n", __func__,
			s->cookie_rx[i], i);
	}

	s->active_rx = s->cookie_rx[0];

	dma_async_issue_pending(chan);
}

static void work_fn_rx(struct work_struct *work)
{
	struct sci_port *s = container_of(work, struct sci_port, work_rx);
	struct uart_port *port = &s->port;
	struct dma_async_tx_descriptor *desc;
	int new;

	if (s->active_rx == s->cookie_rx[0]) {
		new = 0;
	} else if (s->active_rx == s->cookie_rx[1]) {
		new = 1;
	} else {
		dev_err(port->dev, "cookie %d not found!\n", s->active_rx);
		return;
	}
	desc = s->desc_rx[new];

	if (dma_async_is_tx_complete(s->chan_rx, s->active_rx, NULL, NULL) !=
	    DMA_SUCCESS) {
		/*                               */
		struct tty_struct *tty = port->state->port.tty;
		struct dma_chan *chan = s->chan_rx;
		struct sh_desc *sh_desc = container_of(desc, struct sh_desc,
						       async_tx);
		unsigned long flags;
		int count;

		chan->device->device_control(chan, DMA_TERMINATE_ALL, 0);
		dev_dbg(port->dev, "Read %u bytes with cookie %d\n",
			sh_desc->partial, sh_desc->cookie);

		spin_lock_irqsave(&port->lock, flags);
		count = sci_dma_rx_push(s, tty, sh_desc->partial);
		spin_unlock_irqrestore(&port->lock, flags);

		if (count)
			tty_flip_buffer_push(tty);

		sci_submit_rx(s);

		return;
	}

	s->cookie_rx[new] = desc->tx_submit(desc);
	if (s->cookie_rx[new] < 0) {
		dev_warn(port->dev, "Failed submitting Rx DMA descriptor\n");
		sci_rx_dma_release(s, true);
		return;
	}

	s->active_rx = s->cookie_rx[!new];

	dev_dbg(port->dev, "%s: cookie %d #%d, new active #%d\n", __func__,
		s->cookie_rx[new], new, s->active_rx);
}

static void work_fn_tx(struct work_struct *work)
{
	struct sci_port *s = container_of(work, struct sci_port, work_tx);
	struct dma_async_tx_descriptor *desc;
	struct dma_chan *chan = s->chan_tx;
	struct uart_port *port = &s->port;
	struct circ_buf *xmit = &port->state->xmit;
	struct scatterlist *sg = &s->sg_tx;

	/*
                    
                                                                         
                                                                  
                                                                         
                                 
  */
	spin_lock_irq(&port->lock);
	sg->offset = xmit->tail & (UART_XMIT_SIZE - 1);
	sg_dma_address(sg) = (sg_dma_address(sg) & ~(UART_XMIT_SIZE - 1)) +
		sg->offset;
	sg_dma_len(sg) = min((int)CIRC_CNT(xmit->head, xmit->tail, UART_XMIT_SIZE),
		CIRC_CNT_TO_END(xmit->head, xmit->tail, UART_XMIT_SIZE));
	spin_unlock_irq(&port->lock);

	BUG_ON(!sg_dma_len(sg));

	desc = dmaengine_prep_slave_sg(chan,
			sg, s->sg_len_tx, DMA_MEM_TO_DEV,
			DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
	if (!desc) {
		/*               */
		sci_tx_dma_release(s, true);
		return;
	}

	dma_sync_sg_for_device(port->dev, sg, 1, DMA_TO_DEVICE);

	spin_lock_irq(&port->lock);
	s->desc_tx = desc;
	desc->callback = sci_dma_tx_complete;
	desc->callback_param = s;
	spin_unlock_irq(&port->lock);
	s->cookie_tx = desc->tx_submit(desc);
	if (s->cookie_tx < 0) {
		dev_warn(port->dev, "Failed submitting Tx DMA descriptor\n");
		/*               */
		sci_tx_dma_release(s, true);
		return;
	}

	dev_dbg(port->dev, "%s: %p: %d...%d, cookie %d\n", __func__,
		xmit->buf, xmit->tail, xmit->head, s->cookie_tx);

	dma_async_issue_pending(chan);
}
#endif

static void sci_start_tx(struct uart_port *port)
{
	struct sci_port *s = to_sci_port(port);
	unsigned short ctrl;

#ifdef CONFIG_SERIAL_SH_SCI_DMA
	if (port->type == PORT_SCIFA || port->type == PORT_SCIFB) {
		u16 new, scr = serial_port_in(port, SCSCR);
		if (s->chan_tx)
			new = scr | 0x8000;
		else
			new = scr & ~0x8000;
		if (new != scr)
			serial_port_out(port, SCSCR, new);
	}

	if (s->chan_tx && !uart_circ_empty(&s->port.state->xmit) &&
	    s->cookie_tx < 0) {
		s->cookie_tx = 0;
		schedule_work(&s->work_tx);
	}
#endif

	if (!s->chan_tx || port->type == PORT_SCIFA || port->type == PORT_SCIFB) {
		/*                                                  */
		ctrl = serial_port_in(port, SCSCR);
		serial_port_out(port, SCSCR, ctrl | SCSCR_TIE);
	}
}

static void sci_stop_tx(struct uart_port *port)
{
	unsigned short ctrl;

	/*                                                    */
	ctrl = serial_port_in(port, SCSCR);

	if (port->type == PORT_SCIFA || port->type == PORT_SCIFB)
		ctrl &= ~0x8000;

	ctrl &= ~SCSCR_TIE;

	serial_port_out(port, SCSCR, ctrl);
}

static void sci_start_rx(struct uart_port *port)
{
	unsigned short ctrl;

	ctrl = serial_port_in(port, SCSCR) | port_rx_irq_mask(port);

	if (port->type == PORT_SCIFA || port->type == PORT_SCIFB)
		ctrl &= ~0x4000;

	serial_port_out(port, SCSCR, ctrl);
}

static void sci_stop_rx(struct uart_port *port)
{
	unsigned short ctrl;

	ctrl = serial_port_in(port, SCSCR);

	if (port->type == PORT_SCIFA || port->type == PORT_SCIFB)
		ctrl &= ~0x4000;

	ctrl &= ~port_rx_irq_mask(port);

	serial_port_out(port, SCSCR, ctrl);
}

static void sci_enable_ms(struct uart_port *port)
{
	/*
                                            
  */
}

static void sci_break_ctl(struct uart_port *port, int break_state)
{
	/*
                                                             
                                                             
  */
}

#ifdef CONFIG_SERIAL_SH_SCI_DMA
static bool filter(struct dma_chan *chan, void *slave)
{
	struct sh_dmae_slave *param = slave;

	dev_dbg(chan->device->dev, "%s: slave ID %d\n", __func__,
		param->slave_id);

	chan->private = param;
	return true;
}

static void rx_timer_fn(unsigned long arg)
{
	struct sci_port *s = (struct sci_port *)arg;
	struct uart_port *port = &s->port;
	u16 scr = serial_port_in(port, SCSCR);

	if (port->type == PORT_SCIFA || port->type == PORT_SCIFB) {
		scr &= ~0x4000;
		enable_irq(s->cfg->irqs[1]);
	}
	serial_port_out(port, SCSCR, scr | SCSCR_RIE);
	dev_dbg(port->dev, "DMA Rx timed out\n");
	schedule_work(&s->work_rx);
}

static void sci_request_dma(struct uart_port *port)
{
	struct sci_port *s = to_sci_port(port);
	struct sh_dmae_slave *param;
	struct dma_chan *chan;
	dma_cap_mask_t mask;
	int nent;

	dev_dbg(port->dev, "%s: port %d\n", __func__,
		port->line);

	if (s->cfg->dma_slave_tx <= 0 || s->cfg->dma_slave_rx <= 0)
		return;

	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);

	param = &s->param_tx;

	/*                                      */
	param->slave_id = s->cfg->dma_slave_tx;

	s->cookie_tx = -EINVAL;
	chan = dma_request_channel(mask, filter, param);
	dev_dbg(port->dev, "%s: TX: got channel %p\n", __func__, chan);
	if (chan) {
		s->chan_tx = chan;
		sg_init_table(&s->sg_tx, 1);
		/*                                             */
		BUG_ON((int)port->state->xmit.buf & ~PAGE_MASK);
		sg_set_page(&s->sg_tx, virt_to_page(port->state->xmit.buf),
			    UART_XMIT_SIZE, (int)port->state->xmit.buf & ~PAGE_MASK);
		nent = dma_map_sg(port->dev, &s->sg_tx, 1, DMA_TO_DEVICE);
		if (!nent)
			sci_tx_dma_release(s, false);
		else
			dev_dbg(port->dev, "%s: mapped %d@%p to %x\n", __func__,
				sg_dma_len(&s->sg_tx),
				port->state->xmit.buf, sg_dma_address(&s->sg_tx));

		s->sg_len_tx = nent;

		INIT_WORK(&s->work_tx, work_fn_tx);
	}

	param = &s->param_rx;

	/*                                      */
	param->slave_id = s->cfg->dma_slave_rx;

	chan = dma_request_channel(mask, filter, param);
	dev_dbg(port->dev, "%s: RX: got channel %p\n", __func__, chan);
	if (chan) {
		dma_addr_t dma[2];
		void *buf[2];
		int i;

		s->chan_rx = chan;

		s->buf_len_rx = 2 * max(16, (int)port->fifosize);
		buf[0] = dma_alloc_coherent(port->dev, s->buf_len_rx * 2,
					    &dma[0], GFP_KERNEL);

		if (!buf[0]) {
			dev_warn(port->dev,
				 "failed to allocate dma buffer, using PIO\n");
			sci_rx_dma_release(s, true);
			return;
		}

		buf[1] = buf[0] + s->buf_len_rx;
		dma[1] = dma[0] + s->buf_len_rx;

		for (i = 0; i < 2; i++) {
			struct scatterlist *sg = &s->sg_rx[i];

			sg_init_table(sg, 1);
			sg_set_page(sg, virt_to_page(buf[i]), s->buf_len_rx,
				    (int)buf[i] & ~PAGE_MASK);
			sg_dma_address(sg) = dma[i];
		}

		INIT_WORK(&s->work_rx, work_fn_rx);
		setup_timer(&s->rx_timer, rx_timer_fn, (unsigned long)s);

		sci_submit_rx(s);
	}
}

static void sci_free_dma(struct uart_port *port)
{
	struct sci_port *s = to_sci_port(port);

	if (s->chan_tx)
		sci_tx_dma_release(s, false);
	if (s->chan_rx)
		sci_rx_dma_release(s, false);
}
#else
static inline void sci_request_dma(struct uart_port *port)
{
}

static inline void sci_free_dma(struct uart_port *port)
{
}
#endif

static int sci_startup(struct uart_port *port)
{
	struct sci_port *s = to_sci_port(port);
	int ret;

	dev_dbg(port->dev, "%s(%d)\n", __func__, port->line);

	pm_runtime_put_noidle(port->dev);

	sci_port_enable(s);

	ret = sci_request_irq(s);
	if (unlikely(ret < 0))
		return ret;

	sci_request_dma(port);

	sci_start_tx(port);
	sci_start_rx(port);

	return 0;
}

static void sci_shutdown(struct uart_port *port)
{
	struct sci_port *s = to_sci_port(port);

	dev_dbg(port->dev, "%s(%d)\n", __func__, port->line);

	sci_stop_rx(port);
	sci_stop_tx(port);

	sci_free_dma(port);
	sci_free_irq(s);

	sci_port_disable(s);

	pm_runtime_get_noresume(port->dev);
}

static unsigned int sci_scbrr_calc(unsigned int algo_id, unsigned int bps,
				   unsigned long freq)
{
	switch (algo_id) {
	case SCBRR_ALGO_1:
		return ((freq + 16 * bps) / (16 * bps) - 1);
	case SCBRR_ALGO_2:
		return ((freq + 16 * bps) / (32 * bps) - 1);
	case SCBRR_ALGO_3:
		return (((freq * 2) + 16 * bps) / (16 * bps) - 1);
	case SCBRR_ALGO_4:
		return (((freq * 2) + 16 * bps) / (32 * bps) - 1);
	case SCBRR_ALGO_5:
		return (((freq * 1000 / 32) / bps) - 1);
	}

	/*                              */
	WARN_ON(1);

	return ((freq + 16 * bps) / (32 * bps) - 1);
}

static void sci_reset(struct uart_port *port)
{
	struct plat_sci_reg *reg;
	unsigned int status;

	do {
		status = serial_port_in(port, SCxSR);
	} while (!(status & SCxSR_TEND(port)));

	serial_port_out(port, SCSCR, 0x00);	/*                    */

	reg = sci_getreg(port, SCFCR);
	if (reg->size)
		serial_port_out(port, SCFCR, SCFCR_RFRST | SCFCR_TFRST);
}

static void sci_set_termios(struct uart_port *port, struct ktermios *termios,
			    struct ktermios *old)
{
	struct sci_port *s = to_sci_port(port);
	struct plat_sci_reg *reg;
	unsigned int baud, smr_val, max_baud;
	int t = -1;

	/*
                                                                   
                                                                   
                                                                    
                                                                      
                                                                 
                                                          
  */
	max_baud = port->uartclk ? port->uartclk / 16 : 115200;

	baud = uart_get_baud_rate(port, termios, old, 0, max_baud);
	if (likely(baud && port->uartclk))
		t = sci_scbrr_calc(s->cfg->scbrr_algo_id, baud, port->uartclk);

	sci_port_enable(s);

	sci_reset(port);

	smr_val = serial_port_in(port, SCSMR) & 3;

	if ((termios->c_cflag & CSIZE) == CS7)
		smr_val |= 0x40;
	if (termios->c_cflag & PARENB)
		smr_val |= 0x20;
	if (termios->c_cflag & PARODD)
		smr_val |= 0x30;
	if (termios->c_cflag & CSTOPB)
		smr_val |= 0x08;

	uart_update_timeout(port, termios->c_cflag, baud);

	serial_port_out(port, SCSMR, smr_val);

	dev_dbg(port->dev, "%s: SMR %x, t %x, SCSCR %x\n", __func__, smr_val, t,
		s->cfg->scscr);

	if (t > 0) {
		if (t >= 256) {
			serial_port_out(port, SCSMR, (serial_port_in(port, SCSMR) & ~3) | 1);
			t >>= 2;
		} else
			serial_port_out(port, SCSMR, serial_port_in(port, SCSMR) & ~3);

		serial_port_out(port, SCBRR, t);
		udelay((1000000+(baud-1)) / baud); /*                       */
	}

	sci_init_pins(port, termios->c_cflag);

	reg = sci_getreg(port, SCFCR);
	if (reg->size) {
		unsigned short ctrl = serial_port_in(port, SCFCR);

		if (s->cfg->capabilities & SCIx_HAVE_RTSCTS) {
			if (termios->c_cflag & CRTSCTS)
				ctrl |= SCFCR_MCE;
			else
				ctrl &= ~SCFCR_MCE;
		}

		/*
                                                       
                                                        
                                                           
   */
		ctrl &= ~(SCFCR_RFRST | SCFCR_TFRST);

		serial_port_out(port, SCFCR, ctrl);
	}

	serial_port_out(port, SCSCR, s->cfg->scscr);

#ifdef CONFIG_SERIAL_SH_SCI_DMA
	/*
                                            
                                                                     
                                                                   
                                                                      
                                                                        
                                                                      
                                                                       
                                        
  */
	if (s->chan_rx) {
		s->rx_timeout = (port->timeout - HZ / 50) * s->buf_len_rx * 3 /
			port->fifosize / 2;
		dev_dbg(port->dev,
			"DMA Rx t-out %ums, tty t-out %u jiffies\n",
			s->rx_timeout * 1000 / HZ, port->timeout);
		if (s->rx_timeout < msecs_to_jiffies(20))
			s->rx_timeout = msecs_to_jiffies(20);
	}
#endif

	if ((termios->c_cflag & CREAD) != 0)
		sci_start_rx(port);

	sci_port_disable(s);
}

static const char *sci_type(struct uart_port *port)
{
	switch (port->type) {
	case PORT_IRDA:
		return "irda";
	case PORT_SCI:
		return "sci";
	case PORT_SCIF:
		return "scif";
	case PORT_SCIFA:
		return "scifa";
	case PORT_SCIFB:
		return "scifb";
	}

	return NULL;
}

static inline unsigned long sci_port_size(struct uart_port *port)
{
	/*
                                                            
                                                                 
                                                                  
                            
  */
	return 64;
}

static int sci_remap_port(struct uart_port *port)
{
	unsigned long size = sci_port_size(port);

	/*
                                                            
  */
	if (port->membase)
		return 0;

	if (port->flags & UPF_IOREMAP) {
		port->membase = ioremap_nocache(port->mapbase, size);
		if (unlikely(!port->membase)) {
			dev_err(port->dev, "can't remap port#%d\n", port->line);
			return -ENXIO;
		}
	} else {
		/*
                                                          
                                                   
              
   */
		port->membase = (void __iomem *)port->mapbase;
	}

	return 0;
}

static void sci_release_port(struct uart_port *port)
{
	if (port->flags & UPF_IOREMAP) {
		iounmap(port->membase);
		port->membase = NULL;
	}

	release_mem_region(port->mapbase, sci_port_size(port));
}

static int sci_request_port(struct uart_port *port)
{
	unsigned long size = sci_port_size(port);
	struct resource *res;
	int ret;

	res = request_mem_region(port->mapbase, size, dev_name(port->dev));
	if (unlikely(res == NULL))
		return -EBUSY;

	ret = sci_remap_port(port);
	if (unlikely(ret != 0)) {
		release_resource(res);
		return ret;
	}

	return 0;
}

static void sci_config_port(struct uart_port *port, int flags)
{
	if (flags & UART_CONFIG_TYPE) {
		struct sci_port *sport = to_sci_port(port);

		port->type = sport->cfg->type;
		sci_request_port(port);
	}
}

static int sci_verify_port(struct uart_port *port, struct serial_struct *ser)
{
	struct sci_port *s = to_sci_port(port);

	if (ser->irq != s->cfg->irqs[SCIx_TXI_IRQ] || ser->irq > nr_irqs)
		return -EINVAL;
	if (ser->baud_base < 2400)
		/*                                  */
		return -EINVAL;

	return 0;
}

static struct uart_ops sci_uart_ops = {
	.tx_empty	= sci_tx_empty,
	.set_mctrl	= sci_set_mctrl,
	.get_mctrl	= sci_get_mctrl,
	.start_tx	= sci_start_tx,
	.stop_tx	= sci_stop_tx,
	.stop_rx	= sci_stop_rx,
	.enable_ms	= sci_enable_ms,
	.break_ctl	= sci_break_ctl,
	.startup	= sci_startup,
	.shutdown	= sci_shutdown,
	.set_termios	= sci_set_termios,
	.type		= sci_type,
	.release_port	= sci_release_port,
	.request_port	= sci_request_port,
	.config_port	= sci_config_port,
	.verify_port	= sci_verify_port,
#ifdef CONFIG_CONSOLE_POLL
	.poll_get_char	= sci_poll_get_char,
	.poll_put_char	= sci_poll_put_char,
#endif
};

static int __devinit sci_init_single(struct platform_device *dev,
				     struct sci_port *sci_port,
				     unsigned int index,
				     struct plat_sci_port *p)
{
	struct uart_port *port = &sci_port->port;
	int ret;

	sci_port->cfg	= p;

	port->ops	= &sci_uart_ops;
	port->iotype	= UPIO_MEM;
	port->line	= index;

	switch (p->type) {
	case PORT_SCIFB:
		port->fifosize = 256;
		break;
	case PORT_SCIFA:
		port->fifosize = 64;
		break;
	case PORT_SCIF:
		port->fifosize = 16;
		break;
	default:
		port->fifosize = 1;
		break;
	}

	if (p->regtype == SCIx_PROBE_REGTYPE) {
		ret = sci_probe_regmap(p);
		if (unlikely(ret))
			return ret;
	}

	if (dev) {
		sci_port->iclk = clk_get(&dev->dev, "sci_ick");
		if (IS_ERR(sci_port->iclk)) {
			sci_port->iclk = clk_get(&dev->dev, "peripheral_clk");
			if (IS_ERR(sci_port->iclk)) {
				dev_err(&dev->dev, "can't get iclk\n");
				return PTR_ERR(sci_port->iclk);
			}
		}

		/*
                                                          
             
   */
		sci_port->fclk = clk_get(&dev->dev, "sci_fck");
		if (IS_ERR(sci_port->fclk))
			sci_port->fclk = NULL;

		port->dev = &dev->dev;

		sci_init_gpios(sci_port);

		pm_runtime_irq_safe(&dev->dev);
		pm_runtime_get_noresume(&dev->dev);
		pm_runtime_enable(&dev->dev);
	}

	sci_port->break_timer.data = (unsigned long)sci_port;
	sci_port->break_timer.function = sci_break_timer;
	init_timer(&sci_port->break_timer);

	/*
                                                             
  */
	if (!p->error_mask)
		p->error_mask = (p->type == PORT_SCI) ?
			SCI_DEFAULT_ERROR_MASK : SCIF_DEFAULT_ERROR_MASK;

	/*
                                                                 
                                                    
  */
	if (p->overrun_bit != SCIx_NOT_SUPPORTED) {
		if (p->type == PORT_SCI)
			p->overrun_bit = 5;
		else if (p->scbrr_algo_id == SCBRR_ALGO_4)
			p->overrun_bit = 9;
		else
			p->overrun_bit = 0;

		/*
                                                           
               
   */
		p->error_mask |= (1 << p->overrun_bit);
	}

	port->mapbase		= p->mapbase;
	port->type		= p->type;
	port->flags		= p->flags;
	port->regshift		= p->regshift;

	/*
                                                                  
                                                            
                                                     
   
                                                  
  */
	port->irq		= p->irqs[SCIx_RXI_IRQ];
	port->irqflags		= 0;

	port->serial_in		= sci_serial_in;
	port->serial_out	= sci_serial_out;

	if (p->dma_slave_tx > 0 && p->dma_slave_rx > 0)
		dev_dbg(port->dev, "DMA tx %d, rx %d\n",
			p->dma_slave_tx, p->dma_slave_rx);

	return 0;
}

#ifdef CONFIG_SERIAL_SH_SCI_CONSOLE
static void serial_console_putchar(struct uart_port *port, int ch)
{
	sci_poll_put_char(port, ch);
}

/*
                                                          
                                       
 */
static void serial_console_write(struct console *co, const char *s,
				 unsigned count)
{
	struct sci_port *sci_port = &sci_ports[co->index];
	struct uart_port *port = &sci_port->port;
	unsigned short bits;

	sci_port_enable(sci_port);

	uart_console_write(port, s, count, serial_console_putchar);

	/*                                                            */
	bits = SCxSR_TDxE(port) | SCxSR_TEND(port);
	while ((serial_port_in(port, SCxSR) & bits) != bits)
		cpu_relax();

	sci_port_disable(sci_port);
}

static int __devinit serial_console_setup(struct console *co, char *options)
{
	struct sci_port *sci_port;
	struct uart_port *port;
	int baud = 115200;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';
	int ret;

	/*
                                     
  */
	if (co->index < 0 || co->index >= SCI_NPORTS)
		return -ENODEV;

	sci_port = &sci_ports[co->index];
	port = &sci_port->port;

	/*
                                         
  */
	if (!port->ops)
		return -ENODEV;

	ret = sci_remap_port(port);
	if (unlikely(ret != 0))
		return ret;

	sci_port_enable(sci_port);

	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);

	sci_port_disable(sci_port);

	return uart_set_options(port, co, baud, parity, bits, flow);
}

static struct console serial_console = {
	.name		= "ttySC",
	.device		= uart_console_device,
	.write		= serial_console_write,
	.setup		= serial_console_setup,
	.flags		= CON_PRINTBUFFER,
	.index		= -1,
	.data		= &sci_uart_driver,
};

static struct console early_serial_console = {
	.name           = "early_ttySC",
	.write          = serial_console_write,
	.flags          = CON_PRINTBUFFER,
	.index		= -1,
};

static char early_serial_buf[32];

static int __devinit sci_probe_earlyprintk(struct platform_device *pdev)
{
	struct plat_sci_port *cfg = pdev->dev.platform_data;

	if (early_serial_console.data)
		return -EEXIST;

	early_serial_console.index = pdev->id;

	sci_init_single(NULL, &sci_ports[pdev->id], pdev->id, cfg);

	serial_console_setup(&early_serial_console, early_serial_buf);

	if (!strstr(early_serial_buf, "keep"))
		early_serial_console.flags |= CON_BOOT;

	register_console(&early_serial_console);
	return 0;
}

#define uart_console(port)	((port)->cons->index == (port)->line)

static int sci_runtime_suspend(struct device *dev)
{
	struct sci_port *sci_port = dev_get_drvdata(dev);
	struct uart_port *port = &sci_port->port;

	if (uart_console(port)) {
		struct plat_sci_reg *reg;

		sci_port->saved_smr = serial_port_in(port, SCSMR);
		sci_port->saved_brr = serial_port_in(port, SCBRR);

		reg = sci_getreg(port, SCFCR);
		if (reg->size)
			sci_port->saved_fcr = serial_port_in(port, SCFCR);
		else
			sci_port->saved_fcr = 0;
	}
	return 0;
}

static int sci_runtime_resume(struct device *dev)
{
	struct sci_port *sci_port = dev_get_drvdata(dev);
	struct uart_port *port = &sci_port->port;

	if (uart_console(port)) {
		sci_reset(port);
		serial_port_out(port, SCSMR, sci_port->saved_smr);
		serial_port_out(port, SCBRR, sci_port->saved_brr);

		if (sci_port->saved_fcr)
			serial_port_out(port, SCFCR, sci_port->saved_fcr);

		serial_port_out(port, SCSCR, sci_port->cfg->scscr);
	}
	return 0;
}

#define SCI_CONSOLE	(&serial_console)

#else
static inline int __devinit sci_probe_earlyprintk(struct platform_device *pdev)
{
	return -EINVAL;
}

#define SCI_CONSOLE	NULL
#define sci_runtime_suspend	NULL
#define sci_runtime_resume	NULL

#endif /*                              */

static char banner[] __initdata =
	KERN_INFO "SuperH SCI(F) driver initialized\n";

static struct uart_driver sci_uart_driver = {
	.owner		= THIS_MODULE,
	.driver_name	= "sci",
	.dev_name	= "ttySC",
	.major		= SCI_MAJOR,
	.minor		= SCI_MINOR_START,
	.nr		= SCI_NPORTS,
	.cons		= SCI_CONSOLE,
};

static int sci_remove(struct platform_device *dev)
{
	struct sci_port *port = platform_get_drvdata(dev);

	cpufreq_unregister_notifier(&port->freq_transition,
				    CPUFREQ_TRANSITION_NOTIFIER);

	sci_free_gpios(port);

	uart_remove_one_port(&sci_uart_driver, &port->port);

	clk_put(port->iclk);
	clk_put(port->fclk);

	pm_runtime_disable(&dev->dev);
	return 0;
}

static int __devinit sci_probe_single(struct platform_device *dev,
				      unsigned int index,
				      struct plat_sci_port *p,
				      struct sci_port *sciport)
{
	int ret;

	/*              */
	if (unlikely(index >= SCI_NPORTS)) {
		dev_notice(&dev->dev, "Attempting to register port "
			   "%d when only %d are available.\n",
			   index+1, SCI_NPORTS);
		dev_notice(&dev->dev, "Consider bumping "
			   "CONFIG_SERIAL_SH_SCI_NR_UARTS!\n");
		return 0;
	}

	ret = sci_init_single(dev, sciport, index, p);
	if (ret)
		return ret;

	return uart_add_one_port(&sci_uart_driver, &sciport->port);
}

static int __devinit sci_probe(struct platform_device *dev)
{
	struct plat_sci_port *p = dev->dev.platform_data;
	struct sci_port *sp = &sci_ports[dev->id];
	int ret;

	/*
                                                                  
                                                                  
                               
  */
	if (is_early_platform_device(dev))
		return sci_probe_earlyprintk(dev);

	platform_set_drvdata(dev, sp);

	ret = sci_probe_single(dev, dev->id, p, sp);
	if (ret)
		goto err_unreg;

	sp->freq_transition.notifier_call = sci_notifier;

	ret = cpufreq_register_notifier(&sp->freq_transition,
					CPUFREQ_TRANSITION_NOTIFIER);
	if (unlikely(ret < 0))
		goto err_unreg;

#ifdef CONFIG_SH_STANDARD_BIOS
	sh_bios_gdb_detach();
#endif

	return 0;

err_unreg:
	sci_remove(dev);
	return ret;
}

static int sci_suspend(struct device *dev)
{
	struct sci_port *sport = dev_get_drvdata(dev);

	if (sport)
		uart_suspend_port(&sci_uart_driver, &sport->port);

	return 0;
}

static int sci_resume(struct device *dev)
{
	struct sci_port *sport = dev_get_drvdata(dev);

	if (sport)
		uart_resume_port(&sci_uart_driver, &sport->port);

	return 0;
}

static const struct dev_pm_ops sci_dev_pm_ops = {
	.runtime_suspend = sci_runtime_suspend,
	.runtime_resume = sci_runtime_resume,
	.suspend	= sci_suspend,
	.resume		= sci_resume,
};

static struct platform_driver sci_driver = {
	.probe		= sci_probe,
	.remove		= sci_remove,
	.driver		= {
		.name	= "sh-sci",
		.owner	= THIS_MODULE,
		.pm	= &sci_dev_pm_ops,
	},
};

static int __init sci_init(void)
{
	int ret;

	printk(banner);

	ret = uart_register_driver(&sci_uart_driver);
	if (likely(ret == 0)) {
		ret = platform_driver_register(&sci_driver);
		if (unlikely(ret))
			uart_unregister_driver(&sci_uart_driver);
	}

	return ret;
}

static void __exit sci_exit(void)
{
	platform_driver_unregister(&sci_driver);
	uart_unregister_driver(&sci_uart_driver);
}

#ifdef CONFIG_SERIAL_SH_SCI_CONSOLE
early_platform_init_buffer("earlyprintk", &sci_driver,
			   early_serial_buf, ARRAY_SIZE(early_serial_buf));
#endif
module_init(sci_init);
module_exit(sci_exit);

MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:sh-sci");
MODULE_AUTHOR("Paul Mundt");
MODULE_DESCRIPTION("SuperH SCI(F) serial driver");