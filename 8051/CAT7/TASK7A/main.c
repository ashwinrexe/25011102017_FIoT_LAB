/*
 * Experiment: CAT-7 Task 7A
 * Description: HD44780 16x2 Character LCD 4-Bit Driver & Static String Display
 * Platform: Keil C51, AT89S52 @ 11.0592 MHz
 * Wiring: RS = P3.2, EN = P3.3, Data Bus = P3.4..P3.7
 */

#include <reg51.h>
#include <intrins.h>

sbit LCD_RS = P3^2;
sbit LCD_EN = P3^3;

static void delay_ms(unsigned int ms) {
    unsigned int p, q;
    for (p = 0; p < ms; p++) {
        for (q = 0; q < 155; q++);
    }
}

/**
 * @brief Latches a 4-bit nibble onto P3.4..P3.7 and strobes the Enable pin
 */
static void lcd_latch_nibble(unsigned char nibble, bit is_data) {
    LCD_RS = is_data;
    // Map lower 4 bits of parameter into upper nibble of Port 3 (P3.4..P3.7)
    P3 = (P3 & 0x0F) | ((nibble & 0x0F) << 4);

    // Generate Enable strobe pulse (>450 ns high time)
    LCD_EN = 1;
    _nop_();
    _nop_();
    _nop_();
    LCD_EN = 0;
}

/**
 * @brief Sends an 8-bit command byte as two 4-bit nibbles
 */
void lcd_write_command(unsigned char cmd) {
    lcd_latch_nibble((cmd >> 4) & 0x0F, 0); // Send upper nibble
    lcd_latch_nibble(cmd & 0x0F, 0);        // Send lower nibble
    delay_ms(2);
}

/**
 * @brief Sends an 8-bit data character to LCD DDRAM
 */
void lcd_write_char(char ascii_val) {
    lcd_latch_nibble(((unsigned char)ascii_val >> 4) & 0x0F, 1);
    lcd_latch_nibble((unsigned char)ascii_val & 0x0F, 1);
    delay_ms(1);
}

/**
 * @brief Initializes HD44780 controller into 4-bit, 2-line, 5x8 font mode
 */
void lcd_setup(void) {
    delay_ms(50); // Power-on settling delay

    // Reset sequence for 4-bit interface
    lcd_latch_nibble(0x03, 0);
    delay_ms(5);
    lcd_latch_nibble(0x03, 0);
    delay_ms(1);
    lcd_latch_nibble(0x03, 0);
    delay_ms(1);
    lcd_latch_nibble(0x02, 0); // Enter 4-bit mode
    delay_ms(1);

    lcd_write_command(0x28); // 4-bit, 2 lines, 5x8 matrix
    lcd_write_command(0x0C); // Display ON, Cursor OFF, Blink OFF
    lcd_write_command(0x06); // Auto-increment cursor, no display shift
    lcd_write_command(0x01); // Clear display screen
    delay_ms(3);
}

void lcd_print_str(const char *msg) {
    while (*msg != '\0') {
        lcd_write_char(*msg++);
    }
}

void main(void) {
    lcd_setup();

    // Display static lab messages
    lcd_write_command(0x80); // Line 1 start address
    lcd_print_str("FIoT LAB - 8051");

    lcd_write_command(0xC0); // Line 2 start address
    lcd_print_str("LCD DRIVER READY");

    while (1) {
        /* Static display maintained */
    }
}
