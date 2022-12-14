/*!
    \copyright Copyright (c) 2022 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \file chain_va_wuw_apva.h
    \brief The chain_va_wuw_apva chain.

    This file is generated by D:\Work\Qcc\qcc514x-qcc304x-src_headset-ADK-22.2-CS1-r00298.1\adk\tools\packages\chaingen\chaingen_mod\__init__.pyc.
*/

#ifndef _CHAIN_VA_WUW_APVA_H__
#define _CHAIN_VA_WUW_APVA_H__

/*!
\page chain_va_wuw_apva
    @startuml
        object OPR_WUW
        OPR_WUW : id = CAP_ID_VA_APVA
        object EPR_VA_WUW_IN #lightgreen
        OPR_WUW "IN(0)" <-- EPR_VA_WUW_IN
    @enduml
*/

#include <chain.h>

extern const chain_config_t chain_va_wuw_apva_config;

#endif /* _CHAIN_VA_WUW_APVA_H__ */

