#include <lpc17xx.h>
void pwm_init(void);
void PWM1_IRQHandler(void);
unsigned long int i;
unsigned char flag,flag1;

int main(void)
{   
	pwm_init();
	while(1);
}//end of main

void pwm_init(void)
{
	LPC_SC->PCONP = (1<<6);						//PWM1 is powered
	LPC_PINCON->PINSEL3 = 0x00020000;	//pwm1.5 is selected for the pin P1.24  
	LPC_PWM1->PR  = 0x00000000;      	//Count frequency : Fpclk 
	LPC_PWM1->PCR = 0x0002000;      	//select PWM5 single edge and PWM5 output is enabled 
	LPC_PWM1->MCR = 0x00000003;      	//Reset and interrupt on PWMMR0
	LPC_PWM1->MR0 = 60000;           	//setup match register 0 count 
	LPC_PWM1->MR5 = 0x00000100;      	//setup match register MR5 
	LPC_PWM1->LER = 0x000000FF;      	//enable shadow copy register
	LPC_PWM1->TCR = 0x00000002;      	//RESET COUNTER AND PRESCALER
	LPC_PWM1->TCR = 0x00000009;      	//enable PWM and counter
	NVIC_EnableIRQ(PWM1_IRQn);
}
void PWM1_IRQHandler(void)
{
	LPC_PWM1->IR = 0xff; 				//clear the interrupts
	if(flag == 0x00)
    {
			LPC_PWM1->MR5 += 100;
			LPC_PWM1->LER = 0x000000FF;
			if(LPC_PWM1->MR5 >= 57000)						//for 10% of duty cycle MR5=3000,for 50% MR5=15000 ,for 90% MR5= 27000
				{
							flag1 = 0xff;
							flag = 0xff;
							LPC_PWM1->LER = 0x000000fF;
				}	
		 for(i=0;i<8000;i++);
	}
    else if(flag1 == 0xff)
    {
			LPC_PWM1->MR5 -= 100;
			LPC_PWM1->LER = 0x000000fF;
			if(LPC_PWM1->MR5 <= 0x300)					//1% of duty cycle MR5=300
			{
				flag  = 0x00;
				flag1 = 0x00;
				LPC_PWM1->LER = 0X000000fF;
			}
		 for(i=0;i<8000;i++);
	 }
}


