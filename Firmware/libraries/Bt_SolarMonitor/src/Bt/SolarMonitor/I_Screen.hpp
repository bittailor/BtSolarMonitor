//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::SolarMonitor::I_Screen
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_I_Screen__hpp
#define INC__Bt_SolarMonitor_I_Screen__hpp

#include <stdint.h>
#include <stdio.h>

namespace Bt {
namespace SolarMonitor {

#ifndef WHITE
 #define WHITE 0
#endif
#ifndef BLACK
 #define BLACK 1
#endif

class I_Screen {
   public:

      virtual ~I_Screen() {}

      virtual void clear() = 0;
      virtual void display() =0;

      virtual void setTextSize(uint8_t pSize) =0;
      virtual void setTextColor(uint16_t pColor) =0;

      virtual size_t print(const char* pString) =0;
      virtual size_t print(char pChar) =0;
      virtual size_t print(int pInt) =0;
      virtual size_t print(double pDouble, int pDigits = 2) =0;


      virtual size_t println(const char* pString) =0;
      virtual size_t println(char pChar) =0;
      virtual size_t println(int pInt) =0;
      virtual size_t println(double pDouble, int pDigits = 2) =0;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_I_Screen__hpp