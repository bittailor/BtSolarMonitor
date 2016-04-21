//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::SolarMonitor::I_IoSlave
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_I_IoSlave__hpp
#define INC__Bt_SolarMonitor_I_IoSlave__hpp

#include "Bt/SolarMonitor/I_PowerState.hpp"

namespace Bt {
namespace SolarMonitor {

class I_IoSlave {
   public:
      virtual ~I_IoSlave() {}

      virtual I_PowerState::State powerState() = 0;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_I_IoSlave__hpp
