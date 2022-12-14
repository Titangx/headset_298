/*!
\copyright  Copyright (c) 2019-2021 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    
\file
\brief      Utility functions and constants required for HDMA Core.
*/

#ifdef INCLUDE_HDMA

#include "hdma_utils.h"
#include "hdma_queue.h"


#ifdef INCLUDE_HDMA_RSSI_EVENT
/*  Following filters specify half life, max age, and thresholds by urgency: critical, high, low for RSSI */
const hdma_thresholds_t rssi =
{
    .type = HDMA_QUEUE_RSSI,
    .halfLife_ms = {1000, 2000, 5000}, // in milliseconds
    .maxAge_ms = {3000, 10000, 30000}, // in milliseconds
    .absThreshold = {-80, -70, -60}, // in dBm
    .relThreshold = {5, 5, 8}
};
#endif

#ifdef INCLUDE_HDMA_LINK_QUALITY_EVENT
const hdma_thresholds_t link =
{
	.type = HDMA_QUEUE_LINK_QUALITY,
	.halfLife_ms = {1000, 2000, 5000}, // in milliseconds
	.maxAge_ms = {3000, 10000, 30000}, // in milliseconds
	.absThreshold = {62000, 62000, 62000},
	.relThreshold = {400, 800, 1200} 
};

#endif


#ifdef INCLUDE_HDMA_MIC_QUALITY_EVENT
/*  Following filters specify half life, max age, and thresholds by urgency: critical, high, low for Voice Quality  */
const hdma_thresholds_t mic =
{
	.type = HDMA_QUEUE_MIC,
	.halfLife_ms = {1000, 2000, 5000}, // in milliseconds
	.maxAge_ms = {3000, 10000, 30000}, // in milliseconds
	.absThreshold = {6, 9, 12}, 
	.relThreshold = {2, 3, 3}
};
#endif

#endif /* INCLUDE_HDMA */

