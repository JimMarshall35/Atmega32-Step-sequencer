# Atmega32-Step-sequencer

This is C code for a step sequencer using a standard lcd screen module, a rotary encoder with a push button, a buzzer, a button and three (or more) LEDs.

The sequence and playhead is displayed on the LCD screen and the notes are changeable using the rotary encoders push button.

notes are generated by a passive buzzer being toggled high and low by a hardware timer interrupt.

This current design has three different patterns which are saved in eeprom and selectable with the push button. which pattern is selected is shown on the LEDS.

there should be space for 12 patterns in the eeprom, the way the patterns are currently stored.

Currently, before this code in its entirity will work you must first set the eeprom of your chip to this repeating pattern:

(block of 16 for each pattern you want avaliable)

(ascii byte for space)

(float value 0)

(repeated * 16) 



This repository is an Atmel studio 7 project

TODO:
 * change from buzzer to DAC sound generation (when i have the parts to make it)
 * give each pattern its own button
 * get a potentiometer that can be used to change tempo 
 * get the custom character feature of the LCD display to work
 * minimise number of eeprom reads / updates
 
