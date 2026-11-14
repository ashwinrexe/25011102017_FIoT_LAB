/*
 * Experiment: CAT-2 Task 2B
 * Description: AT89S52 Toolchain Verification & Basic GPIO Output Toggle (~1 Hz)
 * Platform: Keil C51, AT89S52 Microcontroller @ 11.0592 MHz
 */

#include <reg51.h>

// On-board user LED mapped to Port 1 Pin 0
sbit USER_LED = P1^0;

/**
 * @brief Generates an approximate software delay via nested CPU loop iterations
 * @param cycles Multiplier controlling total delay duration
 */
static void delay_cycles(unsigned int cycles) {
    unsigned int outer, inner;
    for (outer = 0; outer < cycles; outer++) {
        for (inner = 0; inner < 475; inner++) {
            /* Burn CPU machine cycles */
        }
    }
}

/**
 * @brief Configures Port 1 output state and executes the 1 Hz blinking super-loop
 */
void main(void) {
    // Initialize Port 1 pins to LOW state
    P1 = 0x00;

    while (1) {
        USER_LED = 1;         // Drive LED anode HIGH (Active-HIGH ON)
        delay_cycles(500);    // ~500 ms delay
        USER_LED = 0;         // Drive LED anode LOW (Active-HIGH OFF)
        delay_cycles(500);    // ~500 ms delay
    }
}
