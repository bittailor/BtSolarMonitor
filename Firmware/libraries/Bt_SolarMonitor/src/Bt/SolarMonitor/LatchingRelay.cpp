//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::LatchingRelay
//
//*************************************************************************************************

#include "Bt/SolarMonitor/LatchingRelay.hpp"

namespace Bt {
namespace SolarMonitor {

LatchingRelay::LatchingRelay(Core::I_DigitalOut& pOff, Core::I_DigitalOut& pOn)
: mOff(&pOff), mOn(&pOn)  {
}

LatchingRelay::~LatchingRelay() {

}

void LatchingRelay::onCoil(bool pValue) {
   mOn->write(pValue);
}

void LatchingRelay::offCoil(bool pValue) {
   mOff->write(pValue);
}

} // namespace SolarMonitor
} // namespace Bt
