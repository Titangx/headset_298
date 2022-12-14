/*!
\copyright  Copyright (c) 2005 - 2022 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    
\file
\brief      Key Sync component private header.
*/

#ifndef KEY_SYNC_PRIVATE_H
#define KEY_SYNC_PRIVATE_H

#include <message.h>
#include <task_list.h>

/*! \brief Key sync task data. */
typedef struct
{
    TaskData task;
    task_list_t *listeners;
} key_sync_task_data_t;

/*! \brief Component level visibility of the key sync task data. */
extern key_sync_task_data_t key_sync;

/*! \brief Accessor for key sync task data. */
#define keySync_GetTaskData()   (&key_sync)

/*! \brief Accessor for key sync task. */
#define keySync_GetTask()       (&key_sync.task)
#endif /* KEY_SYNC_PRIVATE_H */
