/*
 * Experiment: CAT-7 Task 7B
 * Description: HD44780 16x2 LCD Multi-Line Cursor Control & Integer Formatter
 * Platform: Keil C51, AT89S52 @ 11.0592 MHz
 */

#include <reg51.h>
#include <intrins.h>

#define DISPLAY_ROWS 2
#define DISPLAY_COLS 16

sbit LCD_RS = P3^2;
sbit LCD_EN = P3^3;

static void delay_ms(unsigned int ms) {
    unsigned int p, q;
    for (p = 0; p < ms; p++) {
        for (q = 0; q < 155; q++);
    }
}

static void lcd_latch_nibble(unsigned char nibble, bit is_data) {
    LCD_RS = is_data;
    P3 = (P3 & 0x0F) | ((nibble & 0x0F) << 4);
    LCD_EN = 1;
    _nop_();
    _nop_();
    LCD_EN = 0;
}

void lcd_cmd(unsigned char cmd) {
    lcd_latch_nibble((cmd >> 4) & 0x0F, 0);
    lcd_latch_nibble(cmd & 0x0F, 0);
    delay_ms(2);
}

void lcd_data(char ch) {
    lcd_latch_nibble(((unsigned char)ch >> 4) & 0x0F, 1);
    lcd_latch_nibble((unsigned char)ch & 0x0F, 1);
    delay_ms(1);
}

void lcd_init(void) {
    delay_ms(40);
    lcd_latch_nibble(0x03, 0);
    delay_ms(5);
    lcd_latch_nibble(0x03, 0);
    delay_ms(1);
    lcd_latch_nibble(0x03, 0);
    delay_ms(1);
    lcd_latch_nibble(0x02, 0);
    delay_ms(1);

    lcd_cmd(0x28); // 4-bit, 2 lines
    lcd_cmd(0x0C); // Display ON, Cursor OFF
    lcd_cmd(0x06); // Increment
    lcd_cmd(0x01); // Clear
    delay_ms(3);
}

/**
 * @brief Sets cursor position at specified row and column
 * @param row 0 for top line, 1 for bottom line
 * @param col 0 to 15
 */
void lcd_set_cursor(unsigned char row, unsigned char col) {
    unsigned char addr = (row == 0) ? 0x80 : 0xC0;
    lcd_cmd(addr + (col & 0x0F));
}

void lcd_puts(unsigned char row, unsigned char col, const char *str) {
    lcd_set_cursor(row, col);
    while (*str != '\0') {
        lcd_data(*str++);
    }
}

/**
 * @brief Prints an unsigned integer to the LCD
 */
void lcd_print_int(unsigned int val) {
    char buf[6];
    int idx = 0;

    if (val == 0) {
        lcd_data('0');
        return;
    }

    while (val > 0) {
        buf[idx++] = (val % 10) + '0';
        val /= 10;
    }

    while (idx > 0) {
        lcd_data(buf[--idx]);
    }
}

void main(void) {
    unsigned int loop_counter = 0;

    lcd_init();
    lcd_puts(0, 0, "Counter Monitor");

    while (1) {
        lcd_puts(1, 0, "Count: ");
        lcd_set_cursor(1, 7);
        lcd_print_int(loop_counter);
        lcd_puts(1, 12, " pcs");

        loop_counter++;
        delay_ms(500);
    }
}
