#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "timer.h"
#include "PIN_DEFS.h"
#include "usart.h" 

void display7Segment(uint8_t combinedState);

int main() {
    // Initialize USART
    usart_init(); // Call the USART initialization function

    // Set up LCD
    lcd_init();
    
    // Set up shift register pins
    DDRD |= (1 << SHIFT_DATA) | (1 << SHIFT_CLOCK);
    DDRB |= (1 << SHIFT_LATCH);

    // Set up buzzer pin
    DDRD |= (1 << BUZZER_PIN);
    
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

        // Transmit received USART data to LCD
        char received_char = usart_receive(); // Receive data from USART
        if (received_char != '\0') {
            lcd_data(received_char);
        }

        if (buttonState1) {
            PORTD |= (1 << LED_1); // Turn on LED 1
            PORTD |= (1 << BUZZER_PIN); // Turn on the buzzer
        } else {
            PORTD &= ~(1 << LED_1); // Turn off LED 1
            PORTD &= ~(1 << BUZZER_PIN); // Turn off the buzzer
        }
    }

    return 0;
}
