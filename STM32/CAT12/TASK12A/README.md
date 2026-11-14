# Continuous Lab Evaluation - 12

**EXP:** Continuous Lab Evaluation - 12  
**DATE:** 20/10/2026  
**NAME OF EXP:** On-board LED Bring-Up  
**HARDWARE SETUP / PLATFORM:** STM32 Nucleo-F103RB Development Board (On-board User LED on PA5) + Keil MDK  
**TASK NO:** 12A (Task 1)  

---

### Circuit Diagram
![Circuit Diagram](../CAT12.png)

---

### Core Logic

* **1. Synchronous Pin State Inversion**  
  Invokes `HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5)` on every loop pass, toggling the output driver without requiring software state tracking.

* **2. Fixed 500 ms Cadence**  
  `HAL_Delay(500)` leverages the SysTick interrupt counter to maintain an accurate 1.0 Hz overall blink frequency (500 ms ON, 500 ms OFF).

---

### Inference
Validates the clock distribution tree and GPIO driver configurations on the Nucleo-64 platform, confirming baseline toolchain functionality.
