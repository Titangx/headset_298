/****************************************************************************
 * Copyright (c) 2014 - 2017 Qualcomm Technologies International, Ltd.
****************************************************************************/
/**
 * \file  cbops_sidetone_filter.c
 * \ingroup cbops
 *
 * This file contains functions for the sidetone filter cbops operator
 */

/****************************************************************************
Include Files
 */
#include "pmalloc/pl_malloc.h"
#include "cbops_c.h"
#include "cbops_sidetone_filter.h"
#include "platform/pl_fractional.h"
#include "peq_c.h"
/****************************************************************************
Public Function Definitions
*/

/*
 * create_sidetone_filter_op
 * It fits into multi-channel model, but it only ever actually uses single in/out channel.
 */
cbops_op* create_sidetone_filter_op(unsigned input_idx, unsigned output_idx,
                                          unsigned max_stages, cbops_sidetone_params *st_params,
                                          unsigned *peq_params)
{
    cbops_op *op;

    op = (cbops_op*)xzpmalloc(sizeof_cbops_op(cbops_sidetone_filter_op, 1, 1) +
                              SH_PEQ_OBJECT_SIZE(max_stages));
    if(op)
    {
        cbops_sidetone_filter_op  *params;
        t_peq_object *peq;
        
        op->function_vector    = cbops_sidetone_filter_table;

        /* Setup cbop param struct header info */
        params = (cbops_sidetone_filter_op*)cbops_populate_param_hdr(op, 1, 1, &input_idx, &output_idx);

        /* Set up the parameters - there are no channel-specific params as such, as it only works on a
         * single channel.
         */

        params->inv_dac_gain = FRACTIONAL(1.0); // TODO - handle gain adjustment for post volume
        peq = (t_peq_object *)params->peq;
        params->params = st_params;
        peq->max_stages = max_stages;
        peq->params_ptr = (t_peq_params *) peq_params;
    }

    return(op);
}


