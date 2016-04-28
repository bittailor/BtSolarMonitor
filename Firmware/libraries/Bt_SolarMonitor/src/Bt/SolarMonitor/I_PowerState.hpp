//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::SolarMonitor::I_PowerState
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_I_PowerState__hpp
#define INC__Bt_SolarMonitor_I_PowerState__hpp

namespace Bt {
namespace SolarMonitor {

class I_PowerState {
   public:
      enum State {Off, OnA, OnB};
      virtual ~I_PowerState() {}
      virtual State state() = 0;
      virtual void state(State pState) = 0;

      virtual void resetNotify() = 0;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_I_PowerState__hpp
