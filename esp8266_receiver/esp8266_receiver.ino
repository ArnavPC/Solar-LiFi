/**
 * Solar-LiFi Receiver (ESP8266)
 * 
 * This ESP8266 code receives data from a laser diode using a photodiode/LDR sensor
 * for LiFi (Light Fidelity) communication. The sensor detects light pulses and
 * decodes them back into digital data.
 * 
 * Components:
 * - ESP8266 (NodeMCU/Wemos D1 Mini)
 * - Photodiode or LDR (Light Dependent Resistor)
 * - Pull-down/Pull-up resistors
 * - Solar Panel (for power supply)
 * 
 * Connections:
 * - Photodiode/LDR Signal -> ESP8266 Analog Pin A0
 * - Photodiode/LDR VCC -> 3.3V
 * - Photodiode/LDR GND -> GND
 * - Solar Panel (+) -> VIN (through voltage regulator if needed)
 * - Solar Panel (-) -> GND
 * 
 * Author: Solar-LiFi Project
 * License: MIT
 */

#include <ESP8266WiFi.h>

// Pin definitions
#define SENSOR_PIN A0       // Analog pin connected to photodiode/LDR
#define LED_PIN LED_BUILTIN // Built-in LED for status indication
#define BAUD_RATE 115200    // Serial communication baud rate

// Reception settings
#define THRESHOLD 512       // Analog threshold value (0-1023) for detecting light
#define BIT_DELAY 100       // Delay in microseconds for each bit (must match transmitter)
#define START_MARKER 0xAA   // Start byte marker
#define END_MARKER 0x55     // End byte marker
#define MAX_MESSAGE_LEN 255 // Maximum message length

// Buffer for received data
byte receivedData[MAX_MESSAGE_LEN];
int dataIndex = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(BAUD_RATE);
  
  // Configure pins
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Disable WiFi to save power (optional)
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  
  // Print startup message
  Serial.println();
  Serial.println("=================================");
  Serial.println("Solar-LiFi Receiver Initialized");
  Serial.println("=================================");
  Serial.println("Waiting for LiFi transmission...");
  Serial.print("Threshold value: ");
  Serial.println(THRESHOLD);
  Serial.println();
  
  // Calibrate sensor
  calibrateSensor();
}

void loop() {
  // Continuously monitor for incoming transmissions
  if (detectStartMarker()) {
    // Start marker detected, receive the message
    digitalWrite(LED_PIN, HIGH);  // Indicate receiving
    
    // Read message length
    byte messageLength = receiveByte();
    
    if (messageLength > 0 && messageLength < MAX_MESSAGE_LEN) {
      // Receive message data
      for (int i = 0; i < messageLength; i++) {
        receivedData[i] = receiveByte();
      }
      
      // Receive end marker
      byte endMarker = receiveByte();
      
      // Verify end marker and print message
      if (endMarker == END_MARKER) {
        Serial.print("Received: ");
        for (int i = 0; i < messageLength; i++) {
          Serial.print((char)receivedData[i]);
        }
        Serial.println();
      } else {
        Serial.println("Error: Invalid end marker");
      }
    }
    
    digitalWrite(LED_PIN, LOW);  // Indicate done receiving
    delay(10);
  }
}

/**
 * Detect the start marker for incoming transmission
 * 
 * @return true if start marker detected, false otherwise
 */
bool detectStartMarker() {
  // Check if a transition is detected
  if (readBit()) {  // Wait for a HIGH bit
    byte potentialMarker = receiveByte();
    if (potentialMarker == START_MARKER) {
      return true;
    }
  }
  return false;
}

/**
 * Receive a single byte via LiFi
 * 
 * @return The received byte
 */
byte receiveByte() {
  byte data = 0;
  
  // Wait for start bit (LOW)
  while (readBit()) {
    delayMicroseconds(10);
  }
  delayMicroseconds(BIT_DELAY);
  
  // Receive 8 data bits (LSB first)
  for (int i = 0; i < 8; i++) {
    if (readBit()) {
      data |= (1 << i);  // Set bit if HIGH
    }
    delayMicroseconds(BIT_DELAY);
  }
  
  // Wait for stop bit (HIGH)
  delayMicroseconds(BIT_DELAY);
  
  return data;
}

/**
 * Read a single bit from the photodiode/LDR
 * 
 * @return true if light detected (HIGH), false otherwise (LOW)
 */
bool readBit() {
  int sensorValue = analogRead(SENSOR_PIN);
  return (sensorValue > THRESHOLD);
}

/**
 * Calibrate the sensor by reading ambient light level
 * This helps set an appropriate threshold value
 */
void calibrateSensor() {
  Serial.println("Calibrating sensor...");
  
  long sum = 0;
  int samples = 100;
  
  for (int i = 0; i < samples; i++) {
    sum += analogRead(SENSOR_PIN);
    delay(10);
  }
  
  int average = sum / samples;
  Serial.print("Ambient light level: ");
  Serial.println(average);
  Serial.print("Current threshold: ");
  Serial.println(THRESHOLD);
  
  if (average > THRESHOLD) {
    Serial.println("WARNING: Ambient light is above threshold!");
    Serial.println("Consider adjusting THRESHOLD value in code or reducing ambient light.");
  }
  
  Serial.println("Calibration complete");
  Serial.println();
}

/**
 * Get current sensor reading
 * Can be used for debugging
 * 
 * @return Current analog sensor value (0-1023)
 */
int getSensorReading() {
  return analogRead(SENSOR_PIN);
}

/**
 * Test function to continuously display sensor readings
 * Uncomment call in loop() for debugging
 */
void debugSensor() {
  int value = analogRead(SENSOR_PIN);
  Serial.print("Sensor: ");
  Serial.print(value);
  Serial.print(" | Threshold: ");
  Serial.print(THRESHOLD);
  Serial.print(" | Status: ");
  Serial.println(value > THRESHOLD ? "HIGH" : "LOW");
  delay(100);
}
