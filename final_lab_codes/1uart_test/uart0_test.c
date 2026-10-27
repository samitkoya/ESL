#include<LPC17xx.h>
void delay(unsigned int r1);
void UART0_Init(void);

unsigned int  i ;
unsigned char *ptr, arr[] = "Hello world\r\n";
#define THR_EMPTY 0x20											//U0THR is Empty 
int main(void)
{
	UART0_Init();
	while(1)
	{
		ptr = arr;
		while ( *ptr != '\0')
		{ 
			while ((LPC_UART0->LSR & THR_EMPTY) != THR_EMPTY) ; //Is U0THR is EMPTY??
			LPC_UART0->THR = *ptr++;			
		}
		for(i=0;i<=80000;i++);
	} 
}

void UART0_Init(void)
{
	LPC_SC->PCONP |= 0x00000008;			//UART0 peripheral enable
	LPC_PINCON->PINSEL0 = 0x00000050;
	LPC_UART0->LCR = 0x00000083;			//enable divisor latch, parity disable, 1 stop bit, 8bit word length
	LPC_UART0->DLM = 0X00; 
	LPC_UART0->DLL = 0x1A;      			//select baud rate 9600 bps for 4Mhz
	LPC_UART0->LCR = 0X00000003;			//Disable divisor latch
	LPC_UART0->FCR = 0x07;						//FIFO enable,RX FIFO reset,TX FIFO reset
}


