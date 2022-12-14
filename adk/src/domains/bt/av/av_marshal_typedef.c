/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \version %%version
    \file 
    \brief The av marshal type definitions. This file is generated by D:/Work/Qcc/qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1/adk/tools/packages/typegen/typegen.py.
*/

#include <csrtypes.h>
#include <marshal_common.h>
#include <task_list.h>
#include <app/marshal/marshal_if.h>
#include <av_typedef.h>
#include <av_marshal_typedef.h>

#include "domain_marshal_types.h"

#ifndef HOSTED_TEST_ENVIRONMENT
COMPILE_TIME_ASSERT(sizeof(avInstanceTaskData) < 120, avInstanceTaskData_exceeded_target_pool_size);
#endif

/*! AV Instance task data structure */
static const marshal_member_descriptor_t avInstanceTaskData_member_descriptors[] =
{
    /*! Delay timer for sending internal AVRCP pause for unrouted source post handover */
    MAKE_MARSHAL_MEMBER(avInstanceTaskData, uint8, send_avrcp_unrouted_pause_post_handover),
    /*! AVRCP task data */
    MAKE_MARSHAL_MEMBER(avInstanceTaskData, avrcpTaskData, avrcp),
    /*! A2DP task data */
    MAKE_MARSHAL_MEMBER(avInstanceTaskData, a2dpTaskData, a2dp),
} ;

const marshal_type_descriptor_t marshal_type_descriptor_avInstanceTaskData = MAKE_MARSHAL_TYPE_DEFINITION(avInstanceTaskData, avInstanceTaskData_member_descriptors);


