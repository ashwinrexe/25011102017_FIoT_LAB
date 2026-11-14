# Continuous Lab Evaluation - 5

**EXP:** Continuous Lab Evaluation - 5  
**DATE:** 10/08/2026  
**NAME OF EXP:** Hardware Timer and Interrupt-Based Timing — Waveform Generation  
**HARDWARE SETUP / PLATFORM:** AT89S52 + Keil C51 + Mixed Signal Oscilloscope  
**TASK NO:** 5B.3 (Task 2 - Waveform Generation)  

---

### Circuit Diagram
![Circuit Diagram](../../CAT5B.png)

---

### Core Logic

* **1. Multi-Profile Pattern Generator Engine**  
  Timer 1 establishes a 500 µs base timebase. Inside the ISR, a discrete modulo sequencer synthesizes multiple distinct signal profiles without floating-point math:
  - **Square Wave**: 50% duty factor at 500 Hz.
  - **Narrow Pulse**: 25% duty factor pulse.
  - **Pulse Train**: Two consecutive pulses followed by an extended recovery interval.
  - **Burst Waveform**: High-frequency pulse burst interleaved with silent pause periods.
  ```c
  switch (active_pattern) {
      case 1: SYNTH_OUTPUT = (wave_slice < 2) ? 1 : 0; break;
      case 2: SYNTH_OUTPUT = (wave_slice == 0) ? 1 : 0; break;
      case 3: SYNTH_OUTPUT = (wave_slice == 0 || wave_slice == 2) ? 1 : 0; break;
      case 4: SYNTH_OUTPUT = (wave_slice < 6 && (wave_slice % 2 == 0)) ? 1 : 0; break;
  }
  ```

* **2. Interleaved Dual-Timer Pipeline**  
  Timer 0 simultaneously runs the LED fade routine on `P1.0`, proving that multiple complex time-critical wave generators can run side-by-side on low-cost 8-bit silicon.

---

### Inference
Software-synthesized waveform state machines implemented inside hardware timer ISRs yield flexible, predictable signal generation for communication strobes, pulse triggers, and burst telemetry without specialized DDS or DAC ICs.
