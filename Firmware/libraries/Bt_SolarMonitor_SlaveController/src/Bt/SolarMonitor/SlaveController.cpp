//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::SlaveController
//
//*************************************************************************************************

#include "Bt/SolarMonitor/SlaveController.hpp"

namespace Bt {
namespace SolarMonitor {

SlaveController::SlaveController()
: mRelayController(mTime, mRelayControllerActionPort, mRelayControllerQueryPort){

}

SlaveController::~SlaveController() {

}

void SlaveController::begin() {
   mRelayController.begin();
}

void SlaveController::loop() {
   mRelayController.loop();
}


} // namespace SolarMonitor
} // namespace Bt

