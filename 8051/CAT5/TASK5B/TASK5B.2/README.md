# Continuous Lab Evaluation - 5

**EXP:** Continuous Lab Evaluation - 5  
**DATE:** 10/08/2026  
**NAME OF EXP:** Hardware Timer and Interrupt-Based Timing — Variable Frequency Pulse Generation  
**HARDWARE SETUP / PLATFORM:** AT89S52 + Keil C51 + Dual-Channel Oscilloscope + Logic Analyzer  
**TASK NO:** 5B.2 (Task 2 - Variable Frequency)  

---

### Circuit Diagram
![Circuit Diagram](../../CAT5B.png)

---

### Core Logic

* **1. Concurrently Configured Hardware Timers**  
  `TMOD = 0x11` configures both Timer 0 and Timer 1 in 16-bit Mode 1, enabling concurrent, orthogonal timing tasks without cross-channel interference.
  ```c
  TMOD = 0x11;
  ET0 = 1; ET1 = 1; EA = 1;
  TR0 = 1; TR1 = 1;
  ```

* **2. Frequency Synthesizer (Timer 1 ISR)**  
  On every Timer 1 overflow, output pin `P1.1` is complemented. Modifying `t1_reload_hi` and `t1_reload_lo` dynamically shifts the output frequency from 100 Hz to over 10 kHz.
  ```c
  void isr_timer1_wave(void) interrupt 3 {
      TH1 = t1_reload_hi;
      TL1 = t1_reload_lo;
      PIN_WAVE_OUT = ~PIN_WAVE_OUT;
  }
  ```

* **3. Asynchronous Multi-Channel Operation**  
  Timer 0 concurrently maintains the 100-step software PWM ramp on `P1.0`, verifying that the 8051 interrupt arbitration hardware handles dual independent periodic interrupts cleanly.

---

### Inference
Simultaneous operation of Timer 0 and Timer 1 demonstrated that the AT89S52 can perform multi-rate signal processing. The variable-frequency square wave and the PWM fader ran concurrently with deterministic edge timing verified via oscilloscope observation.
