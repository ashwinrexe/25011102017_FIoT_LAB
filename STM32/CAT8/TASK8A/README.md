# Continuous Lab Evaluation - 8

**EXP:** Continuous Lab Evaluation - 8  
**DATE:** 01/09/2026  
**NAME OF EXP:** STM32 GPIO Bring-Up and Switch-Controlled LED  
**HARDWARE SETUP / PLATFORM:** STM32F103C8T6 (Blue Pill) + Keil MDK-ARM / STM32CubeIDE + Pushbutton on PA0 + Indicator LED on PA5  
**TASK NO:** 8A  

---

### Circuit Diagram
![Circuit Diagram](../CAT8.png)

---

### Core Logic

* **1. Synchronous Pin Sensing via HAL Drivers**  
  The GPIO subsystem samples physical voltage on pin `PA0` using `HAL_GPIO_ReadPin()`. In this active-low pull-up circuit configuration, ground potential (`GPIO_PIN_RESET`) signifies active user actuation.
  ```c
  current_button_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
  ```

* **2. Direct Digital Output Assertions**  
  When an active state is observed, `HAL_GPIO_WritePin()` sets `PA5` high (`GPIO_PIN_SET`), biasing the external current-limiting resistor to light the LED. Releasing the switch immediately pulls `PA5` low (`GPIO_PIN_RESET`).
  ```c
  if (current_button_state == GPIO_PIN_RESET) {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
  } else {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
  }
  ```

---

### Inference
Direct peripheral register manipulation via STM32 HAL demonstrates responsive GPIO input-output coupling. The polling approach provides straightforward logic verification for breadboard circuit bring-up and sanity checks.
