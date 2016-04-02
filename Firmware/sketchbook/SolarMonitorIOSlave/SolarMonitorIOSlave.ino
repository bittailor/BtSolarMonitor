#include <Bt_SolarMonitor_SlaveController.h>
#include <Bt/SolarMonitor/IOSlavePins.hpp>


Bt::SolarMonitor::SlaveController sSlaveController;

void toggleOnOff() {
   sSlaveController.toggleOnOff();
}

void toggleAB() {
   sSlaveController.toggleAB();
}

void setup() {
  Serial.begin(9600);
  Serial.println("**Solar Monitor IO Slave**");
  sSlaveController.begin();

  pinMode(BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_ON_OFF, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_ON_OFF), toggleOnOff, FALLING);

  pinMode(BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_A_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_A_B), toggleAB, FALLING);


}

void loop() {
   sSlaveController.loop();
}


