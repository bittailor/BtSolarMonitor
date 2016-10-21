//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::IoSlaveCommand
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_IoSlaveCommand__hpp
#define INC__Bt_SolarMonitor_IoSlaveCommand__hpp

#include <stdint.h>

namespace Bt {
namespace SolarMonitor {

struct IoSlaveCommandEnum {
      enum IoSlaveCommand {
         GetPowerState,
         SetBatteryState,
         _LastCommand
      };
};
typedef IoSlaveCommandEnum::IoSlaveCommand IoSlaveCommand;


} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_IoSlaveCommand__hpp
