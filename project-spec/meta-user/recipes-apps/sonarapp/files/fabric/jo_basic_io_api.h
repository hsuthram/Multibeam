/*
 * test_routines.h
 *
 *  Created on: Jun 15, 2016
 *      Author: jcarmona
 */

#ifndef SRC_BASIC_IO_API_H_
#define SRC_BASIC_IO_API_H_


#define GPIO_MAIN_ID        XPAR_AXI_GPIO_0_DEVICE_ID

#define GPIO_OUTPUT_CH	    1

#define GPIO_BIT_SET        1
#define GPIO_BIT_RESET      0

#define GPIO_BIT_MASK__AFE_DIG_SPI_EN 1   // 0000 0000 0000 0001
#define GPIO_BIT_MASK__AFE_HW_RESET   2   // 0000 0000 0000 0010


extern XGpio Gpio_Main;
extern XGpio Gpio_Aux;
extern u8    dig_outputs_reg;
extern u8    dig_inputs_reg;


// Function Prototypes

extern int test_io             (void);

extern int get_data_from_UART  (void);

extern int init_gpios          (void);

extern int write_output_bits   ( u8 output_bits_mask, u8 action);
extern u8  read_input_switches (void);


#endif /* SRC_TEST_ROUTINES_H_ */
