// -----------------------------------------------------------------------------
// Copyright (c) 2022                  Qualcomm Technologies International, Ltd.
//
#ifndef __EARBUD_FIT_TEST_GEN_ASM_H__
#define __EARBUD_FIT_TEST_GEN_ASM_H__

// CodeBase IDs
.CONST $M.EARBUD_FIT_TEST_EARBUD_FIT_TEST_16K_CAP_ID       	0x00CA;
.CONST $M.EARBUD_FIT_TEST_EARBUD_FIT_TEST_16K_ALT_CAP_ID_0       	0x40A2;
.CONST $M.EARBUD_FIT_TEST_EARBUD_FIT_TEST_16K_SAMPLE_RATE       	16000;
.CONST $M.EARBUD_FIT_TEST_EARBUD_FIT_TEST_16K_VERSION_MAJOR       	1;

// Constant Values
.CONST $M.EARBUD_FIT_TEST.CONSTANT.IN_OUT_EAR_CTRL  		0x00000003;
.CONST $M.EARBUD_FIT_TEST.CONSTANT.BIN_SELECT_125Hz 		0x00000001;
.CONST $M.EARBUD_FIT_TEST.CONSTANT.BIN_SELECT_250Hz 		0x00000002;
.CONST $M.EARBUD_FIT_TEST.CONSTANT.BIN_SELECT_375Hz 		0x00000003;
.CONST $M.EARBUD_FIT_TEST.CONSTANT.BIN_SELECT_500Hz 		0x00000004;
.CONST $M.EARBUD_FIT_TEST.CONSTANT.BIN_SELECT_625Hz 		0x00000005;
.CONST $M.EARBUD_FIT_TEST.CONSTANT.BIN_SELECT_750Hz 		0x00000006;
.CONST $M.EARBUD_FIT_TEST.CONSTANT.BIN_SELECT_875Hz 		0x00000007;
.CONST $M.EARBUD_FIT_TEST.CONSTANT.BIN_SELECT_1000Hz		0x00000008;


// Piecewise Disables


// Statistic Block
.CONST $M.EARBUD_FIT_TEST.STATUS.CUR_MODE        		0*ADDR_PER_WORD;
.CONST $M.EARBUD_FIT_TEST.STATUS.OVR_CONTROL     		1*ADDR_PER_WORD;
.CONST $M.EARBUD_FIT_TEST.STATUS.IN_OUT_EAR_CTRL 		2*ADDR_PER_WORD;
.CONST $M.EARBUD_FIT_TEST.STATUS.FIT_QUALITY_FLAG		3*ADDR_PER_WORD;
.CONST $M.EARBUD_FIT_TEST.STATUS.FIT_EVENT       		4*ADDR_PER_WORD;
.CONST $M.EARBUD_FIT_TEST.STATUS.FIT_TIMER       		5*ADDR_PER_WORD;
.CONST $M.EARBUD_FIT_TEST.STATUS.POWER_REF       		6*ADDR_PER_WORD;
.CONST $M.EARBUD_FIT_TEST.STATUS.POWER_INT_MIC   		7*ADDR_PER_WORD;
.CONST $M.EARBUD_FIT_TEST.STATUS.POWER_RATIO     		8*ADDR_PER_WORD;
.CONST $M.EARBUD_FIT_TEST.STATUS.BLOCK_SIZE           	9;

// System Mode
.CONST $M.EARBUD_FIT_TEST.SYSMODE.STANDBY		0;
.CONST $M.EARBUD_FIT_TEST.SYSMODE.FULL   		1;
.CONST $M.EARBUD_FIT_TEST.SYSMODE.MAX_MODES		2;

// System Control
.CONST $M.EARBUD_FIT_TEST.CONTROL.MODE_OVERRIDE		0x2000;

// In/Out of Ear Status
.CONST $M.EARBUD_FIT_TEST.IN_OUT_EAR_CTRL.IN_EAR 		0x0001;
.CONST $M.EARBUD_FIT_TEST.IN_OUT_EAR_CTRL.OUT_EAR		0x0000;

// Fit Quality
.CONST $M.EARBUD_FIT_TEST.FIT_QUALITY_FLAG.FIT_BAD 		0x0000;
.CONST $M.EARBUD_FIT_TEST.FIT_QUALITY_FLAG.FIT_GOOD		0x0001;

// Fit Quality Event
.CONST $M.EARBUD_FIT_TEST.FIT_EVENT.EVENT_CLEAR   		0x0000;
.CONST $M.EARBUD_FIT_TEST.FIT_EVENT.EVENT_DETECTED		0x0001;
.CONST $M.EARBUD_FIT_TEST.FIT_EVENT.EVENT_SENT    		0x0002;

// Parameter Block
.CONST $M.EARBUD_FIT_TEST.PARAMETERS.OFFSET_POWER_SMOOTH_TIME		0*ADDR_PER_WORD;
.CONST $M.EARBUD_FIT_TEST.PARAMETERS.OFFSET_FIT_THRESHOLD    		1*ADDR_PER_WORD;
.CONST $M.EARBUD_FIT_TEST.PARAMETERS.OFFSET_EVENT_GOOD_FIT   		2*ADDR_PER_WORD;
.CONST $M.EARBUD_FIT_TEST.PARAMETERS.OFFSET_BIN_SELECT       		3*ADDR_PER_WORD;
.CONST $M.EARBUD_FIT_TEST.PARAMETERS.STRUCT_SIZE            		4;


#endif // __EARBUD_FIT_TEST_GEN_ASM_H__
