/*
 * spi_interfacing_routines.h
 *
 *  Created on: Nov 8, 2017
 *      Author: jcarmona
 */

#ifndef SRC_JO_SPI_API_H_
#define SRC_JO_SPI_API_H_

#define XSpiPs_SendByte(BaseAddress, Data)  XSpiPs_Out32((BaseAddress) + (u32)XSPIPS_TXD_OFFSET, (u32)(Data))
#define XSpiPs_RecvByte(BaseAddress) 		XSpiPs_In32((u32)((BaseAddress) + (u32)XSPIPS_RXD_OFFSET))



extern int spi_setup                        (XSpiPs *SpiInstancePtr, u16 SpiDeviceId);
extern int spi_transaction_polled_mode      (XSpiPs *SpiPtr, u8 SlaveNum, u8 *MOSI_buffPtr, u8 *MISO_buffPtr, u32 number_of_bytes);


#endif /* SRC_JO_SPI_API_H_ */
