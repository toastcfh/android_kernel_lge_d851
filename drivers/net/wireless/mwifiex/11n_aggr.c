/*
 * Marvell Wireless LAN device driver: 802.11n Aggregation
 *
 * Copyright (C) 2011, Marvell International Ltd.
 *
 * This software file (the "File") is distributed by Marvell International
 * Ltd. under the terms of the GNU General Public License Version 2, June 1991
 * (the "License").  You may use, redistribute and/or modify this File in
 * accordance with the terms and conditions of the License, a copy of which
 * is available by writing to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA or on the
 * worldwide web at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
 *
 * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE
 * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about
 * this warranty disclaimer.
 */

#include "decl.h"
#include "ioctl.h"
#include "util.h"
#include "fw.h"
#include "main.h"
#include "wmm.h"
#include "11n.h"
#include "11n_aggr.h"

/*
                                                                   
  
                                           
  
                                                                       
                                                                       
                                                                       
                                                                       
                                                                       
  
                                                                         
                                     
  
                                          
                                          
 */
static int
mwifiex_11n_form_amsdu_pkt(struct sk_buff *skb_aggr,
			   struct sk_buff *skb_src, int *pad)

{
	int dt_offset;
	struct rfc_1042_hdr snap = {
		0xaa,		/*          */
		0xaa,		/*          */
		0x03,		/*          */
		{0x00, 0x00, 0x00},	/*          */
		0x0000		/*           */
			/*
                                    
                          
    */
	};
	struct tx_packet_hdr *tx_header;

	skb_put(skb_aggr, sizeof(*tx_header));

	tx_header = (struct tx_packet_hdr *) skb_aggr->data;

	/*                */
	dt_offset = 2 * ETH_ALEN;
	memcpy(&tx_header->eth803_hdr, skb_src->data, dt_offset);

	/*                  */
	snap.snap_type = *(u16 *) ((u8 *)skb_src->data + dt_offset);
	dt_offset += sizeof(u16);

	memcpy(&tx_header->rfc1042_hdr, &snap, sizeof(struct rfc_1042_hdr));

	skb_pull(skb_src, dt_offset);

	/*                     */
	tx_header->eth803_hdr.h_proto = htons(skb_src->len + LLC_SNAP_LEN);

	/*             */
	skb_put(skb_aggr, skb_src->len);
	memcpy(skb_aggr->data + sizeof(*tx_header), skb_src->data,
	       skb_src->len);
	*pad = (((skb_src->len + LLC_SNAP_LEN) & 3)) ? (4 - (((skb_src->len +
						      LLC_SNAP_LEN)) & 3)) : 0;
	skb_put(skb_aggr, *pad);

	return skb_aggr->len + *pad;
}

/*
                             
  
                                                            
                                        
 */
static void
mwifiex_11n_form_amsdu_txpd(struct mwifiex_private *priv,
			    struct sk_buff *skb)
{
	struct txpd *local_tx_pd;

	skb_push(skb, sizeof(*local_tx_pd));

	local_tx_pd = (struct txpd *) skb->data;
	memset(local_tx_pd, 0, sizeof(struct txpd));

	/*                                        */
	local_tx_pd->priority = (u8) skb->priority;
	local_tx_pd->pkt_delay_2ms =
		mwifiex_wmm_compute_drv_pkt_delay(priv, skb);
	local_tx_pd->bss_num = priv->bss_num;
	local_tx_pd->bss_type = priv->bss_type;
	/*                                                    */
	local_tx_pd->tx_pkt_offset = cpu_to_le16(sizeof(struct txpd));
	local_tx_pd->tx_pkt_type = cpu_to_le16(PKT_TYPE_AMSDU);
	local_tx_pd->tx_pkt_length = cpu_to_le16(skb->len -
						 sizeof(*local_tx_pd));

	if (local_tx_pd->tx_control == 0)
		/*                               */
		local_tx_pd->tx_control = cpu_to_le32(priv->pkt_tx_ctrl);

	if (GET_BSS_ROLE(priv) == MWIFIEX_BSS_ROLE_STA &&
	    priv->adapter->pps_uapsd_mode) {
		if (true == mwifiex_check_last_packet_indication(priv)) {
			priv->adapter->tx_lock_flag = true;
			local_tx_pd->flags =
				MWIFIEX_TxPD_POWER_MGMT_LAST_PACKET;
		}
	}
}

/*
                            
  
                                                                        
                                                                       
                                                                        
                
  
                                                                       
                                                  
  
                                                                     
                                                                     
                                                                     
                                                                     
 */
int
mwifiex_11n_aggregate_pkt(struct mwifiex_private *priv,
			  struct mwifiex_ra_list_tbl *pra_list, int headroom,
			  int ptrindex, unsigned long ra_list_flags)
			  __releases(&priv->wmm.ra_list_spinlock)
{
	struct mwifiex_adapter *adapter = priv->adapter;
	struct sk_buff *skb_aggr, *skb_src;
	struct mwifiex_txinfo *tx_info_aggr, *tx_info_src;
	int pad = 0, ret;
	struct mwifiex_tx_param tx_param;
	struct txpd *ptx_pd = NULL;

	skb_src = skb_peek(&pra_list->skb_head);
	if (!skb_src) {
		spin_unlock_irqrestore(&priv->wmm.ra_list_spinlock,
				       ra_list_flags);
		return 0;
	}

	tx_info_src = MWIFIEX_SKB_TXCB(skb_src);
	skb_aggr = dev_alloc_skb(adapter->tx_buf_size);
	if (!skb_aggr) {
		dev_err(adapter->dev, "%s: alloc skb_aggr\n", __func__);
		spin_unlock_irqrestore(&priv->wmm.ra_list_spinlock,
				       ra_list_flags);
		return -1;
	}
	skb_reserve(skb_aggr, headroom + sizeof(struct txpd));
	tx_info_aggr =  MWIFIEX_SKB_TXCB(skb_aggr);

	tx_info_aggr->bss_type = tx_info_src->bss_type;
	tx_info_aggr->bss_num = tx_info_src->bss_num;
	skb_aggr->priority = skb_src->priority;

	do {
		/*                                          */
		if (skb_tailroom(skb_aggr) < (skb_src->len + LLC_SNAP_LEN))
			break;

		skb_src = skb_dequeue(&pra_list->skb_head);

		pra_list->total_pkts_size -= skb_src->len;

		atomic_dec(&priv->wmm.tx_pkts_queued);

		spin_unlock_irqrestore(&priv->wmm.ra_list_spinlock,
				       ra_list_flags);
		mwifiex_11n_form_amsdu_pkt(skb_aggr, skb_src, &pad);

		mwifiex_write_data_complete(adapter, skb_src, 0);

		spin_lock_irqsave(&priv->wmm.ra_list_spinlock, ra_list_flags);

		if (!mwifiex_is_ralist_valid(priv, pra_list, ptrindex)) {
			spin_unlock_irqrestore(&priv->wmm.ra_list_spinlock,
					       ra_list_flags);
			return -1;
		}

		if (skb_tailroom(skb_aggr) < pad) {
			pad = 0;
			break;
		}
		skb_put(skb_aggr, pad);

		skb_src = skb_peek(&pra_list->skb_head);

	} while (skb_src);

	spin_unlock_irqrestore(&priv->wmm.ra_list_spinlock, ra_list_flags);

	/*                                         */
	skb_trim(skb_aggr, skb_aggr->len - pad);

	/*            */
	mwifiex_11n_form_amsdu_txpd(priv, skb_aggr);
	if (GET_BSS_ROLE(priv) == MWIFIEX_BSS_ROLE_STA)
		ptx_pd = (struct txpd *)skb_aggr->data;

	skb_push(skb_aggr, headroom);

	/*
                                                   
                                                    
                      
   
                                                   
                                                    
                                                      
                                                
                           
  */
	tx_param.next_pkt_len = 0;

	ret = adapter->if_ops.host_to_card(adapter, MWIFIEX_TYPE_DATA,
					   skb_aggr, &tx_param);
	switch (ret) {
	case -EBUSY:
		spin_lock_irqsave(&priv->wmm.ra_list_spinlock, ra_list_flags);
		if (!mwifiex_is_ralist_valid(priv, pra_list, ptrindex)) {
			spin_unlock_irqrestore(&priv->wmm.ra_list_spinlock,
					       ra_list_flags);
			mwifiex_write_data_complete(adapter, skb_aggr, -1);
			return -1;
		}
		if (GET_BSS_ROLE(priv) == MWIFIEX_BSS_ROLE_STA &&
		    adapter->pps_uapsd_mode && adapter->tx_lock_flag) {
				priv->adapter->tx_lock_flag = false;
				if (ptx_pd)
					ptx_pd->flags = 0;
		}

		skb_queue_tail(&pra_list->skb_head, skb_aggr);

		pra_list->total_pkts_size += skb_aggr->len;

		atomic_inc(&priv->wmm.tx_pkts_queued);

		tx_info_aggr->flags |= MWIFIEX_BUF_FLAG_REQUEUED_PKT;
		spin_unlock_irqrestore(&priv->wmm.ra_list_spinlock,
				       ra_list_flags);
		dev_dbg(adapter->dev, "data: -EBUSY is returned\n");
		break;
	case -1:
		adapter->data_sent = false;
		dev_err(adapter->dev, "%s: host_to_card failed: %#x\n",
			__func__, ret);
		adapter->dbg.num_tx_host_to_card_failure++;
		mwifiex_write_data_complete(adapter, skb_aggr, ret);
		return 0;
	case -EINPROGRESS:
		adapter->data_sent = false;
		break;
	case 0:
		mwifiex_write_data_complete(adapter, skb_aggr, ret);
		break;
	default:
		break;
	}
	if (ret != -EBUSY) {
		spin_lock_irqsave(&priv->wmm.ra_list_spinlock, ra_list_flags);
		if (mwifiex_is_ralist_valid(priv, pra_list, ptrindex)) {
			priv->wmm.packets_out[ptrindex]++;
			priv->wmm.tid_tbl_ptr[ptrindex].ra_list_curr = pra_list;
		}
		/*                                              */
		adapter->bss_prio_tbl[priv->bss_priority].bss_prio_cur =
			list_first_entry(
				&adapter->bss_prio_tbl[priv->bss_priority]
				.bss_prio_cur->list,
				struct mwifiex_bss_prio_node, list);
		spin_unlock_irqrestore(&priv->wmm.ra_list_spinlock,
				       ra_list_flags);
	}

	return 0;
}
