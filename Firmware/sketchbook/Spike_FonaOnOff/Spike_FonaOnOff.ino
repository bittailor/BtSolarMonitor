#define RESET 9

#define ON_OFF_PIN 12
#define PS_STATUS A0

void setup() {
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    while (!Serial) {}
    Serial.begin(115200); 
    Serial.println(F("** FONA ON OFF **"));
    digitalWrite(13, LOW); 

    pinMode(RESET, OUTPUT);
    pinMode(ON_OFF_PIN, OUTPUT);
    pinMode(PS_STATUS, INPUT);
    
    digitalWrite(ON_OFF_PIN, HIGH);

    Serial.println(F("RESET HIGH"));
    digitalWrite(RESET, HIGH);
    delay(10000);
    Serial.println(F("RESET LOW"));
    digitalWrite(RESET, LOW);
    delay(10000);
    Serial.println(F("RESET HIGH"));
    digitalWrite(RESET, HIGH);



    Serial.print("Power Status :");
    Serial.println(digitalRead(PS_STATUS));
  
}

void loop() {
    if (Serial.available() > 0) {
        char command = Serial.read();
        Serial.print("command>"); 
        Serial.println(command);
        switch(command) {
            case '1' : on(); break;
            case '0' : off(); break;
            case '?' : status(); break;
            default: Serial.println("unknown commad"); 
        }
    }
}
    
void on() {
    if(!status()){
        toggle();    
    }
}

void off() {
    if(status()){
        toggle();    
    }    
}

void toggle() {
    Serial.println("Toggle ...");
    digitalWrite(ON_OFF_PIN, LOW);
    delay(3000);
    digitalWrite(ON_OFF_PIN, HIGH);
    Serial.println("... done");
        
}

bool status() {
    bool status = digitalRead(PS_STATUS);
    Serial.print("Power Status :"); 
    Serial.println(status);
    return status;     
}






