#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Bt_SolarMonitor.h>

#include <Bt/SolarMonitor/Screens.hpp>
#include <Bt/SolarMonitor/NokiaScreen.hpp>

using namespace Bt::SolarMonitor;

NokiaScreen sDisplay1(A5, A4, A3);
NokiaScreen sDisplay2(A5, A2, A1);
Screens sScreens(sDisplay1,sDisplay2);
int sCounter = 1000;
#define DELAY 200

void setup()   {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  while (!Serial) {}
  Serial.begin(115200); 
  Serial.println(F("** Spike Screens **"));
  digitalWrite(13, LOW);

  sDisplay1.begin();
  sDisplay2.begin();
  
}


void loop() {
   Serial.println(F("** loop **"));
   sCounter++;
   double v = sCounter % 1600;
   v = (v / 100);
   double i = sCounter % 12000;
   i = i / 2000;
   int rssi = (sCounter % 33) -2;
   int gsmState = (sCounter % 12);
   gsmState = (gsmState > 5) ? 5 : gsmState;

   sScreens.updateMeasurementRecord(MeasurementRecord(
      Measurement(i,v),
      Measurement(i,v),
      Measurement(i,v),
      Measurement(i,v),
      Measurement(i,v),
      Measurement(i,v)
   ));

   sScreens.updateRSSI(rssi);
   sScreens.updateGsmState(gsmState);
   sScreens.updateCounter(sCounter-1000);

//   sScreens.updatePowerState(PowerState::OnA);
//   delay(DELAY);
//   sScreens.updatePowerState(PowerState::OnB);
//   delay(DELAY);
   sScreens.updatePowerState(PowerState::Off);
   delay(DELAY);

}
