# Solar-LiFi

## Overview
Solar-LiFi is an optical wireless communication system that uses a laser module as the transmitter and a solar panel as the receiver. This project demonstrates Light Fidelity (LiFi) technology by transmitting data through visible light at approximately 4 bits per second.

## Features
- **Auto-Calibration**: Receiver automatically calibrates to ambient light and laser intensity
- **Visual Feedback**: Real-time OLED display showing reception status and decoded bits
- **Robust Protocol**: Includes preamble detection, start/stop bits, and checksum verification
- **Low Speed Communication**: Optimized for solar panel response time (250ms per bit)

## Hardware Requirements

### Transmitter (TX)
- **Microcontroller**: ESP32
- **Output**: Laser module (connected to GPIO 4)
- **Power Supply**: USB or battery power for ESP32

### Receiver (RX)
- **Microcontroller**: Arduino UNO
- **Sensor**: Solar Panel (connected to A0 via voltage divider)
- **Display**: SSD1306 OLED (128x32 pixels)
- **Voltage Divider**: 100kΩ and 47kΩ resistors for solar panel signal conditioning

## Wiring Diagram

### Transmitter (ESP32)
```
ESP32 GPIO 4 ----> Laser Module (+)
GND -----------> Laser Module (-)
```

### Receiver (Arduino UNO)
```
Solar Panel (+) ----> [100kΩ] ----> A0 ----> [47kΩ] ----> GND
OLED SDA --------> A4
OLED SCL --------> A5
OLED VCC --------> 5V
OLED GND --------> GND
```

## Software Requirements

### Arduino Libraries
Install these libraries via Arduino IDE Library Manager:
- `Wire.h` (built-in)
- `Adafruit_GFX.h`
- `Adafruit_SSD1306.h`

## Communication Protocol

### Bit Rate
- **Bit Time**: 250ms per bit
- **Data Rate**: ~4 bits/second
- **Encoding**: LSB first, 8 data bits per byte

### Packet Structure
```
[PREAMBLE] [START BIT] [8 DATA BITS] [STOP BIT] ... [CHECKSUM] [TERMINATOR]
```

1. **Preamble**: HIGH (1000ms) → LOW (500ms)
   - Purpose: Synchronize receiver and detect start of transmission
   
2. **Start Bit**: HIGH (250ms)
   - Purpose: Indicate beginning of each byte
   
3. **Data Bits**: 8 bits (250ms each)
   - LSB transmitted first
   - HIGH = 1, LOW = 0
   
4. **Stop Bit**: LOW (250ms)
   - Purpose: Prepare line for next start bit
   
5. **Checksum**: Sum of all bytes modulo 256
   - Purpose: Basic error detection
   
6. **Terminator**: Line goes LOW
   - Purpose: End of transmission

## Setup Instructions

### 1. Upload Transmitter Code
1. Open `transmitter.ino` in Arduino IDE
2. Select **ESP32 Dev Module** as the board
3. Connect ESP32 via USB
4. Upload the code
5. Open Serial Monitor (115200 baud) to see transmission status

### 2. Upload Receiver Code
1. Open `receiver.ino` in Arduino IDE
2. Select **Arduino UNO** as the board
3. Connect Arduino UNO via USB
4. Upload the code
5. Open Serial Monitor (115200 baud) to see reception status

### 3. Calibration Process
The receiver performs automatic calibration on startup:

1. **Step 1 - Ambient Light Measurement (2 seconds)**
   - OLED displays: "1. LASER OFF (2s)"
   - Keep laser OFF or blocked
   - System measures ambient light level

2. **Step 2 - Laser Measurement (4 seconds)**
   - OLED displays: "2. SHINE LASER (4s)"
   - Point laser at solar panel
   - System measures maximum laser intensity

3. **Step 3 - Threshold Calculation**
   - Threshold = (Ambient + Laser) / 2
   - OLED displays ambient, laser, and calculated threshold values
   - Wait 3 seconds before entering receive mode

### 4. Operation
1. Position the laser module to point at the solar panel
2. Adjust distance for optimal signal (typically 10-50cm)
3. Transmitter continuously sends the message "IMAGE#"
4. Receiver displays:
   - "RX LOCKED" when preamble is detected
   - Live bit visualization during decoding
   - Received message on top line
   - "COMPLETE" when packet reception is done

## Modifying the Message

To change the transmitted message, edit the `transmitter.ino` file:

```cpp
const char* message = "IMAGE#"; // Change this to your message
```

**Note**: Messages should end with `#` as a termination marker.

## Troubleshooting

### Receiver Not Detecting Signal
- Verify laser is powered and lit
- Check solar panel connections (A0 pin)
- Ensure proper voltage divider (100kΩ/47kΩ)
- Re-run calibration with proper laser alignment
- Verify OLED displays calibration values

### OLED Not Working
- Check I2C connections (SDA to A4, SCL to A5)
- Verify OLED address (default 0x3C)
- Check power supply (5V and GND)

### Garbled Data Reception
- Increase distance between laser and solar panel
- Reduce ambient light interference
- Check for stable power supply
- Re-run calibration procedure

### Serial Monitor Shows Nothing
- Verify baud rate is set to 115200
- Check USB cable connection
- Ensure correct COM port is selected

## Technical Specifications

### Timing Parameters
- `BIT_TIME_US`: 250,000 µs (250ms per bit)
- `PREAMBLE_MIN_US`: 800,000 µs (minimum preamble duration)
- Calibration ambient measurement: 50 samples over 500ms
- Calibration laser measurement: 4-second window

### Signal Processing
- Triple sampling per bit for noise reduction
- Majority voting (2 out of 3 samples must agree)
- Dynamic threshold adjustment based on calibration
- Hysteresis margin: threshold ± 5 for preamble detection

### Buffer Sizes
- Receive buffer: 32 characters
- Maximum message length: 30 characters (plus null terminator)

## Performance Characteristics

- **Range**: 10-50cm (varies with laser power and ambient light)
- **Data Rate**: ~4 bits/second
- **Packet Rate**: ~1 packet per 3 seconds (including delays)
- **Reliability**: Good in low ambient light conditions
- **Latency**: ~2.5 seconds per character transmission

## Safety Considerations

⚠️ **WARNING**: Laser Safety
- Never point laser at eyes or reflective surfaces
- Use appropriate class laser module (Class 1 or 2)
- Follow local regulations for laser devices
- Keep laser power under 5mW for safety

## Applications

This system demonstrates the principles of:
- Optical wireless communication
- Visible Light Communication (VLC)
- Light Fidelity (LiFi) technology
- Energy harvesting for communication
- Alternative data transmission methods

Potential use cases:
- Educational demonstrations
- Secure line-of-sight communication
- Underwater communication (with appropriate modifications)
- Space-constrained environments
- RF-restricted areas

## Future Enhancements

- Higher data rates with faster sensors
- Forward Error Correction (FEC)
- Multiple channel support
- Bidirectional communication
- Extended range with better optics
- Adaptive threshold adjustment during operation

## License

This project is open source. Please refer to the LICENSE file for more information.

## Contributing

Contributions are welcome! Please feel free to submit issues, fork the repository, and create pull requests for any improvements.

## Credits

Developed as a demonstration of optical wireless communication using readily available components.