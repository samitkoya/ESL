#include <LPC17xx.h>
#include <stdio.h>
#include "UART0.h"
#include "SPI.h"
#define VREF		5280 
#define FULLSCALE	4095 
#define SSEL		0x00010000

unsigned char temp1=0x00;
unsigned int up4bits,low8bits;
unsigned char *ptr,*ptr1,adc_val[8],temp_float[12];
unsigned char temp_op[]= "TEMP('C)=";
float analog_val,temp_valK,temp_valC;
volatile unsigned long i=0,j = 0;
unsigned int avg;

int main(void)
{
	LPC_GPIO0->FIODIR |= SSEL;			//CHIPSELECT LINE MADE AS O/P P0.7
	LPC_GPIO0->FIOSET = SSEL;			//if device is powered up with CS low, set high before comm
	UART0_Init();						//Initializes UART0 
	SPI_Init();							//Initializes SPI
	spi_flag=0;

	while(1)
	{			
					for(i=0;i<8;i++)
				{
					LPC_GPIO0->FIOCLR = SSEL;	//make SS low
					for(j=0;j<1000;j++);
	
        	LPC_SPI->SPDR = 0x01;		//Send the start bit
        	while(spi_flag == 0);
        	spi_flag = 0;

        	LPC_SPI->SPDR = 0xC0;		//Select channel1 in single ended mode & MSB first format     
					while(spi_flag == 0);
								spi_flag = 0;
		
        	up4bits = temp;				//temp has SPI data register value 
        	LPC_SPI->SPDR = 0x00;
					while(spi_flag == 0);		//while((temp = S1SPSR) != 0X80);//while(((temp = S1SPSR)& 0x80) != 0X80);//while((flag == 0x00)&& ((temp = S1SPSR) == 0X80));
        	spi_flag = 0;
		
        	low8bits = temp;			//SPI Data Register.
					up4bits = (up4bits & 0x0f)<<8;      	        
					low8bits|=up4bits; 			//combine the lower bits and upper bits
					avg += low8bits;		 
		
					LPC_GPIO0->FIOSET = SSEL;	//make SS high
					for(j=0;j<1000;j++);
				}
				
			avg >>= 3;	
			analog_val = (float) ( avg * (float)VREF)/(float)FULLSCALE ;
			temp_valK = (float)( (analog_val * 273.14)/(2731.4));		 //temperature in deg K
			temp_valC	= temp_valK - 273.14;							 //temperature in deg C
      sprintf(temp_float,"%3.2f",temp_valC);		
			avg = 0x00;		
			ptr = temp_op;
        while(*ptr!='\0')
        {
        	LPC_UART0->THR = *ptr++; 
					while(tx0_flag == 0);
								tx0_flag= 0;
					for(i=0;i<200;i++);
				}    
			ptr1 = temp_float;
        while(*ptr1!='\0')
        {
        	LPC_UART0->THR = *ptr1++; 
						while(tx0_flag == 0);
									tx0_flag= 0;
					for(i=0;i<200;i++);
	    }  		
        for(j=0;j<65000;j++); 				//delay for display // for 1 sec value 400000//for 10000 counts 25msec.
		LPC_UART0->THR = 0x0D; 				//new line
    while(tx0_flag == 0);
		tx0_flag= 0;		
			
    }
}
