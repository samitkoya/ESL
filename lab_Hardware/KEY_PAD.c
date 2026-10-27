#include <LPC17xx.h>
#include "lcd.h"
void scan(void);

unsigned char Msg1[14] = "ALS BENGALURU";
unsigned char Msg2[13] = "KEY PRESSED=";
unsigned char col,row,var,flag,key,*ptr;
unsigned long int i,var1,temp,temp3;

unsigned char SCAN_CODE[16] = {0x1E,0x1D,0x1B,0x17,
 							  	0x2E,0x2D,0x2B,0x27,
							  	0x4E,0x4D,0x4B,0x47,
							  	0x8E,0x8D,0x8B,0x87};

unsigned char ASCII_CODE[16] = {'0','1','2','3',
 								 '4','5','6','7',
								 '8','9','A','B',
								 'C','D','E','F'};

int main(void)
{
	LPC_PINCON->PINSEL3 = 0x00000000; 			//P1.20 to P1.23 MADE GPIO
	LPC_PINCON->PINSEL0 = 0x00000000;  			//P0.15 as GPIO
	LPC_PINCON->PINSEL1 = 0x00000000; 			//P0.16 t0 P0.18 made GPIO
	LPC_GPIO0->FIODIR &= ~0x00078000; 			//made INput P0.15 to P0.18 (cols)
	LPC_GPIO1->FIODIR |= 0x00F00000; 				//made output P1.20 to P1.23 (rows)
	LPC_GPIO1->FIOSET = 0x00F00000;  

	lcd_init();

	temp1 = 0x80;														//point to first line of LCD
	lcd_com();
	delay_lcd(800);
	lcd_puts(&Msg1[0]);	 										//display the messsage

	temp1 = 0xC0;														//point to first line of LCD
	lcd_com();
	delay_lcd(800);
	lcd_puts(&Msg2[0]);	 										//display the messsage

	while(1)
	{
		while(1)
		{
			for(row=1;row<5;row++)
			{
				if(row == 1)
				var1 = 0x00100000;
				else if(row == 2)
				var1 = 0x00200000;
				else if(row == 3)
				var1 = 0x00400000;
				else if(row == 4)
				var1 = 0x00800000;
			
				temp = var1;

				LPC_GPIO1->FIOSET = 0x00F00000;
				LPC_GPIO1->FIOCLR = var1;

				flag = 0;
				scan();
				if(flag == 1)
				break;
		
			} 																	//end for(row=1;row<5;row++)

			if(flag == 1)
			break;
		
		}							 												//2nd while(1)

		for(i=0;i<16;i++)
		{
			if(key == SCAN_CODE[i])
			{
				key = ASCII_CODE[i];
				break;
			} 																//end if(key == SCAN_CODE[i])

		}																		//end for(i=0;i<16;i++)

		temp1 = 0xCC;
		lcd_com();
		delay_lcd(800);
		lcd_puts(&key);
		
	}																		//end while 1
}																			//end main

void scan(void)
{
 	unsigned long temp3;
	temp3 = LPC_GPIO0->FIOPIN;	
	temp3 &= 0x00078000;
	if(temp3 != 0x00078000)
	{
		for(i=0;i<500;i++);
		temp3 = LPC_GPIO0->FIOPIN;	
		temp3 &= 0x00078000;
		if(temp3 != 0x00078000)
		{
			flag = 1;
			temp3 >>= 15;									//Shifted to come at LN of byte
			temp >>= 16;									//shifted to come at HN of byte
			key = temp3|temp;	
		}																//2nd if(temp3 != 0x00000000)
	}																	//1st if(temp3 != 0x00000000)
}	








/////



#include "lpc17xx.h"

// Hardware Pin Configuration for ALS-SDA-ARMCTXM3-06 Board Layout
#define RS_PIN      (1 << 27)   // P0.27 -> Register Select (RS)
#define EN_PIN      (1 << 28)   // P0.28 -> Enable Pulse (EN) Latch
#define DATA_MASK   (0xF << 23) // P0.23, P0.24, P0.25, P0.26 -> D4 to D7 Data Bus

// Independent Buzzer Pin Selection
#define BUZZER_PIN  (1 << 25)   // P0.25 -> Connected to Onboard Buzzer

// Lab-safe millisecond delay loop (Prevents Watchdog Resets)
void delay_ms(uint32_t ms) {
    volatile uint32_t i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 5000; j++) {
            __NOP(); 
        }
    }
}

// Latches data frames into the LCD display screen
void lcd_latch(void) {
    LPC_GPIO0->FIOSET = EN_PIN;
    delay_ms(2);
    LPC_GPIO0->FIOCLR = EN_PIN;
    delay_ms(2);
}

// Sends a 4-bit nibble safely to the LCD tracks
void lcd_send_nibble(uint8_t nibble) {
    // Read the current state of Port 0, keep all other pins (like Buzzer) intact
    uint32_t current_port_state = LPC_GPIO0->FIOPIN;
    
    // Clear only the LCD data pins (P0.23 to P0.26)
    current_port_state &= ~DATA_MASK;
    
    // Inject the new 4-bit nibble into the P0.23-P0.26 positions
    current_port_state |= ((nibble & 0x0F) << 23);
    
    // Write back to the port output register
    LPC_GPIO0->FIOPIN = current_port_state;
    
    lcd_latch();
}

void lcd_command(uint8_t cmd) {
    LPC_GPIO0->FIOCLR = RS_PIN; // RS = 0 (Command Mode)
    lcd_send_nibble(cmd >> 4);  // High Nibble
    lcd_send_nibble(cmd);       // Low Nibble
}

void lcd_char(char data) {
    LPC_GPIO0->FIOSET = RS_PIN; // RS = 1 (Data Mode)
    lcd_send_nibble(data >> 4);  // High Nibble
    lcd_send_nibble(data);       // Low Nibble
}

void lcd_print(char *str) {
    while(*str) {
        lcd_char(*str++);
    }
}

void lcd_gotoxy(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? (0x80 + col) : (0xC0 + col);
    lcd_command(address);
}

// Triggers a sharp, distinct beep profile
void beep(uint32_t duration_ms) {
    LPC_GPIO0->FIOSET = BUZZER_PIN; // Turn Buzzer ON
    delay_ms(duration_ms);
    LPC_GPIO0->FIOCLR = BUZZER_PIN; // Turn Buzzer OFF
}

void lcd_init(void) {
    // Configure LCD pins and Buzzer pin as outputs
    LPC_GPIO0->FIODIR |= (RS_PIN | EN_PIN | DATA_MASK | BUZZER_PIN);
    
    // Clear states initially (Buzzer off, LCD clear)
    LPC_GPIO0->FIOCLR = (RS_PIN | EN_PIN | DATA_MASK | BUZZER_PIN);
    delay_ms(50); 

    // 4-Bit initialization sequence
    lcd_send_nibble(0x03);
    delay_ms(5);
    lcd_send_nibble(0x03);
    delay_ms(2);
    lcd_send_nibble(0x03);
    lcd_send_nibble(0x02); 

    lcd_command(0x28); // 4-bit mode, 2-line display configuration
    lcd_command(0x0C); // Display ON, Cursor hidden
    lcd_command(0x06); // Auto shift cursor rightward
    lcd_command(0x01); // Clear Display RAM
    delay_ms(5);
}

int main(void) {
    SystemInit(); // Setup core microchip clock parameters
    lcd_init();   // Run the initialization

    // 1. Distinct 3-Beep Startup Pattern 
    beep(150); delay_ms(100);
    beep(150); delay_ms(100);
    beep(400); // Final long beep notification

    // 2. Output text directly to the screen matrix
    lcd_gotoxy(0, 1); 
    lcd_print("MIT Bengaluru");

    lcd_gotoxy(1, 4); 
    lcd_print("MAHE Tech");

    while(1) {
        // Text sits securely on screen without interfering with the buzzer
    }
}																	//end scan

