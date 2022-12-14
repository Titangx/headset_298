/****************************************************************************
Copyright (c) 2004 - 2022 Qualcomm Technologies International, Ltd.


FILE NAME
    connection_private.h

DESCRIPTION
    This file contains the type definitions and function prototypes for the
    Connection Library
*/

#ifndef CONNECTION_PRIVATE_H_
#define CONNECTION_PRIVATE_H_

/* Header files */
#include <stdlib.h>
#include <string.h>

#include <app/message/system_message.h>
#include <message.h>
#include <panic.h>
#include <string.h>

#include "connection.h"
#include "common.h"
#include "init.h"

#ifdef HYDRACORE
#define CTX_FROM_TASK(x) (context_t)(x)
#else
#define CTX_FROM_TASK(x) (uint16)(x)
#endif /* HYDRACORE*/



#ifndef MESSAGE_MAKE
/*
   Normally picked up from message.h, but might not be present pre
   4.1.1h, so fall back to the old behaviour.
*/
#define MESSAGE_MAKE(N,T) T *N = PanicUnlessNew(T)
#endif

/* Macros used to make messsage primitive creation simpler */
#define MAKE_PRIM_C(TYPE) MESSAGE_MAKE(prim,TYPE##_T); prim->common.op_code = TYPE; prim->common.length = sizeof(TYPE##_T);
#define MAKE_PRIM_T(TYPE) MESSAGE_MAKE(prim,TYPE##_T); prim->type = TYPE;

#define MAKE_CL_MESSAGE(TYPE) MESSAGE_MAKE(message,TYPE##_T);
#define MAKE_CL_MESSAGE_WITH_LEN(TYPE, LEN) TYPE##_T *message = (TYPE##_T *) PanicUnlessMalloc(sizeof(TYPE##_T) + LEN);
#define COPY_CL_MESSAGE(src, dst) *(dst) = *(src);
#define COPY_CL_MESSAGE_WITH_LEN(TYPE, LEN, src, dst) memmove((dst), (src), sizeof(TYPE##_T) + LEN);

/* Macros used to generate debug lib printfs. */
#ifdef CONNECTION_DEBUG_LIB
#include <panic.h>
#include <stdio.h>
#define CL_DEBUG_INFO(x) {printf x;}
#define CL_DEBUG(x)     {printf x;  Panic();}
#define checkStatus(x)    { DM_HCI_STANDARD_COMMAND_CFM_T *cfm = (DM_HCI_STANDARD_COMMAND_CFM_T *) x; if (cfm->status != HCI_SUCCESS) CL_DEBUG(("Ignored result prim reports error: 0x%x\n", cfm->status)); }
#else
#define CL_DEBUG_INFO(x)
#define CL_DEBUG(x)
#define checkStatus(x)
#endif /* CONNECTION_DEBUG_LIB */

/* Parameter checking limits for use by debug build */
#ifdef CONNECTION_DEBUG_LIB
/* From the RFCOMM specification TS07.10
 * Server applications registering with an RFCOMM service interface are
 * assigned a Server Channel number in the range 1...30. [0 and 31] should not
 * be used since the corresponding DLCIs are reserved in TS [07.10]
 * It is this value that should be registered in the Service Discovery Database
 */
#define RFCOMM_SERVER_CHANNEL_MIN       (1)
#define RFCOMM_SERVER_CHANNEL_MAX       (30)
#define MIN_AUTHENTICATION_TIMEOUT      (60)
#define MAX_AUTHENTICATION_TIMEOUT      (600)
#define MIN_WRITE_IAC_LAP               (1)
#define MAX_WRITE_IAC_LAP               (4)
#define MIN_TX_POWER                    (-70)
#define MAX_TX_POWER                    (20)
#endif

#define BREDR_KEY_SIZE                  (8)

#define CL_DM_SM_SEC_CONFIG_PENDING      0x01

/* Passed to MessageSend functions if the message being sent does not have a payload defined. */
#define NO_PAYLOAD      (0x0)

/* Max length of the name in change local name or read remote name requests. */
#define  MAX_NAME_LENGTH        (31)

/* As we store the authentication requirements in HCI format we need to define
   our own unknown bit to use internally */
#define AUTH_REQ_UNKNOWN    (0x08)

/* Connection library global flags */
/* Pass SDP search reasult as a reference */
#define CONNECTION_FLAG_SDP_REFERENCE   0x01
/* Enable Secure Connections at initialization */
#define CONNECTION_FLAG_SC_ENABLE       0x02
/* Unused global flag bit */
#define CONNECTION_FLAG_SCOM_ENABLE     0x04
/* Disable Cross Transport Key Derivation, which is enabled by default. */
#define CONNECTION_FLAG_CTKD_DISABLE    0x08
/* Selective Cross Transport Key Derivation during Pairing. */
#define CONNECTION_FLAG_SELECTIVE_CTKD  0x10
/* Unused global flag bit */
#define CONNECTION_FLAG_UNUSED_3        0x20
/* Unused global flag bit */
#define CONNECTION_FLAG_UNUSED_2        0x40
/* Unused global flag bit */
#define CONNECTION_FLAG_UNUSED_1        0x80


/* Connection Library private messages */
#define CM_MSG_BASE     (0x0)

/* Channel map mask */
#define BLE_CHANNEL_MAP_MASK  (0x07)

/* Default Advertising interval */
#define BLE_DEF_ADV_INTERVAL (0x0800)

/* Minimum Advertising interval */
#define BLE_MIN_ADV_INTERVAL (0x00A0)

/* Unknown sink, used in resource locks where the sink is unknown */
#define CL_INVALID_SINK ((Sink)0xFFFF)

/* Isochronous-related defines. */
#define ISOC_CODEC_ID_SIZE 5

/* The Default number of Trusted Device List (TDL) devices to manage */
/* Do not alter these values without considering how they are stored
 * in the PSKEYs below.
 *
 * Up to list size 8 Classic PSKEY configuration is used for backwards
 * compatibility with previous versions, sizes 9-12 have a new PSKEY
 * arrangement.
 */
#define DEFAULT_NO_DEVICES_TO_MANAGE            (12)
#define DEFAULT_NO_DEVICES_TO_MANAGE_CLASSIC    (8)

/* The Max number of TDL devices to be managed as set in ConnectionInit */
#define MAX_TRUSTED_DEVICES  \
    (((connectionState *)connectionGetCmTask())->smState.TdlNumberOfDevices)

/* Max and Min TDL Devices, changes as per classic TDL setting */
#ifdef CL_TDL_EXTEND
#define MAX_NO_DEVICES_TO_MANAGE (DEFAULT_NO_DEVICES_TO_MANAGE)
#else
#define MAX_NO_DEVICES_TO_MANAGE (DEFAULT_NO_DEVICES_TO_MANAGE_CLASSIC)
#endif
#define MIN_NO_DEVICES_TO_MANAGE (1)

/*****************************************************************************
 * PSKEYs used by the connection library
 *
 * Attributes related to a device by its address are persistently stored in
 * PSKEY_CONNLIB0 (100) to PSKEY_CONNLIB49 (149).
*/
#define PSKEY_BASE (100)

/* Classic Connlib PSKEY Map - used for list sizes 1-8 */
#define PSKEY_CLASSIC_TDL_ATTRIBUTE_BASE            (PSKEY_BASE) /* TDL Attributes List */
#define PSKEY_CLASSIC_GATT_DB_HASH                  (109)        /* GATT DB Hash */
#define PSKEY_CLASSIC_TDL_GATT_ATTRIBUTE_BASE       (110)        /* TDL Gatt Attributes List */
#define PSKEY_CLASSIC_SM_DIV_STATE                  (139)        /* Sm Div State */
#define PSKEY_CLASSIC_SM_KEY_STATE_IR_ER            (140)        /* Sm Key State Ir Er */
#define PSKEY_CLASSIC_TRUSTED_DEVICE_LIST           (141)        /* TDL Index and list */

/* New Connlib PSKEY Map - used for list sizes 9-12 */
#define PSKEY_NEW_TRUSTED_DEVICE_LIST     (PSKEY_BASE)                                                        /* TDL Index and list */
#define PSKEY_NEW_TDL_GATT_ATTRIBUTE_BASE (PSKEY_NEW_TRUSTED_DEVICE_LIST + DEFAULT_NO_DEVICES_TO_MANAGE + 1)  /* TDL Gatt Attributes List */
#define PSKEY_NEW_TDL_ATTRIBUTE_BASE      (PSKEY_NEW_TDL_GATT_ATTRIBUTE_BASE + DEFAULT_NO_DEVICES_TO_MANAGE)  /* TDL Attributes List */
#define PSKEY_NEW_GATT_DB_HASH            (PSKEY_NEW_TDL_ATTRIBUTE_BASE + DEFAULT_NO_DEVICES_TO_MANAGE)       /* GATT DB Hash */
#define PSKEY_NEW_SM_DIV_STATE            (PSKEY_NEW_GATT_DB_HASH + 1)                                        /* Sm Div State */
#define PSKEY_NEW_SM_KEY_STATE_IR_ER      (PSKEY_NEW_SM_DIV_STATE + 1)                                        /* Sm Key State Ir Er */
/* Static assert to checks CONNLIB PSKEY allocation has not overflowed */
STATIC_ASSERT(PSKEY_NEW_SM_KEY_STATE_IR_ER<=149, max_conn_lib_pskey_exceeded);

/* Select which TDL PSKEY indexing is in use */
#ifdef CL_TDL_EXTEND
#define PSKEY_TDL_ATTRIBUTE_BASE       (PSKEY_NEW_TDL_ATTRIBUTE_BASE)
#define PSKEY_GATT_DB_HASH             (PSKEY_NEW_GATT_DB_HASH)
#define PSKEY_TDL_GATT_ATTRIBUTE_BASE  (PSKEY_NEW_TDL_GATT_ATTRIBUTE_BASE)
#define PSKEY_SM_DIV_STATE             (PSKEY_NEW_SM_DIV_STATE)
#define PSKEY_SM_KEY_STATE_IR_ER       (PSKEY_NEW_SM_KEY_STATE_IR_ER)
#define PSKEY_TRUSTED_DEVICE_LIST      (PSKEY_NEW_TRUSTED_DEVICE_LIST)
#else
#define PSKEY_TDL_ATTRIBUTE_BASE       (PSKEY_CLASSIC_TDL_ATTRIBUTE_BASE)
#define PSKEY_GATT_DB_HASH             (PSKEY_CLASSIC_GATT_DB_HASH)
#define PSKEY_TDL_GATT_ATTRIBUTE_BASE  (PSKEY_CLASSIC_TDL_GATT_ATTRIBUTE_BASE)
#define PSKEY_SM_DIV_STATE             (PSKEY_CLASSIC_SM_DIV_STATE)
#define PSKEY_SM_KEY_STATE_IR_ER       (PSKEY_CLASSIC_SM_KEY_STATE_IR_ER)
#define PSKEY_TRUSTED_DEVICE_LIST      (PSKEY_CLASSIC_TRUSTED_DEVICE_LIST)
#endif


/* adv_handle is the advertising set ID. Default to 0xFF when not set. */
#define ADV_HANDLE_INVALID                      (0xFF)

/*
 *
 ****************************************************************************/
typedef enum
{
    /* Initialisation */
    CL_INTERNAL_INIT_TIMEOUT_IND = CM_MSG_BASE,
    CL_INTERNAL_INIT_REQ,
    CL_INTERNAL_INIT_CFM,

    /* Inquiry Entity */
    CL_INTERNAL_DM_INQUIRY_REQ,
    CL_INTERNAL_DM_INQUIRY_CANCEL_REQ,
    CL_INTERNAL_DM_READ_REMOTE_NAME_REQ,
    CL_INTERNAL_DM_READ_REMOTE_NAME_CANCEL_REQ,
    CL_INTERNAL_DM_READ_LOCAL_NAME_REQ,
    CL_INTERNAL_DM_WRITE_INQUIRY_TX_REQ,
    CL_INTERNAL_DM_READ_INQUIRY_TX_REQ,
    CL_INTERNAL_DM_READ_TX_POWER_REQ,
    CL_INTERNAL_DM_WRITE_INQUIRY_MODE_REQ,
    CL_INTERNAL_DM_READ_INQUIRY_MODE_REQ,
    CL_INTERNAL_DM_WRITE_EIR_DATA_REQ,
    CL_INTERNAL_DM_READ_EIR_DATA_REQ,
    CL_INTERNAL_DM_ACL_FORCE_DETACH_REQ,
    CL_INTERNAL_DM_ACL_OPEN_REQ,
    CL_INTERNAL_DM_ACL_CLOSE_REQ,

    /* Security Entity */
    CL_INTERNAL_SM_INIT_REQ,
    CL_INTERNAL_SM_READ_LOCAL_OOB_DATA_REQ,
    CL_INTERNAL_SM_AUTHENTICATION_REQ,
    CL_INTERNAL_SM_CANCEL_AUTHENTICATION_REQ,
    CL_INTERNAL_SM_AUTHENTICATION_TIMEOUT_IND,
    CL_INTERNAL_SM_SET_SC_MODE_REQ,
    CL_INTERNAL_SM_SET_SSP_SECURITY_LEVEL_REQ,
    CL_INTERNAL_SM_SEC_MODE_CONFIG_REQ,
    CL_INTERNAL_SM_REGISTER_REQ,
    CL_INTERNAL_SM_REGISTER_OUTGOING_REQ,
    CL_INTERNAL_SM_UNREGISTER_REQ,
    CL_INTERNAL_SM_UNREGISTER_OUTGOING_REQ,
    CL_INTERNAL_SM_ENCRYPT_REQ,
    CL_INTERNAL_SM_ENCRYPTION_KEY_REFRESH_REQ,
    CL_INTERNAL_SM_AUTHORISE_RES,
    CL_INTERNAL_SM_PIN_REQUEST_RES,
    CL_INTERNAL_SM_IO_CAPABILITY_REQUEST_RES,
    CL_INTERNAL_SM_USER_CONFIRMATION_REQUEST_RES,
    CL_INTERNAL_SM_USER_PASSKEY_REQUEST_RES,
    CL_INTERNAL_SM_SEND_KEYPRESS_NOTIFICATION_REQ,
    CL_INTERNAL_SM_DELETE_AUTH_DEVICE_REQ,
    CL_INTERNAL_SM_ADD_AUTH_DEVICE_REQ,
    CL_INTERNAL_SM_GET_AUTH_DEVICE_REQ,
    CL_INTERNAL_SM_ADD_AUTH_DEVICE_RAW_REQ,
    CL_INTERNAL_SM_GET_AUTH_DEVICE_RAW_REQ,
    CL_INTERNAL_SM_SET_TRUST_LEVEL_REQ,
    CL_INTERNAL_SM_ADD_DEVICE_AT_TDL_POS_REQ,
    CL_INTERNAL_SM_AUTH_REPLACE_IRK_REQ,

    /* Baseband Entity */
    CL_INTERNAL_DM_READ_CLASS_OF_DEVICE_REQ,
    CL_INTERNAL_DM_WRITE_CLASS_OF_DEVICE_REQ,
    CL_INTERNAL_DM_WRITE_PAGESCAN_ACTIVITY_REQ,
    CL_INTERNAL_DM_WRITE_INQSCAN_ACTIVITY_REQ,
    CL_INTERNAL_DM_WRITE_INQUIRY_SCAN_TYPE_REQ,
    CL_INTERNAL_DM_WRITE_PAGE_SCAN_TYPE_REQ,
    CL_INTERNAL_DM_WRITE_SCAN_ENABLE_REQ,
    CL_INTERNAL_DM_WRITE_CACHED_PAGE_MODE_REQ,
    CL_INTERNAL_DM_WRITE_CACHED_CLK_OFFSET_REQ,
    CL_INTERNAL_DM_CLEAR_PARAM_CACHE_REQ,
    CL_INTERNAL_DM_WRITE_FLUSH_TIMEOUT_REQ,
    CL_INTERNAL_DM_CHANGE_LOCAL_NAME_REQ,
    CL_INTERNAL_DM_WRITE_IAC_LAP_REQ,
    CL_INTERNAL_SM_CHANGE_LINK_KEY_REQ,

    /* Informational Entity*/
    CL_INTERNAL_DM_READ_BD_ADDR_REQ,
    CL_INTERNAL_DM_READ_LINK_QUALITY_REQ,
    CL_INTERNAL_DM_READ_RSSI_REQ,
    CL_INTERNAL_DM_READ_CLK_OFFSET_REQ,
    CL_INTERNAL_DM_SET_BT_VERSION_REQ,
    CL_INTERNAL_DM_READ_REMOTE_SUPP_FEAT_REQ,
    CL_INTERNAL_DM_READ_REMOTE_SUPP_EXT_FEAT_REQ,
    CL_INTERNAL_DM_READ_LOCAL_VERSION_REQ,
    CL_INTERNAL_DM_READ_AFH_CHANNEL_MAP_REQ,
    CL_INTERNAL_DM_READ_REMOTE_VERSION_REQ,

    /* SDP Entity */
    CL_INTERNAL_SDP_REGISTER_RECORD_REQ,
    CL_INTERNAL_SDP_UNREGISTER_RECORD_REQ,
    CL_INTERNAL_SDP_CONFIG_SERVER_MTU_REQ,
    CL_INTERNAL_SDP_CONFIG_CLIENT_MTU_REQ,
    CL_INTERNAL_SDP_OPEN_SEARCH_REQ,
    CL_INTERNAL_SDP_CLOSE_SEARCH_REQ,
    CL_INTERNAL_SDP_SERVICE_SEARCH_REQ,
    CL_INTERNAL_SDP_ATTRIBUTE_SEARCH_REQ,
    CL_INTERNAL_SDP_SERVICE_SEARCH_ATTRIBUTE_REQ,
    CL_INTERNAL_SDP_TERMINATE_PRIMITIVE_REQ,

    /* L2CAP Connection Management Entity */
    CL_INTERNAL_L2CAP_REGISTER_REQ,
    CL_INTERNAL_L2CAP_UNREGISTER_REQ,
    CL_INTERNAL_L2CAP_CONNECT_REQ,
    CL_INTERNAL_L2CAP_CONNECT_RES,
    CL_INTERNAL_L2CAP_MAP_CONNECTIONLESS_REQ,
    CL_INTERNAL_L2CAP_MAP_CONNECTIONLESS_RES,
    CL_INTERNAL_L2CAP_UNMAP_CONNECTIONLESS_REQ,
    CL_INTERNAL_L2CAP_DISCONNECT_REQ,
    CL_INTERNAL_L2CAP_DISCONNECT_RSP,

    /* RFCOMM Connection Management Entity */
    CL_INTERNAL_RFCOMM_REGISTER_REQ,
    CL_INTERNAL_RFCOMM_UNREGISTER_REQ,
    CL_INTERNAL_RFCOMM_CONNECT_REQ,
    CL_INTERNAL_RFCOMM_CONNECT_RES,
    CL_INTERNAL_RFCOMM_DISCONNECT_REQ,
    CL_INTERNAL_RFCOMM_DISCONNECT_RSP,
    CL_INTERNAL_RFCOMM_PORTNEG_REQ,
    CL_INTERNAL_RFCOMM_PORTNEG_RSP,
    CL_INTERNAL_RFCOMM_CONTROL_REQ,
    CL_INTERNAL_RFCOMM_LINE_STATUS_REQ,

    /* Synchronous Connection Entity */
    CL_INTERNAL_SYNC_REGISTER_REQ,
    CL_INTERNAL_SYNC_UNREGISTER_REQ,
    CL_INTERNAL_SYNC_CONNECT_REQ,
    CL_INTERNAL_SYNC_CONNECT_RES,
    CL_INTERNAL_SYNC_DISCONNECT_REQ,
    CL_INTERNAL_SYNC_RENEGOTIATE_REQ,
    CL_INTERNAL_SYNC_REGISTER_TIMEOUT_IND,
    CL_INTERNAL_SYNC_UNREGISTER_TIMEOUT_IND,

    /* Isochronous Connection Entity */
    CL_INTERNAL_ISOC_REGISTER_REQ,
    CL_INTERNAL_ISOC_CIS_CONNECT_REQ,
    CL_INTERNAL_ISOC_CIS_CONNECT_RES,
    CL_INTERNAL_ISOC_CIS_DISCONNECT_REQ,
    CL_INTERNAL_ISOC_CONFIGURE_CIG_REQ,
    CL_INTERNAL_ISOC_REMOVE_CIG_REQ,
    CL_INTERNAL_ISOC_SETUP_ISOCHRONOUS_DATA_PATH_REQ,
    CL_INTERNAL_ISOC_REMOVE_ISO_DATA_PATH_REQ,

    /* BIG-related Internal prims */
    CL_INTERNAL_ISOC_CREATE_BIG_REQ,
    CL_INTERNAL_ISOC_TERMINATE_BIG_REQ,
    CL_INTERNAL_ISOC_BIG_CREATE_SYNC_REQ,

    /* Link Policy Management Entity */
    CL_INTERNAL_DM_SET_ROLE_REQ,
    CL_INTERNAL_DM_GET_ROLE_REQ,
    CL_INTERNAL_DM_SET_LINK_SUPERVISION_TIMEOUT_REQ,
    CL_INTERNAL_DM_SET_LINK_POLICY_REQ,
    CL_INTERNAL_DM_SET_SNIFF_SUB_RATE_POLICY_REQ,
    CL_INTERNAL_DM_WRITE_PAGE_TIMEOUT_REQ,
    CL_INTERNAL_DM_WRITE_LINK_POLICY_SETTINGS_REQ,

    /* DUT support */
    CL_INTERNAL_DM_DUT_REQ,
    CL_INTERNAL_DM_ULP_TRANSMITTER_TEST_REQ,
    CL_INTERNAL_DM_ULP_RECEIVER_TEST_REQ,
    CL_INTERNAL_DM_ULP_TEST_END_REQ,

    /* Attribute read from PS */
    CL_INTERNAL_SM_GET_ATTRIBUTE_REQ,
    CL_INTERNAL_SM_GET_INDEXED_ATTRIBUTE_REQ,

    /* Authenticated Payload Timeout */
    CL_INTERNAL_DM_READ_APT_REQ,
    CL_INTERNAL_DM_WRITE_APT_REQ,

    /* BLE */
    CL_INTERNAL_DM_BLE_CONNECTION_PARAMETERS_UPDATE_REQ,
    CL_INTERNAL_SM_DM_SECURITY_REQ,

    CL_INTERNAL_L2CAP_TP_CONNECT_REQ,
    CL_INTERNAL_L2CAP_TP_CONNECT_RES,
    CL_INTERNAL_L2CAP_ADD_CREDIT_REQ,

    CL_INTERNAL_DM_BLE_READ_AD_CHAN_TX_PWR_REQ,
    CL_INTERNAL_DM_BLE_SET_SCAN_ENABLE_REQ,
    CL_INTERNAL_DM_BLE_SET_ADVERTISE_ENABLE_REQ,
    CL_INTERNAL_DM_BLE_READ_RANDOM_ADDRESS_REQ,

    CL_INTERNAL_MESSAGE_MORE_DATA,

    /* Advertising Extenstions */
    CL_INTERNAL_DM_BLE_GET_ADV_SCAN_CAPABILITIES_REQ,

    /* Extended Advertising */
    CL_INTERNAL_DM_BLE_EXT_ADV_SETS_INFO_REQ,
    CL_INTERNAL_DM_BLE_EXT_ADV_REGISTER_APP_ADV_SET_REQ,
    CL_INTERNAL_DM_BLE_EXT_ADV_UNREGISTER_APP_ADV_SET_REQ,
    CL_INTERNAL_DM_BLE_EXT_ADVERTISE_ENABLE_REQ,
    CL_INTERNAL_DM_BLE_EXT_ADV_MULTI_ENABLE_REQ,
    CL_INTERNAL_DM_BLE_EXT_ADV_SET_PARAMS_REQ,
    CL_INTERNAL_DM_BLE_EXT_ADV_SET_RANDOM_ADDRESS_REQ,
    CL_INTERNAL_DM_BLE_EXT_ADV_SET_DATA_REQ,
    CL_INTERNAL_DM_BLE_EXT_ADV_SET_SCAN_RESP_DATA_REQ,
    CL_INTERNAL_DM_BLE_EXT_ADV_READ_MAX_ADV_DATA_LEN_REQ,

    /* Extended Scanning */
    CL_INTERNAL_DM_BLE_EXT_SCAN_ENABLE_REQ,
    CL_INTERNAL_DM_BLE_EXT_SCAN_GET_GLOBAL_PARAMS_REQ,
    CL_INTERNAL_DM_BLE_EXT_SCAN_SET_GLOBAL_PARAMS_REQ,
    CL_INTERNAL_DM_BLE_EXT_SCAN_REGISTER_SCANNER_REQ,
    CL_INTERNAL_DM_BLE_EXT_SCAN_UNREGISTER_SCANNER_REQ,
    CL_INTERNAL_DM_BLE_EXT_SCAN_ADV_REPORT_DONE_IND,
    CL_INTERNAL_DM_BLE_EXT_SCAN_GET_CTRL_SCAN_INFO_REQ,
    CL_INTERNAL_DM_BLE_EXT_SCAN_FILTERED_ADV_REPORT_IND, /* Message ID only - no struct */

    /* Periodic Advertising */
    CL_INTERNAL_DM_BLE_PER_ADV_SET_PARAMS_REQ,
    CL_INTERNAL_DM_BLE_PER_ADV_SET_DATA_REQ,
    CL_INTERNAL_DM_BLE_PER_ADV_START_REQ,
    CL_INTERNAL_DM_BLE_PER_ADV_STOP_REQ,
    CL_INTERNAL_DM_BLE_PER_ADV_SET_TRANSFER_REQ,
    CL_INTERNAL_DM_BLE_PER_ADV_READ_MAX_ADV_DATA_LEN_REQ,

    /* Periodic Scanning */
    CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TO_TRAIN_REQ,
    CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TERMINATE_REQ,
    CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TRANSFER_REQ,
    CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TRANSFER_PARAMS_REQ,
    CL_INTERNAL_DM_BLE_PERIODIC_SCAN_START_FIND_TRAINS_REQ,
    CL_INTERNAL_DM_BLE_PERIODIC_SCAN_STOP_FIND_TRAINS_REQ,
    CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_ADV_REPORT_ENABLE_REQ,
    CL_INTERNAL_DM_BLE_PER_SCAN_ADV_REPORT_DONE_IND,
    CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_ADV_REPORT_IND, /* Message ID only - no struct */

    CL_INTERNAL_DM_SC_OVERRIDE_REQ,
    CL_INTERNAL_DM_SC_OVERRIDE_MAX_BDADDR_REQ,
    CL_INTERNAL_DM_SET_LINK_BEHAVIOR_REQ,

    /*LE Power Control*/
    CL_INTERNAL_DM_ULP_ENHANCED_READ_TRANSMIT_POWER_LEVEL_REQ,
    CL_INTERNAL_DM_ULP_READ_REMOTE_TRANSMIT_POWER_LEVEL_REQ,
    CL_INTERNAL_DM_ULP_SET_PATH_LOSS_REPORTING_PARAMETERS_REQ,
    CL_INTERNAL_DM_ULP_SET_PATH_LOSS_REPORTING_ENABLE_REQ,
    CL_INTERNAL_DM_ULP_SET_TRANSMIT_POWER_REPORTING_ENABLE_REQ,
    CL_ULP_PATH_LOSS_THRESHOLD_IND,
    CL_ULP_TRANSMIT_POWER_REPORTING_IND,
    
    /* New */
    CL_INTERNAL_DM_ULP_PERIODIC_ADV_ENABLE_REQ,
    CL_INTERNAL_DM_BLE_SET_DATA_RELATED_ADDRESS_CHANGES_REQ,

} CL_INTERNAL_T;


typedef struct
{
    connectionInitState   state;
} CL_INTERNAL_INIT_CFM_T;

typedef struct
{
    Task    theAppTask;
    uint32  inquiry_lap;
    uint8   max_responses;
    uint8   timeout;
    uint32  class_of_device;
    uint16  min_period;
    uint16  max_period;
} CL_INTERNAL_DM_INQUIRY_REQ_T;

typedef struct
{
    Task theAppTask;
} CL_INTERNAL_DM_INQUIRY_CANCEL_REQ_T;

typedef struct
{
    Task theAppTask;
    bdaddr bd_addr;
} CL_INTERNAL_DM_READ_REMOTE_NAME_REQ_T;

typedef struct
{
    Task theAppTask;
    bdaddr bd_addr;
} CL_INTERNAL_DM_READ_REMOTE_NAME_CANCEL_REQ_T;

typedef struct
{
    Task theAppTask;
} CL_INTERNAL_DM_READ_LOCAL_NAME_REQ_T;

typedef CL_INTERNAL_DM_READ_REMOTE_NAME_REQ_T CL_INTERNAL_DM_READ_AFH_CHANNEL_MAP_REQ_T;

typedef struct
{
    Task theAppTask;
    int8 tx_power;
} CL_INTERNAL_DM_WRITE_INQUIRY_TX_REQ_T;

typedef struct
{
    Task theAppTask;
} CL_INTERNAL_DM_READ_INQUIRY_TX_REQ_T;

typedef struct
{
    Task theAppTask;
    tp_bdaddr tpaddr;
} CL_INTERNAL_DM_READ_TX_POWER_REQ_T;

typedef struct
{
    Task theAppTask;
    inquiry_mode mode;
} CL_INTERNAL_DM_WRITE_INQUIRY_MODE_REQ_T;

typedef struct
{
    uint8 fec_required;
    uint8 size_eir_data;
    uint8 *eir_data;
} CL_INTERNAL_DM_WRITE_EIR_DATA_REQ_T;

typedef struct
{
    Task task;
} CL_INTERNAL_DM_READ_EIR_DATA_REQ_T;

typedef struct
{
    Task theAppTask;
} CL_INTERNAL_DM_READ_INQUIRY_MODE_REQ_T;

typedef struct
{
    bdaddr bd_addr;
} CL_INTERNAL_DM_ACL_OPEN_REQ_T;

typedef struct
{
    bdaddr bd_addr;
    uint16 flags;
    uint8 reason;
} CL_INTERNAL_DM_ACL_CLOSE_REQ_T;

typedef struct
{
    bdaddr bd_addr;
    uint16 flags;
    uint8 reason;
} CL_INTERNAL_DM_ACL_FORCE_DETACH_REQ_T;

typedef struct
{
    Task                theAppTask;
    uint16              options;
    dm_security_mode    security_mode;
    uint16              config;
    cl_sm_wae           write_auth_enable;
    encryption_mode     mode3_enc;
} CL_INTERNAL_SM_INIT_REQ_T;

typedef struct
{
    Task task;
    bdaddr bd_addr;
    uint32  timeout;
} CL_INTERNAL_SM_AUTHENTICATION_REQ_T;

typedef struct
{
    Task            task;
    TRANSPORT_T     transport;
} CL_INTERNAL_SM_READ_LOCAL_OOB_DATA_REQ_T;

typedef struct
{
    Task task;
    bool force;
} CL_INTERNAL_SM_CANCEL_AUTHENTICATION_REQ_T;

typedef struct
{
    Task theAppTask;
    bdaddr bd_addr;
} CL_INTERNAL_SM_AUTHENTICATION_TIMEOUT_IND_T;

typedef struct
{
    Task                theAppTask;
    dm_security_mode    mode;
    encryption_mode     mode3_enc;
} CL_INTERNAL_SM_SET_SC_MODE_REQ_T;

typedef struct
{
    dm_protocol_id          protocol_id;
    uint16                  channel;
    dm_ssp_security_level   ssp_sec_level;
    bool                    outgoing_ok;
    bool                    authorised;
    bool                    disable_legacy;
} CL_INTERNAL_SM_SET_SSP_SECURITY_LEVEL_REQ_T;

typedef struct
{
    Task            theAppTask;
    cl_sm_wae       write_auth_enable;
    bool            debug_keys;
    bool            legacy_auto_pair_key_missing;
} CL_INTERNAL_SM_SEC_MODE_CONFIG_REQ_T;

typedef struct
{
    dm_protocol_id      protocol_id;
    uint16              channel;
    bool                outgoing_ok;
    dm_security_in      security_level;
    uint16               psm;
} CL_INTERNAL_SM_REGISTER_REQ_T;

typedef struct
{
    dm_protocol_id  protocol_id;
    uint16          channel;
} CL_INTERNAL_SM_UNREGISTER_REQ_T;

typedef struct
{
    Task                theAppTask;
    bdaddr              bd_addr;
    dm_protocol_id      protocol_id;
    uint16              remote_channel;
    dm_security_out     outgoing_security_level;
} CL_INTERNAL_SM_REGISTER_OUTGOING_REQ_T;

typedef struct
{
    bdaddr           bd_addr;
    dm_protocol_id   protocol_id;
    uint16           channel;
} CL_INTERNAL_SM_UNREGISTER_OUTGOING_REQ_T;

typedef struct
{
    Task    theAppTask;
    Sink    sink;
    bool    encrypt;
} CL_INTERNAL_SM_ENCRYPT_REQ_T;

typedef struct
{
    typed_bdaddr            taddr;
} CL_INTERNAL_SM_ENCRYPTION_KEY_REFRESH_REQ_T;

typedef struct
{
    typed_bdaddr            taddr;
    uint8                   pin_length;
    uint8                   pin[HCI_MAX_PIN_LENGTH];
} CL_INTERNAL_SM_PIN_REQUEST_RES_T;

typedef struct
{
    tp_bdaddr               tpaddr;
    cl_sm_io_capability     io_capability;
    bool                    bonding;
    mitm_setting            mitm;
    uint16                  key_distribution;
    uint8                   oob_data;
    uint8*                  oob_hash_c;
    uint8*                  oob_rand_r;
} CL_INTERNAL_SM_IO_CAPABILITY_REQUEST_RES_T;

typedef struct
{
    tp_bdaddr           tpaddr;
    bool                confirm;
} CL_INTERNAL_SM_USER_CONFIRMATION_REQUEST_RES_T;

typedef struct
{
    tp_bdaddr           tpaddr;
    bool                cancelled;
    uint32              numeric_value;
} CL_INTERNAL_SM_USER_PASSKEY_REQUEST_RES_T;

typedef struct
{
    tp_bdaddr           tpaddr;
    cl_sm_keypress_type type;
} CL_INTERNAL_SM_SEND_KEYPRESS_NOTIFICATION_REQ_T;

typedef struct
{
    Task                theAppTask;
    bdaddr              bd_addr;
    bool                trusted;
} CL_INTERNAL_SM_SET_TRUST_LEVEL_REQ_T;

typedef struct
{
    bdaddr           bd_addr;
    dm_protocol_id   protocol_id;
    uint32           channel;
    bool             incoming;
    bool             authorised;
} CL_INTERNAL_SM_AUTHORISE_RES_T;

typedef struct
{
    Task                    theAppTask;
    bdaddr                  bd_addr;
    DM_SM_KEY_ENC_BREDR_T   enc_bredr;
    uint16                  trusted;
    uint16                  bonded;
} CL_INTERNAL_SM_ADD_AUTH_DEVICE_REQ_T;

typedef struct
{
    Task                    theAppTask;
    typed_bdaddr            taddr;
    uint16                  size_data;
    uint16                  data[1];
} CL_INTERNAL_SM_ADD_AUTH_DEVICE_RAW_REQ_T;

typedef struct
{
    Task            theAppTask;
    bdaddr          bd_addr;
} CL_INTERNAL_SM_GET_AUTH_DEVICE_REQ_T;

typedef struct
{
    Task            theAppTask;
    typed_bdaddr    taddr;
} CL_INTERNAL_SM_GET_AUTH_DEVICE_RAW_REQ_T;

typedef struct
{
    Task            theAppTask;
    bdaddr          bd_addr;
    cl_irk          new_irk;
} CL_INTERNAL_SM_AUTH_REPLACE_IRK_REQ_T;

typedef struct
{
    Task theAppTask;
} CL_INTERNAL_DM_READ_CLASS_OF_DEVICE_REQ_T;

typedef struct
{
    uint32 class_of_device;
} CL_INTERNAL_DM_WRITE_CLASS_OF_DEVICE_REQ_T;

typedef struct
{
    uint16 ps_interval;
    uint16 ps_window;
} CL_INTERNAL_DM_WRITE_PAGESCAN_ACTIVITY_REQ_T;

typedef struct
{
    uint16 is_interval;
    uint16 is_window;
} CL_INTERNAL_DM_WRITE_INQSCAN_ACTIVITY_REQ_T;

typedef struct
{
    hci_scan_type type;
} CL_INTERNAL_DM_WRITE_INQUIRY_SCAN_TYPE_REQ_T;

typedef struct
{
    hci_scan_type type;
} CL_INTERNAL_DM_WRITE_PAGE_SCAN_TYPE_REQ_T;
typedef struct
{
    hci_scan_enable mode;
} CL_INTERNAL_DM_WRITE_SCAN_ENABLE_REQ_T;

typedef struct
{
    bdaddr              bd_addr;
    page_scan_mode      ps_mode;
    page_scan_rep_mode  ps_rep_mode;
} CL_INTERNAL_DM_WRITE_CACHED_PAGE_MODE_REQ_T;

typedef struct
{
    bdaddr  bd_addr;
    uint16  clock_offset;
} CL_INTERNAL_DM_WRITE_CACHED_CLK_OFFSET_REQ_T;

typedef struct
{
    bdaddr  bd_addr;
} CL_INTERNAL_DM_CLEAR_PARAM_CACHE_REQ_T;

typedef struct
{
    Sink    sink;
    uint16  flush_timeout;
} CL_INTERNAL_DM_WRITE_FLUSH_TIMEOUT_REQ_T;

typedef struct
{
    uint16  length_name;
    uint8   *name;
} CL_INTERNAL_DM_CHANGE_LOCAL_NAME_REQ_T;

typedef struct
{
    Task theAppTask;
    uint16 num_iac;
    uint32 iac[1];
} CL_INTERNAL_DM_WRITE_IAC_LAP_REQ_T;

typedef struct
{
    Task theAppTask;
} CL_INTERNAL_DM_READ_BD_ADDR_REQ_T;

typedef struct
{
    Task theAppTask;
    Sink sink;
    bdaddr bd_addr;            /* only used if sink == NULL */
} CL_INTERNAL_DM_READ_LINK_QUALITY_REQ_T;

typedef struct
{
    Task theAppTask;
    Sink sink;
    tp_bdaddr tpaddr;         /* only used if sink == NULL */
} CL_INTERNAL_DM_READ_RSSI_REQ_T;

typedef struct
{
    Task    theAppTask;
    Sink    sink;
} CL_INTERNAL_DM_READ_CLK_OFFSET_REQ_T;

typedef struct
{
    Task                theAppTask;
    uint8   version;
} CL_INTERNAL_DM_SET_BT_VERSION_REQ_T;

typedef struct
{
    Task    theAppTask;
    Sink    sink;
} CL_INTERNAL_DM_READ_REMOTE_SUPP_FEAT_REQ_T;

typedef struct
{
    Task    theAppTask;
    bdaddr  bd_addr;
    uint16  page_num;
} CL_INTERNAL_DM_READ_REMOTE_SUPP_EXT_FEAT_REQ_T;

typedef struct
{
    Task         theAppTask;
    tp_bdaddr    tpaddr;
} CL_INTERNAL_DM_READ_REMOTE_VERSION_REQ_T;

typedef struct
{
    Task    theAppTask;
} CL_INTERNAL_DM_READ_LOCAL_VERSION_REQ_T;

typedef struct
{
    Sink    sink;
} CL_INTERNAL_SM_CHANGE_LINK_KEY_REQ_T;

#ifndef CL_EXCLUDE_SDP
typedef struct
{
    Task    theAppTask;
    uint16  record_length;
    uint8   *record;
} CL_INTERNAL_SDP_REGISTER_RECORD_REQ_T;

typedef struct
{
    Task    theAppTask;
    uint32  service_handle;
} CL_INTERNAL_SDP_UNREGISTER_RECORD_REQ_T;

typedef struct
{
    uint16 mtu;
} CL_INTERNAL_SDP_CONFIG_SERVER_MTU_REQ_T;

typedef struct
{
    uint16 mtu;
} CL_INTERNAL_SDP_CONFIG_CLIENT_MTU_REQ_T;

typedef struct
{
    Task        theAppTask;
    bdaddr      bd_addr;
} CL_INTERNAL_SDP_OPEN_SEARCH_REQ_T;

typedef struct
{
    Task    theAppTask;
} CL_INTERNAL_SDP_CLOSE_SEARCH_REQ_T;

typedef struct
{
    Task    theAppTask;
    bdaddr  bd_addr;
    uint16    max_responses;
    uint16    flags;
    uint16  length;
    uint8     search_pattern[1];
} CL_INTERNAL_SDP_SERVICE_SEARCH_REQ_T;

typedef struct
{
    Task    theAppTask;
    bdaddr  bd_addr;
    uint32  service_handle;
    uint16  size_attribute_list;
    uint16  max_num_attr;
    uint16  flags;
    uint8   attribute_list[1];
} CL_INTERNAL_SDP_ATTRIBUTE_SEARCH_REQ_T;

typedef struct
{
    Task    theAppTask;
    bdaddr  bd_addr;
    uint16  max_num_attributes;
    uint16  size_search_pattern;
    uint16  size_attribute_list;
    uint16  flags;
    uint8   search_attr[1];
} CL_INTERNAL_SDP_SERVICE_SEARCH_ATTRIBUTE_REQ_T;

typedef struct
{
    Task theAppTask;
} CL_INTERNAL_SDP_TERMINATE_PRIMITIVE_REQ_T;
#endif

#ifndef CL_EXCLUDE_L2CAP
typedef struct
{
    Task                        clientTask;
    uint16                      app_psm;
    uint16                      flags;
} CL_INTERNAL_L2CAP_REGISTER_REQ_T;

typedef struct
{
    Task    theAppTask;
    uint16  app_psm;
} CL_INTERNAL_L2CAP_UNREGISTER_REQ_T;

typedef struct
{
    Task                theAppTask;
    bdaddr              bd_addr;
    uint16              psm_local;
    uint16              psm_remote;
    uint16              conftab_length;
    const uint16       *conftab;
} CL_INTERNAL_L2CAP_CONNECT_REQ_T;

typedef struct
{
    Task                theAppTask;
    tp_bdaddr           taddr;
    uint16              psm_local;
    uint16              psm_remote;
    uint16              length;
    uint16              *data;
} CL_INTERNAL_L2CAP_TP_CONNECT_REQ_T;

typedef struct
{
    Task                theAppTask;
    bool                response;
    uint8               identifier;
    uint16              psm_local;
    uint16              connection_id;
    uint16              conftab_length;
    const uint16       *conftab;
} CL_INTERNAL_L2CAP_CONNECT_RES_T;

typedef struct
{
    Task                theAppTask;
    bool                response;
    uint8               identifier;
    uint16              psm_local;
    uint16              connection_id;
    uint16              length;
    uint16              *data;
} CL_INTERNAL_L2CAP_TP_CONNECT_RES_T;

typedef struct
{
    Task                theAppTask;
    uint16              connection_id;
    uint16              credits;
} CL_INTERNAL_L2CAP_ADD_CREDIT_REQ_T;

typedef struct
{
    Task                            theAppTask;
    bdaddr                          bd_addr;
    uint16                          psm_local;
    uint16                          psm_remote;
    l2cap_connectionless_data_type  type;
} CL_INTERNAL_L2CAP_MAP_CONNECTIONLESS_REQ_T;

typedef struct
{
    Task                            theAppTask;
    Source                          source;
    l2cap_connectionless_data_type  type;
} CL_INTERNAL_L2CAP_MAP_CONNECTIONLESS_RES_T;

typedef struct
{
    Sink                            sink;
} CL_INTERNAL_L2CAP_UNMAP_CONNECTIONLESS_REQ_T;

typedef struct
{
    Task    theAppTask;
    Sink    sink;
} CL_INTERNAL_L2CAP_DISCONNECT_REQ_T;

typedef struct
{
    uint8                 identifier;
    Sink                  sink;
} CL_INTERNAL_L2CAP_DISCONNECT_RSP_T;
#endif

#ifndef DISABLE_BLE
typedef struct
{
    Task                theAppTask;
    typed_bdaddr        taddr;
    uint16              min_interval;
    uint16              max_interval;
    uint16              latency;
    uint16              timeout;
    uint16              min_ce_length;
    uint16              max_ce_length;
} CL_INTERNAL_DM_BLE_CONNECTION_PARAMETERS_UPDATE_REQ_T;
#endif

typedef struct
{
    Task                    theAppTask;
    uint8                   suggested_server_channel;
} CL_INTERNAL_RFCOMM_REGISTER_REQ_T;

#ifndef CL_EXCLUDE_RFCOMM
typedef struct
{
    Task                        theAppTask;
    uint8                   local_server_channel;
} CL_INTERNAL_RFCOMM_UNREGISTER_REQ_T;

typedef struct
{
    Task                    theAppTask;
    bdaddr                  bd_addr;
    uint8                   remote_server_channel;
    uint16                  security_channel;
    rfcomm_config_params    config;
} CL_INTERNAL_RFCOMM_CONNECT_REQ_T;

typedef struct
{
    Task                    theAppTask;
    bool                    response;
    Sink                    sink;
    uint8                   server_channel;
    rfcomm_config_params    config;

} CL_INTERNAL_RFCOMM_CONNECT_RES_T;

typedef struct
{
    Task                    theAppTask;
    Sink                    sink;
} CL_INTERNAL_RFCOMM_DISCONNECT_REQ_T;

typedef struct
{
    Sink                    sink;
} CL_INTERNAL_RFCOMM_DISCONNECT_RSP_T;

typedef struct
{
    Task    theAppTask;
    Sink        sink;
    bool        request;
    port_par    port_params;
} CL_INTERNAL_RFCOMM_PORTNEG_REQ_T;

typedef struct
{
    Task               theAppTask;
    Sink        sink;
    port_par    port_params;
} CL_INTERNAL_RFCOMM_PORTNEG_RSP_T;

typedef struct
{
    Task                theAppTask;
    Sink                sink;
    uint8               break_signal;
    uint8               modem_signal;
} CL_INTERNAL_RFCOMM_CONTROL_REQ_T;

typedef struct
{
    Task                        theAppTask;
    Sink                        sink;
    bool                        error;
    rfcomm_line_status_error    lines_status;
} CL_INTERNAL_RFCOMM_LINE_STATUS_REQ_T;

#endif

#ifndef CL_EXCLUDE_SYNC
typedef struct
{
    Task theAppTask;
} CL_INTERNAL_SYNC_REGISTER_REQ_T;

typedef struct
{
    Task theAppTask;
} CL_INTERNAL_SYNC_UNREGISTER_REQ_T;

typedef struct
{
    Task                     theAppTask;
    Sink                     sink;
    sync_config_params       config_params;
} CL_INTERNAL_SYNC_CONNECT_REQ_T;

typedef struct
{
    Task                theAppTask;
    bdaddr              bd_addr;
    bool                response;
    sync_config_params  config_params;
} CL_INTERNAL_SYNC_CONNECT_RES_T;

typedef struct
{
    Sink                     audio_sink;
    hci_status               reason;
} CL_INTERNAL_SYNC_DISCONNECT_REQ_T;

typedef struct
{
    Task                     theAppTask;
    Sink                     audio_sink;
    sync_config_params       config_params;
} CL_INTERNAL_SYNC_RENEGOTIATE_REQ_T;


typedef struct
{
    Task                     theAppTask;
} CL_INTERNAL_SYNC_REGISTER_TIMEOUT_IND_T;

typedef struct
{
    Task                     theAppTask;
} CL_INTERNAL_SYNC_UNREGISTER_TIMEOUT_IND_T;
#endif

#ifndef CL_EXCLUDE_ISOC
typedef struct
{
    Task    theAppTask;
    uint16  isoc_type;
} CL_INTERNAL_ISOC_REGISTER_REQ_T;

typedef struct
{
    Task    theAppTask;
    uint8   cis_count;
    CL_DM_CIS_CONNECTION_T  *cis_conn[CL_DM_MAX_SUPPORTED_CIS];
} CL_INTERNAL_ISOC_CIS_CONNECT_REQ_T;

typedef struct
{
    Task    theAppTask;
    uint8   status;
    uint16  cis_handle;
} CL_INTERNAL_ISOC_CIS_CONNECT_RES_T;

typedef struct
{
    Task    theAppTask;
    uint16  cis_handle;
    uint8   reason;
} CL_INTERNAL_ISOC_CIS_DISCONNECT_REQ_T;

typedef struct
{
    Task    theAppTask;
    uint16  cis_handle;
    uint8   data_path_direction;
    uint8   data_path_id;
    uint8   codec_id[ISOC_CODEC_ID_SIZE];
    uint32  controller_delay;
    uint8   codec_config_length;
    uint8   *codec_config_data;

}  CL_INTERNAL_ISOC_SETUP_ISOCHRONOUS_DATA_PATH_REQ_T;

typedef struct
{
    Task        theAppTask;
    uint16      handle;        /* CIS or BIS handle */
    uint8       data_path_direction; /* Direction of the path to be removed */
} CL_INTERNAL_ISOC_REMOVE_ISO_DATA_PATH_REQ_T;

typedef struct
{
    Task                theAppTask;
    uint32              sdu_interval_m_to_s; /* Time interval between the start of consecutive SDUs */
    uint32              sdu_interval_s_to_m; /* Time interval between the start of consecutive SDUs */
    uint16              max_transport_latency_m_to_s;   /* Maximum transport latency from master */
    uint16              max_transport_latency_s_to_m;   /* Maximum transport latency from slave */
    uint8               cig_id;              /* Used to identify the CIG */
    uint8               sca;                 /* Sleep clock accuracy */
    uint8               packing;             /* Interleaved, Sequential placement of packets */
    uint8               framing;             /* Indicates the format: framed or unframed */
    uint8               cis_count;           /* Number of CIS under CIG */
    CL_DM_CIS_CONFIG_T  *cis_config[CL_DM_MAX_SUPPORTED_CIS];
                                                /* Array of pointers to cis configuration */
} CL_INTERNAL_ISOC_CONFIGURE_CIG_REQ_T;

typedef struct
{
    Task    theAppTask;
    uint8   cig_id;      /* Identifier of CIG to be removed */
} CL_INTERNAL_ISOC_REMOVE_CIG_REQ_T;

typedef struct
{
    Task                        theAppTask;
    CL_DM_BIG_CONFIG_PARAM_T    big_config;     /* Big Params */
    uint8                       big_handle;     /* Host identifier of BIG */
    uint8                       adv_handle;     /* Handle associated with PA */
    uint8                       num_bis;        /* Total number of BISes in BIG */
    uint8                       encryption;    /* Encrypted BIS data if 1 */
    uint8                       broadcast_code[BROADCAST_CODE_SIZE];
                                      /* Broadcast code to enc BIS payloads */
} CL_INTERNAL_ISOC_CREATE_BIG_REQ_T;

typedef struct
{
    Task    theAppTask;
    uint8   big_handle;     /* Host identifier of BIG */
    uint8   reason;        /* Reason for BIG termination */
} CL_INTERNAL_ISOC_TERMINATE_BIG_REQ_T;

typedef struct
{
    Task    theAppTask;
    uint8   big_handle;    /* Host identifier of BIG */
    uint16  sync_handle;   /* Sync handle of the PA */
    uint8   encryption;    /* Encryption mode of the BIG */
    uint8   broadcast_code[BROADCAST_CODE_SIZE]; /* Broadcast code for encryption */
    uint8   mse;           /* Maximum sub events */
    uint16  big_sync_timeout; /* Sync timeout of BIS PDUs */
    uint8   num_bis;       /* Num of BISes requested */
    uint8   bis[1];          /*Indices corresponding to BISes */
} CL_INTERNAL_ISOC_BIG_CREATE_SYNC_REQ_T;
#endif /* CL_EXCLUDE_ISOC */

typedef struct
{
    Task        theAppTask;
    Sink        sink;
    bdaddr      bd_addr;            /* only used if sink == NULL */
    hci_role    role;
} CL_INTERNAL_DM_SET_ROLE_REQ_T;

typedef struct
{
    Task        theAppTask;
    Sink        sink;
    bdaddr      bd_addr;            /* only used if sink == NULL */
} CL_INTERNAL_DM_GET_ROLE_REQ_T;

typedef struct
{
    Sink        sink;
    uint16      timeout;
} CL_INTERNAL_DM_SET_LINK_SUPERVISION_TIMEOUT_REQ_T;

typedef struct
{
    Sink        sink;
    uint16      size_power_table;
    lp_power_table const *power_table;
} CL_INTERNAL_DM_SET_LINK_POLICY_REQ_T;

typedef struct
{
    bdaddr      bd_addr;
    uint16      link_policy_settings;
} CL_INTERNAL_DM_WRITE_LINK_POLICY_SETTINGS_REQ_T;

typedef struct
{
    Sink        sink;
    uint16      max_remote_latency;
    uint16      min_remote_timeout;
    uint16      min_local_timeout;
} CL_INTERNAL_DM_SET_SNIFF_SUB_RATE_POLICY_REQ_T;

typedef struct
{
    uint16 timeout;
} CL_INTERNAL_DM_WRITE_PAGE_TIMEOUT_REQ_T;

typedef struct
{
    uint8   addr_type;
    bdaddr  bd_addr;
    uint16  ps_base;
    uint16  size_psdata;
} CL_INTERNAL_SM_GET_ATTRIBUTE_REQ_T;

typedef struct
{
    uint16  index;
    uint16  ps_base;
    uint16  size_psdata;
} CL_INTERNAL_SM_GET_INDEXED_ATTRIBUTE_REQ_T;

typedef struct
{
    Task        theAppTask;
    tp_bdaddr   tpaddr;
} CL_INTERNAL_DM_READ_APT_REQ_T;

typedef struct
{
    Task                theAppTask;
    tp_bdaddr           tpaddr;
    uint16              apt;
    cl_apt_route_event  apt_route_event;
} CL_INTERNAL_DM_WRITE_APT_REQ_T;

#ifndef DISABLE_BLE
typedef struct
{
    Task                theAppTask;
    typed_bdaddr        taddr;
    ble_security_type   security;
    ble_connection_type conn_type;
} CL_INTERNAL_SM_DM_SECURITY_REQ_T;

typedef struct
{
    Task theAppTask;
} CL_INTERNAL_DM_BLE_READ_AD_CHAN_TX_PWR_REQ_T;

typedef struct
{
    uint8               tx_channel;
    uint8               length_test_data;
    uint8               packet_payload;
} CL_INTERNAL_DM_ULP_TRANSMITTER_TEST_REQ_T;

typedef struct
{
    uint8               rx_channel;
} CL_INTERNAL_DM_ULP_RECEIVER_TEST_REQ_T;

typedef struct
{
    Task                theAppTask;
    bool                enable;
} CL_INTERNAL_DM_BLE_SET_SCAN_ENABLE_REQ_T;

typedef CL_INTERNAL_DM_BLE_SET_SCAN_ENABLE_REQ_T
            CL_INTERNAL_DM_BLE_SET_ADVERTISE_ENABLE_REQ_T;

typedef struct
{
    Task                            theAppTask;
    ble_read_random_address_flags   flags;
    tp_bdaddr                       *peer_tpaddr;
} CL_INTERNAL_DM_BLE_READ_RANDOM_ADDRESS_REQ_T;

typedef CL_INTERNAL_DM_BLE_READ_AD_CHAN_TX_PWR_REQ_T CL_INTERNAL_DM_BLE_GET_ADV_SCAN_CAPABILITIES_REQ_T;

typedef struct
{
    Task    theAppTask;
    uint16  sync_handle;
    uint8   enable;
} CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_ADV_REPORT_ENABLE_REQ_T;

typedef struct
{
    Task        theAppTask;
    bool        enable;
    uint8       num_of_scanners;
    uint16      scan_handle[DM_ULP_EXT_SCAN_MAX_SCANNERS];
    uint16      duration[DM_ULP_EXT_SCAN_MAX_SCANNERS];
} CL_INTERNAL_DM_BLE_EXT_SCAN_ENABLE_REQ_T;

typedef CL_INTERNAL_DM_BLE_READ_AD_CHAN_TX_PWR_REQ_T CL_INTERNAL_DM_BLE_EXT_ADV_SETS_INFO_REQ_T;

typedef struct
{
    Task        theAppTask;
    uint8       adv_handle;
    uint32      flags;
} CL_INTERNAL_DM_BLE_EXT_ADV_REGISTER_APP_ADV_SET_REQ_T;

typedef struct
{
    Task        theAppTask;
    uint8       adv_handle;
} CL_INTERNAL_DM_BLE_EXT_ADV_UNREGISTER_APP_ADV_SET_REQ_T;

typedef struct
{
    Task        theAppTask;
    bool        enable;
    uint8       adv_handle;
} CL_INTERNAL_DM_BLE_EXT_ADVERTISE_ENABLE_REQ_T;

typedef struct
{
    Task                theAppTask;
    uint8               enable;
    uint8               num_sets;
    CL_EA_ENABLE_CONFIG_T config[CL_DM_BLE_EXT_ADV_MAX_NUM_ENABLE];
} CL_INTERNAL_DM_BLE_EXT_ADV_MULTI_ENABLE_REQ_T;

typedef struct
{
    Task            theAppTask;
    uint8           adv_handle;
    uint16          adv_event_properties;
    uint32          primary_adv_interval_min;
    uint32          primary_adv_interval_max;
    uint8           primary_adv_channel_map;
    uint8           own_addr_type;
    typed_bdaddr    taddr;
    uint8           adv_filter_policy;
    uint16          primary_adv_phy;
    uint8           secondary_adv_max_skip;
    uint16          secondary_adv_phy;
    uint16          adv_sid;
    uint32          reserved;
} CL_INTERNAL_DM_BLE_EXT_ADV_SET_PARAMS_REQ_T;

typedef struct
{
    Task                    theAppTask;
    uint8                   adv_handle;
    ble_local_addr_type     action;
    bdaddr                  random_addr;
} CL_INTERNAL_DM_BLE_EXT_ADV_SET_RANDOM_ADDRESS_REQ_T;

typedef struct
{
    Task                    theAppTask;
    uint8                   adv_handle;
    set_data_req_operation  operation;
    uint8                   adv_data_len;
    uint8                   *adv_data[8];
} CL_INTERNAL_DM_BLE_EXT_ADV_SET_DATA_REQ_T;

typedef struct
{
    Task                    theAppTask;
    uint8                   adv_handle;
    set_data_req_operation  operation;
    uint8                   scan_resp_data_len;
    uint8                   *scan_resp_data[8];
} CL_INTERNAL_DM_BLE_EXT_ADV_SET_SCAN_RESP_DATA_REQ_T;

typedef struct
{
    Task                    theAppTask;
    uint8                   adv_handle;
} CL_INTERNAL_DM_BLE_EXT_ADV_READ_MAX_ADV_DATA_LEN_REQ_T;

typedef struct
{
    Task            theAppTask;
    uint8           adv_handle;
    uint32          flags;
    uint16          periodic_adv_interval_min;
    uint16          periodic_adv_interval_max;
    uint16          periodic_adv_properties;
} CL_INTERNAL_DM_BLE_PER_ADV_SET_PARAMS_REQ_T;

typedef struct
{
    Task                    theAppTask;
    uint8                   adv_handle;
    set_data_req_operation  operation;
    uint8                   adv_data_len;
    uint8                   *adv_data[8];
} CL_INTERNAL_DM_BLE_PER_ADV_SET_DATA_REQ_T;


typedef struct
{
    Task    theAppTask;
    uint8   adv_handle;
} CL_INTERNAL_DM_BLE_PER_ADV_START_REQ_T;

typedef struct
{
    Task  theAppTask;
    uint8 adv_handle;
    uint8 stop_advertising;
} CL_INTERNAL_DM_BLE_PER_ADV_STOP_REQ_T;

typedef struct
{
    Task            theAppTask;
    typed_bdaddr    taddr;
    uint16          service_data;
    uint8           adv_handle;
} CL_INTERNAL_DM_BLE_PER_ADV_SET_TRANSFER_REQ_T;

typedef struct
{
    Task                    theAppTask;
    uint8                   adv_handle;
} CL_INTERNAL_DM_BLE_PER_ADV_READ_MAX_ADV_DATA_LEN_REQ_T;

typedef struct
{
    Task                    theAppTask;
    uint8                   flags;
    uint8                   own_address_type;
    uint8                   scanning_filter_policy;
    uint8                   filter_duplicates;
    uint8                   scanning_phys;
    CL_ES_SCANNING_PHY_T    phy_params[EXT_SCAN_MAX_SCANNING_PHYS];
} CL_INTERNAL_DM_BLE_EXT_SCAN_SET_GLOBAL_PARAMS_REQ_T;

typedef struct
{
    Task                    theAppTask;
} CL_INTERNAL_DM_BLE_EXT_SCAN_GET_GLOBAL_PARAMS_REQ_T;

typedef struct
{
    Task        theAppTask;
    uint32      flags;
    uint16      adv_filter;
    uint16      adv_filter_sub_field1;
    uint32      adv_filter_sub_field2;
    uint16      ad_structure_filter;
    uint16      ad_structure_filter_sub_field1;
    uint32      ad_structure_filter_sub_field2;
    uint16      ad_structure_info_len;
    uint8       *ad_structure_info[CL_AD_STRUCT_INFO_BYTE_PTRS];
} CL_INTERNAL_DM_BLE_EXT_SCAN_REGISTER_SCANNER_REQ_T;

typedef struct
{
    Task    theAppTask;
    uint8   scan_handle;
} CL_INTERNAL_DM_BLE_EXT_SCAN_UNREGISTER_SCANNER_REQ_T;

typedef struct
{
    uint16 size;
    Source source;
} CL_INTERNAL_DM_BLE_EXT_SCAN_ADV_REPORT_DONE_IND_T;

typedef struct
{
    Task                    theAppTask;
} CL_INTERNAL_DM_BLE_EXT_SCAN_GET_CTRL_SCAN_INFO_REQ_T;

typedef struct
{
    uint16 size;
    Source source;
} CL_INTERNAL_DM_BLE_PER_SCAN_ADV_REPORT_DONE_IND_T;

typedef struct
{
    Task        theAppTask;
    uint8       report_periodic;
    uint16      skip;
    uint16      sync_timeout;
    uint8       sync_cte_type;
    uint16      attempt_sync_for_x_seconds;
    uint8       number_of_periodic_trains;
    CL_DM_ULP_PERIODIC_SCAN_TRAINS_T  periodic_trains[CL_MAX_PERIODIC_TRAIN_LIST_SIZE];
} CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TO_TRAIN_REQ_T;

typedef struct
{
    Task    theAppTask;
    uint16  sync_handle;
} CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TERMINATE_REQ_T;

typedef struct
{
    Task theAppTask;
    typed_bdaddr taddr;
    uint16 service_data;
    uint16 sync_handle;
} CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TRANSFER_REQ_T;

typedef struct
{
    Task            theAppTask;
    typed_bdaddr    taddr;
    uint16          skip;
    uint16          sync_timeout;
    uint8           mode;
    uint8           cte_type;
} CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TRANSFER_PARAMS_REQ_T;

typedef struct
{
    Task    theAppTask;
    uint32  flags;
    uint16  scan_for_x_seconds;
    uint16  ad_structure_filter;
    uint16  ad_structure_filter_sub_field1;
    uint32  ad_structure_filter_sub_field2;
    uint16  ad_structure_info_len;
    uint8   *ad_structure_info[CL_AD_STRUCT_INFO_BYTE_PTRS];
} CL_INTERNAL_DM_BLE_PERIODIC_SCAN_START_FIND_TRAINS_REQ_T;

typedef CL_INTERNAL_DM_BLE_EXT_SCAN_UNREGISTER_SCANNER_REQ_T CL_INTERNAL_DM_BLE_PERIODIC_SCAN_STOP_FIND_TRAINS_REQ_T;

typedef enum
{
    unidentified_stream,
    ext_scan_stream,
    per_scan_stream
} conn_lib_stream_types;

typedef struct
{
    Source source;
    conn_lib_stream_types stream_type;
} CL_INTERNAL_MESSAGE_MORE_DATA_T;
#endif

typedef struct
{
    uint8               position;
} CL_INTERNAL_SM_ADD_DEVICE_AT_TDL_POS_REQ_T;

/* State definition for the Connection Library */
typedef enum
{
    connectionUninitialised,
    connectionInitialising,
    connectionReady,
    connectionTestMode
} connectionStates;

typedef struct
{
    Task                    theAppTask;
    bdaddr                  addr;
    uint8                   override_action;
} CL_INTERNAL_DM_SC_OVERRIDE_REQ_T;

typedef struct
{
    Task                    theAppTask;
} CL_INTERNAL_DM_SC_OVERRIDE_MAX_BDADDR_REQ_T;


typedef struct
{
    Task                    theAppTask;
    typed_bdaddr            taddr;
    bool                    l2cap_retry;
} CL_INTERNAL_DM_SET_LINK_BEHAVIOR_REQ_T;

/*LE POWER CONTROL*/
typedef struct
{
    Task                    theAppTask;
    tp_bdaddr               tp_addr;
    uint8                   phy;
} CL_INTERNAL_DM_ULP_ENHANCED_READ_TRANSMIT_POWER_LEVEL_REQ_T;


typedef struct
{
    Task                    theAppTask;
    tp_bdaddr               tp_addr;
    uint8                   phy;  /* type of phy, will indicate whether the
                                     link is on 1M, 2M or Coded PHY type*/
} CL_INTERNAL_DM_ULP_READ_REMOTE_TRANSMIT_POWER_LEVEL_REQ_T;


typedef struct
{
    Task                    theAppTask;
    tp_bdaddr               tp_addr;
    uint8                   high_threshold;   /* High threshold for path loss */
    uint8                   high_hysteresis;  /* Hysteresis value for the high threshold */
    uint8                   low_threshold;    /* Low threshold for path loss */
    uint8                   low_hysteresis;   /* Hysteresis value for the low threshold */
    uint16                  min_time_spent;   /* Minimum time in number of connection events
                                                 to be observed once the path crosses the
                                                 threshold before an event is generated */
} CL_INTERNAL_DM_ULP_SET_PATH_LOSS_REPORTING_PARAMETERS_REQ_T;


typedef struct
{
    Task                    theAppTask;
    tp_bdaddr               tp_addr;
    bool                   enable;
} CL_INTERNAL_DM_ULP_SET_PATH_LOSS_REPORTING_ENABLE_REQ_T;


typedef struct
{
    Task                    theAppTask;
    tp_bdaddr               tp_addr;
    bool                   local_enable;   /* disable or enable local transmit power report */
    bool                   remote_enable;  /* disable or enable remote transmit power report */
} CL_INTERNAL_DM_ULP_SET_TRANSMIT_POWER_REPORTING_ENABLE_REQ_T;


/* Inquiry management state */
typedef struct
{
    Task nameLock;
#ifndef CL_EXCLUDE_INQUIRY
    Task inquiryLock;
    Task iacLock;
    unsigned periodic_inquiry:1;
#endif
} connectionInquiryState;


typedef struct
{
    Task                stateInfoLock;
    Sink                sink;

    /* Used to hold the bluetooth version of our device */
    cl_dm_bt_version    version;
} connectionReadInfoState;

typedef enum
{
    sm_init_set_none = 0,           /*!< None type */
    sm_init_set_security_mode,      /*!< Mode options set. */
    sm_init_set_security_config,    /*!< Config options set. */
    sm_init_set_security_default    /*!< Default security level options set. */
} sm_init_msg_type;

typedef enum
{
    /* ConnectionSmAddAuthrisedDevice() */
    device_req_add_authorised   = 0,
    /* ConnectionSmSetTrustedDevice() */
    device_req_set_trusted,
    /* connectionHandleAddDeviceAtTdlPosition() */
    device_req_add_device
} sm_device_req_type;

typedef struct
{
    /* Valid during the connectionInitialising state */
    unsigned            noDevices:4;
    unsigned            deviceCount:4;
    dm_security_mode    security_mode:8;
    encryption_mode     enc_mode:6;

    /* Used to indicate if security is through SM (TRUE) or LM (FALSE). */
    unsigned            sm_security:1;
    /* Used to indicate if this device is the responder (TRUE) or initiator
     * (FALSE).
     */
    unsigned            responder:1;
    /* Used to tell us what type of pairing we are performing */
    unsigned            authentication_requirements:4;

    sm_init_msg_type    sm_init_msg:4;
    sm_device_req_type  deviceReqType:2;

    uint16              key_distribution;

    uint16              TdlNumberOfDevices;

    /* Message primitives locks */
    Task                setSecurityModeLock;
    Task                authReqLock;
    Task                encryptReqLock;

    Task                deviceReqLock;

    /* Used to hold address of device we are Authenticating against */
    bdaddr              authRemoteAddr;

    /* Sink identifying the connecting being encrypted */
    Sink                sink;

    /* Cached permanent address returned by CL_DM_BLE_SECURITY_CFM when
     * bonding with a Privacy enabled BLE device using Resolvable Random
     * addressing.
     */
    typed_bdaddr        *permanent_taddr;
} connectionSmState;


typedef struct
{
    Task        sdpLock;
    Task        sdpSearchLock;
    bdaddr      sdpServerAddr;
} connectionSdpState;

typedef struct
{
    Sink roleLock;
} connectionLinkPolicyState;


typedef struct
{
    Task        mapLock;
} connectionL2capState;

typedef struct
{
    Task        txPwrLock;
} connectionReadTxPwrState;

typedef struct
{
    Task        bleScanAdLock;
} connectionBleScanAdState;

typedef struct
{
    Task        bleReadRndAddrLock;
} connectionBleReadRndAddrState;

typedef struct
{
    Task        dmIsocOpLock;
} connectionDmIsocState;

typedef struct
{
    Task        dmExtAdvLock;
    uint8       adv_handle;
} connectionDmExtAdvState;

typedef struct
{
    Task        dmExtScanLock;
} connectionDmExtScanState;

typedef struct
{
    Task        dmPerAdvLock;
    uint8       adv_handle;
} connectionDmPerAdvState;

typedef struct
{
    Task        dmPerScanLock;
} connectionDmPerScanState;


typedef struct
{
    Task        DmScOverrideLock;
} connectionDmScOverrideState;


typedef struct
{
    Task        readAfhMapLock;
} connectionDmReadAfhMapState;


/* A general Task lock for Bluestack messages, to be used for
 * functions that are used infrequently. Frequently used functions
 * should use their own, specific lock and not this one.
 */
typedef struct
{
    Task        taskLock;
} connectionGeneralLockState;

/* Structure to hold the instance state for the Connection Library */
typedef struct
{
    TaskData                        task;
    Task                            theAppTask;
    connectionStates                state:8; /* uses only 2 bits */
    unsigned                        flags:8; /* see CONNECTION_FLAG_* */
    const msg_filter                *msgFilter;

    connectionInquiryState          inqState;
    connectionSmState               smState;
    connectionReadInfoState         infoState;
    connectionSdpState              sdpState;
    connectionLinkPolicyState       linkPolicyState;
    connectionL2capState            l2capState;
    connectionReadTxPwrState        readTxPwrState;
    connectionBleScanAdState        bleScanAdState;
    connectionBleReadRndAddrState   bleReadRdnAddrState;
    connectionDmIsocState           dmIsocState;
    connectionDmExtAdvState         dmExtAdvState;
    connectionDmExtScanState        dmExtScanState;
    connectionDmPerAdvState         dmPerAdvState;
    connectionDmPerScanState        dmPerScanState;
    connectionDmScOverrideState     dmScOverrideState;
    connectionDmReadAfhMapState     dmReadAfhMapState;
    connectionGeneralLockState       generalLockState;
    uint16                          configAddrType;

#ifndef DISABLE_CSB
    Task                            csbTask;/*! task to send CSB messages to */
#endif

    /** The number of CL_INTERNAL_DM_WRITE_SCAN_ENABLE_REQ pending a CFM */
    uint16                          outstandingWriteScanEnableReqs;
    /** The task to send CL_DM_WRITE_SCAN_ENABLE_CFM messages to */
    Task                            scanEnableTask;

} connectionState;

typedef struct
{
    Task theAppTask;
    uint8   adv_handle; /* 1 to DM_ULP_EXT_ADV_MAX_ADV_HANDLES - 1 */

    /* Flags field (bit defines above)
       bit 0: If set enable extended advertising while enabling periodic advertising.
       bit 1: If set disable extended advertising while disabling periodic
              advertising. */
    uint16  flags;

    /* Enable field (bit defines above)
       bit 0: If set enable periodic advertising, else disable.
       bit 1: If set add ADI field to AUX_SYNC_IND PDU when enabling periodic
              advertising. */
    uint8   enable;
} CL_INTERNAL_DM_ULP_PERIODIC_ADV_ENABLE_REQ_T;

typedef struct
{
    Task    theAppTask;
    uint8   adv_handle; /* 1 to DM_ULP_EXT_ADV_MAX_ADV_HANDLES - 1 */
    uint8   flags;
    uint8   change_reasons;
} CL_INTERNAL_DM_BLE_SET_DATA_RELATED_ADDRESS_CHANGES_REQ_T;

/****************************************************************************
NAME
    connectionGetCmTask

DESCRIPTION
    This function returns the connection library task so that the connection
    library can post a message to itself.

RETURNS
    The connection library task.
*/
Task connectionGetCmTask(void);

/****************************************************************************
NAME
    connectionGetAppTask

DESCRIPTION
    This function returns the application task.

RETURNS
    The application task.
*/
Task connectionGetAppTask(void);

/****************************************************************************
NAME
    connectionGetBtVersion

DESCRIPTION
    Returns the BT Version read from BlueStack during initialisation.

RETURNS
    cl_dm_bt_version
*/

cl_dm_bt_version connectionGetBtVersion(void);

/****************************************************************************
NAME
    connectionGetMsgFilter

DESCRIPTION
    This function returns the connection library message filter.
*/
const msg_filter *connectionGetMsgFilter(void);

#ifndef DISABLE_CSB
/****************************************************************************
NAME
    connectionGetCsbTask

DESCRIPTION
    This function returns the application task registered for CSB-related
    messages.
*/
Task connectionGetCsbTask(void);
#endif

/****************************************************************************
NAME
    connectionGetScanEnableTask

DESCRIPTION
    This function returns the application task registered for scan enable
    messages.
*/
Task connectionGetScanEnableTask(void);

/****************************************************************************
DESCRIPTION
    This function may be used to modify (by delta) the count of outstanding
    write scan enable requests.
*/
void connectionOutstandingWriteScanEnableReqsSetDelta(signed delta);

/****************************************************************************
DESCRIPTION
    This function returns the number of outstanding write scan enable requests.
*/
uint16 connectionOutstandingWriteScanEnableReqsGet(void);

#endif  /* CONNECTION_PRIVATE_H_ */
