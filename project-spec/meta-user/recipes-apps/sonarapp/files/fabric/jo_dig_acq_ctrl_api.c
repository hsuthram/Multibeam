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

#include "jo_timing_tools.h"
#include "jo_dig_acq_ctrl_api.h"




int jo_dacq_session_cfg ( u32 samp_period_cnts)                            // Routine for the basic intial setup of the jo_dig_acq_ctrl block.
{                                                                          // sampling period cnts are give

	DIG_CTRL_STOP_PINGS ;                                                  // This is to "reset" the acq_run input to the dig_acq_ctrl module so that its state machine gets back to reset state

	Xil_Out32( DIG_CTRL_CFG_CREG,                 0x01);                   // here we put the jo_dig_acq_ctrl block in configuration mode
	Xil_Out32( DIG_CTRL_PING_MODE_CREG,           1               );       // set the ping mode as ACTIVE (i.e. the unit itself generates the ping trigger);  WE NEVER USE IT IN PASSIVE MODE
	Xil_Out32( DIG_CTRL_SAMPLE_PERIOD_CNTS_CREG,  samp_period_cnts);       // program sampling period counts into the module
	Xil_Out32( DIG_CTRL_CFG_CREG,                 0x00);                   // here we move the module out of configuration mode into normal mode

    DIG_CTRL_PING_OUTPUT_ENABLE;                                           // enable pings

    DIG_CTRL_ENABLE_SAMP_SIGNAL;                                           // enable sample signal, which is necessary for the operation of the AFE

	return 0;
}



int jo_dacq_cfg_ping ( jo_dacq_ping_cfg  *dig_acq_ctrl_ping_cfg )                              // Quick configuration of only the parameters relatd to the ping
{
	Xil_Out32(DIG_CTRL_CFG_CREG,                   0x01);

	Xil_Out32(DIG_CTRL_PING_PERIOD_CNTS_CREG,      dig_acq_ctrl_ping_cfg->period_cnts    );
	Xil_Out32(DIG_CTRL_ACQ_WINDOW_START_CNTS_CREG, dig_acq_ctrl_ping_cfg->start_cnts );
	Xil_Out32(DIG_CTRL_ACQ_WINDOW_STOP_CNTS_CREG,  dig_acq_ctrl_ping_cfg->stop_cnts  );
	Xil_Out32(DIG_CTRL_PINGS_CNT_N_CREG,           dig_acq_ctrl_ping_cfg->iterations     );

	Xil_Out32(DIG_CTRL_CFG_CREG,                   0x00);

	return 0;
}



int jo_dacq_is_it_idle(void)
{
	return( Xil_In32(DIG_CTRL_CURRENT_STATUS_MREG) == DIG_CTRL_CURR_STATUS_CODE_IDL);

}


int jo_dacq_read_regs( jo_dacq_monit_regs   *dig_acq_state )
{
	dig_acq_state->curr_status     = Xil_In32(DIG_CTRL_CURRENT_STATUS_MREG);
	dig_acq_state->curr_ping       = Xil_In32(DIG_CTRL_CURRENT_PING_MREG);
	dig_acq_state->hw_presetup_rdy = Xil_In32(DIG_CTRL_SETUP_READY_MREG);

	return 0;
}
