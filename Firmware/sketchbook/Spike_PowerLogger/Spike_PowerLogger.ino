#include <Adafruit_INA219.h>

#define TRIGGER_PIN 8

Adafruit_INA219 ina219;

void (*task)();

void setup() {
    Serial.begin(115200);
    ina219.begin();
    delay(1000);    
    Serial.println();
    task = &waitForStart;
    pinMode(TRIGGER_PIN, INPUT_PULLUP);
}

unsigned long counter = 0;

 

void loop() {
    task();
    delay(1);
}

void waitForStart(){
    if(!digitalRead(TRIGGER_PIN)){
        Serial.println("<begin>");
        Serial.println("V;I;P");
        delay(1000);
        task  = &measure;
        return;    
    }    
}

void measure(){
  if(digitalRead(TRIGGER_PIN)){
    Serial.println("<end>");
    delay(1000);
    task  = &waitForStart;
    return;
  }
  float v = ina219.getBusVoltage_V();
  float i = ina219.getCurrent_mA();
  float p = (v*i);
  Serial.print(v);
  Serial.print(";");
  Serial.print(i);
  Serial.print(";");
  Serial.println(p);
}