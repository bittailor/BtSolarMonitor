#include <Arduino.h>
#include <Wire.h>

#define Sense_L A0
#define Sense_1 A1
#define Sense_2 A2

#define RELAY_1_OFF  6
#define RELAY_1_ON   5
#define RELAY_2_OFF  7
#define RELAY_2_ON   8
#define RELAY_3_OFF 10
#define RELAY_3_ON   9

#define LED_A    A6
#define LED_B    A7


void setup() {
   Serial.begin(9600);
   Serial.println("** Spike Relays **");
   Wire.begin(0x8);

   checkPin(RELAY_1_OFF);
   checkPin(RELAY_2_OFF);
   checkPin(RELAY_3_OFF);
   checkPin(A0);
   checkPin(A1);
   checkPin(LED_A);
   checkPin(LED_B);


   pinMode(Sense_L, INPUT_PULLUP);
   pinMode(Sense_1, INPUT_PULLUP);
   pinMode(Sense_2, INPUT_PULLUP);

   pinMode(RELAY_1_OFF, OUTPUT);
   pinMode(RELAY_1_ON, OUTPUT);
   pinMode(RELAY_2_OFF, OUTPUT);
   pinMode(RELAY_2_ON, OUTPUT);
   pinMode(RELAY_3_OFF, OUTPUT);
   pinMode(RELAY_3_ON, OUTPUT);

   pinMode(LED_A, OUTPUT);
   pinMode(LED_B, OUTPUT);

   digitalWrite(RELAY_1_OFF, LOW);
   digitalWrite(RELAY_1_ON, LOW);
   digitalWrite(RELAY_2_OFF, LOW);
   digitalWrite(RELAY_2_ON, LOW);
   digitalWrite(RELAY_3_OFF, LOW);
   digitalWrite(RELAY_3_ON, LOW);
}

/*
void loop() {
   digitalWrite(RELAY_3_OFF, HIGH);
   
   delay(50);
   digitalWrite(RELAY_3_OFF, LOW);
   delay(200);
}
*/


void loop() {
   if (Serial.available() > 0) {
      char command = Serial.read();
      Serial.print("command>");
      Serial.println(command);
      switch(command) {
         case '0' : toggle(RELAY_1_OFF); break;
         case '1' : toggle(RELAY_1_ON); break;
         case '2' : toggle(RELAY_2_OFF); break;
         case '3' : toggle(RELAY_2_ON); break;
         case '4' : toggle(RELAY_3_OFF); break;
         case '5' : toggle(RELAY_3_ON); break;

         case '6' : digitalWrite(LED_A, LOW); break;
         case '7' : digitalWrite(LED_A, HIGH); break;
         case '8' : digitalWrite(LED_B, LOW); break;
         case '9' : digitalWrite(LED_A, HIGH); break;

         case '?' : status(); break;
         default: Serial.println("unknown commad");
      }
   }
}


void toggle(uint8_t pin) {
   digitalWrite(pin, HIGH);
   delay(50);
   digitalWrite(pin, LOW);
}

void status() {
   Serial.print("Sense_L = ");
   Serial.println(digitalRead(Sense_L));
   Serial.print("Sense_1 = ");
   Serial.println(digitalRead(Sense_1));
   Serial.print("Sense_2 = ");
   Serial.println(digitalRead(Sense_2));
}


void checkPin(uint8_t pin) {

   uint8_t bit = digitalPinToBitMask(pin);
   uint8_t port = digitalPinToPort(pin);
   volatile uint8_t *reg, *out;

   Serial.print("Port of ");Serial.print(pin);Serial.print("is ");Serial.println(port);

   if (port == NOT_A_PIN) {
      Serial.println("Oops NOT_A_PIN");
      return;
   }

   Serial.println("OK");
}
