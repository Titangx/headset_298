/*!
\copyright  Copyright (c) 2021 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\file       headset_soc_config.h
\brief      Application specific voltage->percentage configuration
*/

#ifndef HEADSET_SOC_CONFIG_H_
#define HEADSET_SOC_CONFIG_H_

#include "state_of_charge.h"

/*! \brief Return the voltage->percentage lookup configuration table for the headset application.

    The configuration table can be passed directly to the SoC component in
    domains.

    \param table_length - used to return the number of rows in the config table.

    \return Application specific SoC configuration table.
*/
const soc_lookup_t* HeadsetSoC_GetConfigTable(unsigned* table_length);

#endif /* HEADSET_SOC_CONFIG_H_ */

