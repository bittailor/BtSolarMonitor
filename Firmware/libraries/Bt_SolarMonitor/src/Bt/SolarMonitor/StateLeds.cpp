//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::StateLeds
//
//*************************************************************************************************

#include "Bt/SolarMonitor/StateLeds.hpp"

namespace Bt {
namespace SolarMonitor {

StateLeds::StateLeds(Core::I_DigitalOut& pLedA, Core::I_DigitalOut& pLedB)
: mLedA(&pLedA), mLedB(&pLedB) {

}

StateLeds::~StateLeds() {

}

void StateLeds::show(State pState) {
   switch(pState) {
      case Off : {
         mLedA->write(false);
         mLedB->write(false);
      }break;
      case OnA : {
         mLedA->write(true);
         mLedB->write(false);
      }break;
      case OnB : {
         mLedA->write(false);
         mLedB->write(true);
      }break;
   }
}

} // namespace SolarMonitor
} // namespace Bt
