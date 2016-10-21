//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::WireSlave
//
//*************************************************************************************************

#include "Bt/SolarMonitor/WireSlave.hpp"

#include "Bt/Core/Logger.hpp"

namespace Bt {
namespace SolarMonitor {


//-------------------------------------------------------------------------------------------------

WireSlave::WireSlave(Bt::Core::I_Wire& pWire, I_PowerState& pPowerState, I_BatteryState& pBatteryState)
: mWire(&pWire)
, mPowerState(&pPowerState)
, mBatteryState(&pBatteryState)
, mCommandBuffer(IoSlaveCommand::_LastCommand), mBuffer{0} {
   mReceiveHandlers[IoSlaveCommand::SetBatteryState] = &WireSlave::setBatteryState;
   mRequestHandlers[IoSlaveCommand::GetPowerState] = &WireSlave::getPowerState;
}

//-------------------------------------------------------------------------------------------------

WireSlave::~WireSlave() {

}

//-------------------------------------------------------------------------------------------------

uint32_t WireSlave::workcycle() {
   return FOREVER;
}

//-------------------------------------------------------------------------------------------------

void WireSlave::receiveEvent(int numBytes) {
   LOG("receiveEvent " << numBytes);
   if(numBytes < 1 || (numBytes+1) > WIRESLAVE_BUFFER_LENGTH) {
      ERROR("wire receive failed ! 0 <" << numBytes <<  " < " << (WIRESLAVE_BUFFER_LENGTH-1));
      return;
   }
   int c = mWire->read();
   uint8_t rawCommand = c;
   IoSlaveCommand command = IoSlaveCommand(rawCommand);
   if(0 <= command && command < IoSlaveCommand::_LastCommand) {
      ReceiveHandler handler = mReceiveHandlers[command];
      if(handler){
         (this->*handler)(numBytes-1);
      }
   }
   mCommandBuffer = command;
}

//-------------------------------------------------------------------------------------------------

void WireSlave::requestEvent() {
   LOG("requestEvent " << mCommandBuffer);
   IoSlaveCommand command = mCommandBuffer;
   mCommandBuffer = IoSlaveCommand::_LastCommand;
   if(0 <= command && command < IoSlaveCommand::_LastCommand) {
      RequestHandler handler = mRequestHandlers[command];
      if(handler){
         (this->*handler)();
      }
   }

}

//-------------------------------------------------------------------------------------------------

void WireSlave::getPowerState() {
   LOG("PowerState " << mPowerState->state());
   mPowerState->resetNotify();
   mWire->write(static_cast<uint8_t>(mPowerState->state()));
}

//-------------------------------------------------------------------------------------------------

void WireSlave::setBatteryState(int numBytes) {
   if(numBytes != 1) {
      ERROR("wire setBatteryState failed ! " << numBytes <<  " != " << numBytes);
      return;
   }
   uint8_t rawState = mWire->read();
   I_BatteryState::State state = static_cast<I_BatteryState::State>(rawState);
   mBatteryState->state(state);
   LOG("BatteryState update " << state);

}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
