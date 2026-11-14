/*
 * Experiment: CAT-4 Task 4A
 * Description: Human-Machine Interface with 4x4 Matrix Keypad, 7-Segment Display, and Multi-Pattern LEDs
 * Platform: Keil C51, AT89S52 @ 11.0592 MHz
 */

#include <reg51.h>

// Forward declarations
static void delay_ms(unsigned int ms);
static int scan_matrix_keypad(void);
static int scan_keypad_nonblocking(unsigned int wait_ms);
static void refresh_seven_segment(int num);
static void execute_mode_action(unsigned char key_code);
static void run_chaser_animation(void);
static void run_alternating_animation(void);
static void run_binary_counter(void);
static void run_flash_animation(void);
static void write_led_bus(unsigned char val);

// Global active mode register (-1 = Idle/Clear)
static int current_system_mode = -1;

/**
 * @brief Common-Cathode 7-segment digit lookup table (0 through 9)
 */
static const unsigned char SEGMENT_MAP[10] = {
    0x3F, // '0' (A+B+C+D+E+F)
    0x06, // '1' (B+C)
    0x5B, // '2' (A+B+D+E+G)
    0x4F, // '3' (A+B+C+D+G)
    0x66, // '4' (B+C+F+G)
    0x6D, // '5' (A+C+D+F+G)
    0x7D, // '6' (A+C+D+E+F+G)
    0x07, // '7' (A+B+C)
    0x7F, // '8' (A+B+C+D+E+F+G)
    0x6F  // '9' (A+B+C+D+F+G)
};

static void delay_ms(unsigned int ms) {
    unsigned int a, b;
    for (a = 0; a < ms; a++) {
        for (b = 0; b < 120; b++) {
            /* 1 ms tick */
        }
    }
}

/**
 * @brief Updates 7-Segment Display on Port 0
 */
static void refresh_seven_segment(int num) {
    if (num >= 0 && num <= 9) {
        P0 = SEGMENT_MAP[num];
    } else {
        P0 = 0x00; // Blank display
    }
}

/**
 * @brief Scans 4x4 Matrix Keypad on Port 2 (Rows P2.7..P2.4, Cols P2.3..P2.0)
 * @return Detected key digit 0..9 or -1 if no key pressed
 */
static int scan_matrix_keypad(void) {
    static const int KEY_LAYOUT[4][4] = {
        {1,  2,  3, -1},
        {4,  5,  6, -1},
        {7,  8,  9, -1},
        {-1, 0, -1, -1}
    };
    static const unsigned char ROW_BITS[4] = {7, 6, 5, 4};
    static const unsigned char COL_BITS[4] = {3, 2, 1, 0};
    unsigned char r, c;

    for (r = 0; r < 4; r++) {
        P2 = 0xFF;                          // Set columns high with pull-ups
        P2 &= ~(1 << ROW_BITS[r]);          // Drive active row LOW

        for (c = 0; c < 4; c++) {
            if ((P2 & (1 << COL_BITS[c])) == 0) {
                delay_ms(15);               // Debounce
                if ((P2 & (1 << COL_BITS[c])) == 0) {
                    while ((P2 & (1 << COL_BITS[c])) == 0); // Release wait
                    return KEY_LAYOUT[r][c];
                }
            }
        }
    }
    return -1;
}

/**
 * @brief Non-blocking delay that checks for keypad intervention
 */
static int scan_keypad_nonblocking(unsigned int wait_ms) {
    unsigned int elapsed = 0;
    while (elapsed < wait_ms) {
        int pressed = scan_matrix_keypad();
        if (pressed != -1) {
            return pressed;
        }
        delay_ms(10);
        elapsed += 10;
    }
    return -1;
}

static void write_led_bus(unsigned char val) {
    P1 = val;
}

static void execute_mode_action(unsigned char key_code) {
    switch (key_code) {
        case 1: write_led_bus(0x01); break; // LED 1 ON
        case 2: write_led_bus(0x02); break; // LED 2 ON
        case 3: write_led_bus(0x04); break; // LED 3 ON
        case 4: write_led_bus(0x08); break; // LED 4 ON
        default: break;
    }
}

static void run_chaser_animation(void) {
    unsigned char idx;
    for (idx = 0; idx < 8; idx++) {
        write_led_bus(1 << (idx % 4));
        if (scan_keypad_nonblocking(180) != -1) return;
    }
}

static void run_alternating_animation(void) {
    write_led_bus(0x0A);
    if (scan_keypad_nonblocking(250) != -1) return;
    write_led_bus(0x05);
    if (scan_keypad_nonblocking(250) != -1) return;
}

static void run_binary_counter(void) {
    unsigned char cnt;
    for (cnt = 0; cnt < 16; cnt++) {
        write_led_bus(cnt);
        if (scan_keypad_nonblocking(250) != -1) return;
    }
}

static void run_flash_animation(void) {
    write_led_bus(0x0F);
    if (scan_keypad_nonblocking(250) != -1) return;
    write_led_bus(0x00);
    if (scan_keypad_nonblocking(250) != -1) return;
}

void main(void) {
    int detected_key;

    // Blank displays and LED outputs
    P0 = 0x00;
    P1 = 0x00;

    while (1) {
        refresh_seven_segment(current_system_mode);

        switch (current_system_mode) {
            case 1:
            case 2:
            case 3:
            case 4:
                execute_mode_action(current_system_mode);
                break;
            case 5:
                run_chaser_animation();
                break;
            case 6:
                run_alternating_animation();
                break;
            case 7:
                run_binary_counter();
                break;
            case 8:
                run_flash_animation();
                break;
            case 9:
                write_led_bus(0x00); // All LEDs OFF
                break;
            case 0:
                P0 = 0x00;
                P1 = 0x00;
                current_system_mode = -1; // Reset state
                break;
            default:
                break;
        }

        detected_key = scan_matrix_keypad();
        if (detected_key != -1) {
            current_system_mode = detected_key;
        }
    }
}
