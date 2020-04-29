/*
 * test_routines.c
 *
 *  Created on: Jun 15, 2016
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



int jo_wait (u32 wait_time)
{
	volatile int i;

	i = 0;
	while (i < wait_time)	i++;

	return (0);
}
