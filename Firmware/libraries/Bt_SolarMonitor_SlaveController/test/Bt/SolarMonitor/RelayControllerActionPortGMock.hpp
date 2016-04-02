//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_RelayControllerActionPortGMock__hpp
#define INC__Bt_SolarMonitor_RelayControllerActionPortGMock__hpp

#include <gmock/gmock.h>

#include "Bt/SolarMonitor/I_RelayControllerActionPort.hpp"

namespace Bt {
namespace SolarMonitor {

class RelayControllerActionPortGMock : public I_RelayControllerActionPort {
   public:

      MOCK_METHOD1(relayAToOn, void(bool));
      MOCK_METHOD1(relayAToOff, void(bool));
      MOCK_METHOD1(relayBToOn, void(bool));
      MOCK_METHOD1(relayBToOff, void(bool));
      MOCK_METHOD1(relayLoadToOn, void(bool));
      MOCK_METHOD1(relayLoadToOff, void(bool));
      MOCK_METHOD1(publicState, void(I_StateLeds::State));
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_RelayControllerActionPortGMock__hpp
