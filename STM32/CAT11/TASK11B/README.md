# Continuous Lab Evaluation - 11

**EXP:** Continuous Lab Evaluation - 11  
**DATE:** 12/10/2026  
**NAME OF EXP:** HTTP Server - STM32–Wemos Wi-Fi Modem Using AT Commands  
**HARDWARE SETUP / PLATFORM:** STM32 Development Board + Wemos D1 Mini + Node-RED / Web Dashboard  
**TASK NO:** 11B (Dynamic Rest API & Actuator Control)  

---

### Circuit Diagram
![Circuit Diagram](../CAT11.png)

---

### Core Logic

* **1. Uniform Low-Level Socket Pipeline**  
  The underlying transport pipeline shares the non-blocking AT multi-connection server architecture, listening on TCP Port 80.

* **2. Endpoint Request Parsing**  
  The server inspects the URI payload in incoming HTTP GET requests:
  - `GET /led/on`: Drives on-board LED `PC13` LOW (Active-LOW ON).
  - `GET /led/off`: Drives on-board LED `PC13` HIGH (Active-LOW OFF).
  ```c
  if (strstr(rx_window, "GET /led/on") != NULL) {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
  } else if (strstr(rx_window, "GET /led/off") != NULL) {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
  }
  ```

* **3. Dynamic HTML Document Synthesis**  
  Samples physical pin voltage and renders interactive web buttons reflecting real-time peripheral state.

---

### Inference
Parsing HTTP query strings inside embedded microcontrollers enables cross-platform remote actuation from any web browser or IoT workflow engine (such as Node-RED), establishing full-stack edge device interoperability.
