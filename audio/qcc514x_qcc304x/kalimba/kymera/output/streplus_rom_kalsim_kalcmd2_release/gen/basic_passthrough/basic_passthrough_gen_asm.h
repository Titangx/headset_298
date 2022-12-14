// -----------------------------------------------------------------------------
// Copyright (c) 2022                  Qualcomm Technologies International, Ltd.
//
#ifndef __BASIC_PASSTHROUGH_GEN_ASM_H__
#define __BASIC_PASSTHROUGH_GEN_ASM_H__

// CodeBase IDs
.CONST $M.BASIC_PASSTHROUGH_BPT_CAP_ID       	0x0001;
.CONST $M.BASIC_PASSTHROUGH_BPT_ALT_CAP_ID_0       	0x4002;
.CONST $M.BASIC_PASSTHROUGH_BPT_SAMPLE_RATE       	0;
.CONST $M.BASIC_PASSTHROUGH_BPT_VERSION_MAJOR       	1;

.CONST $M.BASIC_PASSTHROUGH_TTP_CAP_ID       	0x003C;
.CONST $M.BASIC_PASSTHROUGH_TTP_ALT_CAP_ID_0       	0x4067;
.CONST $M.BASIC_PASSTHROUGH_TTP_SAMPLE_RATE       	0;
.CONST $M.BASIC_PASSTHROUGH_TTP_VERSION_MAJOR       	1;

// Constant Values


// Piecewise Disables
.CONST $M.BASIC_PASSTHROUGH.CONFIG.PM_BYPASS		0x00000001;


// Statistic Block
.CONST $M.BASIC_PASSTHROUGH.STATUS.CUR_MODE_OFFSET		0*ADDR_PER_WORD;
.CONST $M.BASIC_PASSTHROUGH.STATUS.PEAK_LEVEL_1   		1*ADDR_PER_WORD;
.CONST $M.BASIC_PASSTHROUGH.STATUS.PEAK_LEVEL_2   		2*ADDR_PER_WORD;
.CONST $M.BASIC_PASSTHROUGH.STATUS.PEAK_LEVEL_3   		3*ADDR_PER_WORD;
.CONST $M.BASIC_PASSTHROUGH.STATUS.PEAK_LEVEL_4   		4*ADDR_PER_WORD;
.CONST $M.BASIC_PASSTHROUGH.STATUS.PEAK_LEVEL_5   		5*ADDR_PER_WORD;
.CONST $M.BASIC_PASSTHROUGH.STATUS.PEAK_LEVEL_6   		6*ADDR_PER_WORD;
.CONST $M.BASIC_PASSTHROUGH.STATUS.PEAK_LEVEL_7   		7*ADDR_PER_WORD;
.CONST $M.BASIC_PASSTHROUGH.STATUS.PEAK_LEVEL_8   		8*ADDR_PER_WORD;
.CONST $M.BASIC_PASSTHROUGH.STATUS.OVR_CONTROL    		9*ADDR_PER_WORD;
.CONST $M.BASIC_PASSTHROUGH.STATUS.BLOCK_SIZE          	10;

// System Mode
.CONST $M.BASIC_PASSTHROUGH.SYSMODE.STATIC 		0;
.CONST $M.BASIC_PASSTHROUGH.SYSMODE.STANDBY		1;
.CONST $M.BASIC_PASSTHROUGH.SYSMODE.MONITOR		2;
.CONST $M.BASIC_PASSTHROUGH.SYSMODE.MAX_MODES		3;

// System Control
.CONST $M.BASIC_PASSTHROUGH.CONTROL.MODE_OVERRIDE		0x2000;

// Parameter Block
.CONST $M.BASIC_PASSTHROUGH.PARAMETERS.OFFSET_CONFIG		0*ADDR_PER_WORD;
.CONST $M.BASIC_PASSTHROUGH.PARAMETERS.OFFSET_GAIN  		1*ADDR_PER_WORD;
.CONST $M.BASIC_PASSTHROUGH.PARAMETERS.STRUCT_SIZE 		2;


#endif // __BASIC_PASSTHROUGH_GEN_ASM_H__
