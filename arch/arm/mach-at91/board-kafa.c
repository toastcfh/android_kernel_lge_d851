/*
 * linux/arch/arm/mach-at91/board-kafa.c
 *
 *  Copyright (C) 2006 Sperry-Sun
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/types.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include <mach/hardware.h>
#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/irq.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/board.h>
#include <mach/cpu.h>

#include "generic.h"


static void __init kafa_init_early(void)
{
	/*                    */
	at91rm9200_set_type(ARCH_REVISON_9200_PQFP);

	/*                                          */
	at91_initialize(18432000);

	/*                 */
	at91_init_leds(AT91_PIN_PB4, AT91_PIN_PB4);

	/*                               */
	at91_register_uart(0, 0, 0);

	/*                                    */
	at91_register_uart(AT91RM9200_ID_US0, 1, ATMEL_UART_CTS | ATMEL_UART_RTS);

	/*                                        */
	at91_set_serial_console(0);
}

static struct macb_platform_data __initdata kafa_eth_data = {
	.phy_irq_pin	= AT91_PIN_PC4,
	.is_rmii	= 0,
};

static struct at91_usbh_data __initdata kafa_usbh_data = {
	.ports		= 1,
	.vbus_pin	= {-EINVAL, -EINVAL},
	.overcurrent_pin= {-EINVAL, -EINVAL},
};

static struct at91_udc_data __initdata kafa_udc_data = {
	.vbus_pin	= AT91_PIN_PB6,
	.pullup_pin	= AT91_PIN_PB7,
};

static void __init kafa_board_init(void)
{
	/*        */
	at91_add_device_serial();
	/*          */
	at91_add_device_eth(&kafa_eth_data);
	/*          */
	at91_add_device_usbh(&kafa_usbh_data);
	/*            */
	at91_add_device_udc(&kafa_udc_data);
	/*     */
	at91_add_device_i2c(NULL, 0);
	/*     */
	at91_add_device_spi(NULL, 0);
}

MACHINE_START(KAFA, "Sperry-Sun KAFA")
	/*                             */
	.timer		= &at91rm9200_timer,
	.map_io		= at91_map_io,
	.init_early	= kafa_init_early,
	.init_irq	= at91_init_irq_default,
	.init_machine	= kafa_board_init,
MACHINE_END
