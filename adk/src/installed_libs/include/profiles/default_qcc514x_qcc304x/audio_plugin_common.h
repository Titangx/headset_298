/****************************************************************************
Copyright (c) 2004 - 2015 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_plugin_common.h
    
DESCRIPTION
    Header file for the audio plugin common library.
*/

/*!
\defgroup audio_plugin_common audio_plugin_common
\ingroup vm_libs

\brief  Header file for the audio plugin common library.
    
\section audio_plugin_common_intro INTRODUCTION
        This defines functionality common to all audio plugins.
*/

/** @{ */

#ifndef _AUDIO_PLUGIN_COMMON_H_
#define _AUDIO_PLUGIN_COMMON_H_

#include "audio_plugin_if.h"
#include "micbias.h"

#define MAX_SUPPORTED_PIOS  3

/****************************************************************************
DESCRIPTION
	Data Description of Mic Gain Levels
*/
typedef struct tag_mic_type
{
   unsigned unused:7;
   unsigned digital_gain:4;
   unsigned analogue_gain:5;   /* low bits */
} T_mic_gain;

/****************************************************************************
DESCRIPTION
    Data Description of Mic bias
*/
typedef struct
{
    uint8 pin;
    uint8 refcount;
} mic_pio;

typedef struct
{
    uint8 refcount_analog[BIAS_CONFIG_PIO];
    mic_pio pio[MAX_SUPPORTED_PIOS];
    uint8 refcount_pcm[BIAS_CONFIG_PIO];
} mic_bias;

/****************************************************************************
DESCRIPTION
	Data Description of a couple of common mic gain levels
*/
extern const T_mic_gain MIC_MUTE;         /* -45db, -24db */
extern const T_mic_gain MIC_DEFAULT_GAIN; /* +3db for digital and analog */

/****************************************************************************
DESCRIPTION
    Get hardware instance from mic parameters
*/
audio_instance AudioPluginGetMicInstance(const audio_mic_params params);

/****************************************************************************
DESCRIPTION
    Get hardware instance from analogue input parameters
*/
audio_instance AudioPluginGetAnalogueInputInstance(const analogue_input_params analogue_in);

/****************************************************************************
DESCRIPTION
    Get mic source
*/
Source AudioPluginGetMicSource(const audio_mic_params mic_params, audio_channel channel);

/****************************************************************************
DESCRIPTION
    Get analogue input source
*/
Source AudioPluginGetAnalogueInputSource(const analogue_input_params analogue_input, audio_channel channel);

/****************************************************************************
DESCRIPTION
    Configure Mic channel
*/
void AudioPluginSetMicRate(audio_channel channel, const audio_mic_params audio_mic, uint32 mic_rate);

/****************************************************************************
DESCRIPTION
    Set mic gain
*/
void AudioPluginSetMicGain(audio_channel channel, const audio_mic_params audio_mic);

/****************************************************************************
DESCRIPTION
    Configure the state of the microphone bias drive
*/
void AudioPluginSetMicBiasDrive(const audio_mic_params params, bool set);

/****************************************************************************
DESCRIPTION
    Set mic bias or digital mic PIO to default state (off)
*/
void AudioPluginInitMicBiasDrive(const audio_mic_params audio_mic);

/****************************************************************************
DESCRIPTION
    Apply mic configuration and set mic PIO
*/
Source AudioPluginMicSetup(audio_channel channel, const audio_mic_params audio_mic);

/****************************************************************************
DESCRIPTION
    Apply analogue input configuration and set line in PIO
*/
Source AudioPluginAnalogueInputSetup(audio_channel channel, const analogue_input_params params, uint32 rate);

/****************************************************************************
DESCRIPTION
    Apply shutdown to a mic previously setup 
*/
void AudioPluginMicShutdown(audio_channel channel, const audio_mic_params * params, bool close_mic);

/****************************************************************************
DESCRIPTION
    Configure to use rate adjustment capability
*/
void AudioPluginUseStandaloneRateAdjustment(Source source, uint32 op);

/****************************************************************************
DESCRIPTION
    Callback function for getting the mic bias voltage from app
*/
void AudioPluginCommonRegisterMicBiasVoltageCallback(unsigned (*callback)(mic_bias_id id));

#endif /* _AUDIO_PLUGIN_COMMON_H_ */

/** @} */
