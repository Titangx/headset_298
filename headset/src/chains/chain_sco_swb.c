/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \file chain_sco_swb.c
    \brief The chain_sco_swb chain.

    This file is generated by D:\Work\Qcc\qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1\adk\tools\packages\chaingen\chaingen_mod\__init__.pyc.
*/

#include <chain_sco_swb.h>
#include <cap_id_prim.h>
#include <opmsg_prim.h>
#include <hydra_macros.h>
#include <../headset_cap_ids.h>
#include <kymera_chain_roles.h>
static const operator_config_t operators[] =
{
    MAKE_OPERATOR_CONFIG_PRIORITY_MEDIUM(CAP_ID_SCO_SWBS_DEC, OPR_SCO_RECEIVE),
    MAKE_OPERATOR_CONFIG_PRIORITY_HIGH(CAP_ID_SCO_SWBS_ENC, OPR_SCO_SEND),
    MAKE_OPERATOR_CONFIG(CAP_ID_CVC_RECEIVE_SWB, OPR_CVC_RECEIVE),
    MAKE_OPERATOR_CONFIG(CAP_ID_CVCHS1MIC_SEND_SWB, OPR_CVC_SEND),
} ;

static const operator_endpoint_t inputs[] =
{
    {OPR_SCO_RECEIVE, EPR_SCO_FROM_AIR, 0},
    {OPR_CVC_SEND, EPR_CVC_SEND_REF_IN, 0},
    {OPR_CVC_SEND, EPR_CVC_SEND_IN1, 1},
} ;

static const operator_endpoint_t outputs[] =
{
    {OPR_CVC_RECEIVE, EPR_SCO_SPEAKER, 0},
    {OPR_SCO_SEND, EPR_SCO_TO_AIR, 0},
} ;

static const operator_connection_t connections[] =
{
    {OPR_SCO_RECEIVE, 0, OPR_CVC_RECEIVE, 0, 1},
    {OPR_CVC_SEND, 0, OPR_SCO_SEND, 0, 1},
} ;

const chain_config_t chain_sco_swb_config = {1, 0, operators, 4, inputs, 3, outputs, 2, connections, 2};

