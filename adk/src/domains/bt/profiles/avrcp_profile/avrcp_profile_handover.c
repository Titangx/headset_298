/*!
\copyright  Copyright (c) 2019-2021 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    
\file       avrcp_profile_handover.c
\brief      AVRCP profile handover related interfaces
*/

/* Only compile if AV defined */
#ifdef INCLUDE_AV
#ifdef INCLUDE_MIRRORING

#include "avrcp_profile.h"
#include "audio_sources.h"
#include "av.h"
#include <panic.h>
#include <avrcp.h>
#include <logging.h>

bool AvrcpProfile_Veto(avInstanceTaskData *the_inst)
{
    bool veto = FALSE;
    int32 first_due=0;

    /* Check for following conditions to veto the handover 
     * 1. Playback lock is set
     * 2. Notification and pass-through locks are set
     * 3. Responses pending to be sent
     */
    if (appAvrcpGetLock(the_inst))
    {
        DEBUG_LOG_INFO("AvrcpProfile_Veto, avrcp.lock(%d)",appAvrcpGetLock(the_inst));
        veto = TRUE;
    }
    else if (the_inst->avrcp.notification_lock)
    {
        DEBUG_LOG_INFO("AvrcpProfile_Veto, avrcp.notification_lock");
        veto = TRUE;
    }
    else if (the_inst->avrcp.playback_lock)
    {
        /* The playback_lock is set when a pause/play is sent to handset and cleared
           when the handset responds. If any further play/pause are requested before
           handset responds, the requests are queued conditionally on the lock.

           If the lock is set, only veto if there are queued requests or received responses
           (this avoids having to handover the queued requests). If there are no queued requests/ 
           responses then the lock itself will be handed over so that requests/responses will
           be handled on the new primary.
        */
        if (MessagePendingFirst(&the_inst->av_task, AV_INTERNAL_AVRCP_PAUSE_REQ, NULL) ||
            MessagePendingFirst(&the_inst->av_task, AV_INTERNAL_AVRCP_PLAY_REQ, NULL) ||
            MessagePendingFirst(&the_inst->av_task, AV_INTERNAL_AVRCP_PLAY_TOGGLE_REQ, NULL) ||
            MessagePendingFirst(&the_inst->av_task, AVRCP_EVENT_PLAYBACK_STATUS_CHANGED_IND, NULL)
            )
        {
            DEBUG_LOG_INFO("AvrcpProfile_Veto, avrcp.playback_lock and pending message");
            veto = TRUE;
        }
        else
        {
            /* AVRCP is not vetoing. If the handover succeeds, the new primary
               needs to resend AV_INTERNAL_AVRCP_CLEAR_PLAYBACK_LOCK_IND with
               the approximately the correct timeout. So the new primary knows
               the correct timeout, the value of the playback_lock is set to the
               milli-seconds remaining before the timer should expire. */
            int32 due;
            if (MessagePendingFirst(&the_inst->av_task,
                                    AV_INTERNAL_AVRCP_CLEAR_PLAYBACK_LOCK_IND, &due))
            {
                /* Always marshal a positive value */
                due = MAX(1, due);
                the_inst->avrcp.playback_lock = due;
            }
        }
    }
    else if (MessagePendingFirst(&the_inst->av_task, AVRCP_EVENT_VOLUME_CHANGED_IND, NULL))
    {
        DEBUG_LOG_INFO("AvrcpProfile_Veto, Volume Indication message pending");
        veto = TRUE;
    }
    else if (MessagePendingFirst(&the_inst->av_task, AV_INTERNAL_AVRCP_CONNECT_REQ, NULL))
    {
        DEBUG_LOG_INFO("AvrcpProfile_Veto, AV_INTERNAL_AVRCP_CONNECT_REQ message pending");
        veto = TRUE;
    }

/* To minimise the size of marshalled data, due time for pending messages is stored in uint8 type.
 * Data is quantised in 256 millisecond steps, for example, 0x1, i.e. 1 step means 256 milliseconds, 0xff means 65280 milliseconds.
 * Any due time value above 65280 milliseconds will be capped to the fixed max value of 65280 milliseconds. */
#define STEP_SIZE 256
#define CAP_MAX_DELAY_MILLISECONDS 65280
#define MILLISECONDS_TO_STEPS(x) (x/STEP_SIZE)
#define STEPS_TO_MILLISECONDS(x) (x*STEP_SIZE)

    if(!MessagePendingFirst(&the_inst->av_task, AV_INTERNAL_A2DP_UNROUTED_PAUSE_REQUEST, &first_due))
    {
        the_inst->send_avrcp_unrouted_pause_post_handover=0;
    }
    else
    {
        the_inst->send_avrcp_unrouted_pause_post_handover = MAX(1, MILLISECONDS_TO_STEPS(MIN(first_due, CAP_MAX_DELAY_MILLISECONDS)));
        DEBUG_LOG_INFO("AvrcpProfile_Veto, AV_INTERNAL_A2DP_UNROUTED_PAUSE_REQUEST exists in queue, due in %d millisecs[%d steps]", first_due, the_inst->send_avrcp_unrouted_pause_post_handover);
    }

    return veto;
}

static void avrcpProfile_SetInstanceDataForPrimaryOrAcceptor(avInstanceTaskData *the_inst)
{
    uint32 delay_in_milliseconds = 0;

    if(the_inst->send_avrcp_unrouted_pause_post_handover)
    {
        delay_in_milliseconds = STEPS_TO_MILLISECONDS(the_inst->send_avrcp_unrouted_pause_post_handover);
        DEBUG_LOG("avrcpProfile_SetInstanceDataForPrimaryOrAcceptor, AV_INTERNAL_A2DP_UNROUTED_PAUSE_REQUEST message due in %d millisecs", delay_in_milliseconds);
        MessageSendLater(&the_inst->av_task, AV_INTERNAL_A2DP_UNROUTED_PAUSE_REQUEST, NULL, delay_in_milliseconds);
        the_inst->send_avrcp_unrouted_pause_post_handover=0;
    }

    /* Get the AVRCP profile library instance for primary device */
    if(AvrcpGetInstanceFromBdaddr(&the_inst->bd_addr, &the_inst->avrcp.avrcp))
    {
        /* Set the client-task in AVRCP profile library instance for receiving the data */
        AvrcpSetAppTask(the_inst->avrcp.avrcp, &the_inst->av_task);

        /* Reset the timeout on the new secondary */
        if (the_inst->avrcp.playback_lock)
        {
            MessageSendLater(&the_inst->av_task,
                             AV_INTERNAL_AVRCP_CLEAR_PLAYBACK_LOCK_IND, NULL,
                             the_inst->avrcp.playback_lock);
        }
    }
    else
    {
        /* AV instance has been created but there is no AVRCP connection yet. 
           Initialize AVRCP instance, keeping it ready for an incoming connection. */
        appAvrcpInstanceInit(the_inst);
    }
}

static void avrcpProfile_SetInstanceDataForSecondaryOrInitiator(avInstanceTaskData *the_inst)
{
    /* If the earbud is a secondary device or initiated NRS handover then move it to 
    disconnecting state */
    if (the_inst->avrcp.state == AVRCP_STATE_CONNECTED)
    {
        the_inst->avrcp.state = AVRCP_STATE_DISCONNECTING;
        appAvrcpClearPlaybackLock(the_inst);
    }
}

void AvrcpProfile_Commit(avInstanceTaskData *the_inst, bool is_primary)
{
    if(is_primary == FALSE)
    {
        avrcpProfile_SetInstanceDataForSecondaryOrInitiator(the_inst);
    }
    else
    {
        avrcpProfile_SetInstanceDataForPrimaryOrAcceptor(the_inst);
    }
}

#endif /* INCLUDE_MIRRORING */
#endif /* INCLUDE_AV */
