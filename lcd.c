/*
 * lcd.c
 *
 * Created: 18/12/2020 13:37:42
 *  Author: Jim Marshall
 */ 
#include "lcd.h"
void lcdEnableToggle(){
	PORTD |= (1<<EN);
	asm volatile("nop");
	asm volatile("nop");
	PORTD &= ~(1<<EN);
}
void lcdInit()
{
	 
	  
	LCD_DIR = 0xff;
	
	_delay_ms(30);
	lcdSendCommandNoCheck(FUNCTION_SET_CMD | (1<<FUNCTION_SET_N_LINES_BIT));//0b00101000); // function Set -
	_delay_ms(20);
	lcdSendCommandNoCheck(ENTRY_MODE_SET );
	_delay_ms(20);
	
	lcdSendCommandNoCheck(ONOFF_CMD | (1<<ONOFF_SET_BIT) | (1<<ONOFF_SET_CURSOR_BIT) | (1<<ONOFF_SET_BLINK_BIT));//0b00001111); // Display ON/OFF
	
	_delay_ms(5);
	lcdSendCommandNoCheck(CLEAR_SCREEN_CMD);      // clear display
	_delay_ms(20);
	
}
void lcdSendCommandNoCheck(unsigned char command){
	//checkIfLCDBusy();
	
	char hi = command >> 4;
	char low = command & 0xf;
	LCD_PORT = hi << 4;
	LCD_PORT &= ~(1<<RS | 1<<RW); // set RS and RW low
	lcdEnableToggle();
	checkIfLCDBusy();
	LCD_PORT = low << 4;
	LCD_PORT &= ~(1<<RS | 1<<RW); // set RS and RW low
	lcdEnableToggle();
}
void lcdSendCommand(unsigned char command){
	checkIfLCDBusy();
	
	char hi = command >> 4;
	char low = command & 0xf;
	LCD_PORT = hi << 4;
	LCD_PORT &= ~(1<<RS | 1<<RW); // set RS and RW low
	lcdEnableToggle();
	checkIfLCDBusy();
	LCD_PORT = low << 4;
	LCD_PORT &= ~(1<<RS | 1<<RW); // set RS and RW low
	lcdEnableToggle();
}
void lcdSendCommandNibble(unsigned char nibble){
	checkIfLCDBusy();
	LCD_PORT = nibble << 4;
	LCD_PORT &= ~(1<<RS | 1<<RW); // set RS and RW low
	
	lcdEnableToggle();
}
void lcdSendData(unsigned char data){
	checkIfLCDBusy();
	
	char hi = data >> 4;
	char low = data & 0x0f;
	LCD_PORT = hi << 4;
	LCD_PORT &= ~(1<<RW); // set RW low
	LCD_PORT |= (1<<RS);  // set RS high
	lcdEnableToggle();
	checkIfLCDBusy();
	LCD_PORT = low << 4;
	LCD_PORT &= ~(1<<RW); // set RW low
	LCD_PORT |= (1<<RS);  // set RS high
	lcdEnableToggle();
}
void checkIfLCDBusy(){      
	_delay_us(50);                                               // datasheed lists 37.5 uS for most commands
    //LCD_PORT |= (1<<D7);
	//LCD_DIR &= ~((1<<D4) | (1<<D5) | (1<<D6) | (1<<D7));       // <- can't get this checking busy flag method to work - just use delay
	//
	//LCD_PORT |= (1<<RW);  // set RW high
	//LCD_PORT &= ~(1<<RS); // set RS low
	//
	//
	//while(1){
		//
		//PORTD |= (1<<EN);
		//asm volatile("nop");
		//asm volatile("nop");
		//PORTD &= ~(1<<EN);
		//if((LCD_PIN & (1<<D7)) > 0)
			//break;
		//
	//}
	//PORTD &= ~(1<<EN);
	//LCD_DIR = 0xff;
}
void lcdSendString( char* string){
	while(*string > 0)
	{
		lcdSendData(*string++);
	}
}
void lcdSetCustomCharacter(unsigned char address, unsigned char* data){

	lcdSendCommand(CGRAM_SET_CMD | (address+(address*8)));
	
	for(int i=0; i<7; i++){
		lcdSendData(data[i]);
	}
	lcdSendCommand(DDRAM_SET_CMD | DDRAM_LINE_1_START_ADDR);
}
void lcdGoto(uint8_t pos,uint8_t ln) //Function ripped from Microchip Technology Inc. LCD library
{
	// if incorrect line or column
	if ((ln > (2-1)) || (pos > (16-1)))
	{
		// Just do nothing
		return;
	}

	// LCD_Goto command
	lcdSendCommand((ln == 1) ? (0xC0 | pos) : (0x80 | pos));

	// Wait for the LCD to finish
	_delay_ms(15);
}