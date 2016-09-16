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
      virtual int disconnect(){return -1;}

      virtual int write(unsigned char* pBuffer, int pLen, int pTimeout){return -1;}
      virtual int read(unsigned char* pBuffer, int pLen, int pTimeout){return -1;}

      virtual int getRSSI() {return -1;}

      virtual void ensureAwake() {};
      virtual void allowSleep() {};


};

class GprsModule : public Core::StateMachine<GprsModuleState,GprsModule>, public I_GprsClient
{
   public:
      enum class State : int {
         OFF = 0,
         PIN_OK = 1,
         NETWORK_REGISTRATION_OK = 2,
         GPRS_READY = 3,
         TCP_CONNECTED = 4,
      };

      struct Settings {
            const char* pin;
            const char* accessPointName;
      };

      class I_Listener {
         public:
            virtual ~I_Listener(){}
            virtual void stateChanged(State pState)=0;
            virtual void onReady()=0;
            virtual void onConnected()=0;
            virtual void onDisconnected()=0;
      };


      GprsModule(Settings pSettings, Core::I_Time& pTime, Core::I_DigitalOut& pOnOffKey, Core::I_DigitalOut& pReset, Core::I_DigitalIn& pPowerState, I_MobileTerminal& pMobileTerminal);
      GprsModule(const GprsModule&) = delete ;
      GprsModule& operator=(const GprsModule&) = delete ;
      ~GprsModule();

      void begin(I_Listener& pListener);

      virtual bool isConnected();

      virtual int connect(const char* pHostname, int pPort);
      virtual int read(unsigned char* pBuffer, int pLen, int pTimeout);
      virtual int write(unsigned char* pBuffer, int pLen, int pTimeout);
      virtual int disconnect();

      virtual void ensureAwake();
      virtual void allowSleep();

      virtual int getRSSI();
      virtual const char* state();

   private:

      class Initial : public StateBase  {
         public:
         Initial(GprsModule& pController):StateBase(pController){}

         virtual const char* name() {
            return "Initial";
         }
         virtual void onEnter() {
            mController->stateChangedCallback(State::OFF);
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

            virtual void onEnter() {
               mController->stateChangedCallback(State::OFF);
            }

      };

      class PoweringOn : public StateBase  {
         public:
            PoweringOn(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {
               mController->stateChangedCallback(State::OFF);
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
               mController->stateChangedCallback(State::OFF);
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
               mController->stateChangedCallback(State::OFF);
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
               mController->stateChangedCallback(State::OFF);
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
               mController->stateChangedCallback(State::OFF);
               mTimer = 10000;
               tryConfigurePin();
            }

            virtual void timeUp(){
               tryConfigurePin();
            }

            void tryConfigurePin() {
               if(mController->mMobileTerminal->checkAndSetPin(mController->mSettings.pin)){
                  mController->nextState(mController->mAwaitNetworkRegistration);
                  return;
               }
               if(mTimer.expired()) {
                  mController->nextState(mController->mReseting);
                  return;
               }
               mController->setTimer(500);
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

            virtual int getRSSI() {
               Return<int> rssi = mController->mMobileTerminal->getRSSI();
               if(rssi) {
                  return rssi.value();
               }
               return -2;
            }

            virtual void onEnter() {
               mController->stateChangedCallback(State::PIN_OK);
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
               mController->setTimer(500);
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

            virtual int getRSSI() {
               Return<int> rssi = mController->mMobileTerminal->getRSSI();
               if(rssi) {
                  return rssi.value();
               }
               return -2;
            }

            virtual void onEnter() {
               mController->stateChangedCallback(State::NETWORK_REGISTRATION_OK);
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
               mController->setTimer(500);
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

            virtual int getRSSI() {
               Return<int> rssi = mController->mMobileTerminal->getRSSI();
               if(rssi) {
                  return rssi.value();
               }
               return -2;
            }


            virtual void onEnter() {
               if(!mController->mMobileTerminal->bringUpWirelessConnection(mController->mSettings.accessPointName)){
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

            virtual int getRSSI() {
               Return<int> rssi = mController->mMobileTerminal->getRSSI();
               if(rssi) {
                  return rssi.value();
               }
               return -2;
            }


            virtual void onEnter() {
               mController->stateChangedCallback(State::GPRS_READY);
               mController->setTimer(0);
            }

            int connect(const char* pHostname, int pPort) {
               LOG("Ready::connect " << pHostname <<  " " <<  pPort);
               Return<void> rc = mController->mMobileTerminal->connect(pHostname, pPort);
               if(rc) {
                  mController->nextState(mController->mConnecting);
                  return 0;
               }
               Return<bool> atOk = mController->mMobileTerminal->checkAtOk();
               if(atOk && atOk.value()) {
                  mController->nextState(mController->mReady);
               } else {
                  mController->nextState(mController->mReseting);
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

            virtual int getRSSI() {
               Return<int> rssi = mController->mMobileTerminal->getRSSI();
               if(rssi) {
                  return rssi.value();
               }
               return -2;
            }


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
                  mController->mConnectFailureCounter = 0;
                  mController->nextState(mController->mConnected);
                  return;
               }
               if(connectionStatus && connectionStatus.value() != ConnectionStatus::CONNECTING){
                  mController->mConnectFailureCounter++;
                  if(mController->mConnectFailureCounter > 5) {
                     mController->mConnectFailureCounter = 0;
                     mController->nextState(mController->mReseting);
                  } else {
                     mController->nextState(mController->mDisconnected);
                  }
                  return;
               }
               if(mTimer.expired()) {
                  mController->mConnectFailureCounter++;
                  if(mController->mConnectFailureCounter > 5) {
                     mController->mConnectFailureCounter = 0;
                     mController->nextState(mController->mReseting);
                  } else {
                     mController->nextState(mController->mDisconnected);
                  }
                  return;
               }
               mController->setTimer(500);
            }

         private:
            Core::Timer mTimer;

      };

      class Connected : public StateBase  {
         public:
            Connected(GprsModule& pController):StateBase(pController){}

            virtual int getRSSI() {
               Return<int> rssi = mController->mMobileTerminal->getRSSI();
               if(rssi) {
                  return rssi.value();
               }
               return -2;
            }


            virtual void onEnter() {
               mController->stateChangedCallback(State::TCP_CONNECTED);
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

            virtual int disconnect(){
               mController->nextState(mController->mDisconnected);
               return 0;
            }

            virtual void ensureAwake() {
               mController->mMobileTerminal->disableSleepMode();
            };

            virtual void allowSleep() {
               mController->mMobileTerminal->enableSleepMode();
            };



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

            virtual int getRSSI() {
               Return<int> rssi = mController->mMobileTerminal->getRSSI();
               if(rssi) {
                  return rssi.value();
               }
               return -2;
            }


            virtual void onEnter() {
               mController->mMobileTerminal->close();
               mController->setTimer(0);
            }

            virtual void timeUp(){
               mController->disconnectedCallback();
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
      void stateChangedCallback(State pState);
      void readyCallback();
      void connectedCallback();
      void disconnectedCallback();


      Settings mSettings;
      uint32_t mConnectFailureCounter;

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
