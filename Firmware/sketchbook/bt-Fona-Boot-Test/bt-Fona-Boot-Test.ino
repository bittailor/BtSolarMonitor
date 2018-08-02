
#include "Adafruit_FONA.h"
#include <BtMcuCore.h>
#include <Bt/Core/StaticStringBuilder.hpp>
#define FONA_RST 9
#define LOOP_DELAY 5000
HardwareSerial* sFonaSerial = &Serial1;
Adafruit_FONA sFona = Adafruit_FONA(FONA_RST);
unsigned long sSucess = 0;
unsigned long sFailed = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("BT FONA BOOT Test"));
  sFonaSerial->begin(115200);
}

void loop() {
  Serial.print(F("Sucess="));
  Serial.print(sSucess);
  Serial.print(F(" Failed="));
  Serial.println(sFailed);

  
  if(sFona.begin(*sFonaSerial)) {
    Serial.println(F("sFona.begin OK"));

    char imei[15] = {0}; // MUST use a 16 character buffer for IMEI!
    uint8_t imeiLen = sFona.getIMEI(imei);
    if (imeiLen > 0) {
      Serial.print("Module IMEI: "); Serial.println(imei);
    }

    sFona.setGPRSNetworkSettings(F("gprs.swisscom.ch"), F(""), F(""));
    ReadDelay(6000);
    uint8_t n = sFona.getNetworkStatus();
    Serial.print(F("fona.getNetworkStatus "));
    Serial.println(n);
    if(n == 1) {
      if(sFona.enableGPRS(true)) {
        Serial.println(F("fona.enableGPRS(true) OK"));
        Serial.print(F("Get ..."));
        uint16_t statuscode;
        int16_t length;
        if (sFona.HTTP_GET_start("broker.shiftr.io/sim/2/balance", &statuscode, (uint16_t *)&length)) {
          readAll(length);
          sFona.HTTP_GET_end();
          Serial.println(F("\n HTTP GET OK"));
          {
            char msg[80];
            Bt::Core::StaticStringBuilder builder(msg,80);
            builder.print(F("Sucess="));
            builder.print(sSucess);
            builder.print(F(" Failed="));
            builder.print(sFailed);      
            if(sFona.HTTP_POST_start("broker.shiftr.io/test/2/info", F("text/plain"), (uint8_t *) msg, strlen(msg), &statuscode, (uint16_t *)&length)) {
              readAll(length);
              sFona.HTTP_POST_end();
              Serial.println(F("\n HTTP POST OK"));
              sSucess++;
              delay(LOOP_DELAY);
              return;
            }      
          }
        }                  
      }
    }
  }
  sFailed++;
  Serial.println(F("failed"));
  delay(LOOP_DELAY);
}


void readAll(int16_t length) {
  while (length > 0) {
    while (sFona.available()) {
      char c = sFona.read();
      Serial.write(c);
      length--;
      if (! length) break;
    }
  }
}

void ReadDelay(long delay) {
  long end =  millis() + delay;
  while(millis() < end) {
    if(sFona.available()) {
      while (sFona.available()) {
        Serial.write(sFona.read());
      }  
    }    
  }
}

