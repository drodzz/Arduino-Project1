#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"


// Global variable to count the number of milliseconds
volatile uint16_t lcd_ms_count = 0;

void display7Segment(uint8_t combinedState);

void lcd_init() {
    DDRB |= (1 << LCD_RS) | (1 << LCD_EN) | (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);
    lcd_cmd(0x02); // Initialize LCD in 4-bit mode
    lcd_cmd(0x28); // 4-bit mode, 2 lines, 5x8 font
    lcd_cmd(0x0C); // Display on, cursor off, blink off
    lcd_cmd(0x01); // Clear display
    while (lcd_ms_count < 2); // Wait for 2 milliseconds
}

void lcd_cmd(unsigned char cmd) {
    PORTB &= ~(1 << LCD_RS); // RS low for command mode
    lcd_send_nibble(cmd >> 4); // Send high nibble
    lcd_trigger();
    lcd_send_nibble(cmd & 0x0F); // Send low nibble
    lcd_trigger();
    while (lcd_ms_count < 2); // Wait for 2 milliseconds
}

void lcd_data(unsigned char data) {
    PORTB |= (1 << LCD_RS); // RS high for data mode
    lcd_send_nibble(data >> 4); // Send high nibble
    lcd_trigger();
    lcd_send_nibble(data & 0x0F); // Send low nibble
    lcd_trigger();
    while (lcd_ms_count < 2); // Wait for 2 milliseconds
}

void lcd_send_nibble(unsigned char nibble) {
    PORTD = (PORTD & 0xF0) | (nibble & 0x0F); // Clear lower nibble and set new value
}

void lcd_trigger() {
    PORTB |= (1 << LCD_EN); // Enable pulse
    PORTB &= ~(1 << LCD_EN); // Disable pulse
}

// Timer1 overflow interrupt service routine
ISR(TIMER1_OVF_vect) {
    lcd_ms_count++; // Increment milliseconds count
}

// Initialize Timer1 for generating precise delays
void timer1_init() {
    // Set prescaler to 64
    TCCR1B |= (1 << CS11) | (1 << CS10);

    // Enable overflow interrupt
    TIMSK1 |= (1 << TOIE1);

    // Initialize counter
    TCNT1 = 0;
}

// Function definition for display7Segment
void display7Segment(uint8_t combinedState) {
    // Create an array to hold segment patterns for digits 0-3
    uint8_t segmentPatterns[] = {
        0b01111110, // 0
        0b00110000, // 1
        0b01101101, // 2
        0b01111001  // 3
    };

    // Shift out segment pattern for the combined digit
    PORTB &= ~(1 << SHIFT_LATCH); // Latch low
    PORTD = segmentPatterns[combinedState]; // Send segment pattern
    PORTB |= (1 << SHIFT_LATCH); // Latch high
}
