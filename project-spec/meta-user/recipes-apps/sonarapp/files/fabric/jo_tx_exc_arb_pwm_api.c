/*
 * test_routines.c
 *
 *  Created on: Jun 15, 2016
 *      Author: jcarmona
 */

#include "jo_tx_exc_arb_pwm_api.h"

// #include <xparameters.h>
// #include <xil_types.h>
// #include <xgpio.h>
// #include <xstatus.h>
// #include <xuartps.h>
// #include <string.h>
// #include <xil_cache.h>
// #include <xil_printf.h>
//



int jo_tx_pwm_arb_cfg  ( int arb_pwm_table_id )
{

    u32 chirp_table_1[150] = {	24836,	24836,	24837,	24837,	24837,	24582,	24840,	24584,	24842,	24587,	24844,	24588,	24846,	24590,	24848,
								24592,	24593,	24595,	24852,	24596,	24597,	24854,	24342,	24856,	24344,	24859,	24346,	24860,	24348,	24606,
								24863,	24350,	24608,	24608,	24610,	24610,	24354,	24612,	24612,	24356,	24614,	24614,	24358,	24616,	24616,
								24360,	24618,	24618,	24618,	24105,	24620,	24620,	24364,	24620,	24364,	24365,	24622,	24365,	24622,	24365,
								24110,	24624,	24368,	24624,	24367,	24367,	24368,	24624,	24367,	24367,	24367,	24367,	24368,	24368,	24368,
								24368,	24367,	24367,	24367,	24367,	24111,	24368,	24367,	24367,	24111,	24368,	24367,	24367,	24365,	24109,
								24366,	24109,	24365,	24365,	24363,	24107,	24107,	24107,	24620,	24105,	24105,	24105,	24617,	24103,	24103,
								24359,	24101,	24101,	24357,	24099,	24099,	24355,	24097,	23840,	24353,	24095,	24095,	24095,	24093,	24349,
								23835,	24091,	24347,	23832,	24089,	24088,	24087,	24086,	23829,	24085,	24084,	24083,	24081,	24080,	23823,
								23823,	24335,	23820,	24076,	23818,	24074,	23817,	24073,	23815,	23814,	24070,	23812,	23812,	23556,	16389    };

	int i;
	u32 vector_length_1 = 150;

	if ( arb_pwm_table_id == 1)
	{
		Xil_Out32(TX_EXC_ARB_PWM_CFG_TABLE_DEPTH_CREG,           0   );    jo_wait(10);                              // reset table depth to 0
		Xil_Out32(TX_EXC_ARB_PWM_CFG_TABLE_RST_PTR_2WR_CMD_CREG, 1   );    jo_wait(10);
		Xil_Out32(TX_EXC_ARB_PWM_CFG_TABLE_RST_PTR_2WR_CMD_CREG, 0   );    jo_wait(10);

		for (i=0; i < tx_exc_pwm_arb->vector_length; i++)
		{
			Xil_Out32(TX_EXC_ARB_PWM_CFG_TABLE_DATA_TO_WR_CREG,  chirp_table_1[i] ); jo_wait(10);
			Xil_Out32(TX_EXC_ARB_PWM_CFG_TABLE_WR_CREG,          0x01             ); jo_wait(10);  // Need to push the data point
			Xil_Out32(TX_EXC_ARB_PWM_CFG_TABLE_WR_CREG,          0x00             ); jo_wait(10);
		}

		Xil_Out32(TX_EXC_ARB_PWM_CFG_TABLE_DEPTH_CREG,           vector_length_1  ); jo_wait(10);                        // set table depth in index format
		Xil_Out32(TX_EXC_ARB_PWM_CFG_TABLE_RST_PTR_2RD_CMD_CREG, 1                ); jo_wait(10);
		Xil_Out32(TX_EXC_ARB_PWM_CFG_TABLE_RST_PTR_2RD_CMD_CREG, 0                ); jo_wait(10);

		jo_wait(20);

		Xil_Out32(TX_EXC_ARB_PWM_ENABLE_CREG, 1 );   jo_wait(10);

		return 0;
	}
	else
	{
		return -1;
	}

}
