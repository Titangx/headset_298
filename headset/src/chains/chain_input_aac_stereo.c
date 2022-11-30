/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \file chain_input_aac_stereo.c
    \brief The chain_input_aac_stereo chain.

    This file is generated by D:\Work\Qcc\qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1\adk\tools\packages\chaingen\chaingen_mod\__init__.pyc.
*/

#include <chain_input_aac_stereo.h>
#include <cap_id_prim.h>
#include <opmsg_prim.h>
#include <hydra_macros.h>
#include <../headset_cap_ids.h>
#include <kymera_chain_roles.h>
static const operator_config_t operators_p0[] =
{
    MAKE_OPERATOR_CONFIG(CAP_ID_RTP_DECODE, OPR_RTP_DECODER),
    MAKE_OPERATOR_CONFIG(HS_CAP_ID_AAC_DECODER, OPR_AAC_DECODER),
} ;

static const operator_config_t operators_p1[] =
{
    MAKE_OPERATOR_CONFIG(CAP_ID_RTP_DECODE, OPR_RTP_DECODER),
    MAKE_OPERATOR_CONFIG_P1(HS_CAP_ID_AAC_DECODER, OPR_AAC_DECODER),
} ;

static const operator_endpoint_t inputs[] =
{
    {OPR_RTP_DECODER, EPR_SINK_MEDIA, 0},
} ;

static const operator_endpoint_t outputs[] =
{
    {OPR_AAC_DECODER, EPR_SOURCE_DECODED_PCM, 0},
    {OPR_AAC_DECODER, EPR_SOURCE_DECODED_PCM_RIGHT, 1},
} ;

static const operator_connection_t connections[] =
{
    {OPR_RTP_DECODER, 0, OPR_AAC_DECODER, 0, 1},
} ;

const chain_config_t chain_input_aac_stereo_config_p0 = {0, 0, operators_p0, 2, inputs, 1, outputs, 2, connections, 1};

const chain_config_t chain_input_aac_stereo_config_p1 = {0, 0, operators_p1, 2, inputs, 1, outputs, 2, connections, 1};

