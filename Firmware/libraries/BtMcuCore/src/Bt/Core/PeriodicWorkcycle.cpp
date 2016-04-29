//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::PeriodicWorkcycle
//
//*************************************************************************************************

#include "Bt/Core/PeriodicWorkcycle.hpp"

namespace Bt {
namespace Core {


//-------------------------------------------------------------------------------------------------

PeriodicWorkcycle::PeriodicWorkcycle(uint32_t iPeriodInMicroSeconds, I_Time& iTime)
: mTime(&iTime), mPeriodInMicroSeconds(iPeriodInMicroSeconds), mNextWorkcycle(0){
}

//-------------------------------------------------------------------------------------------------

PeriodicWorkcycle::~PeriodicWorkcycle() {

}

//-------------------------------------------------------------------------------------------------

uint32_t PeriodicWorkcycle::workcycle() {
   uint32_t now = mTime->microseconds();
   if (now >= mNextWorkcycle) {
      while(now >= mNextWorkcycle) {
         mNextWorkcycle += mPeriodInMicroSeconds;
      }
      oneWorkcycle();
   }
   return mNextWorkcycle - now;
}

//-------------------------------------------------------------------------------------------------

} // namespace Core
} // namespace Bt
