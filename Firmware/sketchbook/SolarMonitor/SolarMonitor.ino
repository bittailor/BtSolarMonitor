#include <Wire.h>
#include <Bt_SolarMonitor.h>
#include <Bt/SolarMonitor/MainController.hpp>

Bt::SolarMonitor::MainController sMainController;

int led = 13;

void setup() {                
   Wire.begin();
   pinMode(led, OUTPUT);
   digitalWrite(led, HIGH);
   Serial.begin(115200);
   Serial.println("**Solar Monitor Main**");
   sMainController.begin();
   digitalWrite(led, LOW);
}

void loop() {
   sMainController.loop();
}
