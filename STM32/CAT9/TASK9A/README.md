# Continuous Lab Evaluation - 9

**EXP:** Continuous Lab Evaluation - 9  
**DATE:** 08/09/2026  
**NAME OF EXP:** STM32 UART Command Controlled LED  
**HARDWARE SETUP / PLATFORM:** STM32F103C8T6 + USART1 (PA9 TX, PA10 RX) + FTDI Serial Converter + PuTTY  
**TASK NO:** 9A  

---

### Circuit Diagram
![Circuit Diagram](../CAT9.png)

---

### Core Logic

* **1. Polled Character Ingestion with Non-Zero Timeout**  
  `HAL_UART_Receive(&huart1, &rx_char_byte, 1, 10)` polls the hardware shift register for 10 ms. When a character arrives, it returns `HAL_OK` without hanging foreground loops.
  ```c
  if (HAL_UART_Receive(&huart1, &rx_char_byte, 1, 10) == HAL_OK) {
      HAL_UART_Transmit(&huart1, &rx_char_byte, 1, 50);
  }
  ```

* **2. Automatic Immediate Echo**  
  Forwarding the received byte directly to `HAL_UART_Transmit()` provides immediate visual confirmation in the host terminal window.

* **3. Millisecond Periodic Telemetry**  
  A non-blocking SysTick check emits a periodic asterisk (`*`) beacon every 3000 ms to confirm link integrity.

---

### Inference
Non-blocking UART reception combined with immediate loopback echoing confirms bi-directional serial transmission integrity while leaving headroom for interleaved periodic telemetry and peripheral tasks.
