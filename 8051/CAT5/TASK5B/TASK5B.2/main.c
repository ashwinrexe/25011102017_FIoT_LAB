/*
 * Experiment: CAT-5 Task 5B.2
 * Description: Dual Timer System: Software PWM Fader (Timer 0) & Variable Frequency Square Wave (Timer 1)
 * Platform: Keil C51, AT89S52 @ 11.0592 MHz
 */

#include <reg52.h>

// Timer 0 PWM globals
static volatile unsigned char pwm_step = 0;
static volatile unsigned char duty_pct = 0;
static volatile unsigned char fade_tick = 0;

// Timer 1 Variable Frequency Reloads (Default: ~1 kHz, toggle half-period ~500 us)
static volatile unsigned char t1_reload_hi = 0xFE;
static volatile unsigned char t1_reload_lo = 0x33;

sbit PIN_FADE_LED   = P1^0;
sbit PIN_WAVE_OUT   = P1^1;

/**
 * @brief Timer 0 ISR: LED Fading (~100 us tick)
 */
void isr_timer0_fade(void) interrupt 1 {
    TH0 = 0xFF;
    TL0 = 0xF7;

    PIN_FADE_LED = (pwm_step < duty_pct) ? 1 : 0;

    pwm_step++;
    if (pwm_step >= 100) {
        pwm_step = 0;
        fade_tick++;
        if (fade_tick >= 10) {
            fade_tick = 0;
            duty_pct++;
            if (duty_pct > 100) {
                duty_pct = 0;
            }
        }
    }
}

/**
 * @brief Timer 1 ISR: Precision Variable Frequency Square Wave Generation
 */
void isr_timer1_wave(void) interrupt 3 {
    TH1 = t1_reload_hi;
    TL1 = t1_reload_lo;
    PIN_WAVE_OUT = ~PIN_WAVE_OUT; // Invert output state on each overflow
}

void main(void) {
    // TMOD: Timer 1 Mode 1 (16-bit), Timer 0 Mode 1 (16-bit) -> 0x11
    TMOD = 0x11;

    // Seed Timer 0
    TH0 = 0xFF;
    TL0 = 0xF7;

    // Seed Timer 1 (1 kHz default)
    TH1 = t1_reload_hi;
    TL1 = t1_reload_lo;

    // Enable interrupts for both timers
    ET0 = 1;
    ET1 = 1;
    EA  = 1;

    // Start timers
    TR0 = 1;
    TR1 = 1;

    while (1) {
        /* Independent concurrent background waveform generation */
    }
}
