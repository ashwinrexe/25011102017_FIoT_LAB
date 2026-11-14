/*
 * Experiment: CAT-5 Task 5B.1
 * Description: Soft-PWM LED Fading with Calibrated Triangular Ramp
 * Platform: Keil C51, AT89S52 @ 11.0592 MHz
 */

#include <reg52.h>

static volatile unsigned char pwm_slice = 0;
static volatile unsigned char active_duty = 0;
static volatile unsigned char rate_counter = 0;
static volatile unsigned char ramp_increasing = 1;

sbit LED_BREATHE = P1^0;

void timer0_fade_isr(void) interrupt 1 {
    TH0 = 0xFF;
    TL0 = 0xF7;

    // Software PWM driver
    if (pwm_slice < active_duty) {
        LED_BREATHE = 1;
    } else {
        LED_BREATHE = 0;
    }

    pwm_slice++;
    if (pwm_slice >= 100) {
        pwm_slice = 0;
        rate_counter++;

        // Update ramp every 10 PWM cycles (~10 ms)
        if (rate_counter >= 10) {
            rate_counter = 0;
            if (ramp_increasing) {
                active_duty++;
                if (active_duty >= 100) {
                    ramp_increasing = 0;
                }
            } else {
                active_duty--;
                if (active_duty == 0) {
                    ramp_increasing = 1;
                }
            }
        }
    }
}

void main(void) {
    TMOD = 0x01; // Timer 0 Mode 1 (16-bit)
    TH0 = 0xFF;
    TL0 = 0xF7;

    ET0 = 1;
    EA  = 1;
    TR0 = 1;

    while (1) {
        // Foreground task idle
    }
}
