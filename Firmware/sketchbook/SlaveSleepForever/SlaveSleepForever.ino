#include <LowPower.h>
 #include <Wire.h>
#include <BtMcuCore.h>
#include <Bt/Core/Logger.hpp>

void receiveEvent(int numBytes) {
}

void requestEvent() {
 }


void setup() {
  Serial.begin(9600);
  LOG("**Slave Sleep forever**");
  delay(10000);
  Wire.begin(0x8);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

void flush() {
   Serial.flush();
   while (!(UCSR0A & (1 << UDRE0))) {
      // Wait for empty transmit buffer
      UCSR0A |= 1 << TXC0; // mark transmission not complete
   }
   while (!(UCSR0A & (1 << TXC0)))
      ; // // Wait for the transmission to complete
}

void loop() {
  
  LOG(" => sleep now");
  flush();
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}
