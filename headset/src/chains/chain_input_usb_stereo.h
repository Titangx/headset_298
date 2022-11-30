/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \file chain_input_usb_stereo.h
    \brief The chain_input_usb_stereo chain.

    This file is generated by D:\Work\Qcc\qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1\adk\tools\packages\chaingen\chaingen_mod\__init__.pyc.
*/

#ifndef _CHAIN_INPUT_USB_STEREO_H__
#define _CHAIN_INPUT_USB_STEREO_H__

/*!
\page chain_input_usb_stereo
    @startuml
        object OPR_USB_AUDIO_RX
        OPR_USB_AUDIO_RX : id = CAP_ID_USB_AUDIO_RX
        object EPR_USB_FROM_HOST #lightgreen
        OPR_USB_AUDIO_RX "IN(0)" <-- EPR_USB_FROM_HOST
        object EPR_SOURCE_DECODED_PCM #lightblue
        EPR_SOURCE_DECODED_PCM <-- "OUT_0(0)" OPR_USB_AUDIO_RX
        object EPR_SOURCE_DECODED_PCM_RIGHT #lightblue
        EPR_SOURCE_DECODED_PCM_RIGHT <-- "OUT_1(1)" OPR_USB_AUDIO_RX
    @enduml
*/

#include <chain.h>

extern const chain_config_t chain_input_usb_stereo_config;

#endif /* _CHAIN_INPUT_USB_STEREO_H__ */
