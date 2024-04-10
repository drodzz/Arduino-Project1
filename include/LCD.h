#ifndef LCD_H_
#define LCD_H_

// Define pins for LCD
#define LCD_RS PB0
#define LCD_EN PB1
#define LCD_D4 PB2
#define LCD_D5 PB3
#define LCD_D6 PB4
#define LCD_D7 PB5

void lcd_init();
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);

#endif /* LCD_H_ */
