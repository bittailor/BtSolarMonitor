//*************************************************************************************************
//
//  BITTAILOR.CH - Firmware     
//
//*************************************************************************************************

#include "RelayController.hpp"

namespace Bt {
namespace SolarMonitor {


//-------------------------------------------------------------------------------------------------

RelayController::RelayController(
         Core::I_Time& pTime,
         I_RelayControllerQueryPort& pQueryPort,
         I_LatchingRelay& pRelayA,
         I_LatchingRelay& pRelayB,
         I_LatchingRelay& pRelayLoad,
         I_StateLeds& pStateLeds)
: Core::StateMachine<I_RelayController, RelayController>(pTime)
, mQueryPort(&pQueryPort)
, mRelayA(&pRelayA)
, mRelayB(&pRelayB)
, mRelayLoad(&pRelayLoad)
, mStateLeds(&pStateLeds)
, mInitial(*this)
, mOff(*this)
, mOnA(*this)
, mOnB(*this)
, mToggleAOn(*this)
, mToggleAOff(*this)
, mSwitchToA(*this)
, mToggleBOn(*this)
, mToggleBOff(*this)
, mSwitchToB(*this)
, mToggleLoadOn(*this)
{
}

//-------------------------------------------------------------------------------------------------

RelayController::~RelayController()
{

}

//-------------------------------------------------------------------------------------------------

const char* RelayController::name() {
   return "RelayController";
}

//-------------------------------------------------------------------------------------------------

void RelayController::begin() {
   init(mInitial);
}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
