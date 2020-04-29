/*
 * test_routines.h
 *
 *  Created on: Jun 15, 2016
 *      Author: jcarmona
 */

#ifndef SRC_JO_TX_EXC_ARB_PWM_API_H_
#define SRC_JO_TX_EXC_ARB_PWM_API_H_


//===================  Segment below is for PWM ARBITRARY


#define  TX_EXC_ARB_PWM_RESET_CREG                      XPAR_PING_PWM_BASEADDR + (0x0 * 4)
#define  TX_EXC_ARB_PWM_ENABLE_CREG                     XPAR_PING_PWM_BASEADDR + (0x1 * 4)
#define  TX_EXC_ARB_PWM_TRIGGER_CREG                    XPAR_PING_PWM_BASEADDR + (0x2 * 4)
#define  TX_EXC_ARB_PWM_CFG_TABLE_DEPTH_CREG            XPAR_PING_PWM_BASEADDR + (0x3 * 4)
#define  TX_EXC_ARB_PWM_CFG_TABLE_DATA_TO_WR_CREG       XPAR_PING_PWM_BASEADDR + (0x4 * 4)
#define  TX_EXC_ARB_PWM_CFG_TABLE_WR_CREG               XPAR_PING_PWM_BASEADDR + (0x5 * 4)
#define  TX_EXC_ARB_PWM_CFG_TABLE_RST_PTR_2RD_CMD_CREG  XPAR_PING_PWM_BASEADDR + (0x6 * 4)
#define  TX_EXC_ARB_PWM_CFG_TABLE_RST_PTR_2WR_CMD_CREG  XPAR_PING_PWM_BASEADDR + (0x7 * 4)



// Status Codes

#define TX_EXCITATION_PWM_HW_S_NOT_CONFIGURED   0
#define TX_EXCITATION_PWM_HW_S_CONFIGURED       1
#define TX_EXCITATION_PWM_HW_S_DISABLED         0
#define TX_EXCITATION_PWM_HW_S_ENABLED          1

#define TX_EXC_ARB_PWM_CFG_TABLE_MAX_SIZE       500    // DECIDE IF KEEPS


extern int jo_tx_pwm_arb_cfg  ( int arb_pwm_table_id    );


#endif /* SRC_JO_TX_EXC_ARB_PWM_API_H_ */
