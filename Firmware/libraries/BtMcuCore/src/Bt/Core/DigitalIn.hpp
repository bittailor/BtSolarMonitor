//*************************************************************************************************
//
//  BITTAILOR.CH - Firmware
//
//*************************************************************************************************

#ifndef INC__Bt_Core_DigitalIn__hpp
#define INC__Bt_Core_DigitalIn__hpp

#include <stdint.h>
#include "Bt/Core/I_DigitalIn.hpp"

namespace Bt {
namespace Core {

class DigitalIn : I_DigitalIn
{
   public:
      DigitalIn(uint8_t pPin);
      ~DigitalIn();

      virtual bool read();

   private:
   	  // Constructor to prohibit copy construction.
      DigitalIn(const DigitalIn&);

      // Operator= to prohibit copy assignment
      DigitalIn& operator=(const DigitalIn&);

      uint8_t mPin;
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_DigitalIn__hpp
