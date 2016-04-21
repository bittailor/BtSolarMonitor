//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::PowerState
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_PowerState__hpp
#define INC__Bt_SolarMonitor_PowerState__hpp

#include "Bt/SolarMonitor/I_PowerState.hpp"

namespace Bt {
namespace SolarMonitor {

class PowerState : public I_PowerState {
   public:
      virtual State state() { return mState; }
      virtual void state(State pState) { mState = pState; }

   private:
      State mState;
};





} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_PowerState__hpp
