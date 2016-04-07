#include <Bt_SolarMonitor_SlaveController.h>
#include <Bt/SolarMonitor/IOSlavePins.hpp>

Bt::SolarMonitor::SlaveController sSlaveController;

void intOnOff() {
   sSlaveController.getOnOffButton().handle(&Bt::Core::I_PushButton::State::change);
}

void intAB() {
   sSlaveController.getABButton().handle(&Bt::Core::I_PushButton::State::change);
}


void setup() {
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
   sSlaveController.loop();
   interrupts();
}


