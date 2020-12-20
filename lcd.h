/*
 * lcd.h
 * 
 * lcd character display library for ST7066 lcd controller
 * Created: 15/12/2020 20:18:59
 *  Author: Jim Marshall
 */ 

#ifndef LCD_H_
#define LCD_H_

#include "common.h"

#define LCD_ROW_LENGTH 16
#define LCD_PORT PORTD
#define LCD_PIN PIND
#define LCD_DIR DDRD
#define EN 1
#define RW 2
#define RS 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define MOV_CURSOR_RIGHT_CMD          0b00010100
#define MOV_CURSOR_LEFT_CMD           0b00010000
#define SCROLL_RIGHT_CMD              0b00011100
#define SCROLL_LEFT_CMD               0b00011000
#define CLEAR_SCREEN_CMD              0x01
#define CURSOR_RETURN_HOME_CMD        0x02

#define ENTRY_MODE_SET                0b00000100
#define ENTRY_MODE_SET_ID             1
#define ENTRY_MODE_SET_S              0

#define FUNCTION_SET_CMD              0b00100000 
#define FUNCTION_SET_4_WIRE_BIT       4
#define FUNCTION_SET_N_LINES_BIT      3
#define FUNCTION_SET_FONT_BIT         2

#define ONOFF_CMD                     0b00001000
#define ONOFF_SET_BIT                 2
#define ONOFF_SET_CURSOR_BIT          1
#define ONOFF_SET_BLINK_BIT           0

#define DDRAM_SET_CMD                 0b1000000
#define DDRAM_LINE_2_START_ADDR       0x40
#define DDRAM_LINE_1_START_ADDR       0x00

#define CGRAM_SET_CMD                 0b01000000

void lcdEnableToggle(void);
void checkIfLCDBusy(void);
void lcdSendCommandNibble(unsigned char nibble);
void lcdSendCommand(unsigned char command);
void lcdSendData(unsigned char data);
void lcdSendString( char* string);
void lcdSetCustomCharacter(unsigned char address, unsigned char* data);
void lcdInit(void);
void lcdGoto(uint8_t pos,uint8_t ln);
void lcdSendCommandNoCheck(unsigned char command);


#endif /* LCD_H_ */