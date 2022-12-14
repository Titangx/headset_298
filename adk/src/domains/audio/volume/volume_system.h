/*!
\copyright  Copyright (c) 2018-2022 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\file		volume_system.h
\defgroup   volume_system Volume System
\ingroup    volume_group
\brief      System volume.
*/

#ifndef VOLUME_SYSTEM_H_
#define VOLUME_SYSTEM_H_

#include "volume_types.h"

/*! @{ */

/*! \brief Gets the system volume.

    \return The system volume
 */
volume_t Volume_GetSystemVolume(void);

/*! \brief Sets the system volume.

    \param volume The new system volume to set
 */
void Volume_SetSystemVolume(volume_t volume);

/*! \brief Calculates the output volume.

    \param trim_volume
    \param mute_state The mute state for the source associated with the volume
 */
volume_t Volume_CalculateOutputVolume(volume_t trim_volume, mute_state_t mute_state);

/*! @} */

#endif /* VOLUME_SYSTEM_H_ */
