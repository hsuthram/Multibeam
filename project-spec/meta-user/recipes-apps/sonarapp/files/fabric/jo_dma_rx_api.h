


/*
 * dma_test_routines.h
 *
 *  Created on: Oct 28, 2016
 *      Author: jcarmona
 */

#ifndef SRC_DMA_INTERFACING_H_
#define SRC_DMA_INTERFACING_H_


/*
 * Device hardware build related constants.
 *
 * Constants defined as CONFIGURABLE can be varied to optimize the performance
 *
 * The number of buffers descriptors can be reduced but then the size of the buffer elements should be reduced
 *
 * It seems that the sum of the buffer element sizes should not exceed 960 K, or maybe we shoul not use 1Mbyte for the whole set of buffer elements
 * we tested with 64 Buffer descriptors and Buffer elements of 16Kbytes and beyond but could not make it work (we may have stepped on other areas of memory)
 * but 64 Buffer descriptors and Buffer lengths of 15Kbytes or less worked fine
 *
 *
 */

#define DMA_DEV_ID		            XPAR_AXIDMA_0_DEVICE_ID

#define DMA_MEM_BASE_ADDR	        0x10000000                            // Defining Memory space for the DMA Tx buffer descriptors and the Tx Buffer Base


// Definition of the Buffer Descriptor Spaces

#define DMA_RX_BD_N                 (4)                        // Configurable must be at least 3 in order to avoid holding up the stream while the dma buffers get released
#define DMA_RX_BD_SPACE_BASE    	(DMA_MEM_BASE_ADDR)
#define DMA_RX_BD_SPACE_LENGTH      ((DMA_RX_BD_N) * sizeof(XAxiDma_Bd))
#define DMA_RX_BD_SPACE_HIGH        ((DMA_RX_BD_SPACE_BASE) + (DMA_RX_BD_SPACE_LENGTH) - 1)


// Definition of the DMA Buffer Blocks

#define DMA_RX_BUFF_ELEMS_N         (DMA_RX_BD_N)
#define DMA_RX_BUFF_WORD_BYTES      (4)
#define DMA_RX_BUFF_ELEM_WORDS      (1024 * 160 * 6)                                           // 32 bit words; originally 1024*60, but changed to 1024*60*6 to support long ranges for raw data acq
#define DMA_RX_BUFF_ELEM_BYTES      ((DMA_RX_BUFF_ELEM_WORDS) * (DMA_RX_BUFF_WORD_BYTES))
#define DMA_RX_BUFFER_LENGTH        ((DMA_RX_BUFF_ELEM_BYTES) * (DMA_RX_BUFF_ELEMS_N))
#define DMA_RX_BUFFER_BASE	   	    ((DMA_RX_BD_SPACE_HIGH) + 1)                               // Derived: Place RX buffers right below the Buffer Descriptors
#define DMA_RX_BUFFER_HIGH		    ((DMA_RX_BUFFER_BASE) + (DMA_RX_BUFFER_LENGTH) - 1)



// Definition of interrupt related constants
#define DMA_RX_INTR_ID              XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR
#define DMA_RX_PRIORITY             0xA0
#define DMA_RX_TRIGGER_TYPE         0x03



typedef struct
 {
	u32           bd_procd_cnt;          // buffer descriptors processed
  	XAxiDma_Bd   *bd_ptr;                // pointer to the buffer descriptor of the current buffer element
  	u32          *bd_buff_ptr;           // pointer to the start of the current buffer element
  	u32          *bd_buff_elem_ptr;      // pointer to the element that will be transferred
 }
  jo_dma_rx_ctrl;


 typedef struct
 {
	 u32 	elems_total;            // = DMA_RX_BD_N;                        // configured in DMA setup (core 2)
	 u32 	bytes_per_word;         // = DMA_RX_BUFF_WORD_BYTES;
	 u32 	words_per_elem_max;     // = DMA_RX_BUFF_ELEM_WORDS;
	 u32 	words_per_usamp;        // = mb_session.beams_n;                 // one value per beam in a usamp
	 u32    elem_usamp_capacity;    // = floor( ((double)(mb_session.dma_cfg.words_per_elem_max)) /
	  	        	                //          ((double)(mb_session.dma_cfg.words_per_usamp))  );
 }
  jo_dma_cfg;



/*
 * DMA Device instances definitions
 */

extern          XAxiDma  AxiDma;	 		                                               // DMA Device instance definitions
extern volatile u8       g_dma_err;
extern volatile u8       g_s2mm_done;

extern int  jo_dma_rx_create                           (void);
extern int  jo_dma_rx_setup                            (void);
extern void jo_dma_rx_interrupt_isr                    (void* CallbackRef);                // Interrupt Service Routine. This one assumes the Interrupt Controller was created by our app. The CallbackRef parameter will be the pointer to the DMA object
extern void jo_dma_rx_interrupt_registration_byAddr    (void);
extern int  jo_dma_rx_interrupt_registration_byPointer (XScuGic *interrupt_ctrl); 	       // This setup assumes that the interrupt controller instance is Int_Ctrl.  It also assumes that the Interrupt Controller Device ID is INTC_DEVICE_ID

extern int  jo_dma_rx_get_num_bufs_ready               (jo_dma_rx_ctrl  *dma_rx );
extern int  jo_dma_rx_get_next_buffer                  (jo_dma_rx_ctrl  *dma_rx );
extern int  jo_dma_rx_release_buffers                  (jo_dma_rx_ctrl  *dma_rx, u32 buf2rel_n);

//extern int DMA_GetData_with_time_measurment(int words_per_buffer);


#endif /* SRC_DMA_INTERFACING_H_ */
