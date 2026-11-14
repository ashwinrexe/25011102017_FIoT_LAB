# Continuous Lab Evaluation - 10

**EXP:** Continuous Lab Evaluation - 10  
**DATE:** 22/09/2026  
**NAME OF EXP:** Connect Wemos to Wi-Fi  
**HARDWARE SETUP / PLATFORM:** STM32F103 Microcontroller + ESP8266 Wemos D1 Mini + Local 2.4 GHz Access Point  
**TASK NO:** 10C  

---

### Circuit Diagram
![Circuit Diagram](../CAT10.png)

---

### Core Logic

* **1. Wi-Fi Station Association (`AT+CWJAP`)**  
  Issues network credentials to connect the module to a local WPA2-PSK access point with an extended 9-second timeout to accommodate 802.11 DHCP handshake negotiations:
  ```c
  send_raw_command("AT+CWJAP=\"SNUC\",\"snu12345\"\r\n", 9000);
  ```

* **2. IP Address Discovery (`AT+CIFSR`)**  
  Upon successful association, querying `AT+CIFSR` retrieves the dynamically leased Station IP address and hardware MAC address.

* **3. Interactive Serial Telemetry**  
  Echoing status updates back to PuTTY verifies connectivity state transitions (`WIFI CONNECTED` followed by `WIFI GOT IP`).

---

### Inference
Autonomous modem control via structured AT sequences allows an embedded STM32 host to manage wireless network association and IP lease acquisition, preparing the hardware for networked socket communications.
