/*************************************************************************
 * EXP 10:Relay/buzzer/Led test
 * Developed by
 * Advanced Electronics Systems. Bengaluru.
 
 *------------------------------------------------------------------------
 * Controller	: LPC1768
 * Project		: ALS-SDA-ARMCTXM3-06
 
 * Description	: Coil of the relay is pulled up at one side and other side is controlled
 * by the controller via ULN2803 an inverting buffer. When Switch SW4 is Pressed ,high is sent 
 * from the controller for Relay, current flows through the coil. LED gets on and 
 * CN5 pin 1 and 2 gets short as well and low is sent from the controller then Buzzer will ON 
**************************************************************************/

#include <LPC17xx.H>
int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL1 = 0x00000000;								//P0.24,P0.25 GPIO
	LPC_GPIO0->FIODIR |= 0x03000000;								//P0.24,P0.25 output
   while(1)
   {
			if(!(LPC_GPIO2->FIOPIN & 0x00000800))
				{
					LPC_GPIO0->FIOSET = 0x03000000;					//relay on
				}	
			else
				{
					 LPC_GPIO0->FIOCLR = 0x03000000;				//relay off
				}
	 }
}																									//end int main(void)



 