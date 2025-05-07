<br>
<div align=center>
<img src="https://github.com/dot1nt/OpenLap/blob/main/assets/image_5.png" width="144"/>
<h1>OpenLap</h1> 
<strong>A single pilot ESP32 / RX5808 based FPV laptimer</strong>
</div>
<br>

![image_1](https://github.com/dot1nt/OpenLap/blob/main/assets/image_1.png)

# Build

## Hardware  
You need an RX5808 with [SPI Mod](https://sheaivey.github.io/rx5808-pro-diversity/docs/rx5808-spi-mod.html). Desolder the antenna and connect the receiver like this:  
- **CH1** → pin `D23` (MOSI)
- **CH2** → pin `D22`
- **CH3** → pin `D18` (SCK)
- **RSSI** → pin `D33`

![image_3](https://github.com/dot1nt/OpenLap/blob/main/assets/image_3.png)

## Firmware  
You’ll need [VS Code](https://code.visualstudio.com/) and the PlatformIO extension.
Once you've downloaded the code (click the green 'Code' button at the top and select 'Download ZIP'), open the project and hit the upload arrow at the bottom to flash the firmware.  
The web page has to be uploaded separately — open PlatformIO (left sidebar) and click "Upload Filesystem Image".

## Configuration  
Settings are in `src/config.h`.
If you are not using the standard ESP32 developement board, you can change the board type in the `platformio.ini` file.
Here are the board types supported by platformio that might work: https://docs.platformio.org/en/latest/boards/index.html#espressif-32

### Battery Monitoring

The ESP32 has a 3.3V input voltage limit, so it's not safe to connect the battery directly to read the voltage. 
To bring the voltage down to a safe level, you can use a [voltage divider](https://en.wikipedia.org/wiki/Voltage_divider) consisting of two resistors to step down the voltage before it reaches the ESP32.

You can configure the resistor values for the voltage divider by adjusting `R1` and `R2` in `src/config.h`.
You can use this [calculator](https://ohmslawcalculator.com/voltage-divider-calculator) to select suitable resistor values.
For a 4s battery, safe resistor values could be `100kOhm` for R1 and `20kOhm` for R2.

To enable battery monitoring:
1. Set `ENABLE_BATTERY_MONITORING` to `true`.
2. Set the pin where the voltage divider is connected by modifying `BATTERY_PIN` (this needs to be an analog input).

Additionally, you can set a minimum voltage threshold in the web page settings to receive a notification when the battery voltage drops below this level.

# Usage  
The ESP32 opens a Wi-Fi AP you can connect to. Wi-Fi name and password can be changed in `src/config.h`. The default password is `Laptimer`.
To access the app, open `http://192.168.4.1` in your browser.  
Pick your band and channel in the settings, save it, then hold your drone in the worst signal spot at the gate and click "Calibrate".
Set lap number to `0` if you want the race to run forever.

![image_2](https://github.com/dot1nt/OpenLap/blob/main/assets/image_2.png)

# Case  
The case files are inside the `cad` folder. Mount the ESP32 with thick double sided tape.

![image_4](https://github.com/dot1nt/OpenLap/blob/main/assets/image_4.png)
