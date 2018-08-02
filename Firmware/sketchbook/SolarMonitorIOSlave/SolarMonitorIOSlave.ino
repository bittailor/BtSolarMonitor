 #include <Wire.h>
#include <LowPower.h>
#include <BtMcuCore.h>
#include <Bt_SolarMonitor.h>

#include <Bt/Core/Logger.hpp>
#include <Bt/SolarMonitor/IoSlavePins.hpp>
#include <Bt/SolarMonitor/SlaveController.hpp>


Bt::SolarMonitor::SlaveController sSlaveController;

void intOnOff() {
   sSlaveController.getOnOffButton().change();
}

void intAB() {
   sSlaveController.getABButton().change();
}

void receiveEvent(int numBytes) {
   sSlaveController.getWireSlave().receiveEvent(numBytes);
}

void requestEvent() {
   sSlaveController.getWireSlave().requestEvent();
}

void setup() {
  Wire.begin(0x8);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  LOG("**Solar Monitor IO Slave**");
  sSlaveController.begin();

  pinMode(BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_ON_OFF, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_ON_OFF), intOnOff, CHANGE);

  pinMode(BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_A_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_A_B), intAB, CHANGE);
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
   static uint32_t sStartTime = millis();
   static uint32_t sInterval = 10;
   noInterrupts();
   uint32_t nextLoopDelay = sSlaveController.oneWorkcycle();
   bool needNextLoop = nextLoopDelay != Bt::Core::I_Runnable::FOREVER;
   interrupts();
   if(!needNextLoop) {
      if (millis() - sStartTime >= sInterval) {
         LOG("->s");
         flush();
         LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
         LOG("->a");
         sStartTime = millis();
      }
   }
}
