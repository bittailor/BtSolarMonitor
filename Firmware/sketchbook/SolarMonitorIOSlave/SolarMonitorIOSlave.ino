#include <Bt_SolarMonitor_SlaveController.h>


#define PIN_BUTTON_ON_OFF   2
#define PIN_BUTTON_A_B      3

void setup() {
  Serial.begin(9600);
  Serial.println("**Solar Monitor IO Slave**");

  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_ON_OFF),onOnOffInterrupt,LOW);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_A_B),onABInterrupt,LOW);


}

void loop() {
}

void onOnOffInterrupt() {

}

void onABInterrupt() {

}
