//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::DigitalOut
//
//*************************************************************************************************

#ifndef INC__Bt_Core_DigitalOut__hpp
#define INC__Bt_Core_DigitalOut__hpp

#include <stdint.h>
#include "Bt/Core/I_DigitalOut.hpp"

namespace Bt {
namespace Core {

class DigitalOut : public I_DigitalOut
{
   public:
      DigitalOut(uint8_t pPin);
      ~DigitalOut();

      virtual void write(bool pValue);

   private:
   	  // Constructor to prohibit copy construction.
      DigitalOut(const DigitalOut&);

      // Operator= to prohibit copy assignment
      DigitalOut& operator=(const DigitalOut&);

      uint8_t mPin;
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_DigitalOut__hpp
