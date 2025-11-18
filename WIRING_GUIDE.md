# Wiring Guide - Solar-LiFi Communication System

This document provides detailed wiring instructions for assembling the Solar-LiFi communication system.

## Table of Contents
- [Tools Required](#tools-required)
- [Safety Precautions](#safety-precautions)
- [Transmitter Wiring](#transmitter-wiring)
- [Receiver Wiring](#receiver-wiring)
- [Solar Panel Integration](#solar-panel-integration)
- [Testing Connections](#testing-connections)

## Tools Required

- Soldering iron and solder
- Wire strippers
- Multimeter
- Screwdriver set
- Breadboard or perfboard
- Heat shrink tubing or electrical tape
- Helping hands or PCB holder

## Safety Precautions

⚠️ **Before Starting:**
1. Disconnect all power sources
2. Double-check component polarity
3. Use appropriate voltage regulators
4. Never look directly at laser diode
5. Work in a well-lit, static-free area
6. Keep laser power < 1mW (Class 1/2)

## Transmitter Wiring (Arduino)

### Step 1: Prepare the Arduino

1. Place Arduino on breadboard or secure it to work surface
2. Identify all required pins:
   - Digital Pin 9 (Laser control)
   - 5V power pin
   - GND pin
   - VIN pin (for solar input)

### Step 2: Connect Laser Diode Module

**KY-008 Laser Module:**
```
Laser Module Pin    →   Arduino Pin
-----------------       ------------
Signal (S)         →   Digital Pin 9
Power (+/VCC)      →   5V [through 100Ω resistor]
Ground (-/GND)     →   GND
```

**Wiring Steps:**
1. **Signal Connection:**
   - Connect laser module's signal pin to Arduino Digital Pin 9
   - Use a red or yellow wire for easy identification

2. **Power Connection:**
   - Solder a 100Ω resistor to the laser module's VCC pin
   - Connect the other end of resistor to Arduino's 5V pin
   - This resistor limits current and protects the laser

3. **Ground Connection:**
   - Connect laser module's GND pin to Arduino's GND pin
   - Use a black wire

### Step 3: Connect Solar Panel

**Solar Panel Connections:**
```
Solar Panel    →   Component         →   Arduino
-----------         ---------             --------
Positive (+)   →   Voltage Regulator →   VIN
                   Input
Negative (-)   →   Common Ground     →   GND
```

**Detailed Steps:**

1. **Voltage Regulator (if solar panel > 12V):**
   - Use 7805 or LM7805 voltage regulator
   - Solar (+) → Regulator Input
   - Solar (-) → Regulator Ground
   - Regulator Output → Arduino VIN
   - Add 100µF capacitor between VIN and GND for smoothing

2. **Direct Connection (if solar panel 6-12V):**
   - Solar (+) → Arduino VIN (with reverse polarity protection diode)
   - Solar (-) → Arduino GND

3. **Add Protection Diode:**
   - Place 1N4007 diode in series with solar (+) wire
   - Diode cathode (stripe) towards Arduino
   - Prevents reverse current flow

### Step 4: Power Smoothing

Add capacitors for stable operation:
- 100µF electrolytic capacitor between VIN and GND
- 100nF ceramic capacitor between 5V and GND (near laser)

### Complete Transmitter Schematic

```
                    100Ω
Solar (+6V) ─┬─[1N4007]─┬─── VIN (Arduino)
             │           │
          [100µF]        │
             │           │
Solar (-)  ──┴───────────┴─── GND (Arduino)
                               │
                               ├─── GND (Laser)
                               │
                         Pin 9 ──── Signal (Laser)
                               │
                          5V ──┬──[100Ω]─── VCC (Laser)
                               │
                            [100nF]
                               │
                              GND
```

## Receiver Wiring (ESP8266)

### Step 1: Prepare the ESP8266

1. Place ESP8266 NodeMCU on breadboard
2. Identify all required pins:
   - A0 (Analog input for photodiode)
   - 3.3V power pin
   - GND pin
   - VIN pin (for solar input)

### Step 2: Connect Photodiode/LDR

**Photodiode Connection (Recommended):**
```
Photodiode Pin    →   Component/Pin    →   ESP8266 Pin
--------------         --------------         -----------
Anode (+)         →   3.3V            →   3.3V
Cathode (-)       →   A0 + [10kΩ to GND] → A0
```

**Detailed Steps:**

1. **Photodiode Setup:**
   - Connect photodiode anode (+, longer leg) to 3.3V
   - Connect photodiode cathode (-, shorter leg) to:
     - ESP8266 pin A0
     - One end of 10kΩ resistor
   - Connect other end of 10kΩ resistor to GND
   - This creates a voltage divider

2. **LDR Alternative:**
   - If using LDR instead of photodiode:
   - One leg → 3.3V
   - Other leg → A0 and 10kΩ resistor to GND

### Step 3: Connect Solar Panel

**Solar Panel Connections:**
```
Solar Panel    →   Component         →   ESP8266
-----------         ---------             --------
Positive (+)   →   Voltage Regulator →   VIN
                   (if needed)
Negative (-)   →   Common Ground     →   GND
```

**Steps:**

1. **With Voltage Regulator (if solar > 12V):**
   - Use LM7805 or similar
   - Solar (+) → Regulator Input
   - Solar (-) → Regulator Ground
   - Regulator Output → ESP8266 VIN

2. **Direct Connection (6-12V solar panel):**
   - Solar (+) → ESP8266 VIN (with diode protection)
   - Solar (-) → ESP8266 GND
   - Add 1N4007 diode for reverse polarity protection

### Step 4: Power Smoothing

Add capacitors:
- 100µF electrolytic between VIN and GND
- 100nF ceramic between 3.3V and GND (near photodiode)

### Complete Receiver Schematic

```
Solar (+6V) ─┬─[1N4007]─┬─── VIN (ESP8266)
             │           │
          [100µF]        │
             │           │
Solar (-)  ──┴───────────┴─── GND (ESP8266)
                               │
                               ├─── GND (via 10kΩ from photodiode)
                               │
                          A0 ──┴─── Cathode (Photodiode)
                               
                        3.3V ─────── Anode (Photodiode)
                               │
                            [100nF]
                               │
                              GND
```

## Solar Panel Integration

### Voltage Regulator Circuit

For solar panels > 12V, use this regulator circuit:

```
Solar (+) ──[1N4007]─┬─[Cap 1µF]─┬─ IN  [7805]  OUT ─┬─[Cap 100nF]─┬─→ VIN
                     │            │                   │              │
                     │           GND                 GND             │
                     │                                               │
Solar (-) ──────────┴───────────────────────────────────────────────┴─→ GND
```

### Battery Backup (Optional)

To ensure continuous operation:

```
Solar Panel (+) ──[Diode]──┬─→ Charge Controller → Battery (+) ─→ VIN
                           │
                      [Capacitor]
                           │
Solar Panel (-) ──────────┴────────────────────── Battery (-) ─→ GND
```

Components needed:
- 3.7V LiPo battery (1000mAh+)
- TP4056 charging module
- Schottky diode (1N5819)

## Testing Connections

### Step 1: Visual Inspection

Before applying power:
1. ✓ Check all wire connections are secure
2. ✓ Verify no short circuits
3. ✓ Confirm correct polarity everywhere
4. ✓ Check component orientations (diodes, capacitors)

### Step 2: Continuity Testing

Use multimeter in continuity mode:
1. Test VIN to GND (should be open circuit)
2. Test 5V to GND on Arduino (should be open circuit)
3. Test 3.3V to GND on ESP8266 (should be open circuit)
4. Test all GND connections are common

### Step 3: Voltage Testing

Before connecting solar panel:
1. Measure solar panel voltage (should be 6-12V in sunlight)
2. If using voltage regulator, verify output voltage
3. Connect USB cable and verify:
   - Arduino: 5V between 5V pin and GND
   - ESP8266: 3.3V between 3.3V pin and GND

### Step 4: Component Testing

**Test Laser Diode:**
1. Upload transmitter code
2. Open Serial Monitor (9600 baud)
3. Send test message
4. Verify laser emits visible red light
5. **NEVER look directly at laser**

**Test Photodiode:**
1. Upload receiver code
2. Open Serial Monitor (115200 baud)
3. Check calibration values
4. Shine flashlight on photodiode
5. Verify reading changes on Serial Monitor

### Step 5: Alignment Test

1. Power both devices
2. Position laser to point at photodiode
3. Start transmitter, send test message
4. Verify receiver displays message
5. Adjust alignment for best reception

## Troubleshooting Wiring Issues

### Issue: Device won't power on
- Check solar panel polarity
- Verify voltage regulator if used
- Test solar panel voltage with multimeter
- Check for loose connections

### Issue: Laser doesn't light up
- Verify pin 9 connection
- Check 100Ω resistor value
- Test laser module separately
- Verify code uploaded successfully

### Issue: No signal detected on receiver
- Check photodiode polarity
- Verify 10kΩ pull-down resistor
- Test photodiode with flashlight
- Adjust alignment between laser and photodiode

### Issue: Intermittent operation
- Add larger capacitors for power smoothing
- Check for loose wire connections
- Verify solar panel provides stable voltage
- Shield from electromagnetic interference

## Best Practices

1. **Wire Management:**
   - Use color coding (red=power, black=ground, other=signal)
   - Keep wires short to reduce interference
   - Bundle wires with cable ties
   - Label all connections

2. **Soldering Tips:**
   - Use 60/40 rosin-core solder
   - Heat pad and component, not solder
   - Apply heat for 2-3 seconds
   - Create shiny, cone-shaped joints
   - Let joints cool naturally

3. **Mechanical Stability:**
   - Secure all components to prevent movement
   - Use hot glue for strain relief on wires
   - Mount on stable surface or enclosure
   - Protect from physical damage

4. **Environmental Protection:**
   - Use weatherproof enclosure if outdoors
   - Add desiccant packs to prevent moisture
   - Seal cable entry points
   - Protect solar panel from hail/debris

## Final Assembly Checklist

- [ ] All components properly connected
- [ ] Polarity verified on all connections
- [ ] Protection diodes installed
- [ ] Smoothing capacitors added
- [ ] No short circuits present
- [ ] Laser properly current-limited
- [ ] Photodiode circuit tested
- [ ] Solar panel connection secure
- [ ] Code uploaded to both devices
- [ ] Serial monitors tested
- [ ] Laser-photodiode aligned
- [ ] Test message transmitted successfully

---

**Safety First!** Always double-check connections before applying power.

**Need Help?** Refer to the main README.md or open an issue on GitHub.
