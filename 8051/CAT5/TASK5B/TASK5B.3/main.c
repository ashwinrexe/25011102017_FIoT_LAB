/*
 * Experiment: CAT-5 Task 5B.3
 * Description: Multi-Profile Waveform Synthesizer (Square, 25% Pulse, Pulse Train, Burst)
 * Platform: Keil C51, AT89S52 @ 11.0592 MHz
 */

#include <reg52.h>

// Timer 0 LED Fading variables
static volatile unsigned char fade_idx = 0;
static volatile unsigned char current_duty = 0;
static volatile unsigned char fade_timer = 0;
static volatile unsigned char fade_dir = 1;

// Timer 1 Waveform Synthesizer variables
static volatile unsigned char active_pattern = 1; // 1: Square, 2: 25% Pulse, 3: Pulse Train, 4: Burst
static volatile unsigned char wave_slice = 0;

sbit LED_PWM_PIN  = P1^0;
sbit SYNTH_OUTPUT = P1^1;

/**
 * @brief Timer 0 ISR: Soft-PWM LED Fader (100 us tick base)
 */
void timer0_isr(void) interrupt 1 {
    TH0 = 0xFF;
    TL0 = 0xF7;

    LED_PWM_PIN = (fade_idx < current_duty) ? 1 : 0;

    fade_idx++;
    if (fade_idx >= 100) {
        fade_idx = 0;
        fade_timer++;
        if (fade_timer >= 10) {
            fade_timer = 0;
            if (fade_dir) {
                if (++current_duty >= 100) fade_dir = 0;
            } else {
                if (--current_duty == 0) fade_dir = 1;
            }
        }
    }
}

/**
 * @brief Timer 1 ISR: Multi-Profile Waveform Synthesis (~500 us tick base)
 */
void timer1_isr(void) interrupt 3 {
    // Reload for ~500 us tick
    TH1 = 0xFE;
    TL1 = 0x33;

    wave_slice++;

    switch (active_pattern) {
        case 1:
            // Profile 1: 50% Symmetrical Square Wave (Period = 2 ms -> 500 Hz)
            SYNTH_OUTPUT = (wave_slice < 2) ? 1 : 0;
            if (wave_slice >= 4) wave_slice = 0;
            break;

        case 2:
            // Profile 2: 25% Duty Cycle Pulse (1 high tick, 3 low ticks)
            SYNTH_OUTPUT = (wave_slice == 0) ? 1 : 0;
            if (wave_slice >= 4) wave_slice = 0;
            break;

        case 3:
            // Profile 3: Pulse Train Sequence (2 fast pulses followed by pause)
            if (wave_slice == 0 || wave_slice == 2) {
                SYNTH_OUTPUT = 1;
            } else {
                SYNTH_OUTPUT = 0;
            }
            if (wave_slice >= 8) wave_slice = 0;
            break;

        case 4:
            // Profile 4: Burst Oscillation (Active burst followed by silent gap)
            if (wave_slice < 6) {
                SYNTH_OUTPUT = (wave_slice % 2 == 0) ? 1 : 0;
            } else {
                SYNTH_OUTPUT = 0;
            }
            if (wave_slice >= 12) wave_slice = 0;
            break;

        default:
            active_pattern = 1;
            break;
    }
}

void main(void) {
    TMOD = 0x11; // Timer 0 Mode 1, Timer 1 Mode 1

    TH0 = 0xFF;
    TL0 = 0xF7;

    TH1 = 0xFE;
    TL1 = 0x33;

    ET0 = 1;
    ET1 = 1;
    EA  = 1;

    TR0 = 1;
    TR1 = 1;

    while (1) {
        /* Operating autonomously via hardware timer interrupts */
    }
}
