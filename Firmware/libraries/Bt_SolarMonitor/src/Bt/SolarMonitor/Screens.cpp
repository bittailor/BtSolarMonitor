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

//static unsigned char sGsmBits[] = {
//   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x0e, 0x00, 0x0b,
//   0x80, 0x09, 0xc0, 0x08, 0x60, 0x08, 0x30, 0x08, 0x18, 0x08, 0x0c, 0x08,
//   0x06, 0x08, 0xfe, 0x0f, 0x00, 0x00, 0x00, 0x00
//};
//
//static unsigned char sMobileBits[] = {
//   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x60, 0x00, 0x60,
//   0x00, 0x60, 0x00, 0x6c, 0x00, 0x6c, 0x00, 0x6c, 0x80, 0x6d, 0x80, 0x6d,
//   0xb0, 0x6d, 0xb0, 0x6d, 0x00, 0x00, 0x00, 0x00
//};
//
//static unsigned char sGsmAntennaBits[] = {
//   0x00, 0x00, 0x00, 0x10, 0x00, 0x24, 0x20, 0x29, 0x70, 0x4a, 0x70, 0x4a,
//   0x20, 0x29, 0x20, 0x24, 0x20, 0x10, 0xf8, 0x00, 0x88, 0x00, 0x88, 0x00,
//   0x88, 0x00, 0x88, 0x00, 0xf8, 0x00, 0x00, 0x00
//};

#define SMALL_ICON_SIZE 8

static unsigned char sRssiSmallBits[] = {
   0x40, 0x40, 0x40, 0x50, 0x50, 0x54, 0x55, 0x00
};

//static unsigned char sInfoSmallBits[] = {
//   0x1c, 0x22, 0x49, 0x41, 0x49, 0x49, 0x22, 0x1c
//};

static unsigned char sOnSmallBits[] = {
   0x00, 0x00, 0x40, 0x20, 0x10, 0x0a, 0x04, 0x00
};

static unsigned char sOffSmallBits[] = {
   0x00, 0x58, 0x24, 0x52, 0x4a, 0x24, 0x1a, 0x00
};

//static unsigned char sMqttSmallBits[] = {
//   0x0f, 0x10, 0x27, 0x48, 0x53, 0x54, 0x55, 0x00
//};

static unsigned char sMessageBits[] = {
   0x00, 0x7f, 0x63, 0x55, 0x49, 0x41, 0x7f, 0x00
};

static unsigned char sSocketSmallBits[] = {
   0x00, 0x07, 0x0f, 0x7f, 0x7f, 0x0f, 0x07, 0x00
};

static unsigned char sPlugSmallBits[] = {
   0x00, 0x18, 0x7c, 0x1f, 0x1f, 0x7c, 0x18, 0x00 };



//-------------------------------------------------------------------------------------------------

Screens::Screens(I_Screen& pOne, I_Screen& pTwo)
: mScreens{&pOne, &pTwo}
, mMeasurementRecord(){
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

void Screens::updateCounter(uint32_t pCounter) {
   mCounter = pCounter;
   updateScreens();
}

//-------------------------------------------------------------------------------------------------

void Screens::updateScreens() {

   mScreens[0]->clear();
   mScreens[0]->setTextColor(BLACK);
   drawPanelCurrent(0,mMeasurementRecord.panelA().current());
   drawBatteryVoltage(0,mMeasurementRecord.batteryA().voltage());
   if(mPowerState == PowerState::OnA) {
      drawPowerState(0);
   } else {
      drawConnectionState(0);
   }
   mScreens[0]->display();

   mScreens[1]->clear();
   mScreens[1]->setTextColor(BLACK);
   drawPanelCurrent(1,mMeasurementRecord.panelB().current());
   drawBatteryVoltage(1,mMeasurementRecord.batteryB().voltage());
   if(mPowerState == PowerState::OnB) {
      drawPowerState(1);
   } else if(mPowerState == PowerState::OnA) {
      drawConnectionState(1);
   }
   mScreens[1]->display();

}


//-------------------------------------------------------------------------------------------------

void Screens::drawPanelCurrent(size_t pScreenIndex, float pCurrent) {
   mScreens[pScreenIndex]->setTextSize(2);
   mScreens[pScreenIndex]->print(" ");
   mScreens[pScreenIndex]->drawXBitmap(0, 0*ICON_SIZE, sSunBits, ICON_SIZE, ICON_SIZE, 1);
   mScreens[pScreenIndex]->setTextSize(1);
   mScreens[pScreenIndex]->print(" ");
   drawMeasureValue(pCurrent, pScreenIndex);
   mScreens[pScreenIndex]->print(" ");
   mScreens[pScreenIndex]->setTextSize(2);
   mScreens[pScreenIndex]->println("A");
}


//-------------------------------------------------------------------------------------------------

void Screens::drawBatteryVoltage(size_t pScreenIndex, float pVoltage) {
   mScreens[pScreenIndex]->setTextSize(2);
   mScreens[pScreenIndex]->print(" ");
   mScreens[pScreenIndex]->drawXBitmap(0, 1*ICON_SIZE, sBatteryBits, ICON_SIZE, ICON_SIZE, 1);
   mScreens[pScreenIndex]->setTextSize(1);
   mScreens[pScreenIndex]->print(" ");
   drawMeasureValue(pVoltage, pScreenIndex);
   mScreens[pScreenIndex]->print(" ");
   mScreens[pScreenIndex]->setTextSize(2);
   mScreens[pScreenIndex]->println("V");
}

//-------------------------------------------------------------------------------------------------

void Screens::drawPowerState(size_t pScreenIndex) {
   mScreens[pScreenIndex]->setTextSize(2);
   mScreens[pScreenIndex]->print(" ");
   mScreens[pScreenIndex]->drawXBitmap(0, 2*ICON_SIZE, sLightBits, ICON_SIZE, ICON_SIZE, 1);
   mScreens[pScreenIndex]->setTextSize(1);
   mScreens[pScreenIndex]->print(" ");
   drawMeasureValue(mMeasurementRecord.load().current(), pScreenIndex);
   mScreens[pScreenIndex]->print(" ");
   mScreens[pScreenIndex]->setTextSize(2);
   mScreens[pScreenIndex]->println("A");
}

//-------------------------------------------------------------------------------------------------

void Screens::drawConnectionState(size_t pScreenIndex) {
   mScreens[pScreenIndex]->setTextSize(1);
   mScreens[pScreenIndex]->drawXBitmap(0, 2*ICON_SIZE+SMALL_ICON_SIZE, sRssiSmallBits, SMALL_ICON_SIZE, SMALL_ICON_SIZE, 1);
   mScreens[pScreenIndex]->setCursor(9,2*ICON_SIZE+SMALL_ICON_SIZE);
   mScreens[pScreenIndex]->print(mRSSI);
   mScreens[pScreenIndex]->print(" ");
   mGsmState = mGsmState % 6;
   int x = 25;
   if(mGsmState < 5) {
      mScreens[pScreenIndex]->drawXBitmap(x, 2*ICON_SIZE+SMALL_ICON_SIZE, sPlugSmallBits, SMALL_ICON_SIZE, SMALL_ICON_SIZE, 1);
      x+=8;
      mScreens[pScreenIndex]->drawXBitmap(x, 2*ICON_SIZE+SMALL_ICON_SIZE, sSocketSmallBits, SMALL_ICON_SIZE, SMALL_ICON_SIZE, 1);
      x+=8;
      for(int i = 0 ; i < mGsmState; i++) {
         mScreens[pScreenIndex]->drawXBitmap(x + (i*8), 2*ICON_SIZE+SMALL_ICON_SIZE, sOnSmallBits, SMALL_ICON_SIZE, SMALL_ICON_SIZE, 1);
      }
      for(int i = mGsmState ; i < 5; i++) {
         mScreens[pScreenIndex]->drawXBitmap(x + (i*8), 2*ICON_SIZE+SMALL_ICON_SIZE, sOffSmallBits, SMALL_ICON_SIZE, SMALL_ICON_SIZE, 1);
      }
   }
   else {
      mScreens[pScreenIndex]->drawXBitmap(x, 2*ICON_SIZE+SMALL_ICON_SIZE, sMessageBits, SMALL_ICON_SIZE, SMALL_ICON_SIZE, 1);
      x+=10;
      mScreens[pScreenIndex]->setCursor(x, 2*ICON_SIZE+SMALL_ICON_SIZE);
      mScreens[pScreenIndex]->print(mCounter%100000000);
   }
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
