/*
 * jo_AFE_interface_api.h
 *
 *  Created on: Mar 29, 2019
 *      Author: jcarmona
 */

#ifndef SRC_JO_AFE_INTERFACE_API_H_
#define SRC_JO_AFE_INTERFACE_API_H_


// #include "xil_types.h"


// jo_AFE58JD18_interface (fpga fabric) -  Control Registers

#define AFE_ITF_SRESET_CREG               (XPAR_JO_AFE_TI_DESERFAB_0_BASEADDR + (0 * 0x4))
#define AFE_ITF_AXI_STREAM_PKG_SIZE_CREG  (XPAR_JO_AFE_TI_DESERFAB_0_BASEADDR + (1 * 0x4))
#define AFE_ITF_SIM_DATA_OFFSET_CREG      (XPAR_JO_AFE_TI_DESERFAB_0_BASEADDR + (2 * 0x4))


// jo_AFE58JD18_interface (fpga fabric) -  Monitoring registers

#define AFE_ITF_CURR_STATUS_MREG          (XPAR_JO_AFE_TI_DESERFAB_0_BASEADDR + (0 * 0x4))

// advanced macros

//#define SET_AFE_PKG_SIZE(a)               Xil_Out32(AFE_ITF_AXI_STREAM_PKG_SIZE_CREG, a);


// Global Variables prototypes

#define AFE_ITF_SIM_DATA_OFFSET_DEFAULT   0
#define AFE_ITF_NOT_CONFIGURED            0
#define AFE_ITF_CONFIGURED                1
#define AFE_CHIP_NOT_CONFIGURED           0
#define AFE_CHIP_CONFIGURED               1



extern int jo_AFE_interface_configure       (void);
extern int jo_AFE_interface_set_tx_pkg_size (int pkg_size);
extern int jo_AFE_interface_reset           (void);



#endif /* SRC_JO_AFE_INTERFACE_API_H_ */
