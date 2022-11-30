/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \version %%version
    \file 
    \brief The a2dp c type definitions. This file is generated by D:/Work/Qcc/qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1/adk/tools/packages/typegen/typegen.py.
*/

#ifndef _A2DP_TYPEDEF_H__
#define _A2DP_TYPEDEF_H__

#include <csrtypes.h>
#include <marshal_common.h>
#include <audio_sync.h>
#include <audio_sources_audio_interface.h>

#define MARSHAL_TYPE_source_state_t MARSHAL_TYPE_uint8

/*! A2DP task bitfields data structure */
typedef struct 
{
    /*! General connection flags */
    unsigned flags:6;
    /*! Number of supported SEIDs */
    unsigned num_seids:3;
    /*! Number of connection retries */
    unsigned connect_retries:3;
    /*! Flag to indicate if connection was locally initiated */
    unsigned local_initiated:1;
    /*! Reason for disconnect */
    unsigned disconnect_reason:4;
    /*! Flag to indicate if audio-sync for locally-initiated media-start is complete */
    unsigned local_media_start_audio_sync_complete:1;
} a2dpTaskDataBitfields;

/*! AV suspend reason enum */
typedef enum 
{
    /*! Initial state */
    A2DP_STATE_NULL = 0x00,
    /*! No A2DP connection */
    A2DP_STATE_DISCONNECTED = 0x01,
    /*! Locally initiated connection in progress */
    A2DP_STATE_CONNECTING_LOCAL = 0x02,
    /*! Remotely initiated connection is in progress */
    A2DP_STATE_CONNECTING_REMOTE = 0x03,
    /*! Locally and remotely initiated connection is in progress */
    A2DP_STATE_CONNECTING_CROSSOVER = 0x04,
    /*! Signalling channel connected */
    A2DP_STATE_CONNECTED_SIGNALLING = 0x10,
    /*! Locally initiated media channel connection in progress */
    A2DP_STATE_CONNECTING_MEDIA_LOCAL = 0x11,
    /*! Remotely initiated media channel connection in progress */
    A2DP_STATE_CONNECTING_MEDIA_REMOTE_SYNC = 0x12,
    /*! Media channel connected (parent-pseudo state) */
    A2DP_STATE_CONNECTED_MEDIA = 0x30,
    /*! Media channel streaming */
    A2DP_STATE_CONNECTED_MEDIA_STREAMING = 0x31,
    /*! Media channel streaming but muted (suspend failed) */
    A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED = 0x32,
    /* Locally initiated media channel suspend in progress */
    A2DP_STATE_CONNECTED_MEDIA_SUSPENDING_LOCAL = 0x33,
    /*! Media channel suspended */
    A2DP_STATE_CONNECTED_MEDIA_SUSPENDED = 0x34,
    /*! Media channel suspended, reconfiguring the codec */
    A2DP_STATE_CONNECTED_MEDIA_RECONFIGURING = 0x35,
    /*! Locally initiated media channel start in progress, syncing slave */
    A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC = 0x70,
    /*! Remotely initiated media channel start in progress, syncing slave */
    A2DP_STATE_CONNECTED_MEDIA_STARTING_REMOTE_SYNC = 0x71,
    /*! Locally initiated media channel disconnection in progress */
    A2DP_STATE_DISCONNECTING_MEDIA = 0x13,
    /*! Disconnecting signalling and media channels */
    A2DP_STATE_DISCONNECTING = 0x0A
} avA2dpState;

/*! AV suspend reason enum */
typedef enum 
{
    /*! Suspend AV due to local request */
    AV_SUSPEND_REASON_LOCAL = (1 << 0),
    /*! Suspend AV due to remote request */
    AV_SUSPEND_REASON_REMOTE = (1 << 1),
    /*! This can be used to clear or set all suspend reason bits*/
    AV_SUSPEND_REASON_ALL = ((1 << 2) - 1)
} avSuspendReason;

/*! Preferred sample rates to negotiate during codec configuration */
typedef enum 
{
    /*! No preferred sample rate, choose best available */
    A2DP_PREFERRED_RATE_NONE = 0,
    /*! Prefer 44.1 kHz, even if higher rates available */
    A2DP_PREFERRED_RATE_44K1,
    /*! Prefer 48 kHz, even if higher rates available */
    A2DP_PREFERRED_RATE_48K,
    /*! Prefer 96 kHz, if supported by both sides (otherwise fall back to lower rate) */
    A2DP_PREFERRED_RATE_96K,
} avA2dpPreferredSampleRate;

/*! A2DP task data structure */
typedef struct 
{
    /*! A2DP operation lock, used to serialise A2DP operations */
    uint16 lock;
    /*! A2DP sync flags */
    uint16 sync_flags;
    /*! Current state of A2DP state machine */
    avA2dpState state;
    /*! A2DP device identifier from A2DP library */
    uint8 device_id;
    /*! A2DP stream identifier from A2DP library */
    uint8 stream_id;
    /*! Currently active SEID */
    uint8 current_seid;
    /*! Used as sync counter and provides sync ids */
    uint8 sync_counter;
    /*! Last known SEID, unlike current_seid this doesn't get cleared on disconnect */
    uint8 last_known_seid;
    /*! Bitmap of active suspend reasons */
    avSuspendReason suspend_state;
    /*! Routing state of this source */
    source_state_t source_state;
    /*! A2DP bitfields data */
    a2dpTaskDataBitfields bitfields;
    /*! The interface a client populates to sync with this instance */
    audio_sync_t sync_if;
    /*! Sink for A2DP media (streaming) */
    Sink media_sink;
    /*! Bitfield of sample rates supported by both local and remote device, for current SEID */
    uint8 supported_sample_rates;
    /*! Preferred sample rate to negotiate, if codec allows */
    avA2dpPreferredSampleRate preferred_sample_rate;
} a2dpTaskData;

#endif /* _A2DP_TYPEDEF_H__ */
