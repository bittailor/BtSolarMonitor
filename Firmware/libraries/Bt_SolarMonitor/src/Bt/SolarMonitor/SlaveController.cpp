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
, mNotify(BT_SOLARMONITOR_IOSLAVE_PIN_NOTIFY)
, mOnOff(BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_ON_OFF)
, mAB(BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_A_B)
, mRelayA(mRelayAToOff, mRelayAToOn)
, mRelayB(mRelayBToOff, mRelayBToOn)
, mRelayLoad(mRelayLoadToOff, mRelayLoadToOn)

, mPowerState(mNotify)

, mRelayController(mTime, mRelayControllerQueryPort, mRelayA, mRelayB, mRelayLoad, mPowerState)

, mOnOffButtonListener(*this, &SlaveController::toggleOnOff)
, mABButtonListener(*this, &SlaveController::toggleAB)

, mOnOffButton(mTime, mOnOff, mOnOffButtonListener)
, mABButton(mTime, mAB, mABButtonListener)

, mWireSlave(mWire, mPowerState) {
   mWorkcycle.add(mRelayController);
   mWorkcycle.add(mOnOffButton);
   mWorkcycle.add(mABButton);
   mWorkcycle.add(mWireSlave);
}

SlaveController::~SlaveController() {

}

void SlaveController::begin() {
   mRelayController.begin();
   mOnOffButton.begin();
   mABButton.begin();
}

uint32_t SlaveController::oneWorkcycle() {
   return mWorkcycle.oneWorkcycle();
}

void SlaveController::toggleOnOff() {
   mRelayController.toggleOnOff();
}

void SlaveController::toggleAB() {
   mRelayController.toggleAB();
}


} // namespace SolarMonitor
} // namespace Bt

