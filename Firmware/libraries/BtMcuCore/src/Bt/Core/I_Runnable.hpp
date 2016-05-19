//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::Core::I_Runnable
//
//*************************************************************************************************

#ifndef INC__Bt_Core_I_Runnable__hpp
#define INC__Bt_Core_I_Runnable__hpp

#include <stdint.h>

#include "Bt/Core/IntrusiveList.hpp"

namespace Bt {
namespace Core {

class I_Runnable : public IntrusiveList<I_Runnable>::Element {
   public:
      static const uint32_t IMMEDIATELY  = 0x0;
      static const uint32_t FOREVER = 0xFFFFFFFFu;

      virtual ~I_Runnable() {}
      virtual uint32_t workcycle() = 0;
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_I_Runnable__hpp
