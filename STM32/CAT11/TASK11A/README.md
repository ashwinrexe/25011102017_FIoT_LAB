# Continuous Lab Evaluation - 11

**EXP:** Continuous Lab Evaluation - 11  
**DATE:** 12/10/2026  
**NAME OF EXP:** HTTP Server - STM32–Wemos Wi-Fi Modem Using AT Commands  
**HARDWARE SETUP / PLATFORM:** STM32F103 Development Board + Wemos D1 Mini (ESP8266) + PC Web Browser  
**TASK NO:** 11A (Static HTML Server)  

---

### Circuit Diagram
![Circuit Diagram](../CAT11.png)

---

### Core Logic

* **1. Multi-Connection Server Initialization**  
  Enabling multi-channel mode (`AT+CIPMUX=1`) permits the module to listen on TCP port 80 simultaneously for concurrent web client handshakes (`AT+CIPSERVER=1,80`).

* **2. Incoming Connection Detection (`+IPD`)**  
  When a browser initiates an HTTP request, the modem issues an asynchronous `+IPD,<link_id>,<len>:` packet. The parser extracts `link_id` to route response frames to the correct socket.
  ```c
  if (strstr(buffer, "+IPD") != NULL) {
      char *ipd_ptr = strstr(buffer, "+IPD,");
      int channel_id = ipd_ptr[5] - '0';
  }
  ```

* **3. HTTP 1.1 Response Frame Dispatch**  
  Formulates standard HTTP header directives (`200 OK`, `Content-Type: text/html`, `Connection: close`) and transmits the exact byte count using `AT+CIPSEND=<id>,<len>`, cleanly terminating the socket via `AT+CIPCLOSE`.

---

### Inference
By orchestrating low-level AT socket primitives, the STM32 acts as a fully compliant standalone HTTP server, delivering web content to any connected client on the local WLAN without requiring external middleware.
