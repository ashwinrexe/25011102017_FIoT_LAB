# Continuous Lab Evaluation - 10

**EXP:** Continuous Lab Evaluation - 10  
**DATE:** 22/09/2026  
**NAME OF EXP:** Establish STM32 ↔ Wemos UART Communication  
**HARDWARE SETUP / PLATFORM:** STM32F103 (USART1 to PC, USART3 to Wemos D1 Mini / ESP8266) + Keil MDK  
**TASK NO:** 10A  

---

### Circuit Diagram
![Circuit Diagram](../CAT10.png)

---

### Core Logic

* **1. Asynchronous Dual-UART Gateway**  
  The STM32 acts as an intelligent hardware bridge between USART3 (interfaced to the ESP8266 Wi-Fi transceiver) and USART1 (interfaced to the developer workstation terminal).

* **2. Power-On Stabilization Window**  
  A 1.2-second settling pause allows the onboard ESP8266 bootloader to initialize radio calibration before accepting serial transmissions.

* **3. Basic AT Handshake**  
  Dispatches `AT\r\n` across USART3 and monitors for synchronous string return:
  ```c
  HAL_UART_Transmit(&huart3, (uint8_t *)ping_cmd, strlen(ping_cmd), 1000);
  HAL_StatusTypeDef rx_status = HAL_UART_Receive(&huart3, esp_response, sizeof(esp_response) - 1, 2500);
  ```

---

### Inference
Establishing dual-UART forwarding between the host terminal and the ESP8266 validates voltage level compatibility, baud matching (115200 baud), and signal routing, forming the physical backbone for wireless IoT operations.
