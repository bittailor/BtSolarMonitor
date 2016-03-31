//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::RelayControllerActionPort
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_RelayControllerActionPort__hpp
#define INC__Bt_SolarMonitor_RelayControllerActionPort__hpp

#include "Bt/Core/DigitalOut.hpp"

#include "Bt/SolarMonitor/I_RelayControllerActionPort.hpp"

namespace Bt {
namespace SolarMonitor {

class RelayControllerActionPort : public I_RelayControllerActionPort
{
   public:
      RelayControllerActionPort();
      ~RelayControllerActionPort();

      virtual void relayAToOn(bool pHigh);
      virtual void relayAToOff(bool pHigh);
      virtual void relayBToOn(bool pHigh);
      virtual void relayBToOff(bool pHigh);
      virtual void relayLoadToOn(bool pHigh);
      virtual void relayLoadToOff(bool pHigh);
      virtual void publicState(I_RelayController::PublicState pState);

   private:
   	  // Constructor to prohibit copy construction.
      RelayControllerActionPort(const RelayControllerActionPort&);

      // Operator= to prohibit copy assignment
      RelayControllerActionPort& operator=(const RelayControllerActionPort&);

      Core::DigitalOut mRelayAToOn;
      Core::DigitalOut mRelayAToOff;
      Core::DigitalOut mRelayBToOn;
      Core::DigitalOut mRelayBToOff;
      Core::DigitalOut mRelayLoadToOn;
      Core::DigitalOut mRelayLoadToOff;


};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_RelayControllerActionPort__hpp
