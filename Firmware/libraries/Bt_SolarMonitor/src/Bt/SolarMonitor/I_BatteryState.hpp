//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::SolarMonitor::I_BatteryState
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_I_BatteryState__hpp
#define INC__Bt_SolarMonitor_I_BatteryState__hpp

namespace Bt {
namespace SolarMonitor {

class I_BatteryState {
   public:
      enum State {UseFromA, UseFromB};
      virtual ~I_BatteryState() {}
      virtual void state(State pState) = 0;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_I_BatteryState__hpp
