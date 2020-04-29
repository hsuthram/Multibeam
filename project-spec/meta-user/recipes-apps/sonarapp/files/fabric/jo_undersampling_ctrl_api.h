/*
 * jo_undersampling_ctrl_api.h.h
 *
 *  Created on: Apr, 22, 2019
 *      Author: jcarmona
 */

#ifndef SRC_JO_UNDERSAMPLING_CTRL_H_
#define SRC_JO_UNDERSAMPLING_CTRL_H_


//#include "xil_types.h"


// jo_undersamp_ctrl (fpga fabric) -  Control Registers

#define UNDERSAMP_CTRL_SRESET_CREG              ( XPAR_UNDERSAMPLING_CTRL_BASEADDR + (0 * 0x4) )
#define UNDERSAMP_CTRL_INTERSAMP_PERIOD_CREG    ( XPAR_UNDERSAMPLING_CTRL_BASEADDR + (1 * 0x4) )
#define UNDERSAMP_CTRL_UNDERSAMP_PERIOD_CREG    ( XPAR_UNDERSAMPLING_CTRL_BASEADDR + (2 * 0x4) )
#define UNDERSAMP_CTRL_LAST_MUX_INDEX_CREG      ( XPAR_UNDERSAMPLING_CTRL_BASEADDR + (3 * 0x4) )
#define UNDERSAMP_CTRL_MUX_SW_DELAY_MASK_CREG   ( XPAR_UNDERSAMPLING_CTRL_BASEADDR + (4 * 0x4) )


// jo_undersamp_ctrl (fpga fabric) -  Monitoring registers

#define UNDERSAMP_CTRL_CURR_STATUS_MREG                ( XPAR_JO_UNDERSAMPLING_CTRL_0_BASEADDR + (0 * 0x4) )


typedef struct
{
	u32 isamps_per_samp;                     // number of intersamples between switching
	u32 samps_per_usamp;                     // in number of intersamples
	u32 mux_sw_dly_mask;
	u32 last_mux_index;
}
jo_usamp_ctrl;


extern int jo_undersamp_ctrl_reset  (void);
extern int jo_undersamp_ctrl_cfg    (jo_usamp_ctrl *cfg_data);       // this command is never used as a stand alone. It is always sent as part (together with the) dig_acq_ctrl configuration command


#endif /* SRC_JO_AFE_INTERFACE_API_H_ */
