#include <Bt_SolarMonitor_SlaveController.h>


Bt::SolarMonitor::SlaveController sSlaveController;

void setup() {
  Serial.begin(9600);
  Serial.println("**Solar Monitor IO Slave**");

  sSlaveController.begin();

}

void loop() {
   sSlaveController.loop();
}
