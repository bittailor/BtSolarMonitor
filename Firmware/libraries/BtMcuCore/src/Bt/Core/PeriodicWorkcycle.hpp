//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::PeriodicWorkcycle
//
//*************************************************************************************************

#ifndef INC__Bt_Core_PeriodicWorkcycle__hpp
#define INC__Bt_Core_PeriodicWorkcycle__hpp

#include "Bt/Core/I_Runnable.hpp"
#include "Bt/Core/Workcycle.hpp"
#include "Bt/Core/I_Time.hpp"


namespace Bt {
namespace Core {

class PeriodicWorkcycle : public Workcycle, public I_Runnable
{
   public:
      PeriodicWorkcycle(uint32_t iPeriodInMicroSeconds, I_Time& iTime);
      ~PeriodicWorkcycle();

      uint32_t workcycle();

   private:
        // Constructor to prohibit copy construction.
      PeriodicWorkcycle(const PeriodicWorkcycle&);

      // Operator= to prohibit copy assignment
      PeriodicWorkcycle& operator=(const PeriodicWorkcycle&);

      I_Time* mTime;
      const uint32_t mPeriodInMicroSeconds;
      uint32_t mNextWorkcycle;

};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_PeriodicWorkcycle__hpp
