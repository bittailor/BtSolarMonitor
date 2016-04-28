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

WireSlave::WireSlave(Bt::Core::I_Wire& pWire, I_PowerState& pPowerState)
: mWire(&pWire)
, mPowerState(&pPowerState)
, mCommandBuffer(IoSlaveCommand::_LastCommand), mBuffer{0} {
   mRequestHandlers[IoSlaveCommand::GetPowerState] = &WireSlave::getPowerState;
}

//-------------------------------------------------------------------------------------------------

WireSlave::~WireSlave() {

}

//-------------------------------------------------------------------------------------------------

bool WireSlave::loop() {
   return false;
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

} // namespace SolarMonitor
} // namespace Bt
