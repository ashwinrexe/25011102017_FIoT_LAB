# Fundamentals of Internet of Things (FIoT) — Laboratory Coursework

Comprehensive repository documenting the experimental design, bare-metal firmware, sensor drivers, and cloud IoT integrations developed for the Fundamentals of Internet of Things (FIoT) Laboratory course.

---

## Hardware Platforms & Toolchains

| Component | Specification |
| :--- | :--- |
| **8051 Microcontroller** | Atmel AT89S52 (8-bit CISC, 11.0592 MHz Crystal Oscillator) |
| **STM32 Microcontrollers** | STM32F103C8T6 (Blue Pill, ARM Cortex-M3 @ 72 MHz) & Nucleo-F103RB |
| **Wireless Transceiver** | Espressif ESP8266 (Wemos D1 Mini) running AT Command Firmware |
| **Sensors & Actuators** | Bosch BME280 (I²C), 4-Phase Stepper Motor (ULN2003), 16×2 HD44780 LCD, 4×4 Matrix Keypad |
| **Development Environments** | Keil uVision (C51 & MDK-ARM), STM32CubeMX, STM32CubeIDE |
| **Protocols & Standards** | GPIO, UART/USART, Timer/PWM, I²C, HTTP 1.1, MQTT |

---

## Laboratory Experiment Index

### Part I: 8051 Embedded Systems (`8051/`)
- **[CAT-2 Task 2B](8051/CAT2/TASK2B/)**: AT89S52 Toolchain Bring-Up & 1.0 Hz Calibrated GPIO Blinker.
- **[CAT-3 Task 3A](8051/CAT3/TASK3A/)**: Multi-Mode LED Pattern Controller with Pushbutton Debounce & FSM.
- **[CAT-4 Task 4A](8051/CAT4/TASK4A/)**: Human-Machine Interface with 4×4 Matrix Keypad, 7-Segment Decoder, and Dynamic LED Patterns.
- **[CAT-5 Task 5A](8051/CAT5/TASK5A/)**: Hardware Timer 0 Interrupt-Driven Soft-PWM LED Fader.
- **[CAT-5 Task 5B](8051/CAT5/TASK5B/)**: 
  - `TASK5B.1`: Software PWM LED Fade Generator.
  - `TASK5B.2`: Dual Timer Variable-Frequency Pulse & Square Wave Generator.
  - `TASK5B.3`: Multi-Profile Waveform Synthesizer (Square, 25% Pulse, Pulse Train, Burst).
- **[CAT-5 Task 5C](8051/CAT5/TASK5C/)**: 4-Phase Stepper Motor Commutation with Matrix Keypad Velocity Selection.
- **[CAT-6 Task 6A](8051/CAT6/TASK6A/)**: 8051 UART Driver Setup (9600 Baud, 8-N-1) and Polling String Transmission.
- **[CAT-6 Task 6B](8051/CAT6/TASK6B/)**: Interrupt-Driven Serial Receiver & Full-Duplex Terminal Echo.
- **[CAT-7 Task 7A](8051/CAT7/TASK7A/)**: HD44780 16×2 LCD 4-Bit Parallel Bus Bring-Up and Static Text Rendering.
- **[CAT-7 Task 7B](8051/CAT7/TASK7B/)**: Multi-Line Cursor Positioning and Lightweight Radix-10 Numeric Formatter.
- **[CAT-7 Task 7C](8051/CAT7/TASK7C/)**: Animated Traverser Display and Coordinate Scrolling on Alphanumeric LCD.

### Part II: STM32 Embedded Architecture & IoT Telemetry (`STM32/`)
- **[CAT-8 Task 8A](STM32/CAT8/TASK8A/)**: Basic GPIO Polling and Switch-Controlled Indicator LED.
- **[CAT-8 Task 8B](STM32/CAT8/TASK8B/)**: Pushbutton Toggle with Software Debounce & State Remembrance (Latching).
- **[CAT-8 Task 8C](STM32/CAT8/TASK8C/)**: Multi-Gesture Button Handler (Short Press Toggle vs. Long Press 2 Hz Flasher).
- **[CAT-9 Task 9A](STM32/CAT9/TASK9A/)**: USART1 Serial Telemetry, Character Echo, and Periodic SysTick Heartbeat.
- **[CAT-9 Task 9B](STM32/CAT9/TASK9B/)**: Interactive Serial Command Parser ('1', '0', 'B', 'S') with Onboard LED Control.
- **[CAT-10 Task 10A](STM32/CAT10/TASK10A/)**: Dual-UART Hardware Gateway (PC Terminal ↔ STM32 ↔ ESP8266 Modem).
- **[CAT-10 Task 10B](STM32/CAT10/TASK10B/)**: ESP8266 AT Command Sequencer (`AT`, `AT+GMR`, `AT+CWMODE=1`).
- **[CAT-10 Task 10C](STM32/CAT10/TASK10C/)**: Autonomous Wi-Fi Station Association (`AT+CWJAP`) and IP Lease Query (`AT+CIFSR`).
- **[CAT-11 Task 11A](STM32/CAT11/TASK11A/)**: Embedded HTTP Server on TCP Port 80 serving Static Webpage Dashboard.
- **[CAT-11 Task 11B](STM32/CAT11/TASK11B/)**: Interactive REST API Endpoint Handler (`/led/on`, `/led/off`) for Cloud/Browser Actuation.
- **[CAT-12 Task 12A](STM32/CAT12/TASK12A/)**: Nucleo-64 PA5 On-board LED Bring-Up (1.0 Hz Toggle).
- **[CAT-12 Task 12B](STM32/CAT12/TASK12B/)**: Nucleo-64 User Pushbutton (PC13) State Latch.
- **[CAT-12 Task 12C](STM32/CAT12/TASK12C/)**: Short-Press vs. Long-Press Duration Discrimination using SysTick Counter.
- **[CAT-13 Task 13](STM32/CAT13/TASK13/)**: Bosch BME280 Environmental Sensor Interfacing over I²C with Sensor API Compensation.
- **[CAT-14 Task 14](STM32/CAT14/TASK14/)**: End-to-End Edge Node: BME280 Acquisition, JSON Formatting, and MQTT Telemetry Uplink with Remote Actuation.

---

## Repository Directory Hierarchy

```text
├── 8051/
│   ├── CAT2/TASK2B/       # GPIO Output & 1 Hz Blink
│   ├── CAT3/TASK3A/       # 4-Mode LED Sequencer & Button FSM
│   ├── CAT4/TASK4A/       # 4x4 Matrix Keypad + 7-Seg Display
│   ├── CAT5/
│   │   ├── TASK5A/       # Timer 0 Soft-PWM Fader
│   │   ├── TASK5B/       # Multi-Waveform Pulse Synthesizers
│   │   └── TASK5C/       # Stepper Motor Commutation
│   ├── CAT6/
│   │   ├── TASK6A/       # UART Init & Polling Transmit
│   │   └── TASK6B/       # UART Interrupt Echo Loopback
│   └── CAT7/
│       ├── TASK7A/       # 16x2 LCD 4-bit Static Driver
│       ├── TASK7B/       # Multi-line & Number Formatting
│       └── TASK7C/       # Dynamic Moving Character Animation
└── STM32/
    ├── CAT8/             # GPIO Input, Debouncing & Multi-Mode
    ├── CAT9/             # USART Echo & Command Parsers
    ├── CAT10/            # STM32 - ESP8266 Modem Bridge & Wi-Fi Joiner
    ├── CAT11/            # Embedded HTTP 1.1 Web Server & REST API
    ├── CAT12/            # Nucleo On-Board LED, Switch & Timing Gestures
    ├── CAT13/            # BME280 I2C Environmental Sensor Driver
    └── CAT14/            # IoT Edge Node: BME280 + Wi-Fi + MQTT
```
