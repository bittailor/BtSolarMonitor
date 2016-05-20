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

namespace Bt {
namespace Net {
namespace Gprs {

class GprsModuleState {
   public:
      virtual ~GprsModuleState(){}
};

class GprsModule : public Core::StateMachine<GprsModuleState,GprsModule>
{
   public:
      GprsModule(Core::I_Time& pTime, Core::I_DigitalOut& pOnOffKey, Core::I_DigitalOut& pReset, Core::I_DigitalIn& pPowerState, I_MobileTerminal& pMobileTerminal);
      GprsModule(const GprsModule&) = delete ;
      GprsModule& operator=(const GprsModule&) = delete ;
      ~GprsModule();

      void begin();

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
               }
               mController->setTimer(0);
            }
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
               if(mController->mMobileTerminal->checkAtOk()) {
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
               mController->setTimer(100);
            }

            virtual const char* name() {
               return "mConfigurePin";
            }

         private:
      };

      class AwaitNetworkRegistration : public StateBase  {
         public:
            AwaitNetworkRegistration(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {
               checkNetworkRegistration();
            }

            virtual void timeUp(){
               checkNetworkRegistration();
            }

            void checkNetworkRegistration() {
               if(mController->mMobileTerminal->checkNetworkRegistration()){
                  mController->nextState(mController->mAwaitGprsAttachment);
                  return;
               }
               mController->setTimer(100);
            }

            virtual const char* name() {
               return "AwaitNetworkRegistration";
            }
      };

      class AwaitGprsAttachment : public StateBase  {
         public:
            AwaitGprsAttachment(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {
               checkGprsAttachment();
            }

            virtual void timeUp(){
               checkGprsAttachment();
            }

            void checkGprsAttachment() {
               if(mController->mMobileTerminal->checkGprsAttachment()){
                  mController->nextState(mController->mDummy);
                  return;
               }
               mController->setTimer(100);
            }

            virtual const char* name() {
               return "AwaitGprsAttachment";
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

      Dummy mDummy;



};

} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_GprsModule__hpp
