/*
 * main.h
 *
 * Created: 19/12/2020 15:43:20
 *  Author: User1
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include "lcd.h"
#include "rotaryencoder.h"
#include "buzzer.h"
#include <avr/eeprom.h>

#define LEDS_PORT PORTB
#define LEDS_DIRECTION DDRB
#define LEDS_
#define LED1_PIN 2
#define LED2_PIN 3
#define LED3_PIN 4
#define NUM_LEDS 3

#define PUSH_BTN_PORT PORTB
#define PUSH_BTN_DIRECTION DDRB
#define PUSH_BTN_INPUT_REG PINB
#define PUSH_BTN_PIN PB5
#define DEBOUNCE_TIME 25
#define LOCK_INPUT_TIME 300

#define SCALE_LENGTH 8

typedef struct{
	char character;
	float frequency;
} Note;

const Note scale[] ={{' ', 0.0},{'b', 246.94},{'c', 261.63},{'d', 293.66},{'e', 329.63},{'f',  349.23},{'g',  392.00}, {'a',440.0}};

Note sequence[LCD_ROW_LENGTH];
Note sequence1[LCD_ROW_LENGTH] = {{' ',0.0},{' ',0.0},{' ',0.0},{' ',0.0},{' ',0.0},{' ',0.0},{' ',0.0},{' ',0.0},{' ',0.0},{' ',0.0},{' ',0.0},{' ',0.0},{' ',0.0},{' ',0.0},{' ',0.0},{' ',0.0}};
Note sequence2[LCD_ROW_LENGTH];
Note* sequences[] = {sequence,sequence1,sequence2};
	
char selectedNote = 0;                      // for selecting note using rotary push button

       
volatile char sequencerPlayhead = 0;        // which note is sequencer currently on
volatile unsigned int timer0OverFlows = 0;  // current number of timer0 overflows at scalefactor 256 

int bpm_overflowCount;                      // number of overflows at scalefactor 256 before next note is moved to.
uint8_t bpm = 180;
signed char cursorPos = 0;  
unsigned char onSequence = 0;

void handleRotaryLeft(void);
void handleRotaryRight(void);
void handleRotaryButtonPress(void);
void initSequence(void);
void setupSequencerInterrupt();
void mainInit(void);
void loadSequenceFromEEPROM(uint8_t sequence_number);
void updateEEPROMSequence(uint8_t sequence_number);
void setUpPushBtnAndLEDS(void);
char pollPushBtn(void);
void changeLEDS(void);


#endif /* MAIN_H_ */