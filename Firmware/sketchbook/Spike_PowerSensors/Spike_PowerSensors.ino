#include <Adafruit_INA219.h>

#define NUMBER_OF_SENSORS 6


Adafruit_INA219 sensors[NUMBER_OF_SENSORS];
uint8_t addresses[NUMBER_OF_SENSORS] = {
         0x40,
         0x44,
         0x41,
         0x45,
         0x4C,
         0x48
};


void setup() {
   pinMode(13, OUTPUT);
   digitalWrite(13, HIGH);
   while (!Serial) {}
   Serial.begin(115200);
   Serial.println("** Spike Power Sensors **");
   digitalWrite(13, LOW);

   delay(1000);
   for (size_t i=0 ; i < NUMBER_OF_SENSORS ; i++) {
      sensors[i].begin(addresses[i]);
      delay(200);
   }

   //sensors[0].setCalibration_load();
   //sensors[1].setCalibration_load();
   //sensors[2].setCalibration_load();
   //sensors[3].setCalibration_load();

   delay(1000);
}
void loop() {
   Serial.println("  -- loop --");
   for (size_t i=0 ; i < NUMBER_OF_SENSORS ; i++) {
      measure(sensors[i],i);
      delay(100);
   }
   delay(1000);
}

void measure(Adafruit_INA219& sensor, uint8_t id){
   Serial.print(id);
   Serial.print(": ");

   float v = sensor.getBusVoltage_V();
   //int16_t iRaw = sensor.getCurrent_raw();
   float i = sensor.getCurrent_mA();
   float vs = sensor.getShuntVoltage_mV();

   Serial.print(" u = ");
   Serial.print(v);
   Serial.print(" i = ");
   Serial.print(i);
   //Serial.print(" i(raw) = ");
   //Serial.print(iRaw);
   Serial.print(" u(s) = ");
   Serial.print(vs);
   Serial.println();
}
