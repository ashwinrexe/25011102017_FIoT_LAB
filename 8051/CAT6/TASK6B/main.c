/*
 * Experiment: CAT-6 Task 6B
 * Description: Interrupt-Driven UART Receiver & Immediate Echo Loopback
 * Platform: Keil C51, AT89S52 @ 11.0592 MHz
 */

#include <reg51.h>

static volatile unsigned char rx_char_buffer;

/**
 * @brief UART Serial Interrupt Service Routine (Vector Address 0x0023, Interrupt 4)
 */
void isr_serial_comms(void) interrupt 4 {
    // Check if character was received
    if (RI) {
        RI = 0;                     // Clear Receive Interrupt flag
        rx_char_buffer = SBUF;      // Read incoming byte from SBUF
        SBUF = rx_char_buffer;      // Echo character immediately back out
    }

    // Check if character transmission concluded
    if (TI) {
        TI = 0;                     // Clear Transmit Interrupt flag
    }
}

/**
 * @brief Initializes UART peripheral with global and serial interrupts enabled
 */
void uart_echo_init(void) {
    SCON = 0x50;                    // Mode 1: 8-bit UART, Receiver Enabled
    TMOD = (TMOD & 0x0F) | 0x20;    // Timer 1 Mode 2 auto-reload
    TH1  = 0xFD;                    // 9600 Baud at 11.0592 MHz
    TR1  = 1;                       // Run Timer 1

    ES   = 1;                       // Enable Serial Interrupt
    EA   = 1;                       // Enable Global Interrupts
}

void main(void) {
    uart_echo_init();

    while (1) {
        /* Foreground processor remains idle; echo is handled entirely in ISR */
    }
}
