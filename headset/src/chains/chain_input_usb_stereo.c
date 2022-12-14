/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \file chain_input_usb_stereo.c
    \brief The chain_input_usb_stereo chain.

    This file is generated by D:\Work\Qcc\qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1\adk\tools\packages\chaingen\chaingen_mod\__init__.pyc.
*/

#include <chain_input_usb_stereo.h>
#include <cap_id_prim.h>
#include <opmsg_prim.h>
#include <hydra_macros.h>
#include <../headset_cap_ids.h>
#include <kymera_chain_roles.h>
static const operator_config_t operators[] =
{
    MAKE_OPERATOR_CONFIG(CAP_ID_USB_AUDIO_RX, OPR_USB_AUDIO_RX),
} ;

static const operator_endpoint_t inputs[] =
{
    {OPR_USB_AUDIO_RX, EPR_USB_FROM_HOST, 0},
} ;

static const operator_endpoint_t outputs[] =
{
    {OPR_USB_AUDIO_RX, EPR_SOURCE_DECODED_PCM, 0},
    {OPR_USB_AUDIO_RX, EPR_SOURCE_DECODED_PCM_RIGHT, 1},
} ;

const chain_config_t chain_input_usb_stereo_config = {0, 0, operators, 1, inputs, 1, outputs, 2, NULL, 0};

