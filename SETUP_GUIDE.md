# Setup and Testing Guide

Complete step-by-step guide for setting up and testing your Solar-LiFi communication system.

## Table of Contents
- [Quick Start](#quick-start)
- [Detailed Setup](#detailed-setup)
- [First-Time Configuration](#first-time-configuration)
- [Testing Procedures](#testing-procedures)
- [Performance Optimization](#performance-optimization)
- [Common Use Cases](#common-use-cases)

## Quick Start

**For experienced users:**

1. Clone repository and open in Arduino IDE
2. Upload `arduino_transmitter.ino` to Arduino
3. Upload `esp8266_receiver.ino` to ESP8266
4. Wire components per [WIRING_GUIDE.md](WIRING_GUIDE.md)
5. Align laser with photodiode
6. Send test message via Serial Monitor

**Expected time:** 30 minutes

## Detailed Setup

### Phase 1: Software Preparation (10 minutes)

#### 1.1 Install Arduino IDE

**Windows:**
```
1. Download from https://www.arduino.cc/en/software
2. Run installer
3. Follow installation wizard
4. Launch Arduino IDE
```

**macOS:**
```
1. Download .dmg file
2. Drag Arduino to Applications
3. Launch from Applications folder
```

**Linux:**
```bash
# Download and extract
cd ~/Downloads
wget https://downloads.arduino.cc/arduino-1.8.19-linux64.tar.xz
tar -xf arduino-1.8.19-linux64.tar.xz
cd arduino-1.8.19
./install.sh
```

#### 1.2 Install ESP8266 Board Support

1. Open Arduino IDE
2. Go to **File → Preferences**
3. In "Additional Board Manager URLs", add:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
4. Click **OK**
5. Go to **Tools → Board → Board Manager**
6. Search for "esp8266"
7. Click **Install** on "esp8266 by ESP8266 Community"
8. Wait for installation to complete

#### 1.3 Download Project Files

**Option A: Using Git**
```bash
git clone https://github.com/ArnavPC/Solar-LiFi.git
cd Solar-LiFi
```

**Option B: Direct Download**
1. Visit https://github.com/ArnavPC/Solar-LiFi
2. Click green "Code" button
3. Select "Download ZIP"
4. Extract to desired location

### Phase 2: Hardware Assembly (20 minutes)

Follow the [WIRING_GUIDE.md](WIRING_GUIDE.md) for detailed instructions.

**Quick Assembly Checklist:**
- [ ] Arduino connected to laser diode (Pin 9)
- [ ] ESP8266 connected to photodiode (Pin A0)
- [ ] Solar panels connected (with voltage regulation)
- [ ] All grounds connected
- [ ] Protection diodes installed
- [ ] Smoothing capacitors added

### Phase 3: Code Upload (10 minutes)

#### 3.1 Upload to Arduino (Transmitter)

1. Connect Arduino via USB
2. Open `arduino_transmitter/arduino_transmitter.ino`
3. Select board:
   - **Tools → Board → Arduino AVR Boards → Arduino Uno**
   (or your specific Arduino model)
4. Select port:
   - **Tools → Port → COM3** (Windows)
   - **Tools → Port → /dev/ttyUSB0** (Linux)
   - **Tools → Port → /dev/cu.usbserial** (macOS)
5. Click **Upload** button (→)
6. Wait for "Done uploading" message
7. Open **Serial Monitor** (Tools → Serial Monitor)
8. Set baud rate to **9600**
9. Verify startup message appears

**Expected Output:**
```
=================================
Solar-LiFi Transmitter Initialized
=================================
Send data via Serial to transmit
Data will be transmitted via laser diode
```

#### 3.2 Upload to ESP8266 (Receiver)

1. Disconnect Arduino (to free up USB port)
2. Connect ESP8266 via USB
3. Open `esp8266_receiver/esp8266_receiver.ino`
4. Select board:
   - **Tools → Board → ESP8266 Boards → NodeMCU 1.0 (ESP-12E Module)**
   (adjust based on your ESP8266 variant)
5. Select port (similar to Arduino)
6. Set upload speed:
   - **Tools → Upload Speed → 115200**
7. Click **Upload** button (→)
8. Wait for "Done uploading" message
9. Open **Serial Monitor**
10. Set baud rate to **115200**
11. Press ESP8266 reset button
12. Verify startup and calibration messages

**Expected Output:**
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
```

### Phase 4: Initial Testing (10 minutes)

#### 4.1 Component Testing

**Test 1: Laser Diode**
1. With Arduino powered on
2. Observe laser diode (DO NOT look directly at it)
3. Type "test" in Arduino Serial Monitor and press Enter
4. Laser should blink rapidly during transmission
5. You should see a dim red dot on nearby surface

**Test 2: Photodiode Sensitivity**
1. With ESP8266 powered on
2. Check "Ambient light level" from calibration
3. Shine flashlight on photodiode
4. Watch for reading changes
5. Verify threshold is appropriate

#### 4.2 Communication Testing

**Simple Test:**
1. Position devices 30cm apart
2. Point laser directly at photodiode
3. In Arduino Serial Monitor, type: `Hello`
4. Press Enter
5. Check ESP8266 Serial Monitor for: `Received: Hello`

**If successful:** Proceed to Phase 5
**If unsuccessful:** See [Troubleshooting](#troubleshooting-during-setup)

## First-Time Configuration

### Optimal Positioning

#### Laser Alignment

```
Transmitter (Arduino)          Receiver (ESP8266)
     [Laser] ─────────────→ [Photodiode]
        │                         │
     30-100cm distance
```

**Tips:**
1. Start with short distance (30cm)
2. Use a ruler or straight edge for alignment
3. Darken room for initial testing
4. Look for laser dot on photodiode surface
5. Secure both devices to prevent movement

### Calibration

#### Automatic Calibration (Recommended)

The receiver performs automatic calibration on startup:
```
Ambient light level: XXX
Current threshold: 512
```

**Interpreting Results:**
- Ambient < 300: Good, dark environment
- Ambient 300-500: Acceptable, adjust if needed
- Ambient > 500: Warning, too bright

#### Manual Threshold Adjustment

If automatic calibration shows warnings:

1. Open `esp8266_receiver.ino`
2. Find this line:
   ```cpp
   #define THRESHOLD 512
   ```
3. Adjust value:
   - Increase if ambient > threshold
   - New threshold = ambient level + 100-200
   - Example: If ambient = 600, set THRESHOLD to 750
4. Save and re-upload code

### Solar Panel Setup

#### Initial Power-Up Sequence

1. **Test USB Power First:**
   - Verify both devices work with USB
   - Complete communication test
   - Ensure no hardware issues

2. **Add Solar Panel:**
   - Connect solar panel to transmitter
   - Remove USB cable
   - Verify operation in sunlight
   - Repeat for receiver

3. **Monitor Stability:**
   - Watch for voltage fluctuations
   - Add capacitors if needed
   - Consider battery backup

## Testing Procedures

### Basic Communication Tests

#### Test 1: Single Character
```
Send: A
Expected: Received: A
```

#### Test 2: Short Message
```
Send: Hello
Expected: Received: Hello
```

#### Test 3: Long Message
```
Send: The quick brown fox jumps over the lazy dog
Expected: Received: The quick brown fox jumps over the lazy dog
```

#### Test 4: Numbers
```
Send: 1234567890
Expected: Received: 1234567890
```

#### Test 5: Special Characters
```
Send: !@#$%^&*()
Expected: Received: !@#$%^&*()
```

### Range Testing

**Procedure:**
1. Start at 30cm distance
2. Test communication
3. Move receiver back 10cm
4. Repeat test
5. Continue until communication fails
6. Record maximum reliable distance

**Expected Range:**
- Indoor (dim light): 2-3 meters
- Outdoor (bright): 1-2 meters
- Dark room: 3-5 meters

### Speed Testing

**Measure transmission speed:**
1. Send message of known length (e.g., 50 characters)
2. Time the transmission
3. Calculate: characters per second
4. Expected: ~100 characters/second with default settings

### Reliability Testing

**10-Message Test:**
1. Send 10 different messages
2. Count successful receptions
3. Calculate success rate
4. Target: >90% success rate

### Environmental Testing

#### Test Different Lighting Conditions

1. **Darkness:** Best performance
2. **Indoor Lighting:** Good performance
3. **Bright Sunlight:** Reduced performance
4. **Variable Light:** May need recalibration

#### Test Different Distances

Create a table of results:
| Distance | Success Rate | Notes |
|----------|--------------|-------|
| 30cm     | 100%         | Perfect |
| 50cm     | 100%         | Good |
| 100cm    | 95%          | Occasional errors |
| 150cm    | 80%          | Frequent errors |
| 200cm    | 50%          | Unreliable |

## Performance Optimization

### Improving Range

1. **Increase Laser Power** (within safety limits)
2. **Use Focusing Lens** on laser
3. **Add Amplifier Circuit** for photodiode
4. **Shield Photodiode** from ambient light
5. **Use Higher Quality Photodiode**

### Improving Reliability

1. **Decrease Bit Rate:**
   ```cpp
   #define BIT_DELAY 200  // Slower but more reliable
   ```

2. **Add Error Detection:**
   - Implement checksum
   - Add parity bits
   - Use error correction codes

3. **Improve Alignment:**
   - Use laser pointer for alignment
   - Mount devices on stable platform
   - Add fine adjustment screws

### Improving Speed

1. **Decrease Bit Delay:**
   ```cpp
   #define BIT_DELAY 50  // Faster but less reliable
   ```

2. **Optimize Code:**
   - Reduce delays
   - Use direct port manipulation
   - Implement buffering

3. **Use Better Components:**
   - High-speed photodiode
   - Laser with faster modulation

## Common Use Cases

### Scenario 1: Weather Station Data

**Setup:**
- Transmitter at weather station (solar-powered)
- Receiver at monitoring location
- Sends temperature, humidity, pressure

**Code Modification:**
```cpp
// In Arduino transmitter
void loop() {
  float temp = readTemperature();
  float humidity = readHumidity();
  String data = String(temp) + "," + String(humidity);
  transmitMessage(data);
  delay(60000); // Send every minute
}
```

### Scenario 2: Security Alert System

**Setup:**
- Motion sensor triggers transmission
- Laser sends alert code
- Receiver sounds alarm

**Code Modification:**
```cpp
// In Arduino transmitter
#define MOTION_PIN 7
void loop() {
  if (digitalRead(MOTION_PIN) == HIGH) {
    transmitMessage("ALERT");
    delay(5000);
  }
}
```

### Scenario 3: Remote Control

**Setup:**
- Button presses send commands
- Receiver controls devices
- Secure, wireless operation

## Troubleshooting During Setup

### Problem: Arduino Not Detected

**Solutions:**
1. Install CH340 or FTDI drivers
2. Try different USB cable
3. Try different USB port
4. Check device manager (Windows)
5. Use `ls /dev/tty*` (Linux/macOS)

### Problem: ESP8266 Upload Fails

**Solutions:**
1. Hold FLASH button during upload
2. Reduce upload speed to 115200
3. Check USB cable (must be data cable)
4. Install USB drivers (CP2102/CH340)
5. Press RESET button after upload

### Problem: No Serial Output

**Solutions:**
1. Verify correct baud rate
2. Press RESET button on board
3. Check USB connection
4. Try different Serial Monitor (PuTTY, screen)
5. Verify code uploaded successfully

### Problem: Compilation Errors

**Solutions:**
1. Install correct board support
2. Select correct board type
3. Update Arduino IDE
4. Check for typos in code
5. Verify all libraries installed

## Next Steps

After successful setup:
1. ✓ Experiment with different messages
2. ✓ Test maximum range
3. ✓ Try different lighting conditions
4. ✓ Modify code for your use case
5. ✓ Add error correction
6. ✓ Implement bidirectional communication
7. ✓ Design custom enclosure
8. ✓ Share your results!

## Performance Benchmarks

### Typical Performance Metrics

**Default Configuration:**
- Bit Rate: ~10 Kbps
- Range: 2-5 meters
- Success Rate: >90%
- Power: <100mA total

**Optimized Configuration:**
- Bit Rate: ~20 Kbps
- Range: 1-3 meters
- Success Rate: >95%
- Power: <100mA total

**Long-Range Configuration:**
- Bit Rate: ~5 Kbps
- Range: 5-10 meters
- Success Rate: >85%
- Power: <100mA total

## Documentation and Logging

### Keeping Test Records

Create a test log:
```
Date: 2024-01-15
Test: Range measurement
Environment: Indoor, dim lighting
Distance: 200cm
Success Rate: 85%
Notes: Occasional errors at this distance
```

### Performance Tracking

Track improvements over time:
1. Record baseline performance
2. Make one change at a time
3. Measure new performance
4. Document what worked

---

**Happy Building!** 🚀

For questions or issues, refer to the [README.md](README.md) or open a GitHub issue.
