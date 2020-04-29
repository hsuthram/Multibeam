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

#include "jo_AFE58JD18_chip_api.h"
#include "jo_AFE_interface_api.h"



int jo_AFE_interface_reset(void)
{
	Xil_Out32(AFE_ITF_SRESET_CREG , 0);    //print("preparing reset \n\r");
	Xil_Out32(AFE_ITF_SRESET_CREG , 1);    //print("reset in process \n\r");
	Xil_Out32(AFE_ITF_SRESET_CREG , 0);    //print("reset done \n\r");

	return(0);
}


int jo_AFE_interface_set_tx_pkg_size ( int pkg_size )
{
	Xil_Out32(AFE_ITF_AXI_STREAM_PKG_SIZE_CREG, pkg_size);

	return 0;
}



int jo_AFE_interface_configure (void)
{
 	Xil_Out32(AFE_ITF_SIM_DATA_OFFSET_CREG, AFE_ITF_SIM_DATA_OFFSET_DEFAULT);

    return(0);
}
