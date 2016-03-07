#include <Wire.h>

#define CMD_READ_REQUEST_COUNTER  0x01
#define CMD_DATA_LOOP_BACK  0x02

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  while (!Serial) {}
  Serial.begin(9600);
  Serial.println(F("** IO Master**"));
  digitalWrite(13, LOW);
  Wire.begin();
  delay(250);
}


void loop() {
  Serial.println(" Query REQUEST_COUNTER : ");
  Wire.beginTransmission(0x8);
  Wire.write(CMD_READ_REQUEST_COUNTER);
  Wire.endTransmission();
  delay(1);
  uint32_t requestCounter = 0;
  size_t requestSize = sizeof(requestCounter);
  size_t receivedSize = Wire.requestFrom(0x8, requestSize);
  if(requestSize != receivedSize) {
    Serial.print(" !!! Size missmatch  "); Serial.print(requestSize); Serial.print(" != "); Serial.println(receivedSize);
    return;
  }
  uint8_t* buffer =  ( uint8_t*)&requestCounter;
  for (int i = 0; i < receivedSize; i++) {
    buffer[i]=Wire.read();
  }
  Serial.print(" REQUEST_COUNTER =  "); Serial.println(requestCounter);
  delay(250);
}
