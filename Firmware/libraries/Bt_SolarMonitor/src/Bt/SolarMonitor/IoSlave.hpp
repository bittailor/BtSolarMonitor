//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::IoSlave
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_IoSlave__hpp
#define INC__Bt_SolarMonitor_IoSlave__hpp

#include "Bt/Core/I_Wire.hpp"
#include "Bt/Core/I_DigitalIn.hpp"

#include "Bt/SolarMonitor/I_IoSlave.hpp"

namespace Bt {
namespace SolarMonitor {

class IoSlave : public I_IoSlave
{
   public:
      IoSlave(Core::I_Wire& pWire, Core::I_DigitalIn& pNotify);
      ~IoSlave();

      void begin();
      bool loop();

      virtual I_PowerState::State powerState();
      virtual void batteryState(I_BatteryState::State pState);

   private:
   	  // Constructor to prohibit copy construction.
      IoSlave(const IoSlave&);

      // Operator= to prohibit copy assignment
      IoSlave& operator=(const IoSlave&);

      void updatePowerState();

      Core::I_Wire* mWire;
      Core::I_DigitalIn* mNotify;
      I_PowerState::State mStateCache;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_IoSlave__hpp
