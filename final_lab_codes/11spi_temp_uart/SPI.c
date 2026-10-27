#include <LPC17xx.h>
#include "SPI.h"
unsigned char spi_flag = 0, temp=0;

void SPI_Init(void)
{   
	LPC_PINCON->PINSEL0 |= 0xC0000000;	//P0.15 as SCK 
	LPC_PINCON->PINSEL1 = 0x0000003C;   //select MISO-P0.17,MOSI-P0.18
	LPC_SPI->SPCCR = 0x1E; //  SPI CLOCK SELECTED AS 100KHZ
	LPC_SPI->SPCR = 0xA0; //8 bit data, actve high clk, master SPI mode,SPI Int enable
                         // Master mode and SCK line is active high
	LPC_SPI->SPINT  = 0x01; //clear the interrupt flag
	NVIC_EnableIRQ(SPI_IRQn);
}

void SPI_IRQHandler(void)
{
    spi_flag = 1;
    temp = LPC_SPI->SPSR;   // To clear SPIF bit we have to read status register.
    temp = LPC_SPI->SPDR;   // Then read the data register(optional)
    LPC_SPI->SPINT = 0x01;  // To clear the SPI interrupt   
}
