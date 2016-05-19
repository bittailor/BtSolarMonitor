//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Workcycle
//
//*************************************************************************************************

#include "Bt/Core/Workcycle.hpp"
#include <Arduino.h>

namespace Bt {
namespace Core {


//-------------------------------------------------------------------------------------------------

Workcycle::Workcycle() {

}

//-------------------------------------------------------------------------------------------------

Workcycle::~Workcycle() {

}

//-------------------------------------------------------------------------------------------------

void Workcycle::add(I_Runnable& iRunnable) {
   mRunnables.pushBack(iRunnable);
}

//-------------------------------------------------------------------------------------------------

void Workcycle::remove(I_Runnable& iRunnable) {
   mRunnables.remove(iRunnable);
}

//-------------------------------------------------------------------------------------------------

uint32_t Workcycle::oneWorkcycle() {
   uint32_t minDelay = I_Runnable::FOREVER;
   for (Runnables::Iterator iterator = mRunnables.begin(), end = mRunnables.end(); iterator != end; ++iterator) {
      uint32_t delay = iterator->workcycle();
      minDelay = min(delay,minDelay);
   }
   //LOG("min delay = " << minDelay);
   return minDelay;

}

//-------------------------------------------------------------------------------------------------

} // namespace Core
} // namespace Bt
