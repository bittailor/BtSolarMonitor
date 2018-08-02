#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

void setup(void) 
{    
  Serial.begin(115200);
  ina219.begin();
}

void loop(void) 
{
  float current_mA = 0;
  current_mA = ina219.getCurrent_mA();
  Serial.println(current_mA);
}
