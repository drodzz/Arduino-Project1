#include <avr/io.h>
#include <util/delay.h>

// Define pins for LCD
#define LCD_RS PB0
#define LCD_EN PB1
#define LCD_D4 PB2
#define LCD_D5 PB3
#define LCD_D6 PB4
#define LCD_D7 PB5

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

// Function prototypes
void lcd_init();
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void display7Segment(uint8_t combinedState);
void button_init();

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

    while (1) {
        // Read state of push buttons
        uint8_t buttonState1 = PINC & (1 << BUTTON_1);
        uint8_t buttonState2 = PINC & (1 << BUTTON_2);

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

        // Delay for stability
        _delay_ms(100);
    }

    return 0;
}

// Initialize LCD
void lcd_init() {
    DDRB |= (1 << LCD_RS) | (1 << LCD_EN) | (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);
    lcd_cmd(0x02); // Initialize LCD in 4-bit mode
    lcd_cmd(0x28); // 4-bit mode, 2 lines, 5x8 font
    lcd_cmd(0x0C); // Display on, cursor off, blink off
    lcd_cmd(0x01); // Clear display
    _delay_ms(2); // Delay for LCD to execute command
}

// Send command to LCD
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

// Send data to LCD
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

// Initialize push button pins
void button_init() {
    DDRC &= ~((1 << BUTTON_1) | (1 << BUTTON_2)); // Set pins as input
    PORTC |= (1 << BUTTON_1) | (1 << BUTTON_2); // Enable pull-up resistors
}

// Display button states on 7-segment display
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
