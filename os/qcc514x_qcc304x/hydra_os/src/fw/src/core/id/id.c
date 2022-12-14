/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   %%version */
/*
 * The patched fw version, plus bcd version for usb
 */

#include "id/id.h"

uint32 patched_fw_version = 0;

#ifdef DESKTOP_TEST_BUILD
#define get_build_id_number() (0xFA11FA11)
#else
/* Need this to be a true function so it can be called from Pylib */
uint32 get_build_id_number(void)
{
    return build_id_number;
}
#endif

uint16 id_get_build_bcd(void)
{
    uint16 shift,bcd = 0;
    uint32 version = patched_fw_version != 0 ?
        patched_fw_version : build_id_number;

    /* Pull each decimal digit out and turn it into the literally identical
     * hex digit */
    for (shift = 0; shift < 16; shift += 4)
    {
        bcd |= (uint16)((version % 10) << shift);
        version /= 10;
    }

    return bcd;
}
