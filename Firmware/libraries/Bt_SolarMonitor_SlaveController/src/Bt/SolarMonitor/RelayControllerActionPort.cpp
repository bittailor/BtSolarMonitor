//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::RelayControllerActionPort
//
//*************************************************************************************************

#include "Bt/SolarMonitor/RelayControllerActionPort.hpp"

#define RELAY_A_ON   5
#define RELAY_A_OFF  6
#define RELAY_B_ON   8
#define RELAY_B_OFF  7
#define RELAY_LOAD_ON   9
#define RELAY_LOAD_OFF 10

namespace Bt {
namespace SolarMonitor {

RelayControllerActionPort::RelayControllerActionPort()
: mRelayAToOn(RELAY_A_ON)
, mRelayAToOff(RELAY_A_OFF)
, mRelayBToOn(RELAY_B_ON)
, mRelayBToOff(RELAY_B_OFF)
, mRelayLoadToOn(RELAY_LOAD_ON)
, mRelayLoadToOff(RELAY_LOAD_OFF)
{

}

RelayControllerActionPort::~RelayControllerActionPort() {

}

void RelayControllerActionPort::relayAToOn(bool pHigh) {

}

void RelayControllerActionPort::relayAToOff(bool pHigh) {
}

void RelayControllerActionPort::relayBToOn(bool pHigh) {
}

void RelayControllerActionPort::relayBToOff(bool pHigh) {
}

void RelayControllerActionPort::relayLoadToOn(bool pHigh) {
}

void RelayControllerActionPort::relayLoadToOff(bool pHigh) {
}

void RelayControllerActionPort::publicState(I_StateLeds::State pState) {
}

} // namespace SolarMonitor
} // namespace Bt
