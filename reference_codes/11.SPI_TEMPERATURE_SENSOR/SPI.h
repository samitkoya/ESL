//Header file for SPI.c file

#ifndef __SPI_H
#define __SPI_H
	extern unsigned char spi_flag,temp;
	void SPI_Init(void);// SPI initialisation
	void SPI_IRQHandler(void);//SPI Interrupt routine
#endif
