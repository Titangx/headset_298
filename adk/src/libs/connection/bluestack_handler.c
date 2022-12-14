/****************************************************************************
Copyright (c) 2004 - 2022 Qualcomm Technologies International, Ltd.


FILE NAME
    bluestack_handler.c

DESCRIPTION
    Handles primitives received by BlueStack and routes them to the
    appropriate handler function depending on their type.

NOTES

*/

/****************************************************************************
    Header files
*/
#include "connection.h"
#include "connection_private.h"
#include "init.h"
#include "bluestack_handler.h"
#include "dm_baseband_handler.h"
#include "dm_baseband_scan.h"
#include "dm_dut_handler.h"
#include "dm_info_handler.h"
#include "dm_inquiry_handler.h"
#include "dm_read_tx_power_handler.h"
#include "dm_sync_handler.h"
#include "dm_isoc_handler.h"
#include "dm_security_handler.h"
#include "l2cap_handler.h"
#include "rfc_handler.h"
#include "sdp_handler.h"
#include "dm_link_policy_handler.h"
#include "dm_bad_message_handler.h"
#include "dm_page_timeout_handler.h"
#include "dm_crypto_api_handler.h"
#include "dm_ble_latency.h"
#include "dm_ble_channel_selection_algorithm.h"
#include "dm_ble_random_address.h"
#include "dm_sc_override.h"
#include "dm_read_afh_chan_map.h"
#include "dm_link_behavior.h"


#ifndef DISABLE_BLE
#include "dm_ble_handler.h"
#include "dm_ble_security.h"
#include "dm_ble_phy_preferences.h"
#include "dm_ble_random_address.h"
#include "dm_ble_connection_parameters_update.h"
#include "dm_ble_advertising.h"
#include "dm_read_ble_ad_chan_tx_pwr.h"
#include "dm_ble_scanning.h"
#include "dm_ble_privacy_mode.h"
#include "dm_ble_power_control.h"
#endif

#ifndef DISABLE_CSB
#include "dm_csb_handler.h"
#endif

#include <types.h>
#include <dm_prim.h>
#include <l2cap_prim.h>
#include <rfcomm_prim.h>
#include <sdc_prim.h>
#include <sds_prim.h>


#include <print.h>
#include <vm.h>

/*lint -e655 -e525 -e830 */

/* Connection state management */
#define SET_CM_STATE(s) theCm->state = (s);

/* List of reason code for default handling of unexpected messages */
typedef enum
{
    connectionUnexpectedCmPrim,
    connectionUnexpectedDmPrim,
    connectionUnexpectedRfcPrim,
    connectionUnexpectedSdpPrim,
    connectionUnexpectedL2capPrim,
    connectionUnhandledMessage
}connectionUnexpectedReasonCode;

/****************************************************************************
NAME
    handleUnexpected

DESCRIPTION
    This macro is called as a result of a message arriving when the
    Connection Library was not expecting it.

RETURNS
    void
*/
#define handleUnexpected(code, state, type) \
    CL_DEBUG_INFO(("handleUnexpected - Code 0x%x State 0x%x MESSAGE:0x%x\n", code, state, type))

/****************************************************************************
NAME
    connectionBluestackHandlerDm

DESCRIPTION
    Message handler for the DM primitives.

RETURNS
    void
*/
static void connectionBluestackHandlerDm(connectionState *theCm, const DM_UPRIM_T *message)
{
    const connectionStates state = theCm->state;
    if (state == connectionReady)
    {

        switch (message->type)
        {
#ifndef CL_EXCLUDE_SYNC
            case DM_SYNC_REGISTER_CFM:
                PRINT(("DM_SYNC_REGISTER_CFM\n"));
                connectionHandleSyncRegisterCfm( (const DM_SYNC_REGISTER_CFM_T *)message);
                return;

            case DM_SYNC_UNREGISTER_CFM:
                PRINT(("DM_SYNC_UNREGISTER_CFM\n"));
                connectionHandleSyncUnregisterCfm( (const DM_SYNC_UNREGISTER_CFM_T *)message );
                return;

            case DM_SYNC_CONNECT_CFM:
                PRINT(("DM_SYNC_CONNECT_CFM\n"));
                connectionHandleSyncConnectCfm(theCm->theAppTask, (const DM_SYNC_CONNECT_CFM_T *) message);
                return;

            case DM_SYNC_CONNECT_COMPLETE_IND:
                PRINT(("DM_SYNC_CONNECT_COMPLETE_IND\n"));
                connectionHandleSyncConnectCompleteInd(theCm->theAppTask, (const DM_SYNC_CONNECT_COMPLETE_IND_T *) message);
                return;

            case DM_SYNC_CONNECT_IND:
                PRINT(("DM_SYNC_CONNECT_IND\n"));
                connectionHandleSyncConnectInd((const DM_SYNC_CONNECT_IND_T *) message);
                return;

            case DM_SYNC_DISCONNECT_IND:
                PRINT(("DM_SYNC_DISCONNECT_IND\n"));
                connectionHandleSyncDisconnectInd((const DM_SYNC_DISCONNECT_IND_T *) message);
                return;

            case DM_SYNC_DISCONNECT_CFM:
                PRINT(("DM_SYNC_DISCONNECT_CFM\n"));
                connectionHandleSyncDisconnectCfm((const DM_SYNC_DISCONNECT_CFM_T *) message);
                return;

            case DM_SYNC_RENEGOTIATE_IND:
                PRINT(("DM_SYNC_RENEGOTIATE_IND\n"));
                connectionHandleSyncRenegotiateInd((const DM_SYNC_RENEGOTIATE_IND_T *) message);
                return;

            case DM_SYNC_RENEGOTIATE_CFM: /* 31783 */
                PRINT(("DM_SYNC_RENEGOTIATE_CFM\n"));
                connectionHandleSyncRenegotiateCfm((const DM_SYNC_RENEGOTIATE_CFM_T *) message);
                return;
#endif
        }

        switch (message->type)
        {
#ifndef CL_EXCLUDE_ISOC
            case DM_ISOC_REGISTER_CFM:
                PRINT(("DM_ISOC_REGISTER_CFM\n"));
                connectionHandleIsocRegisterCfm(&theCm->dmIsocState, (const DM_ISOC_REGISTER_CFM_T *)message);
                return;

            case DM_ISOC_CIS_CONNECT_CFM:
                PRINT(("DM_ISOC_CIS_CONNECT_CFM\n"));
                connectionHandleIsocConnectCfm(&theCm->dmIsocState, (const DM_ISOC_CIS_CONNECT_CFM_T *) message);
                return;

            case DM_ISOC_CIS_CONNECT_IND:
                PRINT(("DM_ISOC_CIS_CONNECT_IND\n"));
                connectionHandleIsocConnectInd(&theCm->dmIsocState, (const DM_ISOC_CIS_CONNECT_IND_T *) message);
                return;

            case DM_ISOC_CIS_DISCONNECT_IND:
                PRINT(("DM_ISOC_CIS_DISCONNECT_IND\n"));
                connectionHandleIsocDisconnectInd(&theCm->dmIsocState, (const DM_ISOC_CIS_DISCONNECT_IND_T *) message);
                return;

            case DM_ISOC_CIS_DISCONNECT_CFM:
                PRINT(("DM_ISOC_CIS_DISCONNECT_CFM\n"));
                connectionHandleIsocDisconnectCfm(&theCm->dmIsocState, (const DM_ISOC_CIS_DISCONNECT_CFM_T *) message);
                return;

            case DM_ISOC_CONFIGURE_CIG_CFM:
                PRINT(("DM_ISOC_CONFIGURE_CIG_CFM\n"));
                connectionHandleIsocConfigureCigCfm(&theCm->dmIsocState, (const DM_ISOC_CONFIGURE_CIG_CFM_T *) message);
                return;

            case DM_ISOC_REMOVE_CIG_CFM:
                PRINT(("DM_ISOC_REMOVE_CIG_CFM\n"));
                connectionHandleIsocRemoveCigCfm(&theCm->dmIsocState, (const DM_ISOC_REMOVE_CIG_CFM_T *) message);
                return;

            case DM_ISOC_SETUP_ISO_DATA_PATH_CFM:
                PRINT(("DM_ISOC_SETUP_ISO_DATA_PATH_CFM\n"));
                connectionHandleIsocSetupDataPathCfm(&theCm->dmIsocState, (const DM_ISOC_SETUP_ISO_DATA_PATH_CFM_T *) message);
                return;

            case DM_ISOC_REMOVE_ISO_DATA_PATH_CFM:
                PRINT(("DM_ISOC_REMOVE_ISO_DATA_PATH_CFM\n"));
                connectionHandleIsocRemoveDataPathCfm(&theCm->dmIsocState, (const DM_ISOC_REMOVE_ISO_DATA_PATH_CFM_T *) message);
                return;

            case DM_ISOC_CREATE_BIG_CFM:
                PRINT(("DM_ISOC_CREATE_BIG_CFM\n"));
                connectionHandleIsocCreateBigCfm(&theCm->dmIsocState, (const DM_ISOC_CREATE_BIG_CFM_T *) message);
                return;

            case DM_ISOC_TERMINATE_BIG_CFM:
                PRINT(("DM_ISOC_TERMINATE_BIG_CFM\n"));
                connectionHandleIsocTerminateBigCfm(&theCm->dmIsocState, (const DM_ISOC_TERMINATE_BIG_CFM_T *) message);
                return;

            case DM_ISOC_BIG_CREATE_SYNC_CFM:
                PRINT(("DM_ISOC_BIG_CREATE_SYNC_CFM\n"));
                connectionHandleIsocBigCreateSyncCfm(&theCm->dmIsocState, (const DM_ISOC_BIG_CREATE_SYNC_CFM_T *) message);
                return;

            case DM_ISOC_BIG_TERMINATE_SYNC_IND:
                PRINT(("DM_ISOC_BIG_TERMINATE_SYNC_IND\n"));
                connectionHandleIsocBigTerminateSyncInd((const DM_ISOC_BIG_TERMINATE_SYNC_IND_T *) message);
                return;

            case DM_HCI_ULP_BIGINFO_ADV_REPORT_IND:
                PRINT(("DM_HCI_ULP_BIGINFO_ADV_REPORT_IND\n"));
                connectionHandleIsocBigInfoAdvReportInd(&theCm->dmIsocState, (const DM_HCI_ULP_BIGINFO_ADV_REPORT_IND_T *) message);
                return;
#endif
        }

        switch (message->type)
        {
            case DM_SM_REGISTER_CFM:
                PRINT(("DM_SM_REGISTER_CFM\n"));
                /* Not currently handled */
                return;

            case DM_SM_UNREGISTER_CFM:
                PRINT(("DM_SM_UNREGISTER_CFM\n"));
                /* Not currently handled */
                return;

            case DM_SM_REGISTER_OUTGOING_CFM:
                PRINT(("DM_SM_REGISTER_OUTGOING_CFM\n"));
                handleRegisterOutgoingCfm((const DM_SM_REGISTER_OUTGOING_CFM_T *)message);
                return;

            case DM_SM_UNREGISTER_OUTGOING_CFM:
                PRINT(("DM_SM_UNREGISTER_OUTGOING_CFM\n"));
                /* Not currently handled */
                return;

            case DM_SM_ADD_DEVICE_CFM: /* 11284 */
                PRINT(("DM_SM_ADD_DEVICE_CFM\n"));
                connectionHandleSmAddDeviceCfmReady(&theCm->smState, (const DM_SM_ADD_DEVICE_CFM_T*)message);
                return;

            case DM_SM_PIN_REQUEST_IND: /* 11287 */
                PRINT(("DM_SM_PIN_REQUEST_IND\n"));
                connectionHandleSmPinReqInd(theCm->theAppTask, (const DM_SM_PIN_REQUEST_IND_T*)message);
                return;
            case DM_SM_KEYS_IND: /* 11288 */
                PRINT(("DM_SM_KEYS_IND\n"));
                connectionHandleSmKeysInd(theCm->theAppTask, &theCm->smState, (const DM_SM_KEYS_IND_T*)message);
                return;
            case DM_SM_AUTHORISE_IND: /* 11289 */
                PRINT(("DM_SM_AUTHORISE_IND\n"));
                connectionHandleSmAuthoriseInd(theCm->theAppTask, (const DM_SM_AUTHORISE_IND_T*)message);
                return;

            case DM_SM_AUTHENTICATE_CFM: /* 11290 */
                PRINT(("DM_SM_AUTHENTICATE_CFM\n"));
                return;

            case DM_SM_ENCRYPT_CFM: /* 11291 */
                PRINT(("DM_SM_ENCRYPT_CFM\n"));
                connectionHandleEncryptCfm(&theCm->smState, (const DM_SM_ENCRYPT_CFM_T *) message);
                return;

            case DM_SM_ENCRYPTION_CHANGE_IND: /* 11292 */
                PRINT(("DM_SM_ENCRYPTION_CHANGE_IND\n"));
                connectionHandleEncryptionChange((const DM_SM_ENCRYPTION_CHANGE_IND_T *)message);
                return;

            case DM_SM_READ_LOCAL_OOB_DATA_CFM: /* 11295 */
                PRINT(("DM_SM_READ_LOCAL_OOB_DATA_CFM\n"));
                connectionHandleReadLocalOobDataCfm(&theCm->smState, (const DM_SM_READ_LOCAL_OOB_DATA_CFM_T*)message);
                return;

            case DM_SM_IO_CAPABILITY_RESPONSE_IND: /* 11297 */
                PRINT(("DM_SM_IO_CAPABILITY_RESPONSE_IND \n"));
                connectionHandleSmIoCapResInd(theCm->theAppTask, &theCm->smState, (const DM_SM_IO_CAPABILITY_RESPONSE_IND_T*)message);
                return;

            case DM_SM_IO_CAPABILITY_REQUEST_IND: /* 11298 */
                PRINT(("DM_SM_IO_CAPABILITY_REQUEST_IND \n"));
                connectionHandleSmIoCapReqInd(theCm->theAppTask, &theCm->smState, (const DM_SM_IO_CAPABILITY_REQUEST_IND_T*)message);
                return;

            case DM_SM_SIMPLE_PAIRING_COMPLETE_IND: /* 11299 */
                PRINT(("DM_SM_SIMPLE_PAIRING_COMPLETE_IND\n"));
                connectionHandleSmSimplePairingCompleteInd(theCm->theAppTask, &theCm->smState, (const DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T*)message);
                return;

            case DM_SM_USER_CONFIRMATION_REQUEST_IND: /* 11300 */
                PRINT(("DM_SM_USER_CONFIRMATION_REQUEST_IND \n"));
                connectionHandleSmUserConfirmationReqInd(theCm->theAppTask, (const DM_SM_USER_CONFIRMATION_REQUEST_IND_T*)message);
                return;

            case DM_SM_USER_PASSKEY_REQUEST_IND: /* 11301 */
                PRINT(("DM_SM_USER_PASSKEY_REQUEST_IND \n"));
                connectionHandleSmUserPasskeyReqInd(theCm->theAppTask, (const DM_SM_USER_PASSKEY_REQUEST_IND_T*)message);
                return;

            case DM_SM_USER_PASSKEY_NOTIFICATION_IND: /* 11303 */
                PRINT(("DM_SM_USER_PASSKEY_NOTIFICATION_IND \n"));
                connectionHandleSmUserPasskeyNotificationInd(theCm->theAppTask, (const DM_SM_USER_PASSKEY_NOTIFICATION_IND_T*)message);
                return;

            case DM_SM_KEYPRESS_NOTIFICATION_IND: /* 11304 */
                PRINT(("DM_SM_KEYPRESS_NOTIFICATION_IND \n"));
                connectionHandleSmKeypressNotificationInd(theCm->theAppTask, (const DM_SM_KEYPRESS_NOTIFICATION_IND_T*)message);
                return;

            case DM_SM_KEY_REQUEST_IND:
                PRINT(("DM_SM_KEY_REQUEST_IND \n"));
                connectionHandleSmKeyReqInd((const DM_SM_KEY_REQUEST_IND_T *)message);
                return;

            case DM_WRITE_SC_HOST_SUPPORT_OVERRIDE_CFM:
                PRINT(("DM_WRITE_SC_HOST_SUPPORT_OVERRIDE_CFM\n"));
                connectionHandleDmScOverrideCfm(
                            &theCm->dmScOverrideState,
                            (const DM_WRITE_SC_HOST_SUPPORT_OVERRIDE_CFM_T*)message
                            );
                return;

            case DM_READ_SC_HOST_SUPPORT_OVERRIDE_MAX_BD_ADDR_CFM:
                PRINT(("DM_READ_SC_HOST_SUPPORT_OVERRIDE_MAX_BD_ADDR_CFM\n"));
                connectionHandleDmScOverrideMaxBdaddrCfm(
                            &theCm->dmScOverrideState,
                            (const DM_READ_SC_HOST_SUPPORT_OVERRIDE_MAX_BD_ADDR_CFM_T*)message
                            );
                return;

            case DM_SET_LINK_BEHAVIOR_CFM:
                PRINT(("DM_SET_LINK_BEHAVIOR_CFM\n"));
                connectionHandleDmSetLinkBehaviorCfm(
                            &theCm->generalLockState,
                            (const DM_SET_LINK_BEHAVIOR_CFM_T*)message
                            );
                return;

#ifndef DISABLE_BLE
            case DM_SM_SECURITY_CFM:
                PRINT(("DM_SM_SECURITY_CFM\n"));
                connectionHandleDmSmSecurityCfm(
                    (const DM_SM_SECURITY_CFM_T *)message
                    );
                return;

            case DM_SM_GENERATE_CROSS_TRANS_KEY_REQUEST_IND:
                PRINT(("DM_SM_GENERATE_CROSS_TRANS_KEY_REQUEST_INDn"));
                connectionHandleDmSmGenerateCrossTransKeyRequestInd(
                        (const DM_SM_GENERATE_CROSS_TRANS_KEY_REQUEST_IND_T *)message
                        );
                return;

            case DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM:
                PRINT(("DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM\n"));
                connectionHandleDmBleUpdateConnectionParametersCfm(
                    &theCm->l2capState,
                    (const DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_T*)message);
                return;

            case DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND:
                PRINT(("DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND\n"));
                connectionHandleDmBleAcceptConnectionParUpdateInd(
                    (const DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_T*)message
                    );
                return;

            case DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND:
                PRINT(("DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND\n"));
                connectionHandleDmBleConnectionUpdateCompleteInd(
                    (const DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_T*)message
                    );
                return;

            case DM_ULP_ADV_PARAM_UPDATE_IND:
                PRINT(("DM_ULP_ADV_PARAM_UPDATE_IND\n"));
                connectionHandleDmBleAdvParamUpdateInd(
                        (const DM_ULP_ADV_PARAM_UPDATE_IND_T *)message
                        );
                return;

            case DM_HCI_ULP_SET_PRIVACY_MODE_CFM:
                PRINT(("DM_HCI_ULP_SET_PRIVACY_MODE_CFM\n"));
                connectionHandleDmUlpSetPrivacyModeCfm(
                    (const DM_HCI_ULP_SET_PRIVACY_MODE_CFM_T*)message
                    );
            return;

            case DM_SM_SIRK_OPERATION_CFM:
                PRINT(("DM_SM_SIRK_OPERATION_CFM\n"));
                connectionHandleSirkOperationCfm(
                    (const DM_SM_SIRK_OPERATION_CFM_T *)message
                    );
                return;
#endif
#ifndef DISABLE_CSB
            case DM_HCI_SET_RESERVED_LT_ADDR_CFM:
                 connectionHandleDmHciSetReservedLtAddrCfm((DM_HCI_SET_RESERVED_LT_ADDR_CFM_T *)message);
                return;

            case DM_HCI_DELETE_RESERVED_LT_ADDR_CFM:
                 connectionHandleDmHciDeleteReservedLtAddrCfm((DM_HCI_DELETE_RESERVED_LT_ADDR_CFM_T *)message);
                return;

            case DM_HCI_SET_CSB_CFM:
                connectionHandleDmHciSetCsbCfm((DM_HCI_SET_CSB_CFM_T *)message);
                return;

            case DM_HCI_WRITE_SYNCHRONIZATION_TRAIN_PARAMS_CFM:
                connectionHandleDmHciWriteSynchronizationTrainParamsCfm((DM_HCI_WRITE_SYNCHRONIZATION_TRAIN_PARAMS_CFM_T *)message);
                return;

            case DM_HCI_START_SYNCHRONIZATION_TRAIN_CFM:
                connectionHandleDmHciStartSynchronizationTrainCfm((DM_HCI_START_SYNCHRONIZATION_TRAIN_CFM_T *)message);
                return;

            case DM_HCI_RECEIVE_SYNCHRONIZATION_TRAIN_CFM:
                connectionHandleDmHciReceiveSynchronizationTrainCfm((DM_HCI_RECEIVE_SYNCHRONIZATION_TRAIN_CFM_T *)message);
                return;

            case DM_HCI_SET_CSB_RECEIVE_CFM:
                connectionHandleDmHciSetCsbReceiveCfm((DM_HCI_SET_CSB_RECEIVE_CFM_T *)message);
                return;

            case DM_HCI_CSB_TIMEOUT_IND:
                connectionHandleDmHciCsbTimeoutInd((DM_HCI_CSB_TIMEOUT_IND_T *)message);
                return;

            case DM_HCI_CSB_AFH_MAP_AVAILABLE_IND:
                connectionHandleDmHciCsbAfhMapAvailableInd((DM_HCI_CSB_AFH_MAP_AVAILABLE_IND_T *)message);
                return;

            case DM_HCI_CSB_CHANNEL_MAP_CHANGE_IND:
                connectionHandleDmHciCsbChannelMapChangeInd((DM_HCI_CSB_CHANNEL_MAP_CHANGE_IND_T *)message);
                return;
#endif
        }

        switch (message->type)
        {

#ifndef CL_EXCLUDE_INQUIRY

            case DM_HCI_PERIODIC_INQUIRY_MODE_CFM: /* 1058 */
                PRINT(("DM_HCI_PERIODIC_INQUIRY_COMPLETE\n"));
                return;

            case DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM: /* 1059 */
                PRINT(("DM_HCI_EXIT_PERIODIC_INQUIRY_CFM\n"));
                connectionHandleExitPeriodicInquiryComplete(&theCm->inqState);
                return;

            case DM_HCI_INQUIRY_CANCEL_CFM: /* 1057 */
                PRINT(("DM_HCI_INQUIRY_CANCEL_CFM\n"));
                connectionHandleInquiryComplete(&theCm->inqState);
                return;

            case DM_HCI_INQUIRY_RESULT_IND:    /* 1060 */
                PRINT(("DM_HCI_INQUIRY_RESULT_IND\n"));
                connectionHandleInquiryResult(&theCm->inqState, (const DM_HCI_INQUIRY_RESULT_IND_T *)message);
                return;

            case DM_HCI_INQUIRY_CFM: /* 1061 */
                PRINT(("DM_HCI_INQUIRY_CFM\n"));
                connectionHandleInquiryComplete(&theCm->inqState);
                return;
#endif

            case DM_HCI_REMOTE_NAME_CFM: /* 1065 */
                PRINT(("DM_HCI_REMOTE_NAME_CFM\n"));
                connectionHandleRemoteNameComplete(&theCm->inqState, (const DM_HCI_REMOTE_NAME_CFM_T *) message);
                return;

            case DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM:
                PRINT(("DM_HCI_REMOTE_NAME_CANCEL_CFM\n"));
                connectionHandleRemoteNameCancelCfm(&theCm->inqState,
                         (const DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM_T *) message);
                return;


            case DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM: /* 1066 */
                PRINT(("DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM\n"));
                connectionHandleReadRemoteSupportedFeaturesCfm(&theCm->infoState, (const DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_T*) message);
                return;

            case DM_HCI_READ_REMOTE_EXT_FEATURES_CFM:
                PRINT(("DM_HCI_READ_REMOTE_EXT_FEATURES_CFM\n"));
                connectionHandleReadRemoteSupportedExtFeaturesCfm(&theCm->infoState,
                         (DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_T*) message);
                return;

            case DM_HCI_READ_REMOTE_VER_INFO_CFM: /* 1067 */
                PRINT(("DM_HCI_READ_REMOTE_VER_INFO_CFM\n"));
                connectionHandleReadRemoteVersionCfm(&theCm->infoState, (const DM_HCI_READ_REMOTE_VER_INFO_CFM_T*) message);
                return;

            case DM_HCI_READ_CLOCK_OFFSET_CFM: /* 1068 */
                PRINT(("DM_HCI_READ_CLOCK_OFFSET_CFM\n"));
                connectionHandleReadClkOffsetComplete(&theCm->infoState, (const DM_HCI_READ_CLOCK_OFFSET_CFM_T *) message);
                return;

            case DM_HCI_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED_IND:
                PRINT(("DM_HCI_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED_IND\n"));
                connectionHandleAPTExpiredInd(theCm->theAppTask, (const DM_HCI_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED_IND_T *) message);
                return;

            case DM_HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_CFM:
                PRINT(("DM_HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_CFM\n"));
                connectionHandleReadAPTCfm(theCm->theAppTask, (DM_HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_CFM_T *) message);
                return;

            case DM_SM_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT_CFM:
                PRINT(("DM_SM_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT_CFM\n"));
                connectionHandleWriteAPTCfm(theCm->theAppTask, (DM_SM_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT_CFM_T *) message);
                return;

        }

        switch (message->type)
        {

#ifndef CL_EXCLUDE_INQUIRY

            case DM_HCI_READ_INQUIRY_MODE_CFM: /* 30730 */
                PRINT(("DM_HCI_READ_INQUIRY_MODE_CFM\n"));
                connectionHandleReadInquiryModeComplete(&theCm->inqState, (const DM_HCI_READ_INQUIRY_MODE_CFM_T *)message);
                return;

            case DM_HCI_WRITE_INQUIRY_MODE_CFM: /* 30731 */
                PRINT(("DM_HCI_WRITE_INQUIRY_MODE_CFM\n"));
                connectionHandleWriteInquiryModeComplete(&theCm->inqState, (const DM_HCI_WRITE_INQUIRY_MODE_CFM_T *)message);
                return;

            case DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND: /* 30742 */
                PRINT(("DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND\n"));
                connectionHandleInquiryResultWithRssi(&theCm->inqState, (const DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_T*)message);
                return;
#endif

            case DM_HCI_SNIFF_SUB_RATING_IND: /* 30784 */
                PRINT(("DM_HCI_SNIFF_SUB_RATING_IND\n"));
                connectionHandleSniffSubRatingInd((const DM_HCI_SNIFF_SUB_RATING_IND_T *)message);
                return;


#ifndef CL_EXCLUDE_INQUIRY

            case DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM: /* 30786 */
                PRINT(("DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM\n"));
                connectionHandleReadEirDataComplete(&theCm->inqState, (const DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T *)message);
                return;

            case DM_HCI_EXTENDED_INQUIRY_RESULT_IND: /* 30788 */
                PRINT(("DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T\n"));
                connectionHandleExtendedInquiryResult(&theCm->inqState, (const DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T *)message);
                return;

            case DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM: /* 30790 */
                PRINT(("DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM\n"));
                connectionHandleReadInquiryTxComplete(&theCm->inqState, (const DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM_T *) message);
                return;
#endif

            case DM_HCI_READ_TX_POWER_LEVEL_CFM:
                 PRINT(("DM_HCI_READ_TX_POWER_LEVEL_CFM\n"));
                 connectionHandleReadTxPowerComplete(&theCm->readTxPwrState,
                                        (const DM_HCI_READ_TX_POWER_LEVEL_CFM_T*)message);
                 return;

            case DM_HCI_LINK_SUPERV_TIMEOUT_IND: /* 30792 */
                PRINT(("DM_HCI_LINK_SUPERV_TIMEOUT_IND\n"));
                connectionHandleLinkSupervisionTimeoutInd((const DM_HCI_LINK_SUPERV_TIMEOUT_IND_T *)message);
                return;

            case DM_HCI_REFRESH_ENCRYPTION_KEY_IND: /* 30794 */
                PRINT(("DM_HCI_REFRESH_ENCRYPTION_KEY_IND\n"));
                connectionHandleEncryptionKeyRefreshInd((const DM_HCI_REFRESH_ENCRYPTION_KEY_IND_T*) message);
                return;
        }

        switch (message->type)
        {
#ifndef CL_EXCLUDE_L2CAP
            case DM_HCI_QOS_SETUP_CFM: /* 2064 */
                PRINT(("DM_HCI_QOS_SETUP_CFM\n"));
                connectionHandleQosSetupCfm((const DM_HCI_QOS_SETUP_CFM_T *) message);
                return;
#endif

            case DM_HCI_ROLE_DISCOVERY_CFM: /* 2066 */
                PRINT(("DM_HCI_ROLE_DISCOVERY_CFM\n"));
                connectionHandleRoleDiscoveryComplete(&theCm->linkPolicyState, (const DM_HCI_ROLE_DISCOVERY_CFM_T *) message);
                return;

            case DM_HCI_SWITCH_ROLE_CFM: /* 2067 */
                PRINT(("DM_HCI_SWITCH_ROLE_CFM\n"));
                connectionHandleDmSwitchRoleComplete(&theCm->linkPolicyState, (const DM_HCI_SWITCH_ROLE_CFM_T *) message);
                return;

            case DM_HCI_READ_LOCAL_NAME_CFM: /* 3147 */
                PRINT(("DM_HCI_READ_LOCAL_NAME_CFM\n"));
                connectionHandleLocalNameComplete(&theCm->inqState, (const DM_HCI_READ_LOCAL_NAME_CFM_T *) message);
                return;

            case DM_HCI_READ_AFH_CHANNEL_MAP_CFM:
                PRINT(("DM_HCI_READ_AFH_CHANNEL_MAP_CFM\n"));
                connectionHandleDmReadAfhChannelMapCfm(&theCm->dmReadAfhMapState, (const DM_HCI_READ_AFH_CHANNEL_MAP_CFM_T *)message);
                return;

            case DM_HCI_READ_CLASS_OF_DEVICE_CFM: /* 3160 */
                PRINT(("DM_HCI_READ_CLASS_OF_DEVICE_CFM\n"));
                connectionHandleReadClassOfDeviceComplete(&theCm->infoState, (const DM_HCI_READ_CLASS_OF_DEVICE_CFM_T *)message);
                return;


#ifndef CL_EXCLUDE_INQUIRY
            case DM_HCI_WRITE_CURRENT_IAC_LAP_CFM: /* 3177 */
                PRINT(("DM_HCI_WRITE_CURRENT_IAC_LAP_CFM\n"));
                connectionHandleWriteIacLapComplete(&theCm->inqState, (const DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_T *)message);
                return;
#endif

            case DM_HCI_READ_BD_ADDR_CFM: /* 4110 */
                PRINT(("DM_HCI_READ_BD_ADDR_CFM\n"));
                connectionHandleReadBdAddrComplete(&theCm->infoState, (const DM_HCI_READ_BD_ADDR_CFM_T *)message);
                return;

            case DM_HCI_GET_LINK_QUALITY_CFM: /* 5129 */
                PRINT(("DM_HCI_GET_LINK_QUALITY_CFM\n"));
                connectionHandleReadLinkQualityComplete(&theCm->infoState, (const DM_HCI_GET_LINK_QUALITY_CFM_T *) message);
                return;

            case DM_HCI_READ_RSSI_CFM: /* 5130 */
                PRINT(("DM_HCI_READ_RSSI_CFM\n"));
                connectionHandleReadRssiComplete(&theCm->infoState, (const DM_HCI_READ_RSSI_CFM_T *) message);
                return;

            case DM_LP_WRITE_POWERSTATES_CFM: /* 30977 */
                PRINT(("DM_LP_WRITE_POWERSTATES_CFM\n"));
                connectionLinkPolicyHandleWritePowerStatesCfm((const DM_LP_WRITE_POWERSTATES_CFM_T *)message);
                return;
        }

        /* Process Crypto messages */
        if(connectionBluestackHandlerDmCrypto(message))
            return;

#ifndef DISABLE_BLE
        /* Process Latency messages */
        if(connectionBluestackHandlerDmLatency(message))
            return;

        /* Process Channel Selection Algorithm messages */
        if(connectionBluestackHandlerDmChannelSelectionAlgorithm(message))
            return;

        /* Process Phy Pref. messages */
        if(connectionBluestackHandlerDmPhyPreferences(message))
            return;
#endif
    }

    if (state != connectionUninitialised)
    {
        if (state != connectionInitialising)
        {
            /* e.g. state is connectionReady or connectionTestMode */

            switch (message->type)
            {
                case DM_HCI_MODE_CHANGE_EVENT_IND: /* 2063 */
                    PRINT(("DM_HCI_MODE_CHANGE_EVENT_IND\n"));
                    connectionHandleDmHciModeChangeEvent(theCm->theAppTask, (const DM_HCI_MODE_CHANGE_EVENT_IND_T *)message);
                    return;

                case DM_ACL_OPEN_CFM: /* 10252 */
                    PRINT(("DM_ACL_OPEN_CFM\n"));
                    connectionHandleDmAclOpenCfm(&theCm->smState, (const DM_ACL_OPEN_CFM_T*)message);
                    return;

                case DM_ACL_OPENED_IND: /* 10253 */
                    PRINT(("DM_ACL_OPENED_IND\n"));
                    connectionHandleDmAclOpenInd(theCm->theAppTask, &theCm->smState, (const DM_ACL_OPENED_IND_T *) message);
                    return;

                case DM_ACL_CLOSED_IND: /* 28697 */
                    PRINT(("DM_ACL_CLOSED_IND\n"));
                    connectionHandleDmAclClosedInd(theCm->theAppTask, &theCm->smState, (const DM_ACL_CLOSED_IND_T*)message);
                    return;

                case DM_ACL_CLOSE_CFM:
                    PRINT(("DM_ACL_CLOSE_CFM\n"));
                    connectionHandleDmAclCloseCfm(theCm->theAppTask, (const DM_ACL_CLOSE_CFM_T*)message);
                    return;

                case DM_SM_BONDING_CFM: /*B-56544 */
                    PRINT(("DM_SM_BONDING_CFM\n"));
                    connectionHandleSmBondingCfm(&theCm->smState, (const DM_SM_BONDING_CFM_T*)message);
                    return;
            }
        }

        /* state can be connectionInitialising, connectionReady OR connectionTestMode */

        switch (message->type)
        {
#ifndef DISABLE_BLE
            case DM_HCI_ULP_TRANSMITTER_TEST_CFM:
                PRINT(("DM_HCI_ULP_TRANSMITTER_TEST_CFM\n"));
                connectionSendUlpTransmitterTestCfmToClient(theCm->theAppTask, ((const DM_HCI_ULP_TRANSMITTER_TEST_CFM_T *)message)->status);
                return;

            case DM_HCI_ULP_RECEIVER_TEST_CFM:
                PRINT(("DM_HCI_ULP_RECEIVER_TEST_CFM\n"));
                connectionSendUlpReceiverTestCfmToClient(theCm->theAppTask, ((const DM_HCI_ULP_RECEIVER_TEST_CFM_T *)message)->status);
                return;

            case DM_HCI_ULP_TEST_END_CFM:
                PRINT(("DM_HCI_ULP_TEST_END_CFM\n"));
                if (((const DM_HCI_ULP_TEST_END_CFM_T *)message)->status == HCI_SUCCESS && state == connectionTestMode)
                {
                    SET_CM_STATE(connectionReady);
                }
                connectionSendUlpTestEndCfmToClient(theCm->theAppTask,
                                                    ((const DM_HCI_ULP_TEST_END_CFM_T *)message)->status,
                                                    ((const DM_HCI_ULP_TEST_END_CFM_T *)message)->number_of_packets);
                return;
#endif /* DISABLE_BLE */

            case DM_SET_BT_VERSION_CFM:
                PRINT(("DM_SET_BT_VERSION_CFM\n"));
                connectionHandleSetBtVersionCfm(&theCm->infoState, (const DM_SET_BT_VERSION_CFM_T*)message);
                return;

            case DM_HCI_READ_LOCAL_VER_INFO_CFM:
                PRINT(("DM_HCI_READ_LOCAL_VER_INFO_CFM\n"));
                connectionHandleReadLocalVersionCfm(&theCm->infoState, (const DM_HCI_READ_LOCAL_VER_INFO_CFM_T *) message);
                return;

            case DM_AM_REGISTER_CFM:
                PRINT(("DM_AM_REGISTER_CFM\n"));
                if (state == connectionInitialising)
                {
                    connectionSendInternalInitCfm(connectionInitDm);
                    return;
                }
                break;

            case DM_SM_ADD_DEVICE_CFM:
                PRINT(("DM_SM_ADD_DEVICE_CFM\n"));
                if (state == connectionInitialising)
                {
                    connectionHandleSmAddDeviceCfm(&theCm->smState, (const DM_SM_ADD_DEVICE_CFM_T*)message);
                    return;
                }
                break;

            case DM_SM_ACCESS_IND:
                PRINT(("DM_SM_ACCESS_IND\n"));
                if (state != connectionTestMode)
                {
                    connectionHandleSmAccessInd(&theCm->sdpState, (const DM_SM_ACCESS_IND_T*)message);
                    return;
                }
                break;

            /* The DM_PRIMS:
                    DM_SM_SET_DEFAULT_SECURITY_REQ
                    DM_SM_SET_SEC_MODE_REQ
                    DM_SM_SET_SEC_MODE_CFM
                    DM_SM_SEC_MODE_CONFIG_REQ
                    DM_SM_SEC_MODE_CONFIG_CFM

               Have been deprecated and replaced by
                    DM_SM_INIT_REQ
                    DM_SM_INIT_CFM

               Connection library maintains the upward API however (except
               during initialisation) for the functions:
                    ConnectionSmSetSecurityMode
                    ConnectionSmSecModeConfig
                    ConnectionSmSetSecurityLevel
             */
            case DM_SM_INIT_CFM:
                PRINT(("DM_SM_INIT_CFM\n"));

                if (state == connectionInitialising)
                {
                    /* During library intialisation, this message can only
                       be in response to a connectionSmInit function call.
                     */
                    handleSecurityInitCfm(
                        (const DM_SM_INIT_CFM_T *)message
                        );
                }
                else if (state == connectionReady)
                {
                    switch (theCm->smState.sm_init_msg)
                    {
                        case sm_init_set_security_mode:
                            connectionHandleSetSecurityModeCfm(
                                &theCm->smState,
                                (const DM_SM_INIT_CFM_T *)message
                                );
                            break;
                        case sm_init_set_security_config:
                            connectionHandleConfigureSecurityCfm(
                                &theCm->smState,
                                (const DM_SM_INIT_CFM_T *)message);
                            break;
                        case sm_init_set_security_default:
                            /* Connection/App expects no cfm for this */
                            break;
                        default:
                            Panic();
                            break;
                    }
                }
                else /* (state == connectionTestMode) */
                {
                    /* Hijack the DM_SM_INIT_CFM response for the call to
                     ConnectionSmSetSecurityMode in
                     connectionHandleEnterDutModeReq
                     */

                    connectionHandleDutSecurityDisabled(
                        theCm->theAppTask,
                        (const DM_SM_INIT_CFM_T *)message
                        );
                }
                theCm->smState.sm_init_msg = sm_init_set_none;
                return;

            case DM_HCI_ENABLE_DUT_MODE_CFM:
                PRINT(("DM_HCI_ENABLE_DUT_MODE_CFM\n"));
                if (state == connectionTestMode)
                {
                    connectionHandleDutCfm(theCm->theAppTask, (const DM_HCI_ENABLE_DUT_MODE_CFM_T *) message);
                    return;
                }
                break;

#ifndef DISABLE_BLE

            case DM_HCI_ULP_ADVERTISING_REPORT_IND:
                PRINT(("DM_HCI_ULP_ADVERTISING_REPORT_IND\n"));
                connectionHandleDmBleAdvertisingReportInd(
                        (const DM_HCI_ULP_ADVERTISING_REPORT_IND_T *)message
                        );
                return;

            case DM_HCI_ULP_SET_ADVERTISING_DATA_CFM:
                PRINT(("DM_HCI_ULP_SET_ADVERTISING_DATA_CFM\n"));
                connectionHandleDmBleSetAdvertisingDataCfm(
                        (const DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_T *)message
                        );
                return;

            case DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM:
                PRINT(("DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM\n"));
                connectionHandleDmBleSetAdvertisingParamsCfm(
                    (const DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_T*) message);
                return;

            case DM_SET_BLE_CONNECTION_PARAMETERS_CFM:
                PRINT(("DM_SET_BLE_CONNECTION_PARAMETERS_CFM\n"));
                connectionHandleDmSetBleConnectionParametersCfm(
                    (const DM_SET_BLE_CONNECTION_PARAMETERS_CFM_T*)message);
                return;

            case DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM:
                PRINT(("DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM\n"));
                connectionHandleDmBleSetScanParamatersCfm(
                        (const DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM_T*)message
                        );
                return;

            case DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM:
                PRINT(("DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM\n"));
                connectionHandleDmBleSetScanResponseDataCfm(
                        (const DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_T*)message
                        );
                return;

            case DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM:
                PRINT(("DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM\n"));
                connectionHandleDmBleReadWhiteListSizeCfm(
                        (const DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM_T*)message
                        );
                return;

            case DM_HCI_ULP_CLEAR_WHITE_LIST_CFM:
                PRINT(("DM_HCI_ULP_CLEAR_WHITE_LIST_CFM\n"));
                connectionHandleDmBleClearWhiteListCfm(
                        (const DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_T*)message
                        );
                return;

            case DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM:
                PRINT(("DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM\n"));
                connectionHandleDmBleAddDeviceToWhiteListCfm(
                        (const DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_T*)message
                        );
                return;

            case DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM:
                PRINT(("DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM\n"));
                connectionHandleDmBleRemoveDeviceFromWhiteListCfm(
                        (const DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_T*)message
                        );
                return;

            case DM_SM_AUTO_CONFIGURE_LOCAL_ADDRESS_CFM:
                PRINT(("DM_SM_AUTO_CONFIGURE_LOCAL_ADDRESS_CFM\n"));
                connectionHandleDmSmAutoConfigureLocalAddressCfm(
                        (const DM_SM_AUTO_CONFIGURE_LOCAL_ADDRESS_CFM_T*)message
                        );
                return;

            case DM_SM_READ_RANDOM_ADDRESS_CFM:
                PRINT(("DM_SM_READ_RANDOM_ADDRESS_CFM\n"));
                connectionHandleDmSmReadRandomAddressCfm(
                        (const DM_SM_READ_RANDOM_ADDRESS_CFM_T *)message
                        );
                return;

            case DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM:
                 PRINT(("DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM\n"));
                 connectionHandleReadBleAdChanTxPwrCfm(
                         &theCm->readTxPwrState,
                         (const DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_T*)message
                         );
                 return;

            case DM_HCI_ULP_SET_SCAN_ENABLE_CFM:
                 PRINT(("DM_HCI_ULP_SET_SCAN_ENABLE_CFM\n"));
                 connectionHandleDmBleSetScanEnableCfm(
                         &theCm->bleScanAdState,
                         (const DM_HCI_ULP_SET_SCAN_ENABLE_CFM_T*)message
                         );
                 return;

            case DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM:
                 PRINT(("DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM\n"));
                 connectionHandleDmBleSetAdvertiseEnableCfm(
                         &theCm->bleScanAdState,
                         (const DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_T*)message
                         );
                 return;

            case DM_ULP_GET_ADV_SCAN_CAPABILITIES_CFM:
                PRINT(("DM_ULP_GET_ADV_SCAN_CAPABILITIES_CFM\n"));
                connectionHandleDmBleGetAdvScanCapabilitiesCfm(
                        &theCm->dmExtAdvState,
                        (const DM_ULP_GET_ADV_SCAN_CAPABILITIES_CFM_T *)message
                        );
                return;

            case DM_ULP_EXT_ADV_TERMINATED_IND:
                PRINT(("DM_ULP_EXT_ADV_TERMINATED_IND\n"));
                connectionHandleDmBleExtAdvTerminatedInd(
                        (const DM_ULP_EXT_ADV_TERMINATED_IND_T *)message
                        );
                return;

            case DM_ULP_EXT_SCAN_DURATION_EXPIRED_IND:
                PRINT(("DM_ULP_EXT_SCAN_DURATION_EXPIRED_IND\n"));
                connectionHandleDmBleExtScanDurationExpiredInd(
                        (const DM_ULP_EXT_SCAN_DURATION_EXPIRED_IND_T *)message
                        );
                return;

            case DM_ULP_EXT_SCAN_FILTERED_ADV_REPORT_IND:
                PRINT(("DM_ULP_EXT_SCAN_FILTERED_ADV_REPORT_IND\n"));
                connectionHandleDmBleExtScanFilteredAdvReportInd(
                        (const DM_ULP_EXT_SCAN_FILTERED_ADV_REPORT_IND_T *)message
                        );
                return;

            case DM_ULP_PERIODIC_SCAN_SYNC_ADV_REPORT_IND:
                PRINT(("DM_ULP_PERIODIC_SCAN_SYNC_ADV_REPORT_IND\n"));
                connectionHandleDmBlePeriodicScanSyncAdvReportInd(
                        (const DM_ULP_PERIODIC_SCAN_SYNC_ADV_REPORT_IND_T *)message
                        );
                return;

            case DM_ULP_EXT_SCAN_CTRL_SCAN_INFO_IND:
                PRINT(("DM_ULP_EXT_SCAN_CTRL_SCAN_INFO_IND\n"));
                connectionHandleDmBleExtScanCtrlScanInfoInd(
                        (const DM_ULP_EXT_SCAN_CTRL_SCAN_INFO_IND_T *)message
                        );
                return;

            case DM_ULP_EXT_SCAN_GET_GLOBAL_PARAMS_CFM:
                PRINT(("DM_ULP_EXT_SCAN_GET_GLOBAL_PARAMS_CFM\n"));
                connectionHandleDmBleExtScanGetGlobalParamsCfm(
                        &theCm->dmExtScanState,
                        (const DM_ULP_EXT_SCAN_GET_GLOBAL_PARAMS_CFM_T*)message
                        );
                return;

            case DM_ULP_EXT_SCAN_SET_GLOBAL_PARAMS_CFM:
                PRINT(("DM_ULP_EXT_SCAN_SET_GLOBAL_PARAMS_CFM\n"));
                connectionHandleDmBleExtScanSetParamsCfm(
                        &theCm->dmExtScanState,
                        (const DM_ULP_EXT_SCAN_SET_GLOBAL_PARAMS_CFM_T*)message
                        );
                return;

            case DM_ULP_EXT_SCAN_ENABLE_SCANNERS_CFM:
                 PRINT(("DM_ULP_EXT_SCAN_ENABLE_SCANNERS_CFM\n"));
                 connectionHandleDmBleExtScanEnableCfm(
                         &theCm->dmExtScanState,
                         (const DM_ULP_EXT_SCAN_ENABLE_SCANNERS_CFM_T*)message
                         );
                 return;

            case DM_ULP_EXT_SCAN_REGISTER_SCANNER_CFM:
                 PRINT(("DM_ULP_EXT_SCAN_REGISTER_SCANNER_CFM\n"));
                 connectionHandleDmBleExtScanRegisterScannerCfm(
                         &theCm->dmExtScanState,
                         (const DM_ULP_EXT_SCAN_REGISTER_SCANNER_CFM_T*)message
                         );
                 return;

            case DM_ULP_EXT_SCAN_UNREGISTER_SCANNER_CFM:
                 PRINT(("DM_ULP_EXT_SCAN_UNREGISTER_SCANNER_CFM\n"));
                 connectionHandleDmBleExtScanUnregisterScannerCfm(
                         &theCm->dmExtScanState,
                         (const DM_ULP_EXT_SCAN_UNREGISTER_SCANNER_CFM_T*)message
                         );
                 return;

            case DM_ULP_EXT_ADV_SETS_INFO_CFM:
                PRINT(("DM_ULP_EXT_ADV_SETS_INFO_CFM\n"));
                connectionHandleDmBleExtAdvSetsInfoCfm(
                        &theCm->dmExtAdvState,
                        (const DM_ULP_EXT_ADV_SETS_INFO_CFM_T*) message);
                return;

            case DM_ULP_EXT_ADV_REGISTER_APP_ADV_SET_CFM:
                PRINT(("DM_ULP_EXT_ADV_REGISTER_APP_ADV_SET_CFM\n"));
                connectionHandleDmBleExtAdvRegisterAppAdvSetCfm(
                        &theCm->dmExtAdvState,
                        (const DM_ULP_EXT_ADV_REGISTER_APP_ADV_SET_CFM_T*) message);
                return;

            case DM_ULP_EXT_ADV_UNREGISTER_APP_ADV_SET_CFM:
                PRINT(("DM_ULP_EXT_ADV_UNREGISTER_APP_ADV_SET_CFM\n"));
                connectionHandleDmBleExtAdvUnregisterAppAdvSetCfm(
                        &theCm->dmExtAdvState,
                        (const DM_ULP_EXT_ADV_UNREGISTER_APP_ADV_SET_CFM_T*) message);
                return;

            case DM_ULP_EXT_ADV_SET_PARAMS_CFM:
                PRINT(("DM_ULP_EXT_ADV_SET_PARAMS_CFM\n"));
                connectionHandleDmBleExtAdvSetParamsCfm(
                        &theCm->dmExtAdvState,
                        (const DM_ULP_EXT_ADV_SET_PARAMS_CFM_T*) message);
                return;

            case DM_ULP_EXT_ADV_MULTI_ENABLE_CFM:
                PRINT(("DM_ULP_EXT_ADV_MULTI_ENABLE_CFM\n"));
                connectionHandleDmBleExtAdvMultiEnableCfm(
                        &theCm->dmExtAdvState,
                        (const DM_ULP_EXT_ADV_MULTI_ENABLE_CFM_T*) message);
                return;

            case DM_ULP_EXT_ADV_SET_RANDOM_ADDR_CFM:
                PRINT(("DM_ULP_EXT_ADV_SET_RANDOM_ADDR_CFM\n"));
                connectionHandleDmBleExtAdvSetRandomAddressCfm(
                        &theCm->dmExtAdvState,
                        (const DM_ULP_EXT_ADV_SET_RANDOM_ADDR_CFM_T*) message);
                return;

            case DM_ULP_EXT_ADV_ENABLE_CFM:
                 PRINT(("DM_HCI_ULP_EXT_ADV_ENABLE_CFM\n"));
                 connectionHandleDmBleExtAdvertiseEnableCfm(
                         &theCm->dmExtAdvState,
                         (const DM_ULP_EXT_ADV_ENABLE_CFM_T*)message
                         );
                 return;

            case DM_HCI_ULP_EXT_ADV_SET_DATA_CFM:
                PRINT(("DM_HCI_ULP_EXT_ADV_SET_DATA_CFM\n"));
                connectionHandleDmBleExtAdvSetDataCfm(
                        &theCm->dmExtAdvState,
                        (const DM_HCI_ULP_EXT_ADV_SET_DATA_CFM_T *)message
                        );
                return;

            case DM_HCI_ULP_EXT_ADV_SET_SCAN_RESP_DATA_CFM:
                PRINT(("DM_HCI_ULP_EXT_ADV_SET_SCAN_RESP_DATA_CFM\n"));
                connectionHandleDmBleExtAdvSetScanRespDataCfm(
                        &theCm->dmExtAdvState,
                        (const DM_HCI_ULP_EXT_ADV_SET_SCAN_RESP_DATA_CFM_T*)message
                        );
                return;

            case DM_ULP_EXT_ADV_READ_MAX_ADV_DATA_LEN_CFM:
                PRINT(("DM_ULP_EXT_ADV_READ_MAX_ADV_DATA_LEN_CFM\n"));
                connectionHandleDmBleExtAdvReadMaxAdvDataLenCfm(
                        &theCm->dmExtAdvState,
                        (const DM_ULP_EXT_ADV_READ_MAX_ADV_DATA_LEN_CFM_T*)message
                        );
                return;

            case DM_ULP_PERIODIC_ADV_SET_PARAMS_CFM:
                PRINT(("DM_ULP_PERIODIC_ADV_SET_PARAMS_CFM\n"));
                connectionHandleDmBlePerAdvSetParamsCfm(
                    &theCm->dmPerAdvState,
                    (const DM_ULP_PERIODIC_ADV_SET_PARAMS_CFM_T*) message);
                return;

            case DM_HCI_ULP_PERIODIC_ADV_SET_DATA_CFM:
                PRINT(("DM_HCI_ULP_PERIODIC_ADV_SET_DATA_CFM\n"));
                connectionHandleDmBlePerAdvSetDataCfm(
                    &theCm->dmPerAdvState,
                    (const DM_HCI_ULP_PERIODIC_ADV_SET_DATA_CFM_T *)message
                    );
                return;

            case DM_ULP_PERIODIC_ADV_START_CFM:
                PRINT(("DM_ULP_PERIODIC_ADV_START_CFM\n"));
                connectionHandleDmBlePerAdvStartCfm(
                    &theCm->dmPerAdvState,
                    (const DM_ULP_PERIODIC_ADV_START_CFM_T*)message
                    );
                return;

            case DM_ULP_PERIODIC_ADV_STOP_CFM:
                PRINT(("DM_ULP_PERIODIC_ADV_STOP_CFM\n"));
                connectionHandleDmBlePerAdvStopCfm(
                    &theCm->dmPerAdvState,
                    (const DM_ULP_PERIODIC_ADV_STOP_CFM_T *)message
                    );
                return;

            case DM_ULP_PERIODIC_ADV_SET_TRANSFER_CFM:
                PRINT(("DM_ULP_PERIODIC_ADV_SET_TRANSFER_CFM\n"));
                connectionHandleDmBlePerAdvSetTransferCfm(
                    &theCm->dmPerAdvState,
                    (const DM_ULP_PERIODIC_ADV_SET_TRANSFER_CFM_T *)message
                    );
                return;

            case DM_ULP_PERIODIC_ADV_READ_MAX_ADV_DATA_LEN_CFM:
                PRINT(("DM_ULP_PER_ADV_READ_MAX_ADV_DATA_LEN_CFM\n"));
                connectionHandleDmBlePerAdvReadMaxAdvDataLenCfm(
                        &theCm->dmPerAdvState,
                        (const DM_ULP_PERIODIC_ADV_READ_MAX_ADV_DATA_LEN_CFM_T*)message
                        );
                return;

            case DM_ULP_PERIODIC_SCAN_SYNC_TO_TRAIN_CFM:
                PRINT(("DM_ULP_PERIODIC_SCAN_SYNC_TO_TRAIN_CFM\n"));
                connectionHandleDmBlePeriodicScanSyncTrainCfm(
                        &theCm->dmPerScanState,
                        (const DM_ULP_PERIODIC_SCAN_SYNC_TO_TRAIN_CFM_T *)message
                        );
                return;

            case DM_ULP_PERIODIC_SCAN_SYNC_TERMINATE_CFM:
                PRINT(("DM_ULP_PERIODIC_SCAN_SYNC_TERMINATE_CFM\n"));
                connectionHandleDmBlePeriodicScanSyncTerminateCfm(
                        &theCm->dmPerScanState,
                        (const DM_ULP_PERIODIC_SCAN_SYNC_TERMINATE_CFM_T *)message
                        );
                return;

            case DM_ULP_PERIODIC_SCAN_SYNC_TO_TRAIN_CANCEL_CFM:
                PRINT(("DM_ULP_PERIODIC_SCAN_SYNC_TO_TRAIN_CANCEL_CFM\n"));
                connectionHandleDmBlePeriodicScanSyncCancelCfm(
                        &theCm->dmPerScanState,
                        (const DM_ULP_PERIODIC_SCAN_SYNC_TO_TRAIN_CANCEL_CFM_T *)message
                        );
                return;

            case DM_ULP_PERIODIC_SCAN_SYNC_TRANSFER_CFM:
                PRINT(("DM_ULP_PERIODIC_SCAN_SYNC_TRANSFER_CFM\n"));
                connectionHandleDmBlePeriodicScanSyncTransferCfm(
                        &theCm->dmPerScanState,
                        (const DM_ULP_PERIODIC_SCAN_SYNC_TRANSFER_CFM_T *)message
                        );
                return;

            case DM_ULP_PERIODIC_SCAN_SYNC_TRANSFER_PARAMS_CFM:
                PRINT(("DM_ULP_PERIODIC_SCAN_SYNC_TRANSFER_PARAMS_CFM\n"));
                connectionHandleDmBlePeriodicScanSyncTransferParamsCfm(
                        &theCm->dmPerScanState,
                        (const DM_ULP_PERIODIC_SCAN_SYNC_TRANSFER_PARAMS_CFM_T *)message
                        );
                return;

            case DM_ULP_PERIODIC_SCAN_SYNC_LOST_IND:
                PRINT(("DM_ULP_PERIODIC_SCAN_SYNC_LOST_IND\n"));
                connectionHandleDmBlePeriodicScanSyncLostInd(
                        (const DM_ULP_PERIODIC_SCAN_SYNC_LOST_IND_T *)message
                        );
                return;

            case DM_ULP_PERIODIC_SCAN_SYNC_TRANSFER_IND:
                PRINT(("DM_ULP_PERIODIC_SCAN_SYNC_TRANSFER_IND\n"));
                connectionHandleDmBlePeriodicScanSyncTransferInd(
                        (const DM_ULP_PERIODIC_SCAN_SYNC_TRANSFER_IND_T *)message
                        );
                return;

            case DM_ULP_PERIODIC_SCAN_START_FIND_TRAINS_CFM:
                PRINT(("DM_ULP_PERIODIC_SCAN_START_FIND_TRAINS_CFM\n"));
                connectionHandleDmBlePeriodicScanStartFindTrainsCfm(
                        &theCm->dmPerScanState,
                        (const DM_ULP_PERIODIC_SCAN_START_FIND_TRAINS_CFM_T *)message
                        );
                return;

            case DM_ULP_PERIODIC_SCAN_STOP_FIND_TRAINS_CFM:
                PRINT(("DM_ULP_PERIODIC_SCAN_STOP_FIND_TRAINS_CFM\n"));
                connectionHandleDmBlePeriodicScanStopFindTrainsCfm(
                        &theCm->dmPerScanState,
                        (const DM_ULP_PERIODIC_SCAN_STOP_FIND_TRAINS_CFM_T *)message
                        );
                return;

            case DM_ULP_PERIODIC_SCAN_SYNC_ADV_REPORT_ENABLE_CFM:
                PRINT(("DM_ULP_PERIODIC_SCAN_SYNC_ADV_REPORT_ENABLE_CFM\n"));
                connectionHandleDmBlePeriodicScanSyncAdvReportEnableCfm(
                        &theCm->dmPerScanState,
                        (const DM_ULP_PERIODIC_SCAN_SYNC_ADV_REPORT_ENABLE_CFM_T *)message
                        );
                return;
            case DM_ULP_ENHANCED_READ_TRANSMIT_POWER_LEVEL_CFM:
                PRINT(("DM_ULP_ENHANCED_READ_TRANSMIT_POWER_LEVEL_CFM\n"));
                connectionHandleUlpEnhancedReadTransmitPowerLevelCfm(&theCm->generalLockState, (const DM_ULP_ENHANCED_READ_TRANSMIT_POWER_LEVEL_CFM_T *)message);
                return;

            case DM_ULP_READ_REMOTE_TRANSMIT_POWER_LEVEL_CFM:
                PRINT(("DM_ULP_READ_REMOTE_TRANSMIT_POWER_LEVEL_CFM\n"))
                connectionHandleUlpReadRemoteTransmitPowerLevelCfm(&theCm->generalLockState,(const DM_ULP_READ_REMOTE_TRANSMIT_POWER_LEVEL_CFM_T *)message);
                return;

            case DM_ULP_SET_PATH_LOSS_REPORTING_PARAMETERS_CFM:
                PRINT(("DM_ULP_SET_PATH_LOSS_REPORTING_PARAMETERS_CFM\n"));
                connectionHandleUlpSetPathLossReportingParametersCfm(&theCm->generalLockState,(DM_ULP_SET_PATH_LOSS_REPORTING_PARAMETERS_CFM_T *)message);
                return;

            case DM_ULP_SET_PATH_LOSS_REPORTING_ENABLE_CFM:
                PRINT(("DM_ULP_SET_PATH_LOSS_REPORTING_ENABLE_CFM\n"));
                connectionHandleUlpSetPathLossReportingEnableCfm(&theCm->generalLockState, (DM_ULP_SET_PATH_LOSS_REPORTING_ENABLE_CFM_T *)message);
                return;

            case DM_ULP_SET_TRANSMIT_POWER_REPORTING_ENABLE_CFM:
                PRINT(("DM_ULP_SET_TRANSMIT_POWER_REPORTING_ENABLE_CFM\n"));
                connectionHandleSetTransmitPowerReportingEnableCfm(&theCm->generalLockState, (DM_ULP_SET_TRANSMIT_POWER_REPORTING_ENABLE_CFM_T *)message);
                return;

            case DM_ULP_PATH_LOSS_THRESHOLD_IND:
                PRINT(("DM_ULP_PATH_LOSS_THRESHOLD_IND\n"));
                connectionHandleUlpPathLossThresholdInd((const DM_ULP_PATH_LOSS_THRESHOLD_IND_T*)message);
                return;

            case DM_ULP_TRANSMIT_POWER_REPORTING_IND:
                PRINT(("DM_ULP_TRANSMIT_POWER_REPORTING_IND\n"));
                connectionHandleUlpTransmitPowerReportingInd((const DM_ULP_TRANSMIT_POWER_REPORTING_IND_T*)message);
                return;

            case DM_ULP_PERIODIC_ADV_ENABLE_CFM:
                PRINT(("DM_ULP_PERIODIC_ADV_ENABLE_CFM\n"));
                connectionHandleDmBlePeriodicAdvEnableCfm(
                        &theCm->dmPerAdvState, 
                        (const DM_ULP_PERIODIC_ADV_ENABLE_CFM_T *)message
                        );
                return;

            case DM_ULP_SET_DATA_RELATED_ADDRESS_CHANGES_CFM:
                PRINT(("DM_ULP_SET_DATA_RELATED_ADDRESS_CHANGES_CFM\n"));
                connectionHandleDmUlpSetDataRelatedAddressChangeCfm(
                        &theCm->dmExtAdvState,
                        (const DM_ULP_SET_DATA_RELATED_ADDRESS_CHANGES_CFM_T*)message
                        );
                return;

#endif /* DISABLE_BLE */

            case DM_HCI_WRITE_SCAN_ENABLE_CFM:
                connectionHandleWriteScanEnableCfm(
                    (const DM_HCI_WRITE_SCAN_ENABLE_CFM_T *)message);

                return;

            /* In the debug version of the lib check the status otherwise ignore. These prims all return
               a message which is essentially a DM_HCI_STANDARD_COMMAND_COMPLETE so we handle them all
               in the same way.    */
            case DM_HCI_WRITE_CLASS_OF_DEVICE_CFM:
            case DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM:
            case DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM:
            case DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM:
            case DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM:
            case DM_HCI_SET_EVENT_FILTER_CFM:
            case DM_WRITE_CACHED_PAGE_MODE_CFM:
            case DM_WRITE_CACHED_CLOCK_OFFSET_CFM:
            case DM_CLEAR_PARAM_CACHE_CFM:
            case DM_HCI_CHANGE_LOCAL_NAME_CFM:
            case DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM:
            case DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM:
            case DM_HCI_SNIFF_SUB_RATE_CFM:
            case DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM:
                checkStatus(message);
                return;

            /* Primitives we ignore. These have a specific return message which we cannot generalise
               with the macro we use for the primitives above so for the moment ignore these. */
            case DM_SM_REMOVE_DEVICE_CFM:
            case DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM:
            case DM_HCI_DELETE_STORED_LINK_KEY_CFM:
            case DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM:
            case DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM:
            case DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM:
            case DM_HCI_CHANGE_CONN_LINK_KEY_CFM:
            case DM_HCI_CREATE_CONNECTION_CANCEL_CFM:
                return;
        }
    }

    if (message->type == DM_BAD_MESSAGE_IND)
    {
       connectionHandleDmBadMessageInd((const DM_BAD_MESSAGE_IND_T *) message);
       return;
    }

    /* Prims we are not handling - for now panic the app */
    handleUnexpected(connectionUnexpectedDmPrim, state, message->type);
}

/****************************************************************************
NAME
    connectionBluestackHandlerReadyL2cap

DESCRIPTION
    L2CAP message handler for the ready state.

RETURNS
    void
*/
#if !defined(CL_EXCLUDE_L2CAP) || !defined(DISABLE_BLE)
static void connectionBluestackHandlerL2cap(connectionState *theCm, const L2CA_UPRIM_T *message)
{
    if (theCm->state == connectionReady)
    {
        switch (message->type)
        {
#ifndef CL_EXCLUDE_L2CAP
            case L2CA_REGISTER_CFM:
                PRINT(("L2CA_REGISTER_CFM\n"));
                connectionHandleL2capRegisterCfm((const L2CA_REGISTER_CFM_T *)message);
                return;

            case L2CA_UNREGISTER_CFM:
                PRINT(("L2CA_UNREGISTER_CFM\n"));
                connectionHandleL2capUnregisterCfm((const L2CA_UNREGISTER_CFM_T *) message);
                return;

            case L2CA_AUTO_CONNECT_IND:
                PRINT(("L2CA_CONNECT_IND\n"));
                connectionHandleL2capConnectInd((const L2CA_AUTO_CONNECT_IND_T *) message);
                return;

            case L2CA_AUTO_TP_CONNECT_IND:
                PRINT(("L2CA_TP_CONNECT_IND\n"));
                connectionHandleL2capTpConnectInd((const L2CA_AUTO_TP_CONNECT_IND_T *) message);
                return;

            case L2CA_AUTO_CONNECT_CFM:
                PRINT(("L2CA_CONNECT_CFM\n"));
                connectionHandleL2capConnectCfm((const L2CA_AUTO_CONNECT_CFM_T *)message);
                return;

            case L2CA_AUTO_TP_CONNECT_CFM:
                PRINT(("L2CA_TP_CONNECT_CFM\n"));
                connectionHandleL2capTpConnectCfm((const L2CA_AUTO_TP_CONNECT_CFM_T *)message);
                return;

            case L2CA_ADD_CREDIT_CFM:
                PRINT(("L2CA_ADD_CREDIT_CFM\n"));
                connectionHandleL2capAddCreditCfm((const L2CA_ADD_CREDIT_CFM_T *)message);
                return;

            case L2CA_CONFIG_IND:
            case L2CA_CONFIG_CFM:
                PRINT(("L2CA_CONFIG_IND or L2CA_CONFIG_CFM\n"));
                Panic();        /* shouldn't get here */
                break;

            case L2CA_DISCONNECT_IND:
                PRINT(("L2CA_DISCONNECT_IND\n"));
                connectionHandleL2capDisconnectInd((const L2CA_DISCONNECT_IND_T *) message);
                return;

            case L2CA_DISCONNECT_CFM:
                PRINT(("L2CA_DISCONNECT_CFM\n"));
                connectionHandleL2capDisconnectCfm((const L2CA_DISCONNECT_CFM_T *) message);
                return;

            case L2CA_TIMEOUT_IND:
                PRINT(("L2CA_TIMEOUT_IND\n"));
                /* A disconnect is always indicated, usually before the TIMEOUT. */
                return;

            case L2CA_GETINFO_CFM:
                PRINT(("L2CA_GETINFO_CFM\n"));
                connectionHandleL2capGetInfoCfm(&theCm->l2capState, (const L2CA_GETINFO_CFM_T *)message);
                return;

            case L2CA_MAP_FIXED_CID_CFM:
                PRINT(("L2CA_MAP_FIXED_CID_CFM\n"));
                connectionHandleL2capMapFixedCidCfm(&theCm->l2capState, (const L2CA_MAP_FIXED_CID_CFM_T *) message);
                return;

            case L2CA_MAP_FIXED_CID_IND:
                PRINT(("L2CA_MAP_FIXED_CID_IND\n"));
                connectionHandleL2capMapFixedCidInd((const L2CA_MAP_FIXED_CID_IND_T *) message);
                return;

            case L2CA_UNMAP_FIXED_CID_IND:
                PRINT(("L2CA_UNMAP_FIXED_CID_IND\n"));
                connectionHandleL2capUnmapFixedCidInd((const L2CA_UNMAP_FIXED_CID_IND_T *) message);
                return;

            case L2CA_RAW_DATA_IND:
                free( VmGetPointerFromHandle(
                            ((const L2CA_RAW_DATA_IND_T*)message)->data
                            )
                    );
                return;
#endif

        }
    }

    /* Prims we are not handling - for now panic the app */
    handleUnexpected(connectionUnexpectedL2capPrim, theCm->state, message->type);
}
#endif

/****************************************************************************
NAME
    connectionBluestackHandlerReadyRfcomm

DESCRIPTION
    Rfcomm message handler for the ready state.

RETURNS
    void
*/
#ifndef CL_EXCLUDE_RFCOMM
static void connectionBluestackHandlerRfcomm(connectionState *theCm, const RFCOMM_UPRIM_T *message)
{
    if (theCm->state == connectionReady)
    {
        switch (message->type)
        {
            case RFC_REGISTER_CFM:
                PRINT(("RFC_REGISTER_CFM\n"));
                connectionHandleRfcommRegisterCfm((const RFC_REGISTER_CFM_T*)message);
                return;

            case RFC_UNREGISTER_CFM:
                PRINT(("RFC_UNREGISTER_CFM\n"));
                connectionHandleRfcommUnregisterCfm((const RFC_UNREGISTER_CFM_T*)message);
                return;

            case RFC_CLIENT_CONNECT_CFM:
                PRINT(("RFC_CLIENT_CONNECT_CFM\n"));
                connectionHandleRfcommClientConnectCfm((const RFC_CONNECT_CFM_T*)message);
                return;

            case RFC_SERVER_CONNECT_CFM:
                PRINT(("RFC_SERVER_CONNECT_CFM\n"));
                connectionHandleRfcommServerConnectCfm((const RFC_SERVER_CONNECT_CFM_T*)message);
                return;

            case RFC_SERVER_CONNECT_IND:
                PRINT(("RFC_SERVER_CONNECT_IND\n"));
                connectionHandleRfcommConnectInd((const RFC_SERVER_CONNECT_IND_T*)message);
                return;

            case RFC_DISCONNECT_IND:
                PRINT(("RFC_DISCONNECT_IND"));
                connectionHandleRfcommDisconnectInd((const RFC_DISCONNECT_IND_T*)message);
                return;

            case RFC_DISCONNECT_CFM:
                PRINT(("RFC_DISCONNECT_CFM"));
                connectionHandleRfcommDisconnectCfm((const RFC_DISCONNECT_CFM_T*)message);
                return;

            case RFC_PORTNEG_CFM:
                PRINT(("RFC_PORTNEG_CFM"));
                connectionHandleRfcommPortNegCfm((const RFC_PORTNEG_CFM_T*)message);
                return;

            case RFC_PORTNEG_IND:
                PRINT(("RFC_PORTNEG_IND"));
                connectionHandleRfcommPortNegInd((const RFC_PORTNEG_IND_T*)message);
                return;

            case RFC_MODEM_STATUS_CFM:
                PRINT(("RFC_MODEM_STATUS_CFM"));
                connectionHandleRfcommControlCfm((const RFC_MODEM_STATUS_CFM_T*)message);
                return;

            case RFC_MODEM_STATUS_IND:
                PRINT(("RFC_MODEM_STATUS_IND"));
                connectionHandleRfcommControlInd((const RFC_MODEM_STATUS_IND_T*)message);
                return;

            case RFC_LINESTATUS_IND:
                PRINT(("RFC_LINESTATUS_IND"));
                connectionHandleRfcommLineStatusInd((const RFC_LINESTATUS_IND_T*)message);
                return;

            case RFC_LINESTATUS_CFM:
                PRINT(("RFC_LINESTATUS_CFM"));
                connectionHandleRfcommLineStatusCfm((const RFC_LINESTATUS_CFM_T*)message);
                return;

            /* Ignore these messages */
            case RFC_ERROR_IND:
                PRINT(("RFC_ERROR_IND"));
                return;

            case RFC_NSC_IND:
                PRINT(("RFC_NSC_IND"));
                return;

            /* Necessary to avoid compiler warning for downstream prims. */
            default:
                break;
        }
    }
    else if (theCm->state == connectionInitialising)
    {
        if (message->type == RFC_INIT_CFM)
        {
            PRINT(("RFC_INIT_CFM\n"));
            connectionSendInternalInitCfm(connectionInitRfc);
            return;
        }
    }

    /* Prims we are not handling - for now panic the app */
    handleUnexpected(connectionUnexpectedRfcPrim, theCm->state, message->type);
}
#endif

/****************************************************************************
NAME
    connectionBluestackHandlerReadySdp

DESCRIPTION
    SDP message handler for the ready state.

RETURNS
    void
*/
#ifndef CL_EXCLUDE_SDP
static void connectionBluestackHandlerSdp(connectionState *theCm, const SDS_UPRIM_T *message)
{
    if (theCm->state == connectionReady)
    {
        switch (message->type)
        {
            case SDS_REGISTER_CFM:
                PRINT(("SDS_REGISTER_CFM\n"));
                connectionHandleSdpRegisterCfm(&theCm->sdpState, (const SDS_REGISTER_CFM_T *)message);
                return;

            case SDS_UNREGISTER_CFM:
                PRINT(("SDS_UNREGISTER_CFM\n"));
                connectionHandleSdpUnregisterCfm(&theCm->sdpState, (const SDS_UNREGISTER_CFM_T *)message);
                return;

            case SDC_OPEN_SEARCH_CFM:
                PRINT(("SDC_OPEN_SEARCH_CFM\n"));
                connectionHandleSdpOpenSearchCfm(&theCm->sdpState, (const SDC_OPEN_SEARCH_CFM_T *)message);
                return;

            case SDC_CLOSE_SEARCH_IND:
                PRINT(("SDC_CLOSE_SEARCH_IND\n"));
                connectionHandleSdpCloseSearchInd(&theCm->sdpState, (const SDC_CLOSE_SEARCH_IND_T *)message);
                return;

            case SDC_SERVICE_SEARCH_CFM:
                PRINT(("SDC_SERVICE_SEARCH_CFM\n"));
                connectionHandleSdpServiceSearchCfm(&theCm->sdpState, (const SDC_SERVICE_SEARCH_CFM_T *)message);
                return;

            case SDC_SERVICE_ATTRIBUTE_CFM:
                PRINT(("SDC_SERVICE_ATTRIBUTE_CFM\n"));
                connectionHandleSdpAttributeSearchCfm(&theCm->sdpState, (const SDC_SERVICE_ATTRIBUTE_CFM_T *)message);
                return;

            case SDC_SERVICE_SEARCH_ATTRIBUTE_CFM:
                PRINT(("SDC_SERVICE_SEARCH_ATTRIBUTE_CFM\n"));
                connectionHandleSdpServiceSearchAttributeCfm(&theCm->sdpState, (const SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_T *)message);
                return;
        }
    }

    /* Prims we are not handling - for now panic the app */
    handleUnexpected(connectionUnexpectedSdpPrim, theCm->state, message->type);
}
#endif

/****************************************************************************
NAME
    connectionBluestackHandlerUninitialised

DESCRIPTION
    Message handler for the uninitialised state.

RETURNS
    void
*/
static void connectionBluestackHandlerUninitialised(connectionState *theCm, MessageId id, Message message)
{
    UNUSED(theCm);
    UNUSED(message);

    /* Depending upon the message id...*/
    if (id == CL_INTERNAL_INIT_REQ)
    {
        PRINT(("CL_INTERNAL_INIT_REQ\n"));
        connectionHandleInternalInit(connectionInit);
    }
    else
    {
        /* Prims we are not handling - Not panicing the app in DEBUG Mode. Just Print this INFO and ignore it.*/
        CL_DEBUG_INFO(("Ignored Unexpected Message - Code 0x%x State 0x%x MsgId MESSAGE:0x%x\n", connectionUnexpectedCmPrim, theCm->state, id));
    }
}

/****************************************************************************
NAME
    connectionBluestackHandlerInitialising

DESCRIPTION
    Message handler for the initialising state.

RETURNS
    void
*/
static void connectionBluestackHandlerInitialising(connectionState *theCm, MessageId id, Message message)
{
    /* Depending upon the message id...*/
    switch (id)
    {
        case CL_INTERNAL_INIT_CFM:
            PRINT(("CL_INTERNAL_INIT_CFM\n"));
            connectionHandleInternalInit(((const CL_INTERNAL_INIT_CFM_T*)message)->state);
            break;

        case CL_INTERNAL_INIT_TIMEOUT_IND:
            PRINT(("CL_INTERNAL_INIT_TIMEOUT_IND\n"));
            (void)MessageCancelFirst(&theCm->task, CL_INTERNAL_INIT_CFM);
            SET_CM_STATE(connectionUninitialised);
            connectionSendInitCfm(theCm->theAppTask, fail, bluetooth_unknown);
            break;

        case CL_INTERNAL_SM_INIT_REQ:
            PRINT(("CL_INTERNAL_SM_INIT_REQ\n"));
            handleSecurityInitReq((const CL_INTERNAL_SM_INIT_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_READ_LOCAL_VERSION_REQ:
             PRINT(("CL_INTERNAL_DM_READ_LOCAL_VERSION_REQ\n"));
             connectionHandleReadLocalVersionRequest(&theCm->infoState, (const CL_INTERNAL_DM_READ_LOCAL_VERSION_REQ_T *)message);
             break;

        case CL_INTERNAL_DM_SET_BT_VERSION_REQ:
            PRINT(("CL_INTERNAL_DM_SET_BT_VERSION_REQ\n"));
            connectionHandleSetBtVersionReq(&theCm->infoState, (const CL_INTERNAL_DM_SET_BT_VERSION_REQ_T *)message);
            break;

        case CL_INTERNAL_SM_ADD_DEVICE_AT_TDL_POS_REQ:
            PRINT(("CL_INTERNAL_SM_ADD_DEVICE_AT_TDL_POS_REQ\n"));
            connectionHandleAddDeviceAtTdlPosReq(&theCm->smState, (const CL_INTERNAL_SM_ADD_DEVICE_AT_TDL_POS_REQ_T *)message);
            break;

        default:
            /* Prims we are not handling - for now panic the app */
            handleUnexpected(connectionUnhandledMessage, theCm->state, id);
            break;
    }
}

/****************************************************************************
NAME
    connectionBluestackHandlerTestMode

DESCRIPTION
    Message handler for the test-mode state.

RETURNS
    void
*/
static void connectionBluestackHandlerTestMode(connectionState *theCm, MessageId id, Message message)
{
    /* Depending upon the message id...*/
    switch (id)
    {
        case CL_INTERNAL_SM_SET_SC_MODE_REQ:
            PRINT(("CL_INTERNAL_SM_SET_SC_MODE_REQ\n"));
            handleSetSecurityModeReq(&theCm->smState, theCm->flags, (const CL_INTERNAL_SM_SET_SC_MODE_REQ_T*)message);
            break;

        case CL_INTERNAL_SM_SET_SSP_SECURITY_LEVEL_REQ:
            PRINT(("CL_INTERNAL_SM_SET_SSP_SECURITY_LEVEL_REQ\n"));
            handleSetSspSecurityLevelReq(&theCm->smState, (const CL_INTERNAL_SM_SET_SSP_SECURITY_LEVEL_REQ_T*)message);
            break;

        case CL_INTERNAL_SM_SEC_MODE_CONFIG_REQ:
            PRINT(("CL_INTERNAL_SM_SEC_MODE_CONFIG_REQ\n"));
            handleSecModeConfigReq(&theCm->smState, &theCm->infoState, theCm->flags, (const CL_INTERNAL_SM_SEC_MODE_CONFIG_REQ_T*)message);
            break;

        case CL_INTERNAL_DM_WRITE_SCAN_ENABLE_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_SCAN_ENABLE_REQ\n"));
            connectionHandleWriteScanEnableRequest((const CL_INTERNAL_DM_WRITE_SCAN_ENABLE_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_READ_LOCAL_VERSION_REQ:
            PRINT(("CL_INTERNAL_DM_READ_LOCAL_VERSION_REQ\n"));
            connectionHandleReadLocalVersionRequest(&theCm->infoState, (const CL_INTERNAL_DM_READ_LOCAL_VERSION_REQ_T *) message);
            break;

        case CL_INTERNAL_DM_SET_BT_VERSION_REQ:
            PRINT(("CL_INTERNAL_DM_SET_BT_VERSION_REQ\n"));
            connectionHandleSetBtVersionReq(&theCm->infoState, (const CL_INTERNAL_DM_SET_BT_VERSION_REQ_T*)message);
            break;

        case CL_INTERNAL_DM_DUT_REQ:
            /* Once the device has entered DUT mode successfully make sure the connection library rejects
             * all subsequent requests to enter DUT and sends back a confirm message to the client.
             */
            PRINT(("CL_INTERNAL_DM_DUT_REQ\n"));
            connectionSendDutCfmToClient(theCm->theAppTask, fail);
            break;
#ifndef DISABLE_BLE
        case CL_INTERNAL_DM_ULP_TRANSMITTER_TEST_REQ:
            PRINT(("CL_INTERNAL_DM_ULP_TRANSMITTER_TEST_REQ\n"));
            connectionSendUlpTransmitterTestCfmToClient(theCm->theAppTask, hci_error_command_disallowed);
            break;

        case CL_INTERNAL_DM_ULP_RECEIVER_TEST_REQ:
            PRINT(("CL_INTERNAL_DM_ULP_RECEIVER_TEST_REQ\n"));
            connectionSendUlpReceiverTestCfmToClient(theCm->theAppTask, hci_error_command_disallowed);
            break;

        case CL_INTERNAL_DM_ULP_TEST_END_REQ:
            PRINT(("CL_INTERNAL_DM_ULP_TEST_END_REQ\n"));
            connectionHandleUlpTestEndReq();
            break; 

#endif /* DISABLE_BLE */

        default:
            /* Prims we are not handling - for now panic the app */
            handleUnexpected(connectionUnhandledMessage, theCm->state, id);
            break;
    }
}

/****************************************************************************
NAME
    connectionBluestackHandlerReady

DESCRIPTION
    Message handler for the ready state.

RETURNS
    void
*/
static void connectionBluestackHandlerReady(connectionState *theCm, MessageId id, Message message)
{
    /* Depending upon the message id...*/
    switch(id)
    {
        case CL_INTERNAL_SM_SEC_MODE_CONFIG_REQ:
            PRINT(("CL_INTERNAL_SM_SEC_MODE_CONFIG_REQ\n"));
            handleSecModeConfigReq(&theCm->smState, &theCm->infoState, theCm->flags, (const CL_INTERNAL_SM_SEC_MODE_CONFIG_REQ_T*)message);
            break;

        case CL_INTERNAL_SM_SET_SSP_SECURITY_LEVEL_REQ:
            PRINT(("CL_INTERNAL_SM_SET_SSP_SECURITY_LEVEL_REQ\n"));
            handleSetSspSecurityLevelReq(&theCm->smState, (const CL_INTERNAL_SM_SET_SSP_SECURITY_LEVEL_REQ_T*)message);
            break;

        case CL_INTERNAL_SM_READ_LOCAL_OOB_DATA_REQ:
            PRINT(("CL_INTERNAL_SM_READ_LOCAL_OOB_DATA_REQ\n"));
            connectionHandleReadLocalOobDataReq(&theCm->infoState, &theCm->smState, (const CL_INTERNAL_SM_READ_LOCAL_OOB_DATA_REQ_T*)message);
            break;

        case CL_INTERNAL_SM_AUTHENTICATION_REQ:
            PRINT(("CL_INTERNAL_SM_AUTHENTICATION_REQ\n"));
            connectionHandleAuthenticationReq(&theCm->smState, (const CL_INTERNAL_SM_AUTHENTICATION_REQ_T*)message);
            break;

        case CL_INTERNAL_SM_CANCEL_AUTHENTICATION_REQ:
            PRINT(("CL_INTERNAL_SM_CANCEL_AUTHENTICATION_REQ\n"));
            connectionHandleCancelAuthenticationReq(&theCm->infoState, &theCm->smState, (const CL_INTERNAL_SM_CANCEL_AUTHENTICATION_REQ_T*)message);
            break;

        case CL_INTERNAL_SM_AUTHENTICATION_TIMEOUT_IND:
            PRINT(("CL_INTERNAL_SM_AUTHENTICATION_TIMEOUT_IND\n"));
            connectionHandleAuthenticationTimeout(&theCm->smState);
            break;

        case CL_INTERNAL_SM_REGISTER_REQ:
            PRINT(("CL_INTERNAL_SM_REGISTER_REQ\n"));
            handleRegisterReq((const CL_INTERNAL_SM_REGISTER_REQ_T*)message);
            break;

        case CL_INTERNAL_SM_UNREGISTER_REQ:
            PRINT(("CL_INTERNAL_SM_UNREGISTER_REQ\n"));
            handleUnRegisterReq((const CL_INTERNAL_SM_UNREGISTER_REQ_T*)message);
            break;

        case CL_INTERNAL_SM_REGISTER_OUTGOING_REQ:
            PRINT(("CL_INTERNAL_SM_REGISTER_OUTGOING_REQ\n"));
            handleRegisterOutgoingReq((const CL_INTERNAL_SM_REGISTER_OUTGOING_REQ_T*)message);
            break;

        case CL_INTERNAL_SM_UNREGISTER_OUTGOING_REQ:
            PRINT(("CL_INTERNAL_SM_UNREGISTER_OUTGOING_REQ\n"));
            handleUnRegisterOutgoingReq((const CL_INTERNAL_SM_UNREGISTER_OUTGOING_REQ_T*)message);
            break;

        case CL_INTERNAL_SM_ENCRYPT_REQ:
            PRINT(("CL_INTERNAL_SM_ENCRYPT_REQ\n"));
            handleEncryptReq(&theCm->smState, (const CL_INTERNAL_SM_ENCRYPT_REQ_T *)message);
            break;

        case CL_INTERNAL_SM_ENCRYPTION_KEY_REFRESH_REQ:
            PRINT(("CL_INTERNAL_SM_ENCRYPTION_KEY_REFRESH_REQ\n"));
            handleEncryptionKeyRefreshReq(&theCm->infoState, (const CL_INTERNAL_SM_ENCRYPTION_KEY_REFRESH_REQ_T *)message);
            break;

        case CL_INTERNAL_SM_PIN_REQUEST_RES:
            PRINT(("CL_INTERNAL_SM_PIN_REQUEST_RES\n"));
            handlePinRequestRes((const CL_INTERNAL_SM_PIN_REQUEST_RES_T*)message);
            break;

        case CL_INTERNAL_SM_IO_CAPABILITY_REQUEST_RES:
            PRINT(("CL_INTERNAL_SM_IO_CAPABILITY_REQUEST_RES\n"));
            handleIoCapabilityRequestRes(&theCm->smState, (const CL_INTERNAL_SM_IO_CAPABILITY_REQUEST_RES_T*)message);
            break;

        case CL_INTERNAL_SM_USER_CONFIRMATION_REQUEST_RES:
            PRINT(("CL_INTERNAL_SM_USER_CONFIRMATION_REQUEST_RES\n"));
            handleUserConfirmationRequestRes((const CL_INTERNAL_SM_USER_CONFIRMATION_REQUEST_RES_T*)message);
            break;

        case CL_INTERNAL_SM_USER_PASSKEY_REQUEST_RES:
            PRINT(("CL_INTERNAL_SM_USER_PASSKEY_REQUEST_RES\n"));
            handleUserPasskeyRequestRes((const CL_INTERNAL_SM_USER_PASSKEY_REQUEST_RES_T*)message);
            break;

        case CL_INTERNAL_SM_SEND_KEYPRESS_NOTIFICATION_REQ:
            PRINT(("CL_INTERNAL_SM_SEND_KEYPRESS_NOTIFICATION_REQ\n"));
            handleSendKeypressNotificationReq((const CL_INTERNAL_SM_SEND_KEYPRESS_NOTIFICATION_REQ_T*)message);
            break;

        case CL_INTERNAL_SM_SET_TRUST_LEVEL_REQ:
            PRINT(("CL_INTERNAL_SM_SET_TRUST_LEVEL_REQ\n"));
            handleSetTrustLevelReq(&theCm->smState, (const CL_INTERNAL_SM_SET_TRUST_LEVEL_REQ_T*)message);
            break;

        case CL_INTERNAL_SM_AUTH_REPLACE_IRK_REQ:
            PRINT(("CL_INTERNAL_SM_AUTH_REPLACE_IRK_REQ"));
            handleGetAuthReplaceIrk(&theCm->smState, (const CL_INTERNAL_SM_AUTH_REPLACE_IRK_REQ_T*)message);
            break;

        case CL_INTERNAL_SM_AUTHORISE_RES:
            PRINT(("CL_INTERNAL_SM_AUTHORISE_RES\n"));
            handleAuthoriseRes((const CL_INTERNAL_SM_AUTHORISE_RES_T*)message);
            break;

        case CL_INTERNAL_SM_ADD_AUTH_DEVICE_REQ:
            PRINT(("CL_INTERNAL_SM_ADD_AUTH_DEVICE_REQ\n"));
            handleAddAuthDeviceReq(&theCm->smState, (const CL_INTERNAL_SM_ADD_AUTH_DEVICE_REQ_T *)message);
            break;

        case CL_INTERNAL_SM_ADD_AUTH_DEVICE_RAW_REQ:
            PRINT(("CL_INTERNAL_SM_ADD_AUTH_DEVICE_RAW_REQ\n"));
            handleAddAuthDeviceRawReq(&theCm->smState, (const CL_INTERNAL_SM_ADD_AUTH_DEVICE_RAW_REQ_T *)message);
            break;

        case CL_INTERNAL_SM_GET_AUTH_DEVICE_REQ:
            PRINT(("CL_INTERNAL_SM_GET_AUTH_DEVICE_REQ\n"));
            handleGetAuthDeviceReq(&theCm->smState, (const CL_INTERNAL_SM_GET_AUTH_DEVICE_REQ_T *)message);
            break;

        case CL_INTERNAL_SM_GET_AUTH_DEVICE_RAW_REQ:
            PRINT(("CL_INTERNAL_SM_GET_AUTH_DEVICE_RAW_REQ\n"));
            handleGetAuthDeviceRawReq(&theCm->smState, (const CL_INTERNAL_SM_GET_AUTH_DEVICE_RAW_REQ_T *)message);
            break;

        case CL_INTERNAL_SM_SET_SC_MODE_REQ:
            PRINT(("CL_INTERNAL_SM_SET_SC_MODE_REQ\n"));
            handleSetSecurityModeReq(&theCm->smState, theCm->flags,  (const CL_INTERNAL_SM_SET_SC_MODE_REQ_T*)message);
            break;

        case CL_INTERNAL_DM_DUT_REQ:
            PRINT(("CL_INTERNAL_DM_DUT_REQ\n"));
            SET_CM_STATE(connectionTestMode);
            connectionHandleEnterDutModeReq(&theCm->infoState);
            break;

#ifndef DISABLE_BLE
        case CL_INTERNAL_DM_ULP_TRANSMITTER_TEST_REQ:
            PRINT(("CL_INTERNAL_DM_ULP_TRANSMITTER_TEST_REQ\n"));
            SET_CM_STATE(connectionTestMode);
            connectionHandleUlpTransmitterTestReq((const CL_INTERNAL_DM_ULP_TRANSMITTER_TEST_REQ_T*)message);
            break;

        case CL_INTERNAL_DM_ULP_RECEIVER_TEST_REQ:
            PRINT(("CL_INTERNAL_DM_ULP_RECEIVER_TEST_REQ\n"));
            SET_CM_STATE(connectionTestMode);
            connectionHandleUlpReceiverTestReq((const CL_INTERNAL_DM_ULP_RECEIVER_TEST_REQ_T*)message);
            break;

        case CL_INTERNAL_DM_ULP_TEST_END_REQ:
            PRINT(("CL_INTERNAL_DM_ULP_TEST_END_REQ\n"));
            connectionSendUlpTestEndCfmToClient(theCm->theAppTask, hci_error_command_disallowed, 0);
            break;

        case CL_INTERNAL_DM_BLE_SET_SCAN_ENABLE_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_SET_SCAN_ENABLE_REQ\n"));
            connectionHandleDmBleSetScanEnableReq(
                        &theCm->bleScanAdState,
                        (const CL_INTERNAL_DM_BLE_SET_SCAN_ENABLE_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_SET_ADVERTISE_ENABLE_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_SET_ADVERTISE_ENABLE_REQ\n"));
            connectionHandleDmBleSetAdvertiseEnableReq(
                        &theCm->bleScanAdState,
                        (const CL_INTERNAL_DM_BLE_SET_ADVERTISE_ENABLE_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_ULP_PERIODIC_ADV_ENABLE_REQ:
            PRINT(("DM_ULP_PERIODIC_ADV_ENABLE_REQ\n"));
            connectionHandleDmBlePeriodicAdvEnableReq(
                        &theCm->dmPerAdvState,
                        (const CL_INTERNAL_DM_ULP_PERIODIC_ADV_ENABLE_REQ_T*)message
                        );
            break;

        case CL_INTERNAL_DM_BLE_READ_RANDOM_ADDRESS_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_READ_RANDOM_ADDRESS_REQ\n"));
            connectionHandleDmBleReadRandomAddress(
                        &theCm->bleReadRdnAddrState,
                        (const CL_INTERNAL_DM_BLE_READ_RANDOM_ADDRESS_REQ_T *) message);
        break;

        case CL_INTERNAL_DM_BLE_GET_ADV_SCAN_CAPABILITIES_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_GET_ADV_SCAN_CAPABILITIES_REQ\n"));
            connectionHandleDmBleGetAdvScanCapabilitiesReq(
                        &theCm->dmExtAdvState,
                        (const CL_INTERNAL_DM_BLE_GET_ADV_SCAN_CAPABILITIES_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_EXT_SCAN_ENABLE_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_SCAN_ENABLE_REQ\n"));
            connectionHandleDmBleExtScanEnableReq(
                        &theCm->dmExtScanState,
                        (const CL_INTERNAL_DM_BLE_EXT_SCAN_ENABLE_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_EXT_ADVERTISE_ENABLE_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_ADVERTISE_ENABLE_REQ\n"));
            connectionHandleDmBleExtAdvertiseEnableReq(
                        &theCm->dmExtAdvState,
                        (const CL_INTERNAL_DM_BLE_EXT_ADVERTISE_ENABLE_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_EXT_ADV_SETS_INFO_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_ADV_SETS_INFO_REQ\n"));
            connectionHandleDmBleExtAdvSetsInfoReq(
                        &theCm->dmExtAdvState,
                        (const CL_INTERNAL_DM_BLE_EXT_ADV_SETS_INFO_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_EXT_ADV_REGISTER_APP_ADV_SET_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_ADV_REGISTER_APP_ADV_SET_REQ\n"));
            connectionHandleDmBleRegisterAppAdvSetReq(
                        &theCm->dmExtAdvState,
                        (const CL_INTERNAL_DM_BLE_EXT_ADV_REGISTER_APP_ADV_SET_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_EXT_ADV_UNREGISTER_APP_ADV_SET_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_ADV_UNREGISTER_APP_ADV_SET_REQ\n"));
            connectionHandleDmBleUnregisterAppAdvSetReq(
                        &theCm->dmExtAdvState,
                        (const CL_INTERNAL_DM_BLE_EXT_ADV_UNREGISTER_APP_ADV_SET_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_EXT_ADV_SET_PARAMS_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_ADV_SET_PARAMS_REQ\n"));
            connectionHandleDmBleExtAdvSetParamsReq(
                        &theCm->dmExtAdvState,
                        (const CL_INTERNAL_DM_BLE_EXT_ADV_SET_PARAMS_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_EXT_ADV_SET_RANDOM_ADDRESS_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_ADV_SET_RANDOM_ADDRESS_REQ\n"));
            connectionHandleDmBleExtAdvSetRandomAddressReq(
                        &theCm->dmExtAdvState,
                        (const CL_INTERNAL_DM_BLE_EXT_ADV_SET_RANDOM_ADDRESS_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_EXT_ADV_MULTI_ENABLE_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_ADV_MULTI_ENABLE_REQ\n"));
            connectionHandleDmBleSetAdvMultiEnableReq(
                        &theCm->dmExtAdvState,
                        (const CL_INTERNAL_DM_BLE_EXT_ADV_MULTI_ENABLE_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_EXT_ADV_SET_DATA_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_ADV_SET_DATA_REQ\n"));
            connectionHandleDmBleExtAdvSetDataReq(
                        &theCm->dmExtAdvState,
                        (const CL_INTERNAL_DM_BLE_EXT_ADV_SET_DATA_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_EXT_ADV_SET_SCAN_RESP_DATA_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_ADV_SET_SCAN_RESP_DATA_REQ\n"));
            connectionHandleDmBleExtAdvSetScanRespDataReq(
                        &theCm->dmExtAdvState,
                        (const CL_INTERNAL_DM_BLE_EXT_ADV_SET_SCAN_RESP_DATA_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_EXT_ADV_READ_MAX_ADV_DATA_LEN_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_ADV_READ_MAX_ADV_DATA_LEN_REQ\n"));
            connectionHandleDmBleExtAdvReadMaxAdvDataLenReq(
                        &theCm->dmExtAdvState,
                        (const CL_INTERNAL_DM_BLE_EXT_ADV_READ_MAX_ADV_DATA_LEN_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_PER_ADV_SET_PARAMS_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_PER_ADV_SET_PARAMS_REQ\n"));
            connectionHandleDmBlePerAdvSetParamsReq(
                        &theCm->dmPerAdvState,
                        (const CL_INTERNAL_DM_BLE_PER_ADV_SET_PARAMS_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_PER_ADV_SET_DATA_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_PER_ADV_SET_DATA_REQ\n"));
            connectionHandleDmBlePerAdvSetDataReq(
                        &theCm->dmPerAdvState,
                        (const CL_INTERNAL_DM_BLE_PER_ADV_SET_DATA_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_PER_ADV_START_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_PER_ADV_START_REQ\n"));
            connectionHandleDmBlePerAdvStartReq(
                        &theCm->dmPerAdvState,
                        (const CL_INTERNAL_DM_BLE_PER_ADV_START_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_PER_ADV_STOP_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_PER_ADV_STOP_REQ\n"));
            connectionHandleDmBlePerAdvStopReq(
                        &theCm->dmPerAdvState,
                        (const CL_INTERNAL_DM_BLE_PER_ADV_STOP_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_PER_ADV_SET_TRANSFER_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_PER_ADV_SET_TRANSFER_REQ\n"));
            connectionHandleDmBlePerAdvSetTransferReq(
                        &theCm->dmPerAdvState,
                        (const CL_INTERNAL_DM_BLE_PER_ADV_SET_TRANSFER_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_PER_ADV_READ_MAX_ADV_DATA_LEN_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_PER_ADV_READ_MAX_ADV_DATA_LEN_REQ\n"));
            connectionHandleDmBlePerAdvReadMaxAdvDataLenReq(
                        &theCm->dmPerAdvState,
                        (const CL_INTERNAL_DM_BLE_PER_ADV_READ_MAX_ADV_DATA_LEN_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_EXT_SCAN_GET_GLOBAL_PARAMS_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_SCAN_GET_GLOBAL_PARAMS_REQ\n"));
            connectionHandleDmBleExtScanGetGlobalParamsReq(
                        &theCm->dmExtScanState,
                        (const CL_INTERNAL_DM_BLE_EXT_SCAN_GET_GLOBAL_PARAMS_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_EXT_SCAN_SET_GLOBAL_PARAMS_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_SCAN_SET_GLOBAL_PARAMS_REQ\n"));
            connectionHandleDmBleExtScanSetParamsReq(
                        &theCm->dmExtScanState,
                        (const CL_INTERNAL_DM_BLE_EXT_SCAN_SET_GLOBAL_PARAMS_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_EXT_SCAN_REGISTER_SCANNER_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_SCAN_REGISTER_SCANNER_REQ\n"));
            connectionHandleDmBleExtScanRegisterScannerReq(
                        &theCm->dmExtScanState,
                        (const CL_INTERNAL_DM_BLE_EXT_SCAN_REGISTER_SCANNER_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_EXT_SCAN_UNREGISTER_SCANNER_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_SCAN_UNREGISTER_SCANNER_REQ\n"));
            connectionHandleDmBleExtScanUnregisterScannerReq(
                        &theCm->dmExtScanState,
                        (const CL_INTERNAL_DM_BLE_EXT_SCAN_UNREGISTER_SCANNER_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TO_TRAIN_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TO_TRAIN_REQ\n"));
            connectionHandleDmBlePeriodicScanSyncTrainReq(
                        &theCm->dmPerScanState,
                        (const CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TO_TRAIN_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TERMINATE_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TERMINATE_REQ\n"));
            connectionHandleDmBlePeriodicScanSyncTerminateReq(
                        &theCm->dmPerScanState,
                        (const CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TERMINATE_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TRANSFER_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TRANSFER_REQ\n"));
            connectionHandleDmBlePeriodicScanSyncTransferReq(
                        &theCm->dmPerScanState,
                        (const CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TRANSFER_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TRANSFER_PARAMS_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TRANSFER_PARAMS_REQ\n"));
            connectionHandleDmBlePeriodicScanSyncTransferParamsReq(
                        &theCm->dmPerScanState,
                        (const CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_TRANSFER_PARAMS_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_PERIODIC_SCAN_START_FIND_TRAINS_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_PERIODIC_SCAN_START_FIND_TRAINS_REQ\n"));
            connectionHandleDmBlePeriodicScanStartFindTrainsReq(
                        &theCm->dmPerScanState,
                        (const CL_INTERNAL_DM_BLE_PERIODIC_SCAN_START_FIND_TRAINS_REQ_T *) message
                        );
            break;

         case CL_INTERNAL_DM_BLE_PERIODIC_SCAN_STOP_FIND_TRAINS_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_PERIODIC_SCAN_STOP_FIND_TRAINS_REQ\n"));
            connectionHandleDmBlePeriodicScanStopFindTrainsReq(
                        &theCm->dmPerScanState,
                        (const CL_INTERNAL_DM_BLE_PERIODIC_SCAN_STOP_FIND_TRAINS_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_ADV_REPORT_ENABLE_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_ADV_REPORT_ENABLE_REQ\n"));
            connectionHandleDmBlePeriodicScanSyncAdvReportEnableReq(
                        &theCm->dmPerScanState,
                        (const CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_ADV_REPORT_ENABLE_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_ADV_REPORT_IND:
                PRINT(("CL_INTERNAL_DM_BLE_PERIODIC_SCAN_SYNC_ADV_REPORT_IND\n"));
                connectionHandleDmBlePeriodicScanSyncAdvReportInd(
                        (const DM_ULP_PERIODIC_SCAN_SYNC_ADV_REPORT_IND_T *)message
                        );
                return;

        case CL_INTERNAL_DM_BLE_PER_SCAN_ADV_REPORT_DONE_IND:
            PRINT(("CL_INTERNAL_DM_BLE_PER_SCAN_ADV_REPORT_DONE_IND\n"));
            connectionHandleDmBlePerScanAdvReportDoneInd(
                    (const CL_INTERNAL_DM_BLE_PER_SCAN_ADV_REPORT_DONE_IND_T *)message
                    );
            return;

        case CL_INTERNAL_DM_BLE_EXT_SCAN_FILTERED_ADV_REPORT_IND:
                PRINT(("CL_INTERNAL_DM_BLE_EXT_SCAN_FILTERED_ADV_REPORT_IND\n"));
                connectionHandleDmBleExtScanFilteredAdvReportInd(
                        (const DM_ULP_EXT_SCAN_FILTERED_ADV_REPORT_IND_T *)message
                        );
                return;

        case CL_INTERNAL_DM_BLE_EXT_SCAN_ADV_REPORT_DONE_IND:
            PRINT(("CL_INTERNAL_DM_BLE_EXT_SCAN_ADV_REPORT_DONE_IND\n"));
            connectionHandleDmBleExtScanFilteredAdvReportDoneInd(
                    (const CL_INTERNAL_DM_BLE_EXT_SCAN_ADV_REPORT_DONE_IND_T *)message
                    );
            return;

        case CL_INTERNAL_DM_BLE_SET_DATA_RELATED_ADDRESS_CHANGES_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_SET_DATA_RELATED_ADDRESS_CHANGES_REQ\n"));
            connectionHandleDmBleSetDataRelatedAddressChangesReq(
                        &theCm->dmExtAdvState,
                        (const CL_INTERNAL_DM_BLE_SET_DATA_RELATED_ADDRESS_CHANGES_REQ_T *) message
                        );
            break;

        case CL_INTERNAL_MESSAGE_MORE_DATA:
        {
            PRINT(("CL_INTERNAL_MESSAGE_MORE_DATA received.\n"));
            CL_INTERNAL_MESSAGE_MORE_DATA_T *msg = (CL_INTERNAL_MESSAGE_MORE_DATA_T *)message;
            connectionHandleMoreData(msg->source, msg->stream_type);
            break;
        }
        case CL_INTERNAL_DM_ULP_ENHANCED_READ_TRANSMIT_POWER_LEVEL_REQ:
        {
            PRINT(("CL_INTERNAL_DM_ULP_ENHANCED_READ_TRANSMIT_POWER_LEVEL_REQ\n"));
            connectionHandleUlpEnhancedReadTransmitPowerLevelReq(
                        &theCm->generalLockState,
                        (const CL_INTERNAL_DM_ULP_ENHANCED_READ_TRANSMIT_POWER_LEVEL_REQ_T *)message);
            break;
        }
       case CL_INTERNAL_DM_ULP_READ_REMOTE_TRANSMIT_POWER_LEVEL_REQ:
        {
            PRINT(("CL_INTERNAL_DM_ULP_READ_REMOTE_TRANSMIT_POWER_LEVEL_REQ\n"));
            connectionHandleUlpReadRemoteTransmitPowerLevelReq(
                        &theCm->generalLockState,
                        (const CL_INTERNAL_DM_ULP_READ_REMOTE_TRANSMIT_POWER_LEVEL_REQ_T *)message);
            break;
        }
        case CL_INTERNAL_DM_ULP_SET_PATH_LOSS_REPORTING_PARAMETERS_REQ:
        {
            PRINT(("CL_INTERNAL_DM_ULP_SET_PATH_LOSS_REPORTING_PARAMETERS_REQ\n"));
            connectionHandleUlpSetPathLossReportingParemetersReq(
                    &theCm->generalLockState,
                    (const CL_INTERNAL_DM_ULP_SET_PATH_LOSS_REPORTING_PARAMETERS_REQ_T *)message);
            break;
        }
        case CL_INTERNAL_DM_ULP_SET_PATH_LOSS_REPORTING_ENABLE_REQ:
        {
            PRINT(("CL_INTERNAL_DM_ULP_SET_PATH_LOSS_REPORTING_REQ\n"));
            connectionHandleUlpSetPathLossReportingReq(
                        &theCm->generalLockState,
                        (const CL_INTERNAL_DM_ULP_SET_PATH_LOSS_REPORTING_ENABLE_REQ_T *)message);
            break;
        }
        case CL_INTERNAL_DM_ULP_SET_TRANSMIT_POWER_REPORTING_ENABLE_REQ:
        {
            PRINT(("CL_INTERNAL_DM_ULP_SET_TRANSMIT_POWER_REPORTING_ENABLE_REQ"));
            connectionHandleUlpSetTransmitPowerReportingEnableReq(
                        &theCm->generalLockState,
                        (const CL_INTERNAL_DM_ULP_SET_TRANSMIT_POWER_REPORTING_ENABLE_REQ_T *)message);
            break;
        }


#endif /* DISABLE_BLE */

        case CL_INTERNAL_DM_SET_BT_VERSION_REQ:
            PRINT(("CL_INTERNAL_DM_SET_BT_VERSION_REQ\n"));
            connectionHandleSetBtVersionReq(&theCm->infoState, (const CL_INTERNAL_DM_SET_BT_VERSION_REQ_T*)message);
            break;

        case CL_INTERNAL_DM_READ_LOCAL_VERSION_REQ:
            PRINT(("CL_INTERNAL_DM_READ_LOCAL_VERSION_REQ\n"));
            connectionHandleReadLocalVersionRequest(&theCm->infoState, (const CL_INTERNAL_DM_READ_LOCAL_VERSION_REQ_T *) message);
            break;

        case CL_INTERNAL_DM_WRITE_INQUIRY_SCAN_TYPE_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_INQUIRY_SCAN_TYPE_REQ\n"));
            connectionHandleWriteInquiryScanTypeRequest(
                (const CL_INTERNAL_DM_WRITE_INQUIRY_SCAN_TYPE_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_WRITE_PAGE_SCAN_TYPE_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_PAGE_SCAN_TYPE_REQ\n"));
            connectionHandleWritePageScanTypeRequest(
                (const CL_INTERNAL_DM_WRITE_PAGE_SCAN_TYPE_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_WRITE_SCAN_ENABLE_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_SCAN_ENABLE_REQ\n"));
            connectionHandleWriteScanEnableRequest((const CL_INTERNAL_DM_WRITE_SCAN_ENABLE_REQ_T *)message);
            break;

#ifndef CL_EXCLUDE_INQUIRY
        case CL_INTERNAL_DM_INQUIRY_REQ:
            PRINT(("CL_INTERNAL_DM_INQUIRY_REQ\n"));
            connectionHandleInquiryStart(&theCm->inqState, (const CL_INTERNAL_DM_INQUIRY_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_INQUIRY_CANCEL_REQ:
            PRINT(("CL_INTERNAL_DM_INQUIRY_CANCEL_REQ\n"));
            connectionHandleInquiryCancel(&theCm->inqState, (const CL_INTERNAL_DM_INQUIRY_CANCEL_REQ_T *)message);
            break;
#endif

        case CL_INTERNAL_DM_READ_REMOTE_NAME_REQ:
            PRINT(("CL_INTERNAL_DM_READ_REMOTE_NAME_REQ\n"));
            connectionHandleReadRemoteName(&theCm->inqState,
                         (const CL_INTERNAL_DM_READ_REMOTE_NAME_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_READ_REMOTE_NAME_CANCEL_REQ:
            PRINT(("CL_INTERNAL_DM_READ_REMOTE_NAME_CANCEL_REQ\n"));
            connectionHandleReadRemoteNameCancel(&theCm->inqState,
                      (const CL_INTERNAL_DM_READ_REMOTE_NAME_CANCEL_REQ_T *)message);
            break;


        case CL_INTERNAL_DM_READ_LOCAL_NAME_REQ:
            PRINT(("CL_INTERNAL_DM_READ_LOCAL_NAME_REQ\n"));
            connectionHandleReadLocalName(&theCm->inqState, (const CL_INTERNAL_DM_READ_LOCAL_NAME_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_READ_AFH_CHANNEL_MAP_REQ:
            PRINT(("CL_INTERNAL_DM_READ_AFH_CHANNEL_MAP_REQ"));
            connectionHandleDmReadAfhChannelMapReq(&theCm->dmReadAfhMapState, (const CL_INTERNAL_DM_READ_AFH_CHANNEL_MAP_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_CHANGE_LOCAL_NAME_REQ:
            PRINT(("CL_INTERNAL_DM_CHANGE_LOCAL_NAME_REQ\n"));
            connectionHandleChangeLocalName((const CL_INTERNAL_DM_CHANGE_LOCAL_NAME_REQ_T *)message);
            break;

#ifndef CL_EXCLUDE_INQUIRY
        case CL_INTERNAL_DM_WRITE_INQUIRY_TX_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_INQUIRY_TX_REQ\n"));
            connectionHandleWriteInquiryTx(&theCm->infoState, (const CL_INTERNAL_DM_WRITE_INQUIRY_TX_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_READ_INQUIRY_TX_REQ:
            PRINT(("CL_INTERNAL_DM_READ_INQUIRY_TX_REQ\n"));
            connectionHandleReadInquiryTx(&theCm->infoState, &theCm->inqState, (const CL_INTERNAL_DM_READ_INQUIRY_TX_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_READ_TX_POWER_REQ:
             connectionHandleReadTxPower(&theCm->readTxPwrState,
                                (const CL_INTERNAL_DM_READ_TX_POWER_REQ_T *)message);
             break;

        case CL_INTERNAL_DM_WRITE_EIR_DATA_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_EIR_DATA_REQ\n"));
            connectionHandleWriteEirDataRequest(&theCm->infoState, (const CL_INTERNAL_DM_WRITE_EIR_DATA_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_READ_EIR_DATA_REQ:
            PRINT(("CL_INTERNAL_DM_READ_EIR_DATA_REQ\n"));
            connectionHandleReadEirDataRequest(&theCm->infoState, &theCm->inqState, (const CL_INTERNAL_DM_READ_EIR_DATA_REQ_T *)message);
            break;
#endif

        case CL_INTERNAL_DM_READ_CLASS_OF_DEVICE_REQ:
            PRINT(("CL_INTERNAL_DM_READ_CLASS_OF_DEVICE_REQ\n"));
            connectionHandleReadClassOfDeviceRequest(&theCm->infoState, (const CL_INTERNAL_DM_READ_CLASS_OF_DEVICE_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_WRITE_PAGESCAN_ACTIVITY_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_PAGESCAN_ACTIVITY_REQ\n"));
            connectionHandleWritePageScanActivityRequest((const CL_INTERNAL_DM_WRITE_PAGESCAN_ACTIVITY_REQ_T *)message);
            break;

#ifndef CL_EXCLUDE_INQUIRY
        case CL_INTERNAL_DM_WRITE_INQSCAN_ACTIVITY_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_INQSCAN_ACTIVITY_REQ\n"));
            connectionHandleWriteInquiryScanActivityRequest((const CL_INTERNAL_DM_WRITE_INQSCAN_ACTIVITY_REQ_T *)message);
            break;
#endif

        case CL_INTERNAL_DM_WRITE_CLASS_OF_DEVICE_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_CLASS_OF_DEVICE_REQ\n"));
            connectionHandleWriteCodRequest((const CL_INTERNAL_DM_WRITE_CLASS_OF_DEVICE_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_WRITE_CACHED_PAGE_MODE_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_CACHED_PAGE_MODE_REQ\n"));
            connectionHandleWriteCachedPageModeRequest((const CL_INTERNAL_DM_WRITE_CACHED_PAGE_MODE_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_WRITE_CACHED_CLK_OFFSET_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_CACHED_CLK_OFFSET_REQ\n"));
            connectionHandleWriteCachedClkOffsetRequest((const CL_INTERNAL_DM_WRITE_CACHED_CLK_OFFSET_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_CLEAR_PARAM_CACHE_REQ:
            PRINT(("CL_INTERNAL_DM_CLEAR_PARAM_CACHE_REQ\n"));
            connectionHandleClearParamCacheRequest((const CL_INTERNAL_DM_CLEAR_PARAM_CACHE_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_WRITE_FLUSH_TIMEOUT_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_FLUSH_TIMEOUT_REQ\n"));
            connectionHandleWriteFlushTimeoutRequest((const CL_INTERNAL_DM_WRITE_FLUSH_TIMEOUT_REQ_T *)message);
            break;

#ifndef CL_EXCLUDE_INQUIRY
        case CL_INTERNAL_DM_WRITE_IAC_LAP_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_IAC_LAP_REQ\n"));
            connectionHandleWriteIacLapRequest(&theCm->inqState, (const CL_INTERNAL_DM_WRITE_IAC_LAP_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_WRITE_INQUIRY_MODE_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_INQUIRY_MODE_REQ\n"));
            connectionHandleWriteInquiryModeRequest(&theCm->inqState, (const CL_INTERNAL_DM_WRITE_INQUIRY_MODE_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_READ_INQUIRY_MODE_REQ:
            PRINT(("CL_INTERNAL_DM_READ_INQUIRY_MODE_REQ\n"));
            connectionHandleReadInquiryModeRequest(&theCm->inqState, (const CL_INTERNAL_DM_READ_INQUIRY_MODE_REQ_T *)message);
            break;
#endif

        case CL_INTERNAL_DM_READ_BD_ADDR_REQ:
            PRINT(("CL_INTERNAL_DM_READ_BD_ADDR_REQ\n"));
            connectionHandleReadAddrRequest(&theCm->infoState, (const CL_INTERNAL_DM_READ_BD_ADDR_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_READ_LINK_QUALITY_REQ:
            PRINT(("CL_INTERNAL_DM_READ_LINK_QUALITY_REQ\n"));
            connectionHandleReadLinkQualityRequest(&theCm->infoState, (const CL_INTERNAL_DM_READ_LINK_QUALITY_REQ_T *)message);
            break;

        case CL_INTERNAL_DM_READ_RSSI_REQ:
            PRINT(("CL_INTERNAL_DM_READ_RSSI_REQ\n"));
            connectionHandleReadRssiRequest(&theCm->infoState, (const CL_INTERNAL_DM_READ_RSSI_REQ_T *) message);
            break;

        case CL_INTERNAL_DM_READ_CLK_OFFSET_REQ:
            PRINT(("CL_INTERNAL_DM_READ_CLK_OFFSET_REQ\n"));
            connectionHandleReadclkOffsetRequest(&theCm->infoState, (const CL_INTERNAL_DM_READ_CLK_OFFSET_REQ_T *) message);
            break;

        case CL_INTERNAL_DM_READ_REMOTE_SUPP_FEAT_REQ:
            PRINT(("CL_INTERNAL_DM_READ_REMOTE_SUPP_FEAT_REQ\n"));
            connectionHandleReadRemoteSupportedFeaturesRequest(&theCm->infoState, (const CL_INTERNAL_DM_READ_REMOTE_SUPP_FEAT_REQ_T *) message);
            break;

        case CL_INTERNAL_DM_READ_REMOTE_SUPP_EXT_FEAT_REQ:
            PRINT(("CL_INTERNAL_DM_READ_REMOTE_SUPP_EXT_FEAT_REQ\n"));
            connectionHandleReadRemoteSupportedExtFeaturesRequest(&theCm->infoState,
                   (CL_INTERNAL_DM_READ_REMOTE_SUPP_EXT_FEAT_REQ_T *) message);
            break;

        case CL_INTERNAL_DM_ACL_OPEN_REQ:
            PRINT(("CL_INTERNAL_DM_ACL_OPEN_REQ\n"));
            connectionDmHandleAclOpen((const CL_INTERNAL_DM_ACL_OPEN_REQ_T*)message);
            break;

        case CL_INTERNAL_DM_ACL_CLOSE_REQ:
            PRINT(("CL_INTERNAL_DM_ACL_CLOSE_REQ\n"));
            connectionDmHandleAclClose((const CL_INTERNAL_DM_ACL_CLOSE_REQ_T*)message);
            break;

        case CL_INTERNAL_DM_READ_REMOTE_VERSION_REQ:
             PRINT(("CL_INTERNAL_DM_READ_REMOTE_VERSION_REQ\n"));
             connectionHandleReadRemoteVersionRequest(&theCm->infoState, (const CL_INTERNAL_DM_READ_REMOTE_VERSION_REQ_T *) message);
             break;

        case CL_INTERNAL_SM_CHANGE_LINK_KEY_REQ:
             PRINT(("CL_INTERNAL_SM_CHANGE_LINK_KEY_REQ\n"));
            connectionHandleSmChangeLinkKeyReq((const CL_INTERNAL_SM_CHANGE_LINK_KEY_REQ_T*) message);
            break;

#ifndef CL_EXCLUDE_SDP
        case CL_INTERNAL_SDP_REGISTER_RECORD_REQ:
            PRINT(("CL_INTERNAL_SDP_REGISTER_RECORD_REQ\n"));
            connectionHandleSdpRegisterRequest(&theCm->sdpState, (const CL_INTERNAL_SDP_REGISTER_RECORD_REQ_T *)message);
            break;

        case CL_INTERNAL_SDP_UNREGISTER_RECORD_REQ:
            PRINT(("CL_INTERNAL_SDP_UNREGISTER_RECORD_REQ\n"));
            connectionHandleSdpUnregisterRequest(&theCm->sdpState, (const CL_INTERNAL_SDP_UNREGISTER_RECORD_REQ_T *)message);
            break;

        case CL_INTERNAL_SDP_CONFIG_SERVER_MTU_REQ:
            PRINT(("CL_INTERNAL_SDP_CONFIG_SERVER_MTU_REQ\n"));
            connectionHandleSdpServerConfigMtu(&theCm->sdpState, (const CL_INTERNAL_SDP_CONFIG_SERVER_MTU_REQ_T *)message);
            break;

        case CL_INTERNAL_SDP_CONFIG_CLIENT_MTU_REQ:
            PRINT(("CL_INTERNAL_SDP_CONFIG_CLIENT_MTU_REQ\n"));
            connectionHandleSdpClientConfigMtu(&theCm->sdpState, (const CL_INTERNAL_SDP_CONFIG_CLIENT_MTU_REQ_T *)message);
            break;

        case CL_INTERNAL_SDP_OPEN_SEARCH_REQ:
            PRINT(("CL_INTERNAL_SDP_OPEN_SEARCH_REQ\n"));
            connectionHandleSdpOpenSearchRequest(&theCm->sdpState, (const CL_INTERNAL_SDP_OPEN_SEARCH_REQ_T *)message);
            break;

        case CL_INTERNAL_SDP_CLOSE_SEARCH_REQ:
            PRINT(("CL_INTERNAL_SDP_CLOSE_SEARCH_REQ\n"));
            connectionHandleSdpCloseSearchRequest(&theCm->sdpState, (const CL_INTERNAL_SDP_CLOSE_SEARCH_REQ_T *)message);
            break;

        case CL_INTERNAL_SDP_SERVICE_SEARCH_REQ:
            PRINT(("CL_INTERNAL_SDP_SERVICE_SEARCH_REQ\n"));
            connectionHandleSdpServiceSearchRequest(&theCm->sdpState, (const CL_INTERNAL_SDP_SERVICE_SEARCH_REQ_T *)message);
            break;

        case CL_INTERNAL_SDP_ATTRIBUTE_SEARCH_REQ:
            PRINT(("CL_INTERNAL_SDP_ATTRIBUTE_SEARCH_REQ\n"));
            connectionHandleSdpAttributeSearchRequest(&theCm->sdpState, (const CL_INTERNAL_SDP_ATTRIBUTE_SEARCH_REQ_T *)message);
            break;

        case CL_INTERNAL_SDP_SERVICE_SEARCH_ATTRIBUTE_REQ:
            PRINT(("CL_INTERNAL_SDP_SERVICE_SEARCH_ATTRIBUTE_REQ\n"));
            connectionHandleSdpServiceSearchAttrRequest(&theCm->sdpState, (const CL_INTERNAL_SDP_SERVICE_SEARCH_ATTRIBUTE_REQ_T *)message);
            break;

        case CL_INTERNAL_SDP_TERMINATE_PRIMITIVE_REQ:
            PRINT(("CL_INTERNAL_SDP_TERMINATE_PRIMITIVE_REQ\n"));
            connectionHandleSdpTerminatePrimitiveRequest(&theCm->sdpState, (const CL_INTERNAL_SDP_TERMINATE_PRIMITIVE_REQ_T *)message);
            break;
#endif

#ifndef CL_EXCLUDE_L2CAP
        case CL_INTERNAL_L2CAP_REGISTER_REQ:
            PRINT(("CL_INTERNAL_L2CAP_REGISTER_REQ\n"));
            connectionHandleL2capRegisterReq((const CL_INTERNAL_L2CAP_REGISTER_REQ_T *)message);
            break;

        case CL_INTERNAL_L2CAP_UNREGISTER_REQ:
            PRINT(("CL_INTERNAL_L2CAP_UNREGISTER_REQ\n"));
            connectionHandleL2capUnregisterReq((const CL_INTERNAL_L2CAP_UNREGISTER_REQ_T *)message);
            break;

        case CL_INTERNAL_L2CAP_CONNECT_REQ:
            PRINT(("CL_INTERNAL_L2CAP_CONNECT_REQ\n"));
            connectionHandleL2capConnectReq((const CL_INTERNAL_L2CAP_CONNECT_REQ_T *)message);
            break;

        case CL_INTERNAL_L2CAP_TP_CONNECT_REQ:
            PRINT(("CL_INTERNAL_L2CAP_TP_CONNECT_REQ\n"));
            connectionHandleL2capTpConnectReq((const CL_INTERNAL_L2CAP_TP_CONNECT_REQ_T *)message);
            break;

        case CL_INTERNAL_L2CAP_ADD_CREDIT_REQ:
            PRINT(("CL_INTERNAL_L2CAP_ADD_CREDIT_REQ\n"));
            connectionHandleL2capAddCreditReq((const CL_INTERNAL_L2CAP_ADD_CREDIT_REQ_T *)message);
            break;

        case CL_INTERNAL_L2CAP_CONNECT_RES:
            PRINT(("CL_INTERNAL_L2CAP_CONNECT_RES\n"));
            connectionHandleL2capConnectRes((const CL_INTERNAL_L2CAP_CONNECT_RES_T *) message);
            break;

        case CL_INTERNAL_L2CAP_TP_CONNECT_RES:
            PRINT(("CL_INTERNAL_L2CAP_TP_CONNECT_RES\n"));
            connectionHandleL2capTpConnectRes((const CL_INTERNAL_L2CAP_TP_CONNECT_RES_T *) message);
            break;

        case CL_INTERNAL_L2CAP_MAP_CONNECTIONLESS_REQ:
            PRINT(("CL_INTERNAL_L2CAP_MAP_CONNECTIONLESS_REQ\n"));
            connectionHandleL2capMapConnectionlessReq(&theCm->l2capState, (const CL_INTERNAL_L2CAP_MAP_CONNECTIONLESS_REQ_T *) message);
            break;

        case CL_INTERNAL_L2CAP_MAP_CONNECTIONLESS_RES:
            PRINT(("CL_INTERNAL_L2CAP_MAP_CONNECTIONLESS_RES\n"));
            connectionHandleL2capMapConnectionlessRsp((const CL_INTERNAL_L2CAP_MAP_CONNECTIONLESS_RES_T *) message);
            break;

        case CL_INTERNAL_L2CAP_UNMAP_CONNECTIONLESS_REQ:
            PRINT(("CL_INTERNAL_L2CAP_UNMAP_CONNECTIONLESS_REQ\n"));
            connectionHandleL2capUnmapConnectionlessReq((const CL_INTERNAL_L2CAP_UNMAP_CONNECTIONLESS_REQ_T *)message);
            break;

        case CL_INTERNAL_L2CAP_DISCONNECT_REQ:
            PRINT(("CL_INTERNAL_L2CAP_DISCONNECT_REQ\n"));
            connectionHandleL2capDisconnectReq((const CL_INTERNAL_L2CAP_DISCONNECT_REQ_T *) message);
            break;

        case CL_INTERNAL_L2CAP_DISCONNECT_RSP:
            PRINT(("CL_INTERNAL_L2CAP_DISCONNECT_RSP"));
            connectionHandleL2capDisconnectRes((const CL_INTERNAL_L2CAP_DISCONNECT_RSP_T*)message);
            break;

#endif /* !CL_EXCLUDE_L2CAP */

#ifndef CL_EXCLUDE_RFCOMM
        case CL_INTERNAL_RFCOMM_REGISTER_REQ:
            PRINT(("CL_INTERNAL_RFCOMM_REGISTER_REQ\n"));
            connectionHandleRfcommRegisterReq((const CL_INTERNAL_RFCOMM_REGISTER_REQ_T *)message);
            break;

        case CL_INTERNAL_RFCOMM_UNREGISTER_REQ:
            PRINT(("CL_INTERNAL_UNRFCOMM_REGISTER_REQ\n"));
            connectionHandleRfcommUnregisterReq((const CL_INTERNAL_RFCOMM_UNREGISTER_REQ_T *)message);
            break;

        case CL_INTERNAL_RFCOMM_CONNECT_REQ:
            PRINT(("CL_INTERNAL_RFCOMM_CONNECT_REQ\n"));
            connectionHandleRfcommConnectReq((const CL_INTERNAL_RFCOMM_CONNECT_REQ_T *)message);
            break;

        case CL_INTERNAL_RFCOMM_CONNECT_RES:
            PRINT(("CL_INTERNAL_RFCOMM_CONNECT_RES\n"));
            connectionHandleRfcommConnectRes((const CL_INTERNAL_RFCOMM_CONNECT_RES_T *)message);
            break;

        case CL_INTERNAL_RFCOMM_DISCONNECT_REQ:
            PRINT(("CL_INTERNAL_RFCOMM_DISCONNECT_REQ\n"));
            connectionHandleRfcommDisconnectReq((const CL_INTERNAL_RFCOMM_DISCONNECT_REQ_T *)message);
            break;

        case CL_INTERNAL_RFCOMM_DISCONNECT_RSP:
            PRINT(("CL_INTERNAL_RFCOMM_DISCONNECT_RSP\n"));
            connectionHandleRfcommDisconnectRsp((const CL_INTERNAL_RFCOMM_DISCONNECT_RSP_T *)message);
            break;

        case CL_INTERNAL_RFCOMM_PORTNEG_REQ:
            PRINT(("CL_INTERNAL_RFCOMM_PORTNEG_REQ"));
            connectionHandleRfcommPortNegReq((const CL_INTERNAL_RFCOMM_PORTNEG_REQ_T *)message);
            break;

        case CL_INTERNAL_RFCOMM_PORTNEG_RSP:
            PRINT(("CL_INTERNAL_RFCOMM_PORT_NEG_RSP"));
            connectionHandleRfcommPortNegRsp((const CL_INTERNAL_RFCOMM_PORTNEG_RSP_T *)message);
            break;

        case CL_INTERNAL_RFCOMM_CONTROL_REQ:
            PRINT(("CL_INTERNAL_RFCOMM_CONTROL_REQ\n"));
            connectionHandleRfcommControlReq((const CL_INTERNAL_RFCOMM_CONTROL_REQ_T *)message);
            break;

        case CL_INTERNAL_RFCOMM_LINE_STATUS_REQ:
            PRINT(("CL_INTERNAL_RFCOMM_LINE_STATUS_REQ\n"));
            connectionHandleRfcommLineStatusReq((const CL_INTERNAL_RFCOMM_LINE_STATUS_REQ_T*)message);
            break;
#endif

#ifndef CL_EXCLUDE_SYNC
        case CL_INTERNAL_SYNC_REGISTER_REQ:
            PRINT(("CL_INTERNAL_SYNC_REGISTER_REQ\n"));
            connectionHandleSyncRegisterReq((const CL_INTERNAL_SYNC_REGISTER_REQ_T *) message);
            break;

        case CL_INTERNAL_SYNC_UNREGISTER_REQ:
            PRINT(("CL_INTERNAL_SYNC_UNREGISTER_REQ\n"));
            connectionHandleSyncUnregisterReq((const CL_INTERNAL_SYNC_UNREGISTER_REQ_T *) message);
            break;

        case CL_INTERNAL_SYNC_CONNECT_REQ:
            PRINT(("CL_INTERNAL_SYNC_CONNECT_REQ\n"));
            connectionHandleSyncConnectReq((const CL_INTERNAL_SYNC_CONNECT_REQ_T *) message);
            break;

        case CL_INTERNAL_SYNC_CONNECT_RES:
            PRINT(("CL_INTERNAL_SYNC_CONNECT_RES\n"));
            connectionHandleSyncConnectRes((const CL_INTERNAL_SYNC_CONNECT_RES_T *) message);
            break;

        case CL_INTERNAL_SYNC_DISCONNECT_REQ:
            PRINT(("CL_INTERNAL_SYNC_DISCONNECT_REQ\n"));
            connectionHandleSyncDisconnectReq((const CL_INTERNAL_SYNC_DISCONNECT_REQ_T *) message);
            break;

        case CL_INTERNAL_SYNC_RENEGOTIATE_REQ:
            PRINT(("CL_INTERNAL_SYNC_RENEGOTIATE_REQ\n"));
            connectionHandleSyncRenegotiateReq((const CL_INTERNAL_SYNC_RENEGOTIATE_REQ_T *) message);
            break;

        case CL_INTERNAL_SYNC_REGISTER_TIMEOUT_IND:
            PRINT(("CL_INTERNAL_SYNC_REGISTER_TIMEOUT_IND\n"));
            connectionHandleSyncRegisterTimeoutInd((const CL_INTERNAL_SYNC_REGISTER_TIMEOUT_IND_T *) message);
            break;

        case CL_INTERNAL_SYNC_UNREGISTER_TIMEOUT_IND:
            PRINT(("CL_INTERNAL_SYNC_UNREGISTER_TIMEOUT_IND\n"));
            connectionHandleSyncUnregisterTimeoutInd((const CL_INTERNAL_SYNC_UNREGISTER_TIMEOUT_IND_T *) message);
            break;
#endif

#ifndef CL_EXCLUDE_ISOC
        case CL_INTERNAL_ISOC_REGISTER_REQ:
            PRINT(("CL_INTERNAL_ISOC_REGISTER_REQ\n"));
            connectionHandleIsocRegisterReq(&theCm->dmIsocState, (const CL_INTERNAL_ISOC_REGISTER_REQ_T *) message);
            break;

        case CL_INTERNAL_ISOC_CIS_CONNECT_REQ:
            PRINT(("CL_INTERNAL_ISOC_CIS_CONNECT_REQ\n"));
            connectionHandleIsocConnectReq(&theCm->dmIsocState, (const CL_INTERNAL_ISOC_CIS_CONNECT_REQ_T *) message);
            break;

        case CL_INTERNAL_ISOC_CIS_CONNECT_RES:
            PRINT(("CL_INTERNAL_ISOC_CIS_CONNECT_RES\n"));
            connectionHandleIsocConnectRes(&theCm->dmIsocState, (const CL_INTERNAL_ISOC_CIS_CONNECT_RES_T *) message);
            break;

        case CL_INTERNAL_ISOC_CIS_DISCONNECT_REQ:
            PRINT(("CL_INTERNAL_ISOC_CIS_DISCONNECT_REQ\n"));
            connectionHandleIsocDisconnectReq(&theCm->dmIsocState, (const CL_INTERNAL_ISOC_CIS_DISCONNECT_REQ_T *) message);
            break;

        case CL_INTERNAL_ISOC_CONFIGURE_CIG_REQ:
            PRINT(("CL_INTERNAL_ISOC_CONFIGURE_CIG_REQ\n"));
            connectionHandleIsocConfigureCigReq(&theCm->dmIsocState, (const CL_INTERNAL_ISOC_CONFIGURE_CIG_REQ_T *) message);
            break;

        case CL_INTERNAL_ISOC_REMOVE_CIG_REQ:
            PRINT(("CL_INTERNAL_ISOC_REMOVE_CIG_REQ\n"));
            connectionHandleIsocRemoveCigReq(&theCm->dmIsocState, (const CL_INTERNAL_ISOC_REMOVE_CIG_REQ_T *) message);
            break;

        case CL_INTERNAL_ISOC_SETUP_ISOCHRONOUS_DATA_PATH_REQ:
            PRINT(("CL_INTERNAL_ISOC_SETUP_ISOCHRONOUS_DATA_PATH_REQ\n"));
            connectionHandleIsocSetupDataPathReq(&theCm->dmIsocState, (const CL_INTERNAL_ISOC_SETUP_ISOCHRONOUS_DATA_PATH_REQ_T *) message);
            break;

        case CL_INTERNAL_ISOC_REMOVE_ISO_DATA_PATH_REQ:
            PRINT(("CL_INTERNAL_ISOC_REMOVE_ISO_DATA_PATH_REQ\n"));
            connectionHandleIsocRemoveDataPathReq(&theCm->dmIsocState, (const CL_INTERNAL_ISOC_REMOVE_ISO_DATA_PATH_REQ_T *) message);
            break;

        case CL_INTERNAL_ISOC_CREATE_BIG_REQ:
            PRINT(("CL_INTERNAL_ISOC_CREATE_BIG_REQ\n"));
            connectionHandleIsocCreateBigReq(&theCm->dmIsocState, (const CL_INTERNAL_ISOC_CREATE_BIG_REQ_T *) message);
            break;

        case CL_INTERNAL_ISOC_TERMINATE_BIG_REQ:
            PRINT(("CL_INTERNAL_ISOC_TERMINATE_BIG_REQ\n"));
            connectionHandleIsocTerminateBigReq(&theCm->dmIsocState, (const CL_INTERNAL_ISOC_TERMINATE_BIG_REQ_T *) message);
            break;

        case CL_INTERNAL_ISOC_BIG_CREATE_SYNC_REQ:
            PRINT(("CL_INTERNAL_ISOC_BIG_CREATE_SYNC_REQ\n"));
            connectionHandleIsocBigCreateSyncReq(&theCm->dmIsocState, (const CL_INTERNAL_ISOC_BIG_CREATE_SYNC_REQ_T *) message);
            break;
#endif

        case CL_INTERNAL_DM_SET_ROLE_REQ:
            PRINT(("CL_INTERNAL_DM_SET_ROLE_REQ\n"));
            connectionHandleLinkPolicySetRoleReq(&theCm->linkPolicyState, (const CL_INTERNAL_DM_SET_ROLE_REQ_T *) message);
            break;

        case CL_INTERNAL_DM_GET_ROLE_REQ:
            PRINT(("CL_INTERNAL_DM_GET_ROLE_REQ\n"));
            connectionHandleLinkPolicyGetRoleReq(&theCm->linkPolicyState, (const CL_INTERNAL_DM_GET_ROLE_REQ_T *) message);
            break;

        case CL_INTERNAL_DM_SET_LINK_SUPERVISION_TIMEOUT_REQ:
            PRINT(("CL_INTERNAL_DM_SET_LINK_SUPERVISION_TIMEOUT_REQ\n"));
            connectionHandleSetLinkSupervisionTimeoutReq((const CL_INTERNAL_DM_SET_LINK_SUPERVISION_TIMEOUT_REQ_T *) message);
            break;

        case CL_INTERNAL_DM_SET_LINK_POLICY_REQ:
            PRINT(("CL_INTERNAL_DM_SET_LINK_POLICY_REQ\n"));
            connectionHandleSetLinkPolicyReq((const CL_INTERNAL_DM_SET_LINK_POLICY_REQ_T *) message);
            break;

        case CL_INTERNAL_DM_WRITE_LINK_POLICY_SETTINGS_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_LINK_POLICY_SETTINGS_REQ\n"));
            connectionHandleWriteLinkPolicySettingsReq((const CL_INTERNAL_DM_WRITE_LINK_POLICY_SETTINGS_REQ_T *) message);
            break;

        case CL_INTERNAL_DM_SET_SNIFF_SUB_RATE_POLICY_REQ:
            PRINT(("CL_INTERNAL_DM_SET_SNIFF_SUB_RATE_POLICY_REQ\n"));
            connectionHandleSetSniffSubRatePolicyReq(&theCm->infoState, (const CL_INTERNAL_DM_SET_SNIFF_SUB_RATE_POLICY_REQ_T *) message);
            break;

        case CL_INTERNAL_DM_WRITE_PAGE_TIMEOUT_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_PAGE_TIMEOUT_REQ\n"));
            connectionHandleWritePageTimeout((const CL_INTERNAL_DM_WRITE_PAGE_TIMEOUT_REQ_T *)message);
        break;

        case CL_INTERNAL_SM_GET_ATTRIBUTE_REQ:
            PRINT(("CL_INTERNAL_SM_GET_ATTRIBUTE_REQ\n"));
            connectionSmHandleGetAttributeReq(theCm->theAppTask, (const CL_INTERNAL_SM_GET_ATTRIBUTE_REQ_T *) message);
            break;

        case CL_INTERNAL_SM_GET_INDEXED_ATTRIBUTE_REQ:
            PRINT(("CL_INTERNAL_SM_GET_INDEXED_ATTRIBUTE_REQ\n"));
            connectionSmHandleGetIndexedAttributeReq(theCm->theAppTask, (const CL_INTERNAL_SM_GET_INDEXED_ATTRIBUTE_REQ_T *) message);
            break;

        case CL_INTERNAL_DM_READ_APT_REQ:
            PRINT(("CL_INTERNAL_DM_READ_APT_REQ\n"));
            connectionHandleReadAPTReq((const CL_INTERNAL_DM_READ_APT_REQ_T *) message);
            break;

        case CL_INTERNAL_DM_WRITE_APT_REQ:
            PRINT(("CL_INTERNAL_DM_WRITE_APT_REQ\n"));
            connectionHandleWriteAPTReq((const CL_INTERNAL_DM_WRITE_APT_REQ_T *) message);
            break;

#ifndef DISABLE_BLE
        case CL_INTERNAL_SM_DM_SECURITY_REQ:
            PRINT(("CL_INTERNAL_SM_DM_SECURITY_REQ\n"));
            connectionHandleInternalBleDmSecurityReq(
                (const CL_INTERNAL_SM_DM_SECURITY_REQ_T *)message
                );
            break;

        case CL_INTERNAL_DM_BLE_CONNECTION_PARAMETERS_UPDATE_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_CONNECTION_PARAMETERS_UPDATE_REQ\n"));
            connectionHandleDmBleConnectionParametersUpdateReq(
                &theCm->l2capState,
                (const CL_INTERNAL_DM_BLE_CONNECTION_PARAMETERS_UPDATE_REQ_T*)message);
            break;

        case CL_INTERNAL_DM_BLE_READ_AD_CHAN_TX_PWR_REQ:
            PRINT(("CL_INTERNAL_DM_BLE_READ_AD_CHAN_TX_PWR_REQ\n"));
            connectionHandleBleReadAdChanTxPwr(
                    &theCm->readTxPwrState,
                    (const CL_INTERNAL_DM_BLE_READ_AD_CHAN_TX_PWR_REQ_T*)message
                    );
            break;
#endif /* !DISABLE_BLE */

        case CL_INTERNAL_DM_SC_OVERRIDE_REQ:
            PRINT(("CL_INTERNAL_DM_SC_OVERRIDE_REQ\n"));
            connectionHandleDmScOverrideReq(
                        &theCm->dmScOverrideState,
                        (const CL_INTERNAL_DM_SC_OVERRIDE_REQ_T*)message
                        );
            break;

        case CL_INTERNAL_DM_SC_OVERRIDE_MAX_BDADDR_REQ:
            PRINT(("CL_INTERNAL_DM_SC_OVERRIDE_MAX_BDADDR_REQ\n"));
            connectionHandleDmScOverrideMaxBdaddrReq(
                        &theCm->dmScOverrideState,
                        (const CL_INTERNAL_DM_SC_OVERRIDE_MAX_BDADDR_REQ_T*)message
                        );
            break;

        case CL_INTERNAL_DM_SET_LINK_BEHAVIOR_REQ:
            PRINT(("CL_INTERNAL_DM_SET_LINK_BEHAVIOR\n"));
            connectionHandleDmSetLinkBehaviorReq(
                        &theCm->generalLockState,
                        (const CL_INTERNAL_DM_SET_LINK_BEHAVIOR_REQ_T*)message
                        );

            break;

        default:
            /* Prims we are not handling - for now panic the app */
            handleUnexpected(connectionUnhandledMessage, theCm->state, id);
    }
}

/****************************************************************************
NAME
    connectionBluestackHandler

DESCRIPTION
    This is the main task handler for all messages sent to the Connection
    Library task.

RETURNS
    void
*/
void connectionBluestackHandler(Task task, MessageId id, Message message)
{
    /* Get access to the Connection Library instance state */
    connectionState *theCm = (connectionState *)task;
    connectionStates state = theCm->state;

    PRINT(("connectionBluestackHandler - Id = MESSAGE:0x%x\n", id));

    /* Handle Bluestack primitives seperately */
    switch (id)
    {
        case MESSAGE_BLUESTACK_DM_PRIM:
            connectionBluestackHandlerDm(theCm, (const DM_UPRIM_T *)message);
            break;

#ifndef CL_EXCLUDE_RFCOMM
        case MESSAGE_BLUESTACK_RFCOMM_PRIM:
            connectionBluestackHandlerRfcomm(theCm, (const RFCOMM_UPRIM_T *)message);
            break;
#endif

#if !defined(CL_EXCLUDE_L2CAP) || !defined(DISABLE_BLE)
        case MESSAGE_BLUESTACK_L2CAP_PRIM:
            connectionBluestackHandlerL2cap(theCm, (const L2CA_UPRIM_T *)message);
            break;
#endif

#ifndef CL_EXCLUDE_SDP
        case MESSAGE_BLUESTACK_SDP_PRIM:
            connectionBluestackHandlerSdp(theCm, (const SDS_UPRIM_T *)message);
            break;
#endif

        case MESSAGE_BLUESTACK_UDP_PRIM:
        case MESSAGE_BLUESTACK_TCP_PRIM:
            handleUnexpected(connectionUnhandledMessage, theCm->state, id);
            break;

#ifndef CL_EXCLUDE_SDP
        /* CL_SDP_CLOSE_SEARCH_CFM Primitive arrived as a result of an internal
           call to close SDP search, can't avoid so ignore
           Handled as a special case to allow the compiler to generate better
           code for the previous switch statements. */
        case CL_SDP_CLOSE_SEARCH_CFM:
            break;
#endif

#ifndef DISABLE_BLE
        case MESSAGE_MORE_DATA:
            PRINT(("MESSAGE_MORE_DATA received.\n"));
            Source src = ((MessageMoreData*)message)->source;
            connectionHandleMoreData(src, unidentified_stream);
            return;
#endif

        /* Everything else must be internal connection library primitives */
        default:
        {
            switch (state)
            {
                case connectionReady:
                    connectionBluestackHandlerReady(theCm, id, message);
                    break;

                case connectionUninitialised:
                    connectionBluestackHandlerUninitialised(theCm, id, message);
                    break;

                case connectionInitialising:
                    connectionBluestackHandlerInitialising(theCm, id, message);
                    break;

                case connectionTestMode:
                    connectionBluestackHandlerTestMode(theCm, id, message);
                    break;
            }
        }
    }
}

/*lint +e655 +e525 +e830 */
