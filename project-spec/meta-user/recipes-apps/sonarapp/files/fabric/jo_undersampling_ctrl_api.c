/*
 * jo_AFE_interface_api.c
 *
 *  Created on: Mar 29, 2019
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
// #include <xscugic.h>
// #include <xspips.h>

#include "jo_undersampling_ctrl_api.h"



int jo_undersamp_ctrl_reset(void)
{
	Xil_Out32(UNDERSAMP_CTRL_SRESET_CREG , 0);    //print("preparing reset \n\r");
	Xil_Out32(UNDERSAMP_CTRL_SRESET_CREG , 1);    //print("reset in process \n\r");
	Xil_Out32(UNDERSAMP_CTRL_SRESET_CREG , 0);    //print("reset done \n\r");

	return(0);
}


int jo_undersamp_ctrl_cfg ( jo_usamp_ctrl *usamp_cfg )
{

	Xil_Out32(UNDERSAMP_CTRL_INTERSAMP_PERIOD_CREG,   usamp_cfg->isamps_per_samp );
	Xil_Out32(UNDERSAMP_CTRL_UNDERSAMP_PERIOD_CREG,   usamp_cfg->samps_per_usamp );
	Xil_Out32(UNDERSAMP_CTRL_LAST_MUX_INDEX_CREG,     usamp_cfg->mux_sw_dly_mask );
	Xil_Out32(UNDERSAMP_CTRL_MUX_SW_DELAY_MASK_CREG,  usamp_cfg->last_mux_index  );

	return 0;
}
