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
         I_PowerState& pPowerState)
: Core::StateMachine<RelayControllerState, RelayController>(pTime)
, mQueryPort(&pQueryPort)
, mRelayA(&pRelayA)
, mRelayB(&pRelayB)
, mRelayLoad(&pRelayLoad)
, mPowerState(&pPowerState)
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
{
}

//-------------------------------------------------------------------------------------------------

RelayController::~RelayController()
{

}

//-------------------------------------------------------------------------------------------------

const char* RelayController::name() {
   return "RC";
}

//-------------------------------------------------------------------------------------------------

void RelayController::begin() {
   init(mInitial);


}

//-------------------------------------------------------------------------------------------------

void RelayController::turnOnLoadRelay() {
    mRelayLoad->onCoil(true);  
    Core::delayInMilliseconds(RELAY_ENERGIZATION_DURATION);
    mRelayLoad->onCoil(false);
} 

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
