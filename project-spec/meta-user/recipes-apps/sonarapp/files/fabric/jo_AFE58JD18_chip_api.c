/*
 * configure_AFE58JD18.c
 *
 *  Created on: Nov 8, 2017
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
// #include <xbasic_types.h>
// #include <xspips.h>
// #include "xil_types.h"


#include "jo_timing_tools.h"
#include "jo_basic_io_api.h"
#include "jo_AFE58JD18_chip_api.h"
#include "jo_spi_api.h"

       XSpiPs                        spi_AFE;
static jo_AFE58JD18_registers_state  afe_chip_registers;


//int jo_AFE_spi_setup(XSpiPs *SpiInstancePtr, u16 SpiDeviceId )
//{
//   /*
//	*  This routine sets up a SPI link, the SpiDeviceId is taken from the XPAR_XSPIPS_n_DEVICE_ID
//	*  it returns the handle to the SPI object, which is needed to operate it (write, read)
//	*
//	*/
//
//	int           status;
//	u32           config_word;
//	XSpiPs_Config *SpiConfig;
//
//
//	SpiConfig = XSpiPs_LookupConfig( SpiDeviceId );                                           // Get the configuration data for the SPI link
//
//			if (NULL == SpiConfig)       {  return XST_FAILURE;	}
//
//	status = XSpiPs_CfgInitialize( SpiInstancePtr, SpiConfig, SpiConfig->BaseAddress);        // Initialize the SPI driver so that it's ready to use. No configuration done here
//
//			if (status != XST_SUCCESS)   {	return XST_FAILURE; }
//
//	status = XSpiPs_SelfTest(SpiInstancePtr);                                                 // Perform a self-test to make sure the SPI hardware is in the system.  This must be done before the final configuration because it does modify some registers
//
//			if (status != XST_SUCCESS)   {	return XST_FAILURE; }
//
//                                                                                              // There is an "options table on xspips_options.h" it is defined with  the following fields (we compare the MASK fields with the XSPIPS_CR_OFFSET details of the Zynq manual pp 1740, they are the same)
//			                                                                                  // the OPTION fields are just indexes of this table.  A function in "xspips_options.h" translates teh OPTIONS of the table into bits of hte config reg using the MASKS of the table
//                                                                                              //
//                                                                                              // 	{XSPIPS_MASTER_OPTION,           XSPIPS_CR_MSTREN_MASK},    (to set it into master or slave mode)
//	                                                                                          //    {XSPIPS_CLK_ACTIVE_LOW_OPTION,   XSPIPS_CR_CPOL_MASK},      (Clock polarity 1: clock is quiescent high,  0: clock is quiescent low)
//	                                                                                          //    {XSPIPS_CLK_PHASE_1_OPTION,      XSPIPS_CR_CPHA_MASK},      (Clock/Data phase:  1 clock is inactive outside the word (clock rises with the data), (0 clock is active outside the word, clock rises in the middle of the data)
//	                                                                                          //    {XSPIPS_DECODE_SSELECT_OPTION,   XSPIPS_CR_SSDECEN_MASK},   (this is for the PERI_SEL option or peripheral select decode
//	                                                                                          //    {XSPIPS_FORCE_SSELECT_OPTION,    XSPIPS_CR_SSFORCE_MASK},   (this is for the manual CS mode, also known as Manual_CS on the Zynq documentation)
//	                                                                                          //    {XSPIPS_MANUAL_START_OPTION,     XSPIPS_CR_MANSTRTEN_MASK}
//
//    config_word = XSPIPS_MASTER_OPTION | XSPIPS_FORCE_SSELECT_OPTION | XSPIPS_CLK_ACTIVE_LOW_OPTION | XSPIPS_CLK_PHASE_1_OPTION;
//
//	XSpiPs_SetOptions(SpiInstancePtr,config_word);    // Here we are configuring the value of the register XSPIPS_CR_OFFSET (see zynq documentation page 1740)
//
//    status = XSpiPs_SetClkPrescaler(SpiInstancePtr,  /*XSPIPS_CLK_PRESCALE_256*/ XSPIPS_CLK_PRESCALE_64);                // check other prescaling factors on xspips.h
//
//    return(status);
//
//
//}



/*
*
*  This routine is used to write and/read the to/from the slave and it assumes that only the master drives the SCLK and that the communication is
*  full duplex. This routine is based on the Xilinx routine XSpiPs_PolledTransfer, which avoids dealing with interrupts
*
*  Transactions are done by serial bytes. i.e. the SCLK pulses will be sent in sets of multiples of 8.
*
*  It is assumed that the Slave will know when it is expected to send and receive data when the SCLK is driven by the master.  This should be part of the
*  upper layers of the communication protocol
*
*  Because the communication is always full duplex:
*
*        - The master puts its MOSI data together with the SCLK, and the slave will know what to do this data (ignore it or register it)

*        - As soon as the SCLK is received the slave will put its data on the MISO line, and the master will know what to do with it (ignore it or register)
*
*        - The data on the MISO and MOSI must have the same length as they are both riding on the SCLK line
*
*
*  In this routine:
*
*        - The data to write is specified in     *MOSI_buffPtr, This pointer cannot be NULL.  If the slave is not expected to receive anything (read only mode), then the slave should ignore the data coming in, which could be 0's
*
*        - The data read from the slave comes in *MISO_buffPtr, if the slave is not expected to reply with anything, then this pointer may be NULL
*
*
*  This routine assumes that the Slave it will do the transaction with has already been selected using the XSpiPs_SetSlaveSelect routine.
*
*
*/


//int jo_AFEchip_spi_debug(jo_AFE58JD18_chip *afe_chip)
//{
//	jo_AFE_chip_load_default_struct (afe_chip);
//	xil_printf("\n\r after loading afe_chip structure");
//	jo_AFE_chip_init ();
//	xil_printf("\n\r out of chip init");
//	jo_AFE_chip_configure(afe_chip);
//
//	return (0);
//}



int jo_AFE_chip_init(void)
{
	u8 buff_write_cmd[AFE_SPI_TX_BUF_WIDTH];                                   // This buffer is used to hold the value that needs to be written to AFE's reg 1 as part of initialization

    write_output_bits  ( GPIO_BIT_MASK__AFE_DIG_SPI_EN, GPIO_BIT_SET);         // The AFE has an SPI bus to get its registers programmed and read.  However, it has two sets of registers that can be accessed this way: the standard registers and the digital demodulator registers
                                                                               // It uses two select lines SEN (to select the standard registers) and SPI_DIG_EN (to select the Demodulator registers), the latter is active low logic
                                                                               // we are not going to use the demodulator, so the SPI_DIG_EN might as well be tied to VCC.
                                                                               // However, in the MBFE1 board does have this line as input and it is controlled via a GPIO line from the FPGA
                                                                               // Since the SPI_DIG_EN is active low, then we just SET the bit to cancel the demodulator spi.
                                                                               // Then we can use the SPI bust to program the rest of the registers (not associated to the DIG_DEMODULATOR)

                                                                               // initialize the SPI to communicate with the AFE chip;  The SPI used to communicate with the AFE has the address 0
	spi_setup (&spi_AFE, (0));                                                 // this sets up the SPI link.  This routine was taken from SDK examples.  See the document "Notes on the programming of the AFE"
    AFE_SPI_SELECT_SLAVE_MAIN;                                                 // call XSpiPs_SetSlaveSelect(&spi_AFE, 0) to select the spi slave

    write_output_bits  ( GPIO_BIT_MASK__AFE_DIG_SPI_EN, GPIO_BIT_SET);         // This is enable us to program the AFE's global registers, ADC registers, VCA registers and JESD registers.   Reseting it to 0 enables access to the Demodulator register map, but we are not using it.

	jo_AFE_chip_hardware_reset (AFE_HW_RST_LENGTH_DFLT);                       // This brings up the line connected to the AFE's reset. It must remain up for at least 100ns

	write_output_bits  ( GPIO_BIT_MASK__AFE_DIG_SPI_EN, GPIO_BIT_SET);         // This is enable us to program the AFE's global registers, ADC registers, VCA registers and JESD registers.   Reseting it to 0 enables access to the Demodulator register map, but we are not using it.

                                                                               // Write 0x0014 to AFE chip reg 1
	buff_write_cmd[0] = 1;                                                     // reg_num;
	buff_write_cmd[1] = 0;                                                     // Most  significant byte = 0x00
	buff_write_cmd[2] = 0x14;                                                  // Least significant byte = 0x14

    XSpiPs_PolledTransfer (&spi_AFE, &buff_write_cmd[0], NULL, AFE_SPI_TX_BUF_WIDTH);
                                                                               xil_printf("\n\r after sending basic config to Reg1 via spi ");
//  jo_wait(100000);
    jo_wait(1000);

    return (0);

}



int jo_AFE_chip_software_reset(u32 wait_time)
{
	u8 buff_array[AFE_SPI_TX_BUF_WIDTH];

	buff_array[0] = AFE_REG_0;                               // register number 0
	buff_array[1] = 0;
	buff_array[2] = afe_chip_registers.reg0 | 0x1;           // setting software reset

    XSpiPs_PolledTransfer (&spi_AFE, &buff_array[0], NULL, AFE_SPI_TX_BUF_WIDTH);

    jo_wait(wait_time);

	buff_array[2] = afe_chip_registers.reg0 & 0xFE;          // remove the reset signal

    XSpiPs_PolledTransfer (&spi_AFE, &buff_array[0], NULL, AFE_SPI_TX_BUF_WIDTH);

	return(0);
}



int jo_AFE_chip_hardware_reset(u32 wait_time)
{

	write_output_bits  ( GPIO_BIT_MASK__AFE_HW_RESET, GPIO_BIT_SET);         // This is enable us to program the AFE's global registers, ADC registers, VCA registers and JESD registers.   Reseting it to 0 enables access to the Demodulator register map, but we are not using it.

	jo_wait(wait_time);

	write_output_bits  ( GPIO_BIT_MASK__AFE_HW_RESET, GPIO_BIT_RESET);         // This is enable us to program the AFE's global registers, ADC registers, VCA registers and JESD registers.   Reseting it to 0 enables access to the Demodulator register map, but we are not using it.

    return(0);
}



int jo_AFE_chip_regs_struct_set_defaults(void)
{
	afe_chip_registers.reg0      = 0x0;                         // (b2)  ADD_OFFSET       = 0 : Normal operation;  R
	                                                            // (b1)  REG_READ_EN      = 0 : register_readout mode disabled;
	                                                            // (b0)  SOFTWARE_RESET   = 0

	afe_chip_registers.reg1      = 0x0014;                      // (b14) LVDS_RATE_2X     = 0 : LVDS rate normal operation
	                                                            // (b6)  EN_JESD          = 0 : JESD interface disabled,
	                                                            // (b5)  DIS_VLDS         = 0 : LVDS enabled
	                                                            // (b4-b1)                = A : by specs of chip
	                                                            // (b0) GLOBAL_PDN        = 0 : Normal mode operation (no power down)

	afe_chip_registers.reg2      = 0x1000;                      // (b12) LOW_LATENCY_EN   = 0 : enable low latency and bypass digital features
	                                                            // (b9-b7) PAT_MODES      = 0 : Normal output (NO PATTERN)

	afe_chip_registers.reg3      = 0x2010;                      // (b15-b13) SER_DATA_RATE = 001: LVDS 14 bits
	                                                            // (b12) DIG_GAIN_EN      = 0 : dig gain disabled
	                                                            // (b8)  DIG_FFSET_EN     = 0 : dig offset disabled
																// (b0)  EN_DEMOD         = 0 : demodulator disabled

	afe_chip_registers.reg4      = 0x0011;                      // (b8) PAT_SELECT_IND    = 0 : all outputs with same pattern
	                                                            // (b4) MSB_FIRST         = 1 : lvds output starts with MSB
	                                                            // (b1-b0) ADC_RES        = 01: ADC resolution 14 bits

	afe_chip_registers.reg5      = 0x0000;                      // (b15-b0) CUSTOM_PATTERN = 0 (no pattern)


	afe_chip_registers.reg195    = 0x2008;                      // (b13) PGA_GAIN         = 0 : set pga gain set to 24dB (when 1 -> 32dB)
    		                                                    // (b7) PGA_CLAMP_DIS     = 1 : disable current clamp (assume POW mode = 0)
    		                                                    // (b6) PGA_CLAMP_LVL     = 0 : ignored as PGA_CLAMP_DIS = 1
    		                                                    // (b4) PGA_HPF_DIS       = 0 : PGAhigh pass filter enabled (0-enablel; 1-disable)
    		                                                    // (b3-b0) LPF_PROG       = 8 : 10 MHz cutoff freq (but we are sampling at 5MHz... so not much help with the antialiasing filter here

	afe_chip_registers.reg196    = 0x2138;                      // (b15) LNA_GAIN_IND_EN  = 0 : LNA control is global for all channels
                                                                // (b14-b13) LNA_GAIN_GLBL= 01: LNA gain 24dB for all channels
                                                                // (b12) LNA_HPF_DIS      = 0 : LNA high pass filter enable
                                                                // (b10-9) INPUT_CLAMP_LVL= 0 : Autoset input clamp level
                                                                // (b8)  ACT_TERM_EN      = 1 : Enable
                                                                // (b7-6) GBL_ACT_TERM    = 0 : 50 ohms (but we have it disabled)
                                                                // (b5) ACT_TERM_IND_RES_EN = 1 : enable internal active termination individual resistor control enabled
                                                                // (b4-0) ACT_TERM_IND_RES = 18 : equivalent to 222 ohms when LNA is at 24dB

	afe_chip_registers.reg197    = 0x0200;                      // (b15) GLB_PDWN         = 0 : normal operation - no global power down
    		                                                    // (b14) FAST_PDWN        = 0 : normal operation - no fast power down
                                                                // (b13) PDWN_LNA         = 0 : normal operation - no powerdown on LND
																// (b12-11) POW_MODES     = 0 : normal operation - low noise mode
    		                                                    // (b9)  LOW_NF           = 1 : enable low noise figure mode
																// (b13) PDWN_LNA         = 0 : normal operation - no powerdown on LND

	afe_chip_registers.reg203    = 0x0080;                      // (b7) EN_DIG_TGC            = 0  : disable TGC attenuator
    		                                                    // (b6-4) DIG_TGC_ATTENUATION = 0  : 0-db attenuation
                                                                // (b3-2) LNA_HPF_PROG        = 01 : cutoff at 50KHz

	afe_chip_registers.reg205    = 0x4000;                      // v2i clamp ON
	return(0);
}



int jo_AFE_chip_regs_configure_all(void)
{
	                                                                  xil_printf("\n\r\n\r in jo_AFE_chip_configure");
	jo_AFE_chip_software_reset           (AFE_SW_RST_LENGTH_DFLT);    xil_printf("\n\r after software reset");
//  jo_AFE_chip_spi_enable_write_mode    ();                          xil_printf("\n\r after spi_enable write mode");          // Enable write mode for the next writes
    jo_AFE_chip_regs_struct_set_defaults ();                          xil_printf("\n\r after chip regs default loading");      // Enable write mode for the next writes

    jo_AFE_chip_spi_write_to_register (AFE_REG_1,     (u16)(afe_chip_registers.reg1  ),   AFE_SPIWRITE_DO_ENABLE );	xil_printf("\n\r just sent %x to the spi write to the reg 1  ",   (u16)(afe_chip_registers.reg1  ));
    jo_AFE_chip_spi_write_to_register (AFE_REG_2,     (u16)(afe_chip_registers.reg2  ),   AFE_SPIWRITE_DO_ENABLE );	xil_printf("\n\r just sent %x to the spi write to the reg 2  ",   (u16)(afe_chip_registers.reg2  ));
    jo_AFE_chip_spi_write_to_register (AFE_REG_3,     (u16)(afe_chip_registers.reg3  ),   AFE_SPIWRITE_DO_ENABLE );	xil_printf("\n\r just sent %x to the spi write to the reg 3  ",   (u16)(afe_chip_registers.reg3  ));
    jo_AFE_chip_spi_write_to_register (AFE_REG_4,     (u16)(afe_chip_registers.reg4  ),   AFE_SPIWRITE_DO_ENABLE );	xil_printf("\n\r just sent %x to the spi write to the reg 4  ",   (u16)(afe_chip_registers.reg4  ));
    jo_AFE_chip_spi_write_to_register (AFE_REG_5,     (u16)(afe_chip_registers.reg5  ),   AFE_SPIWRITE_DO_ENABLE );	xil_printf("\n\r just sent %x to the spi write to the reg 5  ",   (u16)(afe_chip_registers.reg5  ));
    jo_AFE_chip_spi_write_to_register (AFE_REG_195,   (u16)(afe_chip_registers.reg195),   AFE_SPIWRITE_DO_ENABLE );	xil_printf("\n\r just sent %x to the spi write to the reg 195",   (u16)(afe_chip_registers.reg195));
    jo_AFE_chip_spi_write_to_register (AFE_REG_196,   (u16)(afe_chip_registers.reg196),   AFE_SPIWRITE_DO_ENABLE );	xil_printf("\n\r just sent %x to the spi write to the reg 196",   (u16)(afe_chip_registers.reg196));
    jo_AFE_chip_spi_write_to_register (AFE_REG_197,   (u16)(afe_chip_registers.reg197),   AFE_SPIWRITE_DO_ENABLE );	xil_printf("\n\r just sent %xdto the spi write to the reg 197",   (u16)(afe_chip_registers.reg197));
    jo_AFE_chip_spi_write_to_register (AFE_REG_203,   (u16)(afe_chip_registers.reg203),   AFE_SPIWRITE_DO_ENABLE );	xil_printf("\n\r just sent %x to the spi write to the reg 203",   (u16)(afe_chip_registers.reg203));
    jo_AFE_chip_spi_write_to_register (AFE_REG_205,   (u16)(afe_chip_registers.reg205),   AFE_SPIWRITE_DO_ENABLE );	xil_printf("\n\r just sent %x to the spi write to the reg 205",   (u16)(afe_chip_registers.reg205));

 	                                                  xil_printf("\n\r after sending the spi write to the registers \n\r");

    u16 buff_input;
    jo_AFE_chip_spi_enable_read_out_mode ();          // Enable read mode for the next reads

    jo_AFE_chip_spi_read_from_register (AFE_REG_1,    &buff_input, AFE_SPIREAD_ALREADY_ENABLED );     xil_printf("\n\r AFE reg 1 %x",        buff_input);
    jo_AFE_chip_spi_read_from_register (AFE_REG_2,    &buff_input, AFE_SPIREAD_ALREADY_ENABLED );     xil_printf("\n\r AFE reg 2 %x",        buff_input);
    jo_AFE_chip_spi_read_from_register (AFE_REG_3,    &buff_input, AFE_SPIREAD_ALREADY_ENABLED );     xil_printf("\n\r AFE reg 3 %x",        buff_input);
    jo_AFE_chip_spi_read_from_register (AFE_REG_4,    &buff_input, AFE_SPIREAD_ALREADY_ENABLED );     xil_printf("\n\r AFE reg 4 %x \n\r",   buff_input);
    jo_AFE_chip_spi_read_from_register (AFE_REG_5,    &buff_input, AFE_SPIREAD_ALREADY_ENABLED );     xil_printf("\n\r AFE reg 5 %x \n\r",   buff_input);
    jo_AFE_chip_spi_read_from_register (AFE_REG_195,  &buff_input, AFE_SPIREAD_ALREADY_ENABLED );     xil_printf("\n\r AFE reg 195 %x \n\r", buff_input);
    jo_AFE_chip_spi_read_from_register (AFE_REG_196,  &buff_input, AFE_SPIREAD_ALREADY_ENABLED );     xil_printf("\n\r AFE reg 196 %x \n\r", buff_input);
    jo_AFE_chip_spi_read_from_register (AFE_REG_197,  &buff_input, AFE_SPIREAD_ALREADY_ENABLED );     xil_printf("\n\r AFE reg 197 %x \n\r", buff_input);
    jo_AFE_chip_spi_read_from_register (AFE_REG_203,  &buff_input, AFE_SPIREAD_ALREADY_ENABLED );     xil_printf("\n\r AFE reg 203 %x \n\r", buff_input);
    jo_AFE_chip_spi_read_from_register (AFE_REG_205,  &buff_input, AFE_SPIREAD_ALREADY_ENABLED );     xil_printf("\n\r AFE reg 205 %x \n\r", buff_input);

    return(0);
}


int jo_AFE_chip_spi_enable_read_out_mode(void)      // Routine that sends a command via the SPI to the AFE to enable read mode.  In this mode, the AFE will ignore the DATA and pay attention only to the REG# on the SPI packets sent to it afterwards
{
	u8 buff_array[AFE_SPI_TX_BUF_WIDTH];

	buff_array[0] = AFE_REG_0;                              // register number 0
	buff_array[1] = 0;
	buff_array[2] = afe_chip_registers.reg0 | 0x2;          // value to set the read out mode

    XSpiPs_PolledTransfer (&spi_AFE, &buff_array[0], NULL, AFE_SPI_TX_BUF_WIDTH);

	return(0);
}



int jo_AFE_chip_spi_enable_write_mode(void)         // Routine that sends a command via the SPI to the AFE to enable write mode
{
	u8 buff_array[AFE_SPI_TX_BUF_WIDTH];

	buff_array[0] = AFE_REG_0;                              // register number 0
	buff_array[1] = 0;
	buff_array[2] = afe_chip_registers.reg0 & 0xFD;         // value to set the write mode

    XSpiPs_PolledTransfer (&spi_AFE, &buff_array[0], NULL, AFE_SPI_TX_BUF_WIDTH);

	return(0);
}



int jo_AFE_chip_spi_read_from_register(u8 reg_num, u16 *val_read, u8 need_to_enable_read_out_mode)
{
	u8   buff_write_cmd [AFE_SPI_TX_BUF_WIDTH];
	u8   buff_read      [AFE_SPI_RX_BUF_WIDTH];

	if (need_to_enable_read_out_mode)
	{
		buff_write_cmd[0] = AFE_REG_0;		                       // register to enable readout mode
		buff_write_cmd[1] = 0;
		buff_write_cmd[2] = afe_chip_registers.reg0 | 0x2;         // value to set the read out mode

		XSpiPs_PolledTransfer (&spi_AFE, &buff_write_cmd[0], NULL, AFE_SPI_TX_BUF_WIDTH);
	}

	buff_write_cmd[0] = reg_num;				                   // reg_num
	buff_write_cmd[1] = 0;
	buff_write_cmd[2] = 0;                                         // send no data

    XSpiPs_PolledTransfer (&spi_AFE, &buff_write_cmd[0], &buff_read[0], AFE_SPI_TX_BUF_WIDTH);

	*val_read   = buff_read[1] << 8 | buff_read[2];

	return(0);
}



int jo_AFE_chip_spi_write_to_register(u8 reg_num, u16 val_to_write, u8 need_to_enable_write_mode)
{

	u8 buff_write_cmd[AFE_SPI_TX_BUF_WIDTH];

	if (need_to_enable_write_mode)
	{
		buff_write_cmd[0] = AFE_REG_0;                        // register to enable readout mode
		buff_write_cmd[1] = 0;
		buff_write_cmd[2] = afe_chip_registers.reg0 & 0xFD;   // value to set the write mode

	    XSpiPs_PolledTransfer (&spi_AFE, &buff_write_cmd[0], NULL, AFE_SPI_TX_BUF_WIDTH);
	}

    xil_printf("\n\r About to write AFE reg %d with value %d, with value %x", reg_num, val_to_write, val_to_write);

	buff_write_cmd[0] = reg_num;
	buff_write_cmd[1] = val_to_write >> 8;                    // assigning the most significant byte of val_to_write byte 1
	buff_write_cmd[2] = val_to_write & 0xFF;                  // assigning the least significant byte of val_to_write byte 2

    XSpiPs_PolledTransfer (&spi_AFE, &buff_write_cmd[0], NULL, AFE_SPI_TX_BUF_WIDTH);

	return(0);
}



int jo_AFE_chip_TI_debug_set_all_to_zero (void)  // only for debug purposes -- remove after system has been deemed stable and functioning
{
	u16 reg4_data;

//	jo_AFE_chip_spi_read_from_register(04,     &reg4_data, AFE_SPIREAD_DO_ENABLE);

//  jo_AFE_chip_spi_write_to_register (04,     (reg4_data | (u16)(0x0100)), AFE_SPIWRITE_DO_ENABLE );  // enable pat_select_ind

    jo_AFE_chip_spi_write_to_register (21,     (u16)(0x0D80),   AFE_SPIWRITE_DO_ENABLE );  // ch 1,  ch 2
    jo_AFE_chip_spi_write_to_register (33,     (u16)(0x0D80),   AFE_SPIWRITE_DO_ENABLE );  // ch 5,  ch 6
    jo_AFE_chip_spi_write_to_register (45,     (u16)(0x0D80),   AFE_SPIWRITE_DO_ENABLE );  // ch 9,  ch 10
    jo_AFE_chip_spi_write_to_register (57,     (u16)(0x0D80),   AFE_SPIWRITE_DO_ENABLE );  // ch 13, ch 14

    jo_AFE_chip_spi_write_to_register (23,     (u16)(0x00D8),   AFE_SPIWRITE_DO_ENABLE );  // ch 3,  ch 4
    jo_AFE_chip_spi_write_to_register (35,     (u16)(0x00D8),   AFE_SPIWRITE_DO_ENABLE );  // ch 7,  ch 8
    jo_AFE_chip_spi_write_to_register (47,     (u16)(0x00D8),   AFE_SPIWRITE_DO_ENABLE );  // ch 11, ch 12
    jo_AFE_chip_spi_write_to_register (59,     (u16)(0x00D8),   AFE_SPIWRITE_DO_ENABLE );  // ch 15, ch 16

    return (0);
}

int jo_AFE_chip_TI_initialization()  // not used, this is from TI's files
{
	jo_AFE_chip_spi_write_to_register (0,   (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (1,   (20 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (2,   (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (3,   (32784 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (4,   (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (5,   (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (7,   (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (11,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (13,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (15,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (17,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (19,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (21,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (23,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (24,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (25,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (27,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (29,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (31,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (33,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (35,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (36,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (37,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (39,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (41,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (43,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (45,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (47,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (48,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (49,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (51,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (53,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (55,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (57,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (59,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (60,  (0 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (65,  (2 ),   AFE_SPIWRITE_DO_ENABLE );
	jo_AFE_chip_spi_write_to_register (67,  (0 ),   AFE_SPIWRITE_DO_ENABLE );

	return(0);
}
