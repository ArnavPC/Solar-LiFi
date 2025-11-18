# Troubleshooting and FAQ

Comprehensive troubleshooting guide and frequently asked questions for the Solar-LiFi project.

## Table of Contents
- [Troubleshooting Guide](#troubleshooting-guide)
- [Frequently Asked Questions](#frequently-asked-questions)
- [Error Messages](#error-messages)
- [Performance Issues](#performance-issues)
- [Hardware Problems](#hardware-problems)

## Troubleshooting Guide

### Communication Issues

#### Issue 1: No Data Received

**Symptoms:**
- Transmitter sends data
- Receiver shows no messages
- No error messages

**Diagnostic Steps:**
1. Check laser visibility (red dot)
2. Verify receiver is powered
3. Check Serial Monitor baud rates
4. Confirm code uploaded successfully

**Solutions:**

**A. Alignment Problem**
```
1. Move devices closer (30cm)
2. Point laser directly at photodiode
3. Look for laser dot on photodiode
4. Secure devices to prevent movement
```

**B. Threshold Too High**
```cpp
// In esp8266_receiver.ino
#define THRESHOLD 512  // Try lower value: 300-400
```

**C. Photodiode Wiring**
```
Check connections:
- Photodiode anode → 3.3V
- Photodiode cathode → A0 + 10kΩ to GND
```

#### Issue 2: Garbled Data

**Symptoms:**
- Receiver gets messages but they're corrupted
- Random characters instead of sent message
- Partial messages received

**Diagnostic Steps:**
1. Check timing synchronization
2. Verify BIT_DELAY is identical
3. Test power supply stability
4. Check for electromagnetic interference

**Solutions:**

**A. Timing Mismatch**
```cpp
// Ensure both files have SAME value
// arduino_transmitter.ino:
#define BIT_DELAY 100

// esp8266_receiver.ino:
#define BIT_DELAY 100
```

**B. Slow Down Transmission**
```cpp
// Increase delay for more reliable transmission
#define BIT_DELAY 200  // in both files
```

**C. Power Supply Issues**
```
Add larger capacitors:
- 220µF between VIN and GND
- 100nF near each IC
```

**D. Ambient Light Interference**
```
Solutions:
- Shield photodiode from ambient light
- Use tube/shroud around photodiode
- Increase THRESHOLD value
- Test in darker environment
```

#### Issue 3: Intermittent Communication

**Symptoms:**
- Sometimes works, sometimes doesn't
- Messages randomly lost
- Success rate < 70%

**Diagnostic Steps:**
1. Monitor power supply voltage
2. Check for loose connections
3. Verify alignment stability
4. Test at different times of day

**Solutions:**

**A. Unstable Power**
```
Fixes:
- Add 470µF capacitor on VIN
- Use voltage regulator with good line regulation
- Add battery backup for stable power
- Check solar panel wiring
```

**B. Poor Alignment**
```
Improvements:
- Mount both devices on stable platform
- Use adjustable stands
- Add alignment guides
- Mark optimal position
```

**C. Environmental Factors**
```
- Test in controlled environment
- Shield from wind/vibration
- Protect from temperature changes
- Cover photodiode from direct sunlight
```

### Hardware Problems

#### Issue 4: Laser Doesn't Light Up

**Symptoms:**
- No visible laser dot
- Transmitter appears to run normally
- No light emission

**Diagnostic Steps:**
1. Check if laser module is functional
2. Verify pin 9 connection
3. Test with multimeter
4. Check current limiting resistor

**Solutions:**

**A. Test Laser Directly**
```cpp
// Add to setup() for testing
void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  
  // Test pattern
  while(true) {
    digitalWrite(9, HIGH);
    delay(1000);
    digitalWrite(9, LOW);
    delay(1000);
  }
}
```

**B. Check Wiring**
```
Verify:
1. Laser signal pin → Arduino pin 9
2. Laser VCC → 5V via 100Ω resistor
3. Laser GND → Arduino GND
```

**C. Test with Multimeter**
```
Set multimeter to continuity mode:
1. Touch pin 9 and laser signal pin (should beep)
2. Check voltage on laser VCC (should be ~4-5V)
3. Verify GND connection
```

**D. Replace Resistor/Laser**
```
- Try different 100Ω resistor
- Test with known-good laser module
- Verify laser module is 5V compatible
```

#### Issue 5: ESP8266 Won't Upload

**Symptoms:**
- Upload fails repeatedly
- Error messages in Arduino IDE
- ESP8266 not detected

**Diagnostic Steps:**
1. Check USB cable (must be data cable)
2. Install correct drivers
3. Verify board selection
4. Try different USB port

**Solutions:**

**A. Driver Installation**

**Windows:**
```
1. Download CH340 or CP2102 driver
2. Install driver
3. Restart computer
4. Reconnect ESP8266
5. Check Device Manager for COM port
```

**Linux:**
```bash
# Usually automatic, but if needed:
sudo apt-get install python-serial
# Add user to dialout group:
sudo usermod -a -G dialout $USER
# Logout and login again
```

**macOS:**
```
# Install driver from:
https://github.com/adrianmihalko/ch340g-ch34g-ch34x-mac-os-x-driver
# Restart after installation
```

**B. Upload Procedure**
```
1. Select: Tools → Board → NodeMCU 1.0
2. Select: Tools → Port → [Your ESP8266 Port]
3. Hold FLASH button on ESP8266
4. Click Upload
5. Release FLASH when "Connecting..." appears
6. Wait for "Done uploading"
```

**C. Troubleshooting Upload**
```
If still failing:
- Reduce upload speed: Tools → Upload Speed → 115200
- Try esptool.py directly
- Check for bad USB cable
- Test on different computer
```

#### Issue 6: Photodiode Not Responding

**Symptoms:**
- Calibration shows constant value
- No response to light changes
- Sensor reading always 0 or 1023

**Diagnostic Steps:**
1. Test photodiode with flashlight
2. Check analog pin wiring
3. Verify voltage on photodiode
4. Test with known-good sensor

**Solutions:**

**A. Test Sensor Manually**
```cpp
// Add to loop() for testing
void loop() {
  int val = analogRead(A0);
  Serial.print("Sensor value: ");
  Serial.println(val);
  delay(100);
}
```

**B. Verify Wiring**
```
Correct connections:
1. Photodiode anode (+) → 3.3V
2. Photodiode cathode (-) → A0
3. 10kΩ resistor from A0 → GND
```

**C. Test Response**
```
1. Upload debug code above
2. Open Serial Monitor (115200 baud)
3. Cover photodiode with hand (value should drop)
4. Shine flashlight (value should rise)
5. Typical range: 50-900
```

**D. Alternative Sensor Test**
```
If using LDR instead of photodiode:
- Swap connection order if needed
- LDR is non-polarized
- Adjust THRESHOLD accordingly
```

### Software Problems

#### Issue 7: Compilation Errors

**Common Error 1:**
```
'WiFi' was not declared in this scope
```
**Solution:** Install ESP8266 board support (see SETUP_GUIDE.md)

**Common Error 2:**
```
exit status 1 - Error compiling for board Arduino Uno
```
**Solution:** Select correct board type in Tools → Board

**Common Error 3:**
```
Sketch too big
```
**Solution:** Select board with more memory, or optimize code

#### Issue 8: Serial Monitor Shows Garbage

**Symptoms:**
- Random characters displayed
- Unreadable text
- Symbols instead of messages

**Solutions:**

**A. Wrong Baud Rate**
```
Correct baud rates:
- Arduino transmitter: 9600
- ESP8266 receiver: 115200
```

**B. Both Line Ending and Carriage Return**
```
In Serial Monitor:
- Set dropdown to "Newline" or "Both NL & CR"
```

**C. Cable/Connection Issue**
```
- Try different USB cable
- Check for loose connections
- Test on different USB port
```

## Frequently Asked Questions

### General Questions

**Q1: What is LiFi?**

A: LiFi (Light Fidelity) is a wireless communication technology that uses light instead of radio waves to transmit data. It modulates light at high speeds to encode digital information.

**Q2: Why use laser instead of LED?**

A: Lasers provide:
- Better range due to focused beam
- Higher modulation speeds
- Less interference from ambient light
- More precise alignment

However, LEDs are safer and can also work for shorter ranges.

**Q3: Is this safe?**

A: Yes, if you use Class 1 or Class 2 lasers (<1mW). Never look directly at the laser beam. The laser diodes suggested in this project are low-power and safe for educational use.

**Q4: What's the maximum range?**

A: Typical range is 2-5 meters depending on:
- Laser power
- Ambient light conditions
- Photodiode sensitivity
- Bit rate settings

Can be extended to 10+ meters with optimization.

**Q5: Can this work in daylight?**

A: Yes, but performance is reduced. Solutions:
- Shield photodiode from direct sunlight
- Increase THRESHOLD value
- Use higher power laser (within safety limits)
- Add optical filters

### Technical Questions

**Q6: What is the data rate?**

A: Default configuration: ~10 Kbps
Can be increased to 20-50 Kbps with:
- Faster photodiode
- Lower BIT_DELAY value
- Better alignment
- Darker environment

**Q7: Can I make it bidirectional?**

A: Yes! Add laser and photodiode to both devices. You'll need to:
1. Implement collision detection
2. Add turn-taking protocol
3. Align lasers in both directions

**Q8: Why ESP8266 instead of another Arduino?**

A: ESP8266 offers:
- Faster processor for reception
- Built-in WiFi (for future expansion)
- More memory
- Better analog input

But you can use Arduino for receiver too with modifications.

**Q9: Can I transmit audio/video?**

A: Audio: Possible with modifications
Video: Not with this simple system

The current system is designed for text data. Audio would require:
- Much higher bit rates (44 kHz sampling)
- Buffering and compression
- Different modulation scheme

**Q10: Does this work through glass?**

A: Yes! Glass is transparent to visible light. However:
- Some light will reflect
- Range may be reduced
- Dirty/tinted glass affects performance

### Hardware Questions

**Q11: What solar panel size do I need?**

A: Minimum recommendations:
- 6V output voltage
- 1W power rating (167mA @ 6V)
- 100mA minimum current

Larger panels provide:
- Faster charging
- Better stability
- Operation in lower light

**Q12: Can I use different lasers?**

A: Yes, but ensure:
- Operating voltage matches (5V for Arduino)
- Power rating < 1mW for safety
- Wavelength 650nm (red) works best
- Proper current limiting

**Q13: What if I don't have a photodiode?**

A: Alternatives:
1. **LDR (Light Dependent Resistor)**: Works but slower
2. **Phototransistor**: Good alternative
3. **Solar cell**: Can work as light detector
4. **Camera sensor**: Advanced option

**Q14: Do I need the solar panel?**

A: No, solar panels are optional. You can power via:
- USB cable (easiest for testing)
- Battery pack
- Wall adapter
- Combination solar + battery

Solar panels make it portable and eco-friendly.

### Software Questions

**Q15: Can I use different microcontrollers?**

A: Yes! This code can be adapted for:
- **Raspberry Pi Pico**: Faster, more memory
- **STM32**: High performance
- **Teensy**: Very fast, good for higher speeds
- **ESP32**: Better than ESP8266

You'll need to adjust pin definitions and timing.

**Q16: How do I add error correction?**

A: Implement checksums:

```cpp
// Add to transmitter
byte calculateChecksum(byte* data, int len) {
  byte sum = 0;
  for(int i = 0; i < len; i++) {
    sum ^= data[i];
  }
  return sum;
}

// Send checksum after data
byte checksum = calculateChecksum(message, length);
transmitByte(checksum);
```

**Q17: Can I encrypt the data?**

A: Yes! Options:
1. **XOR cipher**: Simple but weak
2. **AES**: Strong, libraries available
3. **Custom encoding**: Roll your own

Example XOR:
```cpp
byte key = 0xA5;
byte encrypted = data ^ key;
transmitByte(encrypted);
```

**Q18: How do I log received data?**

A: Add SD card or EEPROM:

```cpp
#include <SD.h>

void logData(String data) {
  File file = SD.open("lifi_log.txt", FILE_WRITE);
  file.println(data);
  file.close();
}
```

### Troubleshooting Questions

**Q19: Why is my range so short?**

A: Common causes:
1. Ambient light too bright
2. Laser power too low
3. Photodiode not sensitive enough
4. Misalignment
5. Obstruction in path

See "Improving Range" in SETUP_GUIDE.md

**Q20: Why does it work sometimes but not others?**

A: Likely causes:
1. Unstable power supply
2. Temperature changes
3. Vibration/movement
4. Variable ambient light
5. Loose connections

Add capacitors and secure connections.

## Error Messages

### Transmitter Error Messages

```
Error: No data to transmit
Solution: Type message in Serial Monitor
```

```
Error: Serial not available
Solution: Increase baud rate or check USB connection
```

### Receiver Error Messages

```
Error: Invalid end marker
Meaning: Transmission corrupted
Solution: Improve alignment, reduce ambient light
```

```
WARNING: Ambient light is above threshold!
Meaning: Too much background light
Solution: Adjust THRESHOLD or reduce ambient light
```

```
Error: Message length invalid
Meaning: Sync lost during reception
Solution: Improve signal quality, check alignment
```

## Performance Issues

### Low Success Rate (<70%)

**Causes & Solutions:**
1. **Bad alignment**: Realign devices carefully
2. **Timing issues**: Verify BIT_DELAY matches
3. **Power fluctuations**: Add capacitors
4. **Ambient light**: Shield photodiode
5. **Distance too far**: Move closer

### Slow Transmission

**Causes & Solutions:**
1. **BIT_DELAY too high**: Reduce value carefully
2. **Serial baud too low**: Increase (within limits)
3. **Message overhead**: Optimize protocol
4. **Processing delays**: Optimize code

### High Error Rate

**Causes & Solutions:**
1. **Noise**: Add error correction
2. **Weak signal**: Increase laser power
3. **Saturation**: Reduce laser power
4. **Jitter**: Improve timing accuracy

## Hardware Problems

### Overheating Components

**If laser gets hot:**
- Add heatsink
- Reduce duty cycle
- Check resistor value
- Improve ventilation

**If voltage regulator gets hot:**
- Use regulator with better efficiency
- Add heatsink
- Reduce load
- Use switching regulator

### Corrosion or Oxidation

**Prevention:**
- Use conformal coating
- Seal enclosure
- Add desiccant
- Regular maintenance

## Getting More Help

If you've tried everything:

1. **Check GitHub Issues**: Someone may have had same problem
2. **Open New Issue**: Describe problem with details:
   - Hardware used
   - Code modifications
   - Error messages
   - What you've tried
3. **Join Forums**: Arduino/ESP8266 communities
4. **Consult Datasheets**: Check component specifications

## Success Tips

✅ **Do:**
- Test components individually first
- Use proper wiring techniques
- Document your setup
- Make one change at a time
- Take notes on what works

❌ **Don't:**
- Look at laser directly
- Skip calibration
- Ignore error messages
- Make multiple changes at once
- Give up too quickly

---

**Still stuck?** Open an issue on GitHub with detailed information about your problem.

**Found a solution not listed here?** Please contribute by updating this document!
