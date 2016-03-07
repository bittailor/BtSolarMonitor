//*************************************************************************************************
//
//  BITTAILOR.CH - Firmware     
//
//*************************************************************************************************

#ifndef INC__BT_SOLARMONITOR_RELAYCONTROLLER__HPP
#define INC__BT_SOLARMONITOR_RELAYCONTROLLER__HPP

#include <Bt/Core/I_DigitalIn.hpp>

namespace Bt {
namespace SolarMonitor {

class RelayController 
{
   public:
      RelayController();
      ~RelayController();
   
   private:
   	  // Constructor to prohibit copy construction.
      RelayController(const RelayController&);

      // Operator= to prohibit copy assignment
      RelayController& operator=(const RelayController&);


      Core::I_DigitalIn* mLoadASense;
      Core::I_DigitalIn* mLoadBSense;
      Core::I_DigitalIn* mLoadOut;







};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__BT_SOLARMONITOR_RELAYCONTROLLER__HPP
