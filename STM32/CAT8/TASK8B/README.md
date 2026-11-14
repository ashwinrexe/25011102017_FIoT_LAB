# Continuous Lab Evaluation - 8

**EXP:** Continuous Lab Evaluation - 8  
**DATE:** 01/09/2026  
**NAME OF EXP:** STM32 GPIO Bring-Up and Switch-Controlled LED  
**HARDWARE SETUP / PLATFORM:** STM32F103C8T6 + Keil MDK / STM32CubeIDE + Momentary Tactile Switch + LED Output Circuit  
**TASK NO:** 8B  

---

### Circuit Diagram
![Circuit Diagram](../CAT8.png)

---

### Core Logic

* **1. Mechanical Contact Debouncing Window**  
  A 25 ms confirmation delay eliminates transient voltage spikes caused by mechanical spring chatter inside tactile pushbuttons:
  ```c
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) {
      HAL_Delay(25);
      if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) {
          /* Valid actuation */
      }
  }
  ```

* **2. Persistent State Latching**  
  A software flag (`led_latched_state`) toggles between 0 and 1, decoupling the LED state from continuous user presence.

* **3. Release Edge Synchronization**  
  The blocking while-loop (`while (HAL_GPIO_ReadPin(...) == GPIO_PIN_RESET)`) inhibits state re-triggering until the user physically releases the actuator.

---

### Inference
Combining an initial debounce settling window with release-edge synchronization converts a momentary contact switch into a reliable bistable latch, demonstrating stable state retention across repeated user operations.
