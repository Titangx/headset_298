// -----------------------------------------------------------------------------
// Copyright (c) 2022                  Qualcomm Technologies International, Ltd.
//
#include "earbud_fit_test_gen_c.h"

#ifndef __GNUC__ 
_Pragma("datasection CONST")
#endif /* __GNUC__ */

static unsigned defaults_earbud_fit_testEARBUD_FIT_TEST_16K[] = {
   0x00CBC6A8u,			// POWER_SMOOTH_TIME
   0x05000000u,			// FIT_THRESHOLD
   0x00300000u,			// EVENT_GOOD_FIT
   0x00000001u			// BIN_SELECT
};

unsigned *EARBUD_FIT_TEST_GetDefaults(unsigned capid){
	switch(capid){
		case 0x00CA: return defaults_earbud_fit_testEARBUD_FIT_TEST_16K;
		case 0x40A2: return defaults_earbud_fit_testEARBUD_FIT_TEST_16K;
	}
	return((unsigned *)0);
}
