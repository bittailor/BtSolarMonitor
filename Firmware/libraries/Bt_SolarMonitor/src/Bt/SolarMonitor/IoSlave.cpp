//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::IoSlave
//
//*************************************************************************************************

#include "Bt/SolarMonitor/IoSlave.hpp"

#include "Bt/Core/Logger.hpp"
#include "Bt/Core/Timing.hpp"

#include "Bt/SolarMonitor/IoSlaveCommand.hpp"

#define IO_SLAVE_ADDRESS 0x8

namespace Bt {
namespace SolarMonitor {


//-------------------------------------------------------------------------------------------------

IoSlave::IoSlave(Core::I_Wire& pWire) :mWire(&pWire) {

}

//-------------------------------------------------------------------------------------------------

IoSlave::~IoSlave() {

}

//-------------------------------------------------------------------------------------------------

I_PowerState::State IoSlave::powerState() {
   mWire->beginTransmission(IO_SLAVE_ADDRESS);
   mWire->write(IoSlaveCommand::GetPowerState);
   mWire->endTransmission();
   Core::delayInMilliseconds(1);
   size_t requestSize = 1;
   size_t receivedSize = mWire->requestFrom(IO_SLAVE_ADDRESS, requestSize);
   if(requestSize != receivedSize) {
      ERROR("IoSlave::powerState Size missmatch  " << requestSize << " != " << receivedSize);
      return I_PowerState::Off;
   }
   uint8_t rawState = mWire->read();
   return static_cast<I_PowerState::State>(rawState);
}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
