/**********************************************************************
 * EXP7  :PWM Test
 * Developed by
 * Advanced Electronics Systems, Bengaluru
 * --------------------------------------------------------------------
 * Controller	: LPC1768
 * Project		: ALS-SDA-ARMCTXM3-06
 
 * Description	: PWM 1.2 is used. Match register 0 is used for count purpose. PWM 1
 * match register 2 is increamented or decreamented at each interrupted.
 * Port line P3.25.
***********************************************************************/

#include <LPC17xx.H>

void pwm_init(void);
void PWM1_IRQHandler(void);

unsigned long int i;
unsigned char flag,flag1;

int main(void)
{   
	SystemInit();
	SystemCoreClockUpdate();   
	pwm_init();

	while(1);
}//end of main

void pwm_init(void)
{
	LPC_SC->PCONP |= (1<<6);			//PWM1 is powered
	LPC_PINCON->PINSEL7 |= 0x000C0000;	//pwm1.2 is selected for the pin P3.25
	  
	LPC_PWM1->PR  = 0x00000000;      	//Count frequency : Fpclk 
	LPC_PWM1->PCR = 0x00000400;      	//select PWM2 single edge 
	LPC_PWM1->MCR = 0x00000003;      	//Reset and interrupt on PWMMR0
	LPC_PWM1->MR0 = 30000;           	//setup match register 0 count 
	LPC_PWM1->MR2 = 0x00000100;      	//setup match register MR1 
	LPC_PWM1->LER = 0x000000FF;      	//enable shadow copy register
	LPC_PWM1->TCR = 0x00000002;      	//RESET COUNTER AND PRESCALER
	LPC_PWM1->TCR = 0x00000009;      	//enable PWM and counter

	NVIC_EnableIRQ(PWM1_IRQn);
	return;
}

void PWM1_IRQHandler(void)
{
	LPC_PWM1->IR = 0xff; 				//clear the interrupts

	if(flag == 0x00)
    {
		LPC_PWM1->MR2 += 100;
		LPC_PWM1->LER = 0x000000FF;

		if(LPC_PWM1->MR2 >= 27000)
		{
        	flag1 = 0xff;
        	flag = 0xff;
        	LPC_PWM1->LER = 0x000000fF;
		}
	}
    else if(flag1 == 0xff)
    {
		LPC_PWM1->MR2 -= 100;
		LPC_PWM1->LER = 0x000000fF;
    
 		if(LPC_PWM1->MR2 <= 0x500)
		{
			flag  = 0x00;
			flag1 = 0x00;
			LPC_PWM1->LER = 0X000000fF;
		}
	}
}


