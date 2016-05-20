//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Timer
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Timer__hpp
#define INC__Bt_Core_Timer__hpp

#include "Bt/Core/Timing.hpp"

namespace Bt {
namespace Core {

class Timer {
   public:
      Timer() : mInterval(0), mStart(milliseconds()) {
      }

      Timer(int ms) : mInterval(ms), mStart(milliseconds()) {
      }

      bool expired() {
         return (milliseconds() - mStart) >= mInterval;
      }

   private:
      uint32_t mInterval;
      uint32_t mStart;
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Timer__hpp
