# Continuous Lab Evaluation - 8

**EXP:** Continuous Lab Evaluation - 8  
**DATE:** 01/09/2026  
**NAME OF EXP:** STM32 GPIO Bring-Up and Switch-Controlled LED  
**HARDWARE SETUP / PLATFORM:** STM32F103 Microcontroller + Pushbutton Switch + Output LED + SysTick Timer  
**TASK NO:** 8C  

---

### Circuit Diagram
![Circuit Diagram](../CAT8.png)

---

### Core Logic

* **1. SysTick-Based Press Duration Timing**  
  Capturing the initial timestamp (`HAL_GetTick()`) and subtracting it upon release yields an exact hold duration in milliseconds:
  ```c
  uint32_t press_duration = HAL_GetTick() - press_start;
  ```

* **2. Dual Action Classification**  
  - **Short Actuation (< 800 ms)**: Inverts the static state of the LED or halts active flashing.
  - **Long Actuation (>= 800 ms)**: Enables continuous 2 Hz non-blocking flashing mode.

* **3. Non-Blocking Foreground Blinker**  
  A non-blocking time comparator (`HAL_GetTick() - last_blink_tick >= 250`) executes toggles without pausing CPU execution, allowing concurrent button sensing.

---

### Inference
Integrating the ARM Cortex-M SysTick counter with edge detection logic enables multi-gesture input sensing (short click vs. sustained hold) on a single physical button pin, maximizing UI flexibility with minimal hardware overhead.
