//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::I_RelayControllerActionPort
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_I_RelayControllerActionPort__hpp
#define INC__Bt_SolarMonitor_I_RelayControllerActionPort__hpp

namespace Bt {
namespace SolarMonitor {

class I_RelayControllerActionPort {
   public:
      virtual ~I_RelayControllerActionPort() {}

      virtual void relayAToOn(bool pHigh)=0;
      virtual void relayAToOff(bool pHigh)=0;
      virtual void relayBToOn(bool pHigh)=0;
      virtual void relayBToOff(bool pHigh)=0;
      virtual void relayLoadToOn(bool pHigh)=0;
      virtual void relayLoadToOff(bool pHigh)=0;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_I_RelayControllerActionPort__hpp
