#include "usart.h"

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
