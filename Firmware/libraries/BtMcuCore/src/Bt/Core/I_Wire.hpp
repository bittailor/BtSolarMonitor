//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::Core::I_Wire
//
//*************************************************************************************************

#ifndef INC__Bt_Core_I_Wire__hpp
#define INC__Bt_Core_I_Wire__hpp

#include <stddef.h>
#include <stdint.h>

namespace Bt {
namespace Core {

class I_Wire {
   public:
      virtual ~I_Wire() {}

      virtual void beginTransmission(uint8_t pAddress)=0;
      virtual uint8_t endTransmission()=0;
      virtual size_t write(uint8_t pData)=0;
      virtual size_t write(const uint8_t* pData, size_t pQuantity)=0;

      virtual uint8_t requestFrom(uint8_t pAddress, size_t pQuantity)=0;
      virtual int read()=0;

};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_I_Wire__hpp
