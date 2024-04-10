#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

void button_init() {
    DDRC &= ~((1 << BUTTON_1) | (1 << BUTTON_2)); // Set pins as input
    PORTC |= (1 << BUTTON_1) | (1 << BUTTON_2); // Enable pull-up resistors
}

void timer1_init() {
    // Set prescaler to 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);

    // Initialize counter
    TCNT1 = 0;

    // Enable overflow interrupt
    TIMSK1 |= (1 << TOIE1);
}

ISR(TIMER1_OVF_vect) {
    // Read state of push buttons
    buttonState1 = PINC & (1 << BUTTON_1);
    buttonState2 = PINC & (1 << BUTTON_2);
}
