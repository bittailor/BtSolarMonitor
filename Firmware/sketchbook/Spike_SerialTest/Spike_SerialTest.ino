void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

}

void loop() {
  Serial.println("Serial hello");  
  Serial1.println("Serial1 hello");  
  delay(1000);
}
