//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::SlaveController
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_SlaveController__hpp
#define INC__Bt_SolarMonitor_SlaveController__hpp

#include "Bt/SolarMonitor/RelayController.hpp"

#include "Bt/Core/DigitalIn.hpp"
#include "Bt/Core/DigitalOut.hpp"
#include "Bt/Core/Time.hpp"
#include "Bt/Core/PushButton.hpp"
#include "Bt/Core/PushButtonListener.hpp"
#include "Bt/SolarMonitor/LatchingRelay.hpp"
#include "Bt/SolarMonitor/RelayControllerQueryPort.hpp"
#include "Bt/SolarMonitor/StateLeds.hpp"

namespace Bt {
namespace SolarMonitor {

class SlaveController
{
   public:
      SlaveController();
      ~SlaveController();

      void begin();
      bool loop();

      Core::I_PushButton& getOnOffButton() {return mOnOffButton;}
      Core::I_PushButton& getABButton() {return mABButton;}


   private:
   	  // Constructor to prohibit copy construction.
      SlaveController(const SlaveController&);

      // Operator= to prohibit copy assignment
      SlaveController& operator=(const SlaveController&);

      void toggleOnOff();
      void toggleAB();


      Core::Time mTime;

      Core::DigitalOut mRelayAToOn;
      Core::DigitalOut mRelayAToOff;
      Core::DigitalOut mRelayBToOn;
      Core::DigitalOut mRelayBToOff;
      Core::DigitalOut mRelayLoadToOn;
      Core::DigitalOut mRelayLoadToOff;

      Core::DigitalOut mLedA;
      Core::DigitalOut mLedB;

      Core::DigitalIn mOnOff;
      Core::DigitalIn mAB;

      RelayControllerQueryPort mRelayControllerQueryPort;

      LatchingRelay mRelayA;
      LatchingRelay mRelayB;
      LatchingRelay mRelayLoad;
      StateLeds mStateLeds;

      RelayController mRelayController;

      Core::PushButtonListener<SlaveController> mOnOffButtonListener;
      Core::PushButtonListener<SlaveController> mABButtonListener;

      Core::PushButton mOnOffButton;
      Core::PushButton mABButton;


};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_SlaveController__hpp
