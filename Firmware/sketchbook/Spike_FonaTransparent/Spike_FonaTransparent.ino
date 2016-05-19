
void setup() {
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    while (!Serial) {}
    Serial.begin(115200);
    Serial.println(F("** FONA Transparent**"));
    Serial1.begin(115200);
    digitalWrite(13, LOW);
    delay(1000);
}

void loop() {
  if(Serial1.available()){
   Serial.write(Serial1.read());
  }
  //Read Arduino IDE Serial Monitor inputs (if available) and send them to SIM800
  if(Serial.available()){
   Serial1.write(Serial.read());
  }
}
