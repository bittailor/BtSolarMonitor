//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_LatchingRelayGMock__hpp
#define INC__Bt_SolarMonitor_LatchingRelayGMock__hpp

#include <gmock/gmock.h>

#include "Bt/SolarMonitor/I_LatchingRelay.hpp"

namespace Bt {
namespace SolarMonitor {

class LatchingRelayGMock : public I_LatchingRelay {
   public:

      MOCK_METHOD1(onCoil, void(bool pValue));
      MOCK_METHOD1(offCoil, void(bool pValue));

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_LatchingRelayGMock__hpp
