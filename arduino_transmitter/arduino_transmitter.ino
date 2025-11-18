/**
 * Solar-LiFi Transmitter (Arduino)
 * 
 * This Arduino code transmits data using a laser diode for LiFi (Light Fidelity) communication.
 * The laser diode is modulated at high speed to encode digital data into light pulses.
 * 
 * Components:
 * - Arduino (Uno/Nano/Mega)
 * - Laser Diode Module (KY-008 or similar)
 * - Solar Panel (for power supply)
 * - Resistors for current limiting
 * 
 * Connections:
 * - Laser Diode Signal Pin -> Arduino Digital Pin 9
 * - Laser Diode VCC -> 5V (through current limiting resistor)
 * - Laser Diode GND -> GND
 * - Solar Panel (+) -> VIN (through voltage regulator if needed)
 * - Solar Panel (-) -> GND
 * 
 * Author: Solar-LiFi Project
 * License: MIT
 */

// Pin definitions
#define LASER_PIN 9        // Digital pin connected to laser diode
#define BAUD_RATE 9600     // Serial communication baud rate
#define BIT_DELAY 100      // Delay in microseconds for each bit transmission

// Transmission settings
#define START_MARKER 0xAA  // Start byte marker
#define END_MARKER 0x55    // End byte marker

void setup() {
  // Initialize serial communication for debugging and data input
  Serial.begin(BAUD_RATE);
  
  // Configure laser pin as output
  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, LOW);  // Ensure laser is off initially
  
  // Print startup message
  Serial.println("=================================");
  Serial.println("Solar-LiFi Transmitter Initialized");
  Serial.println("=================================");
  Serial.println("Send data via Serial to transmit");
  Serial.println("Data will be transmitted via laser diode");
  Serial.println();
}

void loop() {
  // Check if data is available on Serial
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    
    if (message.length() > 0) {
      Serial.print("Transmitting: ");
      Serial.println(message);
      
      // Transmit the message
      transmitMessage(message);
      
      Serial.println("Transmission complete");
      Serial.println();
    }
  }
}

/**
 * Transmit a complete message via LiFi
 * 
 * @param message The string message to transmit
 */
void transmitMessage(String message) {
  // Send start marker
  transmitByte(START_MARKER);
  
  // Send message length
  transmitByte((byte)message.length());
  
  // Send each character of the message
  for (int i = 0; i < message.length(); i++) {
    transmitByte((byte)message.charAt(i));
  }
  
  // Send end marker
  transmitByte(END_MARKER);
  
  // Add a small gap between transmissions
  delay(10);
}

/**
 * Transmit a single byte via LiFi
 * 
 * @param data The byte to transmit
 */
void transmitByte(byte data) {
  // Transmit start bit (LOW)
  digitalWrite(LASER_PIN, LOW);
  delayMicroseconds(BIT_DELAY);
  
  // Transmit 8 data bits (LSB first)
  for (int i = 0; i < 8; i++) {
    // Set laser based on bit value
    if (data & (1 << i)) {
      digitalWrite(LASER_PIN, HIGH);  // Bit 1 = Laser ON
    } else {
      digitalWrite(LASER_PIN, LOW);   // Bit 0 = Laser OFF
    }
    delayMicroseconds(BIT_DELAY);
  }
  
  // Transmit stop bit (HIGH)
  digitalWrite(LASER_PIN, HIGH);
  delayMicroseconds(BIT_DELAY);
  digitalWrite(LASER_PIN, LOW);
}

/**
 * Alternative function for simple ON/OFF laser control
 * Can be used for testing
 */
void testLaser() {
  // Turn laser ON for 1 second
  digitalWrite(LASER_PIN, HIGH);
  Serial.println("Laser ON");
  delay(1000);
  
  // Turn laser OFF for 1 second
  digitalWrite(LASER_PIN, LOW);
  Serial.println("Laser OFF");
  delay(1000);
}
