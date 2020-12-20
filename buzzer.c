/*
 * buzzer.c
 *
 * Created: 18/12/2020 13:30:15
 *  Author: Jim Marshall
 */ 
#include "buzzer.h"
void buzzerInit(){
	BUZZER_DIRECTION |= (1<<BUZZER_PIN);
	TCCR1A |= (1<<WGM12);                   // set ctc mode for timer 1 (counter resets after match with our value)
	
	TCCR1B |= ((1<<CS10) | (1<<WGM12));     // set prescaler to 1
	
	stopTone();
}
void stopTone(){
	TIMSK1 &= ~(1<<OCIE1A);
}
void startTone(float frequency){
	OCR1A = (int)((1.0/frequency)*F_CPU);
	TIMSK1 |= (1<<OCIE1A); // enable timer 1 compare match interrupt
}
ISR(TIMER1_COMPA_vect){
	BUZZER_PORT ^= (1<<BUZZER_PIN);
}
void startupTune(){
	startTone(440);
	_delay_ms(200);
	startTone(466.1638);
	_delay_ms(200);
	startTone(493.8833);
	_delay_ms(200);
	startTone(523.2511);
	_delay_ms(400);
	stopTone();
}