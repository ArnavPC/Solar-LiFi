/*
 * OPTICAL TX - SOLAR SAFE PROTOCOL
 * Hardware: ESP32
 * Pin: GPIO 4
 * Bit Rate: ~4 bits/sec (250ms pulses)
 */

const int LASER_PIN = 4;
const int BIT_TIME = 250; // ms - Ultra slow for solar panel
const char* message = "IMAGE#"; // Message to send (End with #)

void setup() {
  pinMode(LASER_PIN, OUTPUT);
  Serial.begin(115200);
  digitalWrite(LASER_PIN, LOW);
  delay(2000);
  Serial.println("--- SYSTEM READY ---");
  delay(1000);
}

void loop() {
  Serial.println("Sending Packet...");
  sendPacket(message);
  delay(3000); // Wait before repeating
}

void sendPacket(const char* str) {
  // 1. PREAMBLE: Wake up the RX
  // Pattern: HIGH (1000ms) -> LOW (500ms)
  digitalWrite(LASER_PIN, HIGH);
  delay(1000);
  digitalWrite(LASER_PIN, LOW);
  delay(500);

  // 2. DATA PAYLOAD
  int checksum = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    sendByte(str[i]);
    checksum += str[i];
  }
  
  // 3. CHECKSUM
  sendByte((char)(checksum % 256));
  
  // 4. TERMINATOR (Turn off)
  digitalWrite(LASER_PIN, LOW);
}

void sendByte(char c) {
  Serial.print("Sending: "); Serial.print(c); Serial.print(" [");
  
  // START BIT (HIGH) - Forces Rising Edge for Sync
  digitalWrite(LASER_PIN, HIGH);
  delay(BIT_TIME);
  Serial.print("S");

  // DATA BITS (8 bits, LSB first)
  for (int i = 0; i < 8; i++) {
    int bit = (c >> i) & 0x01;
    digitalWrite(LASER_PIN, bit ? HIGH : LOW);
    delay(BIT_TIME);
    Serial.print(bit);
  }

  // STOP BIT (LOW) - Forces line low to prepare for next Start Bit
  digitalWrite(LASER_PIN, LOW);
  delay(BIT_TIME); 
  Serial.println("]");
}
