/*!
\copyright  Copyright (c) 2005 - 2021 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    
\file       state_proxy.c
\brief      Implementation of component providing local and remote state.
*/

/* local includes */
#include "state_proxy.h"
#include "state_proxy_private.h"
#include "state_proxy_marshal_defs.h"
#include "state_proxy_client_msgs.h"
/* local include for state/events monitored by state proxy */
#include "state_proxy_phy_state.h"
#include "state_proxy_battery.h"
#include "state_proxy_flags.h"
#include "state_proxy_pairing.h"
#include "state_proxy_connection.h"
#include "state_proxy_link_quality.h"
#include "state_proxy_mic_quality.h"
#include "state_proxy_anc.h"
#include "state_proxy_aanc.h"
#include "state_proxy_leakthrough.h"

/* framework includes */
#include <panic.h>
#include <connection_manager.h>
#include <charger_monitor.h>
#include <battery_region.h>
#include "logical_input_switch.h"
#include <phy_state.h>
#include <peer_signalling.h>
#include <hfp_profile.h>
#include <pairing.h>
#include <mirror_profile.h>

/* library includes */
#include <logging.h>
#include <task_list.h>

/* system includes */
#include <message.h>
#include <panic.h>
#include <stdlib.h>

/* Make the type used for message IDs available in debug tools */
LOGGING_PRESERVE_MESSAGE_ENUM(state_proxy_messages)
LOGGING_PRESERVE_MESSAGE_ENUM(state_proxy_internal_messages)

/*! Instance of the state proxy. */
state_proxy_task_data_t state_proxy;

/*****************************
 * Utils
 * \todo move to another file
 *****************************/
/*! \brief Calculate total memory size of a data set. */
static size_t stateProxy_GetStateProxyDataSize(state_proxy_source source)
{
    UNUSED(source);
    return sizeof(state_proxy_data_t);
}

/*! \brief Collate the latest of all the various states monitored by state proxy. */
static void stateProxy_GetInitialState(void)
{
    DEBUG_LOG("stateProxy_GetInitialState");

    /* call each state proxy sub-module that implements a type of state. */
    stateProxy_GetInitialPhyState();
    stateProxy_GetInitialBatteryState();
    stateProxy_GetInitialFlags();
    stateProxy_GetInitialPairingState();
    stateProxy_GetInitialAncData();
    stateProxy_GetInitialLeakthroughData();
}

/****************************
 * Handlers for local events
 ****************************/
/*! \brief Send our initial state to peer when peer signalling is connected. */
static void stateProxy_HandlePeerSigConnectionInd(const PEER_SIG_CONNECTION_IND_T* ind)
{
    DEBUG_LOG("stateProxy_HandlePeerSigConnectionInd status %u", ind->status);

    switch (ind->status)
    {
        case peerSigStatusConnected:
        {
            /* peer signalling is up, send our initial state to sync with
             * peer */
            StateProxy_SendInitialState();
        }
        break;

        case peerSigStatusDisconnected:
            /* fall-through */
        case peerSigStatusLinkLoss:
        {
            /* reset remote device flgas to initial state*/
            stateProxy_SetRemoteInitialFlags();
            
            /* reset initial state sent, so we'll send it again on reconnect
             * and not try and send any event updates until then */
            stateProxy_GetTaskData()->initial_state_sent = FALSE;
            stateProxy_GetTaskData()->initial_state_received = FALSE;
        }
        break;

        default:
        DEBUG_LOG("stateProxy_HandlePeerSigConnectionInd unhandled status %u", ind->status);
        break;
    }
}

static void stateProxy_FlagChangesInState(state_proxy_data_t * new_state, state_proxy_data_t * previous_state)
{
    if(new_state->flags.in_ear != previous_state->flags.in_ear)
    {
        stateProxy_SendRemotePhyStateChangedInd(PHY_STATE_IN_EAR, phy_state_event_in_ear);
    }
}

/********************************************************
 * Handlers for state proxy internal marshalled messages
 ********************************************************/
/*! \brief Handle marshalled message containing initial state of peer.
*/
static void stateProxy_HandleInitialState(const state_proxy_initial_state_t* initial_state)
{
    state_proxy_task_data_t *proxy = stateProxy_GetTaskData();

    DEBUG_LOG("stateProxy_HandleInitialState");

    state_proxy_data_t previous_state = *proxy->remote_state;

    *proxy->remote_state = initial_state->state;

    /* Send updates of selected properties to the clients */

    stateProxy_FlagChangesInState(proxy->remote_state, &previous_state);
    stateProxy_HandleInitialPeerAncData(proxy->remote_state);
    stateProxy_HandleInitialPeerLeakthroughData(proxy->remote_state);

    if(proxy->remote_state->battery != battery_region_unknown)
    {
        MESSAGE_BATTERY_LEVEL_UPDATE_VOLTAGE_T voltage_msg;
        voltage_msg.voltage_mv = proxy->remote_state->battery_voltage;
        stateProxy_HandleRemoteBatteryLevelVoltage(&voltage_msg);
    }

    /* notify clients that initial state has been received from peer, so
     * state proxy is synchronised with peer state */
    proxy->initial_state_received = TRUE;
    stateProxy_MsgStateProxyEventInitialStateReceived();
}

/*! \brief Handle version message transmitted by state proxy on peer.

    \todo Generate hash of the marshal type descriptors and send in the version
          Compare received hash with ours
          Handle version clash failure

    \param
*/
static void stateProxy_HandleRemoteVersion(const state_proxy_version_t* version)
{
    DEBUG_LOG("stateProxy_HandleRemoteVersion state_proxy_version_t version %u", version->version);
}

/*******************************************
 * Transmit state proxy marshalled messages
 *******************************************/

/***********************************
 * Marshalled Message TX CFM and RX
 **********************************/

/*! \brief Handle confirmation of transmission of a marshalled message. */
static void stateProxy_HandleMarshalledMsgChannelTxCfm(const PEER_SIG_MARSHALLED_MSG_CHANNEL_TX_CFM_T* cfm)
{
    SP_LOG_VERBOSE("stateProxy_HandleMarshalledMsgChannelTxCfm channel %u status %u", cfm->channel, cfm->status);
}


/*! \brief Handle incoming marshalled messages.
*/
static void stateProxy_HandleMarshalledMsgChannelRxInd(PEER_SIG_MARSHALLED_MSG_CHANNEL_RX_IND_T* ind)
{
    switch (ind->type)
    {
        /* State Proxy internal messages */
        case MARSHAL_TYPE_state_proxy_initial_state_t:
            stateProxy_HandleInitialState((const state_proxy_initial_state_t*)ind->msg);
            break;

        case MARSHAL_TYPE_state_proxy_version_t:
            stateProxy_HandleRemoteVersion((const state_proxy_version_t*)ind->msg);
            break;

        /* State Proxy event messages */

            /* flags events */
        case MARSHAL_TYPE_PHY_STATE_CHANGED_IND_T:
            stateProxy_HandleRemotePhyStateChangedInd((const PHY_STATE_CHANGED_IND_T*)ind->msg);
            break;
        case MARSHAL_TYPE_state_proxy_msg_empty_payload_t:
            stateProxy_HandleMsgEmptyPayload((const state_proxy_msg_empty_payload_t*)ind->msg);
            break;

            /* pairing events */
        case MARSHAL_TYPE_PAIRING_ACTIVITY_T:
            stateProxy_HandleRemotePairingHandsetActivity((const PAIRING_ACTIVITY_T*)ind->msg);
            break;

            /* battery events */
        case MARSHAL_TYPE_MESSAGE_BATTERY_LEVEL_UPDATE_VOLTAGE_T:
            stateProxy_HandleRemoteBatteryLevelVoltage((const MESSAGE_BATTERY_LEVEL_UPDATE_VOLTAGE_T*)ind->msg);
            break;
        case MARSHAL_TYPE_MESSAGE_BATTERY_REGION_UPDATE_STATE_T:
            stateProxy_HandleRemoteBatteryLevelState((const MESSAGE_BATTERY_REGION_UPDATE_STATE_T*)ind->msg);
            break;

        case MARSHAL_TYPE_STATE_PROXY_LINK_QUALITY_T:
            stateProxy_HandleRemoteLinkQuality((const STATE_PROXY_LINK_QUALITY_T*)ind->msg);
            break;
        case MARSHAL_TYPE_STATE_PROXY_MIC_QUALITY_T:
            stateProxy_HandleRemoteMicQuality((const STATE_PROXY_MIC_QUALITY_T *)ind->msg);
            break;

        case MARSHAL_TYPE_STATE_PROXY_ANC_DATA_T:
            stateProxy_HandleRemoteAncUpdate((const STATE_PROXY_ANC_DATA_T *)ind->msg);
            break;

        case MARSHAL_TYPE_STATE_PROXY_AANC_DATA_T:
            stateProxy_HandleRemoteAancUpdate((const STATE_PROXY_AANC_DATA_T *)ind->msg);

        case MARSHAL_TYPE_STATE_PROXY_LEAKTHROUGH_DATA_T:
            stateProxy_HandleRemoteLeakthroughUpdate((const LEAKTHROUGH_UPDATE_IND_T *)ind->msg);
            break;

        case MARSHAL_TYPE_STATE_PROXY_AANC_LOGGING_T:
            stateProxy_HandleRemoteAancLoggingUpdate((const STATE_PROXY_AANC_LOGGING_T *)ind->msg);
            break;

        case MARSHAL_TYPE_STATE_PROXY_AANC_FB_GAIN_LOGGING_T:
            stateProxy_HandleRemoteAancFBGainLoggingUpdate((const STATE_PROXY_AANC_FB_GAIN_LOGGING_T *)ind->msg);
            break;


            /* connection events TBD */
        default:
            break;
    }
    /* message is an unmarshalled piece of dynamic memory */
    free(ind->msg);
}

/*! State Proxy Message Handler. */
static void stateProxy_HandleMessage(Task task, MessageId id, Message message)
{
    UNUSED(task);

    switch (id)
    {
            /* marshalled messaging */
        case PEER_SIG_MARSHALLED_MSG_CHANNEL_RX_IND:
            stateProxy_HandleMarshalledMsgChannelRxInd((PEER_SIG_MARSHALLED_MSG_CHANNEL_RX_IND_T*)message);
            break;
        case PEER_SIG_MARSHALLED_MSG_CHANNEL_TX_CFM:
            stateProxy_HandleMarshalledMsgChannelTxCfm((PEER_SIG_MARSHALLED_MSG_CHANNEL_TX_CFM_T*)message);
            break;

            /* connection change indications */
        case MIRROR_PROFILE_CONNECT_IND:
            stateProxy_HandleMirrorProfileConnectInd((const MIRROR_PROFILE_CONNECT_IND_T *)message);
            break;
        case CON_MANAGER_TP_CONNECT_IND:
            stateProxy_HandleConManagerConnectInd((const CON_MANAGER_TP_CONNECT_IND_T*)message);
            break;

        /* disconnection change indications */
        case MIRROR_PROFILE_DISCONNECT_IND:
            stateProxy_HandleMirrorProfileDisconnectInd((const MIRROR_PROFILE_DISCONNECT_IND_T *)message);
            break;
        case CON_MANAGER_TP_DISCONNECT_IND:
            stateProxy_HandleConManagerDisconnectInd((const CON_MANAGER_TP_DISCONNECT_IND_T*)message);
            break;

            /* phy state change indications */
        case PHY_STATE_CHANGED_IND:
            stateProxy_HandlePhyStateChangedInd((const PHY_STATE_CHANGED_IND_T*)message);
            break;

            /* battery change indications */
        case MESSAGE_BATTERY_REGION_UPDATE:
            stateProxy_HandleBatteryLevelUpdateState((const MESSAGE_BATTERY_REGION_UPDATE_STATE_T*)message);
            break;
        case MESSAGE_BATTERY_LEVEL_UPDATE_VOLTAGE:
            stateProxy_HandleBatteryLevelUpdateVoltage((const MESSAGE_BATTERY_LEVEL_UPDATE_VOLTAGE_T*)message);
            break;

        case MIRROR_PROFILE_ESCO_CONNECT_IND:
            stateProxy_MicQualityKick();
            break;
        case MIRROR_PROFILE_ESCO_DISCONNECT_IND:
            stateProxy_MicQualityKick();
            break;

            /* Peer signalling notifications */
        case PEER_SIG_CONNECTION_IND:
            stateProxy_HandlePeerSigConnectionInd((const PEER_SIG_CONNECTION_IND_T*)message);
            break;

            /* Pairing notifications */
        case PAIRING_ACTIVITY:
            stateProxy_HandlePairingHandsetActivity((const PAIRING_ACTIVITY_T*)message);
            break;

        case STATE_PROXY_INTERNAL_TIMER_LINK_QUALITY:
            stateProxy_HandleIntervalTimerLinkQuality();
            break;

        case STATE_PROXY_INTERNAL_TIMER_MIC_QUALITY:
            stateProxy_HandleIntervalTimerMicQuality();
            break;

            /* ANC update indication */
        case ANC_UPDATE_STATE_DISABLE_IND:
        case ANC_UPDATE_STATE_ENABLE_IND:
        case ANC_UPDATE_MODE_CHANGED_IND:
        case ANC_UPDATE_GAIN_IND:
        case ANC_TOGGLE_WAY_CONFIG_UPDATE_IND:
        case ANC_SCENARIO_CONFIG_UPDATE_IND:
        case ANC_UPDATE_DEMO_MODE_DISABLE_IND:
        case ANC_UPDATE_DEMO_MODE_ENABLE_IND:
        case ANC_UPDATE_AANC_ADAPTIVITY_PAUSED_IND:
        case ANC_UPDATE_AANC_ADAPTIVITY_RESUMED_IND:
        case ANC_WORLD_VOLUME_GAIN_DB_UPDATE_IND:
        case ANC_WORLD_VOLUME_BALANCE_UPDATE_IND:
        case ANC_UPDATE_PREV_CONFIG_IND:
        case ANC_UPDATE_PREV_MODE_IND:
            stateProxy_HandleLocalAncUpdate(id, message);
            break;

            /* AANC FF Gain update indication */
        case AANC_FF_GAIN_UPDATE_IND:
            stateProxy_HandleAancLoggingUpdate(id, message);
            break;

        case AANC_FB_GAIN_UPDATE_IND:
            stateProxy_HandleAancLoggingUpdate(id, message);
        break;

        case AANC_UPDATE_QUIETMODE_IND:
            stateProxy_HandleAancUpdateInd(id,message);
            break;

        /* Leakthrough Update Indication */
        case LEAKTHROUGH_UPDATE_STATE_IND:
        case LEAKTHROUGH_UPDATE_MODE_IND:
             stateProxy_HandleLeakthroughUpdateInd((const LEAKTHROUGH_UPDATE_IND_T*)message);
             break;


        default:
            DEBUG_LOG("stateProxyHandleMessage unhandled message id MESSAGE:state_proxy_internal_messages:0x%x", id);
            break;
    }
}

static state_proxy_data_t *stateProxy_createData(void)
{
    state_proxy_data_t *data = PanicUnlessMalloc(sizeof(state_proxy_data_t));
    memset(data, 0, sizeof(*data));
    data->mic_quality = MIC_QUALITY_UNAVAILABLE;
    return data;
}

/*******************
 * Public Functions
 *******************/

/*! \brief Initialise the State Proxy component.
    \param[in] Initialise component task.
    \return bool
*/
bool StateProxy_Init(Task init_task)
{
    state_proxy_task_data_t *proxy = stateProxy_GetTaskData();

    UNUSED(init_task);

    DEBUG_LOG("StateProxy_Init");

    /* Initialise component task data */
    memset(proxy, 0, sizeof(*proxy));
    proxy->task.handler = stateProxy_HandleMessage;

    /* Initialise local and remote state stores */
    proxy->local_state = stateProxy_createData();
    proxy->remote_state = stateProxy_createData();

    /* Initialise TaskLists */
    proxy->event_tasks = TaskList_WithDataCreate();
    TaskList_InitialiseWithCapacity(stateProxy_GetEvents(), STATE_PROXY_EVENTS_TASK_LIST_INIT_CAPACITY);

    /* start in paused state */
    proxy->paused = TRUE;

    /* Register with peer signalling to use the State Proxy msg channel */
    appPeerSigMarshalledMsgChannelTaskRegister(stateProxy_GetTask(),
                                               PEER_SIG_MSG_CHANNEL_STATE_PROXY,
                                               state_proxy_marshal_type_descriptors,
                                               NUMBER_OF_MARSHAL_OBJECT_TYPES);

    /* get notification of peer signalling availability to send initial state to peer */
    appPeerSigClientRegister(stateProxy_GetTask());

    /* register for notifications this component is interested in */
    ConManagerRegisterTpConnectionsObserver(cm_transport_bredr, stateProxy_GetTask());
    appPhyStateRegisterClient(stateProxy_GetTask());
    stateProxy_RegisterBatteryClient();
    HfpProfile_RegisterStatusClient(stateProxy_GetTask());
    Pairing_ActivityClientRegister(stateProxy_GetTask());
    MirrorProfile_ClientRegister(stateProxy_GetTask());
    AncStateManager_ClientRegister(stateProxy_GetTask());
    AecLeakthrough_ClientRegister(stateProxy_GetTask());

    /* get our initial state */
    stateProxy_GetInitialState();

    return TRUE;
}

/*! \brief Register a task for event(s) updates. */
void StateProxy_EventRegisterClient(Task client_task, state_proxy_event_type event_mask)
{
    state_proxy_task_data_t *proxy = stateProxy_GetTaskData();

    if (!TaskList_IsTaskOnList(proxy->event_tasks, client_task))
    {
        task_list_data_t data = {0};

        /* first time registration for this task, add to list */
        data.u32 = event_mask;
        TaskList_AddTaskWithData(proxy->event_tasks, client_task, &data);
    }
    else
    {
        /* client already on the list, add events */
        task_list_data_t* data = NULL;
        if (TaskList_GetDataForTaskRaw(proxy->event_tasks, client_task, &data))
        {
            data->u32 |= event_mask;
        }
    }
    stateProxy_LinkQualityKick();
    stateProxy_MicQualityKick();
}

/*! \brief Unregister event(s) updates for the specified task. */
void StateProxy_EventUnregisterClient(Task client_task, state_proxy_event_type event_mask)
{
    state_proxy_task_data_t *proxy = stateProxy_GetTaskData();
    task_list_data_t* data = NULL;

    /* clear the events in the event_mask for the client_task
     * if no events are left registered then remove the task from the list. */
    if (TaskList_GetDataForTaskRaw(proxy->event_tasks, client_task, &data))
    {
        data->u32 &= ~event_mask;
        if (!data->u32)
        {
            TaskList_RemoveTask(proxy->event_tasks, client_task);
        }
    }
    stateProxy_LinkQualityKick();
    stateProxy_MicQualityKick();
}

/*! \brief Register for events concerning state proxy itself. */
void StateProxy_StateProxyEventRegisterClient(Task client_task)
{
    TaskList_AddTask(TaskList_GetFlexibleBaseTaskList(stateProxy_GetEvents()), client_task);
}

/*! \brief Send current device state to peer to initialise event baseline, and enable event forwarding. */
/*! \todo should this post internal version of itself before handling? */
/*! \todo consider renaming to StateProxy_StartAndSync() or similar */
void StateProxy_SendInitialState(void)
{
    state_proxy_task_data_t *proxy = stateProxy_GetTaskData();
    bdaddr peer_addr;
    size_t local_data_size = stateProxy_GetStateProxyDataSize(state_proxy_source_local);

    DEBUG_LOG("StateProxy_SendInitialState");

    if (appDeviceGetPeerBdAddr(&peer_addr))
    {
        state_proxy_initial_state_t* msg = PanicUnlessMalloc(local_data_size);
        memset(msg, 0, local_data_size);
        memcpy(&msg->state, proxy->local_state, local_data_size);

        /* Bring up peer sig connection */
        if (!appPeerSigIsConnected())
        {
            DEBUG_LOG("StateProxy_SendInitialState - force connecting peer_signalling");
            appPeerSigConnect(stateProxy_GetTask(), &peer_addr);
        }

        appPeerSigMarshalledMsgChannelTx(stateProxy_GetTask(),
                PEER_SIG_MSG_CHANNEL_STATE_PROXY,
                msg, MARSHAL_TYPE_state_proxy_initial_state_t);
        /* let state_proxy clients know initial state was sent */
        proxy->initial_state_sent = TRUE;
        /* unpause */
        proxy->paused = FALSE;
        stateProxy_MsgStateProxyEventInitialStateSent();
    }
    else
    {
        /* Note: The system doesn't work if it fails to send the initial state to the other earbud */
        DEBUG_LOG("StateProxy_SendInitialState no peer to send to");
    }
}

/*! \brief Inform state proxy of current device Primary/Secondary role. */
void StateProxy_SetRole(bool primary)
{
    state_proxy_task_data_t *proxy = stateProxy_GetTaskData();
    proxy->is_primary = primary;
}

/*! \brief Prevent State Proxy forwarding any events */
void StateProxy_Stop(void)
{
    state_proxy_task_data_t *proxy = stateProxy_GetTaskData();
    DEBUG_LOG("StateProxy_Stop");
    proxy->paused = TRUE;
}

/*! \brief Has initial state been received from peer.
    \return TRUE if initial state received, otherwise FALSE.
*/
bool StateProxy_InitialStateReceived(void)
{
    return stateProxy_GetTaskData()->initial_state_received;
}

/*************************
 * State Access Functions
 *************************/

/*! \brief A table defining each state/flag accessor function.
    \param X The table is expanded using the macro X.  */
#define FOR_EACH_FLAG(X) \
    X(Is, InCase, , in_case) \
    X(Is, OutOfCase, !, in_case) \
    X(Is, InEar, , in_ear) \
    X(Is, OutOfEar, !, in_ear) \
    X(Is, Pairing, , is_pairing) \
    X(Has, HandsetPairing, , has_handset_pairing) \

/*! \brief X-Macro generator, creating a remote flag accessor function */
#define GENERATE_PEER_ACCESSOR(verb, func, negate, flag) \
bool StateProxy_##verb##Peer##func(void) \
{\
    return !!negate stateProxy_GetRemoteFlag(flag); \
}

/*! Use the table and X-Macro to generate a list of peer flag accessor functions */
FOR_EACH_FLAG(GENERATE_PEER_ACCESSOR)

/*! \brief X-Macro generator, creating a remote flag accessor function */
#define GENERATE_SELF_ACCESSOR(verb, func, negate, flag) \
bool StateProxy_##verb##func(void) \
{\
    return !!negate stateProxy_GetLocalFlag(flag); \
}

/*! Use the table and X-Macro to generate a list of 'self' flag accessor functions */
FOR_EACH_FLAG(GENERATE_SELF_ACCESSOR)

void StateProxy_GetLocalAndRemoteBatteryLevels(uint16 *battery_level, uint16 *peer_battery_level)
{
    /*! \todo gets levels not just state */
    *battery_level = appBatteryGetVoltageAverage();
    *peer_battery_level = stateProxy_GetRemoteData()->battery_voltage;
}

void StateProxy_GetLocalAndRemoteBatteryStates(battery_region_state_t *battery_state,
                                               battery_region_state_t *peer_battery_state)
{
    *battery_state = BatteryRegion_GetState();
    *peer_battery_state = stateProxy_GetRemoteData()->battery;
}

bool StateProxy_IsPrimary(void)
{
    return state_proxy.is_primary;
}

bool StateProxy_GetPeerAncState(void)
{
    return stateProxy_GetRemoteData()->flags.anc_state;
}

uint8 StateProxy_GetPeerAncMode(void)
{
    return stateProxy_GetRemoteData()->anc_mode;
}

bool StateProxy_GetPeerLeakthroughState(void)
{
    return stateProxy_GetRemoteData()->flags.leakthrough_state;
}

uint8 StateProxy_GetPeerLeakthroughMode(void)
{
    return stateProxy_GetRemoteData()->leakthrough_mode;
}

uint8 StateProxy_GetPeerLeakthroughGain(void)
{
    return stateProxy_GetRemoteData()->anc_leakthrough_gain;
}

bool StateProxy_GetPeerQuietModeDetectedState(void)
{
    return stateProxy_GetRemoteData()->flags.aanc_quiet_mode_detected;
}

bool StateProxy_GetPeerQuietModeEnabledState(void)
{
    return stateProxy_GetRemoteData()->flags.aanc_quiet_mode_enabled;
}

state_proxy_data_t* stateProxy_GetData(state_proxy_source source)
{
    return (source == state_proxy_source_local) ?
                stateProxy_GetLocalData() :
                stateProxy_GetRemoteData();
}
