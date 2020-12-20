/*
 * buzzer.h
 *
 * Created: 16/12/2020 21:44:18
 *  Author: Jim Marshall
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_
#include "common.h"


#define GET_0CR1A_VAL(freq) ((1.0/freq)*F_CPU)
#define BUZZER_PORT PORTB
#define BUZZER_DIRECTION DDRB
#define BUZZER_PIN 1

void buzzerInit(void);
void stopTone(void);
void startupTune(void);
void startTone(float frequency);


#endif /* BUZZER_H_ */