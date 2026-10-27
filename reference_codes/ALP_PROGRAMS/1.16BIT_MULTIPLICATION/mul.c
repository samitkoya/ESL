/********************************************************************
 * EXP1: ALP to multiply two 16 bit binary numbers
 * Developed by 
 * Advanced Electronic Systems, Bengaluru
 * ------------------------------------------------------------------
 * Here assembly routine is written for summation purpose. A main code
 * calls the assebly code. A global variable declared at main is C code
 * is loaded at asm code using the variable address. At LCD_DISP
 * conversion to decimal and display on LCD has been done.
 ********************************************************************/


#include <LPC17xx.h>

extern  void MULTIPLY(void);
extern unsigned long int globvar;   //global variable to access at asm code

unsigned long int globvar,result;   
#include "LCD.h"
int main(void)
{ 

	SystemInit();
	MULTIPLY(); //Function Call
	result = globvar;
	LCD_DISP(globvar);
	while(1);

}

