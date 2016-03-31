//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::SolarMonitor::I_RelayController
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_I_RelayController__hpp
#define INC__Bt_SolarMonitor_I_RelayController__hpp

namespace Bt {
namespace SolarMonitor {

class I_RelayController {
   public:

      enum PublicState {Off, OnA, OnB};

      class State {
         public:
         virtual ~State() {}
         virtual void toggleOnOff(){}
         virtual void toggleAB(){}
         virtual void toOn(){}
         virtual void toOff(){}
         virtual void toA(){}
         virtual void toB(){}
      };
      typedef void (State::*Event)();

      virtual ~I_RelayController() {}

      virtual void handle(Event pEvent) = 0;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_I_RelayController__hpp
