// -----------------------------------------------------------------------------
// Copyright (c) 2022                  Qualcomm Technologies International, Ltd.
//
#ifndef __SBC_DECODE_GEN_ASM_H__
#define __SBC_DECODE_GEN_ASM_H__

// CodeBase IDs
.CONST $M.SBC_DECODE_SBCDEC_CAP_ID       	0x0016;
.CONST $M.SBC_DECODE_SBCDEC_ALT_CAP_ID_0       	0x405F;
.CONST $M.SBC_DECODE_SBCDEC_SAMPLE_RATE       	0;
.CONST $M.SBC_DECODE_SBCDEC_VERSION_MAJOR       	0;

// Constant Values


// Piecewise Disables


// Statistic Block
.CONST $M.SBC_DECODE.STATUS.SAMPLING_FREQUENCY		0*ADDR_PER_WORD;
.CONST $M.SBC_DECODE.STATUS.CHANNEL_MODE      		1*ADDR_PER_WORD;
.CONST $M.SBC_DECODE.STATUS.BITPOOL           		2*ADDR_PER_WORD;
.CONST $M.SBC_DECODE.STATUS.NUM_BLOCKS        		3*ADDR_PER_WORD;
.CONST $M.SBC_DECODE.STATUS.NUM_CHANNELS      		4*ADDR_PER_WORD;
.CONST $M.SBC_DECODE.STATUS.NUM_SUBBANDS      		5*ADDR_PER_WORD;
.CONST $M.SBC_DECODE.STATUS.ALLOC_METHOD      		6*ADDR_PER_WORD;
.CONST $M.SBC_DECODE.STATUS.BLOCK_SIZE             	7;

// Channel Mode

// BitPool

// Number of Blocks

// Number of Channels

// Number of Subbands

// Allocation Method

// Parameter Block
.CONST $M.SBC_DECODE.PARAMETERS.OFFSET_CONFIG		0*ADDR_PER_WORD;
.CONST $M.SBC_DECODE.PARAMETERS.STRUCT_SIZE 		1;


#endif // __SBC_DECODE_GEN_ASM_H__
