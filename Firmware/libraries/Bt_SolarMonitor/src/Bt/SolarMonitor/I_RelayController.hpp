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
      virtual ~I_RelayController() {}

      virtual void switchOn() = 0;
      virtual void switchOff() = 0;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_I_RelayController__hpp
