//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::PeriodicCallback
//
//*************************************************************************************************

#ifndef INC__Bt_Core_PeriodicCallback__hpp
#define INC__Bt_Core_PeriodicCallback__hpp

#include "Bt/Core/I_Runnable.hpp"
#include "Bt/Core/I_Time.hpp"
#include "Bt/Core/Function.hpp"

namespace Bt {
namespace Core {

class PeriodicCallback : public I_Runnable {
   public:
      PeriodicCallback(I_Time& pTime, uint32_t pPeriodInMilliseconds, Function<void()> pCallback);
      virtual uint32_t workcycle();

   private:
      // Constructor to prohibit copy construction.
      PeriodicCallback(const PeriodicCallback&);

      // Operator= to prohibit copy assignment
      PeriodicCallback& operator=(const PeriodicCallback&);

      I_Time* mTime;
      const uint32_t mPeriodInMilliseconds;
      uint32_t mStartTime;
      Function<void()> mCallback;
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_PeriodicCallback__hpp
