#include <Wire.h>
#include <LowPower.h>
#include <Bt_SolarMonitor.h>
#include <Bt/SolarMonitor/IOSlavePins.hpp>
#include <Bt/SolarMonitor/SlaveController.hpp>


Bt::SolarMonitor::SlaveController sSlaveController;

void intOnOff() {
   sSlaveController.getOnOffButton().handle(&Bt::Core::I_PushButton::State::change);
}

void intAB() {
   sSlaveController.getABButton().handle(&Bt::Core::I_PushButton::State::change);
}

void setup() {
  Wire.begin(0x8);
  Serial.begin(9600);
  Serial.println("**Solar Monitor IO Slave**");
  sSlaveController.begin();

  pinMode(BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_ON_OFF, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_ON_OFF), intOnOff, CHANGE);

  pinMode(BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_A_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_A_B), intAB, CHANGE);
}

void loop() {
   noInterrupts();
   bool needNextLoop = sSlaveController.loop();
   interrupts();
   if(!needNextLoop) {
      LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
   }
}
