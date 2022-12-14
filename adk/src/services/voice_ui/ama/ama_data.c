/*!
\copyright  Copyright (c) 2019 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    
\file       ama_data.c
\brief  Implementation of ama data 
*/

#include "ama_data.h"
#include <panic.h>
#include <logging.h>

#ifdef INCLUDE_AMA

/* Defines the ama data structure */
typedef struct
{
    ama_state_t state;
    ama_audio_data_t audio_config;
    bool stopping_session;
}ama_data_t;

static ama_data_t ama_data;

/***************************************************************************/
void AmaData_SetState(ama_state_t state)
{
    ama_data.state= state;
}

/***************************************************************************/
ama_state_t AmaData_GetState(void)
{
    return  ama_data.state;
}

/***************************************************************************/
bool AmaData_IsReadyToSendStartSpeech(void)
{
   return (AmaData_GetState() == ama_state_idle);
}

/***************************************************************************/
bool AmaData_IsSendingVoiceData(void)
{
   return (AmaData_GetState() == ama_state_sending);
}

/***************************************************************************/
ama_audio_data_t* AmaData_GetAudioData(void)
{
    return &ama_data.audio_config;
}

/***************************************************************************/
void AmaData_SetAudioData(ama_audio_data_t *config)
{
    ama_data.audio_config = *config;
}

/************************************************************************/

void AmaData_SetStoppingSession(bool stopping)
{
    ama_data.stopping_session = stopping;
}

/************************************************************************/

bool AmaData_IsStoppingSession(void)
{
    return ama_data.stopping_session;
}

#endif /* INCLUDE_AMA */

