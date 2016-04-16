//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_RelayControllerQueryPortGMock__hpp
#define INC__Bt_SolarMonitor_RelayControllerQueryPortGMock__hpp

#include <gmock/gmock.h>

#include "../../../../Bt_SolarMonitor/src/Bt/SolarMonitor/I_RelayControllerQueryPort.hpp"

namespace Bt {
namespace SolarMonitor {

class RelayControllerQueryPortGMock : public I_RelayControllerQueryPort {
   public:

      MOCK_METHOD0(loadASense, bool());
      MOCK_METHOD0(loadBSense, bool());
      MOCK_METHOD0(loadOut, bool());
      MOCK_METHOD0(isBatteryABetter, bool());
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_RelayControllerQueryPortGMock__hpp
