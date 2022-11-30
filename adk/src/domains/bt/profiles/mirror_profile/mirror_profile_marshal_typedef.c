/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \version %%version
    \file 
    \brief The mirror_profile marshal type definitions. This file is generated by D:/Work/Qcc/qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1/adk/tools/packages/typegen/typegen.py.
*/

#include <csrtypes.h>
#include <kymera_adaptation_voice_protected.h>
#include <app/marshal/marshal_if.h>
#include <mirror_profile_typedef.h>
#include <mirror_profile_marshal_typedef.h>
#include <marshal_common.h>

/*! A2DP media stream context sent from Primary to Secondary */
static const marshal_member_descriptor_t mirror_profile_stream_context_t_member_descriptors[] =
{
    /*! The BT address */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, bdaddr, addr),
    /*! L2CAP CID */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint16, cid),
    /*! L2CAP maximum transmission unit */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint16, mtu),
    /*! A2DP volume */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint8, volume),
    /*! Defines the current audio state - disconnected, connected, active */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint8, audio_state),
    /*! Negotiated AVDTP SEID (stream endpoing ID)  */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint8, seid),
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint8, codec_sub_type),
    /*! Codec bitrate bps */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint32, bitrate),
    /*! Audio sample rate (/25Hz) */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint16, sample_rate),
    /*! Audio sample bit width */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint8, sample_width),
    /*! Audio channel type */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint8, channel_type),
    /*! Audio source */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint8, audio_source),
    /*! Other flags */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint8, flags),
    /*! Content protection type */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint16, content_protection_type),
    /*! Length of extra CPI (always 0) */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint8, content_protection_information_length),
    /*! Q2Q Mode */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint8, q2q_mode),
    /*! Split Mode */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_t, uint32, aptx_features),
} ;

/*! A2DP media stream context response sent from Secondary to Primary */
static const marshal_member_descriptor_t mirror_profile_stream_context_response_t_member_descriptors[] =
{
    /*! L2CAP CID */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_response_t, uint16, cid),
    /*! Negotiated AVDTP SEID (stream endpoing ID)  */
    MAKE_MARSHAL_MEMBER(mirror_profile_stream_context_response_t, uint8, seid),
} ;

/*! Secondary sends this message to primary to tell it when to unmute during a mute-sync start */
static const marshal_member_descriptor_t mirror_profile_sync_a2dp_unmute_ind_t_member_descriptors[] =
{
    /*! The time at which to unmute */
    MAKE_MARSHAL_MEMBER(mirror_profile_sync_a2dp_unmute_ind_t, marshal_rtime_t, unmute_time),
} ;

/*! Secondary sends this message to primary to tell it when to unmute during a SCO mute-sync start */
static const marshal_member_descriptor_t mirror_profile_sync_sco_unmute_ind_t_member_descriptors[] =
{
    /*! The time at which to unmute */
    MAKE_MARSHAL_MEMBER(mirror_profile_sync_sco_unmute_ind_t, marshal_rtime_t, unmute_time),
} ;

/*! Unicast audio stream context sent from Primary to Secondary */
static const marshal_member_descriptor_t mirror_profile_lea_unicast_audio_conf_req_t_member_descriptors[] =
{
    /*! Audio source */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, audio_source),
    /*! Voice source */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, voice_source),
    /*! Codec version */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, codec_version),
    /*! CIG identifier */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, cig_id),
    /*! Audio context */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint16, audio_context),
    /*! Sink CIS identifier */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, sink_cis_id),
    /*! Sink ase identifier */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, sink_ase_id),
    /*! Sink target latency */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, sink_target_latency),
    /*! Sink target phy */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, sink_target_phy),
    /*! Sink codec identifier */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, sink_codec_id),
    /*! Sink sample frequency */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint16, sink_sampling_frequency),
    /*! Sink frame duration */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint16, sink_frame_duration),
    /*! Sink sdu interval */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint32, sink_sdu_interval),
    /*! Sink framing */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, sink_framing),
    /*! Sink phy */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, sink_phy),
    /*! Sink frame length */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint16, sink_frame_length),
    /*! Sink Frame Blocks per sdu */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, sink_blocks_per_sdu),
    /*! Sink retransmission number */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, sink_retransmission_num),
    /*! Sink max transport latency */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint16, sink_max_transport_latency),
    /*! Sink presentation dalay */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint32, sink_presentation_delay),
    /*! Source CIS identifier */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, source_cis_id),
    /*! Source ase identifier */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, source_ase_id),
    /*! Source target latency */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, source_target_latency),
    /*! Source target phy */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, source_target_phy),
    /*! Source codec identifier */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, source_codec_id),
    /*! Source sample frequency */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint16, source_sampling_frequency),
    /*! Source frame duration */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint16, source_frame_duration),
    /*! Source sdu interval */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint32, source_sdu_interval),
    /*! Source framing */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, source_framing),
    /*! Source phy */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, source_phy),
    /*! Source frame length */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint16, source_frame_length),
    /*! Source Frame Blocks per sdu */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, source_blocks_per_sdu),
    /*! Source retransmission number */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint8, source_retransmission_num),
    /*! Source max transport latency */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint16, source_max_transport_latency),
    /*! Source presentation dalay */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_conf_req_t, uint32, source_presentation_delay),
} ;

/*! Unicast audio stream context clear sent from Primary to Secondary */
static const marshal_member_descriptor_t mirror_profile_lea_unicast_audio_clear_req_t_member_descriptors[] =
{
    /*! Not used */
    MAKE_MARSHAL_MEMBER(mirror_profile_lea_unicast_audio_clear_req_t, uint8, dummy),
} ;

const marshal_type_descriptor_t marshal_type_descriptor_mirror_profile_hfp_volume_ind_t = MAKE_MARSHAL_TYPE_DEFINITION_BASIC(mirror_profile_hfp_volume_ind_t);
const marshal_type_descriptor_t marshal_type_descriptor_mirror_profile_hfp_codec_and_volume_ind_t = MAKE_MARSHAL_TYPE_DEFINITION_BASIC(mirror_profile_hfp_codec_and_volume_ind_t);
const marshal_type_descriptor_t marshal_type_descriptor_mirror_profile_a2dp_volume_ind_t = MAKE_MARSHAL_TYPE_DEFINITION_BASIC(mirror_profile_a2dp_volume_ind_t);
const marshal_type_descriptor_t marshal_type_descriptor_mirror_profile_stream_context_t = MAKE_MARSHAL_TYPE_DEFINITION(mirror_profile_stream_context_t, mirror_profile_stream_context_t_member_descriptors);
const marshal_type_descriptor_t marshal_type_descriptor_mirror_profile_stream_context_response_t = MAKE_MARSHAL_TYPE_DEFINITION(mirror_profile_stream_context_response_t, mirror_profile_stream_context_response_t_member_descriptors);
const marshal_type_descriptor_t marshal_type_descriptor_mirror_profile_sync_a2dp_unmute_ind_t = MAKE_MARSHAL_TYPE_DEFINITION(mirror_profile_sync_a2dp_unmute_ind_t, mirror_profile_sync_a2dp_unmute_ind_t_member_descriptors);
const marshal_type_descriptor_t marshal_type_descriptor_mirror_profile_sync_sco_unmute_ind_t = MAKE_MARSHAL_TYPE_DEFINITION(mirror_profile_sync_sco_unmute_ind_t, mirror_profile_sync_sco_unmute_ind_t_member_descriptors);
const marshal_type_descriptor_t marshal_type_descriptor_mirror_profile_lea_unicast_audio_conf_req_t = MAKE_MARSHAL_TYPE_DEFINITION(mirror_profile_lea_unicast_audio_conf_req_t, mirror_profile_lea_unicast_audio_conf_req_t_member_descriptors);
const marshal_type_descriptor_t marshal_type_descriptor_mirror_profile_lea_unicast_audio_clear_req_t = MAKE_MARSHAL_TYPE_DEFINITION(mirror_profile_lea_unicast_audio_clear_req_t, mirror_profile_lea_unicast_audio_clear_req_t_member_descriptors);


#define EXPAND_AS_TYPE_DEFINITION(type) (const marshal_type_descriptor_t *)&marshal_type_descriptor_##type,
const marshal_type_descriptor_t * const mirror_profile_marshal_type_descriptors[] =
{
    MARSHAL_COMMON_TYPES_TABLE(EXPAND_AS_TYPE_DEFINITION)
    MIRROR_PROFILE_MARSHAL_TYPES_TABLE(EXPAND_AS_TYPE_DEFINITION)
} ;

#undef EXPAND_AS_TYPE_DEFINITION

