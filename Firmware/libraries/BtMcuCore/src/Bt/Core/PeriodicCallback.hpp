//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::PeriodicCallback
//
//*************************************************************************************************

#ifndef INC__Bt_Core_PeriodicCallback__hpp
#define INC__Bt_Core_PeriodicCallback__hpp

#include "Bt/Core/I_Runnable.hpp"
#include "Bt/Core/I_Time.hpp"

namespace Bt {
namespace Core {


class PeriodicCallbackBase : public I_Runnable {
   public:
      PeriodicCallbackBase(I_Time& pTime, uint32_t pPeriodInMicroseconds);

      virtual uint32_t workcycle();
   protected:
      virtual void callback() = 0;
   private:
      I_Time* mTime;
      const uint32_t mPeriodInMicroseconds;
      uint32_t mStartTime;
};

template<typename T>
class PeriodicCallback : public PeriodicCallbackBase
{
   public:
      typedef void (T::*Callback)();

      PeriodicCallback(I_Time& pTime, uint32_t pPeriodInMicroseconds, T& pObject, Callback pCallback)
      : PeriodicCallbackBase(pTime,pPeriodInMicroseconds), mObject(pObject), mCallback(pCallback) {
      }

      virtual void callback() {
         (mObject->*mCallback)();
      }

   private:
   	  // Constructor to prohibit copy construction.
      PeriodicCallback(const PeriodicCallback&);

      // Operator= to prohibit copy assignment
      PeriodicCallback& operator=(const PeriodicCallback&);

      T* mObject;
      Callback mCallback;

};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_PeriodicCallback__hpp
