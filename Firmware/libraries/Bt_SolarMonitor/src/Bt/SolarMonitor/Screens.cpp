//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Screens
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Screens.hpp"

#include <Arduino.h>
#include <math.h>

namespace Bt {
namespace SolarMonitor {

//-------------------------------------------------------------------------------------------------

#define ICON_SIZE 16

static const unsigned char sSunBits[] = {
   0x00, 0x00, 0x80, 0x00, 0x84, 0x10, 0x88, 0x08, 0x10, 0x04, 0x80, 0x00,
   0xc0, 0x01, 0xee, 0x3b, 0xc0, 0x01, 0x80, 0x00, 0x10, 0x04, 0x88, 0x08,
   0x84, 0x10, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char sBatteryBits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x18, 0x0c, 0x18, 0xfe, 0x3f,
   0xfe, 0x3f, 0x56, 0x35, 0x56, 0x35, 0x56, 0x35, 0xfe, 0x3f, 0xfe, 0x3f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char sLightBits[] = {
   0x00, 0x00, 0xc0, 0x03, 0x20, 0x04, 0x10, 0x08, 0x08, 0x10, 0x08, 0x10,
   0x08, 0x10, 0x10, 0x08, 0x20, 0x04, 0x40, 0x02, 0xc0, 0x03, 0xc0, 0x03,
   0xc0, 0x03, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00
};

static unsigned char sGsmBits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x0e, 0x00, 0x0b,
   0x80, 0x09, 0xc0, 0x08, 0x60, 0x08, 0x30, 0x08, 0x18, 0x08, 0x0c, 0x08,
   0x06, 0x08, 0xfe, 0x0f, 0x00, 0x00, 0x00, 0x00
};

static unsigned char sMobileBits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x60, 0x00, 0x60,
   0x00, 0x60, 0x00, 0x6c, 0x00, 0x6c, 0x00, 0x6c, 0x80, 0x6d, 0x80, 0x6d,
   0xb0, 0x6d, 0xb0, 0x6d, 0x00, 0x00, 0x00, 0x00
};

static unsigned char GsmAntennaBits[] = {
   0x00, 0x00, 0x00, 0x10, 0x00, 0x24, 0x20, 0x29, 0x70, 0x4a, 0x70, 0x4a,
   0x20, 0x29, 0x20, 0x24, 0x20, 0x10, 0xf8, 0x00, 0x88, 0x00, 0x88, 0x00,
   0x88, 0x00, 0x88, 0x00, 0xf8, 0x00, 0x00, 0x00
};

#define SMALL_ICON_SIZE 8

static unsigned char RssiSmallBits[] = {
   0x40, 0x40, 0x40, 0x50, 0x50, 0x54, 0x55, 0x00
};

static unsigned char InfoSmallBits[] = {
   0x1c, 0x22, 0x49, 0x41, 0x49, 0x49, 0x22, 0x1c
};

static unsigned char OnSmallBits[] = {
   0x00, 0x00, 0x40, 0x20, 0x10, 0x0a, 0x04, 0x00
};

static unsigned char OffSmallBits[] = {
   0x00, 0x58, 0x24, 0x52, 0x4a, 0x24, 0x1a, 0x00
};



//-------------------------------------------------------------------------------------------------

Screens::Screens(I_Screen& pOne, I_Screen& pTwo)
: mScreens{&pOne, &pTwo}
, mMeasurementRecord(Measurement(),Measurement(),Measurement(),Measurement(),Measurement(),Measurement()){
}

//-------------------------------------------------------------------------------------------------

Screens::~Screens() {

}

//-------------------------------------------------------------------------------------------------

void Screens::updateMeasurementRecord(const MeasurementRecord& pMeasurementRecord) {
   mMeasurementRecord = pMeasurementRecord;
   updateScreens();
}

//-------------------------------------------------------------------------------------------------

void Screens::updatePowerState(PowerState::State pPowerState) {
   mPowerState = pPowerState;
   updateScreens();
}

//-------------------------------------------------------------------------------------------------

void Screens::updateRSSI(int pRSSI) {
   mRSSI = pRSSI;
   updateScreens();
}

//-------------------------------------------------------------------------------------------------

void Screens::updateGsmState(int pGsmState) {
   mGsmState = pGsmState;
   updateScreens();
}

//-------------------------------------------------------------------------------------------------

void Screens::updateCounter(int pCounter) {
   mCounter = pCounter;
   updateScreens();
}


//-------------------------------------------------------------------------------------------------

/*
void Screens::update(const MeasurementRecord& pMeasurementRecord, PowerState::State pPowerState, bool pConnectionState) {
   mMeasurementRecord = pMeasurementRecord;
   mPowerState = pPowerState;
   updateScreens();

}
*/

//-------------------------------------------------------------------------------------------------

void Screens::updateScreens() {

   mScreens[0]->clear();

   //mScreens[0]->print("I");
   mScreens[0]->print(" ");
   mScreens[0]->drawXBitmap(0, 0*ICON_SIZE, sSunBits, ICON_SIZE, ICON_SIZE, 1);
   mScreens[0]->setTextSize(1);
   mScreens[0]->print(" ");
   drawMeasureValue(mMeasurementRecord.panelA().current(), 0);
   mScreens[0]->print(" ");
   mScreens[0]->setTextSize(2);
   mScreens[0]->println("A");
   mScreens[0]->display();


   mScreens[0]->print(" ");
   mScreens[0]->drawXBitmap(0, 1*ICON_SIZE, sBatteryBits, ICON_SIZE, ICON_SIZE, 1);
   //mScreens[0]->print("U");
   mScreens[0]->setTextSize(1);
   mScreens[0]->print(" ");
   drawMeasureValue(mMeasurementRecord.batteryA().voltage(), 0);
   mScreens[0]->print(" ");
   mScreens[0]->setTextSize(2);
   mScreens[0]->println("V");

   if(mPowerState == PowerState::OnA) {
      drawPowerState(0);
   } else {
      drawConnectionState(0);
   }
   mScreens[0]->display();


   mScreens[1]->clear();
   mScreens[1]->setTextColor(BLACK);
   mScreens[1]->print("I");
   mScreens[1]->setTextSize(1);
   mScreens[1]->print(" ");
   drawMeasureValue(mMeasurementRecord.panelB().current(), 1);
   mScreens[1]->print(" ");
   mScreens[1]->setTextSize(2);
   mScreens[1]->println("A");
   mScreens[1]->display();

   mScreens[1]->print("U");
   mScreens[1]->setTextSize(1);
   mScreens[1]->print(" ");
   drawMeasureValue(mMeasurementRecord.batteryB().voltage(), 1);
   mScreens[1]->print(" ");
   mScreens[1]->setTextSize(2);
   mScreens[1]->println("V");
   mScreens[1]->display();

   if(mPowerState == PowerState::OnB) {
      drawPowerState(1);
   }

   if(mPowerState == PowerState::OnA) {
      drawConnectionState(1);
   }

   mScreens[1]->display();

}

//-------------------------------------------------------------------------------------------------

void Screens::drawPowerState(size_t pScreenIndex) {
   mScreens[pScreenIndex]->print(" ");
   mScreens[pScreenIndex]->drawXBitmap(0, 2*ICON_SIZE, sLightBits, ICON_SIZE, ICON_SIZE, 1);
   mScreens[pScreenIndex]->setTextSize(1);
   mScreens[pScreenIndex]->print(" ");
   drawMeasureValue(mMeasurementRecord.load().current(), pScreenIndex);
   mScreens[pScreenIndex]->print(" ");
   mScreens[pScreenIndex]->setTextSize(2);
   mScreens[pScreenIndex]->println("A");
   mScreens[pScreenIndex]->display();
}

//-------------------------------------------------------------------------------------------------

void Screens::drawConnectionState(size_t pScreenIndex) {
   mScreens[pScreenIndex]->setTextSize(1);
   mScreens[pScreenIndex]->drawXBitmap(0, 2*ICON_SIZE+SMALL_ICON_SIZE, RssiSmallBits, SMALL_ICON_SIZE, SMALL_ICON_SIZE, 1);
   mScreens[pScreenIndex]->setCursor(10,2*ICON_SIZE+SMALL_ICON_SIZE);
   mScreens[pScreenIndex]->print(mRSSI);
   mScreens[pScreenIndex]->print(" ");
   mGsmState = mGsmState % 6;
   int x = 26;
   for(int i = 0 ; i < mGsmState; i++) {
      mScreens[pScreenIndex]->drawXBitmap(x + (i*8), 2*ICON_SIZE+SMALL_ICON_SIZE, OnSmallBits, SMALL_ICON_SIZE, SMALL_ICON_SIZE, 1);
   }
   for(int i = mGsmState ; i < 5; i++) {
      mScreens[pScreenIndex]->drawXBitmap(x + (i*8), 2*ICON_SIZE+SMALL_ICON_SIZE, OffSmallBits, SMALL_ICON_SIZE, SMALL_ICON_SIZE, 1);
   }
   mScreens[pScreenIndex]->setCursor(70, 2*ICON_SIZE+SMALL_ICON_SIZE);
   mScreens[pScreenIndex]->print(mCounter%100);

   mScreens[pScreenIndex]->setTextSize(2);
}

//-------------------------------------------------------------------------------------------------

void Screens::drawMeasureValue(double value, size_t pScreenIndex) {
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
