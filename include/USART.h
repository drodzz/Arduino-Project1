#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <avr/interrupt.h>

// USART initialization function
void usart_init();

// USART transmit function
void usart_transmit(char data);

// USART receive function
char usart_receive();

#endif 
