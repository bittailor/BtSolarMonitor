//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::WireSlave
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_WireSlave__hpp
#define INC__Bt_SolarMonitor_WireSlave__hpp

#include <Bt/Core/I_Wire.hpp>
#include <Bt/Core/I_Runnable.hpp>
#include <Bt/SolarMonitor/IoSlaveCommand.hpp>
#include <Bt/SolarMonitor/I_PowerState.hpp>
#include <Bt/SolarMonitor/I_BatteryState.hpp>


namespace Bt {
namespace SolarMonitor {

class WireSlave : public Core::I_Runnable
{
   public:
      WireSlave(Bt::Core::I_Wire& pWire, I_PowerState& pPowerState, I_BatteryState& pBatteryState);
      ~WireSlave();

      void receiveEvent(int numBytes);
      void requestEvent();

      virtual uint32_t workcycle();

   private:
      typedef void (WireSlave::*ReceiveHandler)(int);
      typedef void (WireSlave::*RequestHandler)();

      enum {WIRESLAVE_BUFFER_LENGTH = 20};
      enum {NUMBER_OF_COMMANDS = (int)IoSlaveCommand::_LastCommand};

   	  // Constructor to prohibit copy construction.
      WireSlave(const WireSlave&);

      // Operator= to prohibit copy assignment
      WireSlave& operator=(const WireSlave&);

      void getPowerState();
      void setBatteryState(int numBytes);

      Bt::Core::I_Wire* mWire;
      I_PowerState* mPowerState;
      I_BatteryState* mBatteryState;

      volatile IoSlaveCommand mCommandBuffer;
      volatile uint8_t mBuffer[WIRESLAVE_BUFFER_LENGTH];

      ReceiveHandler mReceiveHandlers[NUMBER_OF_COMMANDS];
      RequestHandler mRequestHandlers[NUMBER_OF_COMMANDS];

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_WireSlave__hpp
