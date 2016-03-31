//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::SlaveController
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_SlaveController__hpp
#define INC__Bt_SolarMonitor_SlaveController__hpp

#include "Bt/SolarMonitor/RelayController.hpp"

#include "Bt/Core/Time.hpp"
#include "Bt/SolarMonitor/RelayControllerActionPort.hpp"
#include "Bt/SolarMonitor/RelayControllerQueryPort.hpp"

namespace Bt {
namespace SolarMonitor {

class SlaveController
{
   public:
      SlaveController();
      ~SlaveController();

      void begin();
      void loop();


   private:
   	  // Constructor to prohibit copy construction.
      SlaveController(const SlaveController&);

      // Operator= to prohibit copy assignment
      SlaveController& operator=(const SlaveController&);

      Core::Time mTime;
      RelayControllerActionPort mRelayControllerActionPort;
      RelayControllerQueryPort mRelayControllerQueryPort;
      RelayController mRelayController;


};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_SlaveController__hpp
