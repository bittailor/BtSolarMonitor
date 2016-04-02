//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_StateLedsGMock__hpp
#define INC__Bt_SolarMonitor_StateLedsGMock__hpp

#include <gmock/gmock.h>

#include "Bt/SolarMonitor/I_StateLeds.hpp"

namespace Bt {
namespace SolarMonitor {

class StateLedsGMock : public I_StateLeds {
   public:

      MOCK_METHOD1(show, void(State pState));
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_StateLedsGMock__hpp
