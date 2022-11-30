/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \version %%version
    \file 
    \brief The peer_link_keys c type definitions. This file is generated by D:/Work/Qcc/qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1/adk/tools/packages/typegen/typegen.py.
*/

#ifndef _PEER_LINK_KEYS_TYPEDEF_H__
#define _PEER_LINK_KEYS_TYPEDEF_H__

#include <csrtypes.h>
#include <bdaddr.h>
#include <marshal_common.h>

/*! Link key types */
typedef enum 
{
    /*! BR/EDR link key, generated by key H6/H7 deriviation process. */
    PEER_LINK_KEYS_KEY_TYPE_0 = 0
} peer_link_keys_key_type_t;

typedef struct 
{
    /*! Address of the device to update the key for */
    bdaddr addr;
    /*! Link key type */
    peer_link_keys_key_type_t key_type;
    /*! Link key size in bytes */
    uint8 key_size_bytes;
    /*! Link key data */
    uint8 key[16];
} peer_link_keys_add_key_req_t;

typedef struct 
{
    /*! Address of the device the key was updated for */
    bdaddr addr;
    /*! Status of the key update */
    bool synced;
} peer_link_keys_add_key_cfm_t;

#endif /* _PEER_LINK_KEYS_TYPEDEF_H__ */

