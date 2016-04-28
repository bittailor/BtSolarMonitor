//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_PowerStateGMock__hpp
#define INC__Bt_SolarMonitor_PowerStateGMock__hpp

#include <gmock/gmock.h>

#include "Bt/SolarMonitor/I_PowerState.hpp"

namespace Bt {
namespace SolarMonitor {

class PowerStateGMock : public I_PowerState {
   public:

      MOCK_METHOD0(state, State());
      MOCK_METHOD1(state, void(State));
      MOCK_METHOD0(resetNotify, void());
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_PowerStateGMock__hpp
