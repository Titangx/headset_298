/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \file chain_va_mic_2mic_cvc_wuw.h
    \brief The chain_va_mic_2mic_cvc_wuw chain.

    This file is generated by D:\Work\Qcc\qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1\adk\tools\packages\chaingen\chaingen_mod\__init__.pyc.
*/

#ifndef _CHAIN_VA_MIC_2MIC_CVC_WUW_H__
#define _CHAIN_VA_MIC_2MIC_CVC_WUW_H__

/*!
\page chain_va_mic_2mic_cvc_wuw
    @startuml
        object OPR_VAD
        OPR_VAD : id = CAP_ID_VAD
        object OPR_SPLITTER
        OPR_SPLITTER : id = CAP_ID_SPLITTER
        object OPR_MIC_GAIN
        OPR_MIC_GAIN : id = CAP_ID_BASIC_PASS
        object OPR_CVC_SEND
        OPR_CVC_SEND : id = CAP_ID_VA_CVC_2MIC
        OPR_CVC_SEND "MIC1_IN(1)"<-- "MIC1_OUT(0)" OPR_MIC_GAIN
        OPR_CVC_SEND "MIC2_IN(2)"<-- "MIC2_OUT(1)" OPR_MIC_GAIN
        OPR_VAD "IN(0)"<-- "OUT(1)" OPR_CVC_SEND
        OPR_SPLITTER "IN(0)"<-- "OUT(0)" OPR_VAD
        object EPR_VA_MIC_AEC_IN #lightgreen
        OPR_CVC_SEND "AEC_IN(0)" <-- EPR_VA_MIC_AEC_IN
        object EPR_VA_MIC_MIC1_IN #lightgreen
        OPR_MIC_GAIN "MIC1_IN(0)" <-- EPR_VA_MIC_MIC1_IN
        object EPR_VA_MIC_MIC2_IN #lightgreen
        OPR_MIC_GAIN "MIC2_IN(1)" <-- EPR_VA_MIC_MIC2_IN
        object EPR_VA_MIC_WUW_OUT #lightblue
        EPR_VA_MIC_WUW_OUT <-- "DETECTION_OUT(0)" OPR_SPLITTER
        object EPR_VA_MIC_ENCODE_OUT #lightblue
        EPR_VA_MIC_ENCODE_OUT <-- "CAPTURE_OUT(1)" OPR_SPLITTER
    @enduml
*/

#include <chain.h>

extern const chain_config_t chain_va_mic_2mic_cvc_wuw_config;

#endif /* _CHAIN_VA_MIC_2MIC_CVC_WUW_H__ */

