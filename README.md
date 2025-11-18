# Solar-LiFi Communication System

A solar-powered Light Fidelity (LiFi) communication system using Arduino and ESP8266. This project demonstrates wireless data transmission using visible light (laser diode) instead of radio waves.

## 🌟 Project Overview

This project implements a simple LiFi communication system where:
- **Arduino (Transmitter)**: Modulates a laser diode to encode and transmit data as light pulses
- **ESP8266 (Receiver)**: Uses a photodiode/LDR to detect light pulses and decode the data
- **Solar Panel**: Powers both devices, making the system energy-efficient and portable

LiFi (Light Fidelity) is a wireless communication technology that uses light to transmit data. It offers advantages like:
- No electromagnetic interference
- Secure communication (light doesn't pass through walls)
- High-speed potential
- Works in RF-sensitive environments

## 📋 Table of Contents

- [Components Required](#components-required)
- [Circuit Diagrams](#circuit-diagrams)
- [Installation](#installation)
- [Usage](#usage)
- [How It Works](#how-it-works)
- [Configuration](#configuration)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## 🔧 Components Required

### Bill of Materials (BOM)

#### Transmitter Side (Arduino)
- 1x Arduino Uno/Nano/Mega
- 1x Laser Diode Module (KY-008 or 5V laser diode)
- 1x 100Ω Resistor (current limiting for laser)
- 1x Solar Panel (6V, 1W minimum)
- 1x USB cable (for programming)
- Jumper wires
- Breadboard (optional)

#### Receiver Side (ESP8266)
- 1x ESP8266 NodeMCU or Wemos D1 Mini
- 1x Photodiode or LDR (Light Dependent Resistor)
- 1x 10kΩ Resistor (pull-down for sensor)
- 1x Solar Panel (6V, 1W minimum)
- 1x Micro USB cable (for programming)
- Jumper wires
- Breadboard (optional)

#### Additional Components
- Voltage regulators (if solar panel voltage > device voltage rating)
- Capacitors for power smoothing (100µF recommended)
- Enclosures for weatherproofing (optional)

**Estimated Total Cost**: $30-50 USD (depending on component quality and source)

## 📐 Circuit Diagrams

### Arduino Transmitter Circuit

```
Solar Panel (6V)
    |
    +--- [Voltage Regulator] ---> VIN (Arduino)
    |
    +----------------------------> GND (Arduino)

Laser Diode Module:
    Signal Pin --> Pin 9 (Arduino)
    VCC --------> 5V (Arduino) [via 100Ω resistor]
    GND --------> GND (Arduino)
```

#### Pin Connections:
| Component | Arduino Pin |
|-----------|-------------|
| Laser Diode Signal | Digital Pin 9 |
| Laser Diode VCC | 5V (via resistor) |
| Laser Diode GND | GND |
| Solar Panel (+) | VIN |
| Solar Panel (-) | GND |

### ESP8266 Receiver Circuit

```
Solar Panel (6V)
    |
    +--- [Voltage Regulator] ---> VIN (ESP8266)
    |
    +----------------------------> GND (ESP8266)

Photodiode/LDR:
    Signal Pin --> A0 (ESP8266)
    VCC --------> 3.3V (ESP8266)
    GND --------> GND (ESP8266) [via 10kΩ pull-down resistor]
```

#### Pin Connections:
| Component | ESP8266 Pin |
|-----------|-------------|
| Photodiode/LDR Signal | A0 (Analog) |
| Photodiode/LDR VCC | 3.3V |
| Photodiode/LDR GND | GND (via 10kΩ resistor) |
| Solar Panel (+) | VIN |
| Solar Panel (-) | GND |

## 💻 Installation

### Prerequisites

1. **Arduino IDE**: Download and install from [arduino.cc](https://www.arduino.cc/en/software)
2. **ESP8266 Board Support**: 
   - Open Arduino IDE
   - Go to File → Preferences
   - Add `http://arduino.esp8266.com/stable/package_esp8266com_index.json` to Additional Board Manager URLs
   - Go to Tools → Board → Board Manager
   - Search for "ESP8266" and install

### Setup Instructions

#### 1. Clone the Repository

```bash
git clone https://github.com/ArnavPC/Solar-LiFi.git
cd Solar-LiFi
```

#### 2. Upload Transmitter Code (Arduino)

1. Open `arduino_transmitter/arduino_transmitter.ino` in Arduino IDE
2. Select your Arduino board: Tools → Board → Arduino Uno (or your model)
3. Select the correct port: Tools → Port → (Your Arduino Port)
4. Click Upload button (→)
5. Wait for "Done uploading" message

#### 3. Upload Receiver Code (ESP8266)

1. Open `esp8266_receiver/esp8266_receiver.ino` in Arduino IDE
2. Select ESP8266 board: Tools → Board → ESP8266 Boards → NodeMCU 1.0 (or your model)
3. Select the correct port: Tools → Port → (Your ESP8266 Port)
4. Click Upload button (→)
5. Wait for "Done uploading" message

#### 4. Hardware Assembly

1. **Assemble transmitter circuit** following the circuit diagram above
2. **Assemble receiver circuit** following the circuit diagram above
3. **Connect solar panels** to both devices (through voltage regulators if needed)
4. **Align laser and photodiode**: Ensure the laser beam points directly at the photodiode/LDR

## 🚀 Usage

### Basic Operation

1. **Power on both devices** using solar panels or USB
2. **Open Serial Monitor** for transmitter (Arduino):
   - Tools → Serial Monitor
   - Set baud rate to 9600
3. **Open Serial Monitor** for receiver (ESP8266):
   - Tools → Serial Monitor
   - Set baud rate to 115200

### Sending Data

1. In the Arduino Serial Monitor, type your message
2. Press Enter to send
3. The message will be transmitted via laser
4. Check the ESP8266 Serial Monitor to see received data

### Example

**Arduino Transmitter Serial Monitor:**
```
=================================
Solar-LiFi Transmitter Initialized
=================================
Send data via Serial to transmit
Data will be transmitted via laser diode

Hello World
Transmitting: Hello World
Transmission complete
```

**ESP8266 Receiver Serial Monitor:**
```
=================================
Solar-LiFi Receiver Initialized
=================================
Waiting for LiFi transmission...
Threshold value: 512

Calibrating sensor...
Ambient light level: 245
Current threshold: 512
Calibration complete

Received: Hello World
```

## 🔍 How It Works

### Transmission Process (Arduino)

1. **User Input**: Message entered via Serial Monitor
2. **Encoding**: Message converted to bytes with start/end markers
3. **Modulation**: Each bit modulates the laser (ON = 1, OFF = 0)
4. **Transmission**: Light pulses sent through space

### Reception Process (ESP8266)

1. **Detection**: Photodiode converts light pulses to electrical signals
2. **Sampling**: Analog pin reads signal levels
3. **Demodulation**: Signal compared to threshold to extract bits
4. **Decoding**: Bits assembled into bytes and displayed

### Protocol Details

- **Start Marker**: 0xAA (170 in decimal)
- **End Marker**: 0x55 (85 in decimal)
- **Bit Duration**: 100 microseconds
- **Data Format**: [START][LENGTH][DATA...][END]
- **Modulation**: On-Off Keying (OOK)

## ⚙️ Configuration

### Adjusting Transmission Speed

In `arduino_transmitter.ino`:
```cpp
#define BIT_DELAY 100  // Increase for longer range, decrease for higher speed
```

In `esp8266_receiver.ino`:
```cpp
#define BIT_DELAY 100  // Must match transmitter
```

### Adjusting Light Sensitivity

In `esp8266_receiver.ino`:
```cpp
#define THRESHOLD 512  // Adjust based on ambient light (0-1023)
```

**Tips:**
- Higher threshold = less sensitive (for bright environments)
- Lower threshold = more sensitive (for dark environments)
- Use the calibration output to set appropriate value

### Power Settings

- **Solar Panel Voltage**: Ensure voltage is compatible with device (5-12V for Arduino, 5-12V for ESP8266)
- **Current Rating**: Minimum 500mA recommended for stable operation
- **Battery Backup**: Consider adding rechargeable batteries for continuous operation

## 🔧 Troubleshooting

### Common Issues

#### 1. No Data Received

**Symptoms**: Receiver shows no messages
**Solutions**:
- Check laser alignment with photodiode
- Verify all connections are secure
- Reduce ambient light (use in darker environment)
- Adjust THRESHOLD value in receiver code
- Check if laser diode is working (should see red dot)

#### 2. Garbled Data

**Symptoms**: Received messages are corrupted
**Solutions**:
- Ensure BIT_DELAY is same on both devices
- Improve laser-photodiode alignment
- Reduce distance between transmitter and receiver
- Shield photodiode from ambient light
- Check power supply stability

#### 3. Intermittent Reception

**Symptoms**: Some messages received, others lost
**Solutions**:
- Stabilize solar panel output with capacitors
- Ensure laser has consistent power
- Remove obstacles between laser and photodiode
- Increase BIT_DELAY for more reliable transmission

#### 4. Calibration Shows High Ambient Light

**Symptoms**: Warning message during calibration
**Solutions**:
- Cover photodiode from direct sunlight/room lights
- Lower the THRESHOLD value
- Use a photodiode with better directional sensitivity
- Add a tube/shroud around photodiode

### Debug Mode

To enable sensor debugging in `esp8266_receiver.ino`, add this to `loop()`:
```cpp
void loop() {
  debugSensor();  // Uncomment to see real-time sensor values
}
```

## 📚 Technical Specifications

- **Communication Type**: Unidirectional (Simplex)
- **Modulation**: On-Off Keying (OOK)
- **Transmission Medium**: Visible Light (650nm red laser)
- **Bit Rate**: ~10 Kbps (configurable)
- **Maximum Range**: 2-5 meters (depending on laser power and ambient light)
- **Power Consumption**: 
  - Arduino: ~50mA during transmission
  - ESP8266: ~80mA during reception
- **Operating Voltage**: 
  - Arduino: 5V
  - ESP8266: 3.3V logic, 5V power

## 🔒 Safety Warnings

⚠️ **IMPORTANT SAFETY INFORMATION**

- **Laser Safety**: Never look directly into the laser beam or point it at eyes
- **Use Class 1 or 2 lasers only** (< 1mW power output)
- **Adult Supervision**: This project involves electronics and lasers - adult supervision required for minors
- **Electrical Safety**: Ensure proper polarity when connecting solar panels and batteries
- **Heat**: Laser diodes can get warm during prolonged use - ensure adequate cooling

## 🤝 Contributing

Contributions are welcome! Here are some ways you can contribute:

- Report bugs and issues
- Suggest new features or improvements
- Submit pull requests with enhancements
- Improve documentation
- Share your builds and modifications

Please feel free to fork this repository and submit pull requests.

## 📝 Future Enhancements

- [ ] Bidirectional communication
- [ ] Error correction codes (CRC, Hamming)
- [ ] Encryption for secure communication
- [ ] Web interface for ESP8266
- [ ] Multiple channel support
- [ ] Automatic gain control (AGC)
- [ ] Battery management system
- [ ] Data logging capabilities
- [ ] Mobile app integration

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👥 Authors

- **Solar-LiFi Project Team**

## 🙏 Acknowledgments

- Arduino and ESP8266 communities
- Open-source contributors
- LiFi research community

## 📞 Support

For questions, issues, or suggestions:
- Open an issue on GitHub
- Check the troubleshooting section
- Review existing issues and discussions

---

**Made with ❤️ for sustainable and innovative communication technology**