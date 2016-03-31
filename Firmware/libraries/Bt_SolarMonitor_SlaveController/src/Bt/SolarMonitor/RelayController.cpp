//*************************************************************************************************
//
//  BITTAILOR.CH - Firmware     
//
//*************************************************************************************************

#include "RelayController.hpp"

namespace Bt {
namespace SolarMonitor {


//-------------------------------------------------------------------------------------------------

RelayController::RelayController(Core::I_Time& pTime, I_RelayControllerActionPort& pActionPort, I_RelayControllerQueryPort& pQueryPort)
: Core::StateMachine<I_RelayController, RelayController>(pTime)
, mActionPort(&pActionPort)
, mQueryPort(&pQueryPort)
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