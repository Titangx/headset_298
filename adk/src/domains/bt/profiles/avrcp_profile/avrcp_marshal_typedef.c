/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \version %%version
    \file 
    \brief The avrcp marshal type definitions. This file is generated by D:/Work/Qcc/qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1/adk/tools/packages/typegen/typegen.py.
*/

#include <csrtypes.h>
#include <marshal_common.h>
#include <avrcp.h>
#include <task_list.h>
#include <app/marshal/marshal_if.h>
#include <avrcp_typedef.h>
#include <avrcp_marshal_typedef.h>

#include "domain_marshal_types.h"

/*! AVRCP task data structure */
static const marshal_member_descriptor_t avrcpTaskData_member_descriptors[] =
{
    /*! Playback status lock, set when AVRCP Play or Pause sent.  Cleared when playback status received, or after timeout */
    MAKE_MARSHAL_MEMBER(avrcpTaskData, uint16, playback_lock),
    /*! AVRCP bitfields data */
    MAKE_MARSHAL_MEMBER(avrcpTaskData, avrcpTaskDataBitfields, bitfields),
    /*! Current state of AVRCP state machine */
    MAKE_MARSHAL_MEMBER(avrcpTaskData, avAvrcpState, state),
    /*! Current play status of the AVRCP connection. This is not always known. See \ref avrcp_play_hint */
    MAKE_MARSHAL_MEMBER(avrcpTaskData, avrcp_play_status, play_status),
    /*! Our local guess at the play status. Not always accurate. */
    MAKE_MARSHAL_MEMBER(avrcpTaskData, avrcp_play_status, play_hint),
} ;

const marshal_type_descriptor_t marshal_type_descriptor_avrcpTaskDataBitfields = MAKE_MARSHAL_TYPE_DEFINITION_BASIC(avrcpTaskDataBitfields);
const marshal_type_descriptor_t marshal_type_descriptor_avAvrcpState = MAKE_MARSHAL_TYPE_DEFINITION_BASIC(avAvrcpState);
const marshal_type_descriptor_t marshal_type_descriptor_avrcpTaskData = MAKE_MARSHAL_TYPE_DEFINITION(avrcpTaskData, avrcpTaskData_member_descriptors);

