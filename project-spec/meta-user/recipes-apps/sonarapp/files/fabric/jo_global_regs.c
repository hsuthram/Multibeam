/*
 * jo_global_regs.c
 *
 *  Created on: Sep 16, 2019
 *      Author: jcarmona
 */

// #include <xparameters.h>
// #include <xil_types.h>
// #include <xgpio.h>
// #include <xstatus.h>
// #include <xuartps.h>
// #include <string.h>
// #include <xil_cache.h>
// #include <xil_printf.h>

#include "jo_timing_tools.h"
#include "jo_global_regs.h"



int jo_global_regs_select_path_raw_data ( void)
{
	Xil_Out32(GLOBAL_REGS_PATH_SELECT_CREG, GLOBA_REGS_SELECT_RAW_DATA_PATH);

	return 0;
}


int jo_global_regs_select_path_dsp ( void)
{
	Xil_Out32(GLOBAL_REGS_PATH_SELECT_CREG, GLOBA_REGS_SELECT_DSP_PATH);

	return 0;
}
