#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "timer.h"
#include <avr/interrupt.h>

void display7Segment(uint8_t combinedState);

// Define pins for 7-segment display
#define SHIFT_DATA PD2
#define SHIFT_CLOCK PD3
#define SHIFT_LATCH PB6

// Define pins for push buttons
#define BUTTON_1 PC0
#define BUTTON_2 PC1

// Define pins for LEDs
#define LED_1 PD5
#define LED_2 PD6

volatile uint8_t buttonState1 = 0;
volatile uint8_t buttonState2 = 0;

int main() {
    // Set up LCD
    lcd_init();
    
    // Set up shift register pins
    DDRD |= (1 << SHIFT_DATA) | (1 << SHIFT_CLOCK);
    DDRB |= (1 << SHIFT_LATCH);
    
    // Set up push button pins
    button_init();
    
    // Set up LED pins
    DDRD |= (1 << LED_1) | (1 << LED_2);

    // Initialize Timer1
    timer1_init();

    // Enable global interrupts
    sei();

    while (1) {
        // Control LEDs based on button states
        if (buttonState1) {
            PORTD |= (1 << LED_1);
        } else {
            PORTD &= ~(1 << LED_1);
        }

        if (buttonState2) {
            PORTD |= (1 << LED_2);
        } else {
            PORTD &= ~(1 << LED_2);
        }

        // Display button states on 7-segment display
        display7Segment(buttonState1 | (buttonState2 << 1));
    }

    return 0;
}
