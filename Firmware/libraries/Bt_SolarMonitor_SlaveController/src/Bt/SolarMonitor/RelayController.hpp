//*************************************************************************************************
//
//  BITTAILOR.CH -
//
//*************************************************************************************************

#ifndef INC__BT_SOLARMONITOR_RELAYCONTROLLER__HPP
#define INC__BT_SOLARMONITOR_RELAYCONTROLLER__HPP

#include <Bt/Core/I_DigitalIn.hpp>
#include <Bt/Core/I_InterruptIn.hpp>
#include <Bt/Core/I_DigitalOut.hpp>
#include <Bt/Core/StateMachine.hpp>

#include <Bt/SolarMonitor/I_RelayController.hpp>
#include <Bt/SolarMonitor/I_RelayControllerActionPort.hpp>
#include <Bt/SolarMonitor/I_RelayControllerQueryPort.hpp>

namespace Bt {
namespace SolarMonitor {



class RelayController : public Core::StateMachine<I_RelayController, RelayController>
{
   public:

      RelayController(Core::I_Time& pTime, I_RelayControllerActionPort& pActionPort, I_RelayControllerQueryPort& pQueryPort);
      ~RelayController();

      void begin();

   protected:
      virtual const char* name();
   
   private:

      static const uint32_t RELAY_ENERGIZATION_DURATION = 50;

      class Initial : public StateBase  {
         public:
            Initial(RelayController& pController):StateBase(pController){}

            virtual const char* name() {
               return "Initial";
            }

            virtual void onEnter() {
               if(mController->mQueryPort->loadASense() && mController->mQueryPort->loadBSense()) {
                  mController->nextState(mController->mOff);
               } else if (mController->mQueryPort->loadASense()) {
                  mController->nextState(mController->mOnB);
               } else {
                  mController->nextState(mController->mOnA);
               }
            }
      };

      class Off : public StateBase  {
         public:
            Off(RelayController& pController) :StateBase(pController){}

            virtual void onEnter() {
               mController->mActionPort->publicState(I_RelayController::Off);
            }

            virtual const char* name() {
               return "Off";
            }

            virtual void toggleOnOff(){
               toOn();
            }

            virtual void toOn(){
               if(mController->mQueryPort->isBatteryABetter()){
                  mController->nextState(mController->mToggleAOn);
               } else {
                  mController->nextState(mController->mToggleBOn);
               }
            }
      };

      class OnA : public StateBase {
         public:
            OnA(RelayController& pController) :StateBase(pController){}

            virtual const char* name() {
               return "OnA";
            }

            virtual void onEnter() {
               if(!mController->mQueryPort->loadOut()){
                  mController->nextState(mController->mToggleLoadOn);
               } else {
                  mController->mActionPort->publicState(I_RelayController::OnA);
               }
            }

            virtual void onExit() {
            }

            virtual void toggleOnOff() {
               toOff();
            }

            virtual void toggleAB() {
               toB();
            }

            virtual void toOff() {
               mController->nextState(mController->mToggleAOff);
            }

            virtual void toB() {
               mController->nextState(mController->mSwitchToB);
            }

      };

      class OnB : public StateBase {
         public:
            OnB(RelayController& pController) :StateBase(pController){}

            virtual const char* name() {
               return "OnB";
            }

            virtual void onEnter() {
               if(!mController->mQueryPort->loadOut()){
                  mController->nextState(mController->mToggleLoadOn);
               } else {
                  mController->mActionPort->publicState(I_RelayController::OnB);
               }
            }

            virtual void onExit() {
            }

            virtual void toggleOnOff() {
               toOff();
            }

            virtual void toggleAB() {
               toA();
            }

            virtual void toOff() {
               mController->nextState(mController->mToggleBOff);
            }

            virtual void toA() {
               mController->nextState(mController->mSwitchToA);
            }
      };

      class ToggleAOn : public StateBase  {
         public:
            ToggleAOn(RelayController& pController) :StateBase(pController){}

            virtual const char* name() {
               return "ToggleAOn";
            }

            virtual void onEnter(){
               mController->mActionPort->relayAToOn(true);
               mController->setTimer(RELAY_ENERGIZATION_DURATION);
            }

            virtual void timeUp(){
               mController->mActionPort->relayAToOn(false);
               mController->nextState(mController->mOnA);
            }
      };

      class ToggleAOff : public StateBase  {
         public:
            ToggleAOff(RelayController& pController) :StateBase(pController){}

            virtual const char* name() {
               return "ToggleAOff";
            }

            virtual void onEnter(){
               mController->mActionPort->relayAToOff(true);
               mController->setTimer(RELAY_ENERGIZATION_DURATION);
            }

            virtual void timeUp(){
               mController->mActionPort->relayAToOff(false);
               mController->nextState(mController->mOff);
            }
      };

      class SwitchToA : public StateBase  {
         public:
            SwitchToA(RelayController& pController) :StateBase(pController){}

            virtual const char* name() {
               return "SwitchToA";
            }

            virtual void onEnter(){
               mController->mActionPort->relayBToOff(true);
               mController->setTimer(RELAY_ENERGIZATION_DURATION);
            }

            virtual void timeUp(){
               mController->mActionPort->relayBToOff(false);
               mController->nextState(mController->mToggleAOn);
            }
      };

      class ToggleBOn : public StateBase  {
         public:
            ToggleBOn(RelayController& pController) :StateBase(pController){}

            virtual const char* name() {
               return "ToggleBOn";
            }

            virtual void onEnter(){
               mController->mActionPort->relayBToOn(true);
               mController->setTimer(RELAY_ENERGIZATION_DURATION);
            }

            virtual void timeUp(){
               mController->mActionPort->relayBToOn(false);
               mController->nextState(mController->mOnB);
            }
      };

      class ToggleBOff : public StateBase  {
         public:
            ToggleBOff(RelayController& pController) :StateBase(pController){}

            virtual const char* name() {
               return "ToggleBOff";
            }

            virtual void onEnter(){
               mController->mActionPort->relayBToOff(true);
               mController->setTimer(RELAY_ENERGIZATION_DURATION);
            }

            virtual void timeUp(){
               mController->mActionPort->relayBToOff(false);
               mController->nextState(mController->mOff);
            }
      };

      class SwitchToB : public StateBase  {
         public:
            SwitchToB(RelayController& pController) :StateBase(pController){}

            virtual const char* name() {
               return "SwitchToB";
            }

            virtual void onEnter(){
               mController->mActionPort->relayAToOff(true);
               mController->setTimer(RELAY_ENERGIZATION_DURATION);
            }

            virtual void timeUp(){
               mController->mActionPort->relayAToOff(false);
               mController->nextState(mController->mToggleBOn);
            }
      };

      class ToggleLoadOn : public StateBase  {
         public:
            ToggleLoadOn(RelayController& pController) :StateBase(pController){}

            virtual const char* name() {
               return "ToggleLoadOn";
            }

            virtual void onEnter(){
               mController->mActionPort->relayLoadToOn(true);
               mController->setTimer(RELAY_ENERGIZATION_DURATION);
            }

            virtual void timeUp(){
               mController->mActionPort->relayLoadToOn(false);
               if(mController->mQueryPort->loadASense()) {
                  mController->nextState(mController->mOnB);
               } else {
                  mController->nextState(mController->mOnA);
               }
            }
      };


   	// Constructor to prohibit copy construction.
      RelayController(const RelayController&);

      // Operator= to prohibit copy assignment
      RelayController& operator=(const RelayController&);

      I_RelayControllerActionPort* mActionPort;
      I_RelayControllerQueryPort* mQueryPort;

      Initial mInitial;
      Off mOff;
      OnA mOnA;
      OnB mOnB;
      ToggleAOn mToggleAOn;
      ToggleAOff mToggleAOff;
      SwitchToA mSwitchToA;
      ToggleBOn mToggleBOn;
      ToggleBOff mToggleBOff;
      SwitchToB mSwitchToB;
      ToggleLoadOn mToggleLoadOn;

      //Core::StateMachine<State> mStateMachine;

};


/*
    Core::I_InterruptIn* mButtonOnOff;
    Core::I_InterruptIn* mButtonAB;

    Core::I_DigitalIn* mLoadASense;
    Core::I_DigitalIn* mLoadBSense;
    Core::I_DigitalIn* mLoadOut;

    Core::I_DigitalOut* mRelayAToOn;
    Core::I_DigitalOut* mRelayAToOff;
    Core::I_DigitalOut* mRelayBToOn;
    Core::I_DigitalOut* mRelayBToOff;
    Core::I_DigitalOut* mRelayLoadToOn;
    Core::I_DigitalOut* mRelayLoadToOff;
    */

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__BT_SOLARMONITOR_RELAYCONTROLLER__HPP
