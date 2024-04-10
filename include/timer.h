#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>

// Define pins for push buttons
#define BUTTON_1 PC0
#define BUTTON_2 PC1

extern volatile uint8_t buttonState1;
extern volatile uint8_t buttonState2;

void button_init();
void timer1_init();

#endif 
