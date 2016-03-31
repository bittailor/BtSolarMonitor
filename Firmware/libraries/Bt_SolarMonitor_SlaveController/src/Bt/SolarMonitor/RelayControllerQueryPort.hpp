//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::RelayControllerQueryPort
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_RelayControllerQueryPort__hpp
#define INC__Bt_SolarMonitor_RelayControllerQueryPort__hpp

#include "Bt/SolarMonitor/I_RelayControllerQueryPort.hpp"

#include "Bt/Core/DigitalIn.hpp"

namespace Bt {
namespace SolarMonitor {

class RelayControllerQueryPort : public I_RelayControllerQueryPort
{
   public:
      RelayControllerQueryPort();
      ~RelayControllerQueryPort();

      virtual bool loadASense();
      virtual bool loadBSense();
      virtual bool loadOut();
      virtual bool isBatteryABetter();

   private:
   	  // Constructor to prohibit copy construction.
      RelayControllerQueryPort(const RelayControllerQueryPort&);

      // Operator= to prohibit copy assignment
      RelayControllerQueryPort& operator=(const RelayControllerQueryPort&);

      Core::DigitalIn mLoadASense;
      Core::DigitalIn mLoadBSense;
      Core::DigitalIn mLoadOut;


};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_RelayControllerQueryPort__hpp
