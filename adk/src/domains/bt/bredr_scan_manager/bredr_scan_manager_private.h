/*!
\copyright  Copyright (c) 2019-2020 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    
\file
\brief      Private interface to module managing inquiry and page scanning.
*/

#ifndef BREDR_SCAN_MANAGER_PRIVATE_H_
#define BREDR_SCAN_MANAGER_PRIVATE_H_

#include "bredr_scan_manager.h"
#include "bredr_scan_manager_protected.h"
#include "task_list.h"
#include "connection_abstraction.h"
#include "logging.h"
#include "bandwidth_manager.h"

#include <message.h>
#include <panic.h>
#include <hydra_macros.h>

/* Insist of the correct ordering of page/inquiry scan messages */
COMPILE_TIME_ASSERT(BREDR_SCAN_MANAGER_PAGE_SCAN_PAUSED_IND + INQ_SCAN_MESSAGE_OFFSET == BREDR_SCAN_MANAGER_INQUIRY_SCAN_PAUSED_IND, SCAN_PAUSED_IND_invalid_message_order);
COMPILE_TIME_ASSERT(BREDR_SCAN_MANAGER_PAGE_SCAN_RESUMED_IND + INQ_SCAN_MESSAGE_OFFSET == BREDR_SCAN_MANAGER_INQUIRY_SCAN_RESUMED_IND, SCAN_RESUMED_IND_invalid_message_order);

/*! @brief Truncated page scan states */
typedef enum
{
    bredr_truncated_scan_state_initialised = 0,
    bredr_truncated_scan_state_disabled = 0x10,
    bredr_truncated_scan_state_enabled = 0x20,
    bredr_truncated_scan_state_suspended = 0x40,
} bredr_truncated_scan_state_t;

/*! Scan states, one-hot encoded */
typedef enum bsm_scan_states
{
    /*! Scanning is disabled */
    BSM_SCAN_DISABLED  = 0x01,
    /*! Scanning is being enabled */
    BSM_SCAN_ENABLING  = 0x02,
    /*! Scanning is being disabled */
    BSM_SCAN_DISABLING = 0x04,
    /*! Scanning is enabled */
    BSM_SCAN_ENABLED   = 0x08,

} bsm_scan_enable_state_t;

/*! Scan context, for both page and inquiry scanning */
typedef struct
{
    /*! Configured operating parameter set */
    const bredr_scan_manager_parameters_t *params;

    /*! List of clients */
    task_list_with_data_t clients;

    /*! Active scan parameters. These are stored to allow changes in scan
        parameters to be detected. */
    bredr_scan_manager_scan_parameters_t scan_params;

    /*! The current scan state */
    bsm_scan_enable_state_t state;

    /*! Active index in array of params */
    uint8 params_index;

    /*! Adding this offset to a message type base id, results in the scan type
        specific message */
    uint8 message_offset;

    /* Active scanning type that requested by client*/
    bredr_scan_manager_scan_type_t type;

    /*! Lock to indicate page/inquiry scan is disabled and is not in a transitional state */
    uint16 lock;

} bsm_scan_context_t;

/*! @brief Scan Manager state. */
typedef struct
{
    /*! Module's task */
    TaskData task_data;

    /*! Page scan context */
    bsm_scan_context_t page_scan;

    /*! Inquiry scan context */
    bsm_scan_context_t inquiry_scan;

    /*! Truncated page scan context */
    bsm_scan_context_t truncated_page_scan;

    /*! The task that requested the general disable (if any). Cleared once disable successful. */
    Task disable_task;

    /*! The task that requested to pause truncated page scan (if any). Cleared once truncated page scan is paused. */
    Task tps_pause_task;

    /*! Pointer to callback function to call when eir setup is complete */
    eir_setup_complete_callback_t eir_setup_complete_callback;

    /*! Indicates EIR setup is in progress */
    bool eir_setup_in_progress;

} bredr_scan_manager_state_t;

extern bredr_scan_manager_state_t bredr_scan_manager;

#define bredrScanManager_GetTaskData() (&bredr_scan_manager)

/*! \brief Get pointer to page scan context.
    \return The context.
*/
static inline bsm_scan_context_t *bredrScanManager_PageScanContext(void)
{
    return &bredr_scan_manager.page_scan;
}

/*! \brief Get pointer to inquiry scan context.
    \return The context.
*/
static inline bsm_scan_context_t *bredrScanManager_InquiryScanContext(void)
{
    return &bredr_scan_manager.inquiry_scan;
}

/*! \brief Get pointer to truncated page scan context.
    \return The context.
*/
static inline bsm_scan_context_t *bredrScanManager_TruncatedPageScanContext(void)
{
    return &bredr_scan_manager.truncated_page_scan;
}

/*! \brief Set the disable task.

    \param disabler The new disable task.

    \return The old disable task.
*/
static inline Task bredrScanManager_SetDisableTask(Task disabler)
{
    Task old = bredr_scan_manager.disable_task;
    bredr_scan_manager.disable_task = disabler;
    return old;
}

/*! \brief Get the disable task.

    \return The disable task.
*/
static inline Task bredrScanManager_GetDisableTask(void)
{
    return bredr_scan_manager.disable_task;
}

/*! \brief Determine if scanning is disabled.

    \return TRUE if disabled.
*/
static inline bool bredrScanManager_IsDisabled(void)
{
    return (bredr_scan_manager.disable_task != NULL);
}

/*! \brief Send disable confirm message to the disable client.
*/
static inline void bredrScanManager_SendDisableCfm(bool disabled)
{
    MESSAGE_MAKE(cfm, BREDR_SCAN_MANAGER_DISABLE_CFM_T);
    cfm->disabled = disabled;
    MessageSend(bredrScanManager_GetDisableTask(), BREDR_SCAN_MANAGER_DISABLE_CFM, cfm);
}

/*! \brief Set the task requesting the pause of the truncated page scan.

    \param disabler The new task requesting the pause of the truncated page scan.

    \return The old task.
*/
static inline Task bredrScanManager_SetTpsPauseTask(Task task_requesting_pause)
{
    Task old = bredr_scan_manager.tps_pause_task;
    bredr_scan_manager.tps_pause_task = task_requesting_pause;
    return old;
}

/*! \brief Get the task requesting the pause of the truncated page scan.

    \return The task requesting the pause of the truncated page scan.
*/
static inline Task bredrScanManager_GetTpsPauseTask(void)
{
    return bredr_scan_manager.tps_pause_task;
}

/*! \brief Determine if truncated page scanning is paused.

    \return TRUE if disabled.
*/
static inline bool bredrScanManager_IsTpsPauseRequested(void)
{
    return (bredr_scan_manager.tps_pause_task != NULL);
}

/*! \brief Send disable confirm message to the task requesting the pause of the truncated page scan.
*/
static inline void bredrScanManager_SendTpsPauseCfm(bool paused)
{
    MESSAGE_MAKE(cfm, BREDR_SCAN_MANAGER_TPS_PAUSED_CFM_T);
    cfm->paused = paused;
    MessageSend(bredrScanManager_GetTpsPauseTask(), BREDR_SCAN_MANAGER_TPS_PAUSED_CFM, cfm);
}

/*! \brief Initialise the scanner instance.
    \param context The scan context
    \param message_offset The offset between pairs of page scan and inquiry scan
    message ids in the enum bredr_scan_manager_messages.
*/
void bredrScanManager_InstanceInit(bsm_scan_context_t *context, uint8 message_offset);

/*! \brief Register a scan parameter set.
    \param context The scan context.
    \param params The scan params.
*/
void bredrScanManager_InstanceParameterSetRegister(bsm_scan_context_t *context,
                                                   const bredr_scan_manager_parameters_t *params);

/*! \brief Select the user of a scan parameter set.
    \param context The scan context.
    \param index Index in the registered scan parameter set.
*/
void bredrScanManager_InstanceParameterSetSelect(bsm_scan_context_t *context, uint8 index);

/*! \brief Add or update a client to a the scan instance.
    \param context The scan context (either page or inquiry scan context).
    \param client The client to add/update.
    \param type The client's scan type.
*/
void bredrScanManager_InstanceClientAddOrUpdate(bsm_scan_context_t *context, Task client,
                                                bredr_scan_manager_scan_type_t type);

/*! \brief Remove a client from the client list.
    \param context The scan context (either page or inquiry scan context).
    \param client The client to remove.
*/
void bredrScanManager_InstanceClientRemove(bsm_scan_context_t *context, Task client);

/*! \brief Inform scan context the firmware has completed its transition and is
           now in the last requested scan state.
    \param context The scan context (either page or inquiry scan context).
*/
void bredrScanManager_InstanceCompleteTransition(bsm_scan_context_t *context);

/*! \brief Query if the client has enabled scanning.
    \param context The scan context (either page or inquiry scan context).
    \param client The client to query.
*/
bool bredrScanManager_InstanceIsScanEnabledForClient(bsm_scan_context_t *context, Task client);

/*! \brief Pause scan
    \param context The scan context (either page or inquiry scan context).
*/
void bredrScanManager_InstancePause(bsm_scan_context_t *context);

/*! \brief Resume scan
    \param context The scan context (either page or inquiry scan context).
*/
void bredrScanManager_InstanceResume(bsm_scan_context_t *context);

/*! \brief Call the ConnectionWriteScanEnable function with appropriate
    parameters (considering both page and inquiry scan contexts)
    \param context The scan context (either page or inquiry scan context).
    \note This function may enable or disable scanning considering both page
    and inquiry scan contexts.
*/
void bredrScanManager_ConnectionWriteScanEnable(void);

/*! \brief Call the ConnectionWrite[X]scanActivity function with appropriate
    parameters (considering both page and inquiry scan contexts)
*/
void bredrScanManager_ConnectionWriteScanActivity(bsm_scan_context_t *context);

/*! \brief Handle the CL message.
    \param status Status of the message.
    \param outstanding Number of outstanding scan requests.
*/
void bredrScanManager_ConnectionHandleClDmWriteScanEnableCfm(bool status, uint16 outstanding);

/*! \brief Adjust page scan activity parameters based on throttle requirement as
           informed by Bandwidth Manager.

    \param throttle_required Indication about whether page scan activity
            shall throttle its bandwidth usage or not.
*/
void bredrScanManager_InstanceAdjustPageScanBandwidth(bool throttle_required);

/*! \brief Compare present and new required scan parameters and call the
           connection library function to change parameters if they differ.

    \param context Pointer to scan manager context object.

    \param type Scan type.

*/
void bredrScanManager_InstanceUpdateScanActivity(bsm_scan_context_t *context,
                                                        bredr_scan_manager_scan_type_t type);

/*! \brief Suspend ongoing truncated page scan request

    \param task Task that requests suspend truncated page scan operation

*/
void BredrScanManager_TruncatedPageScanSuspend(Task task);

/*! \brief Resume suspended truncated page scan request
*/
void  BredrScanManager_TruncatedPageScanResume(void);

/*! \brief Initialise internal state for truncated page scan
*/
void  BredrScanManager_TruncatedPageScanInit(void);

/*! \brief This function is called whenever the scan client/pause/param state
    changes. It evaluates the requirements of the current set of clients
    (including paused) and sets a goal, #bredrScanManager_InstanceSetGoal
    decides whether any changes to state are required */
void bredrScanManager_InstanceRefresh(bsm_scan_context_t *context);

#endif
