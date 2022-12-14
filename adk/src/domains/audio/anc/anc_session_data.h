/*!
\copyright  Copyright (c) 2021-2022 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\file       anc_session_data.h
\addtogroup anc_state_manager
\brief      Handles session data corresponding to ANC domain.

Responsibilities:
  Handles and persists session data corresponding to ANC domain.
*/

#ifndef ANC_SESSION_DATA_H_
#define ANC_SESSION_DATA_H_

#include <types.h>
#include "anc_state_manager.h"

/*! @} */

typedef struct {
    anc_toggle_way_config_t toggle_configurations;
    anc_toggle_config_during_scenario_t standalone_config;
    anc_toggle_config_during_scenario_t playback_config;
    anc_toggle_config_during_scenario_t sco_config;
    anc_toggle_config_during_scenario_t va_config;
    anc_mode_t requested_mode;
#ifdef ENABLE_AUTO_AMBIENT
    bool atr_enabled;
    atr_vad_release_duration_t release_time_config;
#endif
} anc_session_data_t;

/*!
    \brief Retrieves the session data stored in PS.
           If PS is empty, stores default values in session_data
    \param session_data placeholder for session data
*/
#ifdef ENABLE_ANC
void AncSessionData_GetSessionData(anc_session_data_t* session_data);
#else
#define AncSessionData_GetSessionData(x) ((void)(0*x))
#endif

/*!
    \brief Stores ANC session data in PS.
           If data stored in PS is same as session_data, returns TRUE
    \param session_data data to be stored in PS
    \return TRUE on succesful store in PS, else FALSE.
*/
#ifdef ENABLE_ANC
bool AncSessionData_SetSessionData(anc_session_data_t* session_data);
#else
#define AncSessionData_SetSessionData(x) (FALSE)
#endif

/*!
    \brief Restore the default ANC session data.
*/
#ifdef ENABLE_ANC
void AncSessionData_RestoreDefault(void);
#else
#define AncSessionData_RestoreDefault() ((void)(0))
#endif

/*! @} */
#endif /* ANC_SESSION_DATA_H_ */
