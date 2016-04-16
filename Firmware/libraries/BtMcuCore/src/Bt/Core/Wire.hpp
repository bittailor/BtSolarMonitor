//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Wire
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Wire__hpp
#define INC__Bt_Core_Wire__hpp

#include "Bt/Core/I_Wire.hpp"

namespace Bt {
namespace Core {

class WireWrapper : public I_Wire
{
   public:
      WireWrapper();
      ~WireWrapper();

      virtual void beginTransmission(uint8_t pAddress);
      virtual uint8_t endTransmission();
      virtual size_t write(uint8_t pData);

      virtual uint8_t requestFrom(uint8_t pAddress, size_t pQuantity);
      virtual int read();

   private:
   	  // Constructor to prohibit copy construction.
      WireWrapper(const WireWrapper&);

      // Operator= to prohibit copy assignment
      WireWrapper& operator=(const WireWrapper&);
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Wire__hpp
