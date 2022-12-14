/**
* Copyright (c) 2014 - 2017 Qualcomm Technologies International, Ltd.
* \file  cvc_send.c
* \ingroup  capabilities
*
*  CVC send
*
*/

/****************************************************************************
Include Files
*/
#include <string.h>
#include "capabilities.h"
#include "cvc_send_cap_c.h"
#include "mem_utils/dynloader.h"
#include "mem_utils/exported_constants.h"
#include "mem_utils/exported_constant_files.h"
#include "../lib/audio_proc/iir_resamplev2_util.h"
#include "cvc_processing_c.h"

/****************************************************************************

Local Definitions
*/

/* Reference inputs 0-2 */
#define CVC_SEND_NUM_INPUTS_MASK    0x7
#define CVC_SEND_NUM_OUTPUTS_MASK   0x7

/* CVS send terminal mic 0*/
#define CVC_SEND_TERMINAL_MIC0      1

/* Voice quality metric error code */
#define CVC_SEND_VOICE_QUALITY_METRIC_ERROR_CODE 0xFF

/* bit field for kicking VA channels */
#define TOUCHED_CVC_VA_SOURCES     0x01E 

/****************************************************************************
Private Constant Definitions
*/

#ifdef CAPABILITY_DOWNLOAD_BUILD
#define CVCHS1MIC_SEND_NB_CAP_ID           CAP_ID_DOWNLOAD_CVCHS1MIC_SEND_NB
#define CVCHS1MIC_SEND_WB_CAP_ID           CAP_ID_DOWNLOAD_CVCHS1MIC_SEND_WB
#define CVCHS1MIC_SEND_UWB_CAP_ID          CAP_ID_DOWNLOAD_CVCHS1MIC_SEND_UWB
#define CVCHS1MIC_SEND_SWB_CAP_ID          CAP_ID_DOWNLOAD_CVCHS1MIC_SEND_SWB
#define CVCHS1MIC_SEND_FB_CAP_ID           CAP_ID_DOWNLOAD_CVCHS1MIC_SEND_FB
#define CVCHS2MIC_MONO_SEND_NB_CAP_ID      CAP_ID_DOWNLOAD_CVCHS2MIC_MONO_SEND_NB
#define CVCHS2MIC_MONO_SEND_WB_CAP_ID      CAP_ID_DOWNLOAD_CVCHS2MIC_MONO_SEND_WB
#define CVCHS2MIC_WAKEON_WB_CAP_ID         CAP_ID_DOWNLOAD_CVCHS2MIC_WAKEON_WB
#define CVCHS2MIC_BARGEIN_WB_CAP_ID        CAP_ID_DOWNLOAD_CVCHS2MIC_BARGEIN_WB

#define CVCHS2MIC_MONO_SEND_UWB_CAP_ID     CAP_ID_DOWNLOAD_CVCHS2MIC_MONO_SEND_UWB
#define CVCHS2MIC_MONO_SEND_SWB_CAP_ID     CAP_ID_DOWNLOAD_CVCHS2MIC_MONO_SEND_SWB
#define CVCHS2MIC_MONO_SEND_FB_CAP_ID      CAP_ID_DOWNLOAD_CVCHS2MIC_MONO_SEND_FB
#define CVCHS2MIC_BINAURAL_SEND_NB_CAP_ID  CAP_ID_DOWNLOAD_CVCHS2MIC_BINAURAL_SEND_NB
#define CVCHS2MIC_BINAURAL_SEND_WB_CAP_ID  CAP_ID_DOWNLOAD_CVCHS2MIC_BINAURAL_SEND_WB
#define CVCHS2MIC_BINAURAL_SEND_UWB_CAP_ID CAP_ID_DOWNLOAD_CVCHS2MIC_BINAURAL_SEND_UWB
#define CVCHS2MIC_BINAURAL_SEND_SWB_CAP_ID CAP_ID_DOWNLOAD_CVCHS2MIC_BINAURAL_SEND_SWB
#define CVCHS2MIC_BINAURAL_SEND_FB_CAP_ID  CAP_ID_DOWNLOAD_CVCHS2MIC_BINAURAL_SEND_FB
#define CVCHF1MIC_SEND_NB_CAP_ID           CAP_ID_DOWNLOAD_CVCHF1MIC_SEND_NB
#define CVCHF1MIC_SEND_WB_CAP_ID           CAP_ID_DOWNLOAD_CVCHF1MIC_SEND_WB
#define CVCHF1MIC_SEND_UWB_CAP_ID          CAP_ID_DOWNLOAD_CVCHF1MIC_SEND_UWB
#define CVCHF1MIC_SEND_SWB_CAP_ID          CAP_ID_DOWNLOAD_CVCHF1MIC_SEND_SWB
#define CVCHF1MIC_SEND_FB_CAP_ID           CAP_ID_DOWNLOAD_CVCHF1MIC_SEND_FB
#define CVCHF2MIC_SEND_NB_CAP_ID           CAP_ID_DOWNLOAD_CVCHF2MIC_SEND_NB
#define CVCHF2MIC_SEND_WB_CAP_ID           CAP_ID_DOWNLOAD_CVCHF2MIC_SEND_WB
#define CVCHF2MIC_SEND_UWB_CAP_ID          CAP_ID_DOWNLOAD_CVCHF2MIC_SEND_UWB
#define CVCHF2MIC_SEND_SWB_CAP_ID          CAP_ID_DOWNLOAD_CVCHF2MIC_SEND_SWB
#define CVCHF2MIC_SEND_FB_CAP_ID           CAP_ID_DOWNLOAD_CVCHF2MIC_SEND_FB
#define CVCSPKR1MIC_SEND_NB_CAP_ID         CAP_ID_DOWNLOAD_CVCSPKR1MIC_SEND_NB
#define CVCSPKR1MIC_SEND_WB_CAP_ID         CAP_ID_DOWNLOAD_CVCSPKR1MIC_SEND_WB
#define CVCSPKR1MIC_SEND_UWB_CAP_ID        CAP_ID_DOWNLOAD_CVCSPKR1MIC_SEND_UWB
#define CVCSPKR1MIC_SEND_SWB_CAP_ID        CAP_ID_DOWNLOAD_CVCSPKR1MIC_SEND_SWB
#define CVCSPKR1MIC_SEND_FB_CAP_ID         CAP_ID_DOWNLOAD_CVCSPKR1MIC_SEND_FB
#define CVCSPKR2MIC_SEND_NB_CAP_ID         CAP_ID_DOWNLOAD_CVCSPKR2MIC_SEND_NB
#define CVCSPKR2MIC_SEND_WB_CAP_ID         CAP_ID_DOWNLOAD_CVCSPKR2MIC_SEND_WB
#define CVCSPKR2MIC_SEND_UWB_CAP_ID        CAP_ID_DOWNLOAD_CVCSPKR2MIC_SEND_UWB
#define CVCSPKR2MIC_SEND_SWB_CAP_ID        CAP_ID_DOWNLOAD_CVCSPKR2MIC_SEND_SWB
#define CVCSPKR2MIC_SEND_FB_CAP_ID         CAP_ID_DOWNLOAD_CVCSPKR2MIC_SEND_FB
#define CVCSPKR3MIC_SEND_NB_CAP_ID         CAP_ID_DOWNLOAD_CVCSPKR3MIC_SEND_NB
#define CVCSPKR3MIC_SEND_WB_CAP_ID         CAP_ID_DOWNLOAD_CVCSPKR3MIC_SEND_WB
#define CVCSPKR3MIC_SEND_UWB_CAP_ID        CAP_ID_DOWNLOAD_CVCSPKR3MIC_SEND_UWB
#define CVCSPKR3MIC_SEND_SWB_CAP_ID        CAP_ID_DOWNLOAD_CVCSPKR3MIC_SEND_SWB
#define CVCSPKR3MIC_SEND_FB_CAP_ID         CAP_ID_DOWNLOAD_CVCSPKR3MIC_SEND_FB
#define CVCSPKR3MIC_CIRC_SEND_NB_CAP_ID    CAP_ID_DOWNLOAD_CVCSPKR3MIC_CIRC_SEND_NB
#define CVCSPKR3MIC_CIRC_SEND_WB_CAP_ID    CAP_ID_DOWNLOAD_CVCSPKR3MIC_CIRC_SEND_WB
#define CVCSPKR3MIC_CIRC_SEND_UWB_CAP_ID   CAP_ID_DOWNLOAD_CVCSPKR3MIC_CIRC_SEND_UWB
#define CVCSPKR3MIC_CIRC_SEND_SWB_CAP_ID   CAP_ID_DOWNLOAD_CVCSPKR3MIC_CIRC_SEND_SWB
#define CVCSPKR3MIC_CIRC_SEND_FB_CAP_ID    CAP_ID_DOWNLOAD_CVCSPKR3MIC_CIRC_SEND_FB

#define CVCSPKR3MIC_CIRC_VA_SEND_WB_CAP_ID      CAP_ID_DOWNLOAD_CVCSPKR3MIC_CIRC_VA_SEND_WB
#define CVCSPKR3MIC_CIRC_VA4B_SEND_WB_CAP_ID    CAP_ID_DOWNLOAD_CVCSPKR3MIC_CIRC_VA4B_SEND_WB

#define CVCSPKR4MIC_SEND_NB_CAP_ID         CAP_ID_DOWNLOAD_CVCSPKR4MIC_SEND_NB
#define CVCSPKR4MIC_SEND_WB_CAP_ID         CAP_ID_DOWNLOAD_CVCSPKR4MIC_SEND_WB
#define CVCSPKR4MIC_SEND_UWB_CAP_ID        CAP_ID_DOWNLOAD_CVCSPKR4MIC_SEND_UWB
#define CVCSPKR4MIC_SEND_SWB_CAP_ID        CAP_ID_DOWNLOAD_CVCSPKR4MIC_SEND_SWB
#define CVCSPKR4MIC_SEND_FB_CAP_ID         CAP_ID_DOWNLOAD_CVCSPKR4MIC_SEND_FB
#define CVCSPKR4MIC_CIRC_SEND_UWB_CAP_ID   CAP_ID_DOWNLOAD_CVCSPKR4MIC_CIRC_SEND_UWB
#define CVCSPKR4MIC_CIRC_SEND_SWB_CAP_ID   CAP_ID_DOWNLOAD_CVCSPKR4MIC_CIRC_SEND_SWB
#define CVCSPKR4MIC_CIRC_SEND_FB_CAP_ID    CAP_ID_DOWNLOAD_CVCSPKR4MIC_CIRC_SEND_FB

#define CVCEB2MIC_IE_WB_CAP_ID             CAP_ID_DOWNLOAD_CVCEB2MIC_IE_WB
#define CVCEB3MIC_MONO_IE_WB_CAP_ID        CAP_ID_DOWNLOAD_CVCEB3MIC_MONO_IE_WB
#define CVCEB2MIC_IE_NB_CAP_ID             CAP_ID_DOWNLOAD_CVCEB2MIC_IE_NB
#define CVCEB3MIC_MONO_IE_NB_CAP_ID        CAP_ID_DOWNLOAD_CVCEB3MIC_MONO_IE_NB

#else

#define CVCHS1MIC_SEND_NB_CAP_ID           CAP_ID_CVCHS1MIC_SEND_NB
#define CVCHS1MIC_SEND_WB_CAP_ID           CAP_ID_CVCHS1MIC_SEND_WB
#define CVCHS1MIC_SEND_UWB_CAP_ID          CAP_ID_CVCHS1MIC_SEND_UWB
#define CVCHS1MIC_SEND_SWB_CAP_ID          CAP_ID_CVCHS1MIC_SEND_SWB
#define CVCHS1MIC_SEND_FB_CAP_ID           CAP_ID_CVCHS1MIC_SEND_FB
#define CVCHS2MIC_MONO_SEND_NB_CAP_ID      CAP_ID_CVCHS2MIC_MONO_SEND_NB
#define CVCHS2MIC_MONO_SEND_WB_CAP_ID      CAP_ID_CVCHS2MIC_MONO_SEND_WB
#define CVCHS2MIC_WAKEON_WB_CAP_ID         CAP_ID_CVCHS2MIC_WAKEON_WB
#define CVCHS2MIC_BARGEIN_WB_CAP_ID        CAP_ID_CVCHS2MIC_BARGEIN_WB

#define CVCHS2MIC_MONO_SEND_UWB_CAP_ID     CAP_ID_CVCHS2MIC_MONO_SEND_UWB
#define CVCHS2MIC_MONO_SEND_SWB_CAP_ID     CAP_ID_CVCHS2MIC_MONO_SEND_SWB
#define CVCHS2MIC_MONO_SEND_FB_CAP_ID      CAP_ID_CVCHS2MIC_MONO_SEND_FB
#define CVCHS2MIC_BINAURAL_SEND_NB_CAP_ID  CAP_ID_CVCHS2MIC_BINAURAL_SEND_NB
#define CVCHS2MIC_BINAURAL_SEND_WB_CAP_ID  CAP_ID_CVCHS2MIC_BINAURAL_SEND_WB
#define CVCHS2MIC_BINAURAL_SEND_UWB_CAP_ID CAP_ID_CVCHS2MIC_BINAURAL_SEND_UWB
#define CVCHS2MIC_BINAURAL_SEND_SWB_CAP_ID CAP_ID_CVCHS2MIC_BINAURAL_SEND_SWB
#define CVCHS2MIC_BINAURAL_SEND_FB_CAP_ID  CAP_ID_CVCHS2MIC_BINAURAL_SEND_FB
#define CVCHF1MIC_SEND_NB_CAP_ID           CAP_ID_CVCHF1MIC_SEND_NB
#define CVCHF1MIC_SEND_WB_CAP_ID           CAP_ID_CVCHF1MIC_SEND_WB
#define CVCHF1MIC_SEND_UWB_CAP_ID          CAP_ID_CVCHF1MIC_SEND_UWB
#define CVCHF1MIC_SEND_SWB_CAP_ID          CAP_ID_CVCHF1MIC_SEND_SWB
#define CVCHF1MIC_SEND_FB_CAP_ID           CAP_ID_CVCHF1MIC_SEND_FB
#define CVCHF2MIC_SEND_NB_CAP_ID           CAP_ID_CVCHF2MIC_SEND_NB
#define CVCHF2MIC_SEND_WB_CAP_ID           CAP_ID_CVCHF2MIC_SEND_WB
#define CVCHF2MIC_SEND_UWB_CAP_ID          CAP_ID_CVCHF2MIC_SEND_UWB
#define CVCHF2MIC_SEND_SWB_CAP_ID          CAP_ID_CVCHF2MIC_SEND_SWB
#define CVCHF2MIC_SEND_FB_CAP_ID           CAP_ID_CVCHF2MIC_SEND_FB
#define CVCSPKR1MIC_SEND_NB_CAP_ID         CAP_ID_CVCSPKR1MIC_SEND_NB
#define CVCSPKR1MIC_SEND_WB_CAP_ID         CAP_ID_CVCSPKR1MIC_SEND_WB
#define CVCSPKR1MIC_SEND_UWB_CAP_ID        CAP_ID_CVCSPKR1MIC_SEND_UWB
#define CVCSPKR1MIC_SEND_SWB_CAP_ID        CAP_ID_CVCSPKR1MIC_SEND_SWB
#define CVCSPKR1MIC_SEND_FB_CAP_ID         CAP_ID_CVCSPKR1MIC_SEND_FB
#define CVCSPKR2MIC_SEND_NB_CAP_ID         CAP_ID_CVCSPKR2MIC_SEND_NB
#define CVCSPKR2MIC_SEND_WB_CAP_ID         CAP_ID_CVCSPKR2MIC_SEND_WB
#define CVCSPKR2MIC_SEND_UWB_CAP_ID        CAP_ID_CVCSPKR2MIC_SEND_UWB
#define CVCSPKR2MIC_SEND_SWB_CAP_ID        CAP_ID_CVCSPKR2MIC_SEND_SWB
#define CVCSPKR2MIC_SEND_FB_CAP_ID         CAP_ID_CVCSPKR2MIC_SEND_FB
#define CVCSPKR3MIC_SEND_NB_CAP_ID         CAP_ID_CVCSPKR3MIC_SEND_NB
#define CVCSPKR3MIC_SEND_WB_CAP_ID         CAP_ID_CVCSPKR3MIC_SEND_WB
#define CVCSPKR3MIC_SEND_UWB_CAP_ID        CAP_ID_CVCSPKR3MIC_SEND_UWB
#define CVCSPKR3MIC_SEND_SWB_CAP_ID        CAP_ID_CVCSPKR3MIC_SEND_SWB
#define CVCSPKR3MIC_SEND_FB_CAP_ID         CAP_ID_CVCSPKR3MIC_SEND_FB
#define CVCSPKR3MIC_CIRC_SEND_NB_CAP_ID    CAP_ID_CVCSPKR3MIC_CIRC_SEND_NB
#define CVCSPKR3MIC_CIRC_SEND_WB_CAP_ID    CAP_ID_CVCSPKR3MIC_CIRC_SEND_WB
#define CVCSPKR3MIC_CIRC_SEND_UWB_CAP_ID   CAP_ID_CVCSPKR3MIC_CIRC_SEND_UWB
#define CVCSPKR3MIC_CIRC_SEND_SWB_CAP_ID   CAP_ID_CVCSPKR3MIC_CIRC_SEND_SWB
#define CVCSPKR3MIC_CIRC_SEND_FB_CAP_ID    CAP_ID_CVCSPKR3MIC_CIRC_SEND_FB

#define CVCSPKR3MIC_CIRC_VA_SEND_WB_CAP_ID    CAP_ID_CVCSPKR3MIC_CIRC_VA_SEND_WB
#define CVCSPKR3MIC_CIRC_VA4B_SEND_WB_CAP_ID    CAP_ID_CVCSPKR3MIC_CIRC_VA4B_SEND_WB

#define CVCSPKR4MIC_SEND_NB_CAP_ID         CAP_ID_CVCSPKR4MIC_SEND_NB
#define CVCSPKR4MIC_SEND_WB_CAP_ID         CAP_ID_CVCSPKR4MIC_SEND_WB
#define CVCSPKR4MIC_SEND_UWB_CAP_ID        CAP_ID_CVCSPKR4MIC_SEND_UWB
#define CVCSPKR4MIC_SEND_SWB_CAP_ID        CAP_ID_CVCSPKR4MIC_SEND_SWB
#define CVCSPKR4MIC_SEND_FB_CAP_ID         CAP_ID_CVCSPKR4MIC_SEND_FB
#define CVCSPKR4MIC_CIRC_SEND_UWB_CAP_ID   CAP_ID_CVCSPKR4MIC_CIRC_SEND_UWB
#define CVCSPKR4MIC_CIRC_SEND_SWB_CAP_ID   CAP_ID_CVCSPKR4MIC_CIRC_SEND_SWB
#define CVCSPKR4MIC_CIRC_SEND_FB_CAP_ID    CAP_ID_CVCSPKR4MIC_CIRC_SEND_FB

#define CVCEB2MIC_IE_WB_CAP_ID             CAP_ID_CVCEB2MIC_IE_WB
#define CVCEB3MIC_MONO_IE_WB_CAP_ID        CAP_ID_CVCEB3MIC_MONO_IE_WB
#define CVCEB2MIC_IE_NB_CAP_ID             CAP_ID_CVCEB2MIC_IE_NB
#define CVCEB3MIC_MONO_IE_NB_CAP_ID        CAP_ID_CVCEB3MIC_MONO_IE_NB
#endif

/* Message handlers */

/** The cvc send capability function handler table */
const handler_lookup_struct cvc_send_handler_table =
{
    cvc_send_create,          /* OPCMD_CREATE */
    cvc_send_destroy,         /* OPCMD_DESTROY */
    base_op_start,            /* OPCMD_START */
    base_op_stop,             /* OPCMD_STOP */
    base_op_reset,            /* OPCMD_RESET */
    cvc_send_connect,         /* OPCMD_CONNECT */
    cvc_send_disconnect,      /* OPCMD_DISCONNECT */
    cvc_send_buffer_details,  /* OPCMD_BUFFER_DETAILS */
    base_op_get_data_format,  /* OPCMD_DATA_FORMAT */
    cvc_send_get_sched_info   /* OPCMD_GET_SCHED_INFO */
};

/* Null terminated operator message handler table */

const opmsg_handler_lookup_table_entry cvc_send_opmsg_handler_table[] =
    {{OPMSG_COMMON_ID_GET_CAPABILITY_VERSION,           base_op_opmsg_get_capability_version},
    {OPMSG_COMMON_ID_SET_CONTROL,                       cvc_send_opmsg_obpm_set_control},
    {OPMSG_COMMON_ID_GET_PARAMS,                        cvc_send_opmsg_obpm_get_params},
    {OPMSG_COMMON_ID_GET_DEFAULTS,                      cvc_send_opmsg_obpm_get_defaults},
    {OPMSG_COMMON_ID_SET_PARAMS,                        cvc_send_opmsg_obpm_set_params},
    {OPMSG_COMMON_ID_GET_STATUS,                        cvc_send_opmsg_obpm_get_status},

    {OPMSG_COMMON_ID_SET_UCID,                          cvc_send_opmsg_set_ucid},
    {OPMSG_COMMON_ID_GET_LOGICAL_PS_ID,                 cvc_send_opmsg_get_ps_id},
    {OPMSG_COMMON_GET_VOICE_QUALITY,                    cvc_send_opmsg_get_voice_quality},

    {0, NULL}};


/****************************************************************************
CVC send capability data declarations
*/
#ifdef INSTALL_OPERATOR_CVC_HEADSET_1MIC
    const CAPABILITY_DATA cvc_send_1mic_nb_hs_cap_data =
        {
            CVCHS1MIC_SEND_NB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_1M_HS_NB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            2, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS1MIC_SEND_NB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS1MIC_SEND_NB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

    const CAPABILITY_DATA cvc_send_1mic_wb_hs_cap_data =
        {
            CVCHS1MIC_SEND_WB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_1M_HS_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            2, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS1MIC_SEND_WB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS1MIC_SEND_WB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#ifdef INSTALL_OPERATOR_CVC_24K
    const CAPABILITY_DATA cvc_send_1mic_uwb_hs_cap_data =
        {
            CVCHS1MIC_SEND_UWB_CAP_ID,      /* Capability ID */
            GEN_CVC_SEND_1M_HS_UWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            2, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS1MIC_SEND_UWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS1MIC_SEND_UWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_32K
    const CAPABILITY_DATA cvc_send_1mic_swb_hs_cap_data =
        {
            CVCHS1MIC_SEND_SWB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_1M_HS_SWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            2, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS1MIC_SEND_SWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS1MIC_SEND_SWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif
#ifdef INSTALL_OPERATOR_CVC_48K
    const CAPABILITY_DATA cvc_send_1mic_fb_hs_cap_data =
        {
            CVCHS1MIC_SEND_FB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_1M_HS_FB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            2, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS1MIC_SEND_FB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS1MIC_SEND_FB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif
#endif

#ifdef INSTALL_OPERATOR_CVC_HEADSET_2MIC_MONO
    const CAPABILITY_DATA cvc_send_2mic_hs_mono_nb_cap_data =
        {
            CVCHS2MIC_MONO_SEND_NB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_HSE_NB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS2MIC_MONO_SEND_NB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS2MIC_MONO_SEND_NB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

    const CAPABILITY_DATA cvc_send_2mic_hs_mono_wb_cap_data =
        {
            CVCHS2MIC_MONO_SEND_WB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_HSE_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
     
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS2MIC_MONO_SEND_WB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS2MIC_MONO_SEND_WB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#ifdef INSTALL_OPERATOR_CVC_24K
    const CAPABILITY_DATA cvc_send_2mic_hs_mono_uwb_cap_data =
        {
            CVCHS2MIC_MONO_SEND_UWB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_HSE_UWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS2MIC_MONO_SEND_UWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS2MIC_MONO_SEND_UWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#endif
#ifdef INSTALL_OPERATOR_CVC_32K
    const CAPABILITY_DATA cvc_send_2mic_hs_mono_swb_cap_data =
        {
            CVCHS2MIC_MONO_SEND_SWB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_HSE_SWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS2MIC_MONO_SEND_SWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS2MIC_MONO_SEND_SWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#endif
#ifdef INSTALL_OPERATOR_CVC_48K
    const CAPABILITY_DATA cvc_send_2mic_hs_mono_fb_cap_data =
        {
            CVCHS2MIC_MONO_SEND_FB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_HSE_FB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS2MIC_MONO_SEND_FB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS2MIC_MONO_SEND_FB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif
#endif

#ifdef INSTALL_OPERATOR_CVC_HEADSET_BINAURAL
    const CAPABILITY_DATA cvc_send_2mic_hs_binaural_nb_cap_data =
        {
            CVCHS2MIC_BINAURAL_SEND_NB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_HSB_NB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS2MIC_BINAURAL_SEND_NB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS2MIC_BINAURAL_SEND_NB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

    const CAPABILITY_DATA cvc_send_2mic_hs_binaural_wb_cap_data =
        {
            CVCHS2MIC_BINAURAL_SEND_WB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_HSB_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS2MIC_BINAURAL_SEND_WB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS2MIC_BINAURAL_SEND_WB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#ifdef INSTALL_OPERATOR_CVC_24K
    const CAPABILITY_DATA cvc_send_2mic_hs_binaural_uwb_cap_data =
        {
            CVCHS2MIC_BINAURAL_SEND_UWB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_HSB_UWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS2MIC_BINAURAL_SEND_UWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS2MIC_BINAURAL_SEND_UWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_32K
    const CAPABILITY_DATA cvc_send_2mic_hs_binaural_swb_cap_data =
        {
            CVCHS2MIC_BINAURAL_SEND_SWB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_HSB_SWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS2MIC_BINAURAL_SEND_SWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS2MIC_BINAURAL_SEND_SWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_48K
    const CAPABILITY_DATA cvc_send_2mic_hs_binaural_fb_cap_data =
        {
            CVCHS2MIC_BINAURAL_SEND_FB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_HSB_FB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS2MIC_BINAURAL_SEND_FB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS2MIC_BINAURAL_SEND_FB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif
#endif

#ifdef INSTALL_OPERATOR_CVC_AUTO_1MIC
    const CAPABILITY_DATA cvc_send_1mic_nb_auto_cap_data =
        {
            CVCHF1MIC_SEND_NB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_1M_AUTO_NB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            2, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHF1MIC_SEND_NB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHF1MIC_SEND_NB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

    const CAPABILITY_DATA cvc_send_1mic_wb_auto_cap_data =
        {
            CVCHF1MIC_SEND_WB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_1M_AUTO_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            2, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHF1MIC_SEND_WB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHF1MIC_SEND_WB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#ifdef INSTALL_OPERATOR_CVC_24K
    const CAPABILITY_DATA cvc_send_1mic_uwb_auto_cap_data =
        {
            CVCHF1MIC_SEND_UWB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_1M_AUTO_UWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            2, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHF1MIC_SEND_UWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHF1MIC_SEND_UWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#endif
#ifdef INSTALL_OPERATOR_CVC_32K
    const CAPABILITY_DATA cvc_send_1mic_swb_auto_cap_data =
        {
            CVCHF1MIC_SEND_SWB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_1M_AUTO_SWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            2, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHF1MIC_SEND_SWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHF1MIC_SEND_SWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#endif
#ifdef INSTALL_OPERATOR_CVC_48K
    const CAPABILITY_DATA cvc_send_1mic_fb_auto_cap_data =
        {
            CVCHF1MIC_SEND_FB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_1M_AUTO_FB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            2, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHF1MIC_SEND_FB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHF1MIC_SEND_FB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif
#endif

#ifdef INSTALL_OPERATOR_CVC_AUTO_2MIC
    const CAPABILITY_DATA cvc_send_2mic_nb_auto_cap_data =
        {
            CVCHF2MIC_SEND_NB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_2M_AUTO_NB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHF2MIC_SEND_NB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHF2MIC_SEND_NB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

    const CAPABILITY_DATA cvc_send_2mic_wb_auto_cap_data =
        {
            CVCHF2MIC_SEND_WB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_2M_AUTO_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHF2MIC_SEND_WB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHF2MIC_SEND_WB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#ifdef INSTALL_OPERATOR_CVC_24K
    const CAPABILITY_DATA cvc_send_2mic_uwb_auto_cap_data =
        {
            CVCHF2MIC_SEND_UWB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_2M_AUTO_UWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHF2MIC_SEND_UWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHF2MIC_SEND_UWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_32K
    const CAPABILITY_DATA cvc_send_2mic_swb_auto_cap_data =
        {
            CVCHF2MIC_SEND_SWB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_2M_AUTO_SWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHF2MIC_SEND_SWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHF2MIC_SEND_SWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_48K
    const CAPABILITY_DATA cvc_send_2mic_fb_auto_cap_data =
        {
            CVCHF2MIC_SEND_FB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_2M_AUTO_FB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHF2MIC_SEND_FB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHF2MIC_SEND_FB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif
#endif

#ifdef INSTALL_OPERATOR_CVC_SPEAKER_1MIC
    const CAPABILITY_DATA cvc_send_1mic_speaker_nb_cap_data =
        {
            CVCSPKR1MIC_SEND_NB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_1M_SPKR_NB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            2, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR1MIC_SEND_NB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR1MIC_SEND_NB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

    const CAPABILITY_DATA cvc_send_1mic_speaker_wb_cap_data =
        {
            CVCSPKR1MIC_SEND_WB_CAP_ID,          /* Capability ID */
            GEN_CVC_SEND_1M_SPKR_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            2, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR1MIC_SEND_WB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR1MIC_SEND_WB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#ifdef INSTALL_OPERATOR_CVC_24K
    const CAPABILITY_DATA cvc_send_1mic_speaker_uwb_cap_data =
        {
            CVCSPKR1MIC_SEND_UWB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_1M_SPKR_UWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            2, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR1MIC_SEND_UWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR1MIC_SEND_UWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_32K
    const CAPABILITY_DATA cvc_send_1mic_speaker_swb_cap_data =
        {
            CVCSPKR1MIC_SEND_SWB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_1M_SPKR_SWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            2, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR1MIC_SEND_SWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR1MIC_SEND_SWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_48K
    const CAPABILITY_DATA cvc_send_1mic_speaker_fb_cap_data =
        {
            CVCSPKR1MIC_SEND_FB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_1M_SPKR_FB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            2, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR1MIC_SEND_FB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR1MIC_SEND_FB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif
#endif

#ifdef INSTALL_OPERATOR_CVC_SPEAKER_2MIC
    const CAPABILITY_DATA cvc_send_2mic_speaker_nb_cap_data =
        {
            CVCSPKR2MIC_SEND_NB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_SPKRB_NB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR2MIC_SEND_NB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR2MIC_SEND_NB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

    const CAPABILITY_DATA cvc_send_2mic_speaker_wb_cap_data =
        {
            CVCSPKR2MIC_SEND_WB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_SPKRB_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR2MIC_SEND_WB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR2MIC_SEND_WB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#ifdef INSTALL_OPERATOR_CVC_24K
    const CAPABILITY_DATA cvc_send_2mic_speaker_uwb_cap_data =
        {
            CVCSPKR2MIC_SEND_UWB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_SPKRB_UWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR2MIC_SEND_UWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR2MIC_SEND_UWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_32K
    const CAPABILITY_DATA cvc_send_2mic_speaker_swb_cap_data =
        {
            CVCSPKR2MIC_SEND_SWB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_SPKRB_SWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR2MIC_SEND_SWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR2MIC_SEND_SWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_48K
    const CAPABILITY_DATA cvc_send_2mic_speaker_fb_cap_data =
        {
            CVCSPKR2MIC_SEND_FB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_SPKRB_FB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR2MIC_SEND_FB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR2MIC_SEND_FB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif
#endif

#ifdef INSTALL_OPERATOR_CVC_SPEAKER_3MIC
    const CAPABILITY_DATA cvc_send_3mic_speaker_nb_cap_data =
        {
            CVCSPKR3MIC_SEND_NB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_3M_SPKRB_NB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            4, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR3MIC_SEND_NB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR3MIC_SEND_NB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

    const CAPABILITY_DATA cvc_send_3mic_speaker_wb_cap_data =
        {
            CVCSPKR3MIC_SEND_WB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_3M_SPKRB_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            4, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR3MIC_SEND_WB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR3MIC_SEND_WB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#ifdef INSTALL_OPERATOR_CVC_24K
    const CAPABILITY_DATA cvc_send_3mic_speaker_uwb_cap_data =
        {
            CVCSPKR3MIC_SEND_UWB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_3M_SPKRB_UWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            4, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR3MIC_SEND_UWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR3MIC_SEND_UWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_32K
    const CAPABILITY_DATA cvc_send_3mic_speaker_swb_cap_data =
        {
            CVCSPKR3MIC_SEND_SWB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_3M_SPKRB_SWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            4, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR3MIC_SEND_SWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR3MIC_SEND_SWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_48K
    const CAPABILITY_DATA cvc_send_3mic_speaker_fb_cap_data =
        {
            CVCSPKR3MIC_SEND_FB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_3M_SPKRB_FB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            4, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR3MIC_SEND_FB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR3MIC_SEND_FB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif


#endif

#ifdef INSTALL_OPERATOR_CVC_SPEAKER_4MIC
    const CAPABILITY_DATA cvc_send_4mic_speaker_nb_cap_data =
        {
            CVCSPKR4MIC_SEND_NB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_4M_SPKRB_NB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            5, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR4MIC_SEND_NB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR4MIC_SEND_NB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

    const CAPABILITY_DATA cvc_send_4mic_speaker_wb_cap_data =
        {
            CVCSPKR4MIC_SEND_WB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_4M_SPKRB_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            5, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR4MIC_SEND_WB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR4MIC_SEND_WB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#ifdef INSTALL_OPERATOR_CVC_24K
    const CAPABILITY_DATA cvc_send_4mic_speaker_uwb_cap_data =
        {
            CVCSPKR4MIC_SEND_UWB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_4M_SPKRB_UWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            5, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR4MIC_SEND_UWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR4MIC_SEND_UWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#endif
#ifdef INSTALL_OPERATOR_CVC_32K
    const CAPABILITY_DATA cvc_send_4mic_speaker_swb_cap_data =
        {
            CVCSPKR4MIC_SEND_SWB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_4M_SPKRB_SWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            5, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR4MIC_SEND_SWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR4MIC_SEND_SWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#endif
#ifdef INSTALL_OPERATOR_CVC_48K
    const CAPABILITY_DATA cvc_send_4mic_speaker_fb_cap_data =
        {
            CVCSPKR4MIC_SEND_FB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_4M_SPKRB_FB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            5, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR4MIC_SEND_FB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR4MIC_SEND_FB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif


#endif

#ifdef INSTALL_OPERATOR_CVC_SPKR_CIRC_3MIC
    const CAPABILITY_DATA cvc_send_3mic_circ_speaker_wb_cap_data =
        {
            CVCSPKR3MIC_CIRC_SEND_WB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_3M_SPKRCIRC_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            4, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR3MIC_CIRC_SEND_WB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR3MIC_CIRC_SEND_WB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */

#ifdef INSTALL_OPERATOR_CVC_24K
    const CAPABILITY_DATA cvc_send_3mic_circ_speaker_uwb_cap_data =
        {
            CVCSPKR3MIC_CIRC_SEND_UWB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_3M_SPKRCIRC_UWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            4, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR3MIC_CIRC_SEND_UWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR3MIC_CIRC_SEND_UWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_32K
    const CAPABILITY_DATA cvc_send_3mic_circ_speaker_swb_cap_data =
        {
            CVCSPKR3MIC_CIRC_SEND_SWB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_3M_SPKRCIRC_SWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            4, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR3MIC_CIRC_SEND_SWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR3MIC_CIRC_SEND_SWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_48K
    const CAPABILITY_DATA cvc_send_3mic_circ_speaker_fb_cap_data =
        {
            CVCSPKR3MIC_CIRC_SEND_FB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_3M_SPKRCIRC_FB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            4, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR3MIC_CIRC_SEND_FB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR3MIC_CIRC_SEND_FB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif
#endif /* INSTALL_OPERATOR_CVC_SPKR_CIRC_3MIC */

#ifdef INSTALL_OPERATOR_CVC_SPKR_CIRC_4MIC
#ifdef INSTALL_OPERATOR_CVC_24K
    const CAPABILITY_DATA cvc_send_4mic_circ_speaker_uwb_cap_data =
        {
            CVCSPKR4MIC_CIRC_SEND_UWB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_4M_SPKRCIRC_UWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            5, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR4MIC_CIRC_SEND_UWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR4MIC_CIRC_SEND_UWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_32K
    const CAPABILITY_DATA cvc_send_4mic_circ_speaker_swb_cap_data =
        {
            CVCSPKR4MIC_CIRC_SEND_SWB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_4M_SPKRCIRC_SWB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            5, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR4MIC_CIRC_SEND_SWB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR4MIC_CIRC_SEND_SWB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_48K
    const CAPABILITY_DATA cvc_send_4mic_circ_speaker_fb_cap_data =
        {
            CVCSPKR4MIC_CIRC_SEND_FB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_4M_SPKRCIRC_FB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            5, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR4MIC_CIRC_SEND_FB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR4MIC_CIRC_SEND_FB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif
#endif /* INSTALL_OPERATOR_CVC_SPKR_CIRC_4MIC */

#ifdef INSTALL_OPERATOR_CVC_SPKR_CIRC_3MIC_VA
    const CAPABILITY_DATA cvc_send_3mic_circ_speaker_va_wb_cap_data =
        {
            CVCSPKR3MIC_CIRC_VA_SEND_WB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_3M_SPKRCIRC_VA_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            4, 5,                            /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR3MIC_CIRC_VA_SEND_WB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR3MIC_CIRC_VA_SEND_WB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_SPKR_CIRC_3MIC_VA4B
     const CAPABILITY_DATA cvc_send_3mic_circ_speaker_va4b_wb_cap_data =
        {
            CVCSPKR3MIC_CIRC_VA4B_SEND_WB_CAP_ID,                                                       /* Capability ID */
            GEN_CVC_SEND_3M_SPKRCIRC_VA4B_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            4, 5,                            /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCSPKR3MIC_CIRC_VA4B_SEND_WB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCSPKR3MIC_CIRC_VA4B_SEND_WB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_HEADSET_2MIC_MONO_VA_WAKEON
    const CAPABILITY_DATA cvc_send_2mic_hs_mono_wb_va_wakeon_cap_data =
        {
            CVCHS2MIC_WAKEON_WB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_HSE_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS2MIC_WAKEON_WB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS2MIC_WAKEON_WB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif

#ifdef INSTALL_OPERATOR_CVC_HEADSET_2MIC_MONO_VA_BARGEIN
    const CAPABILITY_DATA cvc_send_2mic_hs_mono_wb_va_bargein_cap_data =
        {
            CVCHS2MIC_BARGEIN_WB_CAP_ID,       /* Capability ID */
            GEN_CVC_SEND_2M_HSE_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };
#if !defined(CAPABILITY_DOWNLOAD_BUILD)
MAP_INSTANCE_DATA(CAP_ID_CVCHS2MIC_BARGEIN_WB, CVC_SEND_OP_DATA)
#else
MAP_INSTANCE_DATA(CAP_ID_DOWNLOAD_CVCHS2MIC_BARGEIN_WB, CVC_SEND_OP_DATA)
#endif /* CAPABILITY_DOWNLOAD_BUILD */
#endif
    
typedef void (*cvc_send_config_function_type)(CVC_SEND_OP_DATA *op_extra_data, unsigned data_variant);

typedef struct cvc_send_config_data
{
    cvc_send_config_function_type config_func;
    unsigned cap_ids[NUM_DATA_VARIANTS];
} cvc_send_config_data;

#ifdef INSTALL_OPERATOR_CVC_EARBUD_2MIC_IE
    const CAPABILITY_DATA cvc_send_earbud_2mic_wb_ie_cap_data =
        {
            CVCEB2MIC_IE_WB_CAP_ID,         /* Capability ID */
            GEN_CVC_SEND_2M_EB_IE_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,             /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,        /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table,   /* Pointer to operator message handler function table */
            cvc_send_process_data,          /* Pointer to data processing function */
            0,                              /* Reserved */
            sizeof(CVC_SEND_OP_DATA)        /* Size of capability-specific per-instance data */
        };

    MAP_INSTANCE_DATA(CVCEB2MIC_IE_WB_CAP_ID, CVC_SEND_OP_DATA)

    const CAPABILITY_DATA cvc_send_earbud_2mic_nb_ie_cap_data =
        {
            CVCEB2MIC_IE_NB_CAP_ID,         /* Capability ID */
            GEN_CVC_SEND_2M_EB_IE_NB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            3, CVC_NUM_OUTPUTS,             /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,        /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table,   /* Pointer to operator message handler function table */
            cvc_send_process_data,          /* Pointer to data processing function */
            0,                              /* Reserved */
            sizeof(CVC_SEND_OP_DATA)        /* Size of capability-specific per-instance data */
        };
     MAP_INSTANCE_DATA(CVCEB2MIC_IE_NB_CAP_ID, CVC_SEND_OP_DATA)
     
#endif

#ifdef INSTALL_OPERATOR_CVC_EARBUD_3MIC_MONO_IE
    const CAPABILITY_DATA cvc_send_earbud_3mic_mono_ie_cap_data =
        {
            CVCEB3MIC_MONO_IE_WB_CAP_ID,  /* Capability ID */
            GEN_CVC_SEND_3M_EBE_IE_WB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            4, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };

    MAP_INSTANCE_DATA(CVCEB3MIC_MONO_IE_WB_CAP_ID, CVC_SEND_OP_DATA)

    const CAPABILITY_DATA cvc_send_earbud_3mic_mono_nb_ie_cap_data =
        {
            CVCEB3MIC_MONO_IE_NB_CAP_ID,  /* Capability ID */
            GEN_CVC_SEND_3M_EBE_IE_NB_VERSION_MAJOR, CVC_SEND_CAP_VERSION_MINOR, /* Version information - hi and lo parts */
            4, CVC_NUM_OUTPUTS,           /* Max number of sinks/inputs and sources/outputs */
            &cvc_send_handler_table,      /* Pointer to message handler function table */
            cvc_send_opmsg_handler_table, /* Pointer to operator message handler function table */
            cvc_send_process_data,        /* Pointer to data processing function */
            0,                               /* Reserved */
            sizeof(CVC_SEND_OP_DATA)      /* Size of capability-specific per-instance data */
        };

    MAP_INSTANCE_DATA(CVCEB3MIC_MONO_IE_NB_CAP_ID, CVC_SEND_OP_DATA)

#endif

/*
 *   cVc send capability configuration table:
 *       cap1_config_func(), cap1_id_nb, cap1_id_wb, cap1_id_uwb, cap1_id_swb, cap1_id_fb,
 *       cap2_config_func(), cap2_id_nb, cap2_id_wb, cap2_id_uwb, cap2_id_swb, cap2_id_fb,
 *       ...
 *       0;
 */
cvc_send_config_data cvc_send_caps[] = {
#ifdef INSTALL_OPERATOR_CVC_HEADSET_1MIC
    {
        &CVC_SEND_CAP_Config_headset_1mic,
        {
            CVCHS1MIC_SEND_NB_CAP_ID,  CVCHS1MIC_SEND_WB_CAP_ID,
            CVCHS1MIC_SEND_UWB_CAP_ID, CVCHS1MIC_SEND_SWB_CAP_ID, CVCHS1MIC_SEND_FB_CAP_ID
        }
    },
#endif

#ifdef INSTALL_OPERATOR_CVC_HEADSET_2MIC_MONO
    {
        &CVC_SEND_CAP_Config_headset_2mic_mono,
        {
            CVCHS2MIC_MONO_SEND_NB_CAP_ID,   CVCHS2MIC_MONO_SEND_WB_CAP_ID,
            CVCHS2MIC_MONO_SEND_UWB_CAP_ID,  CVCHS2MIC_MONO_SEND_SWB_CAP_ID,  CVCHS2MIC_MONO_SEND_FB_CAP_ID
        }
    },
#endif

#ifdef INSTALL_OPERATOR_CVC_HEADSET_BINAURAL
    {
        &CVC_SEND_CAP_Config_headset_2mic_binaural,
        {
            CVCHS2MIC_BINAURAL_SEND_NB_CAP_ID,  CVCHS2MIC_BINAURAL_SEND_WB_CAP_ID,
            CVCHS2MIC_BINAURAL_SEND_UWB_CAP_ID, CVCHS2MIC_BINAURAL_SEND_SWB_CAP_ID, CVCHS2MIC_BINAURAL_SEND_FB_CAP_ID
        }
    },
#endif

#ifdef INSTALL_OPERATOR_CVC_SPEAKER_1MIC
    {
        &CVC_SEND_CAP_Config_speaker_1mic,
        {
            CVCSPKR1MIC_SEND_NB_CAP_ID,   CVCSPKR1MIC_SEND_WB_CAP_ID,
            CVCSPKR1MIC_SEND_UWB_CAP_ID,  CVCSPKR1MIC_SEND_SWB_CAP_ID,  CVCSPKR1MIC_SEND_FB_CAP_ID
        }
    },
#endif

#ifdef INSTALL_OPERATOR_CVC_SPEAKER_2MIC
    {
        &CVC_SEND_CAP_Config_speaker_2mic,
        {
            CVCSPKR2MIC_SEND_NB_CAP_ID,   CVCSPKR2MIC_SEND_WB_CAP_ID,
            CVCSPKR2MIC_SEND_UWB_CAP_ID,  CVCSPKR2MIC_SEND_SWB_CAP_ID,  CVCSPKR2MIC_SEND_FB_CAP_ID
        }
    },
#endif

#ifdef INSTALL_OPERATOR_CVC_SPEAKER_3MIC
    {
        &CVC_SEND_CAP_Config_speaker_3mic,
        {
            CVCSPKR3MIC_SEND_NB_CAP_ID,   CVCSPKR3MIC_SEND_WB_CAP_ID,
            CVCSPKR3MIC_SEND_UWB_CAP_ID,  CVCSPKR3MIC_SEND_SWB_CAP_ID,  CVCSPKR3MIC_SEND_FB_CAP_ID
        }
    },
#endif

#ifdef INSTALL_OPERATOR_CVC_SPEAKER_4MIC
    {
        &CVC_SEND_CAP_Config_speaker_4mic,
        {
            CVCSPKR4MIC_SEND_NB_CAP_ID,   CVCSPKR4MIC_SEND_WB_CAP_ID,
            CVCSPKR4MIC_SEND_UWB_CAP_ID,  CVCSPKR4MIC_SEND_SWB_CAP_ID,  CVCSPKR4MIC_SEND_FB_CAP_ID
        }
    },
#endif

#ifdef INSTALL_OPERATOR_CVC_AUTO_1MIC
    {
        &CVC_SEND_CAP_Config_auto_1mic,
        {
            CVCHF1MIC_SEND_NB_CAP_ID,  CVCHF1MIC_SEND_WB_CAP_ID,
            CVCHF1MIC_SEND_UWB_CAP_ID, CVCHF1MIC_SEND_SWB_CAP_ID, CVCHF1MIC_SEND_FB_CAP_ID
        }
    },
#endif

#ifdef INSTALL_OPERATOR_CVC_AUTO_2MIC
    {
        &CVC_SEND_CAP_Config_auto_2mic,
        {
            CVCHF2MIC_SEND_NB_CAP_ID,  CVCHF2MIC_SEND_WB_CAP_ID,
            CVCHF2MIC_SEND_UWB_CAP_ID, CVCHF2MIC_SEND_SWB_CAP_ID, CVCHF2MIC_SEND_FB_CAP_ID
        }
    },
#endif

#ifdef INSTALL_OPERATOR_CVC_SPKR_CIRC_3MIC
    {
        &CVC_SEND_CAP_Config_spkr_circ_3mic,
        {
            0,  CVCSPKR3MIC_CIRC_SEND_WB_CAP_ID,
            0, 0, 0
        }
    }, 
#endif

#ifdef INSTALL_OPERATOR_CVC_SPKR_CIRC_3MIC_VA
    {
        &CVC_SEND_CAP_Config_spkr_circ_3mic_va,
        {
            0,  CVCSPKR3MIC_CIRC_VA_SEND_WB_CAP_ID,
            0, 0, 0
        }
    }, 
#endif

#ifdef INSTALL_OPERATOR_CVC_SPKR_CIRC_3MIC_VA4B
    {
        &CVC_SEND_CAP_Config_spkr_circ_3mic_va4b,
        {
            0,  CVCSPKR3MIC_CIRC_VA4B_SEND_WB_CAP_ID,
            0, 0, 0
        }
    }, 
#endif

#ifdef INSTALL_OPERATOR_CVC_HEADSET_2MIC_MONO_VA_WAKEON
    {
        &CVC_SEND_CAP_Config_headset_2mic_wakeon,
        {
            0,   CVCHS2MIC_WAKEON_WB_CAP_ID,
            0,  0,  0
        }
     },
#endif
       
#ifdef INSTALL_OPERATOR_CVC_HEADSET_2MIC_MONO_VA_BARGEIN
    {
        &CVC_SEND_CAP_Config_headset_2mic_bargein,
        {
            0,   CVCHS2MIC_BARGEIN_WB_CAP_ID,
            0,  0,  0
        }
     },
#endif

#ifdef INSTALL_OPERATOR_CVC_EARBUD_2MIC_IE
    {
        &CVC_SEND_CAP_Config_earbud_2mic_ie,
        {
            CVCEB2MIC_IE_NB_CAP_ID,  CVCEB2MIC_IE_WB_CAP_ID,
            0,  0,  0
        }
    },
#endif

#ifdef INSTALL_OPERATOR_CVC_EARBUD_3MIC_MONO_IE
    {
        &CVC_SEND_CAP_Config_earbud_3mic_ie,
        {
            CVCEB3MIC_MONO_IE_NB_CAP_ID,  CVCEB3MIC_MONO_IE_WB_CAP_ID,
            0,  0,  0
        }
    },
#endif

    {
        /* end of table */
        NULL, {0, 0, 0, 0, 0}
    }
};


/*
 * cvc_send_config(CVC_SEND_OP_DATA *op_extra_data)
 *    Search for op_extra_data->cap_id from cvc_send_caps[] table, if found
 *
 *       set the following field based on specified cap_id:
 *          op_extra_data->data_variant
 *          op_extra_data->major_config
 *          op_extra_data->num_mics
 *          op_extra_data->mic_config
 *          op_extra_data->frame_size
 *          op_extra_data->sample_rate
 *
 *       and initialize other internal fields
 */
static bool cvc_send_config(CVC_SEND_OP_DATA *op_extra_data)
{
    cvc_send_config_data *caps;
    unsigned cap_id = op_extra_data->cap_id;
    unsigned variant = 0;

    for (caps = cvc_send_caps; caps->config_func != NULL; caps++)
    {
        for (variant = DATA_VARIANT_NB; variant <= DATA_VARIANT_FB; variant++)
        {
            if (caps->cap_ids[variant] == cap_id)
            {
                caps->config_func(op_extra_data, variant);

                switch(op_extra_data->data_variant)
                {
                case DATA_VARIANT_WB:  // 16 kHz
                    op_extra_data->frame_size = 120;
                    op_extra_data->sample_rate = 16000;
                    break;
                case DATA_VARIANT_UWB: // 24 kHz
                    op_extra_data->frame_size = 120;
                    op_extra_data->sample_rate = 24000;
                    break;
                case DATA_VARIANT_SWB: // 32 kHz
                    op_extra_data->frame_size = 240;
                    op_extra_data->sample_rate = 32000;
                    break;
                case DATA_VARIANT_FB:  // 48 kHz
                    op_extra_data->frame_size = 240;
                    op_extra_data->sample_rate = 48000;
                    break;
                case DATA_VARIANT_NB:  // 8 kHz
                default:
                    op_extra_data->frame_size = 60;
                    op_extra_data->sample_rate = 8000;
                    break;
                }
                op_extra_data->ReInitFlag = 1;
                op_extra_data->Host_mode = GEN_CVC_SEND_SYSMODE_FULL;
                op_extra_data->Cur_mode = GEN_CVC_SEND_SYSMODE_STANDBY;
                op_extra_data->mute_control_ptr = &op_extra_data->Cur_Mute;

                return TRUE;
            }
        }
    }

   return FALSE;
}

static inline CVC_SEND_OP_DATA *get_instance_data(OPERATOR_DATA *op_data)
{
    return (CVC_SEND_OP_DATA *) base_op_get_instance_data(op_data);
}

/****************************************************************************
Public Function Declarations
*/


/* ********************************** API functions ************************************* */

bool ups_state_snd(void* instance_data,PS_KEY_TYPE key,PERSISTENCE_RANK rank,
                 uint16 length, unsigned* data, STATUS_KYMERA status,uint16 extra_status_info)
{
    CVC_SEND_OP_DATA   *op_extra_data = get_instance_data((OPERATOR_DATA*)instance_data);

    L2_DBG_MSG4("ups_state_snd len=%d rank=%d status=%d extra_info=%d \n", length, (unsigned)rank, (unsigned)status, extra_status_info );
    if((length==2)&&(status==STATUS_OK))
    {
        op_extra_data->mdgc_gain = ((data[0]&0xFFFF)<<16) | (data[1]&0xFFFF);
        /* Set the Reinit flag after setting the paramters */
        op_extra_data->ReInitFlag = 1;
    }

    return(TRUE);
}

bool ups_state_snd_eqmap(void* instance_data,PS_KEY_TYPE key,PERSISTENCE_RANK rank,
                 uint16 length, unsigned* data, STATUS_KYMERA status,uint16 extra_status_info)
{
    CVC_SEND_OP_DATA   *op_extra_data = get_instance_data((OPERATOR_DATA*)instance_data);
    L2_DBG_MSG4("ups_state_snd_eqmap len=%d rank=%d status=%d extra_info=%d \n", length, (unsigned)rank, (unsigned)status, extra_status_info );
    if((length==DATA_VARIANT_NB_NBINS*2 || length==DATA_VARIANT_WB_NBINS*2)&&(status==STATUS_OK))
    {
         /* unpack 2*nbins half-words (small endian format) */
         for (unsigned i=0; i<length/2; i++)
		 {
             op_extra_data->ptr_map_ratio[i] = (data[2*i] & 0xFFFF)  | ( (data[2*i+1] & 0xFFFF)<<16) ;
         }
         op_extra_data->ReInitFlag = 1;
    }
    return(TRUE);
}

#if defined(INSTALL_OPERATOR_CREATE_PENDING) && defined(INSTALL_CAPABILITY_CONSTANT_EXPORT)
void cvc_send_create_pending_cb(OPERATOR_DATA *op_data,
                                uint16 cmd_id, void *msg_body,
                                pendingContext *context, unsigned cb_value)
{
    external_constant_callback_when_available(op_data, (void*)cb_value, cmd_id,
                                              msg_body, context);
}

#endif

void cvc_send_release_constants(OPERATOR_DATA *op_data)
{
#if defined(INSTALL_OPERATOR_CREATE_PENDING) && defined(INSTALL_CAPABILITY_CONSTANT_EXPORT)
    CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);
    INT_OP_ID int_id = base_op_get_int_op_id(op_data);

    external_constant_release(cvclib_dataDynTable_Main, int_id);
    external_constant_release(aec520_DynamicMemDynTable_Main, int_id);
    external_constant_release(ASF100_DynamicMemDynTable_Main, int_id);
    external_constant_release(oms280_DynamicMemDynTable_Main, int_id);
    external_constant_release(filter_bank_DynamicMemDynTable_Main, int_id);
    external_constant_release(vad410_DynamicMemDynTable_Main, int_id);
    external_constant_release(op_extra_data->dyn_main, int_id);
#else
    NOT_USED(op_data);
#endif
}

bool cvc_send_create(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);
    PS_KEY_TYPE key;

    patch_fn_shared(cvc_send_wrapper);
    /* Setup Response to Creation Request.   Assume Failure*/
    if (!base_op_build_std_response_ex(op_data, STATUS_CMD_FAILED, response_data))
    {
        /* We call cvc_send_release_constants as there is a slim chance we fail on
         * the second pass through */
        cvc_send_release_constants(op_data);
        return(FALSE);
    }

    /* Initialize extended data for operator.  Assume intialized to zero*/
    op_extra_data->cap_id = base_op_get_cap_id(op_data);

    /* Capability Specific Configuration */
    if (FALSE == cvc_send_config(op_extra_data)) {
        return(TRUE);
    }

#if defined(INSTALL_OPERATOR_CREATE_PENDING) && defined(INSTALL_CAPABILITY_CONSTANT_EXPORT)
    /* Reserve (and request) any dynamic memory tables that may be in external
     * file system.
     * A negative return value indicates a fatal error */
    INT_OP_ID int_id = base_op_get_int_op_id(op_data);
    if (   !external_constant_reserve(cvclib_dataDynTable_Main, int_id)
        || !external_constant_reserve(aec520_DynamicMemDynTable_Main, int_id)
        || !external_constant_reserve(ASF100_DynamicMemDynTable_Main, int_id)
        || !external_constant_reserve(oms280_DynamicMemDynTable_Main, int_id)
        || !external_constant_reserve(filter_bank_DynamicMemDynTable_Main, int_id)
        || !external_constant_reserve(vad410_DynamicMemDynTable_Main, int_id)
        || !external_constant_reserve(op_extra_data->dyn_main, int_id))
    {
        L2_DBG_MSG("cvc_send_create failed reserving constants");
        cvc_send_release_constants(op_data);
        return TRUE;
    }

    /* Now see if these tables are available yet */
    if (   !is_external_constant_available(cvclib_dataDynTable_Main, int_id)
        || !is_external_constant_available(aec520_DynamicMemDynTable_Main, int_id)
        || !is_external_constant_available(ASF100_DynamicMemDynTable_Main, int_id)
        || !is_external_constant_available(oms280_DynamicMemDynTable_Main, int_id)
        || !is_external_constant_available(filter_bank_DynamicMemDynTable_Main, int_id)
        || !is_external_constant_available(vad410_DynamicMemDynTable_Main, int_id)
        || !is_external_constant_available(op_extra_data->dyn_main, int_id))
    {
        /* Free the response created above, before it gets overwritten with the pending data */
        pdelete(*response_data);

        /* Database isn't available yet. Arrange for a callback
         * Only need to check on one table */
        *response_id = (unsigned)op_extra_data->dyn_main;
        *response_data = (void*)(pending_operator_cb)cvc_send_create_pending_cb;

        L4_DBG_MSG("cvc_send_create - requesting callback when constants available");
        return (bool)HANDLER_INCOMPLETE;
    }
#endif

    patch_fn_shared(cvc_send_wrapper);


    /*allocate the volume control shared memory*/
    op_extra_data->shared_volume_ptr = allocate_shared_volume_cntrl();
    if(!op_extra_data->shared_volume_ptr)
    {
        cvc_send_release_constants(op_data);
        return(TRUE);
    }

    /* call the "create" assembly function */
    if(CVC_SEND_CAP_Create(op_extra_data))
    {
        /* Free all the scratch memory we reserved */
        CVC_SEND_CAP_Destroy(op_extra_data);
        release_shared_volume_cntrl(op_extra_data->shared_volume_ptr);
        op_extra_data->shared_volume_ptr = NULL;
        cvc_send_release_constants(op_data);
        return(TRUE);
    }

    if(!cvc_send_register_component((void*)op_extra_data))
    {
        /* Free all the scratch memory we reserved, exit with fail response msg. Even if it had failed
         * silently, subsequent security checks will fail in lack of a successful registration.
         */
        CVC_SEND_CAP_Destroy(op_extra_data);
        release_shared_volume_cntrl(op_extra_data->shared_volume_ptr);
        op_extra_data->shared_volume_ptr = NULL;
        cvc_send_release_constants(op_data);
        return(TRUE);
    }


    if(!cpsInitParameters(&op_extra_data->parms_def,(unsigned*)CVC_SEND_GetDefaults(op_extra_data->cap_id),(unsigned*)op_extra_data->params,sizeof(CVC_SEND_PARAMETERS)))
    {
        /* Free all the scratch memory we reserved, exit with fail response msg. Even if it had failed
         * silently, subsequent security checks will fail in lack of a successful registration.
         */
        CVC_SEND_CAP_Destroy(op_extra_data);
        release_shared_volume_cntrl(op_extra_data->shared_volume_ptr);
        op_extra_data->shared_volume_ptr = NULL;
        cvc_send_release_constants(op_data);
        return(TRUE);
    }

     /* Read state info for MDGC */
    key = MAP_CAPID_UCID_SBID_TO_PSKEYID(op_extra_data->cap_id,0,OPMSG_P_STORE_STATE_VARIABLE_SUB_ID);
    ps_entry_read((void*)op_data,key,PERSIST_ANY,ups_state_snd);
    L4_DBG_MSG1("ps req MGDC: key=%d \n", key );

#ifdef CVC_EARBUD_3MIC_MONO_IE_ENABLE
     /* Read EQmap vector */
    if (op_extra_data->ptr_map_ratio)
    {
        /* first, set map ratio to default values, as a fallback */
        init_eq_map_ratio(op_extra_data->data_variant, op_extra_data->ptr_map_ratio);
        /* make the p-store request */
        key = MAP_CAPID_UCID_SBID_TO_PSKEYID(op_extra_data->cap_id, UCID_EQMAP, OPMSG_P_STORE_STATE_VARIABLE_SUB_ID);
        ps_entry_read((void*)op_data,key,PERSIST_ANY,ups_state_snd_eqmap);
        L4_DBG_MSG1("ps req EQMAP: key=%d \n", key );
    }
#endif //#ifdef CVC_EARBUD_3MIC_MONO_IE_ENABLE

#if defined(BUILD_CVC_SEND_USE_IIR_RESAMPLER)
    /* We don't have a new constant table to add - only register our interest in the IIR RESAMPLER constant tables. */
    iir_resamplerv2_add_config_to_list(NULL);
#endif

#ifdef CVC_SEND_SUPPORT_METADATA
    op_extra_data->mic_metadata_buffer = NULL;
    op_extra_data->op_metadata_buffer = NULL;
#endif

    base_op_change_response_status(response_data,STATUS_OK);
    return TRUE;
}

bool cvc_send_ups_set_state(void* instance_data, PS_KEY_TYPE key, PERSISTENCE_RANK rank, STATUS_KYMERA status,
                                     uint16 extra_status_info)
{
    L2_DBG_MSG3("ups_set_state  rank=%d status=%d extra_info=%d \n", (unsigned)rank, (unsigned)status, extra_status_info );
    return TRUE;
}

bool cvc_send_ups_set_state_eqmap(void* instance_data, PS_KEY_TYPE key, PERSISTENCE_RANK rank, STATUS_KYMERA status,
                                     uint16 extra_status_info)
{
    L2_DBG_MSG3("ups_set_state_eqmap  rank=%d status=%d extra_info=%d \n", (unsigned)rank, (unsigned)status, extra_status_info );
    return TRUE;
}

bool cvc_send_destroy(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);

     patch_fn_shared(cvc_send_wrapper);
    
    /* Setup Response to Destroy Request.*/
    if(!base_op_destroy(op_data, message_data, response_id, response_data))
    {
        cvc_send_release_constants(op_data);
        return(FALSE);
    }

    /* calling the "destroy" assembly function - this frees up all the capability-internal memory */
    CVC_SEND_CAP_Destroy(op_extra_data);

    /*free volume control shared memory*/
    release_shared_volume_cntrl(op_extra_data->shared_volume_ptr);
    op_extra_data->shared_volume_ptr = NULL;

    base_op_change_response_status(response_data,STATUS_OK);

    /* Save state info to ucid 0 */
      {
          unsigned key = MAP_CAPID_UCID_SBID_TO_PSKEYID(op_extra_data->cap_id,0,OPMSG_P_STORE_STATE_VARIABLE_SUB_ID);
          uint16 state_data[2];

          state_data[0] = (op_extra_data->mdgc_gain>>16)&0xFFFF;
            state_data[1] = op_extra_data->mdgc_gain&0xFFFF;
          ps_entry_write((void*)op_data,key,PERSIST_ANY,2,state_data,cvc_send_ups_set_state);
          L4_DBG_MSG1("pswrite MGDC: key=%d \n", key );

      }
    /* Save state from map ratio vector info to ucid */
    if ( op_extra_data->ptr_map_ratio )
    {
        unsigned key = MAP_CAPID_UCID_SBID_TO_PSKEYID(op_extra_data->cap_id, UCID_EQMAP, OPMSG_P_STORE_STATE_VARIABLE_SUB_ID);
        uint16 bin_count;
        switch(op_extra_data->data_variant)
        {
              case DATA_VARIANT_WB:  // 16 kHz
                  bin_count = DATA_VARIANT_WB_NBINS; //129
                  break;
              default: // only 16K and 8K are supported
                  bin_count = DATA_VARIANT_NB_NBINS; //65
                  break;
        }

        /* save bin_count*2 half-words */
        ps_entry_write((void*)op_data,key,PERSIST_ANY, (uint16)(bin_count*2), (uint16*)op_extra_data->ptr_map_ratio,cvc_send_ups_set_state_eqmap);
        L4_DBG_MSG1("pswrite EQMAP: key=%d \n", key );
    }                                                                                       

    cvc_send_release_constants(op_data);

#if defined(BUILD_CVC_SEND_USE_IIR_RESAMPLER)
      /* delete the configuration list */
      iir_resamplerv2_delete_config_list();
#endif

    return(TRUE);
}




void cvc_send_set_requested_features(CVC_SEND_OP_DATA *op_extra_data)
{
    op_extra_data->op_feature_requested = 0;
    int num_va_outputs = op_extra_data->num_va_outputs;
    int i;

    /* Check microphone connection */
    /* If microphone is not all connected, fail */
    for (i=1; i<=op_extra_data->num_mics; i++) {
        if(!op_extra_data->input_stream[i]) {
            return;
        }
    }

    /* Check VA channel connections */
    if(num_va_outputs)
    {
        op_extra_data->op_feature_requested = CVC_REQUESTED_FEATURE_VA;
        for (i=1; i<=num_va_outputs; i++) {
            if(!op_extra_data->output_stream[i]) {
                op_extra_data->op_feature_requested = 0;
                break;
            }
        }
    }

    /* Check VOICE channel connection */
    if(op_extra_data->output_stream[0])
    {
        op_extra_data->op_feature_requested |= CVC_REQUESTED_FEATURE_VOICE;
    }

    /* If neither VOICE nor VA is requested, fail */
    if (!op_extra_data->op_feature_requested) 
    {
        return;
    }

    /* Check AEC channel connection */
    if(op_extra_data->input_stream[0])
    {
        op_extra_data->op_feature_requested |= CVC_REQUESTED_FEATURE_AEC;
    }
}

bool cvc_send_connect(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);
    unsigned terminal_id = OPMGR_GET_OP_CONNECT_TERMINAL_ID(message_data);    /* extract the terminal_id */
    tCbuffer* pterminal_buf = OPMGR_GET_OP_CONNECT_BUFFER(message_data);

    patch_fn_shared(cvc_send_wrapper);

    /* Setup Response to Connection Request.   Assume Failure*/
    if (!base_op_build_std_response_ex(op_data, STATUS_CMD_FAILED, response_data))
    {
        return(FALSE);
    }

    /* (i)  check if the terminal ID is valid . The number has to be less than the maximum number of sinks or sources .  */
    /* (ii) check if we are connecting to the right type . It has to be a buffer pointer and not endpoint connection */
    if( !base_op_is_terminal_valid(op_data, terminal_id) || !pterminal_buf)
    {
        base_op_change_response_status(response_data,STATUS_INVALID_CMD_PARAMS);
        return TRUE;
    }

    /* Connect the appropriate stream map */
    if (terminal_id & TERMINAL_SINK_MASK)
    {
        op_extra_data->input_stream[terminal_id&CVC_SEND_NUM_INPUTS_MASK] = pterminal_buf;

#ifdef CVC_SEND_SUPPORT_METADATA
        if(0 != (terminal_id & ~TERMINAL_SINK_MASK))
        {
            /* if this is a mic input terminal, then
             * the first connected buffer with metadata
             * will be used as metadata buffer
             */
            if(NULL == op_extra_data->mic_metadata_buffer &&
               buff_has_metadata(pterminal_buf))
            {
                op_extra_data->mic_metadata_buffer = pterminal_buf;
            }
        }
#endif /* CVC_SEND_SUPPORT_METADATA */

    }
    else
    {
        op_extra_data->output_stream[terminal_id&CVC_SEND_NUM_OUTPUTS_MASK] = pterminal_buf;
#ifdef CVC_SEND_SUPPORT_METADATA
        if(NULL == op_extra_data->op_metadata_buffer &&
               buff_has_metadata(pterminal_buf))
        {
            op_extra_data->op_metadata_buffer = pterminal_buf;
        }
#endif
    }

    /* Allow Connect while running.  Re-enable processing alter all connections are completed */
    cvc_send_set_requested_features(op_extra_data);

    base_op_change_response_status(response_data,STATUS_OK);
    return TRUE;
}



bool cvc_send_disconnect(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);
    unsigned terminal_id = OPMGR_GET_OP_DISCONNECT_TERMINAL_ID(message_data);

     patch_fn_shared(cvc_send_wrapper);
    
    /* Setup Response to Disconnection Request.   Assume Failure*/
    if (!base_op_build_std_response_ex(op_data, STATUS_CMD_FAILED, response_data))
    {
        return(FALSE);
    }

    /* check if the terminal ID is valid . The number has to be less than the maximum number of sinks or sources.  */
    if(!base_op_is_terminal_valid(op_data, terminal_id))
    {
        base_op_change_response_status(response_data,STATUS_INVALID_CMD_PARAMS);
        return TRUE;
    }

    /* Disconnect the relevant terminal */
    if (terminal_id & TERMINAL_SINK_MASK)
    {
#ifdef CVC_SEND_SUPPORT_METADATA
        if(terminal_id&CVC_SEND_NUM_INPUTS_MASK)
        {
            tCbuffer *this_buf = op_extra_data->input_stream[terminal_id&CVC_SEND_NUM_INPUTS_MASK];
            if(this_buf == op_extra_data->mic_metadata_buffer)
            {
                /* disconnecting buffer is the metadata buffer,
                 * change the metadata buffer to another connected
                 * buffer with metadata, if there is any.
                 */
                tCbuffer *new_metadata_buf = NULL;
                int mic_idx;
                for(mic_idx=1; mic_idx <= op_extra_data->num_mics; mic_idx++)
                {
                    tCbuffer *mic_buffer = op_extra_data->input_stream[mic_idx];

                    if(mic_buffer != NULL &&
                       mic_buffer != this_buf &&
                       buff_has_metadata(mic_buffer))
                    {
                        new_metadata_buf = mic_buffer;
                        break;
                    }
                }
                op_extra_data->mic_metadata_buffer = new_metadata_buf;
            }
        }
#endif /* CVC_SEND_SUPPORT_METADATA */

        op_extra_data->input_stream[terminal_id&CVC_SEND_NUM_INPUTS_MASK] = NULL;

    }
    else
    {
#ifdef CVC_SEND_SUPPORT_METADATA
        tCbuffer *this_buf = op_extra_data->output_stream[terminal_id&CVC_SEND_NUM_OUTPUTS_MASK];
        if(this_buf == op_extra_data->op_metadata_buffer)
        {
            tCbuffer *new_metadata_buf = NULL;
            int op;
            for(op=0; op < CVC_SEND_MAX_NUM_OUTPUT; op++)
            {
               if(NULL != op_extra_data->output_stream[op])
               {
                   tCbuffer *op_buffer = op_extra_data->output_stream[op];
    
                   if(op_buffer != NULL &&
                         op_buffer != this_buf &&
                         buff_has_metadata(op_buffer))
                   {
                      new_metadata_buf = op_buffer;
                      break;
                   }
               }
            }
            op_extra_data->op_metadata_buffer = new_metadata_buf;
        }
#endif /* CVC_SEND_SUPPORT_METADATA */
        op_extra_data->output_stream[terminal_id&CVC_SEND_NUM_OUTPUTS_MASK] = NULL;
    }

    cvc_send_set_requested_features(op_extra_data);

    base_op_change_response_status(response_data,STATUS_OK);
    return TRUE;
}


bool cvc_send_buffer_details(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    CVC_SEND_OP_DATA *opx_data = get_instance_data(op_data);
    unsigned terminal_id = OPMGR_GET_OP_BUF_DETAILS_TERMINAL_ID(message_data);

    patch_fn_shared(cvc_send_wrapper);
    
    if (!base_op_buffer_details(op_data, message_data, response_id, response_data))
    {
        return FALSE;
    }
    /* No inplace support input samples may be dropped */
    ((OP_BUF_DETAILS_RSP*)*response_data)->b.buffer_size= opx_data->frame_size<<1;

    if(terminal_id == TERMINAL_SINK_MASK)
    {
        /* Reference buffer needs more space */
        ((OP_BUF_DETAILS_RSP*)*response_data)->b.buffer_size += (opx_data->frame_size>>1);
    }
#ifdef CVC_SEND_SUPPORT_METADATA
    else
    {
        CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);

        /* metadata is supported in all terminals except the reference one */
        ((OP_BUF_DETAILS_RSP*)*response_data)->supports_metadata = TRUE;
        if(terminal_id & TERMINAL_SINK_MASK)
        {
            /* A mic terminal, if we already have a connected buffer with metadata pass that one */
            ((OP_BUF_DETAILS_RSP*)*response_data)->metadata_buffer = op_extra_data->mic_metadata_buffer;
        }
        else
        {
            /* Output terminal */
#ifdef CVC_SEND_SUPPORT_METADATA
            ((OP_BUF_DETAILS_RSP*)*response_data)->metadata_buffer = op_extra_data->op_metadata_buffer;
#else
            ((OP_BUF_DETAILS_RSP*)*response_data)->metadata_buffer = NULL;
#endif            
        }
    }
#endif /* SCO_RX_OP_GENERATE_METADATA */

    return TRUE;
}

bool cvc_send_get_sched_info(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);
    OP_SCHED_INFO_RSP* resp;

    resp = base_op_get_sched_info_ex(op_data, message_data, response_id);
    if (resp == NULL)
    {
        return base_op_build_std_response_ex(op_data, STATUS_CMD_FAILED, response_data);
    }
    *response_data = resp;

    /* Same buffer size for sink and source.
     * No additional verification needed.*/
    resp->block_size = op_extra_data->frame_size;

    return TRUE;
}

/* ************************************* Data processing-related functions and wrappers **********************************/

void cvc_send_process_data(OPERATOR_DATA *op_data, TOUCHED_TERMINALS *touched)
{
    CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);
    int samples_to_process, stream_amount_data;
    int mic_index;
    unsigned frame_size = op_extra_data->frame_size;
    unsigned doubled_frame_size = frame_size * 2;
    unsigned sinks_not_having_enough_data_for_current_frame = 0;
    unsigned sinks_not_having_enough_data_for_next_frame = 0;

    patch_fn(cvc_send_process_data_patch);

    unsigned op_feature_requested = op_extra_data->op_feature_requested;

    /* Bypass processing until all streams are connected */
    if(op_feature_requested == 0)
    {
       return;
    }

    /* number of samples to process at the reference */
    if(op_feature_requested & CVC_REQUESTED_FEATURE_AEC)
    {
       samples_to_process = cbuffer_calc_amount_data_in_words(op_extra_data->input_stream[0]);   /* Reference */

        /* Issue backward kick from this sink to request more data if, at the exit point of this 
        * function, the data in the input buffer is insufficient for frame processing.
        */
        if(samples_to_process < frame_size)
        {
            sinks_not_having_enough_data_for_current_frame = TOUCHED_SINK_0;
        }
        else if (samples_to_process < doubled_frame_size)
        {
            sinks_not_having_enough_data_for_next_frame = TOUCHED_SINK_0;
        }

    }
    else
    {
       samples_to_process = frame_size;
    }
    /* number of samples to process at the mics */
    for(mic_index=1; mic_index <= op_extra_data->num_mics; mic_index++)
    {
        stream_amount_data = cbuffer_calc_amount_data_in_words(op_extra_data->input_stream[mic_index]);
        if (stream_amount_data < samples_to_process)
        {
            samples_to_process = stream_amount_data;
        }
        
        /* Issue backward kick from this sink to request more data if, at the exit point of this 
        * function, the data in the input buffer is insufficient for frame processing.
        */
        if(stream_amount_data < frame_size)
        {
            sinks_not_having_enough_data_for_current_frame |= TOUCHED_SINK_0 << mic_index;
        }
        else if(stream_amount_data < doubled_frame_size)
        {
            sinks_not_having_enough_data_for_next_frame |= TOUCHED_SINK_0 << mic_index;
        }
    }
#ifdef CVC_SEND_SUPPORT_METADATA
    patch_fn_shared(cvc_send_wrapper);

    if(op_extra_data->mic_metadata_buffer!= NULL)
    {
        /* if mic inputs have metadata, then limit the amount of
        * consuming to the amount of metadata available.
        */
        unsigned meta_data_available =
           buff_metadata_available_octets(op_extra_data->mic_metadata_buffer)/OCTETS_PER_SAMPLE;

        samples_to_process = MIN(samples_to_process, meta_data_available);
    }

    if(samples_to_process < frame_size)
    {
        /* Unable to trigger processing due to insufficient input data, 
         * kick backward to request more data */
        touched->sinks = sinks_not_having_enough_data_for_current_frame;
        return;
    }

    if (op_extra_data->op_metadata_buffer != NULL) {
        /* if outputs have metadata, also check the space available */

        /* Voice channel */
        if (NULL != op_extra_data->output_stream[0])
        {
            unsigned space_available = cbuffer_calc_amount_space_in_words(op_extra_data->output_stream[0]);
            samples_to_process = MIN(samples_to_process, space_available);        
        }
        /* VA channel */
        if (NULL != op_extra_data->output_stream[1])
        {
            unsigned space_available = cbuffer_calc_amount_space_in_words(op_extra_data->output_stream[1]);
            samples_to_process = MIN(samples_to_process, space_available);        
        }
    }
#endif /* CVC_SEND_SUPPORT_METADATA */

    /* Check for sufficient data and space */
    if(samples_to_process < frame_size)
    {
        /* Unable to trigger processing due to insufficient output space, 
         * return without kicking backward */
        return;
    }

    /* At this point, the data & space requirements of frame processing are satisfied, and
     * once the frame processing is done, request more data for next frame by kicking back */
    touched->sinks = sinks_not_having_enough_data_for_next_frame;

    if (op_extra_data->Ovr_Control & GEN_CVC_SEND_CONTROL_MODE_OVERRIDE)
    {
        op_extra_data->Cur_mode = op_extra_data->Obpm_mode;
    }
    else if (op_extra_data->major_config!=CVC_SEND_CONFIG_HEADSET)
    {
        op_extra_data->Cur_mode = op_extra_data->Host_mode;
    }
    else if (op_extra_data->Host_mode != GEN_CVC_SEND_SYSMODE_FULL)
    {
        op_extra_data->Cur_mode = op_extra_data->Host_mode;
    }
    else
    {
        unsigned temp = op_extra_data->Cur_mode;
        if ((temp == GEN_CVC_SEND_SYSMODE_FULL) || (temp == GEN_CVC_SEND_SYSMODE_LOWVOLUME) )
        {
           /* TODO - need to redefine OFFSET_LVMODE_THRES to dB/60 */
            unsigned vol_level = 15 - (((int)op_extra_data->shared_volume_ptr->current_volume_level)/(-360));

            if (vol_level < op_extra_data->params->OFFSET_LVMODE_THRES)
            {
               op_extra_data->Cur_mode = GEN_CVC_SEND_SYSMODE_LOWVOLUME;
            }
            else
            {
               op_extra_data->Cur_mode = GEN_CVC_SEND_SYSMODE_FULL;
            }

            if (temp != op_extra_data->Cur_mode)
            {
               op_extra_data->ReInitFlag = 1;
            }
        }
        else
        {
            op_extra_data->Cur_mode = op_extra_data->Host_mode;
        }
    }

#ifdef CVC_SEND_SUPPORT_METADATA
    patch_fn_shared(cvc_send_wrapper);

    if(NULL != op_extra_data->mic_metadata_buffer)
    {
        /* transport metadata from input to output */
        metadata_strict_transport(op_extra_data->mic_metadata_buffer, 
                                  op_extra_data->op_metadata_buffer,
                              frame_size*OCTETS_PER_SAMPLE);
    }
    else if (op_extra_data->op_metadata_buffer != NULL)
    {
        /* input doesn't have metadata but output does,
         * Normally this shouldn't be the case but in this
         * case we only append a balank tag to output buffer.
         * TODO: This could be done in metadata_strict_transport function.
         */
        metadata_tag *out_mtag = buff_metadata_new_tag();
        unsigned afteridx = frame_size * OCTETS_PER_SAMPLE;
        if(out_mtag != NULL)
        {
            out_mtag->length = afteridx;
        }
        buff_metadata_append(op_extra_data->op_metadata_buffer, out_mtag, 0, afteridx);
    }
#endif /* CVC_SEND_SUPPORT_METADATA */

    /* call the "process" assembly function */
    CVC_SEND_CAP_Process(op_extra_data);

    /* Set touched->sources for forward kicking */    
    if(op_feature_requested & CVC_REQUESTED_FEATURE_VOICE) 
    {
        touched->sources = TOUCHED_SOURCE_0;
    }

    if(op_feature_requested & CVC_REQUESTED_FEATURE_VA) 
    {
        /* in order to kick all the connected VA channels, we need to assign a binary number 
        such as 11110 (for 4 VA channels) or 10 (for 1 VA channel) to the touched->sources.
        
        The 0 on the last bit (least significant bit) of the binary number represents the voice channel 
        (1st output channel) whose kicking logic should be determined separately. */
        
        touched->sources |= (TOUCHED_SOURCE_1 << (op_extra_data->num_va_outputs)) - TOUCHED_SOURCE_1;
    }
}


/* **************************** Operator message handlers ******************************** */



bool cvc_send_opmsg_obpm_set_control(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{

    CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);
    unsigned           i,num_controls,cntrl_value;
    CPS_CONTROL_SOURCE cntrl_src;
    OPMSG_RESULT_STATES result = OPMSG_RESULT_STATES_NORMAL_STATE;

    patch_fn(cvc_send_opmsg_obpm_set_control_patch);

    if(!cps_control_setup(message_data, resp_length, resp_data,&num_controls))
    {
       return FALSE;
    }

    for(i=0;i<num_controls;i++)
    {
        unsigned  cntrl_id=cps_control_get(message_data,i,&cntrl_value,&cntrl_src);
        /* Only interested in lower 8-bits of value */
        cntrl_value &= 0xFF;

        if (cntrl_id == OPMSG_CONTROL_MODE_ID)
        {
            /* Control is Mode */
            if (cntrl_value >= GEN_CVC_SEND_SYSMODE_MAX_MODES)
            {
                result = OPMSG_RESULT_STATES_INVALID_CONTROL_VALUE;
                break;
            }

            if(cntrl_src == CPS_SOURCE_HOST)
            {
               op_extra_data->Host_mode = cntrl_value;
            }
            else
            {
                op_extra_data->Obpm_mode = cntrl_value;

                /* When the override bit in the control id is high, then we override the
                 * OBPM's ability to override the control value. In other words we let the control
                 * value be reset to the host's control value when the OPMSG_CONTROL_OBPM_OVERRIDE
                 * bit is high in the control id.*/
                if (cntrl_src == CPS_SOURCE_OBPM_DISABLE)
                {
                    op_extra_data->Ovr_Control &= ~GEN_CVC_SEND_CONTROL_MODE_OVERRIDE;
                }
                else
                {
                    op_extra_data->Ovr_Control |= GEN_CVC_SEND_CONTROL_MODE_OVERRIDE;
                }
            }
            op_extra_data->ReInitFlag = 1;
        }
        else if (cntrl_id == OPMSG_CONTROL_MUTE_ID)
        {
            if (cntrl_value > 1)
            {
                result = OPMSG_RESULT_STATES_INVALID_CONTROL_VALUE;
                break;
            }
            /* Control is Mute */
            if(cntrl_src == CPS_SOURCE_HOST)
            {
               op_extra_data->host_mute = cntrl_value;
            }
            else
            {
                op_extra_data->obpm_mute = cntrl_value;

                /* When the override bit in the control id is high, then we override the
                 * OBPM's ability to override the control value. In other words we let the control
                 * value be reset to the host's control value when the OPMSG_CONTROL_OBPM_OVERRIDE
                 * bit is high in the control id.*/
                if(cntrl_src == CPS_SOURCE_OBPM_DISABLE)
                {
                    op_extra_data->Ovr_Control &= ~GEN_CVC_SEND_CONTROL_MUTE_OVERRIDE;
                }
                else
                {
                    op_extra_data->Ovr_Control |= GEN_CVC_SEND_CONTROL_MUTE_OVERRIDE;
                }
            }
            op_extra_data->Cur_Mute = ( op_extra_data->Ovr_Control & GEN_CVC_SEND_CONTROL_MUTE_OVERRIDE) ? op_extra_data->obpm_mute : op_extra_data->host_mute;
        }
        else if (cntrl_id == OPMSG_CONTROL_OMNI_ID)
        {
            if(op_extra_data->omni_mode_ptr == NULL)
            {
                result = OPMSG_RESULT_STATES_INVALID_CONTROL_VALUE;
                break;
            }
            if (cntrl_value > 1)
            {
                result = OPMSG_RESULT_STATES_INVALID_CONTROL_VALUE;
                break;
            }
            /* Control is Mute */
            if(cntrl_src == CPS_SOURCE_HOST)
            {
                op_extra_data->host_omni = cntrl_value;
            }
            else
            {
                op_extra_data->obpm_omni = cntrl_value;

                /* When the override bit in the control id is high, then we override the
                 * OBPM's ability to override the control value. In other words we let the control
                 * value be reset to the host's control value when the OPMSG_CONTROL_OBPM_OVERRIDE
                 * bit is high in the control id.*/
                if(cntrl_src == CPS_SOURCE_OBPM_DISABLE)
                {
                    op_extra_data->Ovr_Control &= ~GEN_CVC_SEND_CONTROL_OMNI_OVERRIDE;
                }
                else
                {
                    op_extra_data->Ovr_Control |= GEN_CVC_SEND_CONTROL_OMNI_OVERRIDE;
                }
            }
            *(op_extra_data->omni_mode_ptr) = ( op_extra_data->Ovr_Control & GEN_CVC_SEND_CONTROL_OMNI_OVERRIDE) ? op_extra_data->obpm_omni : op_extra_data->host_omni;
        }
        else
        {
            result = OPMSG_RESULT_STATES_UNSUPPORTED_CONTROL;
            break;
        }
    }

    cps_response_set_result(resp_data,result);

    return TRUE;
}

bool cvc_send_opmsg_obpm_get_params(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
    CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);

    return cpsGetParameterMsgHandler(&op_extra_data->parms_def ,message_data, resp_length,resp_data);
}

bool cvc_send_opmsg_obpm_get_defaults(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
    CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);

    return cpsGetDefaultsMsgHandler(&op_extra_data->parms_def ,message_data, resp_length,resp_data);
}

bool cvc_send_opmsg_obpm_set_params(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
    CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);
    bool retval;

    patch_fn(cvc_send_opmsg_obpm_set_params_patch);

    retval = cpsSetParameterMsgHandler(&op_extra_data->parms_def ,message_data, resp_length,resp_data);

    /* Set the Reinit flag after setting the paramters */
    op_extra_data->ReInitFlag = 1;

    return retval;
}

unsigned cvc_send_combine_status_flags(unsigned **flags)
{
    unsigned combined_bitflag = 0;

    // Self Clean Flag 
    if (*flags[SELFCLEAN_FLAG_POS]) combined_bitflag |= GEN_CVC_SEND_CVC_STATUS_BITFLAG_SELFCLEAN_DETECTED;
    // High Noise Flag 
    if (*flags[HIGHNOISE_FLAG_POS]) combined_bitflag |= GEN_CVC_SEND_CVC_STATUS_BITFLAG_HIGHNOISE_FLAG;
    // Mic Malfunction Flag 
    if (*flags[MIC_MALFUNC_FLAG_POS]) combined_bitflag |= GEN_CVC_SEND_CVC_STATUS_BITFLAG_MIC_MALFUNC_FLAG;
    // Loose Fit Flag 
    if (*flags[LOOSE_FIT_FLAG_POS]) combined_bitflag |= GEN_CVC_SEND_CVC_STATUS_BITFLAG_LOOSE_FIT_FLAG;
    // Three Mic Flag 
    if (*flags[THREE_MIC_FLAG_POS]) combined_bitflag |= GEN_CVC_SEND_CVC_STATUS_BITFLAG_THREE_MIC_FLAG;
    // Ref Power Flag 
    if (*flags[REF_PWR_FLAG_POS]) combined_bitflag |= GEN_CVC_SEND_CVC_STATUS_BITFLAG_REF_POWER_FLAG;

    return combined_bitflag;
}

bool cvc_send_opmsg_obpm_get_status(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
    CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);
    unsigned  *resp;
    unsigned **stats = (unsigned**)op_extra_data->status_table;

    patch_fn(cvc_send_opmsg_obpm_get_status_patch);

    if(!common_obpm_status_helper(message_data,resp_length,resp_data,sizeof(CVC_SEND_STATISTICS),&resp))
    {
         return FALSE;
    }

    if(resp)
    {
        resp = cpsPack2Words(op_extra_data->Cur_mode, op_extra_data->Ovr_Control, resp);
        resp = cpsPack2Words(*stats[0], *stats[1], resp);
        resp = cpsPack2Words(*stats[2], *stats[3], resp);
        /* Reset IN/OUT Peak Detectors*/
        *(stats[1])=0;
        *(stats[2])=0;
        *(stats[3])=0;
        resp = cpsPack2Words(*stats[4], *stats[5], resp);
        resp = cpsPack2Words(*stats[6], *stats[7], resp);
        resp = cpsPack2Words(*stats[8], *stats[9], resp);
        resp = cpsPack2Words(*stats[10], *stats[11], resp);
        resp = cpsPack2Words(*op_extra_data->mute_control_ptr, *stats[13], resp);
        resp = cpsPack2Words(*stats[14], *stats[15], resp);
        resp = cpsPack2Words(*stats[16], *stats[17], resp);
        resp = cpsPack2Words(*stats[18], *stats[19], resp);
        resp = cpsPack2Words(*stats[20] , *stats[21], resp);
        resp = cpsPack2Words(cvc_send_combine_status_flags((unsigned**)stats[22]), *stats[23], resp);
        resp = cpsPack2Words(*stats[24] , *stats[25], resp);
        resp = cpsPack1Word(*stats[26], resp);

        /* Reset Peak Detectors AEC_REF/MIC3/MIC4/VA1 */
        *(stats[14])=0;
        *(stats[15])=0;
        *(stats[16])=0;
        *(stats[19])=0;
    }

    return TRUE;
}

bool ups_params_snd(void* instance_data,PS_KEY_TYPE key,PERSISTENCE_RANK rank,
                 uint16 length, unsigned* data, STATUS_KYMERA status,uint16 extra_status_info)
{
    CVC_SEND_OP_DATA *op_extra_data = get_instance_data((OPERATOR_DATA*)instance_data);

    cpsSetParameterFromPsStore(&op_extra_data->parms_def,length,data,status);

    /* Set the Reinit flag after setting the paramters */
    op_extra_data->ReInitFlag = 1;

    return(TRUE);
}

bool cvc_send_opmsg_set_ucid(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
    CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);
    PS_KEY_TYPE key;
    bool retval;

    retval = cpsSetUcidMsgHandler(&op_extra_data->parms_def,message_data,resp_length,resp_data);

    key = MAP_CAPID_UCID_SBID_TO_PSKEYID(op_extra_data->cap_id,op_extra_data->parms_def.ucid,OPMSG_P_STORE_PARAMETER_SUB_ID);
    ps_entry_read((void*)op_data,key,PERSIST_ANY,ups_params_snd);

    return retval;
}

bool cvc_send_opmsg_get_ps_id(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
    CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);

    return cpsGetUcidMsgHandler(&op_extra_data->parms_def,op_extra_data->cap_id,message_data,resp_length,resp_data);
}

bool cvc_send_opmsg_get_voice_quality(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
    
    CVC_SEND_OP_DATA *op_extra_data = get_instance_data(op_data);
    
    /* call the assembly function */
    unsigned metric = compute_voice_quality_metric(op_extra_data);
    
    /* CVC_SEND_VOICE_QUALITY_METRIC_ERROR_CODE is the error code indicating 
    either mode is not supported or the DMS object is bypassed */
    if(metric != CVC_SEND_VOICE_QUALITY_METRIC_ERROR_CODE){
       *resp_length = OPMSG_RSP_PAYLOAD_SIZE_RAW_DATA(1);
       
      /* raw data pointer */
       unsigned* resp = xzppmalloc((*resp_length)* sizeof(unsigned), MALLOC_PREFERENCE_NONE); 
       if (!resp){
          /* return if memory allocation fails */
          return FALSE;
       }   
       
       /* set operator message ID in the raw data */
       resp[0] = OPMSG_COMMON_GET_VOICE_QUALITY;    
       resp[1] = metric;
       /* assign the raw data pointer *resp to the **resp_data */
       *resp_data = (OP_OPMSG_RSP_PAYLOAD*)resp;
    }
    
    return TRUE;
}
