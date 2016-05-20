#define FONA_KEY 12
#define FONA_PS A0
#define FONA_RST 9
#define FONA_APN "gprs.swisscom.ch"

#include <BtMcuCore.h>
#include <BtMcuNet.h>

#include <Bt/Core/Time.hpp>
#include <Bt/Core/DigitalOut.hpp>
#include <Bt/Core/DigitalIn.hpp>
#include <Bt/Core/Workcycle.hpp>
#include <Bt/Net/Gprs/MobileTerminal.hpp>
#include <Bt/Net/Gprs/GprsModule.hpp>

using namespace Bt;


Core::Time sTime;
Core::DigitalOut sOnOffKey(FONA_KEY);
Core::DigitalOut sReset(FONA_RST);
Core::DigitalIn sPowerState(FONA_PS);
Net::Gprs::MobileTerminal sMobileTerminal(Serial1);

Net::Gprs::GprsModule sGprsModule(sTime,sOnOffKey,sReset,sPowerState,sMobileTerminal);

Core::Workcycle sWorkcycle;

void setup() {
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    while (!Serial) {}
    Serial.begin(115200);
    Serial.println(F("** Spike GPRS Module**"));
    Serial1.begin(115200);
    digitalWrite(13, LOW);
    delay(1000);


    sWorkcycle.add(sMobileTerminal);
    sWorkcycle.add(sGprsModule);

    sOnOffKey.begin();
    sReset.begin();
    sPowerState.begin();
    sGprsModule.begin();

}

void loop() {
   sWorkcycle.oneWorkcycle();
}
