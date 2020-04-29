/*
 * jo_dig_acq_ctrl_api.h
 *
 *  Created on: Jul 9, 2018
 *      Author: jcarmona
 */

#ifndef SRC_JO_DIG_ACQ_CTRL_API_H_
#define SRC_JO_DIG_ACQ_CTRL_API_H_




// DIgital Control Module Write-to Registers

#define DIG_CTRL_RESET_SM_CREG              ((XPAR_JO_DIG_ACQ_CTRL_0_BASEADDR) + (0x0 * 4))
#define DIG_CTRL_SAMP_SIGNAL_ENA_CREG       ((XPAR_JO_DIG_ACQ_CTRL_0_BASEADDR) + (0x1 * 4))
#define DIG_CTRL_CFG_CREG                   ((XPAR_JO_DIG_ACQ_CTRL_0_BASEADDR) + (0x2 * 4))
#define DIG_CTRL_ACQ_RUN_CREG               ((XPAR_JO_DIG_ACQ_CTRL_0_BASEADDR) + (0x3 * 4))
#define DIG_CTRL_PING_OUTPUT_CREG           ((XPAR_JO_DIG_ACQ_CTRL_0_BASEADDR) + (0x4 * 4))          // 1: ping command is sent to PWM control to generate excitation signal.... 0: ping command is not sent, this will only produce a "blank" ping
#define DIG_CTRL_PING_MODE_CREG             ((XPAR_JO_DIG_ACQ_CTRL_0_BASEADDR) + (0x5 * 4))          // 1: Active,   0: Passive (externally triggered ping)
#define DIG_CTRL_SAMPLE_PERIOD_CNTS_CREG    ((XPAR_JO_DIG_ACQ_CTRL_0_BASEADDR) + (0x6 * 4))          // in increments of the FPGA clock period (10ns)
#define DIG_CTRL_PING_PERIOD_CNTS_CREG      ((XPAR_JO_DIG_ACQ_CTRL_0_BASEADDR) + (0x7 * 4))          // in number of samples
#define DIG_CTRL_ACQ_WINDOW_START_CNTS_CREG ((XPAR_JO_DIG_ACQ_CTRL_0_BASEADDR) + (0x8 * 4))          // in number of samples
#define DIG_CTRL_ACQ_WINDOW_STOP_CNTS_CREG  ((XPAR_JO_DIG_ACQ_CTRL_0_BASEADDR) + (0x9 * 4))          // in number of samples
#define DIG_CTRL_PINGS_CNT_N_CREG           ((XPAR_JO_DIG_ACQ_CTRL_0_BASEADDR) + (0xa * 4))

#define DIG_CTRL_CURRENT_STATUS_MREG        ((XPAR_JO_DIG_ACQ_CTRL_0_BASEADDR) + (0x0 * 4))          // state of the dig_acq_ctrl module's state machine
#define DIG_CTRL_CURRENT_PING_MREG          ((XPAR_JO_DIG_ACQ_CTRL_0_BASEADDR) + (0x1 * 4))
#define DIG_CTRL_SETUP_READY_MREG           ((XPAR_JO_DIG_ACQ_CTRL_0_BASEADDR) + (0x2 * 4))          // flag from external hardware indicating it is ready to operate

#define DIG_CTRL_RESET_MODULE               (Xil_Out32((DIG_CTRL_RESET_SM_CREG),        0x01))
#define DIG_CTRL_ENABLE_SAMP_SIGNAL         (Xil_Out32((DIG_CTRL_SAMP_SIGNAL_ENA_CREG), 0x01))
#define DIG_CTRL_DISABLE_SAMP_SIGNAL        (Xil_Out32((DIG_CTRL_SAMP_SIGNAL_ENA_CREG), 0x00))
#define DIG_CTRL_PING_OUTPUT_ENABLE         (Xil_Out32((DIG_CTRL_PING_OUTPUT_CREG),     0x01))
#define DIG_CTRL_PING_OUTPUT_DISABLE        (Xil_Out32((DIG_CTRL_PING_OUTPUT_CREG),     0x00))
#define DIG_CTRL_RUN_PINGS                  (Xil_Out32((DIG_CTRL_ACQ_RUN_CREG),         0x01))
#define DIG_CTRL_STOP_PINGS                 (Xil_Out32((DIG_CTRL_ACQ_RUN_CREG),         0x00))


#define DIG_CTRL_CURR_STATUS_CODE                0    // Reset
#define DIG_CTRL_CURR_STATUS_CODE_IDL            1    // Idle
#define DIG_CTRL_CURR_STATUS_CODE_CFG            2    // Config state
#define DIG_CTRL_CURR_STATUS_CODE_ACQ_INI        3    // Initialize acquisition timers
#define DIG_CTRL_CURR_STATUS_CODE_ACQ_W2START    4    // wait for acq window to begin
#define DIG_CTRL_CURR_STATUS_CODE_ACQ_INWND      5    // acq in process (within window)
#define DIG_CTRL_CURR_STATUS_CODE_ACQ_OUTWND     6    // acq out of window (waiting for sampling signal to expire
#define DIG_CTRL_CURR_STATUS_CODE_ACQ_PNGDONE    7    // acq done for current ping
#define DIG_CTRL_CURR_STATUS_CODE_ACQ_DONE       8    // acq done for the whole set of pings
#define DIG_CTRL_CURR_STATUS_CODE_WET            9    // Waiting for external trigger;
#define DIG_CTRL_CURR_STATUS_CODE_ACQ_WAIT2PING  10   // wait to sync ping with samplig signal rising edge


#define DIG_CTRL_RST_LEN_CPU_CLKS           100

                                                      // status codes
#define DIG_CTRL_HW_S_NOT_CONFIGURED          0
#define DIG_CTRL_HW_S_CONFIGURED              1
#define DIG_CTRL_HW_S_SAMP_SIGNAL_DISABLED    0
#define DIG_CTRL_HW_S_SAMP_SIGNAL_ENABLED     1


//  Structures



typedef struct
{
	int  start_cnts;
	int  stop_cnts;
	int  period_cnts;
	int  iterations;
}
jo_dacq_ping_cfg;



typedef struct
{
	int  curr_status;
    int  curr_ping;
	int  hw_presetup_rdy;
}
 jo_dacq_monit_regs;



  // Global Variables prototypes



  // Function Prototypes


    extern int jo_dacq_session_cfg  ( u32 samp_period_cnts);                                    // Routine for the basic intial setup of the jo_dig_acq_ctrl block.
	extern int jo_dacq_cfg_ping     ( jo_dacq_ping_cfg   *dig_acq_ctrl_ping_cfg );
	extern int jo_dacq_is_it_idle   ( void);
	extern int jo_dacq_read_regs    ( jo_dacq_monit_regs *dig_acq_state );

#endif /* SRC_JO_DIG_ACQ_CTRL_API_H_ */
