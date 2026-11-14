# Continuous Lab Evaluation - 12

**EXP:** Continuous Lab Evaluation - 12  
**DATE:** 20/10/2026  
**NAME OF EXP:** On-board Switch Control with State Remembrance  
**HARDWARE SETUP / PLATFORM:** STM32 Nucleo-F103RB (User Button PC13, User LED PA5) + Keil MDK  
**TASK NO:** 12B (Task 2)  

---

### Circuit Diagram
![Circuit Diagram](../CAT12.png)

---

### Core Logic

* **1. Active-Low Button Sensing**  
  The Nucleo user switch (B1) is wired to `PC13` with an external pull-up. Pressing the switch shorts the line to ground (`GPIO_PIN_RESET`).

* **2. Two-Stage Mechanical Filtering**  
  A 40 ms settling delay eliminates contact noise before state inversion is executed.

* **3. Release Synchronization**  
  A release loop prevents rapid multi-toggling, ensuring a single clean toggle per button press.

---

### Inference
State latching with debouncing reliably decouples user actuation from output state, turning a momentary contact into a stable on/off power switch.
