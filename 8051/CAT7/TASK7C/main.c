/*
 * Experiment: CAT-7 Task 7C
 * Description: Dynamic Cursor Animation & Scrolling Glyph Traverser on 16x2 LCD
 * Platform: Keil C51, AT89S52 @ 11.0592 MHz
 */

#include <reg51.h>
#include <intrins.h>

#define DISPLAY_ROWS 2
#define DISPLAY_COLS 16
#define TRAVERSER_CHAR '#'

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

    lcd_cmd(0x28);
    lcd_cmd(0x0C); // Display ON, Cursor OFF
    lcd_cmd(0x06);
    lcd_cmd(0x01); // Clear
    delay_ms(3);
}

void lcd_set_cursor(unsigned char row, unsigned char col) {
    unsigned char addr = (row == 0) ? 0x80 : 0xC0;
    lcd_cmd(addr + (col & 0x0F));
}

void main(void) {
    unsigned char row = 0;
    unsigned char col = 0;

    lcd_init();

    while (1) {
        // Draw traverser glyph at current coordinate
        lcd_set_cursor(row, col);
        lcd_data(TRAVERSER_CHAR);

        delay_ms(180);

        // Clear previous position
        lcd_set_cursor(row, col);
        lcd_data(' ');

        // Advance column across display width
        col++;
        if (col >= DISPLAY_COLS) {
            col = 0;
            row ^= 1; // Toggle row (0 -> 1 -> 0)
        }
    }
}
