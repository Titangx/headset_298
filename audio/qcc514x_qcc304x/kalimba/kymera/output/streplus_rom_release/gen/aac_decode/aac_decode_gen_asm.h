// -----------------------------------------------------------------------------
// Copyright (c) 2022                  Qualcomm Technologies International, Ltd.
//
#ifndef __AAC_DECODE_GEN_ASM_H__
#define __AAC_DECODE_GEN_ASM_H__

// CodeBase IDs
.CONST $M.AAC_DECODE_AACD_CAP_ID       	0x0018;
.CONST $M.AAC_DECODE_AACD_ALT_CAP_ID_0       	0x404F;
.CONST $M.AAC_DECODE_AACD_SAMPLE_RATE       	0;
.CONST $M.AAC_DECODE_AACD_VERSION_MAJOR       	0;

// Constant Values


// Piecewise Disables


// Statistic Block
.CONST $M.AAC_DECODE.STATUS.SF_INDEX          		0*ADDR_PER_WORD;
.CONST $M.AAC_DECODE.STATUS.CHANNEL_CONFIG    		1*ADDR_PER_WORD;
.CONST $M.AAC_DECODE.STATUS.AUDIO_OBJ_TYPE    		2*ADDR_PER_WORD;
.CONST $M.AAC_DECODE.STATUS.EXT_AUDIO_OBJ_TYPE		3*ADDR_PER_WORD;
.CONST $M.AAC_DECODE.STATUS.SBR_PRESENT       		4*ADDR_PER_WORD;
.CONST $M.AAC_DECODE.STATUS.MP4_FRAME_COUNT   		5*ADDR_PER_WORD;
.CONST $M.AAC_DECODE.STATUS.BLOCK_SIZE             	6;

// Parameter Block
.CONST $M.AAC_DECODE.PARAMETERS.OFFSET_CONFIG		0*ADDR_PER_WORD;
.CONST $M.AAC_DECODE.PARAMETERS.STRUCT_SIZE 		1;


#endif // __AAC_DECODE_GEN_ASM_H__