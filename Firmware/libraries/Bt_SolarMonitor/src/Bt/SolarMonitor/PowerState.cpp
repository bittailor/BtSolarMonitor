//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::PowerState
//
//*************************************************************************************************

#include "Bt/SolarMonitor/PowerState.hpp"

#include "Bt/Core/Logger.hpp"

namespace Bt {
namespace SolarMonitor {

//-------------------------------------------------------------------------------------------------

PowerState::PowerState(Core::I_DigitalOut& pNotifyMaster): mNotifyMaster(&pNotifyMaster) {
   mNotifyMaster->write(true);
}

//-------------------------------------------------------------------------------------------------

void PowerState::state(State pState) {
   mState = pState;
   LOG("setNotify");
   mNotifyMaster->write(false);
}

//-------------------------------------------------------------------------------------------------

void PowerState::resetNotify() {
   LOG("resetNotify");
   mNotifyMaster->write(true);
}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
