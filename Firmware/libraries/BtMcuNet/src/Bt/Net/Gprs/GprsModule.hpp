//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::GprsModule
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Gprs_GprsModule__hpp
#define INC__Bt_Net_Gprs_GprsModule__hpp

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
               mController->setTimer(2000);
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
               mAtOkCounter = 0;
               mController->mMobileTerminal->checkAtOk(Core::Function<void(bool)>::build<SyncAt,&SyncAt::AtOkCallback>(*this));
               mController->setTimer(10000);
            }

            virtual void timeUp(){
               //mController->nextState(mController->mReseting);
            }

            virtual const char* name() {
               return "SyncAt";
            }

            void AtOkCallback(bool pOk) {
               if(pOk) {
                  mAtOkCounter++;
                  if(mAtOkCounter > 3){
                     mController->resetTimer();
                     mController->nextState(mController->mConfigure);
                     return;
                  }
               }
               mAtOkCounter = 0;
               mController->mMobileTerminal->checkAtOk(Core::Function<void(bool)>::build<SyncAt,&SyncAt::AtOkCallback>(*this));
            }

         private:
            int mAtOkCounter;
      };

      class Configure : public StateBase  {
         public:
            Configure(GprsModule& pController):StateBase(pController){}

            virtual void onEnter() {

            }

            virtual const char* name() {
               return "Configure";
            }

         private:
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
      Configure mConfigure;



};

} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_GprsModule__hpp
