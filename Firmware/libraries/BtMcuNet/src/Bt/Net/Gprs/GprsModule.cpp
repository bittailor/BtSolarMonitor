//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::GprsModule
//
//*************************************************************************************************

#include "Bt/Net/Gprs/GprsModule.hpp"

namespace Bt {
namespace Net {
namespace Gprs {


//-------------------------------------------------------------------------------------------------

GprsModule::GprsModule(Core::I_Time& pTime, Core::I_DigitalOut& pOnOffKey, Core::I_DigitalOut& pReset, Core::I_DigitalIn& pPowerState, I_MobileTerminal& pMobileTerminal)
: Core::StateMachine<GprsModuleState,GprsModule>(pTime)
, mOnOffKey(&pOnOffKey)
, mReset(&pReset)
, mPowerState(&pPowerState)
, mMobileTerminal(&pMobileTerminal)
, mInitial(*this)
, mOff(*this)
, mPoweringOn(*this)
, mReseting(*this)
, mWaitForPowerOn(*this)
, mSyncAt(*this)
, mConfigure(*this) {

}

//-------------------------------------------------------------------------------------------------

GprsModule::~GprsModule() {

}

//-------------------------------------------------------------------------------------------------

void GprsModule::begin() {
   init(mInitial);
}

//-------------------------------------------------------------------------------------------------

const char* GprsModule::name() {
   return "GM";
}

//-------------------------------------------------------------------------------------------------

} // namespace Gprs
} // namespace Net
} // namespace Bt
