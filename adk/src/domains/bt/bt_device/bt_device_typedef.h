/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \version %%version
    \file 
    \brief The bt_device c type definitions. This file is generated by D:/Work/Qcc/qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1/adk/tools/packages/typegen/typegen.py.
*/

#ifndef _BT_DEVICE_TYPEDEF_H__
#define _BT_DEVICE_TYPEDEF_H__

#include <csrtypes.h>
#include <csrtypes.h>
#include <marshal_common.h>

/*! Device attributes store in Persistent Store */
typedef struct 
{
    /*! Last set A2dp volume for the device */
    uint8 a2dp_volume;
    /*! HFP or HSP profile */
    uint8 hfp_profile;
    /*! Type of device enumeration, DEVICE_TYPE_XXX */
    uint8 type;
    /*! Device Link mode */
    uint8 link_mode;
    /*! Bitmap of supported profiles */
    uint8 reserved_1;
    /*! Bitmap of last connected profiles */
    uint8 reserved_2;
    /*! Padding */
    uint16 padding;
    /*! Misc. flags */
    uint16 flags;
    /*! Supported SCO forwarding features */
    uint16 sco_fwd_features;
    /*! Left Battery Server client configuration */
    uint16 battery_server_config_l;
    /*! Right Battery Server client configuration */
    uint16 battery_server_config_r;
    /*! GATT Server client configuration */
    uint16 gatt_server_config;
    /*! GATT Server services changed flag */
    uint8 gatt_server_services_changed;
    /*! Last selected Voice Assistant */
    uint8 voice_assistant;
    /*! Device used by the device test service */
    uint8 dts;
    /*! Bitmap of supported profiles */
    uint32 supported_profiles;
    /*! Bitmap of last connected profiles */
    uint32 last_connected_profiles;
    /*! Last set Hfp volume for the device */
    uint8 hfp_volume;
    /*! Last set Hfp mic gain for the device */
    uint8 hfp_mic_gain;
    /*! Voice assistant device flags */
    uint8 va_flags;
    /*! Voice Assistant locale */
    uint8 va_locale[4];
    /*! Handset Service config */
    uint32 handset_service_config;
    /*! Last set analog audio volume for the device */
    uint8 analog_audio_volume;
    /*! Account Key Index associated with the handset device */
    uint8 handset_account_key_index;
    /*! Connection Switch flag for SASS */
    uint8 sass_switch_disabled;
    /*! Gatt service discovery flag */
    uint8 gsd_completed;
} bt_device_pdd_t;

#endif /* _BT_DEVICE_TYPEDEF_H__ */

