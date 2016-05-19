//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::SlaveController
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_SlaveController__hpp
#define INC__Bt_SolarMonitor_SlaveController__hpp

#include "Bt/Core/Wire.hpp"
#include "Bt/Core/DigitalIn.hpp"
#include "Bt/Core/DigitalOut.hpp"
#include "Bt/Core/Time.hpp"
#include "Bt/Core/PushButton.hpp"
#include "Bt/Core/PushButtonListener.hpp"
#include "Bt/Core/Workcycle.hpp"

#include "Bt/SolarMonitor/LatchingRelay.hpp"
#include "Bt/SolarMonitor/RelayController.hpp"
#include "Bt/SolarMonitor/RelayControllerQueryPort.hpp"
#include "Bt/SolarMonitor/PowerState.hpp"
#include "Bt/SolarMonitor/WireSlave.hpp"

namespace Bt {
namespace SolarMonitor {

class SlaveController
{
   public:
      SlaveController();
      ~SlaveController();

      void begin();
      uint32_t oneWorkcycle();

      Core::I_PushButton& getOnOffButton() {return mOnOffButton;}
      Core::I_PushButton& getABButton() {return mABButton;}
      WireSlave& getWireSlave() {return mWireSlave;}


   private:
   	  // Constructor to prohibit copy construction.
      SlaveController(const SlaveController&);

      // Operator= to prohibit copy assignment
      SlaveController& operator=(const SlaveController&);

      void toggleOnOff();
      void toggleAB();

      Core::Workcycle mWorkcycle;

      Core::Time mTime;

      Core::DigitalOut mRelayAToOn;
      Core::DigitalOut mRelayAToOff;
      Core::DigitalOut mRelayBToOn;
      Core::DigitalOut mRelayBToOff;
      Core::DigitalOut mRelayLoadToOn;
      Core::DigitalOut mRelayLoadToOff;

      Core::DigitalOut mLedA;
      Core::DigitalOut mLedB;
      Core::DigitalOut mNotify;

      Core::DigitalIn mOnOff;
      Core::DigitalIn mAB;

      RelayControllerQueryPort mRelayControllerQueryPort;

      LatchingRelay mRelayA;
      LatchingRelay mRelayB;
      LatchingRelay mRelayLoad;
      PowerState mPowerState;

      RelayController mRelayController;

      Core::PushButtonListener<SlaveController> mOnOffButtonListener;
      Core::PushButtonListener<SlaveController> mABButtonListener;

      Core::PushButton mOnOffButton;
      Core::PushButton mABButton;

      Core::WireWrapper mWire;
      WireSlave mWireSlave;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_SlaveController__hpp
