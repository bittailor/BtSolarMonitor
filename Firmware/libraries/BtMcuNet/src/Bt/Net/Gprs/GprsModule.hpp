//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::GprsModule
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Gprs_GprsModule__hpp
#define INC__Bt_Net_Gprs_GprsModule__hpp

#include <Bt/Core/Logger.hpp>
#include <Bt/Core/Timer.hpp>
#include <Bt/Core/I_DigitalOut.hpp>
#include <Bt/Core/I_DigitalIn.hpp>
#include <Bt/Core/StateMachine.hpp>
#include "Bt/Net/Gprs/I_MobileTerminal.hpp"
#include "Bt/Net/Gprs/I_GprsClient.hpp"

namespace Bt {
namespace Net {
namespace Gprs {

class GprsModuleState {
   public:
      virtual ~GprsModuleState(){}
      virtual bool isConnected() {return false;}
      virtual int connect(const char* pHostname, int pPort){return -1;}
      virtual int write(unsigned char* pBuffer, int pLen, int pTimeout){return -1;}
      virtual int read(unsigned char* pBuffer, int pLen, int pTimeout){return -1;}
};

class GprsModule : public Core::StateMachine<GprsModuleState,GprsModule>, public I_GprsClient
{
   public:
      class I_Listener {
         public:
            virtual ~I_Listener(){}
            virtual void onReady()=0;
            virtual void onConnected()=0;
      };


      GprsModule(Core::I_Time& pTime, Core::I_DigitalOut& pOnOffKey, Core::I_DigitalOut& pReset, Core::I_DigitalIn& pPowerState, I_MobileTerminal& pMobileTerminal);
      GprsModule(const GprsModule&) = delete ;
      GprsModule& operator=(const GprsModule&) = delete ;
      ~GprsModule();

      void begin(I_Listener& pListener);

      virtual bool isConnected();

      virtual int connect(const char* pHostname, int pPort);
      virtual int read(unsigned char* pBuffer, int pLen, int pTimeout);
      virtual int write(unsigned char* pBuffer, int pLen, int pTimeout);
      virtual int disconnect();

   private:

      class Initial : public StateBase  {
         public:
         Initial(GprsModule& pController):StateBase(pController){}

         virtual const char* name() {
            return "Initial";
         }
         virtual void onEnter() {
            if(mController->mPowerState->read()) {
               mController->nextState(mController->mReseting);
            } else {
               mController->nextState(mController->mPoweringOn);
            }
         }
      };

      class Off : public StateBase  {
         public:
            Off(GprsModule& pController):StateBase(pController){}

            virtual const char* name() {
               return "Off";
            }
      };

      class PoweringOn : public StateBase  {
         public:
            PoweringOn(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {
               mController->mOnOffKey->write(false);
               mController->setTimer(3000);
            }

            virtual void timeUp(){
               mController->mOnOffKey->write(true);
               mController->nextState(mController->mWaitForPowerOn);
            }

            virtual const char* name() {
               return "PoweringOn";
            }
      };

      class Reseting : public StateBase  {
         public:
            Reseting(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {
               mController->mReset->write(false);
               mController->setTimer(100);
            }

            virtual void timeUp(){
               mController->mReset->write(true);
               mController->nextState(mController->mWaitForPowerOn);
            }

            virtual const char* name() {
               return "Reseting";
            }
      };

      class WaitForPowerOn : public StateBase {
         public:
            WaitForPowerOn(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {
               mTimer = 10000;
               check();
            }

            virtual void timeUp(){
               check();
            }

            virtual const char* name() {
               return "WaitForPowerOn";
            }

            void check(){
               if(mController->mPowerState->read()) {
                  mController->nextState(mController->mSyncAt);
                  return;
               }
               if(mTimer.expired()) {
                  mController->nextState(mController->mReseting);
                  return;
               }
               mController->setTimer(0);
            }
         private:
            Core::Timer mTimer;
      };

      class SyncAt : public StateBase  {
         public:
            SyncAt(GprsModule& pController):StateBase(pController), mAtOkCounter(0){}

            virtual void onEnter() {
               mCounter = 0;
               mAtOkCounter = 0;
               ceckAtOk();
            }

            virtual void timeUp(){
               ceckAtOk();
            }

            virtual const char* name() {
               return "SyncAt";
            }

            void ceckAtOk() {
               mCounter++;
               Return<bool> atOk = mController->mMobileTerminal->checkAtOk();
               if(atOk && atOk.value()) {
                  mAtOkCounter++;
                  if(mAtOkCounter > 3){
                     mController->mMobileTerminal->disableEcho();
                     if(!mController->mMobileTerminal->disableEcho()) {
                        mController->nextState(mController->mReseting);
                        return;
                     }
                     mController->nextState(mController->mConfigurePin);
                     return;
                  }
               }
               if(mCounter > 20) {
                  mController->nextState(mController->mReseting);
                  return;
               }
               mController->setTimer(10);
            }

         private:
            int mCounter;
            int mAtOkCounter;
      };

      class ConfigurePin : public StateBase  {
         public:
            ConfigurePin(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {
               mTimer = 10000;
               tryConfigurePin();
            }

            virtual void timeUp(){
               tryConfigurePin();
            }

            void tryConfigurePin() {
               if(mController->mMobileTerminal->checkAndSetPin("1210")){
                  mController->nextState(mController->mAwaitNetworkRegistration);
                  return;
               }
               if(mTimer.expired()) {
                  mController->nextState(mController->mReseting);
                  return;
               }
               mController->setTimer(100);
            }

            virtual const char* name() {
               return "mConfigurePin";
            }

         private:
            Core::Timer mTimer;
      };

      class AwaitNetworkRegistration : public StateBase  {
         public:
            AwaitNetworkRegistration(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {
               mTimer = 60000;
               checkNetworkRegistration();
            }

            virtual void timeUp(){
               checkNetworkRegistration();
            }

            void checkNetworkRegistration() {
               Return<bool> networkRegistration = mController->mMobileTerminal->checkNetworkRegistration();
               if(networkRegistration && networkRegistration.value()){
                  mController->nextState(mController->mAwaitGprsAttachment);
                  return;
               }
               if(mTimer.expired()) {
                  mController->nextState(mController->mReseting);
                  return;
               }
               mController->setTimer(100);
            }

            virtual const char* name() {
               return "AwaitNetworkRegistration";
            }
         private:
            Core::Timer mTimer;
      };

      class AwaitGprsAttachment : public StateBase  {
         public:
            AwaitGprsAttachment(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {
               mTimer = 10000;
               checkGprsAttachment();
            }

            virtual void timeUp(){
               checkGprsAttachment();
            }

            void checkGprsAttachment() {
               Return<bool> gprsAttachment = mController->mMobileTerminal->checkGprsAttachment();
               if(gprsAttachment && gprsAttachment.value()){
                  mController->nextState(mController->mBringUpWirelessConnection);
                  return;
               }
               if(mTimer.expired()) {
                  mController->nextState(mController->mReseting);
                  return;
               }
               mController->setTimer(100);
            }

            virtual const char* name() {
               return "AwaitGprsAttachment";
            }
         private:
            Core::Timer mTimer;
      };

      class BringUpWirelessConnection : public StateBase  {
         public:
            BringUpWirelessConnection(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {
               if(!mController->mMobileTerminal->bringUpWirelessConnection("gprs.swisscom.ch")){
                  mController->nextState(mController->mReseting);
                  return;
               }
               mController->nextState(mController->mReady);
            }

            virtual const char* name() {
               return "BringUpWirelessConnection";
            }
      };

      class Ready : public StateBase  {
         public:
            Ready(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {
               mController->setTimer(0);
            }

            int connect(const char* pHostname, int pPort) {
               LOG("Ready::connect " << pHostname <<  " " <<  pPort);
               Return<void> rc = mController->mMobileTerminal->connect(pHostname, pPort);
               if(rc) {
                  mController->nextState(mController->mConnecting);
                  return 0;
               }
               return -1;
            }

            virtual void timeUp(){
               mController->readyCallback();
            }

            virtual const char* name() {
               return "Ready";
            }
      };

      class Connecting : public StateBase  {
         public:
            Connecting(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {
               mTimer = 160000;
               checkConnected();
            }

            virtual void timeUp(){
               checkConnected();
            }

            virtual const char* name() {
               return "Connecting";
            }

            void checkConnected() {
               Return<ConnectionStatus> connectionStatus = mController->mMobileTerminal->getConnectionStatus();
               if(connectionStatus && connectionStatus.value() == ConnectionStatus::CONNECT_OK){
                  mController->nextState(mController->mConnected);
                  return;
               }
               if(mTimer.expired()) {
                  mController->nextState(mController->mReady);
                  return;
               }
               mController->setTimer(100);
            }

         private:
            Core::Timer mTimer;

      };

      class Connected : public StateBase  {
         public:
            Connected(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {
               mController->setTimer(0);
            }

            virtual bool isConnected() {
               Return<ConnectionStatus> connectionStatus = mController->mMobileTerminal->getConnectionStatus();
               if(connectionStatus.state() == ReturnCode::RC_TIMEOUT) {
                  connectionStatus = mController->mMobileTerminal->getConnectionStatus();
               }
               if(connectionStatus && connectionStatus.value() == ConnectionStatus::CONNECT_OK){
                  return true;
               }
               mController->nextState(mController->mDisconnected);
               return false;
            }

            virtual int write(unsigned char* pBuffer, int pLen, int pTimeout) {
               Return<int> rc = mController->mMobileTerminal->write(pBuffer, pLen, pTimeout);
               if(rc) {
                  return rc.value();
               }
               isConnected();
               return -1;
            }

            virtual int read(unsigned char* pBuffer, int pLen, int pTimeout) {
               Return<int> rc = mController->mMobileTerminal->read(pBuffer, pLen, pTimeout);
               if(rc) {
                  return rc.value();
               }
               isConnected();
               return -1;
            }

            virtual void timeUp(){
               mController->connectedCallback();
            }

            virtual const char* name() {
               return "Connected";
            }
      };

      class Disconnected : public StateBase  {
         public:
            Disconnected(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {
               mController->setTimer(0);
            }

            virtual void timeUp(){
               Return<bool> atOk = mController->mMobileTerminal->checkAtOk();
               if(atOk && atOk.value()) {
                  mController->nextState(mController->mReady);
               } else {
                  mController->nextState(mController->mReseting);
               }
            }

            virtual const char* name() {
               return "Disconnected";
            }
      };



      class Dummy : public StateBase  {
         public:
            Dummy(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {
            }

            virtual void timeUp(){
            }

            virtual const char* name() {
               return "Dummy";
            }
      };


      virtual const char* name();
      void readyCallback();
      void connectedCallback();

      Core::I_DigitalOut* mOnOffKey;
      Core::I_DigitalOut* mReset;
      Core::I_DigitalIn* mPowerState;

      I_MobileTerminal* mMobileTerminal;

      Initial mInitial;
      Off mOff;
      PoweringOn mPoweringOn;
      Reseting mReseting;
      WaitForPowerOn mWaitForPowerOn;
      SyncAt mSyncAt;
      ConfigurePin mConfigurePin;
      AwaitNetworkRegistration mAwaitNetworkRegistration;
      AwaitGprsAttachment mAwaitGprsAttachment;
      BringUpWirelessConnection mBringUpWirelessConnection;
      Ready mReady;
      Connecting mConnecting;
      Connected mConnected;
      Disconnected mDisconnected;

      Dummy mDummy;

      I_Listener* mListener;





};

} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_GprsModule__hpp
