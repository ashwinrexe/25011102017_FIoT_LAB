/*
 * Experiment: CAT-3 Task 3A
 * Description: Interactive Multi-Mode LED Pattern Controller with Button Debouncing
 * Platform: Keil C51, AT89S52 @ 11.0592 MHz
 */

#include <reg51.h>

// Direct Port 3 SFR definition for 4-bit active-low LEDs
sfr PORT3_IO = 0xB0;

// Pushbutton input pin on P3.4
sbit PUSH_BUTTON = P3^4;

/**
 * @brief Software millisecond delay routine
 */
static void pause_ms(unsigned int duration_ms) {
    unsigned int x, y;
    for (x = 0; x < duration_ms; x++) {
        for (y = 0; y < 118; y++) {
            /* ~1 ms at 11.0592 MHz crystal */
        }
    }
}

/**
 * @brief Drives lower nibble active-low LEDs while preserving upper nibble input pins
 * @param pattern 4-bit logic pattern (1 = LED ON, 0 = LED OFF)
 */
static void apply_led_pattern(unsigned char pattern) {
    // Invert lower 4 bits for Active-Low hardware configuration
    // Ensure P3.4 remains logic 1 to serve as a pulled-up input pin
    PORT3_IO = (PORT3_IO & 0xF0) | ((~pattern) & 0x0F);
}

/**
 * @brief Scans pushbutton with debouncing and release-edge detection
 * @return 1 on confirmed button press event, 0 otherwise
 */
static unsigned char poll_button_press(void) {
    if (PUSH_BUTTON == 0) {            // Detect active-low transition
        pause_ms(25);                  // 25 ms debounce filter
        if (PUSH_BUTTON == 0) {
            while (PUSH_BUTTON == 0) {
                /* Block until physical key release */
            }
            pause_ms(25);              // Release debounce filter
            return 1;
        }
    }
    return 0;
}

void main(void) {
    unsigned char active_mode = 0;     // State index (0 = Standby, 1..4 = Operating Modes)
    unsigned char anim_phase = 0;
    unsigned char counter_val = 0;

    // Default Port 3 state: LEDs OFF (lower nibble HIGH), inputs pulled HIGH
    PORT3_IO = 0xFF;

    while (1) {
        // Poll button to advance operating mode
        if (poll_button_press()) {
            active_mode++;
            if (active_mode > 4) {
                active_mode = 1;       // Wrap around to Mode 1
            }
            anim_phase = 0;
            counter_val = 0;
        }

        switch (active_mode) {
            case 0:
                // Standby: All LEDs OFF
                apply_led_pattern(0x00);
                break;

            case 1:
                // Mode 1: Ring Shift Sequence (LED 0 -> 1 -> 2 -> 3 -> 0)
                apply_led_pattern(1 << anim_phase);
                anim_phase = (anim_phase + 1) % 4;
                pause_ms(180);
                break;

            case 2:
                // Mode 2: 4-Bit Binary Up-Counter (0x0 to 0xF)
                apply_led_pattern(counter_val & 0x0F);
                counter_val = (counter_val + 1) & 0x0F;
                pause_ms(280);
                break;

            case 3:
                // Mode 3: Alternating Strobe (0b1010 <-> 0b0101)
                apply_led_pattern(anim_phase ? 0x05 : 0x0A);
                anim_phase ^= 1;
                pause_ms(280);
                break;

            case 4:
                // Mode 4: Synchronous Pulse Flasher (All ON <-> All OFF)
                apply_led_pattern(anim_phase ? 0x00 : 0x0F);
                anim_phase ^= 1;
                pause_ms(280);
                break;

            default:
                active_mode = 0;
                break;
        }
    }
}
