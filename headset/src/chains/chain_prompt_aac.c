/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \file chain_prompt_aac.c
    \brief The chain_prompt_aac chain.

    This file is generated by D:\Work\Qcc\qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1\adk\tools\packages\chaingen\chaingen_mod\__init__.pyc.
*/

#include <chain_prompt_aac.h>
#include <cap_id_prim.h>
#include <opmsg_prim.h>
#include <hydra_macros.h>
#include <../headset_cap_ids.h>
#include <kymera_chain_roles.h>
static const operator_config_t operators_p0[] =
{
    MAKE_OPERATOR_CONFIG(CAP_ID_BASIC_PASS, OPR_TONE_PROMPT_ENCODED_BUFFER),
    MAKE_OPERATOR_CONFIG(CAP_ID_AAC_DECODER, OPR_AAC_DECODER),
    MAKE_OPERATOR_CONFIG(CAP_ID_BASIC_PASS, OPR_TONE_PROMPT_DECODED_BUFFER),
} ;

static const operator_config_t operators_p1[] =
{
    MAKE_OPERATOR_CONFIG(CAP_ID_BASIC_PASS, OPR_TONE_PROMPT_ENCODED_BUFFER),
    MAKE_OPERATOR_CONFIG_P1(CAP_ID_AAC_DECODER, OPR_AAC_DECODER),
    MAKE_OPERATOR_CONFIG_P1(CAP_ID_BASIC_PASS, OPR_TONE_PROMPT_DECODED_BUFFER),
} ;

static const operator_endpoint_t inputs[] =
{
    {OPR_TONE_PROMPT_ENCODED_BUFFER, EPR_PROMPT_IN, 0},
} ;

static const operator_endpoint_t outputs[] =
{
    {OPR_TONE_PROMPT_DECODED_BUFFER, EPR_TONE_PROMPT_CHAIN_OUT, 0},
} ;

static const operator_connection_t connections[] =
{
    {OPR_TONE_PROMPT_ENCODED_BUFFER, 0, OPR_AAC_DECODER, 0, 1},
    {OPR_AAC_DECODER, 0, OPR_TONE_PROMPT_DECODED_BUFFER, 0, 1},
} ;

const chain_config_t chain_prompt_aac_config_p0 = {0, 0, operators_p0, 3, inputs, 1, outputs, 1, connections, 2};

const chain_config_t chain_prompt_aac_config_p1 = {0, 0, operators_p1, 3, inputs, 1, outputs, 1, connections, 2};

