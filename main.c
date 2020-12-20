/*
 * lcdscreen.c
 *
 * Created: 15/12/2020 13:52:40
 * Author : Jim Marshall
 */ 


#include "main.h"

int main(void)
{
	mainInit();
    while (1) 
    {
		if (pollPushBtn()){
			changeLEDS();
			_delay_ms(LOCK_INPUT_TIME);
		}
		int8_t  c = RotaryGetStatus();
		switch (c)
		{
			case 1:
				handleRotaryLeft();
				RotaryResetStatus();
				break;
			case 2:
				handleRotaryRight();
				RotaryResetStatus();
				break;
			case 3:
				handleRotaryButtonPress();
				RotaryResetStatus();
				break;
		}
    }
}
void mainInit(){
	cli();
	
	buzzerInit();
	lcdInit();
	
	
	Timer2_Start();
	
	sei();
	startupTune();
	cli();
	
	
	lcdSendString("Jim Marshall");
	_delay_ms(2000);
	lcdSendCommand(CLEAR_SCREEN_CMD);
	_delay_ms(5);
	initSequence();
	 
	lcdSendCommand(DDRAM_SET_CMD | DDRAM_LINE_2_START_ADDR); // set ddram address 0x40 - second line
	cursorPos = 0;
	RotaryInit();
	setupSequencerInterrupt();
	setUpPushBtnAndLEDS();
	sei();
}
void initSequence(){
	
	loadSequenceFromEEPROM(onSequence);
	for(char i=0; i<LCD_ROW_LENGTH; i++){
		lcdGoto(i,1);
		lcdSendData(sequences[onSequence][i].character);
	}
}
void handleRotaryLeft(){
	selectedNote = 0;
	cursorPos--;
	if(cursorPos<0){
		cursorPos = LCD_ROW_LENGTH-1;
	}
	lcdSendCommand(DDRAM_SET_CMD | (DDRAM_LINE_2_START_ADDR+cursorPos));

}
void handleRotaryRight(){
	selectedNote = 0;
	cursorPos++;
	if(cursorPos>LCD_ROW_LENGTH-1){
		cursorPos = 0;
	}
	lcdSendCommand(DDRAM_SET_CMD | (DDRAM_LINE_2_START_ADDR+cursorPos));
}
 void handleRotaryButtonPress(){
	 selectedNote++;
	 if(selectedNote > SCALE_LENGTH-1 ){
		 selectedNote = 0;
	 }
	 lcdSendCommand(MOV_CURSOR_LEFT_CMD);
	 lcdSendCommand(MOV_CURSOR_RIGHT_CMD);     // don't know why this is necessary - but it seems to be
	 lcdSendData(scale[selectedNote].character);
	 lcdSendCommand(MOV_CURSOR_LEFT_CMD);
	 
	 sequences[onSequence][cursorPos].frequency = scale[selectedNote].frequency;
	 sequences[onSequence][cursorPos].character = scale[selectedNote].character;

	 updateEEPROMSequence(onSequence);
}
void setupSequencerInterrupt(){
	bpm_overflowCount = (int)((60.0/(float)bpm)*122.0);
	TCCR0A = 0;
	TCCR0B =(1<<CS02);                 //prescaler 256 ~122 interrupts/s
	TIMSK0 =(1<<TOIE0);                //Enable Timer0 Overflow interrupts
}
void loadSequenceFromEEPROM(uint8_t sequence_number){
	eeprom_read_block(sequences[sequence_number],(void*)(sequence_number*sizeof(sequence)),sizeof(sequence));
}
void updateEEPROMSequence(uint8_t sequence_number){
	eeprom_update_block(sequences[sequence_number],(void*)(sequence_number*sizeof(sequence)),sizeof(sequence));
}
void setUpPushBtnAndLEDS(){
	LEDS_DIRECTION |= ((1<<LED1_PIN)|(1<<LED2_PIN)|(1<<LED3_PIN));     // set leds as output
	PUSH_BTN_DIRECTION &= ~(1<<PUSH_BTN_PIN);                        // set push buton as input
	
	PUSH_BTN_PORT |= (1<<PUSH_BTN_PIN);                               // set button pullup 
	LEDS_PORT &= ~((1<<LED1_PIN)|(1<<LED2_PIN)|(1<<LED3_PIN));
	LEDS_PORT |= (1<<(LED1_PIN));
}
char pollPushBtn(){
	if(!(PUSH_BTN_INPUT_REG & (1<<PUSH_BTN_PIN))){
		_delay_ms(DEBOUNCE_TIME);
		if(!(PUSH_BTN_INPUT_REG & (1<<PUSH_BTN_PIN))){
			return 1;
		}
	}
	return 0;
}
void changeLEDS(){
	static signed char LEDNo = 0;
	LEDNo++;
	if (LEDNo >= NUM_LEDS){
		LEDNo = 0;
	}
	LEDS_PORT &= ~((1<<LED1_PIN)|(1<<LED2_PIN)|(1<<LED3_PIN));
	LEDS_PORT |= (1<<(LED1_PIN+LEDNo));
	onSequence = LEDNo;
	lcdSendCommand(CLEAR_SCREEN_CMD);
	_delay_ms(10);
	initSequence();
}
ISR(TIMER0_OVF_vect){
	timer0OverFlows++;
	if(timer0OverFlows > bpm_overflowCount){
		timer0OverFlows = 0;
		sequencerPlayhead++;
		if(sequencerPlayhead >= LCD_ROW_LENGTH){
			sequencerPlayhead = 0;
		}
		if(sequences[onSequence][sequencerPlayhead].character != ' '){
			startTone(sequences[onSequence][sequencerPlayhead].frequency);
		}
		else{
			stopTone();
		}		
		lcdGoto(sequencerPlayhead,0);
		lcdSendData('*');
		if(sequencerPlayhead-1 >=0){
			lcdGoto(sequencerPlayhead-1,0);
			lcdSendData(' ');
		}
		else{
			lcdGoto(LCD_ROW_LENGTH-1,0);
			lcdSendData(' ');
		}
		lcdGoto(cursorPos,1);
	}
}