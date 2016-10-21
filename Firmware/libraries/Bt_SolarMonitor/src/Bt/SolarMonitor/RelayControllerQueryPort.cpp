//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::RelayControllerQueryPort
//
//*************************************************************************************************

#include "Bt/SolarMonitor/RelayControllerQueryPort.hpp"

#include "IoSlavePins.hpp"

#define Sense_L A0
#define Sense_A A1
#define Sense_B A2

namespace Bt {
namespace SolarMonitor {

RelayControllerQueryPort::RelayControllerQueryPort()
: mLoadASense(BT_SOLARMONITOR_IOSLAVE_PIN_Sense_A)
, mLoadBSense(BT_SOLARMONITOR_IOSLAVE_PIN_Sense_B)
, mLoadOut(BT_SOLARMONITOR_IOSLAVE_PIN_Sense_L)
, mBatteryState(I_BatteryState::UseFromA){

}

RelayControllerQueryPort::~RelayControllerQueryPort() {

}

bool RelayControllerQueryPort::loadASense() {
   return mLoadASense.read();
}

bool RelayControllerQueryPort::loadBSense() {
   return mLoadBSense.read();
}

bool RelayControllerQueryPort::loadOut() {
   return mLoadOut.read();
}

bool RelayControllerQueryPort::isBatteryABetter() {
   return mBatteryState == I_BatteryState::UseFromA;
}

void RelayControllerQueryPort::state(I_BatteryState::State pState) {
   mBatteryState = pState;
}

} // namespace SolarMonitor
} // namespace Bt
