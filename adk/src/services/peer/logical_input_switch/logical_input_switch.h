/*!
\copyright  Copyright (c) 2019-2021 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\file
\defgroup   logical_input_switch Logical Input Switch
\ingroup    peer_service
\brief      The Logical Input Switch belongs to the Peer Service.
            It is used in TWS topologies to re-route Logical Inputs (which are arising
            from user button press events) from one of the peer devices to the other peer,
            so that they can be subitted to the UI module there (i.e. remote rather than
            local end of the peer link).

            By default the Logical Input Switch is configured to pass any Logical Inputs
            straight through, i.e. to the local UI module.

            The Logical Input Switch has the concept of validity. Because only the
            Application knows the number and range of Logical Inputs that the Input Event
            Manager library will produce (because the configuration of these is determined
            by the Application), it needs to inform the Logical Input Switch of the range
            of messages corresponding to valid Logical Inputs. The Loogical Input Switch
            shall not re-route or passthrough any message it receives which is found to be
            outside of this range of validity.

@startuml
            skinparam roundcorner 20
            hide footbox

            participant "Application" as app
            participant "Input Event Manager" as iem
            participant "Logical Input Switch" as lis
            participant "UI" as ui
            participant "Role Selection Service" as rss
            participant "Peer Signalling" as ps

            group Initialisation
            note over app
                This is the same for both peer devices
            end note

            app->lis: LogicalInputSwitch_Init()
            app->lis: LogicalInputSwitch_SetLogicalInputIdRange(min_id, max_id)
            app->iem: InputEventManagerInit(LogicalInputSwitch_GetTask())

            end group

            group Peer Role Selection
            note over rss
                TWS Topology set-up occurs, one peer
                device shall become the Primary. This
                shall recieve all the Logical Inputs
                and generate all the UI Inputs.

                At Primary:
            end note

            rss->lis: LogicalInputSwitch_SetRerouteToPeer(TRUE)

            note over rss
                At Secondary:
            end note

            rss->lis: LogicalInputSwitch_SetRerouteToPeer(FALSE)
            end group

            group Button Press at Secondary

            iem->lis: MessageSend(Logical_Input_MFB_Press)
            lis->ps: PeerSig_MarshalledMsgChannelTx(logical_input_ind)

            note over ps
                The Logical Input is marshalled between
                the two peer devices. It is delivered
                to the Primary and applied to the UI:
            end note

            ps->lis: MessageSend(PEER_SIG_MARSHALLED_MSG_CHANNEL_RX_IND)
            lis->ui: MessageSend(Ui_GetUiTask(),Logical_Input_MFB_Press)

            end group

            group Button Press at Primary

            iem->lis: MessageSend(Logical_Input_MFB_Press)
            lis->ui: MessageSend(Ui_GetUiTask(),Logical_Input_MFB_Press)

            end group

@enduml

*/

#ifndef LOGICAL_INPUT_SWITCH_H
#define LOGICAL_INPUT_SWITCH_H

#include <message.h>
#include <ui.h>

/*\{*/
/*! \brief Initialises the Logical Input Switch component.
    \param init_task Parameter description.
    \return bool value description.
*/
bool LogicalInputSwitch_Init(Task init_task);

/*! \brief Allows the application to set the valid range of Logical Inputs in the system.
    \param lowest the logical input representing the lower bound of the range
    \param highest the logical input representing the higher bound of the range
*/
void LogicalInputSwitch_SetLogicalInputIdRange(uint16 lowest, uint16 highest);

/*! \brief Gets the Task structure associated with the Logical Input Switch component.
    \return The Logical Input Switch Task.
*/
Task LogicalInputSwitch_GetTask(void);

/*! \brief Set to re-route any Logical Inputs presented to the Logical Input Switch
           away from the Local UI and instead to the remote UI on a connected Peer device.

    \param reroute_enabled when TRUE, any logical inputs presented to the Loigical Input
           Switch will be re-routed to a connected Peer device.
*/
void LogicalInputSwitch_SetRerouteToPeer(bool reroute_enabled);

/*! \brief Send a passthrough device specific logical input containing a specific UI input.

           This is similar to sending a passthrough logical input however as it is
           device specific the ui_input will always be injected on the local device
           and will not be re-routed to peer regardless of if re-routing has been
           enabled.

    \param ui_input the UI input to inject into the system.
*/
void LogicalInputSwitch_SendPassthroughDeviceSpecificLogicalInput(ui_input_t ui_input);
/*\}*/

#endif /* LOGICAL_INPUT_SWITCH_H */
