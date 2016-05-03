//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::PeriodicCallback
//
//*************************************************************************************************

#include "Bt/Core/PeriodicCallback.hpp"

namespace Bt {
namespace Core {


//-------------------------------------------------------------------------------------------------

PeriodicCallback::PeriodicCallback(I_Time& pTime, uint32_t pPeriodInMilliseconds, Function<void()> pCallback)
: mTime(&pTime), mPeriodInMilliseconds(pPeriodInMilliseconds), mStartTime(mTime->milliseconds()), mCallback(pCallback){
}

//-------------------------------------------------------------------------------------------------

uint32_t PeriodicCallback::workcycle() {
   uint32_t now = mTime->milliseconds();
   uint32_t diff = now - mStartTime;
   if (diff >= mPeriodInMilliseconds) {
      mStartTime = now;
      if(mCallback) {
         mCallback();
      }
      return mPeriodInMilliseconds;
   }
   return mPeriodInMilliseconds - diff;
}
//-------------------------------------------------------------------------------------------------

} // namespace Core
} // namespace Bt
