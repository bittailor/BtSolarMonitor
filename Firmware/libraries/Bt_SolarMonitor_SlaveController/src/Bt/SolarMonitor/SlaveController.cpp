//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::SlaveController
//
//*************************************************************************************************

#include "Bt/SolarMonitor/SlaveController.hpp"

#include "Bt/SolarMonitor/IOSlavePins.hpp"

namespace Bt {
namespace SolarMonitor {

SlaveController::SlaveController()
: mRelayAToOn(BT_SOLARMONITOR_IOSLAVE_PIN_RELAY_A_ON)
, mRelayAToOff(BT_SOLARMONITOR_IOSLAVE_PIN_RELAY_A_OFF)
, mRelayBToOn(BT_SOLARMONITOR_IOSLAVE_PIN_RELAY_B_ON)
, mRelayBToOff(BT_SOLARMONITOR_IOSLAVE_PIN_RELAY_B_OFF)
, mRelayLoadToOn(BT_SOLARMONITOR_IOSLAVE_PIN_RELAY_LOAD_ON)
, mRelayLoadToOff(BT_SOLARMONITOR_IOSLAVE_PIN_RELAY_LOAD_OFF)
, mLedA(BT_SOLARMONITOR_IOSLAVE_PIN_LED_A)
, mLedB(BT_SOLARMONITOR_IOSLAVE_PIN_LED_B)
, mRelayA(mRelayAToOff, mRelayAToOn)
, mRelayB(mRelayBToOff, mRelayBToOn)
, mRelayLoad(mRelayLoadToOff, mRelayLoadToOn)
, mStateLeds(mLedA, mLedB)

, mRelayController(mTime, mRelayControllerQueryPort, mRelayA, mRelayB, mRelayLoad, mStateLeds){
}

SlaveController::~SlaveController() {

}

void SlaveController::begin() {
   mRelayController.begin();
}

void SlaveController::loop() {
   mRelayController.loop();
}

void SlaveController::toggleOnOff() {
   mRelayController.handle(&RelayController::State::toggleOnOff);
}

void SlaveController::toggleAB() {
   mRelayController.handle(&RelayController::State::toggleAB);
}


} // namespace SolarMonitor
} // namespace Bt

