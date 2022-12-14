/*!
\copyright  Copyright (c) 2019 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\file       headset_utils.h
\brief      Utility definitions.
*/

#ifndef HEADSET_UTILS_H_
#define HEADSET_UTILS_H_


/* Useful macro to ensure linker doesn't discard a function that's not
 * referenced in the code but maybe called from pydbg.  Example:
 *
 * keep void *test_func(void)
 */
#ifdef GC_SECTIONS
#define keep _Pragma("codesection KEEP_PM")
#else
#define keep
#endif

#endif /* HEADSET_UTILS_H_ */
