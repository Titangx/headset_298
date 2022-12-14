/*!
\copyright  Copyright (c) 2019-2021 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\file
\brief      Source file converts System Events to corresponding Audio Prompt UI Events
            by table look-up, using a configuration table passed in by the Application.
            It then plays these Prompts when required using the Kymera audio framework
            Aux path.
*/
#ifndef UI_PROMPTS_H
#define UI_PROMPTS_H

#include <ui_indicator.h>

#include <domain_message.h>
#include <csrtypes.h>
#include <message.h>
#include <kymera.h>
#include <task_list.h>
#include <rtime.h>

/*! Defines that can be used by the application to create its prompt configuration based on the default ADK prompt format */
#ifdef INCLUDE_AAC_PROMPTS
#define PROMPT_FILE_EXTENSION ".aac"
#define PROMPT_FORMAT PROMPT_FORMAT_AAC
#else
#define PROMPT_FILE_EXTENSION ".sbc"
#define PROMPT_FORMAT PROMPT_FORMAT_SBC
#endif

#define PROMPT_FILENAME(prompt_name) prompt_name PROMPT_FILE_EXTENSION

/*! The maximum number of prompts to queue when waiting for the kymera resource */
#define UI_PROMPTS_MAX_QUEUE_SIZE 5

#define UI_PROMPTS_MAX_USER_FUNCTIONS   3

/*! A prompt user function to get event specific indications */
typedef const ui_event_indicator_table_t* (*ui_prompts_data_fn)(MessageId id);

typedef struct {
    /*! Pointer to a prompt user function to retrieve event specific indications */
    ui_prompts_data_fn  user_fn;
    /*! A message ID that the prompt user function will handle */
    MessageId           message_id;
} ui_prompts_user_config_t;

/*! \brief ui_prompt task structure */
typedef struct
{
    /*! The task. */
    TaskData task;

    /*! Seperate task for handling conditional messages to play prompts */
    TaskData prompt_task;

    /*! The configuration table of System Event to prompts to play, passed from the Application. */
    const ui_event_indicator_table_t * sys_event_to_prompt_data_mappings;
    uint8 mapping_table_size;

    /*! This is a hold off time for after a voice prompt is played, in milliseconds. When a voice prompt
    is played, for the period of time specified, any repeat of this prompt will not be played. If the no
    repeat period set to zero, then all prompts will be played, regardless of whether the prompt was
    recently played.*/
    Delay no_repeat_period_in_ms;

    /*! The last prompt played, used to avoid repeating prompts. */
    MessageId last_prompt_played;

    /*! Used to gate audio prompts and prevent them from being played. */
    unsigned prompt_playback_enabled :1;

    /*! Needed to indicate back to the Power Manager UI Prompt completion, if a prompt was configured
    to be played on Power Off. */
    unsigned indicate_when_power_shutdown_prepared :1;

    /*! This counter indicates the number of prompts to play before shutdown prepared response can be
    issued back to the Power Manager. */
    unsigned prompts_remaining_till_shutdown : 2;

    /*! Specifies whether the UI Indicator should generate UI Inidications from System Event messages
    received at the task handler. \note this is not the same as whether indication playback is enabled. */
    unsigned generate_ui_events :1;

    /*! The number of valid user prompt handlers in the following table */
    size_t                      num_user_prompts;
    /*! The user prompts table */
    ui_prompts_user_config_t    user_prompts[UI_PROMPTS_MAX_USER_FUNCTIONS];
} ui_prompts_task_data_t;

/*! brief Set/reset play_prompt flag. This is flag is used to check if prompts can be played or not.
  application will set and reset the flag. */
void UiPrompts_SetPromptPlaybackEnabled(bool play_prompt);

/*! Get the pointer to UI Prompts Task */
Task UiPrompts_GetUiPromptsTask(void);

/*! brief Initialise UI Prompts module */
bool UiPrompts_Init(Task init_task);

/*! brief De-initialise UI Prompts module */
bool UiPrompts_DeInit(void);

/*! brief Used by the Application to set the mapping of System Events to Audio Prompts. */
void UiPrompts_SetPromptConfiguration(const ui_event_indicator_table_t * table, uint8 size);

/*! brief Used by the Application to set the back off timer for which a play of the same audio prompt will not be repeated. */
void UiPrompts_SetNoRepeatPeriod(Delay no_repeat_period_in_ms);

/*! brief Used by the UI component to notify the Audio Prompts UI Indicator of a UI event. */
void UiPrompts_NotifyUiIndication(MessageId sys_event, rtime_t time_to_play);

/*! brief Used by the Application to control whether the UI Indicator shall generate UI events. */
void UiPrompts_GenerateUiEvents(bool generate);

/*! brief Used by the UI component to prepare for the prompt associated with a given system event. */
void UiPrompts_PrepareForPrompt(MessageId sys_event);

/*! brief Used by the UI component to notify the Audio Prompts UI Indicator of a UI prepare for prompt event. */
void UiPrompts_NotifyUiPrepareIndication(MessageId sys_event);

/*! brief Used by a prompt user to specify a custom handler for a given message ID */
void UiPrompts_SetUserPromptDataFunction(ui_prompts_data_fn fn, MessageId id);

/*! brief Used by a prompt user to remove a custom handler for a given message ID */
void UiPrompts_ClearUserPromptDataFunction(MessageId id);

#endif // UI_PROMPTS_H
