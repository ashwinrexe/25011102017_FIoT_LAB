# Continuous Lab Evaluation - 5

**EXP:** Continuous Lab Evaluation - 5  
**DATE:** 10/08/2026  
**NAME OF EXP:** Hardware Timer and Interrupt-Based Timing — LED Fade  
**HARDWARE SETUP / PLATFORM:** AT89S52 + Keil C51 + Oscilloscope + Breadboard LED Circuit  
**TASK NO:** 5B.1 (Task 2 - LED Fade)  

---

### Circuit Diagram
![Circuit Diagram](../../CAT5B.png)

---

### Core Logic

* **1. Timer 0 Interrupt Base (100 µs)**  
  Reloads 16-bit Timer 0 on every overflow (`TH0 = 0xFF`, `TL0 = 0xF7`) to establish an interrupt frequency of approximately 10 kHz.
  ```c
  void timer0_fade_isr(void) interrupt 1 {
      TH0 = 0xFF;
      TL0 = 0xF7;
  ```

* **2. 100 Hz Effective PWM Output**  
  Accumulating 100 interrupt slices per PWM cycle produces an overall PWM repetition rate of 100 Hz, avoiding human optical flicker.
  ```c
  if (pwm_slice < active_duty) {
      LED_BREATHE = 1;
  } else {
      LED_BREATHE = 0;
  }
  ```

* **3. Dual-Slope Fading Sweep**  
  Every 10 full PWM periods (~10 ms), the duty cycle updates by 1%, taking approximately 1 second to transition from 0% to 100% brightness.
  ```c
  if (ramp_increasing) {
      if (++active_duty >= 100) ramp_increasing = 0;
  } else {
      if (--active_duty == 0) ramp_increasing = 1;
  }
  ```

---

### Inference
Modulating the duty cycle of an active-high LED using interrupt-driven software PWM validates that high-precision analog-like dimming can be synthesized on microcontrollers lacking dedicated hardware PWM peripherals.
