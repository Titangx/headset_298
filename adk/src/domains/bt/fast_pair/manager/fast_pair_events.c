/*!
\copyright  Copyright (c) 2008 - 2021 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    
\file       fast_pair_events.c
\brief      Fast Pair Events
*/

#include "fast_pair_events.h"
#include "fast_pair_advertising.h"


/*! \brief Handle messages from Connection Library */
bool fastPair_AuthenticateCfm(CL_SM_AUTHENTICATE_CFM_T *cfm)
{
    fast_pair_state_event_auth_args_t args;
    fast_pair_state_event_t event;

    /* Assign args to SM Auth event */
    args.auth_cfm = cfm;

    event.id = fast_pair_state_event_auth;
    event.args = &args;

    return fastPair_StateMachineHandleEvent(event);
}

/*! \brief Handle messages from Connection Manager */
bool fastPair_ConManagerConnectInd(CON_MANAGER_TP_CONNECT_IND_T *cfm)
{
    fast_pair_state_event_connect_args_t args;
    fast_pair_state_event_t event;
    
    /* Assign args to the BLE connect/disconnect event */
    args.connect_ind = cfm;
    
    event.id = fast_pair_state_event_connect;
    event.args = &args;
    
    return fastPair_StateMachineHandleEvent(event);
}


/*! \brief Handle messages from Connection Manager */
bool fastPair_ConManagerDisconnectInd(CON_MANAGER_TP_DISCONNECT_IND_T *cfm)
{
    fast_pair_state_event_disconnect_args_t args;
    fast_pair_state_event_t event;
    
    /* Assign args to the BLE connect/disconnect event */
    args.disconnect_ind = cfm;
    
    event.id = fast_pair_state_event_disconnect;
    event.args = &args;
    
    return fastPair_StateMachineHandleEvent(event);
}

/*! \brief Handle messages from Connection Manager */
bool fastPair_ConManagerHandsetConnectAllowInd(void)
{
    fast_pair_state_event_t event = {fast_pair_state_event_handset_connect_allow, NULL};

    return fastPair_StateMachineHandleEvent(event);
}

/*! \brief Handle messages from Conenction Manager */
bool fastPair_ConManagerHandsetConnectDisallowInd(void)
{
    fast_pair_state_event_t event = {fast_pair_state_event_handset_connect_disallow, NULL};

    return fastPair_StateMachineHandleEvent(event);
}

/*! \brief Handle messages from Conenction Message Dispatcher */
bool fastPair_CacheRandomAddressCfm(const CL_SM_BLE_READ_RANDOM_ADDRESS_CFM_T *cfm)
{
    fast_pair_state_event_rpa_addr_args_t args;
    fast_pair_state_event_t event;

    /* Assign args to the RPA Address event */
    args.rpa_cfm = cfm;
    
    event.id = fast_pair_state_event_rpa_addr;
    event.args = &args;

    return fastPair_StateMachineHandleEvent(event);
}

/*! \brief Handle Crypto ECDH Shared Secret confirm */
bool fastPair_SharedSecretCfm(CL_CRYPTO_GENERATE_SHARED_SECRET_KEY_CFM_T *cfm)
{
    fast_pair_state_event_crypto_shared_secret_args_t args;
    fast_pair_state_event_t event;
    
    /* Assign args to the ECDH Shared Secret event */
    args.crypto_shared_secret_cfm = cfm;
    
    event.id = fast_pair_state_event_crypto_shared_secret;
    event.args = &args;
    
    return fastPair_StateMachineHandleEvent(event);
}

/*! \brief Handle Crypto Hash confirm */
bool fastPair_HashCfm(CL_CRYPTO_HASH_CFM_T *cfm)
{
    fast_pair_state_event_crypto_hash_args_t args;
    fast_pair_state_event_t event;
    
    /* Assign args to the Hash256 event */
    args.crypto_hash_cfm = cfm;
    
    event.id = fast_pair_state_event_crypto_hash;
    event.args = &args;
    
    return fastPair_StateMachineHandleEvent(event);
    
}

/*! \brief Handle Crypto AES Encrypt confirm */
bool fastPair_EncryptCfm(CL_CRYPTO_ENCRYPT_CFM_T *cfm)
{
    fast_pair_state_event_crypto_encrypt_args_t args;
    fast_pair_state_event_t event;
    
    /* Assign args to the AES Encrypt event */
    args.crypto_encrypt_cfm = cfm;
    
    event.id = fast_pair_state_event_crypto_encrypt;
    event.args = &args;
    
    return fastPair_StateMachineHandleEvent(event);
}

/*! \brief Handle Crypto AES Encrypt confirm */
bool fastPair_DecryptCfm(CL_CRYPTO_DECRYPT_CFM_T *cfm)
{
    fast_pair_state_event_crypto_decrypt_args_t args;
    fast_pair_state_event_t event;
    
    /* Assign args to the AES Encrypt event */
    args.crypto_decrypt_cfm = cfm;
    
    event.id = fast_pair_state_event_crypto_decrypt;
    event.args = &args;
    
    return fastPair_StateMachineHandleEvent(event);
}

/*! \brief Handle Crypto AES CTR Encrypt/Decrypt confirm */
bool fastPair_AesCtrCfm(CL_CRYPTO_ENCRYPT_DECRYPT_AES_CTR_CFM_T *cfm)
{
    fast_pair_state_event_crypto_aes_ctr_args_t args;
    fast_pair_state_event_t event;
    
    /* Assign args to the AES CTR event */
    args.crypto_aes_ctr_cfm = cfm;
    
    event.id = fast_pair_state_event_crypto_aes_ctr;
    event.args = &args;
    
    return fastPair_StateMachineHandleEvent(event);
}

/*! \brief Handle Key Based Pairing Characterstic Write by Fast Pair Seeker */
void fastPair_KeyBasedPairingWrite(const gatt_cid_t cid, const uint8 *enc_data, uint32 enc_data_len)
{
    fast_pair_state_event_kbp_write_args_t args;
    fast_pair_state_event_t event;
    
    /* Assign args to the KbP write event */
    args.cid = cid;
    args.enc_data = enc_data;
    args.size = enc_data_len;
    
    event.id = fast_pair_state_event_kbp_write;
    event.args = &args;
    
    fastPair_StateMachineHandleEvent(event);
}

/*! \brief Handle Passkey Characterstic Write by Fast Pair Seeker */
void fastPair_PasskeyWrite(const gatt_cid_t cid, const uint8 *enc_data, uint32 enc_data_len)
{
    fast_pair_state_event_passkey_write_args_t args;
    fast_pair_state_event_t event;
    
    /* Assign args to the passkey write event */
    args.cid = cid;
    args.enc_data = enc_data;
    args.size = enc_data_len;
    
    event.id = fast_pair_state_event_passkey_write;
    event.args = &args;
    
    fastPair_StateMachineHandleEvent(event);
}

/*! \brief Handle Account Key Characterstic Write by Fast Pair Seeker */
void fastPair_AccountkeyWrite(const gatt_cid_t cid, const uint8 *enc_data, uint32 enc_data_len)
{
    fast_pair_state_event_account_key_write_args_t args;
    fast_pair_state_event_t event;

    /* Assign args to the account key write event */
    args.cid = cid;
    args.enc_data = enc_data;
    args.size = enc_data_len;

    event.id = fast_pair_state_event_account_key_write;
    event.args = &args;
    
    fastPair_StateMachineHandleEvent(event);
}


/*! \brief Handle Additional Data (ex:Name) write */
void fastPair_AdditionalDataWrite(const uint8 *enc_data, uint32 enc_data_len)
{
    fast_pair_state_event_additional_data_write_args_t args;
    fast_pair_state_event_t event;

    /* Assign args to the data write event */
    args.enc_data = enc_data;
    args.size = enc_data_len;

    event.id = fast_pair_state_event_additional_data_write;
    event.args = &args;

    fastPair_StateMachineHandleEvent(event);
}

/*! Store Name to persistent storage */
void fastPair_PNameWrite(const uint8 *pname_data, uint32 pname_data_len)
{
    fast_pair_state_event_pname_write_args_t args;
    fast_pair_state_event_t event;

    /* Assign args to the data write event */
    args.pname_data = pname_data;
    args.size = pname_data_len;

    event.id = fast_pair_state_event_pname_write;
    event.args = &args;

    fastPair_StateMachineHandleEvent(event);
}


/*! \brief Handle New Pairing Request */
void fastPair_ReceivedPairingRequest(bool accept)
{
    fast_pair_state_event_t event;

    event.id = fast_pair_state_event_pairing_request;
    event.args = &accept;
    
    fastPair_StateMachineHandleEvent(event);
}

/*! \brief Handle Provider Passkey Generated Event*/
void fastPair_ProviderPasskey(bool isPasskeySame, uint32 provider_passkey, bdaddr handset_bdaddr)
{
    fast_pair_state_event_provider_passkey_write_args_t args;
    fast_pair_state_event_t event;

    args.isPasskeySame = isPasskeySame;
    args.provider_passkey = provider_passkey;
    args.handset_bdaddr = handset_bdaddr;
    event.id = fast_pair_state_event_provider_passkey;
    event.args = &args;
    
    fastPair_StateMachineHandleEvent(event);
}


/*! \brief Handle FP Timeout Event */
bool fastPair_TimerExpired(void)
{
    fast_pair_state_event_t event = {fast_pair_state_event_timer_expire, NULL};
    
    return fastPair_StateMachineHandleEvent(event);
}

/*! \brief Handle Power Off Event */
bool fastPair_PowerOff(void)
{
    fast_pair_state_event_t event = {fast_pair_state_event_power_off, NULL};
    
    return fastPair_StateMachineHandleEvent(event);
}

/*! \brief Handle the handset connection allow/disallow event from connection manager */
bool fastPair_HandsetConnectStatusChange(fast_pair_state_event_id event_id)
{
    bool status = FALSE;

    switch(event_id)
    {
        case fast_pair_state_event_handset_connect_allow:
        {
            status = fastPair_AdvNotifyChangeInConnectableState(TRUE);
        }
        break;

        case fast_pair_state_event_handset_connect_disallow:
        {
            status = fastPair_AdvNotifyChangeInConnectableState(FALSE);
        }
        break;

        default:
            DEBUG_LOG("FP Events:  Invalid event id in fastPair_HandsetConnectStatusChange ");
        break;
    }

    return status;
}

/*! \brief Handle the Pairing activity messages from pairing module */
void fastPair_PairingActivity(PAIRING_ACTIVITY_T *message)
{
    fastPairTaskData *theFastPair;
    theFastPair = fastPair_GetTaskData();

    switch(message->status)
    {
        case pairingActivityInProgress:
        {
            fastPair_AdvNotifyChangeInIdentifiable(TRUE);
        }
        break;

        case pairingActivitySuccess:
        {
            /* Store the handset public address in FP Data structure only if we are doing FP Process
               This address can be used later to associate the account key with this handset 
               In case of RWA procedure, this thing is taken care while matching seeker address.*/
            if(fastPair_GetState(theFastPair) == FAST_PAIR_STATE_WAIT_PASSKEY ||
               fastPair_GetState(theFastPair) == FAST_PAIR_STATE_WAIT_ACCOUNT_KEY)
            {
                memcpy(&theFastPair->handset_bd_addr, &(message->device_addr), sizeof(bdaddr));
            }

            /* Set retroactively writing account key flag only if we have not done FP process */
            if(fastPair_GetState(theFastPair) == FAST_PAIR_STATE_IDLE && (theFastPair->prev_state != FAST_PAIR_STATE_WAIT_PASSKEY))
            {
                /* Set the retroactively writing account key flag and start 60 seconds timer now */
                fastPair_SetRetroactivelyWritingAccountKeyFlag(TRUE);
                fastPair_StartTimerForWritingAccountKeyRetroactively();
            }
        }
        /* fall-through */
        case pairingActivityNotInProgress:
        {
            fastPair_AdvNotifyChangeInIdentifiable(FALSE);
        }
        break;

        default:
            DEBUG_LOG("FP Events:  Invalid message id in fastPair_PairingActivity ");
        break;
    }

}

/*! \brief Handle the provider initiated pairing event */
void fastPair_ProviderInitiatePairing(void)
{
    fast_pair_state_event_t event = {fast_pair_state_event_provider_initiate_pairing, NULL};

    fastPair_StateMachineHandleEvent(event);
}

