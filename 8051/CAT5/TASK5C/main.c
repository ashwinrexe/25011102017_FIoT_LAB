/*
 * Experiment: CAT-5 Task 5C
 * Description: 4-Phase Stepper Motor Sequence Driver with Matrix Keypad Speed Selection
 * Platform: Keil C51, AT89S52 @ 11.0592 MHz
 */

#include <reg51.h>

// Stepper coil step phase table (Full-Step Wave Drive on P1.0..P1.3)
static const unsigned char COIL_PHASES[4] = {
    0x08, // Coil A
    0x04, // Coil B
    0x02, // Coil C
    0x01  // Coil D
};

static volatile unsigned char phase_ptr = 0;
static volatile unsigned char timer_tick_cnt = 0;
static volatile unsigned char speed_prescaler = 2; // Default speed

/**
 * @brief Scans first row of matrix keypad on Port 2 to select motor speed
 * Keys 1, 2, 3 correspond to High, Medium, and Low speed prescalers
 */
static void check_speed_input(void) {
    P2 = 0xFF;
    P2 &= ~(1 << 7); // Pull Row 0 LOW

    if ((P2 & (1 << 3)) == 0) { // Key 1 pressed -> High Speed
        speed_prescaler = 1;
    } else if ((P2 & (1 << 2)) == 0) { // Key 2 pressed -> Medium Speed
        speed_prescaler = 3;
    } else if ((P2 & (1 << 1)) == 0) { // Key 3 pressed -> Low Speed
        speed_prescaler = 6;
    }
}

/**
 * @brief Timer 0 ISR: Generates stepping pulses at prescaled intervals
 */
void isr_stepper_timer(void) interrupt 1 {
    TH0 = 0xFC; // ~1 ms tick interval
    TL0 = 0x66;

    timer_tick_cnt++;
    if (timer_tick_cnt >= speed_prescaler) {
        timer_tick_cnt = 0;

        // Drive next coil phase to Port 1 lower nibble
        P1 = (P1 & 0xF0) | COIL_PHASES[phase_ptr];

        phase_ptr = (phase_ptr + 1) % 4; // Advance full-step phase
    }
}

void main(void) {
    // Port 1 lower nibble output for motor coils
    P1 = 0x00;

    // Timer 0 in 16-bit mode (Mode 1)
    TMOD = (TMOD & 0xF0) | 0x01;
    TH0 = 0xFC;
    TL0 = 0x66;

    ET0 = 1;
    EA  = 1;
    TR0 = 1; // Start Timer 0

    while (1) {
        check_speed_input(); // Continually poll keypad for dynamic velocity changes
    }
}
