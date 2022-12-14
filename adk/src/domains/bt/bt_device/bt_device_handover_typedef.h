/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \version %%version
    \file 
    \brief The bt_device_handover c type definitions. This file is generated by D:/Work/Qcc/qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1/adk/tools/packages/typegen/typegen.py.
*/

#ifndef _BT_DEVICE_HANDOVER_TYPEDEF_H__
#define _BT_DEVICE_HANDOVER_TYPEDEF_H__

#include <csrtypes.h>
#include <csrtypes.h>
#include <marshal_common.h>
#include <bt_device_typedef.h>
#include <domain_marshal_types.h>

#define MARSHAL_TYPE_audio_source_t MARSHAL_TYPE_uint8
#define MARSHAL_TYPE_voice_source_t MARSHAL_TYPE_uint8

/*! Device data to marshal during handover */
typedef struct 
{
    /*! Persistent device data */
    bt_device_pdd_t pdd;
    /*! Bitmap of currently connected profiles */
    uint32 connected_profiles;
    /*! Audio source associated with the device */
    audio_source_t audio_source;
    /*! Voice source associated with the device */
    voice_source_t voice_source;
} bt_device_handover_t;

/*! Device data to marshal during handover for an LE address */
typedef struct 
{
    /*! Persistent device data */
    bt_device_pdd_t pdd;
    /*! Random address associated with the device */
    tp_bdaddr random_tp_bdaddr;
} bt_device_handover_le_t;

#endif /* _BT_DEVICE_HANDOVER_TYPEDEF_H__ */

