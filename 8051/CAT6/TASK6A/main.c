/*
 * Experiment: CAT-6 Task 6A
 * Description: 8051 UART Driver Initialization (9600 Baud) and Polling String Transmission
 * Platform: Keil C51, AT89S52 @ 11.0592 MHz
 */

#include <reg51.h>

/**
 * @brief Configures Timer 1 and UART peripheral for 9600 Baud, 8-N-1 format
 */
void uart_configure(void) {
    // SCON: Mode 1 (8-bit UART), Receiver Enabled (REN = 1) -> 0x50
    SCON = 0x50;

    // TMOD: Configure Timer 1 in Mode 2 (8-bit Auto-Reload)
    TMOD = (TMOD & 0x0F) | 0x20;

    // TH1 reload value for 9600 baud at 11.0592 MHz crystal
    // Baud Rate = (Osc / 384) / (256 - TH1) -> (11059200 / 384) / 3 = 9600
    TH1 = 0xFD;

    TR1 = 1; // Start Timer 1
    TI  = 0; // Clear transmit flag
}

/**
 * @brief Transmits a single character over the UART interface
 * @param ch ASCII character to send
 */
void uart_send_byte(char ch) {
    SBUF = ch;
    while (!TI) {
        /* Wait until hardware sets Transmit Interrupt flag */
    }
    TI = 0; // Clear TI flag for subsequent transmission
}

/**
 * @brief Transmits a null-terminated string over UART
 * @param str Pointer to constant character array
 */
void uart_send_text(const char *str) {
    while (*str != '\0') {
        uart_send_byte(*str++);
    }
}

/**
 * @brief Simple busy-wait delay
 */
static void delay_ms(unsigned int ms) {
    unsigned int x, y;
    for (x = 0; x < ms; x++) {
        for (y = 0; y < 120; y++);
    }
}

void main(void) {
    uart_configure();

    // Send startup message
    uart_send_text("\r\n================================\r\n");
    uart_send_text("  AT89S52 UART Communication Ready \r\n");
    uart_send_text("  Baud Rate: 9600 | 8-N-1        \r\n");
    uart_send_text("================================\r\n");

    while (1) {
        uart_send_text("[HEARTBEAT] Serial Transmit OK\r\n");
        delay_ms(1500);
    }
}
