/*
 * interrupts.c
 *
 *  Created on: Nov 15, 2016
 *      Author: jcarmona
 */


// #include <xparameters.h>
#include <string.h>
// #include <xil_types.h>
// #include <xil_cache.h>
// #include <xil_printf.h>
//
// #include <xstatus.h>
// #include <xuartps.h>
// #include <xgpio.h>
// #include <xaxidma.h>
// #include <xaxidma_bd.h>
// #include <xaxidma_bdring.h>
// #include <xaxidma_hw.h>
// #include <xscugic.h>
// #include <xscugic_hw.h>

#include "jo_interrupts.h"



XScuGic  Int_Ctrl;

/*
 *  Definition of the Interrupt controller by Pointer.
 *
 *  This setup uses the routines of the xscugic.h and is based on the application notes from examples
 *  that use the DMA.
 *
 *  Do not use this interrupt setup if the application is based on the ECHO LWIP as this one uses
 *  the interrupt controller setup method by Addr (described below).
 *
 *  APPARENTLY IT IS NOT POSSIBLE TO COMBINE BOTH STYLES OR METHODS
 *
 *  This setup assumes that a global variable to hold the interrupt controller has been declared such as:
 *
 *           XScuGic  interrupt_ctrl;
 *
 *  The call to this routine should be
 *
 *           jo_interrupts_controller_setup_byPointer (&interrupt_ctrl)
 *
 *  In this routine must be included the definition of the interrupts and their service routines
 *
 */
                                                                                                                   // It also assumes that the Interrupt Controller Device ID is INTC_DEVICE_ID
int jo_interrupts_controller_setup_byPointer (XScuGic *interrupt_ctrl)
{
	int             Status = 0;                                                                                    // Variable just to get a status value to compare
	XScuGic_Config *p_intc_cfg;                                                                                    // Pointer to the Configuration of the Interrupt Controller Device


    p_intc_cfg  = XScuGic_LookupConfig(INTC_DEVICE_ID);												               // Retrieve Interrupt Configuration for the interrupt controller INTC_DEVICE_ID, which is the same device ide used in Style_B in the initializtion XScuGic_DeviceInitialize
				                                                                     if (!p_intc_cfg) { xil_printf("ERROR! No hardware configuration found for Interrupt Controller with device id %d.\r\n", INTC_DEVICE_ID);	return INTC_LOOKUP_CFG_FAIL;	}

	Status = XScuGic_CfgInitialize ( interrupt_ctrl,                                                               // (creates and ?) initializes the interrupt controller driver according to configuration retrieved in line before
			                         p_intc_cfg,
									 p_intc_cfg->CpuBaseAddress
								   );
				                                                                     if (Status != XST_SUCCESS)  { xil_printf("ERROR! Initialization of Interrupt Controller failed with %d.\r\n", Status);  return INTC_INIT_FAIL; }

	//--- insert here the definition and habilitation of each interrupt

    //    jo_interrupt_XXX_registration_byPointer(interrupt_ctrl, XXXX)

    //---


	Xil_ExceptionInit();																				           // Initialize exception table and register the interrupt controller handler with exception table
	Xil_ExceptionRegisterHandler  ( XIL_EXCEPTION_ID_INT,
			                        (Xil_ExceptionHandler)XScuGic_InterruptHandler,
								    interrupt_ctrl
								  );
	Xil_ExceptionEnable();																				           // Enable non-critical exceptions

	return INTC_INIT_SUCCESS;
}


/*
 *  Routine to Configure and Register Interrupts (interrupt service routines, prioirties etc.) by INTERRUPT CONTROLLER POINTER
 *
 *  This is based on the subroutines from xscugic_hw.c which are low-level driver functions that can be
 *  used to access the device.  The user should refer to the hardware device specification for more
 *  details of the device operation.
 *
 *  DO NOT USE THIS METHOD IF: The interrupt controller was defined 'By Address" or if the application
 *  uses is based on the echo lwip environment (which uses the By Address) method
 *
 *  Use this method to register interrupts if the controller was setup using jo_interrupt_controller_setup_byAddr or if the build is based
 *  on the echo LWIP application
 *
 *  DO NOT USE THIS TYPE OF INTERRUPT REGISTRATION IF THE INTERRUPT CONTROLLER WAS CREATED/DEFINED USING  jo_interrupts_controller_setup_byPointer
 *
 *  This style assumes that the interrupt controller is identified by INTC_DEVICE_ID
 *
 */


void jo_interrupt_XXX_registration_byPointer(XScuGic *interrupt_ctrl /*, include parameters as necessary */)
{
    //--- insert here the definition and connection of each interrupt
	//--- insert here the calls to configure each individual interrupt
	//--- may call other routines... interrupts may be different from each other in their configuration
	//
	//	XScuGic_SetPriorityTriggerType ( interrupt_ctrl,                                                            // Set interrupt priorities and trigger type
	//			                         XXX_INTR_ID,                                                               // Interrupt id e.g DMA_RX_INTR_ID
	//								     0xXX,                                                                      // priority
	//								     0xX                                                                        // trigger type
	//								   );
	//

	//--- Connect the interrupt to the controller and enable the interrupt
    //--- Each and every interrupt must go through these steps
	//
	//	Status      = XScuGic_Connect  ( interrupt_ctrl,                                                            // Connect the device driver handler that will be called when an interrupt for the device occurs, the handler defined above performs the specific interrupt processing for the device.  The AXIDma will be passed to the interrupt service routine as a Callback parameter
	//			                         XXX_INTR_ID,                                                               // interrupt id
	//								     (Xil_InterruptHandler)XXX_interrupt_isr,                                   // associated isr
	//								     &xxx_isr_Arguments                                                         // &xxx_isr_Arguments is a callback parameter object for the isr
	//								   );
	//
	//	if (Status != XST_SUCCESS)  { xil_printf("ERROR! Failed to connect s2mm_isr to the interrupt controller.\r\n", Status);  return (Status); }
    //
    //  XScuGic_Enable(interrupt_ctrl , XXX_interrupt_isr);															// Enable interrupt

}


/*
 *  use the following routine to disconnect an interrupt (this step is necessary when redefining an interrupt
 */

void jo_interrupt_XXX_disconnect_byPointer(XScuGic *interrupt_ctrl, u16 intr_id)
{
	XScuGic_Disconnect(interrupt_ctrl, intr_id);
}



//***********************************************************************************

/*
 *  Definition of the Interrupt controller - by Address (Style B)
 *
 *  It is based on the subroutines from xscugic_hw.c which are low-level driver functions that can be
 *  used to access the device.  The user should refer to the hardware device specification for more
 *  details of the device operation.
 *
 *  These routines allow using the ScuGic device without having to create an instance of XScuGic structure
 *
 *  These routines are used by the echo lwip application.
 *  SO, IF USING THE ECHO LWIP APPLICATION DO NOT USE THIS jo_interrupt_controller_setup_B, use instead setup_C
 *  as they interrupt object will have been already initialized by the application
 *
 *  This style assumes that the interrupt controller is identified by INTC_DEVICE_ID
 *
 *  The routine below contains the interrupt setup from "platform_zynq.c" of the lwip echo application
 *
 *  Note: If using this setup with an environment of the lwip echo, then comment the first 3 lines as they are
 *  already included in platform_zyq.c.  Only add the new interrupts.
 *
 *
 */


void jo_interrupt_controller_setup_byAddr(void)
{
	Xil_ExceptionInit();

	XScuGic_DeviceInitialize(INTC_DEVICE_ID);                                                                        // somehow this will create an interrupt controller "we cannot see" but "can refer to it" via its ID

	Xil_ExceptionRegisterHandler ( XIL_EXCEPTION_ID_IRQ_INT,                                                         // Connect the interrupt controller interrupt handler to the hardware interrupt handling logic in the processor.
			                       (Xil_ExceptionHandler)XScuGic_DeviceInterruptHandler,
			                       (void *)INTC_DEVICE_ID
								 );

	// register each interrupt here (or call them right after this function) using

	// jo_interrupt_XXX_registration_byAddr

	return;
}



/* Template for the routine to Configure and Register Interrupts BY ADDR (interrupt service routines, prioirties etc.
 *
 * Copy this template to the file that originates the interrupt and configure it
 *
 *  This is based on the subroutines from xscugic_hw.c which are low-level driver functions that can be
 *  used to access the device.  The user should refer to the hardware device specification for more
 *  details of the device operation.
 *
 *  This method of registering interrupts is ALSO used by the echo lwip application. So it
 *
 *  Use this method to register interrupts if the controller was setup using jo_interrupt_controller_setup_byAddr or if the build is based
 *  on the echo LWIP application
 *
 *  DO NOT USE THIS TYPE OF INTERRUPT REGISTRATION IF THE INTERRUPT CONTROLLER WAS CREATED/DEFINED USING  jo_interrupts_controller_setup_byPointer
 *
 *  This style assumes that the interrupt controller is identified by INTC_DEVICE_ID
 *
 */


void jo_interrupt_XXX_registration_byAddr(void)
{

	//--- insert here the calls to configure each individual interrupt
	//--- Note it may be necessary to call other routines... interrupts may be different from each other in their configuration
	//
	//--- Note there may be an issue with two of the constants used in the interrupt setup by address
	//    the file platform_zynq.c defines two constants
	//
    //                 #define INTC_BASE_ADDR       XPAR_SCUGIC_0_CPU_BASEADDR
    //                 #define INTC_DIST_BASE_ADDR  XPAR_SCUGIC_0_DIST_BASEADDR
	//
	//    and in the xparameter.h the two other constants are defined as
	//
	//                 #define XPAR_SCUGIC_0_CPU_BASEADDR  0xF8F00100U
    //                 #define XPAR_SCUGIC_0_DIST_BASEADDR 0xF8F01000U
	//
	//    both are the same... it is not clear what is the meaning of DIST and why the routine that
	//    set sup the priority and trigger XScuGic_SetPriTrigTypeByDistAddr  uses the "ByDistAddr"
	//    one would assume it requires as address XPAR_SCUGIC_0_DIST_BASEADDR instead of the other
	//    but they are the same.  It is necessary to always check the value that the BSP assigns to
	//    both constants if they are not the same, then verify what happens and document it here...
	//


    //--- Example of how to setup the priority and trigger for the interrupt
	//
    //   XScuGic_SetPriTrigTypeByDistAddr ( XPAR_SCUGIC_0_DIST_BASEADDR     // same as XPAR_SCUGIC_0_CPU_BASEADDR which is u32 DistBaseAddress
    //                                      XPAR_XXX_INTR                   // u32 Int_Id, ... Note: For interrupts from the Fabric (coming through IRQ_F2P) look for the interrupt ID on xparameters.h: XPAR_FABRIC_* followed by the source and id
    //                                      priority,                       // u8  Priority,
    //                                      trigger                         // u8  Trigger
    //                                    );


	//--- Connect the interrupt to the controller and enable the interrupt
    //--- Each and every interrupt must go through these steps

	//	XScuGic_RegisterHandler      ( XPAR_SCUGIC_0_CPU_BASEADDR,
    //                                 XPAR_XXX_INTR                        //u32 Int_Id, ... Note: For interrupts from the Fabric (coming through IRQ_F2P) look for the interrupt ID on xparameters.h: XPAR_FABRIC_* followed by the source and id
	//								   (Xil_ExceptionHandler)XXX_interrupt_isr,
	//								   (void *)&xxx_isr_Arguments
	//								 );
    //
    //  XScuGic_EnableIntr( XPAR_SCUGIC_0_CPU_BASEADDR, XPAR_XXX_INTR);

}
