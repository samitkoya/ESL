
#include <LPC17xx.H>
void Clock_Wise(void);
void AClock_Wise(void);

unsigned long i;
int main(void)
{
	LPC_PINCON->PINSEL1 = 0x00000000;	//P0.26 GPIO, P0.26 controls dir
	LPC_PINCON->PINSEL3 = 0x00000000;	//P1.24 GPIO
	LPC_GPIO0->FIODIR |= 0x04000000;	//P0.26 output
	LPC_GPIO1->FIODIR |= 0x01000000;	//P1.24 output

	while(1)
	{
		Clock_Wise();
		for(i=0;i<300000;i++);
		AClock_Wise();
		for(i=0;i<300000;i++);
	}		//end while(1)
}			//end main

void Clock_Wise(void)
{
	LPC_GPIO1->FIOCLR = 0x01000000;		//P1.24 Kept low to off DCM
	for(i=0;i<10000;i++);							//delay to componsate inertia
	LPC_GPIO0->FIOSET = 0x04000000;		//coil is on
	LPC_GPIO1->FIOSET = 0x01000000;		//motor in on
}										//end void Clock_Wise(void)

void AClock_Wise(void)
{
	LPC_GPIO1->FIOCLR = 0x01000000;		//P1.24 Kept low to off DCM
	for(i=0;i<10000;i++);				//delay to componsate inertia
	LPC_GPIO0->FIOCLR = 0x04000000;		//coil is off
	LPC_GPIO1->FIOSET = 0x01000000;		//Motor is on
}										//end void AClock_Wise(void)

