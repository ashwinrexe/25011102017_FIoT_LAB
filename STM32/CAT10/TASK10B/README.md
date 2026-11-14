# Continuous Lab Evaluation - 10

**EXP:** Continuous Lab Evaluation - 10  
**DATE:** 22/09/2026  
**NAME OF EXP:** Explore the ESP8266 AT Command Interface  
**HARDWARE SETUP / PLATFORM:** STM32F103 + ESP8266 Module + Serial Debug Terminal  
**TASK NO:** 10B  

---

### Circuit Diagram
![Circuit Diagram](../CAT10.png)

---

### Core Logic

* **1. Automated AT Command Dispatcher**  
  A centralized transceiver routine (`dispatch_modem_command`) multiplexes outgoing commands to the modem while mirroring each byte to the developer console for real-time inspection.
  ```c
  void dispatch_modem_command(const char *cmd_str, uint32_t timeout_limit);
  ```

* **2. Inter-Character Burst Detection**  
  Rather than blocking for the entire timeout window, the parser tracks `last_rx_tick`. An idle inter-byte gap exceeding 120 ms signals transmission completion, accelerating state progression.

* **3. Diagnostics & Mode Configuration**  
  - `AT\r\n`: Verifies basic command response (`OK`).
  - `AT+GMR\r\n`: Extracts firmware revision, SDK build, and core version.
  - `AT+CWMODE=1\r\n`: Configures the Wi-Fi transceiver into pure Station (STA) client mode.

---

### Inference
Automating command verification via an inter-byte timeout parser provides reliable modem diagnostics, confirming that the ESP8266 AT parser operates with complete firmware integrity.
