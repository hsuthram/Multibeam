/*
 * spi_interfacing_routines.c
 *
 *  Created on: Nov 8, 2017
 *      Author: jcarmona
 */

/*
 *   This file deals with the routines that an SPI master needs to communicate with its slavs
 *
 *   In the Zynq, an SPI master can have upto 3 slaves.  The Zynq may be configured with 3 SPIs {0,1,2}
 *
 *   Lines of a SPI master
 *
 *   			SCLK                            one clock line going to all slaves
 *   			SLAVE_SEL, SDATA_IN, SDATA_OUT  each slave has these three lines
 *
 *
 *   The main configuration routines are in xspips_options and the Configuration codes are in xspips_hw.h
 *
 */



#include "xparameters.h"
#include "xspips.h"
#include "xil_types.h"
#include "jo_spi_api.h"



XSpiPs spi_0;


int spi_setup(XSpiPs *SpiInstancePtr, u16 SpiDeviceId )
{
   /*
	*  This routine sets up a SPI link, the SpiDeviceId is taken from the XPAR_XSPIPS_n_DEVICE_ID
	*  it returns the handle to the SPI object, which is needed to operate it (write, read)
	*
	*/

	int           status;
	u32           config_word;
	XSpiPs_Config *SpiConfig;


	SpiConfig = XSpiPs_LookupConfig( SpiDeviceId );                                           // Get the configuration data for the SPI link

			if (NULL == SpiConfig)       {  return XST_FAILURE;	}

	status = XSpiPs_CfgInitialize( SpiInstancePtr, SpiConfig, SpiConfig->BaseAddress);        // Initialize the SPI driver so that it's ready to use. No configuration done here

			if (status != XST_SUCCESS)   {	return XST_FAILURE; }

	status = XSpiPs_SelfTest(SpiInstancePtr);                                                 // Perform a self-test to make sure the SPI hardware is in the system.  This must be done before the final configuration because it does modify some registers

			if (status != XST_SUCCESS)   {	return XST_FAILURE; }

                                                                                              // There is an "options table on xspips_options.h" it is defined with  the following fields (we compare the MASK fields with the XSPIPS_CR_OFFSET details of the Zynq manual pp 1740, they are the same)
			                                                                                  // the OPTION fields are just indexes of this table.  A function in "xspips_options.h" translates teh OPTIONS of the table into bits of hte config reg using the MASKS of the table
                                                                                              //
                                                                                              // 	{XSPIPS_MASTER_OPTION,           XSPIPS_CR_MSTREN_MASK},    (to set it into master or slave mode)
	                                                                                          //    {XSPIPS_CLK_ACTIVE_LOW_OPTION,   XSPIPS_CR_CPOL_MASK},      (Clock polarity 1: clock is quiescent high,  0: clock is quiescent low)
	                                                                                          //    {XSPIPS_CLK_PHASE_1_OPTION,      XSPIPS_CR_CPHA_MASK},      (Clock/Data phase:  1 clock is inactive outside the word (clock rises with the data), (0 clock is active outside the word, clock rises in the middle of the data)
	                                                                                          //    {XSPIPS_DECODE_SSELECT_OPTION,   XSPIPS_CR_SSDECEN_MASK},   (this is for the PERI_SEL option or peripheral select decode
	                                                                                          //    {XSPIPS_FORCE_SSELECT_OPTION,    XSPIPS_CR_SSFORCE_MASK},   (this is for the manual CS mode, also known as Manual_CS on the Zynq documentation)
	                                                                                          //    {XSPIPS_MANUAL_START_OPTION,     XSPIPS_CR_MANSTRTEN_MASK}

    config_word = XSPIPS_MASTER_OPTION | XSPIPS_FORCE_SSELECT_OPTION | XSPIPS_CLK_ACTIVE_LOW_OPTION | XSPIPS_CLK_PHASE_1_OPTION;

	XSpiPs_SetOptions(SpiInstancePtr,config_word);    // Here we are configuring the value of the register XSPIPS_CR_OFFSET (see zynq documentation page 1740)

    status = XSpiPs_SetClkPrescaler(SpiInstancePtr,  /*XSPIPS_CLK_PRESCALE_256*/ XSPIPS_CLK_PRESCALE_64);                // check other prescaling factors on xspips.h

    return(status);


}



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


int spi_transaction_polled_mode (XSpiPs *SpiPtr, u8 SlaveNum, u8 *MOSI_buffPtr, u8 *MISO_buffPtr, u32 number_of_bytes)
{
	u32 StatusReg;
	u32 ConfigReg;
	u32 bytes_recvd;
	u32 CheckTransfer;
	s32 Status_Polled;                // this is the main output or return of this function
	u8 TempData;

	/*
	 * Checking of the input parameters The MISO_buffPtr argument can be NULL.
	 */
	Xil_AssertNonvoid( SpiPtr          != NULL);
	Xil_AssertNonvoid( MOSI_buffPtr    != NULL);
	Xil_AssertNonvoid( number_of_bytes       > 0U);
	Xil_AssertNonvoid( SpiPtr->IsReady == XIL_COMPONENT_IS_READY);

	/*
	 * Check whether there is another transfer in progress. Not thread-safe.
	 */
	if (SpiPtr->IsBusy == TRUE)
	{
		Status_Polled = (s32)XST_DEVICE_BUSY;
	}
	else
	{
		SpiPtr->IsBusy = TRUE;                                     // Set the busy flag, which will be cleared when the transfer is entirely done.

		                                                           // Set up buffer pointers.
		SpiPtr->SendBufferPtr  = MOSI_buffPtr;                     // Here we connect the spi send buffer to the actual buffer where the data to send has been stored
		SpiPtr->RecvBufferPtr  = MISO_buffPtr;                     // Here we connect the spi receive buffer to the location where the data will be loaded

		SpiPtr->RequestedBytes = number_of_bytes;                  // Number of bytes to transfer
		SpiPtr->RemainingBytes = number_of_bytes;                  // Number of bytes left to transfer (the same as we have not started the transmission)

                                                                   // This block is to raise the SlaveSelect line for the SPI (if Manual mode)
	    if (XSpiPs_IsManualChipSelect(SpiPtr) == TRUE)
	    {
			ConfigReg = XSpiPs_ReadReg( SpiPtr->Config.BaseAddress, XSPIPS_CR_OFFSET);

			ConfigReg &= (u32)(~XSPIPS_CR_SSCTRL_MASK);
			ConfigReg |= SpiPtr->SlaveSelect;                      // Set the slave select value in the config reg

			XSpiPs_WriteReg(SpiPtr->Config.BaseAddress, XSPIPS_CR_OFFSET, ConfigReg);
		}

		XSpiPs_Enable(SpiPtr);                                     // Enable the SPI transmission.



		if(MOSI_buffPtr!=NULL)
		{
																										   // Fill the TXFIFO with as many bytes as it will take until they have all been transmitted
			while(SpiPtr->RemainingBytes > (u32)0U)
			{

				while ((SpiPtr->RemainingBytes > (u32)0U) )                                                // Load fifo  --  Need to make sure the fifo does not overload!
				{
					XSpiPs_SendByte(SpiPtr->Config.BaseAddress, *SpiPtr->SendBufferPtr);
					SpiPtr->SendBufferPtr += 1;
					SpiPtr->RemainingBytes--;
				}
																										   // If master mode and manual start mode, issue manual start command to start the transfer.

				if ((XSpiPs_IsManualStart(SpiPtr) == TRUE)	&& (XSpiPs_IsMaster(SpiPtr) == TRUE))
				{
					ConfigReg  = XSpiPs_ReadReg(	SpiPtr->Config.BaseAddress, XSPIPS_CR_OFFSET);
					ConfigReg |= XSPIPS_CR_MANSTRT_MASK;

					XSpiPs_WriteReg(SpiPtr->Config.BaseAddress, XSPIPS_CR_OFFSET, ConfigReg);
				}
                                                                                                          // Wait for the transfer to finish by polling Tx fifo status.
				CheckTransfer = (u32)0U;
				while (CheckTransfer == 0U)
				{
					StatusReg = XSpiPs_ReadReg( SpiPtr->Config.BaseAddress,  XSPIPS_SR_OFFSET );
					if ( (StatusReg & XSPIPS_IXR_MODF_MASK) != 0U)
					{
                                                                                                          // Clear the mode fail bit
						XSpiPs_WriteReg( SpiPtr->Config.BaseAddress, XSPIPS_SR_OFFSET,	XSPIPS_IXR_MODF_MASK);
						return (s32)XST_SEND_ERROR;
					}
					CheckTransfer = (StatusReg & XSPIPS_IXR_TXOW_MASK);
				}
			}
		}


        if (MISO_buffPtr != NULL)
        {

            bytes_recvd = 0;
    		while (bytes_recvd < number_of_bytes)
    		{
    			TempData = (u8)XSpiPs_RecvByte(	SpiPtr->Config.BaseAddress );

    			if (SpiPtr->RecvBufferPtr != NULL)
    			{
    				*(SpiPtr->RecvBufferPtr) = TempData;
    				SpiPtr->RecvBufferPtr   += 1;
    			}

    			bytes_recvd++;
    	    }
        }


                                                            // Clear the slave selects now, before terminating the transfer.
		if (XSpiPs_IsManualChipSelect(SpiPtr) == TRUE)
		{
			ConfigReg  = XSpiPs_ReadReg( SpiPtr->Config.BaseAddress, XSPIPS_CR_OFFSET );
			ConfigReg |= XSPIPS_CR_SSCTRL_MASK;

			XSpiPs_WriteReg( SpiPtr->Config.BaseAddress,  XSPIPS_CR_OFFSET, ConfigReg );
		}

		SpiPtr->IsBusy = FALSE;                             // Clear the busy flag.

		XSpiPs_Disable(SpiPtr);                             // Disable the device.
		Status_Polled = (s32)XST_SUCCESS;

	}
	return Status_Polled;

}


