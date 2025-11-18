# Quick Start Guide

Get your Solar-LiFi system running in 15 minutes!

## What You'll Need

- Arduino Uno + USB cable
- ESP8266 NodeMCU + USB cable  
- Laser diode module (KY-008)
- Photodiode or LDR
- 100Ω resistor
- 10kΩ resistor
- Jumper wires
- Breadboard
- Arduino IDE installed

## 5 Simple Steps

### Step 1: Install Software (3 minutes)

1. Download Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
2. Add ESP8266 support:
   - File → Preferences
   - Add URL: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
   - Tools → Board Manager → Install "esp8266"

### Step 2: Wire Transmitter (3 minutes)

**Arduino Connections:**
```
Laser Module → Arduino
Signal       → Pin 9
VCC          → 5V (through 100Ω resistor)
GND          → GND
```

### Step 3: Wire Receiver (3 minutes)

**ESP8266 Connections:**
```
Photodiode → ESP8266
Anode (+)  → 3.3V
Cathode(-) → A0 (+ 10kΩ resistor to GND)
```

### Step 4: Upload Code (4 minutes)

**Arduino:**
1. Open `arduino_transmitter/arduino_transmitter.ino`
2. Select: Tools → Board → Arduino Uno
3. Select: Tools → Port → (your Arduino port)
4. Click Upload (→)

**ESP8266:**
1. Open `esp8266_receiver/esp8266_receiver.ino`
2. Select: Tools → Board → NodeMCU 1.0
3. Select: Tools → Port → (your ESP8266 port)
4. Click Upload (→)

### Step 5: Test! (2 minutes)

1. Open Serial Monitor for Arduino (9600 baud)
2. Open Serial Monitor for ESP8266 (115200 baud)
3. Place devices 30cm apart
4. Point laser at photodiode
5. Type "Hello" in Arduino Serial Monitor
6. Press Enter
7. See "Received: Hello" in ESP8266 monitor!

## Troubleshooting

**Nothing received?**
- Align laser directly at photodiode (look for red dot)
- Dim the lights in your room
- Move devices closer together

**Garbled text?**
- Verify BIT_DELAY is 100 in both files
- Improve laser-photodiode alignment
- Check all wire connections

## Next Steps

✅ Working? Great! Now explore:
- [README.md](README.md) - Full documentation
- [SETUP_GUIDE.md](SETUP_GUIDE.md) - Detailed instructions
- [WIRING_GUIDE.md](WIRING_GUIDE.md) - Circuit diagrams
- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) - Common issues

## Adding Solar Power (Optional)

Once basic communication works:
1. Add 6V solar panel to VIN pin (both devices)
2. Add 100µF capacitor between VIN and GND
3. Test in sunlight
4. See [WIRING_GUIDE.md](WIRING_GUIDE.md) for details

---

**Need help?** Check [TROUBLESHOOTING.md](TROUBLESHOOTING.md) or open an issue on GitHub!

**It works!** 🎉 Star the repo and share your project!
