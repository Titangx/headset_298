/*!
\copyright  Copyright (c) 2019-2022 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\file
\brief      Mirror profile channel for sending messages between Primary & Secondary.
*/

#ifdef INCLUDE_MIRRORING

#include <stdlib.h>

#include <bdaddr.h>
#include <sink.h>

#include "audio_sources.h"
#include "kymera_adaptation_audio_protected.h"
#include "volume_messages.h"
#include "kymera.h"
#include "av_instance.h"

#include "mirror_profile_signalling.h"
#include "mirror_profile_typedef.h"
#include "mirror_profile_marshal_typedef.h"
#include "mirror_profile_private.h"
#include "mirror_profile_voice_source.h"
#include "mirror_profile_audio_source.h"

/*! The stream context rate is represented as Hz/25 */
#define STREAM_CONTEXT_RATE_MULTIPLIER 25

#define peerSigTx(message, type) appPeerSigMarshalledMsgChannelTx(\
    MirrorProfile_GetTask(), \
    PEER_SIG_MSG_CHANNEL_MIRROR_PROFILE, \
    (message), MARSHAL_TYPE(type))

#define peerSigCancelTx(type) appPeerSigMarshalledMsgChannelTxCancelAll(\
    MirrorProfile_GetTask(), \
    PEER_SIG_MSG_CHANNEL_MIRROR_PROFILE, \
    MARSHAL_TYPE(type))


/*! Flags in the mirror_profile_stream_context_t message */
#define MIRROR_PROFILE_STREAM_CONTEXT_FLAG_SEND_RESPONSE 0x01

/*
    Functions sending a mirror_profile channel message
*/

void MirrorProfile_SendHfpVolumeToSecondary(voice_source_t source, uint8 volume)
{
    mirror_profile_hfp_volume_ind_t* msg = PanicUnlessMalloc(sizeof(*msg));
    msg->voice_source = source;
    msg->volume = volume;
    peerSigCancelTx(mirror_profile_hfp_volume_ind_t);
    peerSigTx(msg, mirror_profile_hfp_volume_ind_t);
}

void MirrorProfile_SendHfpCodecAndVolumeToSecondary(voice_source_t voice_source, hfp_codec_mode_t codec_mode, uint8 volume)
{
    mirror_profile_hfp_codec_and_volume_ind_t* msg = PanicUnlessMalloc(sizeof(*msg));
    msg->codec_mode = codec_mode;
    msg->volume = volume;
    msg->voice_source = voice_source;
    peerSigTx(msg, mirror_profile_hfp_codec_and_volume_ind_t);
}

void MirrorProfile_SendA2dpVolumeToSecondary(audio_source_t source, uint8 volume)
{
    mirror_profile_a2dp_volume_ind_t* msg = PanicUnlessMalloc(sizeof(*msg));
    msg->audio_source = source;
    msg->volume = volume;
    peerSigCancelTx(mirror_profile_a2dp_volume_ind_t);
    peerSigTx(msg, mirror_profile_a2dp_volume_ind_t);
}

static void mirrorProfile_SendA2dpStreamContextToSecondaryImpl(audio_source_t source, bool request_response)
{
    mirror_profile_cached_context_t* cached_context = MirrorProfile_GetCachedContext(source);

    if (appPeerSigIsConnected())
    {
        mirror_profile_stream_context_t *context = PanicUnlessMalloc(sizeof(*context));
        avInstanceTaskData *av_inst = AvInstance_GetSinkInstanceForAudioSource(source);
        bdaddr* handset_addr = PanicNull(AvInstance_GetBdaddr(av_inst));

        memset(context, 0, sizeof(*context));

        context->addr = *handset_addr;
        PanicFalse(!BdaddrIsZero(&context->addr));
        context->cid = cached_context->cid;
        context->mtu = cached_context->mtu;
        context->seid = cached_context->seid;
        context->sample_rate = (uint16)((cached_context->sample_rate) / STREAM_CONTEXT_RATE_MULTIPLIER);
        context->content_protection_type = cached_context->content_protection ?
                    UINT16_BUILD(AVDTP_CP_TYPE_SCMS_MSB, AVDTP_CP_TYPE_SCMS_LSB) : 0;

        context->volume = mirrorProfile_GetMirroredAudioVolume(source);
        context->q2q_mode = cached_context->q2q_mode;
        context->aptx_features = cached_context->aptx_features;
        context->audio_source = source;

        if (request_response)
        {
            context->flags |= MIRROR_PROFILE_STREAM_CONTEXT_FLAG_SEND_RESPONSE;
        }

        /* If the mirrored instance is already streaming, force the audio state
           sent to active. This ensures the secondary starts in the
           MIRROR_PROFILE_A2DP_START_SECONDARY_SYNC_UNMUTE state meaning primary
           and secondary should start with a synchronised unmute. */
        if (av_inst && appA2dpIsStreaming(av_inst))
        {
            context->audio_state = AUDIO_SYNC_STATE_ACTIVE;
        }
        else
        {
            context->audio_state = cached_context->state;
        }

        peerSigTx(context, mirror_profile_stream_context_t);

        MIRROR_LOG("MirrorProfile_SendA2dpStreamContextToSecondary. enum:audio_source_t:%d %d", source, context->audio_state);
    }
    else
    {
        MirrorProfile_ClearStreamChangeLock();
    }
}

void MirrorProfile_SendA2dpStreamContextToSecondary(audio_source_t source)
{
    mirrorProfile_SendA2dpStreamContextToSecondaryImpl(source, FALSE);
}

void MirrorProfile_SendA2dpStreamContextToSecondaryRequestResponse(audio_source_t source)
{
    mirrorProfile_SendA2dpStreamContextToSecondaryImpl(source, TRUE);
}

void MirrorProfile_SendA2dpStreamContextToSecondaryBlockUntilResponse(audio_source_t source)
{
    MirrorProfile_SetStreamChangeLock();
    MirrorProfile_SendA2dpStreamContextToSecondaryRequestResponse(source);
    /* Kick the sm once the stream context message response is received */
    MessageSendConditionally(MirrorProfile_GetTask(),
                             MIRROR_INTERNAL_KICK_TARGET_STATE, NULL,
                             MirrorProfile_GetStreamChangeLockAddr());
}

void MirrorProfile_SendA2pdUnmuteTimeToPrimary(rtime_t unmute_time)
{
    if (MirrorProfile_IsSecondary())
    {
        mirror_profile_sync_a2dp_unmute_ind_t *ind = PanicUnlessMalloc(sizeof(*ind));
        // Clock domain conversion is done by peer signalling type conversion */
        ind->unmute_time = unmute_time;
        peerSigTx(ind, mirror_profile_sync_a2dp_unmute_ind_t);
    }
}

void MirrorProfile_HandleKymeraScoStarted(void)
{
    if (MirrorProfile_IsSecondary())
    {
        mirror_profile_sync_sco_unmute_ind_t *ind = PanicUnlessMalloc(sizeof(*ind));
        rtime_t unmute_time = rtime_add(VmGetTimerTime(), mirrorProfileConfig_ScoSyncUnmuteDelayUs());
        // Clock domain conversion is done by peer signalling type conversion */
        ind->unmute_time = unmute_time;
        peerSigTx(ind, mirror_profile_sync_sco_unmute_ind_t);
        Kymera_ScheduleScoSyncUnmute(RtimeTimeToMsDelay(unmute_time));
    }
}

static void mirrorProfile_UpdateAudioVolumeFromPeer(audio_source_t audio_source, int new_volume)
{
    /* only if we have valid audio_source from primary, we allow the volume update */
    if(audio_source != audio_source_none)
    {
        volume_t volume = AudioSources_GetVolume(audio_source);
        if (volume.value != new_volume)
        {
            Volume_SendAudioSourceVolumeUpdateRequest(audio_source, event_origin_peer, new_volume);
        }
    }
}

static void mirrorProfile_HandleA2dpStreamContext(const mirror_profile_stream_context_t *context)
{
    mirror_profile_a2dp_t *a2dp_state = MirrorProfile_GetA2dpState();
    mirror_profile_cached_context_t* cached_context = MirrorProfile_GetCachedContext(context->audio_source);

    MIRROR_LOG("mirrorProfile_HandleA2dpStreamContext enum:audio_source_t:%d ind_state:%d q2q|seid:%02x cid 0x%x",
                    context->audio_source, context->audio_state, (context->q2q_mode<<4)|context->seid, context->cid);

    /* Update mirrored audio source only if this is the cid we are currently mirroring */
    if(context->cid == a2dp_state->cid)
    {
        a2dp_state->audio_source = context->audio_source;
    }

    cached_context->cid = context->cid;
    cached_context->mtu = context->mtu;
    cached_context->seid = context->seid;
    cached_context->sample_rate = context->sample_rate * STREAM_CONTEXT_RATE_MULTIPLIER;
    cached_context->content_protection = (context->content_protection_type != 0);
    cached_context->q2q_mode = context->q2q_mode;
    cached_context->aptx_features = context->aptx_features;

    mirrorProfile_SetAudioSyncState(context->audio_source, context->audio_state);
    mirrorProfile_UpdateAudioVolumeFromPeer(context->audio_source, context->volume);

    if (context->flags & MIRROR_PROFILE_STREAM_CONTEXT_FLAG_SEND_RESPONSE)
    {
        mirror_profile_stream_context_response_t *response = PanicUnlessMalloc(sizeof(*response));
        memset(response, 0, sizeof(*response));

        response->cid = context->cid;
        response->seid = context->seid;
        peerSigTx(response, mirror_profile_stream_context_response_t);
    }
}

static void mirrorProfile_HandleA2dpStreamContextResponse(const mirror_profile_stream_context_response_t *response)
{
    UNUSED(response);
    MirrorProfile_ClearStreamChangeLock();
    MIRROR_LOG("mirrorProfile_HandleA2dpStreamContextResponse clearing lock");
}

static void mirrorProfile_HandleA2dpSyncUnmute(const mirror_profile_sync_a2dp_unmute_ind_t *ind)
{
    if (MirrorProfile_IsPrimary())
    {
        appKymeraA2dpSetSyncUnmuteTime(ind->unmute_time);
    }
}

static void mirrorProfile_HandleScoSyncUnmute(const mirror_profile_sync_sco_unmute_ind_t *ind)
{
    if (MirrorProfile_IsPrimary())
    {
        Kymera_ScheduleScoSyncUnmute(RtimeTimeToMsDelay(ind->unmute_time));
    }
}

static void mirrorProfile_HandleHfpCodecAndVolume(const mirror_profile_hfp_codec_and_volume_ind_t *ind)
{
    mirror_profile_esco_t *esco = MirrorProfile_GetScoState();
    MirrorProfile_SetScoCodec(ind->codec_mode);
    esco->voice_source = ind->voice_source;
    MirrorProfile_SetScoVolume(ind->voice_source, ind->volume);
    if (MirrorProfile_IsEscoConnected() && esco->codec_mode != hfp_codec_mode_none)
    {
        MirrorProfile_StartScoAudio();
    }
}


/*
    Handlers for receiving mirror_profile channel messages.
*/

/* \brief Handle PEER_SIG_MARSHALLED_MSG_CHANNEL_RX_IND */
void MirrorProfile_HandlePeerSignallingMessage(const PEER_SIG_MARSHALLED_MSG_CHANNEL_RX_IND_T *ind)
{
    MIRROR_LOG("MirrorProfile_HandlePeerSignallingMessage. Channel 0x%x, type %d", ind->channel, ind->type);

    switch (ind->type)
    {
    case MARSHAL_TYPE(mirror_profile_hfp_volume_ind_t):
        {
            const mirror_profile_hfp_volume_ind_t* vol_ind = (const mirror_profile_hfp_volume_ind_t*)ind->msg;

            MirrorProfile_SetScoVolume(vol_ind->voice_source, vol_ind->volume);
        }
        break;

    case MARSHAL_TYPE(mirror_profile_hfp_codec_and_volume_ind_t):
        mirrorProfile_HandleHfpCodecAndVolume((const mirror_profile_hfp_codec_and_volume_ind_t*)ind->msg);
        break;

    case MARSHAL_TYPE(mirror_profile_a2dp_volume_ind_t):
        {
            const mirror_profile_a2dp_volume_ind_t* vol_ind = (const mirror_profile_a2dp_volume_ind_t*)ind->msg;

            MIRROR_LOG("MirrorProfile_HandlePeerSignallingMessage enum:audio_source_t:%d volume %d", vol_ind->audio_source, vol_ind->volume);
            mirrorProfile_UpdateAudioVolumeFromPeer(vol_ind->audio_source, vol_ind->volume);
        }
        break;

    case MARSHAL_TYPE(mirror_profile_stream_context_t):
        mirrorProfile_HandleA2dpStreamContext((const mirror_profile_stream_context_t*)ind->msg);
        break;

    case MARSHAL_TYPE(mirror_profile_stream_context_response_t):
        mirrorProfile_HandleA2dpStreamContextResponse((const mirror_profile_stream_context_response_t*)ind->msg);
        break;

    case MARSHAL_TYPE(mirror_profile_sync_a2dp_unmute_ind_t):
        mirrorProfile_HandleA2dpSyncUnmute((const mirror_profile_sync_a2dp_unmute_ind_t *)ind->msg);
        break;

    case MARSHAL_TYPE(mirror_profile_sync_sco_unmute_ind_t):
        mirrorProfile_HandleScoSyncUnmute((const mirror_profile_sync_sco_unmute_ind_t *)ind->msg);
        break;


    default:
        MIRROR_LOG("MirrorProfile_HandlePeerSignallingMessage unhandled type 0x%x", ind->type);
        break;
    }

    /* free unmarshalled msg */
    free(ind->msg);
}

/* \brief Handle PEER_SIG_MARSHALLED_MSG_CHANNEL_TX_CFM */
void MirrorProfile_HandlePeerSignallingMessageTxConfirm(const PEER_SIG_MARSHALLED_MSG_CHANNEL_TX_CFM_T *cfm)
{
    UNUSED(cfm);

    switch (cfm->type)
    {
        case MARSHAL_TYPE(mirror_profile_stream_context_t):
            if (cfm->status != peerSigStatusSuccess)
            {
                MirrorProfile_ClearStreamChangeLock();
            }
        break;


        default:
        break;
    }
}

#endif /* INCLUDE_MIRRORING */
