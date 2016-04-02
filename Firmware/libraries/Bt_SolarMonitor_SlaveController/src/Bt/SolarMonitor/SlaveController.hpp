//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::SlaveController
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_SlaveController__hpp
#define INC__Bt_SolarMonitor_SlaveController__hpp

#include "Bt/SolarMonitor/RelayController.hpp"

#include "Bt/Core/Time.hpp"
#include "Bt/SolarMonitor/LatchingRelay.hpp"
#include "Bt/SolarMonitor/StateLeds.hpp"
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

      void toggleOnOff();
      void toggleAB();


   private:
   	  // Constructor to prohibit copy construction.
      SlaveController(const SlaveController&);

      // Operator= to prohibit copy assignment
      SlaveController& operator=(const SlaveController&);

      Core::Time mTime;

      Core::DigitalOut mRelayAToOn;
      Core::DigitalOut mRelayAToOff;
      Core::DigitalOut mRelayBToOn;
      Core::DigitalOut mRelayBToOff;
      Core::DigitalOut mRelayLoadToOn;
      Core::DigitalOut mRelayLoadToOff;

      Core::DigitalOut mLedA;
      Core::DigitalOut mLedB;

      RelayControllerQueryPort mRelayControllerQueryPort;

      LatchingRelay mRelayA;
      LatchingRelay mRelayB;
      LatchingRelay mRelayLoad;
      StateLeds mStateLeds;

      RelayController mRelayController;


};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_SlaveController__hpp
