//*************************************************************************************************
//
//  BITTAILOR.CH - Firmware
//
//*************************************************************************************************

#ifndef INC__Bt_Core_DigitalIn__hpp
#define INC__Bt_Core_DigitalIn__hpp

#include "Bt/Core/I_DigitalIn.hpp"

namespace Bt {
namespace Core {

class DigitalIn : I_DigitalIn
{
   public:
      DigitalIn();
      ~DigitalIn();

   private:
   	  // Constructor to prohibit copy construction.
      DigitalIn(const DigitalIn&);

      // Operator= to prohibit copy assignment
      DigitalIn& operator=(const DigitalIn&);
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_DigitalIn__hpp
