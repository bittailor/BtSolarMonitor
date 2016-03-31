//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::RelayControllerQueryPort
//
//*************************************************************************************************

#include "Bt/SolarMonitor/RelayControllerQueryPort.hpp"

#ifdef ARDUINO
   #include <Arduino.h>
#else
   #define A0 (14ul)
   #define A1 (15ul)
   #define A2 (16ul)
#endif

#define Sense_L A0
#define Sense_A A1
#define Sense_B A2

namespace Bt {
namespace SolarMonitor {

RelayControllerQueryPort::RelayControllerQueryPort()
: mLoadASense(Sense_A)
, mLoadBSense(Sense_B)
, mLoadOut(Sense_L) {

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
   return true;
}

} // namespace SolarMonitor
} // namespace Bt
