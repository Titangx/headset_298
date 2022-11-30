/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \version %%version
    \file 
    \brief The a2dp marshal type definitions. This file is generated by D:/Work/Qcc/qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1/adk/tools/packages/typegen/typegen.py.
*/

#include <csrtypes.h>
#include <marshal_common.h>
#include <audio_sync.h>
#include <audio_sources_audio_interface.h>
#include <app/marshal/marshal_if.h>
#include <a2dp_typedef.h>
#include <a2dp_marshal_typedef.h>

#include "domain_marshal_types.h"

#ifndef HOSTED_TEST_ENVIRONMENT
COMPILE_TIME_ASSERT(sizeof(source_state_t) == sizeof(uint8), source_state_t_is_not_the_same_size_as_uint8);
#endif

/*! A2DP task data structure */
static const marshal_member_descriptor_t a2dpTaskData_member_descriptors[] =
{
    /*! Current state of A2DP state machine */
    MAKE_MARSHAL_MEMBER(a2dpTaskData, avA2dpState, state),
    /*! Currently active SEID */
    MAKE_MARSHAL_MEMBER(a2dpTaskData, uint8, current_seid),
    /*! Used as sync counter and provides sync ids */
    MAKE_MARSHAL_MEMBER(a2dpTaskData, uint8, sync_counter),
    /*! Last known SEID, unlike current_seid this doesn't get cleared on disconnect */
    MAKE_MARSHAL_MEMBER(a2dpTaskData, uint8, last_known_seid),
    /*! Bitmap of active suspend reasons */
    MAKE_MARSHAL_MEMBER(a2dpTaskData, avSuspendReason, suspend_state),
    /*! Routing state of this source */
    MAKE_MARSHAL_MEMBER(a2dpTaskData, source_state_t, source_state),
    /*! A2DP bitfields data */
    MAKE_MARSHAL_MEMBER(a2dpTaskData, a2dpTaskDataBitfields, bitfields),
} ;

const marshal_type_descriptor_t marshal_type_descriptor_a2dpTaskDataBitfields = MAKE_MARSHAL_TYPE_DEFINITION_BASIC(a2dpTaskDataBitfields);
const marshal_type_descriptor_t marshal_type_descriptor_avA2dpState = MAKE_MARSHAL_TYPE_DEFINITION_BASIC(avA2dpState);
const marshal_type_descriptor_t marshal_type_descriptor_avSuspendReason = MAKE_MARSHAL_TYPE_DEFINITION_BASIC(avSuspendReason);
const marshal_type_descriptor_t marshal_type_descriptor_avA2dpPreferredSampleRate = MAKE_MARSHAL_TYPE_DEFINITION_BASIC(avA2dpPreferredSampleRate);
const marshal_type_descriptor_t marshal_type_descriptor_a2dpTaskData = MAKE_MARSHAL_TYPE_DEFINITION(a2dpTaskData, a2dpTaskData_member_descriptors);


