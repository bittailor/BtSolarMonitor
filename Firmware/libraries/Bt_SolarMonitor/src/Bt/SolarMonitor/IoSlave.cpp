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

IoSlave::IoSlave(Core::I_Wire& pWire, Core::I_DigitalIn& pNotify)
: mWire(&pWire)
, mNotify(&pNotify)
, mStateCache() {

}

//-------------------------------------------------------------------------------------------------

IoSlave::~IoSlave() {

}

//-------------------------------------------------------------------------------------------------

void IoSlave::begin() {
   updatePowerState();
}

//-------------------------------------------------------------------------------------------------

bool IoSlave::loop() {
   if(!mNotify->read()) {
      updatePowerState();
      return true;
   }
   return false;
}


//-------------------------------------------------------------------------------------------------

I_PowerState::State IoSlave::powerState() {
   return mStateCache;
}

//-------------------------------------------------------------------------------------------------

void IoSlave::batteryState(I_BatteryState::State pState) {
   mWire->beginTransmission(IO_SLAVE_ADDRESS);
   mWire->write(IoSlaveCommand::SetBatteryState);
   mWire->write(static_cast<uint8_t>(pState));
   mWire->endTransmission();
}

//-------------------------------------------------------------------------------------------------

void IoSlave::updatePowerState() {
   mWire->beginTransmission(IO_SLAVE_ADDRESS);
   mWire->write(IoSlaveCommand::GetPowerState);
   mWire->endTransmission();
   Core::delayInMilliseconds(1);
   size_t requestSize = 1;
   size_t receivedSize = mWire->requestFrom(IO_SLAVE_ADDRESS, requestSize);
   if(requestSize != receivedSize) {
      ERROR("IoSlave::powerState Size missmatch  " << requestSize << " != " << receivedSize);
      mStateCache = I_PowerState::Off;
      return;
   }
   uint8_t rawState = mWire->read();
   mStateCache = static_cast<I_PowerState::State>(rawState);
}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
