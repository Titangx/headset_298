/*!
\copyright  Copyright (c) 2015 - 2021 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    
\file
\brief      Interface to TWS Topology use of peer signalling marshalled message channel.
*/

#ifndef TWS_TOPOLOGY_PEER_SIG_H_
#define TWS_TOPOLOGY_PEER_SIG_H_

#include <peer_signalling.h>

/*! \brief Handle incoming message on the topology peer signalling channel. */
void TwsTopology_HandleMarshalledMsgChannelRxInd(PEER_SIG_MARSHALLED_MSG_CHANNEL_RX_IND_T* ind);

/*! \brief Handle confirmation that message was transmitted on topology peer signalling channel. */
void TwsTopology_HandleMarshalledMsgChannelTxCfm(PEER_SIG_MARSHALLED_MSG_CHANNEL_TX_CFM_T* cfm);

#endif /* TWS_TOPOLOGY_PEER_SIG_H_ */
