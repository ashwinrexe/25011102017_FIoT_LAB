# Continuous Lab Evaluation - 9

**EXP:** Continuous Lab Evaluation - 9  
**DATE:** 08/09/2026  
**NAME OF EXP:** STM32 UART Command Controlled LED  
**HARDWARE SETUP / PLATFORM:** STM32F103 Blue Pill + USART1 (PA9/PA10) + On-Board LED (PC13) + Serial Monitor  
**TASK NO:** 9B  

---

### Circuit Diagram
![Circuit Diagram](../CAT9.png)

---

### Core Logic

* **1. Character Command Protocol**  
  The terminal parser translates individual ASCII control codes into discrete hardware states:
  - `'1'`: Illuminates on-board LED (`PC13` pulled LOW).
  - `'0'`: Extinguishes on-board LED (`PC13` driven HIGH).
  - `'B'` / `'b'`: Transitions controller into 250 ms toggle mode.
  - `'S'` / `'s'`: Dispatches system telemetry status report over serial.

* **2. Non-Blocking Mode Scheduler**  
  The main loop executes continuous state dispatch without blocking delays, enabling immediate command preemption while the LED is flashing:
  ```c
  if (active_led_mode == 2) {
      if (HAL_GetTick() - last_led_blink_tick >= 250) {
          HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
          last_led_blink_tick = HAL_GetTick();
      }
  }
  ```

* **3. Formatted Terminal Feedback**  
  Every command execution returns a diagnostic acknowledgment string formatted via `snprintf()`.

---

### Inference
Parsing single-character serial command packets within an asynchronous main loop provides a lightweight remote control interface, establishing foundational protocol handling for embedded networked nodes.
