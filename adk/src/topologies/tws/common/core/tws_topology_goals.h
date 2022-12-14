/*!
\copyright  Copyright (c) 2019-2021 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    
\brief      Interface to TWS Topology goal handling.
*/

#ifndef TWS_TOPOLOGY_GOALS_H
#define TWS_TOPOLOGY_GOALS_H

#include <message.h>


/*! Definition of goals which the topology may be instructed (by the SM) to achieve. */
typedef enum
{
    /*! Empty goal. */
    tws_topology_goal_none,

    /*! Goal is to pair with a peer Earbud. */
    tws_topology_goal_pair_peer,

    /*! Goal is determine the Primary or Secondary role of the Earbud. */
    tws_topology_goal_find_role,

    /*! Goal is to create BREDR ACL to Primary. */
    tws_topology_goal_secondary_connect_peer,

    /*! Goal is to connect peer BREDR profiles to Secondary. */
    tws_topology_goal_primary_connect_peer_profiles,

    /*! Goal is to enable page scan on Primary for Secondary to connect BREDR ACL.
        Note - the primary _must_ connect all L2CAP channels to the secondary. */
    tws_topology_goal_primary_connectable_peer,

    /*! Goal is to discard current role, disconnect links and cancel any
        activity. Typically used going into the case. */
    tws_topology_goal_no_role_idle,

    /*! Goal is to take on the Primary role. */
    tws_topology_goal_become_primary,

    /*! Goal is to take on the Secondary role. */
    tws_topology_goal_become_secondary,

    /*! Goal is to take on the Acting Primary role. */
    tws_topology_goal_become_acting_primary,

    /*! Goal to prepare for dynamic handover */
    tws_topology_goal_dynamic_handover_prepare,
    /*! Goal to perform dynamic handover from primary to secondary roles */
    tws_topology_goal_dynamic_handover,

    /*! Goal to undo preparations for dynamic handover */
    tws_topology_goal_dynamic_handover_undo_prepare,

    /*! Always the final goal */
    tws_topology_goals_end,

} tws_topology_goal_id;


/*! \brief Query if a goal is currently active.

    \param goal The goal being queried for status.

    \return TRUE if goal is currently active.
*/
bool TwsTopology_IsGoalActive(tws_topology_goal_id goal);

/*! \brief Query if a goal is in the execution queue.

    \param goal The goal being queried.

    \return TRUE if goal is currently queued.
*/
bool TwsTopology_IsGoalQueued(tws_topology_goal_id goal);

/*! \brief Check if there are any pending goals.

    \return TRUE if there are one or more goals queued; FALSE otherwise.
*/
bool TwsTopology_IsAnyGoalPending(void);

/*! \brief Handler for new and queued goals.
    \note This is a common handler for new goals generated by the topology SM
    and goals queued waiting on completion or cancelling already in-progress
    goals.
*/
void TwsTopology_HandleGoalDecision(Task task, MessageId id, Message message);

/*! \brief Initialise the tws topology goals */
void TwsTopology_GoalsInit(void);

#endif /* TWS_TOPOLOGY_GOALS_H */
