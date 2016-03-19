//*************************************************************************************************
//
//  BITTAILOR.CH - Firmware     
//
//*************************************************************************************************

#include "RelayController.hpp"

namespace Bt {
namespace SolarMonitor {


//-------------------------------------------------------------------------------------------------

RelayController::RelayController(I_RelayControllerActionPort& pActionPort, I_RelayControllerQueryPort& pQueryPort)
: mActionPort(&pActionPort)
, mQueryPort(&pQueryPort)
, mInitial(*this)
, mOff_A(*this)
, mOff_B(*this)
, mOn_A(*this)
, mOn_B(*this)
, mToggleAOn(*this)
, mToggleAOff(*this)
, mToggleBOn(*this)
, mToggleBOff(*this)
, mToggleLoadOn(*this)
, mToggleLoadOff(*this)
, mStateMachine(mInitial)
{


}

//-------------------------------------------------------------------------------------------------

RelayController::~RelayController()
{

}

//-------------------------------------------------------------------------------------------------

void RelayController::Initial::onEnter(){
   //std::cout << "Initial::onEnter" << std::endl;
   if(mController->mQueryPort->loadASense() && mController->mQueryPort->loadBSense()) {
      mController->mStateMachine.nextState(mController->mToggleAOn); // TODO decide on voltage
   } else if (mController->mQueryPort->loadASense()) {
      if(mController->mQueryPort->loadOut()) {
         mController->mStateMachine.nextState(mController->mOn_B);
      } else {
         mController->mStateMachine.nextState(mController->mOff_B);
      }

   } else {
      if(mController->mQueryPort->loadOut()) {
         mController->mStateMachine.nextState(mController->mOn_A);
      } else {
         mController->mStateMachine.nextState(mController->mOff_A);
      }
}
}


//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
