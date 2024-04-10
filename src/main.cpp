#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "timer.h"
#include "PIN_DEFS.h"

void display7Segment(uint8_t combinedState);

// Global variable to hold received USART data
volatile char received_char = '\0';

// USART initialization function
void usart_init() {
    // Set baud rate to 9600
    UBRR0H = (uint8_t)(103 >> 8);
    UBRR0L = (uint8_t)(103);

    // Enable receiver and transmitter
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);

    // Enable USART receive interrupt
    UCSR0B |= (1 << RXCIE0);
}

// USART transmit function
void usart_transmit(char data) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    // Put data into buffer, sends the data
    UDR0 = data;
}

// USART receive function
char usart_receive() {
    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0)))
        ;
    // Get and return received data from buffer
    return UDR0;
}

// USART receive interrupt service routine
ISR(USART_RX_vect) {
    // Received data
    received_char = UDR0;
}

int main() {
    // Initialize USART
    usart_init();

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

        // Transmit received USART data to LCD
        if (received_char != '\0') {
            lcd_data(received_char);
            received_char = '\0'; // Reset received_char
        }
    }

    return 0;
}
