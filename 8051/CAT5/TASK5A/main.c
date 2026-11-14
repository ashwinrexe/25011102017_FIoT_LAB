/*
 * Experiment: CAT-5 Task 5A
 * Description: Hardware Timer 0 Interrupt-Driven Software PWM for LED Dimming & Fading
 * Platform: Keil C51, AT89S52 @ 11.0592 MHz
 */

#include <reg52.h>

// Software PWM counters
static volatile unsigned char pwm_phase = 0;
static volatile unsigned char target_duty = 0;
static volatile unsigned char step_prescaler = 0;
static volatile unsigned char ramp_up = 1;

// LED output on Port 1 Pin 0
sbit FADE_LED = P1^0;

/**
 * @brief Timer 0 ISR - fires at ~100 us intervals for fine-grained software PWM
 */
void isr_timer0(void) interrupt 1 {
    // Reload Timer 0 for ~100 us tick interval (11.0592 MHz crystal)
    TH0 = 0xFF;
    TL0 = 0xF7;

    // Pulse-Width Modulation pin update
    if (pwm_phase < target_duty) {
        FADE_LED = 1;
    } else {
        FADE_LED = 0;
    }

    pwm_phase++;
    if (pwm_phase >= 100) {
        pwm_phase = 0;
        step_prescaler++;

        // Duty cycle ramping logic evaluated every 10 complete PWM cycles (~10 ms)
        if (step_prescaler >= 10) {
            step_prescaler = 0;

            if (ramp_up) {
                target_duty++;
                if (target_duty >= 100) {
                    ramp_up = 0; // Reverse to fading down
                }
            } else {
                target_duty--;
                if (target_duty == 0) {
                    ramp_up = 1; // Reverse to fading up
                }
            }
        }
    }
}

void main(void) {
    // Timer 0 configuration: Mode 1 (16-bit timer)
    TMOD = (TMOD & 0xF0) | 0x01;
    TH0 = 0xFF;
    TL0 = 0xF7;

    // Enable Timer 0 interrupt and global interrupts
    ET0 = 1;
    EA  = 1;
    TR0 = 1; // Start Timer 0

    while (1) {
        /* Main loop remains free; timing is purely hardware interrupt driven */
    }
}
