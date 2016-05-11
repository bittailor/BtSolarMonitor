//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Screens
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Screens.hpp"

#include <math.h>

namespace Bt {
namespace SolarMonitor {


//-------------------------------------------------------------------------------------------------

Screens::Screens(I_Screen& pOne, I_Screen& pTwo): mScreens{&pOne, &pTwo} {

}

//-------------------------------------------------------------------------------------------------

Screens::~Screens() {

}

//-------------------------------------------------------------------------------------------------

void Screens::update(const MeasurementRecord& pMeasurementRecord, PowerState::State pPowerState, bool pConnectionState) {

   mScreens[0]->clear();
   mScreens[0]->setTextColor(BLACK);
   mScreens[0]->print("I");
   mScreens[0]->setTextSize(1);
   mScreens[0]->print(" ");
   printMeasureValue(pMeasurementRecord.panelA().current(), 0);
   mScreens[0]->print(" ");
   mScreens[0]->setTextSize(2);
   mScreens[0]->println("A");
   mScreens[0]->display();

   mScreens[0]->print("U");
   mScreens[0]->setTextSize(1);
   mScreens[0]->print(" ");
   printMeasureValue(pMeasurementRecord.batteryA().voltage(), 0);
   mScreens[0]->print(" ");
   mScreens[0]->setTextSize(2);
   mScreens[0]->println("V");
   mScreens[0]->display();

   if(pPowerState == PowerState::OnA) {
      mScreens[0]->print("L");
      mScreens[0]->setTextSize(1);
      mScreens[0]->print(" ");
      printMeasureValue(pMeasurementRecord.load().current(), 0);
      mScreens[0]->print(" ");
      mScreens[0]->setTextSize(2);
      mScreens[0]->println("A");
      mScreens[0]->display();
   }

   if(pPowerState != PowerState::OnA) {
      mScreens[0]->setTextSize(1);
      mScreens[0]->println("");
      mScreens[0]->println(pConnectionState? "Connected" : "Disconnected");
      mScreens[0]->display();
      mScreens[0]->setTextSize(2);
   }

   mScreens[1]->clear();
   mScreens[1]->setTextColor(BLACK);
   mScreens[1]->print("I");
   mScreens[1]->setTextSize(1);
   mScreens[1]->print(" ");
   printMeasureValue(pMeasurementRecord.panelB().current(), 1);
   mScreens[1]->print(" ");
   mScreens[1]->setTextSize(2);
   mScreens[1]->println("A");
   mScreens[1]->display();

   mScreens[1]->print("U");
   mScreens[1]->setTextSize(1);
   mScreens[1]->print(" ");
   printMeasureValue(pMeasurementRecord.batteryB().voltage(), 1);
   mScreens[1]->print(" ");
   mScreens[1]->setTextSize(2);
   mScreens[1]->println("V");
   mScreens[1]->display();

   if(pPowerState == PowerState::OnB) {
      mScreens[1]->print("L");
      mScreens[1]->setTextSize(1);
      mScreens[1]->print(" ");
      printMeasureValue(pMeasurementRecord.load().current(), 1);
      mScreens[1]->print(" ");
      mScreens[1]->setTextSize(2);
      mScreens[1]->println("A");
      mScreens[1]->display();
   }

   if(pPowerState == PowerState::OnA) {
        mScreens[1]->setTextSize(1);
        mScreens[1]->println("");
        mScreens[1]->println(pConnectionState? "Connected" : "Disconnected");
        mScreens[1]->display();
        mScreens[0]->setTextSize(2);
     }

}

//-------------------------------------------------------------------------------------------------

void Screens::printMeasureValue(double value, size_t pScreenIndex) {
   if(value < 0) {
      value = 0.0;
   }
   double integral;
   double fractional = modf(value,&integral) * 1000;

   static const size_t BUFFER_SIZE = 10;
   char buffer[BUFFER_SIZE] = {0};

   snprintf(buffer, BUFFER_SIZE, "%2d", ((int)integral));
   mScreens[pScreenIndex]->setTextSize(2);
   mScreens[pScreenIndex]->print(buffer);
   snprintf(buffer, BUFFER_SIZE, "%03d", ((int)fractional));
   mScreens[pScreenIndex]->setTextSize(1);
   mScreens[pScreenIndex]->print(buffer);
}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
