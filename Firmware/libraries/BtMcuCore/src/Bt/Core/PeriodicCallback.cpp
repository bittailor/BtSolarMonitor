//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::PeriodicCallback
//
//*************************************************************************************************

#include "Bt/Core/PeriodicCallback.hpp"

namespace Bt {
namespace Core {


//-------------------------------------------------------------------------------------------------

PeriodicCallbackBase::PeriodicCallbackBase(I_Time& pTime, uint32_t pPeriodInMicroseconds)
: mTime(&pTime), mPeriodInMicroseconds(pPeriodInMicroseconds), mStartTime(0){
}

//-------------------------------------------------------------------------------------------------

uint32_t PeriodicCallbackBase::workcycle() {
   uint32_t diff = mTime->milliseconds() - mStartTime;
   if (diff >= mPeriodInMicroseconds) {
      mStartTime = mTime->milliseconds();
      callback();
   }
   return diff - mPeriodInMicroseconds;
}
//-------------------------------------------------------------------------------------------------

} // namespace Core
} // namespace Bt
