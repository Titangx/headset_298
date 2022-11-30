/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \version %%version
    \file 
    \brief The avrcp marshal type declarations. This file is generated by D:/Work/Qcc/qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1/adk/tools/packages/typegen/typegen.py.
*/

#ifndef _AVRCP_MARSHAL_TYPEDEF_H__
#define _AVRCP_MARSHAL_TYPEDEF_H__

#include <csrtypes.h>
#include <marshal_common.h>
#include <avrcp.h>
#include <task_list.h>
#include <app/marshal/marshal_if.h>

extern const marshal_type_descriptor_t marshal_type_descriptor_avrcpTaskDataBitfields;
extern const marshal_type_descriptor_t marshal_type_descriptor_avAvrcpState;
extern const marshal_type_descriptor_t marshal_type_descriptor_avrcpTaskData;

#define AVRCP_MARSHAL_TYPES_TABLE(ENTRY)\
    ENTRY(avrcpTaskDataBitfields)\
    ENTRY(avAvrcpState)\
    ENTRY(avrcpTaskData)

#endif /* _AVRCP_MARSHAL_TYPEDEF_H__ */

