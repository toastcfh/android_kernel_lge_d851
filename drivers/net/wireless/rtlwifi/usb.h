/******************************************************************************
 *
 * Copyright(c) 2009-2012  Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 * Realtek Corporation, No. 2, Innovation Road II, Hsinchu Science Park,
 * Hsinchu 300, Taiwan.
 *
 *****************************************************************************/

#ifndef __RTL_USB_H__
#define __RTL_USB_H__

#include <linux/skbuff.h>

#define RTL_RX_DESC_SIZE		24

#define RTL_USB_DEVICE(vend, prod, cfg) \
	.match_flags = USB_DEVICE_ID_MATCH_DEVICE, \
	.idVendor = (vend), \
	.idProduct = (prod), \
	.driver_info = (kernel_ulong_t)&(cfg)

#define USB_HIGH_SPEED_BULK_SIZE	512
#define USB_FULL_SPEED_BULK_SIZE	64


#define RTL_USB_MAX_TXQ_NUM		4		/*              */
#define RTL_USB_MAX_EP_NUM		6		/*               */
#define RTL_USB_MAX_TX_URBS_NUM		8

enum rtl_txq {
	/*                                                 
                                     
                                       */
	RTL_TXQ_BK,
	RTL_TXQ_BE,
	RTL_TXQ_VI,
	RTL_TXQ_VO,
	/*                                    */
	RTL_TXQ_BCN,
	RTL_TXQ_MGT,
	RTL_TXQ_HI,

	/*              */
	__RTL_TXQ_NUM,
};

struct rtl_ep_map {
	u32 ep_mapping[__RTL_TXQ_NUM];
};

struct _trx_info {
	struct rtl_usb *rtlusb;
	u32 ep_num;
};

static inline void _rtl_install_trx_info(struct rtl_usb *rtlusb,
					 struct sk_buff *skb,
					 u32 ep_num)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	info->rate_driver_data[0] = rtlusb;
	info->rate_driver_data[1] = (void *)(__kernel_size_t)ep_num;
}


/*                           */
enum rtl_usb_state {
	USB_STATE_STOP	= 0,
	USB_STATE_START	= 1,
};

#define IS_USB_STOP(rtlusb_ptr) (USB_STATE_STOP == (rtlusb_ptr)->state)
#define IS_USB_START(rtlusb_ptr) (USB_STATE_START == (rtlusb_ptr)->state)
#define SET_USB_STOP(rtlusb_ptr) \
	do {							\
		(rtlusb_ptr)->state = USB_STATE_STOP;		\
	} while (0)

#define SET_USB_START(rtlusb_ptr)				\
	do { \
		(rtlusb_ptr)->state = USB_STATE_START;		\
	} while (0)

struct rtl_usb {
	struct usb_device *udev;
	struct usb_interface *intf;
	enum rtl_usb_state state;

	/*                              */
	u32 reg_bcn_ctrl_val;
	/*                          */
	u8	disableHWSM;
	/*           */
	enum acm_method acm_method;
	/*                     */
	u32 irq_mask[2];
	bool irq_enabled;

	u16 (*usb_mq_to_hwq)(__le16 fc, u16 mac80211_queue_index);

	/*    */
	u8 out_ep_nums ;
	u8 out_queue_sel;
	struct rtl_ep_map ep_map;

	u32 max_bulk_out_size;
	u32 tx_submitted_urbs;
	struct sk_buff_head tx_skb_queue[RTL_USB_MAX_EP_NUM];

	struct usb_anchor tx_pending[RTL_USB_MAX_EP_NUM];
	struct usb_anchor tx_submitted;

	struct sk_buff *(*usb_tx_aggregate_hdl)(struct ieee80211_hw *,
						struct sk_buff_head *);
	int (*usb_tx_post_hdl)(struct ieee80211_hw *,
			       struct urb *, struct sk_buff *);
	void (*usb_tx_cleanup)(struct ieee80211_hw *, struct sk_buff *);

	/*    */
	u8 in_ep_nums ;
	u32 in_ep;		/*                         */
	u32 rx_max_size;	/*                         */
	u32 rx_urb_num;		/*                                          */
	struct usb_anchor	rx_submitted;
	void (*usb_rx_segregate_hdl)(struct ieee80211_hw *, struct sk_buff *,
				     struct sk_buff_head *);
	void (*usb_rx_hdl)(struct ieee80211_hw *, struct sk_buff *);
};

struct rtl_usb_priv {
	struct rtl_usb dev;
	struct rtl_led_ctl ledctl;
};

#define rtl_usbpriv(hw)	 (((struct rtl_usb_priv *)(rtl_priv(hw))->priv))
#define rtl_usbdev(usbpriv)	(&((usbpriv)->dev))



int __devinit rtl_usb_probe(struct usb_interface *intf,
			    const struct usb_device_id *id);
void rtl_usb_disconnect(struct usb_interface *intf);
int rtl_usb_suspend(struct usb_interface *pusb_intf, pm_message_t message);
int rtl_usb_resume(struct usb_interface *pusb_intf);

#endif
