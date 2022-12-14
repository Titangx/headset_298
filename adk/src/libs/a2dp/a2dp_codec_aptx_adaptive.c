/****************************************************************************
Copyright (c) 2020 Qualcomm Technologies International, Ltd.


FILE NAME
    a2dp_codec_aptx_adaptive.c

DESCRIPTION
    This file contains aptX Adaptive specific code.

NOTES

*/

#ifndef A2DP_SBC_ONLY

/****************************************************************************
    Header files
*/
#include "a2dp.h"
#include "a2dp_private.h"
#include "a2dp_caps_parse.h"
#include "a2dp_codec_aptx_adaptive.h"
#include "a2dp_data_block.h"
#include "a2dp_codec_handler.h"
#include <byte_utils.h>

#ifdef A2DP_DEBUG_LIB
#include <print.h>
#include <panic.h>
#define DEBUG_PANIC(x) {PRINT(x);  Panic();}
#else
#define DEBUG_PANIC(x) {}
#endif

/* Length of codec service capability (decoder and encoder have the same length) */
#define APTX_AD_LENGTH_OF_CAP (19)

/* This octet index is relative to the AVDTP_SERVICE_MEDIA_CODEC in the encoder service capability */
typedef enum
{
    aptx_ad_q2q_mode_octet         = 10,
    aptx_ad_custom_avrcp_cmd_octet = 10,
    aptx_ad_sample_rate_octet      = 10,
    aptx_ad_channel_mode_octet     = 11,
    aptx_ad_ll0_in_ms_octet        = 12,
    aptx_ad_ll1_in_ms_octet        = 13,
    aptx_ad_hq_in_2ms_octet        = 14,
    aptx_ad_tws_in_2ms_octet       = 15,
    aptx_ad_cap_ext_version_octet  = 19,
    aptx_ad_supported_features_byte1 = 20
} aptx_adaptive_encoder_service_capability_octet_index_t;

typedef enum
{
    aptx_ad_cap_ext_version_1 = 0,
    aptx_ad_cap_ext_version_2 = 1,
} aptx_adaptive_encoder_capability_extension_version_number;

/* Defines the distance between AVDTP_SERVICE_MEDIA_CODEC and the start of VENDOR ID in the service capability */
#define OFFSET_IN_SERVICE_CAP (4)

/*! Default aptx packet size (Largest aptX adaptive packet size that can fit in a 2-DH5 payload) */
#define APTX_AD_DEFAULT_PACKET_SIZE 668
#define APTX_AD_DEFAULT_PACKET_SIZE_QHS 960

#define APTX_AD_CUSTOM_AVRCP_CMD_MASK (1 << 2)
/*! Mask for Q2Q Enabled bit */
#define APTX_AD_Q2Q_MODE_MASK         (1 << 1)

typedef enum
{
    aptx_ad_sample_rate_44_1_mask = (1 << 3),
    aptx_ad_sample_rate_48_mask   = (1 << 4),
    aptx_ad_sample_rate_96_mask   = (1 << 5),
    aptx_ad_sample_rate_192_mask  = (1 << 6),
    aptx_ad_sample_rate_96_twm_mask  = (1 << 7)
} aptx_adaptive_sampling_rate_mask_t;

#define APTX_AD_ALL_SAMPLING_RATES_MASK (\
                                            aptx_ad_sample_rate_44_1_mask | \
                                            aptx_ad_sample_rate_48_mask   | \
                                            aptx_ad_sample_rate_96_mask   | \
                                            aptx_ad_sample_rate_192_mask  | \
                                            aptx_ad_sample_rate_96_twm_mask \
                                        )

#define APTX_AD_NO_SAMPLING_RATES_MASK  (0xFF - APTX_AD_ALL_SAMPLING_RATES_MASK)

typedef enum
{
    aptx_ad_channel_mode_mono_mask         = (1 << 0),
    aptx_ad_channel_mode_stereo_mask       = (1 << 1),
    aptx_ad_channel_mode_tws_stereo_mask   = (1 << 2),
    aptx_ad_channel_mode_joint_stereo_mask = (1 << 3),
    aptx_ad_channel_mode_tws_mono_mask     = (1 << 4),
    aptx_ad_channel_mode_tws_plus_mask     = (1 << 5)
} aptx_adaptive_channel_mode_mask_t;

typedef enum
{
    aptx_ad_fb1_twm             = 1 ,
    aptx_ad_fb1_kernel_21       = (1 << 1),
    aptx_ad_fb1_split_streaming = (1 << 2),
    aptx_ad_fb1_split_ack       = (1 << 3),
} aptx_adaptive_features_byte1_t;

#define APTX_AD_ALL_CHANNEL_MODES_MASK (\
                                            aptx_ad_channel_mode_mono_mask         | \
                                            aptx_ad_channel_mode_stereo_mask       | \
                                            aptx_ad_channel_mode_tws_stereo_mask   | \
                                            aptx_ad_channel_mode_joint_stereo_mask | \
                                            aptx_ad_channel_mode_tws_mono_mask     | \
                                            aptx_ad_channel_mode_tws_plus_mask       \
                                       )

#define APTX_AD_NO_CHANNEL_MODES_MASK  (0xFF - APTX_AD_ALL_CHANNEL_MODES_MASK)

typedef enum
{
    aptx_ad_generic_unsupported = 0,
    aptx_ad_generic_supported   = 1
} aptx_ad_generic_supported_t;

static aptx_adaptive_ttp_latencies_t getNq2qTargetTtpLatencies(const uint8 *service_caps)
{
    aptx_adaptive_ttp_latencies_t nq2q_ttp;

    nq2q_ttp.low_latency_0_in_ms = service_caps[aptx_ad_ll0_in_ms_octet];
    nq2q_ttp.low_latency_1_in_ms = service_caps[aptx_ad_ll1_in_ms_octet];
    nq2q_ttp.high_quality_in_2ms = service_caps[aptx_ad_hq_in_2ms_octet];
    nq2q_ttp.tws_legacy_in_2ms   = service_caps[aptx_ad_tws_in_2ms_octet];

    return nq2q_ttp;
}

static uint32 getSamplingRates(uint8 sampling_rates)
{
    uint32 sampling_rate = 0;

    if (sampling_rates & aptx_ad_sample_rate_192_mask)
        sampling_rate = 192000;
    else if (sampling_rates & aptx_ad_sample_rate_96_twm_mask)
        sampling_rate = 96000;
    else if (sampling_rates & aptx_ad_sample_rate_96_mask)
        sampling_rate = 96000;
    else if (sampling_rates & aptx_ad_sample_rate_48_mask)
        sampling_rate = 48000;
    else if (sampling_rates & aptx_ad_sample_rate_44_1_mask)
        sampling_rate = 44100;
    /* Compatibility of sampling rates has been checked during capability exchange, so this should never be called */
    else
        DEBUG_PANIC(("a2dp: Invalid sampling rate"));

    return sampling_rate;
}

static a2dp_channel_mode getChannelMode(uint8 channel_modes)
{
    a2dp_channel_mode channel_mode = 0;

    if (channel_modes & aptx_ad_channel_mode_mono_mask)
        channel_mode = a2dp_mono;
    else if (channel_modes & aptx_ad_channel_mode_stereo_mask)
        channel_mode = a2dp_stereo;
    else if (channel_modes & aptx_ad_channel_mode_tws_stereo_mask)
        channel_mode = a2dp_joint_stereo;
    else if (channel_modes & aptx_ad_channel_mode_joint_stereo_mask)
        channel_mode = a2dp_joint_stereo;
    else if (channel_modes & aptx_ad_channel_mode_tws_mono_mask)
        channel_mode = a2dp_mono;
    else if (channel_modes & aptx_ad_channel_mode_tws_plus_mask)
        channel_mode = a2dp_mono;
    /* Compatibility of channel modes has been checked during capability exchange, so this should never be called */
    else
        DEBUG_PANIC(("a2dp: Invalid channel mode"));

    return channel_mode;
}

static bool isCustomAvrcpCommandSupported(uint8 avrcp_cmd_octet)
{
    if (avrcp_cmd_octet & APTX_AD_CUSTOM_AVRCP_CMD_MASK)
        return TRUE;
    else
        return FALSE;
}

static bool isQ2qModeEnabled(uint8 source_type_octet)
{
    if (source_type_octet & APTX_AD_Q2Q_MODE_MASK)
        return FALSE;
    else
        return TRUE;
}

static void selectSamplingRateIfSupportedByBothSides(uint8 *remote_sampling_rates, uint8 local_sampling_rates,
                                                     aptx_adaptive_sampling_rate_mask_t sampling_rate)
{
    if (*remote_sampling_rates & local_sampling_rates & sampling_rate)
        *remote_sampling_rates &= (APTX_AD_NO_SAMPLING_RATES_MASK | sampling_rate);
}

static bool areSamplingRatesCompatible(uint8 remote_sampling_rates, uint8 local_sampling_rates)
{
    if ((remote_sampling_rates & local_sampling_rates & APTX_AD_ALL_SAMPLING_RATES_MASK) == 0)
        return FALSE;
    else
        return TRUE;
}

static uint8 selectOptimalSamplingRate(uint8 remote_sampling_rates, uint8 local_sampling_rates)
{
    uint8 sampling_rate = 0;

    if (areSamplingRatesCompatible(remote_sampling_rates, local_sampling_rates))
    {
        /* selectSamplingRateIfSupportedByBothSides() will modify remote_sampling_rates if it finds a matching rate,
           preventing further matches, so ensure each rate is checked in descending order of preference. */
        selectSamplingRateIfSupportedByBothSides(&remote_sampling_rates, local_sampling_rates, aptx_ad_sample_rate_192_mask);
        selectSamplingRateIfSupportedByBothSides(&remote_sampling_rates, local_sampling_rates, aptx_ad_sample_rate_96_twm_mask);
        selectSamplingRateIfSupportedByBothSides(&remote_sampling_rates, local_sampling_rates, aptx_ad_sample_rate_96_mask);
        selectSamplingRateIfSupportedByBothSides(&remote_sampling_rates, local_sampling_rates, aptx_ad_sample_rate_48_mask);
        selectSamplingRateIfSupportedByBothSides(&remote_sampling_rates, local_sampling_rates, aptx_ad_sample_rate_44_1_mask);
        sampling_rate = remote_sampling_rates;
    }
    /* Compatibility of sampling rates has been checked during capability exchange, so this should never be called */
    else
        DEBUG_PANIC(("a2dp: Invalid sampling rate"));

    return sampling_rate;
}

static void selectChannelModeIfSupportedByBothSides(uint8 *remote_channel_modes, uint8 local_channel_modes,
                                                    aptx_adaptive_channel_mode_mask_t channel_mode)
{
    if (*remote_channel_modes & local_channel_modes & channel_mode)
        *remote_channel_modes &= (APTX_AD_NO_CHANNEL_MODES_MASK | channel_mode);
}

static bool areChannelModesCompatible(uint8 remote_channel_modes, uint8 local_channel_modes)
{
    if ((remote_channel_modes & local_channel_modes & APTX_AD_ALL_CHANNEL_MODES_MASK) == 0)
        return FALSE;
    else
        return TRUE;
}

static uint8 selectOptimalChannelMode(uint8 remote_channel_modes, uint8 local_channel_modes)
{
    uint8 channel_mode = 0;

    if (areChannelModesCompatible(remote_channel_modes, local_channel_modes))
    {
        selectChannelModeIfSupportedByBothSides(&remote_channel_modes, local_channel_modes, aptx_ad_channel_mode_joint_stereo_mask);
        selectChannelModeIfSupportedByBothSides(&remote_channel_modes, local_channel_modes, aptx_ad_channel_mode_stereo_mask);
        selectChannelModeIfSupportedByBothSides(&remote_channel_modes, local_channel_modes, aptx_ad_channel_mode_tws_plus_mask);
        selectChannelModeIfSupportedByBothSides(&remote_channel_modes, local_channel_modes, aptx_ad_channel_mode_tws_stereo_mask);
        selectChannelModeIfSupportedByBothSides(&remote_channel_modes, local_channel_modes, aptx_ad_channel_mode_tws_mono_mask);
        selectChannelModeIfSupportedByBothSides(&remote_channel_modes, local_channel_modes, aptx_ad_channel_mode_mono_mask);
        channel_mode = remote_channel_modes;
    }
    /* Compatibility of channel modes has been checked during capability exchange, so this should never be called */
    else
        DEBUG_PANIC(("a2dp: Invalid channel mode"));

    return channel_mode;
}



static uint8 selectRemoteFeatures(uint8 remote_features, uint8 local_features)
{
    uint8 features = remote_features;

    /* check Split mode */
    if (!(remote_features & local_features & aptx_ad_fb1_split_streaming))
    {/* Not supported at both ends, so we need to disable */
        features &= (0xFF - aptx_ad_fb1_split_streaming);
    }
    if (!(remote_features & local_features & aptx_ad_fb1_split_ack))
    {/* Not supported at both ends, so we need to disable */
        features &= (0xFF - aptx_ad_fb1_split_ack);
    }
    if (!(remote_features & local_features & aptx_ad_fb1_kernel_21))
    {/* Not supported at both ends, so we need to disable */
        features &= (0xFF - aptx_ad_fb1_kernel_21);
    }
    if (!(remote_features & local_features & aptx_ad_fb1_twm))
    {/* Not supported at both ends, so we need to disable it */
        features &= (0xFF - aptx_ad_fb1_twm);
    }
    return features;
}

/**************************************************************************/

bool areAptxAdCodecsCompatible(const uint8 *local_caps, const uint8 *remote_caps, uint8 local_losc, uint8 remote_losc)
{
    unsigned sample_rate_octet = aptx_ad_sample_rate_octet - OFFSET_IN_SERVICE_CAP;
    unsigned channel_mode_octet = aptx_ad_channel_mode_octet - OFFSET_IN_SERVICE_CAP;

    /* check length to prevent read off end of buffer */
    if ((local_losc < APTX_AD_LENGTH_OF_CAP) || (remote_losc < APTX_AD_LENGTH_OF_CAP))
        return FALSE;

    if (areSamplingRatesCompatible(remote_caps[sample_rate_octet], local_caps[sample_rate_octet]) == FALSE)
        return FALSE;

    if (areChannelModesCompatible(remote_caps[channel_mode_octet], local_caps[channel_mode_octet]) == FALSE)
        return FALSE;

    return TRUE;
}

void selectOptimalAptxAdCapsSink(const uint8 *local_codec_caps, uint8 *remote_codec_caps)
{
    remote_codec_caps[aptx_ad_sample_rate_octet] = selectOptimalSamplingRate(remote_codec_caps[aptx_ad_sample_rate_octet], local_codec_caps[aptx_ad_sample_rate_octet]);
    remote_codec_caps[aptx_ad_q2q_mode_octet] |= aptx_ad_generic_supported;
    remote_codec_caps[aptx_ad_channel_mode_octet] = selectOptimalChannelMode(remote_codec_caps[aptx_ad_channel_mode_octet], local_codec_caps[aptx_ad_channel_mode_octet]);
    remote_codec_caps[aptx_ad_supported_features_byte1] = selectRemoteFeatures(remote_codec_caps[aptx_ad_supported_features_byte1], local_codec_caps[aptx_ad_supported_features_byte1]);
}

void selectOptimalAptxAdCapsSource(const uint8 *local_codec_caps, uint8 *remote_codec_caps)
{
    remote_codec_caps[aptx_ad_sample_rate_octet] = selectOptimalSamplingRate(remote_codec_caps[aptx_ad_sample_rate_octet], local_codec_caps[aptx_ad_sample_rate_octet]);
    remote_codec_caps[aptx_ad_channel_mode_octet] = selectOptimalChannelMode(remote_codec_caps[aptx_ad_channel_mode_octet], local_codec_caps[aptx_ad_channel_mode_octet]);
    remote_codec_caps[aptx_ad_supported_features_byte1] = selectRemoteFeatures(remote_codec_caps[aptx_ad_supported_features_byte1], local_codec_caps[aptx_ad_supported_features_byte1]);
}

void getAptxAdConfigSettings(const uint8 *service_caps, a2dp_codec_settings *codec_settings)
{
    if (service_caps)
    {

        codec_settings->codecData.aptx_ad_params.q2q_enabled = isQ2qModeEnabled(service_caps[aptx_ad_q2q_mode_octet]);
        codec_settings->codecData.aptx_ad_params.avrcp_cmd_supported = isCustomAvrcpCommandSupported(service_caps[aptx_ad_custom_avrcp_cmd_octet]);
        codec_settings->rate = getSamplingRates(service_caps[aptx_ad_sample_rate_octet]);
        codec_settings->channel_mode = getChannelMode(service_caps[aptx_ad_channel_mode_octet]);
        codec_settings->codecData.aptx_ad_params.nq2q_ttp = getNq2qTargetTtpLatencies(service_caps);
        codec_settings->codecData.aptx_ad_params.is_twsp_mode = (service_caps[aptx_ad_channel_mode_octet] & aptx_ad_channel_mode_tws_plus_mask) ? 1 : 0;

        if (codec_settings->codecData.aptx_ad_params.q2q_enabled)
        {
            codec_settings->codecData.packet_size = APTX_AD_DEFAULT_PACKET_SIZE_QHS;
        }
        else
        {
            codec_settings->codecData.packet_size = APTX_AD_DEFAULT_PACKET_SIZE;
        }

        if ( service_caps[aptx_ad_cap_ext_version_octet] == aptx_ad_cap_ext_version_1 )
        {
            codec_settings->codecData.aptx_ad_params.version = aptx_ad_version_1_1;
            codec_settings->codecData.aptx_ad_params.features = 0;
        }
        else if ( service_caps[aptx_ad_cap_ext_version_octet] == aptx_ad_cap_ext_version_2 )
        {
            if ( service_caps[aptx_ad_supported_features_byte1] & aptx_ad_fb1_kernel_21 )
            {
                codec_settings->codecData.aptx_ad_params.version = aptx_ad_version_2_1;
            } else
            {
                codec_settings->codecData.aptx_ad_params.version = aptx_ad_version_2_0;
            }
            codec_settings->codecData.aptx_ad_params.features = ByteUtilsGet4BytesFromStream(&service_caps[aptx_ad_supported_features_byte1]);
        } else
        {
            /* unknown or unsupported version */
            codec_settings->codecData.aptx_ad_params.version = aptx_ad_version_unknown;
            codec_settings->codecData.aptx_ad_params.features = 0;
        }
    }
    /* We need the service capability, since this function should not decide the settings to be used */
    else
        DEBUG_PANIC(("a2dp: The service capability passed is NULL"));
}

bool getAptxAdQ2qMode(const remote_device *device)
{
    const uint8 *codec_caps;

    if((codec_caps = blockGetBase( (uint8)device->bitfields.device_id, data_block_configured_service_caps)) == NULL)
    {
        return FALSE;
    }

    if (!a2dpFindCodecSpecificInformation(&codec_caps, NULL))
        return FALSE;

    if(codec_caps[3] == AVDTP_MEDIA_CODEC_NONA2DP)
    {
        if (isCodecAptxAdaptive(codec_caps))
        {
            /* Check Q2Q mode */
            return isQ2qModeEnabled(codec_caps[aptx_ad_q2q_mode_octet]);
        }
    }

    return FALSE;
}


#else
    static const int dummy;
#endif /* A2DP_SBC_ONLY */
