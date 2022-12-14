/*!
\copyright  Copyright (c) 2019-2021 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\file
\brief      Logical input switch implementation.
*/

#include "logical_input_switch.h"
#include "logical_input_switch_marshal_defs.h"

#include <touch.h>
#include <bt_device.h>
#include <multidevice.h>
#include <peer_signalling.h>
#include <state_proxy.h>
#include <ui.h>
#include <domain_message.h>

#include <bdaddr.h>
#include <input_event_manager.h>
#include <logging.h>
#include <panic.h>
#include <stdlib.h>
#include <vmtypes.h>

/* The full range of logical inputs are split across two different message
 * groups, one of which is used to identify device specific messages. */
#define MIN_LOGICAL_INPUT_MSG DEVICE_SPECIFIC_LOGICAL_INPUT_MESSAGE_BASE
#define MAX_LOGICAL_INPUT_MSG LAST_ID_IN_MSG_GRP(LOGICAL_INPUT)

/* Message ids reserved for passthrough logical inputs */
enum logical_input_switch_passthrough_internal_id
{
    PASSTHROUGH_DEVICE_SPECIFIC_LOGICAL_INPUT_MSG = INTERNAL_MESSAGE_BASE,
    PASSTHROUGH_LOGICAL_INPUT_MSG,

    /*! This must be the final message */
    LOGICAL_INPUT_PASSTHROUGH_MESSAGE_END
};
ASSERT_INTERNAL_MESSAGES_NOT_OVERFLOWED(LOGICAL_INPUT_PASSTHROUGH_MESSAGE_END)

typedef struct
{
    ui_input_t ui_input;
} PASSTHROUGH_LOGICAL_INPUT_MSG_T;

static void logicalInputSwitch_HandleMessage(Task task, MessageId id, Message message);

static TaskData lis_task = { .handler=logicalInputSwitch_HandleMessage };

static bool reroute_logical_inputs_to_peer = FALSE;

static uint16 lowest_valid_logical_input = MIN_LOGICAL_INPUT_MSG;
static uint16 highest_valid_logical_input = MAX_LOGICAL_INPUT_MSG;

static inline bool logicalInputSwitch_IsPassthroughLogicalInput(uint16 logical_input)
{
    return logical_input == PASSTHROUGH_LOGICAL_INPUT_MSG ||
           logical_input == PASSTHROUGH_DEVICE_SPECIFIC_LOGICAL_INPUT_MSG;
}

static inline bool logicalInputSwitch_IsDeviceSpecificLogicalInput(uint16 logical_input)
{
    return ID_IN_MSG_GRP(DEVICE_SPECIFIC_LOGICAL_INPUT, logical_input) ||
           logical_input == PASSTHROUGH_DEVICE_SPECIFIC_LOGICAL_INPUT_MSG;
}

static ui_input_t logicalInputSwitch_PassthroughUiInput(uint16 logical_input, PASSTHROUGH_LOGICAL_INPUT_MSG_T* message)
{
    if (logicalInputSwitch_IsPassthroughLogicalInput(logical_input))
    {
        PanicNull(message);
        return message->ui_input;
    }
    else
    {
        return ui_input_invalid;
    }
}

static bool logicalInputSwitch_IsValidLogicalInput(uint16 logical_input)
{
    bool logical_input_in_valid_range = logical_input >= lowest_valid_logical_input && logical_input <= highest_valid_logical_input;

    return logical_input_in_valid_range || logicalInputSwitch_IsPassthroughLogicalInput(logical_input);
}

static void logicalInputSwitch_PassLogicalInputToLocalUi(uint16 logical_input, ui_input_t ui_input, bool is_from_right_device)
{
    if (logicalInputSwitch_IsPassthroughLogicalInput(logical_input))
    {
        DEBUG_LOG("logicalInputSwitch_PassLogicalInputToLocalUi Injecting UI Input %04X", ui_input);

        Ui_InjectRedirectableUiInput(ui_input, FALSE);
    }
    else
    {
        Ui_InjectLogicalInput(logical_input, is_from_right_device);
    }
}

static void logicalInputSwitch_SendLogicalInputToPeer(uint16 logical_input, ui_input_t ui_input)
{
    logical_input_ind_t* msg = PanicUnlessMalloc(sizeof(logical_input_ind_t));
    msg->logical_input = logical_input;
    msg->passthrough_ui_input = ui_input;

    DEBUG_LOG("logicalInputSwitch_SendLogicalInputToPeer %04X %04X", msg->logical_input, msg->passthrough_ui_input);

    appPeerSigMarshalledMsgChannelTx(LogicalInputSwitch_GetTask(),
                                     PEER_SIG_MSG_CHANNEL_LOGICAL_INPUT_SWITCH,
                                     msg,
                                     MARSHAL_TYPE_logical_input_ind_t);
}

static inline void logicalInputSwitch_HandleMarshalledMsgChannelTxCfm(PEER_SIG_MARSHALLED_MSG_CHANNEL_TX_CFM_T* cfm)
{
    DEBUG_LOG("logicalInputSwitch_HandleMarshalledMsgChannelTxCfm %d", cfm->status);
}

static void logicalInputSwitch_HandleMarshalledMsgChannelRxInd(PEER_SIG_MARSHALLED_MSG_CHANNEL_RX_IND_T* ind)
{
    logical_input_ind_t *msg;
    PanicNull(ind);
    msg = (logical_input_ind_t *)ind->msg;
    PanicNull(msg);

    /* If this is the left device, the Logical Input must have originated from the Right, and vice versa. */
    bool is_from_right_device = Multidevice_IsLeft();

    logicalInputSwitch_PassLogicalInputToLocalUi(msg->logical_input, msg->passthrough_ui_input, is_from_right_device);
    free(msg);
}

static void logicalInputSwitch_HandleMessage(Task task, MessageId id, Message message)
{
    UNUSED(task);

    switch(id)
    {
    case PEER_SIG_MARSHALLED_MSG_CHANNEL_RX_IND:
        logicalInputSwitch_HandleMarshalledMsgChannelRxInd((PEER_SIG_MARSHALLED_MSG_CHANNEL_RX_IND_T*)message);
        break;

    case PEER_SIG_MARSHALLED_MSG_CHANNEL_TX_CFM:
        logicalInputSwitch_HandleMarshalledMsgChannelTxCfm((PEER_SIG_MARSHALLED_MSG_CHANNEL_TX_CFM_T*)message);
        break;

    default:
        if (logicalInputSwitch_IsValidLogicalInput(id))
        {
            ui_input_t passthrough_ui_input = logicalInputSwitch_PassthroughUiInput(id, (PASSTHROUGH_LOGICAL_INPUT_MSG_T*)message);

            if (reroute_logical_inputs_to_peer &&
                !logicalInputSwitch_IsDeviceSpecificLogicalInput(id))
            {
                logicalInputSwitch_SendLogicalInputToPeer(id, passthrough_ui_input);
            }
            else
            {
                /*if we are not a pair or we have not finished initialising yet and we don't
                know if we are left or right -> assume we are the right one*/
                bool is_right_device = TRUE;
                if(Multidevice_IsPair())
                {
                    is_right_device = !Multidevice_IsLeft();
                }
                logicalInputSwitch_PassLogicalInputToLocalUi(id, passthrough_ui_input, is_right_device);
            }
        }
        break;
    }
}

static void logicalInputSwitch_SendPassthroughLogicalInput_helper(ui_input_t ui_input, MessageId passthrough_msg)
{
    MESSAGE_MAKE(msg, PASSTHROUGH_LOGICAL_INPUT_MSG_T);
    msg->ui_input = ui_input;

    MessageSend(LogicalInputSwitch_GetTask(), passthrough_msg, msg);
}

Task LogicalInputSwitch_GetTask(void)
{
    return &lis_task;
}

void LogicalInputSwitch_SetRerouteToPeer(bool reroute_enabled)
{
    reroute_logical_inputs_to_peer = reroute_enabled;
}

void LogicalInputSwitch_SetLogicalInputIdRange(uint16 lowest, uint16 highest)
{
    if ((lowest < MIN_LOGICAL_INPUT_MSG) || (highest > MAX_LOGICAL_INPUT_MSG) || (lowest > highest))
    {
        DEBUG_LOG("LogicalInputSwitch_SetLogicalInputIdRange Invalid range %04X %04X", lowest, highest);
        Panic();
    }

    lowest_valid_logical_input = lowest;
    highest_valid_logical_input = highest;
}

static void logicalInputSwitch_SendPassthroughLogicalInput(ui_input_t ui_input)
{
    logicalInputSwitch_SendPassthroughLogicalInput_helper(ui_input, PASSTHROUGH_LOGICAL_INPUT_MSG);
}

void LogicalInputSwitch_SendPassthroughDeviceSpecificLogicalInput(ui_input_t ui_input)
{
    logicalInputSwitch_SendPassthroughLogicalInput_helper(ui_input, PASSTHROUGH_DEVICE_SPECIFIC_LOGICAL_INPUT_MSG);
}

bool LogicalInputSwitch_Init(Task init_task)
{
    COMPILE_TIME_ASSERT((DEVICE_SPECIFIC_LOGICAL_INPUT_MESSAGE_LIMIT + 1) == LOGICAL_INPUT_MESSAGE_BASE,messages_non_consecutive);

    LogicalInputSwitch_SetRerouteToPeer(FALSE);

    LogicalInputSwitch_SetLogicalInputIdRange(MIN_LOGICAL_INPUT_MSG, MAX_LOGICAL_INPUT_MSG);

    appPeerSigMarshalledMsgChannelTaskRegister(LogicalInputSwitch_GetTask(),
                                               PEER_SIG_MSG_CHANNEL_LOGICAL_INPUT_SWITCH,
                                               logical_input_switch_marshal_type_descriptors,
                                               NUMBER_OF_MARSHAL_OBJECT_TYPES);

    Ui_RegisterUiInputsRedirector(logicalInputSwitch_SendPassthroughLogicalInput);

    UNUSED(init_task);
    return TRUE;
}

static void logicalInputSwitch_RegisterMessageGroup(Task task, message_group_t group)
{
    PanicFalse((group == LOGICAL_INPUT_MESSAGE_GROUP)||
               (group == DEVICE_SPECIFIC_LOGICAL_INPUT_MESSAGE_GROUP));
    InputEventManager_RegisterClient(task);
#ifdef INCLUDE_CAPSENSE
    /* add clients to receive touch event notifications */
    TouchSensorClientRegister(task, 0, NULL);
#endif
}

MESSAGE_BROKER_GROUP_REGISTRATION_MAKE(LOGICAL_INPUT, logicalInputSwitch_RegisterMessageGroup, NULL);
MESSAGE_BROKER_GROUP_REGISTRATION_MAKE(DEVICE_SPECIFIC_LOGICAL_INPUT, logicalInputSwitch_RegisterMessageGroup, NULL);
