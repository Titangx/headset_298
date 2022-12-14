/*!
\copyright  Copyright (c) 2020-2022 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\file
\defgroup   voice_assistant peer signalling
\ingroup    Service
\brief      A component responsible for updating the peer device with the active/selected voice assistant

Responsibilities:
- A component responsible for updating the peer device with the active voice assistant
*/

#include <voice_ui_peer_sig.h>
#include "voice_ui_peer_sig_typedef.h"
#include "voice_ui_peer_sig_marshal_typedef.h"
#include "voice_ui_config.h"
#include <voice_ui_container.h>
#include <peer_signalling.h>
#include <device_properties.h>
#include <bdaddr.h>
#include <bt_device.h>
#include <panic.h>
#include <logging.h>
#include <stdlib.h>
#include <voice_ui.h>

/* Transmit a marshalled Voice UI message to the peer */
#define VOICE_UI_PEER_TX(message, type) appPeerSigMarshalledMsgChannelTx((Task)&task, \
                                        PEER_SIG_MSG_CHANNEL_VOICE_UI, \
                                        message, \
                                        MARSHAL_TYPE_##type)



/*! \brief This variable is used for retry mechanism and keep track whether the secondary EB -
 *  has been updated with the active VA */
static bool pending_update = FALSE;
static bool pending_reboot = FALSE;

static void voiceUi_MessageHandler(Task task, MessageId id, Message message);

static const TaskData task = {voiceUi_MessageHandler};

static void voiceUi_HandlePeerSigConnectInd(const PEER_SIG_CONNECTION_IND_T *ind);
static void voiceUi_HandlePeerSigMarshalledMsgChannelRxInd(const PEER_SIG_MARSHALLED_MSG_CHANNEL_RX_IND_T *ind);
static void voiceUi_HandlePeerSigMarshalledMsgChannelTxCfm(const PEER_SIG_MARSHALLED_MSG_CHANNEL_TX_CFM_T *cfm);
static void voiceUi_AssistantPeerSynchComplete(void);

void VoiceUi_PeerSignallingInit(void)
{
    appPeerSigClientRegister((Task)&task);
    appPeerSigMarshalledMsgChannelTaskRegister((Task)&task,
                                               PEER_SIG_MSG_CHANNEL_VOICE_UI,
                                               voice_ui_peer_sig_marshal_type_descriptors,
                                               NUMBER_OF_VOICE_UI_PEER_SIG_MARSHAL_TYPES);
}

void VoiceUi_UpdateSelectedPeerVaProvider(bool reboot)
{
    DEBUG_LOG("VoiceUi_UpdateSelectedPeerVaProvider: reboot=%u", reboot);

    if(!appPeerSigIsConnected())
    {
        pending_update = TRUE;
    }
    else
    {
        voice_ui_selected_va_provider_t *msg = PanicUnlessMalloc(sizeof(voice_ui_selected_va_provider_t));

        /* Retrieve the active VA from the database.*/
        bdaddr bd_addr;
        appDeviceGetMyBdAddr(&bd_addr);
        device_t device = BtDevice_GetDeviceForBdAddr(&bd_addr);

        if (device)
        {
            /* Initialise to a known default value first before reading the latest value from the PS Keys */
            msg->va_provider = VOICE_UI_PROVIDER_DEFAULT;
            msg->reboot = reboot;

            pending_reboot = reboot;

            Device_GetPropertyU8(device, device_property_voice_assistant, &(msg->va_provider));
            VOICE_UI_PEER_TX(msg, voice_ui_selected_va_provider_t);
        }
        else
        {
            free(msg);
        }
    }
}

static void voiceUi_MessageHandler(Task task, MessageId id, Message message)
{
    UNUSED(task);

    switch (id)
    {
        case PEER_SIG_CONNECTION_IND:
            voiceUi_HandlePeerSigConnectInd((PEER_SIG_CONNECTION_IND_T *)message);
            break;
            
        case PEER_SIG_MARSHALLED_MSG_CHANNEL_RX_IND:
            voiceUi_HandlePeerSigMarshalledMsgChannelRxInd((PEER_SIG_MARSHALLED_MSG_CHANNEL_RX_IND_T *)message);
            break;
            
        case PEER_SIG_MARSHALLED_MSG_CHANNEL_TX_CFM:
            voiceUi_HandlePeerSigMarshalledMsgChannelTxCfm((PEER_SIG_MARSHALLED_MSG_CHANNEL_TX_CFM_T *)message);
            break;

        default:
            break;
    }
}

static void voiceUi_HandlePeerSigConnectInd(const PEER_SIG_CONNECTION_IND_T *ind)
{  
    DEBUG_LOG("voiceUi_HandlePeerSigConnectInd: ind->status=enum:peerSigStatus:%d, pending=%d",
              ind->status, pending_update);

    if((ind->status == peerSigStatusConnected) && pending_update)
    {
        VoiceUi_UpdateSelectedPeerVaProvider(pending_reboot);
    }
}

static void voiceUi_HandlePeerSigSelectedVaProvider(voice_ui_selected_va_provider_t *msg)
{
    DEBUG_LOG_DEBUG("voiceUi_HandlePeerSigSelectedVaProvider: provider=%d reboot=%u",
              msg->va_provider, msg->reboot);

    pending_reboot = msg->reboot;
    VoiceUi_DeselectCurrentAssistant();
    VoiceUi_SetSelectedAssistant(msg->va_provider, NO_REBOOT_AFTER_VA_CHANGE);
    if(msg->va_provider != voice_ui_provider_none)
    {
        VoiceUi_SetSelectedVoiceAssistantInterface(msg->va_provider);
    }

    voiceUi_AssistantPeerSynchComplete();
}

static void voiceUi_HandlePeerSigMarshalledMsgChannelRxInd(const PEER_SIG_MARSHALLED_MSG_CHANNEL_RX_IND_T *ind)
{
    DEBUG_LOG("voiceUi_HandlePeerSigMarshalledMsgChannelRxInd, channel=%d type=%d",
              ind->channel, ind->type);

    switch (ind->type)
    {
    case MARSHAL_TYPE_voice_ui_selected_va_provider_t:
        voiceUi_HandlePeerSigSelectedVaProvider((voice_ui_selected_va_provider_t *) ind->msg);
        break;
    }

    /* Free unmarshalled message after use. */
    free(ind->msg);
}

static void voiceUi_HandlePeerSigMarshalledMsgChannelTxCfm(const PEER_SIG_MARSHALLED_MSG_CHANNEL_TX_CFM_T *cfm)
{
    DEBUG_LOG("voiceUi_HandlePeerSigMarshalledMsgChannelTxCfm, channel=%d type=%d status=enum:peerSigStatus:%d",
              cfm->channel, cfm->type, cfm->status);

    if ((cfm->channel == PEER_SIG_MSG_CHANNEL_VOICE_UI) &&
        (cfm->type == MARSHAL_TYPE_voice_ui_selected_va_provider_t) &&
        (cfm->status == peerSigStatusSuccess))
    {
        voiceUi_AssistantPeerSynchComplete();
    }
}


static void voiceUi_AssistantPeerSynchComplete(void)
{
    DEBUG_LOG("voiceUi_AssistantPeerSynchComplete: reboot=%u", pending_reboot);

    if (pending_reboot)
    {
        VoiceUi_RebootLater();
    }

    pending_update = FALSE;
}
