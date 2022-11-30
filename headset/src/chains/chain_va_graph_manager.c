/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \file chain_va_graph_manager.c
    \brief The chain_va_graph_manager chain.

    This file is generated by D:\Work\Qcc\qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1\adk\tools\packages\chaingen\chaingen_mod\__init__.pyc.
*/

#include <chain_va_graph_manager.h>
#include <cap_id_prim.h>
#include <opmsg_prim.h>
#include <hydra_macros.h>
#include <../headset_cap_ids.h>
#include <kymera_chain_roles.h>
static const operator_config_t operators[] =
{
    MAKE_OPERATOR_CONFIG(CAP_ID_DOWNLOAD_VA_GRAPH_MANAGER, OPR_VA_GRAPH_MANAGER),
} ;

const chain_config_t chain_va_graph_manager_config = {0, 0, operators, 1, NULL, 0, NULL, 0, NULL, 0};
