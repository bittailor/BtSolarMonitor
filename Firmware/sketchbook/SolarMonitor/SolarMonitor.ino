#include <Wire.h>
#include <BtMcuCore.h>
#include <Bt_SolarMonitor.h>
#include <Bt/SolarMonitor/MainController.hpp>
#include <Bt/Core/StaticStringBuilder.hpp>

Bt::SolarMonitor::MainController sMainController;

int led = 13;
const size_t COMMAND_BUFFER_SIZE = 200;
char sCommandBuffer[COMMAND_BUFFER_SIZE] = {0};
bool sRunning = true;
Bt::Core::StaticStringBuilder sCommandBuilder(sCommandBuffer, COMMAND_BUFFER_SIZE);

void shutdown() {
   sRunning = false;
   Serial.println("start shutdown ...");
   sMainController.shutdown();
}

void executeCommand(const char* pCommand) {
   if(strcmp("s",pCommand)==0) {
      shutdown();
   }else if(strcmp("shutdown",pCommand)==0) {
      shutdown();
   } else {
      Serial.print("Unknown command: '");
      Serial.print(pCommand);
      Serial.println("'");
   }
}

void setup() {                
   Wire.begin();
   pinMode(led, OUTPUT);
   digitalWrite(led, HIGH);
   Serial.begin(115200);
   delay(1000);
   Serial.println("**Solar Monitor Main**");
   Serial.print("FONA_SERIAL.bufferCapacity() = ");Serial.println(Serial1.bufferCapacity());
   sMainController.begin();
   digitalWrite(led, LOW);
}

void loop() {
   if(sRunning) {
      sMainController.loop();
   }
   while(Serial.available()) {
      char c = Serial.read();
      if(c == '\n') {
         executeCommand(sCommandBuffer);
         sCommandBuilder.clear();
      } else {
         Serial.print(" ++ Append '");
         Serial.print(c);
         Serial.println("'");
         sCommandBuilder.print(c);
      }
   }
}
