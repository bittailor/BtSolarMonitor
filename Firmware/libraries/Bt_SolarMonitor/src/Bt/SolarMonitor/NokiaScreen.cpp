//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::NokiaScreen
//
//*************************************************************************************************

#include "Bt/SolarMonitor/NokiaScreen.hpp"

namespace Bt {
namespace SolarMonitor {

//-------------------------------------------------------------------------------------------------

NokiaScreen::NokiaScreen(int8_t pDCPin, int8_t pCSPin, int8_t pRSTPin)
: mPCD8544(pDCPin, pCSPin, pRSTPin) {

}

//-------------------------------------------------------------------------------------------------

NokiaScreen::~NokiaScreen() {

}
//-------------------------------------------------------------------------------------------------

void NokiaScreen::begin() {
   mPCD8544.begin();
}

//-------------------------------------------------------------------------------------------------

void NokiaScreen::clear() {
   mPCD8544.clearDisplay();
}

//-------------------------------------------------------------------------------------------------

void NokiaScreen::display() {
   mPCD8544.display();
}

//-------------------------------------------------------------------------------------------------

void NokiaScreen::setTextSize(uint8_t pSize) {
   mPCD8544.setTextSize(pSize);
}

//-------------------------------------------------------------------------------------------------

void NokiaScreen::setTextColor(uint16_t pColor) {
   mPCD8544.setTextColor(pColor);
}

//-------------------------------------------------------------------------------------------------

void NokiaScreen::setCursor(int16_t pX, int16_t pY) {
   mPCD8544.setCursor(pX, pY);
}

//-------------------------------------------------------------------------------------------------

size_t NokiaScreen::print(const char* pString) {
   return mPCD8544.print(pString);
}

//-------------------------------------------------------------------------------------------------

size_t NokiaScreen::print(char pChar) {
   return mPCD8544.print(pChar);
}

//-------------------------------------------------------------------------------------------------

size_t NokiaScreen::print(int pInt) {
   return mPCD8544.print(pInt);
}

//-------------------------------------------------------------------------------------------------

size_t NokiaScreen::print(unsigned int pInt){
   return mPCD8544.print(pInt);
}

//-------------------------------------------------------------------------------------------------

size_t NokiaScreen::print(long pLong){
   return mPCD8544.print(pLong);
}

//-------------------------------------------------------------------------------------------------

size_t NokiaScreen::print(unsigned long pLong){
   return mPCD8544.print(pLong);
}

//-------------------------------------------------------------------------------------------------

size_t NokiaScreen::print(double pDouble, int pDigits) {
   return mPCD8544.print(pDouble, pDigits);
}

//-------------------------------------------------------------------------------------------------

size_t NokiaScreen::println(const char* pString) {
   return mPCD8544.println(pString);
}

//-------------------------------------------------------------------------------------------------

size_t NokiaScreen::println(char pChar) {
   return mPCD8544.println(pChar);
}

//-------------------------------------------------------------------------------------------------

size_t NokiaScreen::println(int pInt) {
   return mPCD8544.println(pInt);
}

//-------------------------------------------------------------------------------------------------

size_t NokiaScreen::println(unsigned int pInt){
   return mPCD8544.println(pInt);
}

//-------------------------------------------------------------------------------------------------

size_t NokiaScreen::println(long pLong){
   return mPCD8544.println(pLong);
}

//-------------------------------------------------------------------------------------------------

size_t NokiaScreen::println(unsigned long pLong){
   return mPCD8544.println(pLong);
}

//-------------------------------------------------------------------------------------------------

size_t NokiaScreen::println(double pDouble, int pDigits) {
   return mPCD8544.println(pDouble, pDigits);
}

//-------------------------------------------------------------------------------------------------

void NokiaScreen::drawXBitmap(int16_t pX, int16_t pY, const uint8_t *pBitmap, int16_t pW, int16_t pH, uint16_t color) {
   mPCD8544.drawXBitmap(pX, pY, pBitmap, pW, pH, color);
}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
