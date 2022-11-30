// -----------------------------------------------------------------------------
// Copyright (c) 2022                  Qualcomm Technologies International, Ltd.
//
#ifndef __PEQ_GEN_ASM_H__
#define __PEQ_GEN_ASM_H__

// CodeBase IDs
.CONST $M.PEQ_PEQ_CAP_ID       	0x0049;
.CONST $M.PEQ_PEQ_ALT_CAP_ID_0       	0x405C;
.CONST $M.PEQ_PEQ_SAMPLE_RATE       	0;
.CONST $M.PEQ_PEQ_VERSION_MAJOR       	1;

// Constant Values
.CONST $M.PEQ.CONSTANT.CORE_TYPE_Single_precision		0x00000000;
.CONST $M.PEQ.CONSTANT.CORE_TYPE_First_order     		0x00000001;
.CONST $M.PEQ.CONSTANT.CORE_TYPE_Double_precision		0x00000002;


// Piecewise Disables
.CONST $M.PEQ.CONFIG.BYPASS		0x00000001;


// Statistic Block
.CONST $M.PEQ.STATUS.CUR_MODE         		0*ADDR_PER_WORD;
.CONST $M.PEQ.STATUS.OVR_CONTROL      		1*ADDR_PER_WORD;
.CONST $M.PEQ.STATUS.OVR_COEFF_MODE   		2*ADDR_PER_WORD;
.CONST $M.PEQ.STATUS.COMPILED_CONFIG  		3*ADDR_PER_WORD;
.CONST $M.PEQ.STATUS.OP_STATE         		4*ADDR_PER_WORD;
.CONST $M.PEQ.STATUS.OP_INTERNAL_STATE		5*ADDR_PER_WORD;
.CONST $M.PEQ.STATUS.BLOCK_SIZE            	6;

// System Mode
.CONST $M.PEQ.SYSMODE.STATIC   		0;
.CONST $M.PEQ.SYSMODE.MUTE     		1;
.CONST $M.PEQ.SYSMODE.FULL     		2;
.CONST $M.PEQ.SYSMODE.PASS_THRU		3;
.CONST $M.PEQ.SYSMODE.MAX_MODES		4;

// System Control
.CONST $M.PEQ.CONTROL.MODE_OVERRIDE		0x2000;

// Coefficients override mode indicator
.CONST $M.PEQ.OvrCoeff.ON		1;

// CompCfg

// Operator state

// Operator internal state

// Parameter Block
.CONST $M.PEQ.PARAMETERS.OFFSET_PEQ_CONFIG  		0*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_CORE_TYPE   		1*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_NUM_BANDS   		2*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_MASTER_GAIN 		3*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE1_TYPE 		4*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE1_FC   		5*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE1_GAIN 		6*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE1_Q    		7*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE2_TYPE 		8*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE2_FC   		9*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE2_GAIN 		10*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE2_Q    		11*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE3_TYPE 		12*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE3_FC   		13*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE3_GAIN 		14*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE3_Q    		15*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE4_TYPE 		16*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE4_FC   		17*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE4_GAIN 		18*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE4_Q    		19*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE5_TYPE 		20*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE5_FC   		21*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE5_GAIN 		22*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE5_Q    		23*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE6_TYPE 		24*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE6_FC   		25*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE6_GAIN 		26*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE6_Q    		27*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE7_TYPE 		28*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE7_FC   		29*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE7_GAIN 		30*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE7_Q    		31*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE8_TYPE 		32*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE8_FC   		33*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE8_GAIN 		34*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE8_Q    		35*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE9_TYPE 		36*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE9_FC   		37*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE9_GAIN 		38*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE9_Q    		39*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE10_TYPE		40*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE10_FC  		41*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE10_GAIN		42*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.OFFSET_STAGE10_Q   		43*ADDR_PER_WORD;
.CONST $M.PEQ.PARAMETERS.STRUCT_SIZE       		44;


#endif // __PEQ_GEN_ASM_H__
