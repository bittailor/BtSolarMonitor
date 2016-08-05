//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::NokiaScreen
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_NokiaScreen__hpp
#define INC__Bt_SolarMonitor_NokiaScreen__hpp

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "Bt/SolarMonitor/I_Screen.hpp"

namespace Bt {
namespace SolarMonitor {

class NokiaScreen : public I_Screen
{
   public:
      NokiaScreen(int8_t pDCPin, int8_t pCSPin, int8_t pRSTPin);
      ~NokiaScreen();

      void begin();

      virtual void clear();
      virtual void display();

      virtual void setTextSize(uint8_t pSize);
      virtual void setTextColor(uint16_t pColor);
      virtual void setCursor(int16_t pX, int16_t pY);

      virtual size_t print(const char* pString);
      virtual size_t print(char pChar);
      virtual size_t print(int pInt);
      virtual size_t print(double pDouble, int pDigits= 2);

      virtual size_t println(const char* pString);
      virtual size_t println(char pChar);
      virtual size_t println(int pInt);
      virtual size_t println(double pDouble, int pDigits= 2);

      virtual void drawXBitmap(int16_t pX, int16_t pY, const uint8_t *pBitmap, int16_t pW, int16_t pH, uint16_t color);


   private:
   	  // Constructor to prohibit copy construction.
      NokiaScreen(const NokiaScreen&);

      // Operator= to prohibit copy assignment
      NokiaScreen& operator=(const NokiaScreen&);

      Adafruit_PCD8544 mPCD8544;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_NokiaScreen__hpp
