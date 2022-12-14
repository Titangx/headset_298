#ifndef __FEATURE_H__
#define __FEATURE_H__
#include <app/feature/feature_if.h>

/*! file  @brief Related to licensing of new features
*/

#if TRAPSET_CORE

/**
 *  \brief This trap checks whether a feature is licensed for a device.
 *     
 *     This trap verifies whether a device is licensed to use a given feature.
 *  
 *  \param feature Identifier for a given feature. Refer \#feature_id
 *  \return TRUE if device has valid license for feature, else FALSE.
 * 
 * \ingroup trapset_core
 */
bool FeatureVerifyLicense(feature_id feature);

/**
 * 
 * \ingroup trapset_core
 */
bool FeatureChallengeResponse(feature_id feature, const uint32 * challenge_ptr, uint32 * response_ptr);
#endif /* TRAPSET_CORE */
#endif
