# DST Project- 2014-2017 (MS Thesis - Subhrasankha Dey)
## Arduino_code_IITKGP_WSN
Contains the part of source code for Wireless Sensor Network developed at the IIT Kharagpur for rainfall-runoff measurement, data logging, and interactive web-server. 
## Python_code_XBEE_Wireless_Transmit_Receive
Contains the part of source code for Wireless Sensor Network developed at the IIT Kharagpur for rainfall-runoff measurement, data logging, and interactive web-server. 
## HTML_code_IITKGP_WSN
Contains the part of source code for Wireless Sensor Network developed at the IIT Kharagpur for rainfall-runoff measurement, data logging, and interactive web-server. 
### Publication - Real-time OGC compliant online data monitoring and acquisition network for management of hydro-meteorological hazards (https://doi.org/10.1080/09715010.2016.1264896)

# Details of Arduino Wireless Sensor Rainfall–Runoff Measurement (DST, IIT-KGP)

**Project:** Real-time, low-cost hydro-meteorological data acquisition network  
**Origin:** DST project — IIT Kharagpur (IIT KGP)  
**Purpose:** Collect rainfall and related meteorological/hydraulic data with Arduino-based field nodes, host lightweight web pages per node, log data locally (SD + RTC), and forward readings via RF→Ethernet gateways with optional client-side logging.

---

## 📂 Repository Structure

- `DST_Project_Codes/HTML_code_IITKGP_WSN/index.html`  
- `DST_Project_Codes/Arduino_code_IITKGP_WSN/Civil_DHT11_ethernet_allsensors_01_06_15.ino`  
- `DST_Project_Codes/Arduino_code_IITKGP_WSN/Datalogger_step_09-08-2016.ino`  
- `DST_Project_Codes/Python_code_XBEE_Wireless_Transmit_Receive/Step_reciever_22-06-2015/final_ethernet_xbee_cordi`  
- `DST_Project_Codes/Python_code_XBEE_Wireless_Transmit_Receive/CIVILpy`
- `DST_Project_Codes/Python_code_XBEE_Wireless_Transmit_Receive/VSRC.py`
- `DST_Project_Codes/Python_code_XBEE_Wireless_Transmit_Receive/STEP.py` 
- `DST_Project_Codes/Python_code_XBEE_Wireless_Transmit_Receive/SOWR.py`  
- `DST_Project_Codes/Python_code_XBEE_Wireless_Transmit_Receive/Router_xBEE`  

---

## 📄 File Summaries

### `index.html`
- Google Maps interface with station markers and links to station web UIs on LAN (e.g., `http://10.171.9.208/`).

<img width="1016" height="590" alt="image" src="https://github.com/user-attachments/assets/990fcbdd-ce28-445c-8ffb-323a130f2975" />


### `Civil_DHT11_ethernet_allsensors_01_06_15.ino`
- **Role:** Station sketch with Ethernet web server.  
- **Sensors:** DHT11 (humidity, temperature), wind vane (ADC), wind speed (pulse count), tipping-bucket rain gauge (0.254 mm/tip), ACS712 current sensor.  
- **Behavior:** Hosts auto-refreshing HTML page (every 15s) showing timestamped meteorological data.

### `Datalogger_step_09-08-2016.ino`
- **Role:** SD-card datalogger for analog inputs.  
- **Behavior:** Creates daily/hourly CSV files with timestamp + analog readings every 60 seconds.

### `final_ethernet_xbee_cordi`
- **Role:** Arduino Ethernet + Serial gateway.  
- **Behavior:** Reads ASCII digits from Serial (XBee), converts to integer, applies calibration, serves value as HTML page on static LAN IP.

### `civil.py` and `sowr.py`
- **Role:** Python-based scrapers/loggers.  
- **Behavior:**  
  - Fetch station HTML.  
  - Parse rainfall values from specific `<p>` tags.  
  - Append to daily log files under Apache htdocs with 5-minute update window.  
- **Output:** Text logs organized by month and day (e.g.,  
  `...\Data\Civil\Rainfall <MM>-2015\Civil_<DD>.txt`).

### `Router_xBEE`
- **Role:** Arduino sketch for XBee router nodes.  
- **Behavior:**  
  - Reads ASCII digits from Serial until newline.  
  - Stores digits into buffer, reconstructs a 3-digit integer.  
  - Prints parsed sensor reading every second.  
- **Use:** Deploy on Arduino + XBee to forward RF payloads into numeric values.

---

## 🔄 System Workflow

1. **Sensors & Station Nodes**  
   - Civil station sketch reads sensors and serves results via Ethernet (static LAN IP).  
   - Rain gauge ticks → rainfall in mm (0.254 mm/tip).  

2. **Index / Navigation**  
   - `index.html` provides a Google Maps page linking to stations.  

3. **Wireless Bridging (XBee)**  
   - Router nodes (`Router_xBEE`) parse RF payloads.  
   - Gateway nodes (`final_ethernet_xbee_cordi`) receive via Serial + serve via Ethernet.  

4. **Client Logging**  
   - Python scrapers (`civil.py`, `sowr.py`) fetch HTML, extract rainfall, and log values.  

5. **Local Redundancy**  
   - Datalogger sketch logs analog readings to SD card every minute.

---

## ⚙️ Deployment Checklist

- **Civil Station**  
  - Flash `Civil_DHT11_ethernet_allsensors_01_06_15.ino`.  
  - Configure sensors + Ethernet IP.  
  - Verify station page opens in browser.  

- **Datalogger Node**  
  - Flash `Datalogger_step_09-08-2016.ino`.  
  - Insert SD card, confirm periodic CSV logging.  

- **Router Node (XBee)**  
  - Flash `Router_xBEE.ino`.  
  - Attach XBee and check Serial Monitor output.  

- **Gateway Node (Ethernet + XBee)**  
  - Flash `final_ethernet_xbee_cordi.ino`.  
  - Verify served HTML page shows sensor/pressure values.  

- **Python Loggers**  
  - Run `civil.py` / `sowr.py` on LAN-accessible PC with Python 3 + BeautifulSoup.  
  - Confirm logs appear under `htdocs/...`.

---

## 🛠 Notes

- Router_XBee parsing logic:  
  ```cpp
  p = (data[2] - 48) + 10 * (data[1] - 48) + 100 * (data[0] - 48);




## Distributed Computing Project
### Chord DHT Implementation in C/C++ (Coursework - Distributed Computing)
This project is an implementation of a Chord Distributed Hash Table (DHT). The system is designed to simulate a distributed key-value store by spawning multiple processes on a single Unix/Linux-based machine, where each process acts as a node in the DHT ring.
The project consists of a C++ backend that manages the DHT logic and a simple C-based command-line interface (CLI) for user interaction.

## CMOS_Camera_Codes
Contains the source code of integrating OV7670 CMOS camera with Arduino and make it compatible with Quadcopters.

## Quadcopter_Codes
Contains dev codes for a custom made quadcopter. Video - https://www.youtube.com/watch?v=NtLcCc_VHIg
