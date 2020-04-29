/*
 * test_routines.c
 *
 *  Created on: Jun 15, 2016
 *      Author: jcarmona
 */





// 
// #include <xparameters.h>
// #include <xil_types.h>
// #include <xgpio.h>
// #include <xstatus.h>
// #include <xuartps.h>
// #include <string.h>
// #include <xil_cache.h>
// #include <xil_printf.h>
// #include <xscugic.h>

#include "jo_basic_io_api.h"

XGpio Gpio_Main;
XGpio Gpio_Aux;

u8    dig_outputs_reg, dig_inputs_reg;


int test_io (void)
{

  u32 status;

//  xil_printf("%c[2J",27);														// Clear the screen

  status = XGpio_Initialize(&Gpio_Main, GPIO_MAIN_ID);							// Initialize the Leds and Switches GPIO driver so that it's ready to use,
  if (status != XST_SUCCESS) return  XST_FAILURE;

 /* only for ZED
  u32 DataRead;
  u32 OldData;
  XGpio_SetDataDirection(&Gpio_Main,  GPIO_DIPSW_CH,   0xFFFFFFFF);		    	// Set the direction for the Dipswitches gpio to be inputs
  XGpio_SetDataDirection(&Gpio_Main,  GPIO_OUTPUT_CH,  0x0);    				// Set the direction for the LEDs gpio to be ouptuts

  OldData      = 0xFFFFFFFF;

  while(OldData != 0x0)
  {
    DataRead = XGpio_DiscreteRead(&Gpio_Main, GPIO_DIPSW_CH);   				// Read the state of the DIP switches
    XGpio_DiscreteWrite(&Gpio_Main, GPIO_OUTPUT_CH, DataRead);				// Set the GPIO outputs to the DIP switch values

    if(DataRead != OldData)													    // Send the data to the UART if the settings change
    {
      xil_printf("\n\r DIP Switch settings: 0x%X",   DataRead);

      OldData = DataRead;														// Record the DIP switch settings
    }
  }
*/
  return 0;
}


int init_gpios(void)
{
	int status;

	  status = XGpio_Initialize(&Gpio_Main, GPIO_MAIN_ID);							// Initialize the Leds and Switches GPIO driver so that it's ready to use,
	  if (status != XST_SUCCESS) return  XST_FAILURE;

/* only for ZED	  XGpio_SetDataDirection(&Gpio_Main,  GPIO_DIPSW_CH,   0xFFFFFFFF);		    	// Set the direction for the Dipswitches gpio to be inputs */
	  XGpio_SetDataDirection(&Gpio_Main,  GPIO_OUTPUT_CH,  0x0);    				// Set the direction for the LEDs gpio to be ouptuts

	  dig_outputs_reg = 0;

	  return(status);
}


u8 read_input_switches (void)
{
/* only for ZED   return XGpio_DiscreteRead(&Gpio_Main, GPIO_DIPSW_CH); */
  return (0);

}


int write_output_bits( u8 output_bits_mask, u8 action)
{
	if (action == GPIO_BIT_SET)                                       // set bits in mask
	{
		dig_outputs_reg = dig_outputs_reg | output_bits_mask;
	}
	else
	{
		dig_outputs_reg = dig_outputs_reg & (~output_bits_mask);
	}

    XGpio_DiscreteWrite(&Gpio_Main, GPIO_OUTPUT_CH, dig_outputs_reg);				// Set the GPIO outputs to the DIP switch values

    return(0);

}

//-----------------------------------

int get_data_from_UART(void)
{
	return (XUartPs_RecvByte(XPS_UART1_BASEADDR));
}
