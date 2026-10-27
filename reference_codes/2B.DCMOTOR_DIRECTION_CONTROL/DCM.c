/********************************************************************************
 * EXP2B.DCM Direction Control
 * Developed by
 * Advanced Electronics Systems. Bengaluru.
 *-------------------------------------------------------------------------------
 * Controller	: LPC1768
 * Project		: ALS-SDA-ARMCTXM3-06
 
 * Description	:Direction of the DCM is cotrolled in this software by alternatively inter-
 * changing the supply with the help Relay.	Port lines: P1.24 and P0.26. 
 * Port line P1.24 used for the PWM feature.When T0TC matches 
 * the MR0 counts interrupt is generated
 * and Duty cycle will be changed.Depends upon the Duty cycle Motor speed is also changes. 
 * PWM 1.5 is used. Match register 0 is used for count purpose. PWM 1
 * match register 5 is increamented or decreamented at each interrupted.
 ----------------------------------------------------------------------------

*********************************************************************************/

#include <LPC17xx.H>
 
void Clock_Wise(void);
void AClock_Wise(void);

unsigned long i;

int main(void)
{
	SystemInit();
	LPC_PINCON->PINSEL1 = 0x00000000;	//P0.26 GPIO, P0.26 controls dir
	LPC_PINCON->PINSEL3 = 0x00000000;	//P1.24 GPIO
	LPC_GPIO0->FIODIR |= 0x04000000;	//P0.26 output
	LPC_GPIO1->FIODIR |= 0x01000000;	//P1.24 output

	while(1)
	{
		Clock_Wise();
		for(i=0;i<200000;i++);
		AClock_Wise();
		for(i=0;i<200000;i++);

	}		//end while(1)
}			//end main

void Clock_Wise(void)
{
	LPC_GPIO1->FIOCLR = 0x01000000;		//P1.24 Kept low to off DCM
	for(i=0;i<10000;i++);							//delay to componsate inertia
	LPC_GPIO0->FIOSET = 0x04000000;		//coil is on
	LPC_GPIO1->FIOSET = 0x01000000;		//motor in on
	return;	 	
}										//end void Clock_Wise(void)

void AClock_Wise(void)
{
	LPC_GPIO1->FIOCLR = 0x01000000;		//P1.24 Kept low to off DCM
	for(i=0;i<10000;i++);				//delay to componsate inertia
	LPC_GPIO0->FIOCLR = 0x04000000;		//coil is off
	LPC_GPIO1->FIOSET = 0x01000000;		//Motor is on
	return;
}										//end void AClock_Wise(void)

