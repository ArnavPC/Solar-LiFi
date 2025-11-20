/*
 * OPTICAL RX - PRESENTATION VERSION (AUTO-CALIBRATION)
 * Hardware: Arduino UNO + Solar Panel (A0) + SSD1306 OLED (128x32)
 * Wiring: 
 * - OLED SDA -> A4
 * - OLED SCL -> A5
 * - Solar Panel -> A0 (Divider: 100k/47k)
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- OLED CONFIGURATION ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32  
#define SCREEN_ADDRESS 0x3C 
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- OPTICAL PROTOCOL CONFIGURATION ---
const int SOLAR_PIN = A0;
const unsigned long BIT_TIME_US = 250000; // 250ms per bit
const unsigned long PREAMBLE_MIN_US = 800000; 

// VARIABLE THRESHOLD (Set by Calibration)
int threshold = 25; 

// Global Buffers
char rxBuffer[32]; 
int rxIndex = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("--- RECEIVER STARTING ---");

  // 1. Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  
  Wire.setClock(400000); // Fast I2C

  // 2. Run Auto-Calibration
  performCalibration();

  Serial.println("Ready. Waiting for Preamble...");
}

void loop() {
  // 1. Wait for PREAMBLE using calculated threshold
  if (detectPreamble()) {
    Serial.println("SYNC LOCKED");
    updateDisplay("RX LOCKED", "........");
    
    readPacket();
    
    updateDisplay("DONE:", "COMPLETE");
    delay(2000); 
  }
}

void performCalibration() {
  Serial.println("Starting Calibration...");
  
  // --- STEP 1: MEASURE AMBIENT ---
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("CALIBRATION MODE");
  display.setCursor(0,16);
  display.println("1. LASER OFF (2s)");
  display.display();
  delay(2000); // Give user time to read and block laser

  long ambientSum = 0;
  for(int i=0; i<50; i++) {
    ambientSum += analogRead(SOLAR_PIN);
    delay(10);
  }
  int ambient = ambientSum / 50;
  Serial.print("Ambient Level: "); Serial.println(ambient);

  // --- STEP 2: MEASURE LASER ---
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("CALIBRATION MODE");
  display.setCursor(0,16);
  display.println("2. SHINE LASER (4s)");
  display.display();
  
  int maxVal = ambient;
  unsigned long startParams = millis();
  
  // 4 Second window to catch the laser beam
  while(millis() - startParams < 4000) {
    int val = analogRead(SOLAR_PIN);
    if (val > maxVal) maxVal = val;
  }
  Serial.print("Max Laser Level: "); Serial.println(maxVal);

  // --- STEP 3: CALCULATE THRESHOLD ---
  // Safety: If laser wasn't detected (maxVal is close to ambient), add default buffer
  if (maxVal < ambient + 10) {
    threshold = ambient + 15; // Fallback safety
  } else {
    threshold = (ambient + maxVal) / 2;
  }

  Serial.print("Calculated Threshold: "); Serial.println(threshold);

  // Show Result on Screen
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Amb:"); display.print(ambient);
  display.print(" Las:"); display.println(maxVal);
  display.setCursor(0,16);
  display.print("Set Thresh: "); display.print(threshold);
  display.display();
  
  delay(3000); // Show result for 3 seconds before starting
}

// --- DISPLAY HELPER ---
void updateDisplay(const char* topStatus, const char* bitString) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (rxIndex > 0) {
    display.print("MSG: "); display.println(rxBuffer);
  } else {
    display.println(topStatus);
  }

  display.setCursor(0, 16); 
  display.setTextSize(2);   
  display.println(bitString);
  display.display();
}

bool detectPreamble() {
  // Use dynamic 'threshold' variable
  if (analogRead(SOLAR_PIN) > threshold) {
    unsigned long highStart = micros();
    
    while (analogRead(SOLAR_PIN) > (threshold - 5)) {
      if (micros() - highStart > 2000000) break; 
    }
    
    unsigned long duration = micros() - highStart;

    if (duration > PREAMBLE_MIN_US && duration < 1500000) {
      unsigned long lowStart = micros();
      while (analogRead(SOLAR_PIN) < threshold) {
         if (micros() - lowStart > 1500000) return false; 
      }
      return true; 
    }
  }
  return false;
}

void readPacket() {
  rxIndex = 0;
  memset(rxBuffer, 0, sizeof(rxBuffer)); 
  
  while (true) {
    char byteRead = readByteAndVisualize();
    if (byteRead == 0) break; 

    if (rxIndex < 30) {
      rxBuffer[rxIndex++] = byteRead;
      rxBuffer[rxIndex] = '\0';
    }
    Serial.print(byteRead); 
  }
  Serial.println();
}

char readByteAndVisualize() {
  unsigned long startWait = micros();
  while (analogRead(SOLAR_PIN) < threshold) {
    if (micros() - startWait > 1000000) return 0; 
  }
  
  unsigned long bitStart = micros();
  int value = 0;
  char bitVis[9] = "........"; 
  
  for (int i = 0; i < 8; i++) {
    unsigned long targetSampleTime = bitStart + ((i + 1) * BIT_TIME_US) + (BIT_TIME_US / 2);
    
    while (micros() < targetSampleTime);
    
    int samples = 0;
    if(analogRead(SOLAR_PIN) > threshold) samples++;
    if(analogRead(SOLAR_PIN) > threshold) samples++;
    if(analogRead(SOLAR_PIN) > threshold) samples++;
    
    int bitVal = (samples >= 2) ? 1 : 0;
    
    if (bitVal) {
      value |= (1 << i);
      bitVis[i] = '1';
    } else {
      bitVis[i] = '0';
    }

    updateDisplay("DECODING...", bitVis);
  }

  unsigned long stopBitEnd = bitStart + (10 * BIT_TIME_US);
  while (micros() < stopBitEnd);
  
  return (char)value;
}
