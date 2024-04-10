#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

void lcd_init() {
    DDRB |= (1 << LCD_RS) | (1 << LCD_EN) | (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);
    lcd_cmd(0x02); // Initialize LCD in 4-bit mode
    lcd_cmd(0x28); // 4-bit mode, 2 lines, 5x8 font
    lcd_cmd(0x0C); // Display on, cursor off, blink off
    lcd_cmd(0x01); // Clear display
    _delay_ms(2); // Delay for LCD to execute command
}

void lcd_cmd(unsigned char cmd) {
    PORTB &= ~(1 << LCD_RS); // RS low for command mode
    PORTD = (cmd >> 4) & 0x0F; // Send high nibble
    PORTB |= (1 << LCD_EN); // Enable pulse
    _delay_us(1); // Delay for enable pulse width
    PORTB &= ~(1 << LCD_EN); // Disable pulse
    _delay_us(100); // Delay for command execution
    PORTD = cmd & 0x0F; // Send low nibble
    PORTB |= (1 << LCD_EN); // Enable pulse
    _delay_us(1); // Delay for enable pulse width
    PORTB &= ~(1 << LCD_EN); // Disable pulse
    _delay_ms(2); // Delay for command execution
}

void lcd_data(unsigned char data) {
    PORTB |= (1 << LCD_RS); // RS high for data mode
    PORTD = (data >> 4) & 0x0F; // Send high nibble
    PORTB |= (1 << LCD_EN); // Enable pulse
    _delay_us(1); // Delay for enable pulse width
    PORTB &= ~(1 << LCD_EN); // Disable pulse
    _delay_us(100); // Delay for data execution
    PORTD = data & 0x0F; // Send low nibble
    PORTB |= (1 << LCD_EN); // Enable pulse
    _delay_us(1); // Delay for enable pulse width
    PORTB &= ~(1 << LCD_EN); // Disable pulse
    _delay_ms(2); // Delay for data execution
}
