/*
 * interrupts.h
 *
 *  Created on: Nov 15, 2016
 *      Author: jcarmona
 */

#ifndef SRC_INTERRUPTS_H_
#define SRC_INTERRUPTS_H_



#define INTC_DEVICE_ID          XPAR_SCUGIC_SINGLE_DEVICE_ID

#define RESET_TIMEOUT_COUNTER	10000

#define INTC_INIT_SUCCESS       0
#define INTC_LOOKUP_CFG_FAIL    1
#define INTC_INIT_FAIL          2


extern XScuGic  Int_Ctrl;
extern int      jo_interrupts_controller_setup_byPointer (XScuGic *interrupt_ctrl);
extern void     jo_interrupt_controller_setup_byAddr(void);


#endif /* SRC_INTERRUPTS_H_ */
