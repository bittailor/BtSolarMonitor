//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::GprsModule
//
//*************************************************************************************************

#include "Bt/Net/Gprs/GprsModule.hpp"

#include <Bt/Core/Logger.hpp>

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
, mConfigurePin(*this)
, mAwaitNetworkRegistration(*this)
, mAwaitGprsAttachment(*this)
, mBringUpWirelessConnection(*this)
, mReady(*this)
, mConnecting(*this)
, mConnected(*this)
, mDummy(*this)
, mListener(nullptr){

}

//-------------------------------------------------------------------------------------------------

GprsModule::~GprsModule() {

}

//-------------------------------------------------------------------------------------------------

void GprsModule::begin(I_Listener& pListener) {
   mListener = &pListener;
   mOnOffKey->write(true);
   mReset->write(true);
   init(mInitial);
}

//-------------------------------------------------------------------------------------------------

bool GprsModule::isConnected() {
   return handle(&GprsModuleState::isConnected);
}

//-------------------------------------------------------------------------------------------------

int GprsModule::connect(const char* pHostname, int pPort) {
   return handle(&GprsModuleState::connect,pHostname,pPort);
}

//-------------------------------------------------------------------------------------------------

int GprsModule::read(unsigned char* pBuffer, int pLen, int pTimeout) {
   int rc = handle(&GprsModuleState::read,pBuffer, pLen, pTimeout);
   LOG("GprsModule::read " << pLen << " => " <<  rc);
   return rc;
}

//-------------------------------------------------------------------------------------------------

int GprsModule::write(unsigned char* pBuffer, int pLen, int pTimeout) {
   int rc = handle(&GprsModuleState::write,pBuffer, pLen, pTimeout);
   LOG("GprsModule::write " << pLen << " => " <<  rc);
   return rc;
}
//-------------------------------------------------------------------------------------------------

int GprsModule::disconnect() {
   return -1;
}

//-------------------------------------------------------------------------------------------------

const char* GprsModule::name() {
   return "GM";
}

//-------------------------------------------------------------------------------------------------

void GprsModule::readyCallback() {
   if(mListener != nullptr) {
      mListener->onReady();
   }
}

//-------------------------------------------------------------------------------------------------

void GprsModule::connectedCallback() {
   if(mListener != nullptr) {
      mListener->onConnected();
   }
}

//-------------------------------------------------------------------------------------------------

} // namespace Gprs
} // namespace Net
} // namespace Bt
