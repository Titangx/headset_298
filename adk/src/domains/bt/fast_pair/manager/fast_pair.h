/*!
\copyright  Copyright (c) 2008-2022 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    
\file       fast_pair.h
\defgroup   fast_pair_domain    Fast Pair
\ingroup    bt_domain
\brief      Header file for the Fast Pair task
*/

#ifndef FAST_PAIR_H_
#define FAST_PAIR_H_

#include <connection.h>
#include <connection_manager.h>
#include <pairing.h>
#include <task_list.h>
#include <logging.h>
#include <panic.h>
#include <stdlib.h>
#include <util.h>
#include <byte_utils.h>

#include "domain_message.h"

/*! @{ */

#define PASSKEY_INVALID         0xFF000000UL


#define MAX_FAST_PAIR_ACCOUNT_KEYS              (5)

#define FAST_PAIR_ENCRYPTED_REQUEST_LEN         (16)
#define FAST_PAIR_ENCRYPTED_RESPONSE_LEN         (16)
#define FAST_PAIR_ENCRYPTED_PASSKEY_BLOCK_LEN   (16)
#define FAST_PAIR_ENCRYPTED_ACCOUNT_KEY_LEN     (16)

#define FAST_PAIR_PUBLIC_KEY_LEN                (64)
#define FAST_PAIR_PRIVATE_KEY_LEN               (32)
#define FAST_PAIR_ACCOUNT_KEY_LEN               (16)
#define FAST_PAIR_AES_KEY_LEN                   (16)
#define FAST_PAIR_AES_BLOCK_SIZE                (16)


/* Personalized Name maximum length used for storage */
#define FAST_PAIR_PNAME_DATA_LEN                (64)
/* Personalized Name string size. This is the first byte in the storage.
   Then the FAST_PAIR_PNAME_DATA_LEN bytes follow */
#define FAST_PAIR_PNAME_SIZE_LEN                    (1)
/* Personalized name length used for storage in uint8 */
#define FAST_PAIR_PNAME_STORAGE_LEN             (FAST_PAIR_PNAME_DATA_LEN+FAST_PAIR_PNAME_SIZE_LEN)

#define FAST_PAIR_PROVIDER_ADDRESS_OFFSET       (2)
#define FAST_PAIR_SEEKER_ADDRESS_OFFSET         (8)


/* FP state timeout in seconds*/
#define FAST_PAIR_STATE_TIMEOUT                 (10)
/* FP Seeker triggered discoverability timeout in seconds */
#define FAST_PAIR_DISCOVERABILITY_TIMEOUT       (10)
/* Quarantine timeout in seconds*/
#define FAST_PAIR_QUARANTINE_TIMEOUT            (300)
/* Maximum bumber of Failure attempts */
#define FAST_PAIR_MAX_FAIL_ATTEMPTS             (10)
/* Maximum BLE connections allowed */
#define MAX_BLE_CONNECTIONS    (2)
/* Retroactively Writing Account Key timeout in seconds */
#define FAST_PAIR_RETROACTIVELY_WRITING_ACCOUNT_KEY_TIMEOUT  (60)
/* Timeout to stop ringing the device when handset link gets timed out */
#define FAST_PAIR_STOP_RING_TIMEOUT             (30)

#define FAST_PAIR_ADD_DATA_PACKET_NONCE_INDEX 8
#define FAST_PAIR_ADD_DATA_PACKET_DATA_INDEX  16
#define FAST_PAIR_ADD_DATA_WITH_PNAME_ENCRYPTED_PKT_LEN (FAST_PAIR_ADD_DATA_PACKET_DATA_INDEX+FAST_PAIR_PNAME_DATA_LEN)

/*!
    @brief Exposes Fast Pair interface for handover.
*/
extern const handover_interface fast_pair_handover_if;

/*! \brief Type of passkey owners */
typedef enum
{
    fast_pair_passkey_seeker = 2,
    fast_pair_passkey_provider = 3
} fast_pair_passkey_owner_t;

/*! \brief Type of events passed to fast pair state machine */
typedef enum
{
     fast_pair_state_event_kbp_write = 0,              /*! Key based charactersitic write occured */
     fast_pair_state_event_passkey_write,              /*! Passkey charactersitic write occured */
     fast_pair_state_event_account_key_write,          /*! Account key charactersitic write occured */
     fast_pair_state_event_pairing_request,            /*! BR/EDR Pairing request recevived */
     fast_pair_state_event_provider_passkey,           /*! Provider Passkey recieved */
     fast_pair_state_event_additional_data_write,      /*! Additional Data write recieved */
     fast_pair_state_event_pname_write,                /*! Personalized name charactersitic write occured */
     fast_pair_state_event_connect,                    /*! ACL connection occured */
     fast_pair_state_event_disconnect,                 /*! ACL disconnection occured */
     fast_pair_state_event_handset_connect_allow,      /*! handset connection allowed */
     fast_pair_state_event_handset_connect_disallow,   /*! handset connection disallowed*/
     fast_pair_state_event_rpa_addr,                   /*! Read RPA Addr confirmation */
     fast_pair_state_event_crypto_shared_secret,       /*! Crypto ECDH Shared Secret confirmation */
     fast_pair_state_event_crypto_hash,                /*! Crypto Hash confirmation */
     fast_pair_state_event_crypto_encrypt,             /*! Crypto AES Encrypt confirmation */
     fast_pair_state_event_crypto_decrypt,             /*! Crypto AES Decrypt confirmation */
     fast_pair_state_event_crypto_aes_ctr,             /*! Crypto AES CTR confirmation */
     fast_pair_state_event_timer_expire,               /*! FP Procedure Timer expired */
     fast_pair_state_event_power_off,                  /*! Power Off event by user */
     fast_pair_state_event_provider_initiate_pairing,  /*! Pairing event initiated by Provider */
     fast_pair_state_event_auth,                       /*! CL_SM_AUTHENTICATE_CFM received from application */
} fast_pair_state_event_id;


/*! \brief Event structure that is used to inject an event and any corresponding
    arguments into the state machine. */
typedef struct
{
    fast_pair_state_event_id id;
    void *args;
} fast_pair_state_event_t;

/*! \brief Event arguments for SM authentication event */
typedef struct
{
    CL_SM_AUTHENTICATE_CFM_T* auth_cfm;
} fast_pair_state_event_auth_args_t;

/*! \brief Fast Pair module state machine states */
typedef enum fast_pair_states
{
    FAST_PAIR_STATE_NULL,            /*!< Startup state */
    FAST_PAIR_STATE_IDLE,            /*!< No fast pairing happening */

    FAST_PAIR_STATE_WAIT_AES_KEY,    /*!< Processing Key Based Pairing Write */
    FAST_PAIR_STATE_WAIT_PAIRING_REQUEST,   /*!< Waiting for FP Seeker to send pairing request */

    FAST_PAIR_STATE_WAIT_PASSKEY,    /*!< Waiting for FP Seeker to write passkey */
    FAST_PAIR_STATE_WAIT_ACCOUNT_KEY,/*!< Wait for Account Key write by FP Seeker */

    FAST_PAIR_STATE_WAIT_ADDITIONAL_DATA, /*!< Wait for Additional data */

    FAST_PAIR_STATE_PNAME  /*!< Personalized name state */
} fastPairState;



/*! \brief Stucture used to to store account keys and
    iterate through the list to calculate the right 'K'
 */
typedef struct
{
    uint8 num_keys;
    uint8 num_keys_processed;
    uint8* keys;
} fast_pair_account_keys_t;

/*! \brief Fast Pair Session Data structure */
typedef struct
{
    /*! Place holder for ASPK */
    uint16* private_key;
    /*! Place holder for public key provided by FP Seeker */
    uint16* public_key;
    /*! Place holder for encrypted data written by FP Seeker */
    uint16* encrypted_data;
    /*! Place holder for calculated AES key */
    uint16* aes_key;
    /*! Data ID value in KBP action request */
    uint8 kbp_action_request_data_id;
    /*! Personalized Name requested by seeker in KBP request */
    uint8 kbp_pname_request_flag;
    /*! Cached account keys read from PS */
    fast_pair_account_keys_t   account_key;

} fast_pair_session_data_t;

/*! \brief Fast Pair Pname Additional Data structure */
typedef struct
{
    /*! Encrypted additional data PName packet */
    uint8 encr_add_data_pname[FAST_PAIR_ADD_DATA_WITH_PNAME_ENCRYPTED_PKT_LEN];
    /*! Nonce */
    uint8 *nonce;
    /*! Additional data lenth */
    uint8 add_data_length;
    /*! Personalized name lenth */
    uint8 pname_length;
    /*! Place holder for encrypted/decrypted PName data */
    uint8 *data;
    /*! Encrypted/Decrypted PName data size */
    uint8 data_size;
} fast_pair_pname_add_data_t;

/*! \brief Fast Pair task structure */
typedef struct
{
    /*! The fast pairing module task */
    TaskData task;
    /*! The current fast pairing state */
    fastPairState state;
    /*! previous pairing state */
    fastPairState prev_state;
    /*! RPA address used to make LE connection with FP Seeker, 0 otherwise */
    bdaddr   rpa_bd_addr;
    /*! Failure Counter */
    uint16  failure_count;
    /*! Session Data. Will be cleaned when moving to Idle */
    fast_pair_session_data_t session_data;
    /*! Peer BD address used to make ACL connection */
    bdaddr peer_bd_addr[MAX_BLE_CONNECTIONS];
    /*! Random address used to make ACL connection */
    bdaddr own_random_address[MAX_BLE_CONNECTIONS];
    /*! FP Seeker's BD address */
    bdaddr seeker_addr;
    /*! Flag to identify pairing initiator, FALSE if seeker; TRUE if provider*/
    bool provider_pairing_requested;
    /*! Flag to identify retroactively writing account key, 
    True if account key write can happen retroactively, FALSE otherwise */
    bool retroactively_writing_account_key;
    /*! Personalized Name Data */
    fast_pair_pname_add_data_t pname_add_data;
    /*! Handset Public Address */
    bdaddr handset_bd_addr;
} fastPairTaskData;


/*! Initialise the fast pair application module.

    This function is called to initilaze fast pairing module.
    If a message is processed then the function returns TRUE.

    \param  init_task       Initialization Task

    \returns TRUE if successfully processed
 */
bool FastPair_Init(Task init_task);

/*! Message Handler to handle CL message coming from application

    \param   id        Identifier of the message
    \param  message        The message content
    \param  already_handled        boolean variable to check if message is already handled or not

    \returns  already_handled
 */
bool FastPair_HandleConnectionLibraryMessages(MessageId id, Message message, bool already_handled);

/*! Handler for all messages to fast pair Module

    This function is called to handle any messages sent to the fast pairing module.
    If a message is processed then the function returns TRUE.

    \note Some connection library messages can be sent directly as the
        request is able to specify a destination for the response.

    \param  task            Task to which this message was sent
    \param  id              Identifier of the message
    \param  message         The message content (if any)

    \returns None
 */
void FastPair_HandleMessage(Task task, MessageId id, Message message);


/*! \brief Initialize the Fast Pair Session Data Module

    \param None

    \return None
 */
void FastPair_RegisterPersistentDeviceDataUser(void);

/*! Handler for all events sent to fast pair state machine

    This function is called to handle any event sent to
    the FP state machine. If a message is processed then the function returns TRUE.

    \param  event           Identifier of the event

    \returns TRUE if the event has been processed, otherwise returns the
        value in already_handled
 */
bool fastPair_StateMachineHandleEvent(fast_pair_state_event_t event);

/*! Set state of Fast Pair procedure

    This function is called set the fast pair state.

    \param  theFastPair     Fast Pair Data context

    \param  state           Fast pair state
 */
void fastPair_SetState(fastPairTaskData *theFastPair, fastPairState state);

/*! Get state of Fast Pair procedure

    This function is called fetch the fast pair state.

    \param  theFastPair     Fast Pair Data context

    \returns fast pair state
 */
fastPairState fastPair_GetState(fastPairTaskData *theFastPair);

/*! Get pointer to Fast Pair data structure

    This function is called fetch the fast pair context.

    \param  None

    \returns fast pair data context
 */
fastPairTaskData* fastPair_GetTaskData(void);

/*! Start 10 seconds Timer for next step in Fast Pair procedure

    This function is Starts timer.

    \param  isQuarantine    If TRUE then 5 miinutes timer is set.
                            Else 10 seconds timer is started

 */
void fastPair_StartTimer(bool isQuarantine);

/*! Stop Timer and continue Fast Pair procedure

    This function is Stops timer.

 */
void fastPair_StopTimer(void);

/*! \brief Account Key Synchronization API
    This is the API which triggers the synchronization between the peers.
 */
void AccountKeySync_Sync(void);

/*! \brief Fast Pair Delete Account Keys API
 */
void FastPair_DeleteAccountKeys(void);

/*! \brief Set the retroactively writing account key flag to the given value.

    \param  flag_value  Flag value to set.

    \return None

 */
void fastPair_SetRetroactivelyWritingAccountKeyFlag(bool flag_value);

/*! \brief Start 60 seconds timer for retroactively writing account key

    \param  None

    \return None

 */
void fastPair_StartTimerForWritingAccountKeyRetroactively(void);

/*! \brief Helper function to convert the endianness format of the input array.

    \param  input_array Pointer to the input array.
    \param  array_size Size of input array.
    \param  output_array Pointer to the output array.

    \return None

 */
void fastPair_ConvertEndiannessFormat(uint8 *input_array, uint8 array_size, uint8 *output_array);

/*! \brief Fast Pair PName Synchronization API
    This is the API which triggers the synchronization between the peers.
 */
void FastPair_PNameSync_Sync(void);


/*! \brief Delete the Fast Pair Personalized Name
 */
void FastPair_DeletePName(void);

/*! \brief Is idle */
#define fastPair_IsIdle() \
    (fastPair_GetTaskData()->state == FAST_PAIR_STATE_IDLE)

/*! @} */

#endif /* FAST_PAIR_H_ */
