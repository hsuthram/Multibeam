/*
 * dma_test_routines.c
 *
 *  Created on: Oct 28, 2016
 *      Author: jcarmona
 */


#include <stdio.h>
// #include <xparameters.h>
// #include <xil_types.h>
// #include <xgpio.h>
// #include <xstatus.h>
// #include <xuartps.h>
// #include <string.h>
// #include <xaxidma.h>
// #include <xaxidma_bd.h>
// #include <xaxidma_bdring.h>
// #include <xaxidma_hw.h>
// #include <xil_cache.h>
// #include <xil_printf.h>
// #include <xscugic.h>
// #include <xscugic_hw.h>
// #include <xtime_l.h>

#include "jo_dma_rx_api.h"
#include "jo_interrupts.h"



        XAxiDma          AxiDma;	 		                             	// DMA Device instance definitions
static  XAxiDma_BdRing  *RxRingPtr;

volatile u8       g_dma_err   = 0;                                          // variables used in the interrupt service routine
volatile u8       g_s2mm_done = 0;                                          // This is a global variable that is used as flag to indicate the DMA transfer routines that an interrupt has taken place and the DMA data downloading needs to take place.  The ISR sets the flag and does the interrupt bookkeeping, while the DMA routine is waiting ina loop for the flag to be set. Once it is set it clears it and proceeds to do the data transfer. It is assumed that the data transfer will be done before the next interrupt occurs


//----------------- DMA CONFIGURATION AND SETUP ROUTINES --------------------//

/*****************************************************************************/
/**
*
* This function configures the DMA engine
*
* @param	Config is a pointer returning to the actual configuration of the DMA engine
*
* @return	XST_SUCCESS if the setup is successful, XST_FAILURE otherwise.
*
* @note
*
* 		Main Steps to setup the receiver DMA part
*
* 		- Get Config based on teh DMA_DEV_ID from the bsp
* 		-
* 		- Initialize the DMA engine according to this configuration
* 		-
* 		- Verify that the DMA has Scatter and gather configuration
*
*
******************************************************************************/

int jo_dma_rx_create(void)
{
	int             Status;
	XAxiDma_Config *Config;

	Config = XAxiDma_LookupConfig(DMA_DEV_ID);							// Get Config data to initialize DMA engine

	         if (!Config) { xil_printf("\n\r No config found for %d\r\n", DMA_DEV_ID); return XST_FAILURE; }

	Status = XAxiDma_CfgInitialize(&AxiDma, Config);					// Initialize DMA engine

	         if (Status != XST_SUCCESS) { xil_printf("\n\r Initialization failed %d\r\n", Status); return XST_FAILURE; }

	Status = XAxiDma_HasSg(&AxiDma);                                    // Verify that DMA has Scatter and gather enabled

			 if (!Status) { xil_printf("\n\r Device configured as Simple mode \r\n"); return XST_FAILURE; }

	return XST_SUCCESS;
}



/*****************************************************************************/
/**
*
* This function sets up RX channel of the DMA engine to be ready for packet
* reception
*
* @param	AxiDmaInstPtr is the pointer to the instance of the DMA engine.
*
* @return	XST_SUCCESS if the setup is successful, XST_FAILURE otherwise.
*
* @note
*
* 		Main Steps to setup the receiver DMA part
*
* 		- Prepare for Setup
* 		- 		Get a pointer to the AxiDMA
* 		- 		Disable Interrupts for the setup time period
* 		-
* 		- Create BD Ring
* 		-       Estimate the number of Bd based on the Rx space and alignment
* 		-       Create BD ring
* 		-       Clear the space associated with the BD
* 		-
* 		- Allocate the Free Bds
* 		-       Get the Free Bd from the ring
* 		-       Allocate the Free Bds
* 		-
* 		- Associate the Bds to the actual buffers
* 		-       Starting with the first Bd and the Base
* 		-       Continue until all are associated
*
*
*  Remember
*  S/M_AXIS_S2MM � Slave/Master stream to memory map (device to DMA)
*  S/M_AXIS_MM2S � Slave / Master memory map to stream (DMA to device)
*
*
*
*
******************************************************************************/


//------- DMA SETUP routines

int jo_dma_rx_setup (void)
{
	int             Delay = 0;
	int             Coalesce = 1;
	int             Status;
	XAxiDma_Bd      BdTemplate;
	XAxiDma_Bd     *BdPtr;
	XAxiDma_Bd     *BdCurPtr;
	u32             BdCount;
	u32             FreeBdCount;
	u32             RxBufferPtr;
	int             Index;

                                                         // ---- Preliminary Settings
	RxRingPtr   = XAxiDma_GetRxRing(&AxiDma);

	XAxiDma_BdRingIntDisable  (RxRingPtr, XAXIDMA_IRQ_ALL_MASK);			    // Disable all RX interrupts before RxBD space setup, using hte RxRingPtr it refers back to the AxiDMA parent object and disables interrupts
	XAxiDma_BdRingSetCoalesce (RxRingPtr, Coalesce, Delay);					    // Set delay and coalescing

														 // ----- Create Ring   (Setup Rx BD space)

	BdCount     = XAxiDma_BdRingCntCalc (XAXIDMA_BD_MINIMUM_ALIGNMENT,          // Estimate the number of Buffer Descriptors based on the alignment and space assigned
			                             DMA_RX_BD_SPACE_LENGTH);

	Status      = XAxiDma_BdRingCreate  (RxRingPtr,                             // Create the ring of Buffer descriptors
			                             DMA_RX_BD_SPACE_BASE,
			                             DMA_RX_BD_SPACE_BASE,
										 XAXIDMA_BD_MINIMUM_ALIGNMENT,
										 BdCount);
				                                                                if (Status != XST_SUCCESS) { xil_printf("\n\r DMA_RX create BD ring failed %d\r\n", Status); return XST_FAILURE; }

	XAxiDma_BdClear (&BdTemplate);												// Setup Rx BD space - Clear the space by creating an empty template and copy it to where the RxRingPtr points to

	Status      = XAxiDma_BdRingClone (RxRingPtr, &BdTemplate);
			                                                                    if (Status != XST_SUCCESS) { xil_printf("\n\r DMA_RX clone BD failed %d\r\n", Status); return XST_FAILURE; }

			                                             // ----- Allocate Buffers

	FreeBdCount = XAxiDma_BdRingGetFreeCnt(RxRingPtr);							//  Get the free BDs in the ring.  would the number of free BDs be equal to BdCount ??
	Status      = XAxiDma_BdRingAlloc     (RxRingPtr, FreeBdCount, &BdPtr);     //  Allocate the BD structure in memory.  BdPtr returns the pointer to the free buffers on the RxRing

	                                                                            if (Status != XST_SUCCESS) { xil_printf("\n\r DMA_RX alloc BD failed %d\r\n", Status); return XST_FAILURE; }
																				// Associate the free BDs of the RxRing to the actual buffer space
	BdCurPtr    = BdPtr;														// Start pointing to the first free BD in the RxRing
	RxBufferPtr = DMA_RX_BUFFER_BASE;											// Use the free 32 bit pointer to point to the base of the Rx Buffer

	for (Index = 0; Index < FreeBdCount; Index++)
	{
		Status = XAxiDma_BdSetBufAddr (BdCurPtr, RxBufferPtr);					// Associate the BD in turn to the Buffer segment in turn
		                                                                        if (Status != XST_SUCCESS)  { xil_printf("\n\r Set buffer addr %x on BD %x failed %d\r\n",   (unsigned int)RxBufferPtr,  (unsigned int)BdCurPtr, Status); return XST_FAILURE; }

		Status = XAxiDma_BdSetLength  (BdCurPtr, DMA_RX_BUFF_ELEM_BYTES,        // Assign the buffer length to the current BD
				                       RxRingPtr->MaxTransferLen);
		                                                                        if (Status != XST_SUCCESS)  { xil_printf("\n\r Rx set length %d on BD %x failed %d\r\n",  DMA_RX_BUFF_ELEM_BYTES, (unsigned int)BdCurPtr, Status); return XST_FAILURE; }

		XAxiDma_BdSetCtrl (BdCurPtr, 0);								        // The Receive BDs do not need to set anything for the control. The hardware will set the SOF/EOF bits per stream status
		XAxiDma_BdSetId   (BdCurPtr, RxBufferPtr);								// Assign the pointer to the buffer region to the current BD's Id ???

		RxBufferPtr += DMA_RX_BUFF_ELEM_BYTES;                                  // Move to the next immediate buffer region
//		RxBufferPtr += DMA_RX_BUFF_ELEM_WORDS;                                  // Move to the next immediate buffer region
		BdCurPtr     = (XAxiDma_Bd *)XAxiDma_BdRingNext (RxRingPtr, BdCurPtr);  // Move to the next immediate BD in the ring and repeat association of buffer to BD
	}

    memset((void *)DMA_RX_BUFFER_BASE, 0, DMA_RX_BUFFER_LENGTH);			    // Clear the initial receive buffer, so we can verify data

                                                         //-------- Launch Ring

	Status = XAxiDma_BdRingToHw  (RxRingPtr, FreeBdCount,  BdPtr);	   	        // Submit the RxRing to hardware - (i guess it must just clear or set some flags)
	                                                                            if (Status != XST_SUCCESS) { xil_printf("\n\r RX submit hw failed %d\r\n", Status); return XST_FAILURE; }
	XAxiDma_IntrEnable (&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);

	Status = XAxiDma_BdRingStart (RxRingPtr);									// Start RX DMA channel
                                                                                if (Status != XST_SUCCESS) { xil_printf("\n\r RX start hw failed %d\r\n", Status); return XST_FAILURE;  }
	return XST_SUCCESS;
}



/******************************************************************************/
/**
 * Function to define the DMA interrupt service routine
 *
 * This function simply sets a flag to let the environment know that there is
 * a complete data buffer element available with data
 *
 */


void jo_dma_rx_interrupt_isr(void* CallbackRef)                                   // Interrupt Service Routine. This one assumes the Interrupt Controller was created by our app. The CallbackRef parameter will be the pointer to the DMA object
{
																						// Local variables
	u32      irq_status;
	int      time_out;
	XAxiDma* p_dma_inst = (XAxiDma*)CallbackRef;										// Here we connect the pointer to the DMA object to the CallBackRef pointer

	                                                                                    // Note the disable below had XAXIDMA_DMA_TO_DEVICE instead of XAXIDMA_DEVICE_TO_DMA
	XAxiDma_IntrDisable (p_dma_inst, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);      // Disable interrupt from the DMA to the micro
																						//mmXAxiDma_IntrDisable(p_dma_inst, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);

	irq_status = XAxiDma_IntrGetIrq(p_dma_inst, XAXIDMA_DEVICE_TO_DMA);					// Read pending interrupts associated to the DMA_RX device

	XAxiDma_IntrAckIrq (p_dma_inst, irq_status, XAXIDMA_DEVICE_TO_DMA);                 // Acknowledge pending interrupts associated to the DMA_RX device

	if (!(irq_status & XAXIDMA_IRQ_ALL_MASK))											// If no interrupt is asserted, we do not do anything
	{
		xil_printf("No interrupt in AXI DMA, but stopped anyway!\n");
		return;
	}

	if ((irq_status & XAXIDMA_IRQ_ERROR_MASK))											// If interrupt error flag is asserted  -- NEED TO REVIEW THIS AS THE XILINX EXAQMPLE IS NOT SURE ABOUT HOW TO DEAL WITH THIS
	{

		xil_printf("Error with AXI DMA!\n");
		g_dma_err = 1;																	   // 1) Raise error flag,
		XAxiDma_Reset(p_dma_inst);														   // 2) reset the hardware to recover from the error;
		time_out = RESET_TIMEOUT_COUNTER;                                                  // 3) Reset should never fail for transmit channel, so wait for it to occur
		while (time_out)
		{
			if (XAxiDma_ResetIsDone(p_dma_inst))
				break;
			time_out -= 1;
		}
		return;                                                                           // 4) and return with no further processing.
	}

	if (irq_status & XAXIDMA_IRQ_IOC_MASK)											   // if Completion interrupt asserted, then set the DONE flag to indicate data is ready in the buffer
		g_s2mm_done = 1;
	                                                                                   // Re-enable interrupt associated to DMA_RX device

	XAxiDma_IntrEnable(p_dma_inst, (XAXIDMA_IRQ_IOC_MASK | XAXIDMA_IRQ_ERROR_MASK), XAXIDMA_DMA_TO_DEVICE);

}



void jo_dma_rx_interrupt_registration_byAddr(void)
{

    XScuGic_SetPriTrigTypeByDistAddr ( XPAR_SCUGIC_0_DIST_BASEADDR,     // u32 DistBaseAddress,
	                                   DMA_RX_INTR_ID,                  // u32 Int_Id, ... Note: For interrupts from the Fabric (coming through IRQ_F2P) look for the interrupt ID on xparameters.h: XPAR_FABRIC_* followed by the source and id
                                       DMA_RX_PRIORITY,                 // u8  Priority,
                                       DMA_RX_TRIGGER_TYPE              // u8  Trigger
			                         );


	//--- Connect the interrupt to the controller and enable the interrupt
    //--- Each and every interrupt must go through these steps

	XScuGic_RegisterHandler          ( XPAR_SCUGIC_0_CPU_BASEADDR,
				                       DMA_RX_INTR_ID,                   // u32 Int_Id, ... Note: For interrupts from the Fabric (coming through IRQ_F2P) look for the interrupt ID on xparameters.h: XPAR_FABRIC_* followed by the source and id
									   (Xil_ExceptionHandler)jo_dma_rx_interrupt_isr,
									   &AxiDma                           // void: argument for the ISR
									 );

    XScuGic_EnableIntr( XPAR_SCUGIC_0_DIST_BASEADDR, DMA_RX_INTR_ID);

}



int jo_dma_rx_interrupt_registration_byPointer (XScuGic *interrupt_ctrl) 				  	     		                            // This setup assumes that the interrupt controller instance is Int_Ctrl.  It also assumes that the Interrupt Controller Device ID is INTC_DEVICE_ID
{                                                                                                               // check dma_rx_interrupt_setup_orig_bak
	int             Status = 0;                                                                                 // Variable just to get a status value to compare

	XScuGic_SetPriorityTriggerType ( interrupt_ctrl,                                                            // Set interrupt priorities and trigger type
			                         DMA_RX_INTR_ID,                                                            // Interrupt id
									 DMA_RX_PRIORITY,                                                           // priority
									 DMA_RX_TRIGGER_TYPE                                                        // trigger type
								   );

	Status      = XScuGic_Connect  ( interrupt_ctrl,                                                            // Connect the device driver handler that will be called when an interrupt for the device occurs, the handler defined above performs the specific interrupt processing for the device.  The AXIDma will be passed to the interrupt service routine as a Callback parameter
			                         DMA_RX_INTR_ID,                                                            // interrupt id
								     (Xil_InterruptHandler)jo_dma_rx_interrupt_isr,                             // associated isr
								     &AxiDma                                                                    // &AXIDma is a callback parameter fo the isr
								   );
	                                                                                 if (Status != XST_SUCCESS)  { xil_printf("ERROR! Failed to connect s2mm_isr to the interrupt controller.\r\n", Status);  return INTC_INIT_SUCCESS; }
	XScuGic_Enable ( interrupt_ctrl, DMA_RX_INTR_ID );                                                          // Enable all interrupts

	return 0;
}




//------------ DMA UTILITY ROUTINES  ---- To be used during loading and unloading of the DMA buffers


                                                             // This routine "opens" the ring and gets how many buffer elements are loaded
                                                             // It also "brings" and gets access to the first buffer element available

int jo_dma_rx_get_num_bufs_ready ( jo_dma_rx_ctrl  *dma_rx )
{
	dma_rx->bd_procd_cnt     = XAxiDma_BdRingFromHw(  RxRingPtr, XAXIDMA_ALL_BDS, &(dma_rx->bd_ptr ));    // Get the number of buffer elements that received data
                                                                                                          // and also the pointer to the Descriptor of the first buffer element
	dma_rx->bd_buff_elem_ptr = XAxiDma_BdGetBufAddr(dma_rx->bd_ptr);                                      // Get the address of the current buffer element in the loop
//	dma_rx->bd_buff_elem_ptr = (INTPTR)(XAxiDma_BdGetBufAddr(dma_rx->bd_ptr));                            // Get the address of the current buffer element in the loop

	Xil_DCacheInvalidateRange ( (INTPTR)(dma_rx->bd_buff_elem_ptr), DMA_RX_BUFF_ELEM_BYTES );             // Releasing all the buffer elements in the loop

	return(0);
}

															// This routine "gets" the next loaded buffer element in the ring.
                                                            // this routine must be called only after calling jo_dma_rx_get_num_bufs_ready
															// It also "brings" and gets access to the first buffer element available

int jo_dma_rx_get_next_buffer ( jo_dma_rx_ctrl  *dma_rx )
{
	dma_rx->bd_ptr           = (XAxiDma_Bd *)XAxiDma_BdRingNext( RxRingPtr, dma_rx->bd_ptr );
	dma_rx->bd_buff_elem_ptr = XAxiDma_BdGetBufAddr(dma_rx->bd_ptr);                                // Get the address of the current buffer element in the loop
//	dma_rx->bd_buff_elem_ptr = (INTPTR)(XAxiDma_BdGetBufAddr(dma_rx->bd_ptr));                            // Get the address of the current buffer element in the loop

    Xil_DCacheInvalidateRange ( (INTPTR)(dma_rx->bd_buff_elem_ptr), DMA_RX_BUFF_ELEM_BYTES );             // Releasing all the buffer elements in the loop

    return (0);
}


                                                              // This routine "frees up" the buffers that received data
                                                              // and that were processed


int jo_dma_rx_release_buffers (jo_dma_rx_ctrl  *dma_rx, u32 buf2rel_n)
{
	int status, free_bd_cnt;
	XAxiDma_Bd *aux_bd_ptr;

	status      = XAxiDma_BdRingFree       (RxRingPtr, buf2rel_n, dma_rx->bd_ptr );                       // Free the buffer elements starting with the one pointed by BdPtr
	free_bd_cnt = XAxiDma_BdRingGetFreeCnt (RxRingPtr);                                                   // Get number of free buffers
	status      = XAxiDma_BdRingAlloc      (RxRingPtr, free_bd_cnt,  &(aux_bd_ptr) );                     // Allocate the buffer just released and Redirect BdPtr to the new buffer element that starts the chain??
	status      = XAxiDma_BdRingToHw       (RxRingPtr, free_bd_cnt, aux_bd_ptr  );                        // Bring buffers back to hardware

	return(status);
}


//int DMA_GetData_with_time_measurment(int words_per_buffer)
//{
//	XAxiDma_BdRing *RxRingPtr;
//	XAxiDma_Bd     *BdPtr;
//	XAxiDma_Bd     *Curr_BdPtr;
//	int             ProcessedBdCount;
//	int             FreeBdCount;
//	int             Status;
//
////	uXX             *RxPacket;
//	u32            *RxPacket;
//	unsigned int   *RxPacket_unsigned_int;
//	int             i, k;
//
//
//
//    XTime tStart_XAxiDma_Global,            tEnd_XAxiDma_Global,              tNet_XAxiDma_Global;
//    XTime tStart_XAxiDma_BdRingFromHw,      tEnd_XAxiDma_BdRingFromHw,        tNet_XAxiDma_BdRingFromHw;
//    XTime tStart_XAxiDma_BdGetBufAddr,      tEnd_XAxiDma_BdGetBufAddr,        tNet_XAxiDma_BdGetBufAddr;
//    XTime tStart_Xil_DCacheInvalidateRange, tEnd_Xil_DCacheInvalidateRange,   tNet_Xil_DCacheInvalidateRange;
//    XTime tStart_XAxiDma_BdRingNext,        tEnd_XAxiDma_BdRingNext,          tNet_XAxiDma_BdRingNext;
//	XTime tStart_XAxiDma_BdRingFree,        tEnd_XAxiDma_BdRingFree,          tNet_XAxiDma_BdRingFree;
//	XTime tStart_XAxiDma_BdRingGetFreeCnt,  tEnd_XAxiDma_BdRingGetFreeCnt,    tNet_XAxiDma_BdRingGetFreeCnt;
//	XTime tStart_XAxiDma_BdRingAlloc,       tEnd_XAxiDma_BdRingAlloc,         tNet_XAxiDma_BdRingAlloc;
//	XTime tStart_XAxiDma_BdRingToHw,        tEnd_XAxiDma_BdRingToHw,          tNet_XAxiDma_BdRingToHw;
//
//	XTime tStart_XAxiDma_iter_b[100];
//	XTime tStart_XAxiDma_iter_e[100];
//	volatile int   iter_cnt = 0;
//	volatile int   loop_cnt;
//
//
//	RxRingPtr   = XAxiDma_GetRxRing(&AxiDma);                                                        // Connect RxRingPtr to the DMA structure
//
//    XTime_GetTime(&tStart_XAxiDma_Global);
//    loop_cnt = 0;
//    iter_cnt = 0;
//
//	do                                                                                               // Exit when there are no more Buffer elements to service
//	{
//
//             XTime_GetTime(&tStart_XAxiDma_BdRingFromHw);
//		ProcessedBdCount = XAxiDma_BdRingFromHw(RxRingPtr, XAXIDMA_ALL_BDS, &BdPtr);                 // Wait until the data has been received by the Rx channel
//	         XTime_GetTime(&tEnd_XAxiDma_BdRingFromHw);
//	         tNet_XAxiDma_BdRingFromHw = tEnd_XAxiDma_BdRingFromHw - tStart_XAxiDma_BdRingFromHw;
//
//		if (ProcessedBdCount > 0)
//		{
//			XTime_GetTime(&tStart_XAxiDma_iter_b[iter_cnt]);
//
//			Curr_BdPtr = BdPtr;
//
//			for (i=0; i < ProcessedBdCount; i++)                                                     // Service each Buffer element in the list
//			{
//
//			         XTime_GetTime(&tStart_XAxiDma_BdGetBufAddr);
//	    		RxPacket                = (u32 *)XAxiDma_BdGetBufAddr(Curr_BdPtr);
//	    	         XTime_GetTime(&tEnd_XAxiDma_BdGetBufAddr);
//	    	         tNet_XAxiDma_BdGetBufAddr = tEnd_XAxiDma_BdGetBufAddr - tStart_XAxiDma_BdGetBufAddr;
//
//	    		RxPacket_unsigned_int   = ( unsigned int *)RxPacket;
//
//
//	                 XTime_GetTime(&tStart_Xil_DCacheInvalidateRange);
//	    		Xil_DCacheInvalidateRange ( RxPacket_unsigned_int, DMA_RX_BUFF_ELEM_BYTES );         // Invalidate the DMA DestBuffer before receiving the data, in case the Data Cache is enabled
//	                 XTime_GetTime(&tEnd_Xil_DCacheInvalidateRange);
//	                 tNet_Xil_DCacheInvalidateRange = tEnd_Xil_DCacheInvalidateRange - tStart_Xil_DCacheInvalidateRange;
//
//				/********
//				 * Begin segment where we service each buffer element that is ready to process
//				 ********/
//
//
//				for (k = 0; k < words_per_buffer; k++)                                    // This is the loop where we consume the data on the DMA buffer
////				for (k = 0; k < DMA_RX_BUFFER_ELEMENT_WORDS; k++)                                    // This is the loop where we consume the data on the DMA buffer
//				{
////			    	for (b = 0; b < 4; b++)                                                          // Dummy prototype: this loop should be replaced by the routine/method that will copy/transfer the data from the buffer
////			    		Ethernet_Send_data (1 byte, *( (char8*)( (char8*)(&RxPacket[k]) + b ) ) );   // Ethernet_Send_data needs to be replaced by the actual function that sends data via the ethernet channel
//
//                             ;
//				}                                                                                    // end of data_consumption loop
//
//
//				/********
//				 * END segment where we service each buffer element that is ready to process
//				 ********/
//   		              XTime_GetTime(&tStart_XAxiDma_BdRingNext);
//			    Curr_BdPtr = (XAxiDma_Bd *)XAxiDma_BdRingNext(RxRingPtr, Curr_BdPtr);	             // Onto next BD
//	 	              XTime_GetTime(&tEnd_XAxiDma_BdRingNext);
//		              tNet_XAxiDma_BdRingNext = tEnd_XAxiDma_BdRingNext - tStart_XAxiDma_BdRingNext;
//
//			}
//         		XTime_GetTime(&tStart_XAxiDma_BdRingFree);
//		    Status    = XAxiDma_BdRingFree (RxRingPtr, ProcessedBdCount, BdPtr);	         // Free all processed RX BDs for future transmission
//		        XTime_GetTime(&tEnd_XAxiDma_BdRingFree);
//		        tNet_XAxiDma_BdRingFree = tEnd_XAxiDma_BdRingFree - tStart_XAxiDma_BdRingFree;
//												   if (Status != XST_SUCCESS)
//												          { xil_printf("\n\r Failed to free %d rx BDs %d\r\n", ProcessedBdCount, Status); return XST_FAILURE; }
//
//     		     XTime_GetTime(&tStart_XAxiDma_BdRingGetFreeCnt);
//            FreeBdCount = XAxiDma_BdRingGetFreeCnt (RxRingPtr);                                      // Return processed BDs to RX channel so we are ready to receive new packets:
//                 XTime_GetTime(&tEnd_XAxiDma_BdRingGetFreeCnt);
//                 tNet_XAxiDma_BdRingGetFreeCnt = tEnd_XAxiDma_BdRingGetFreeCnt - tStart_XAxiDma_BdRingGetFreeCnt;
//
//                 XTime_GetTime(&tStart_XAxiDma_BdRingAlloc);
//            Status      = XAxiDma_BdRingAlloc      (RxRingPtr, FreeBdCount, &BdPtr);                 //     Allocate all free RX BDs
//                 XTime_GetTime(&tEnd_XAxiDma_BdRingAlloc);
//                 tNet_XAxiDma_BdRingAlloc = tEnd_XAxiDma_BdRingAlloc - tStart_XAxiDma_BdRingAlloc;
//                                                    if (Status != XST_SUCCESS)
//												          { xil_printf("\n\r bd alloc failed\r\n"); return XST_FAILURE; }
//
//		         XTime_GetTime(&tStart_XAxiDma_BdRingToHw);
//     		Status      = XAxiDma_BdRingToHw       (RxRingPtr, FreeBdCount, BdPtr);                  //     Pass the BDs to RX channel
//     	         XTime_GetTime(&tEnd_XAxiDma_BdRingToHw);
//     	         tNet_XAxiDma_BdRingToHw = tEnd_XAxiDma_BdRingToHw - tStart_XAxiDma_BdRingToHw ;
//     	                                            if (Status != XST_SUCCESS)
//												          { xil_printf("\n\r Submit %d rx BDs failed %d\r\n", FreeBdCount, Status); return XST_FAILURE; }
//
//			XTime_GetTime(&tStart_XAxiDma_iter_e[iter_cnt]);
//			BdCount_Array[iter_cnt++] = ProcessedBdCount;
//		}
//        else
//        {
////    		while (g_s2mm_done == 0)
//    			;
//
////    		g_s2mm_done = 0;
//        }
//
//		loop_cnt++;
//
//	}
//	while ( 1 );
//
//    XTime_GetTime(&tEnd_XAxiDma_Global);
//
//    tNet_XAxiDma_Global = tEnd_XAxiDma_Global - tStart_XAxiDma_Global;
//
//
//	return XST_SUCCESS;
//}
