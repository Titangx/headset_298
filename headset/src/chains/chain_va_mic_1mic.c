/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \file chain_va_mic_1mic.c
    \brief The chain_va_mic_1mic chain.

    This file is generated by D:\Work\Qcc\qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1\adk\tools\packages\chaingen\chaingen_mod\__init__.pyc.
*/

#include <chain_va_mic_1mic.h>
#include <cap_id_prim.h>
#include <opmsg_prim.h>
#include <hydra_macros.h>
#include <../headset_chain_config.h>
#include <kymera_chain_roles.h>
static const operator_config_t operators[] =
{
    MAKE_OPERATOR_CONFIG(CAP_ID_BASIC_PASS, OPR_MIC_GAIN),
} ;

static const operator_endpoint_t inputs[] =
{
    {OPR_MIC_GAIN, EPR_VA_MIC_MIC1_IN, 0},
} ;

static const operator_endpoint_t outputs[] =
{
    {OPR_MIC_GAIN, EPR_VA_MIC_ENCODE_OUT, 0},
} ;

const chain_config_t chain_va_mic_1mic_config = {1, 0, operators, 1, inputs, 1, outputs, 1, NULL, 0};

