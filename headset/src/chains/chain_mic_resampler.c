/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \file chain_mic_resampler.c
    \brief The chain_mic_resampler chain.

    This file is generated by D:\Work\Qcc\qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1\adk\tools\packages\chaingen\chaingen_mod\__init__.pyc.
*/

#include <chain_mic_resampler.h>
#include <cap_id_prim.h>
#include <opmsg_prim.h>
#include <hydra_macros.h>
#include <kymera_chain_roles.h>
static const operator_config_t operators[] =
{
    MAKE_OPERATOR_CONFIG(CAP_ID_IIR_RESAMPLER, OPR_MIC_RESAMPLER),
} ;

static const operator_endpoint_t inputs[] =
{
    {OPR_MIC_RESAMPLER, EPR_AEC_RESAMPLER_IN_REF, 0},
    {OPR_MIC_RESAMPLER, EPR_MIC_RESAMPLER_IN1, 1},
    {OPR_MIC_RESAMPLER, EPR_MIC_RESAMPLER_IN2, 2},
    {OPR_MIC_RESAMPLER, EPR_MIC_RESAMPLER_IN3, 3},
    {OPR_MIC_RESAMPLER, EPR_MIC_RESAMPLER_IN4, 4},
    {OPR_MIC_RESAMPLER, EPR_MIC_RESAMPLER_IN5, 5},
    {OPR_MIC_RESAMPLER, EPR_MIC_RESAMPLER_IN6, 6},
    {OPR_MIC_RESAMPLER, EPR_MIC_RESAMPLER_IN7, 7},
} ;

static const operator_endpoint_t outputs[] =
{
    {OPR_MIC_RESAMPLER, EPR_AEC_RESAMPLER_OUT_REF, 0},
    {OPR_MIC_RESAMPLER, EPR_MIC_RESAMPLER_OUT1, 1},
    {OPR_MIC_RESAMPLER, EPR_MIC_RESAMPLER_OUT2, 2},
    {OPR_MIC_RESAMPLER, EPR_MIC_RESAMPLER_OUT3, 3},
    {OPR_MIC_RESAMPLER, EPR_MIC_RESAMPLER_OUT4, 4},
    {OPR_MIC_RESAMPLER, EPR_MIC_RESAMPLER_OUT5, 5},
    {OPR_MIC_RESAMPLER, EPR_MIC_RESAMPLER_OUT6, 6},
    {OPR_MIC_RESAMPLER, EPR_MIC_RESAMPLER_OUT7, 7},
} ;

const chain_config_t chain_mic_resampler_config = {0, 0, operators, 1, inputs, 8, outputs, 8, NULL, 0};

