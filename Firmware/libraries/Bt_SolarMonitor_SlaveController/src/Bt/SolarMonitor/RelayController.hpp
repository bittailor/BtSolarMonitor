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



class RelayController : public I_RelayController
{
   public:



      RelayController(I_RelayControllerActionPort& pActionPort, I_RelayControllerQueryPort& pQueryPort);
      ~RelayController();


   
   private:

      static const uint32_t RELAY_ENERGIZATION_DURATION = 50;

      class State {
         public:
            State(RelayController& pController) :mController(&pController){}
            virtual ~State(){}
            virtual void onEnter(){}
            virtual void onExit(){}

            virtual void evToggleOnOff(){}
            virtual void evToggleAB(){}
            virtual void evToOn(){}
            virtual void evToOff(){}
            virtual void evToA(){}
            virtual void evToB(){}
            virtual void evTimeUp(){}
         protected:
            RelayController* mController;
      };

      class Initial : public State  {
         public:
            Initial(RelayController& pController):State(pController){}
            virtual void onEnter();
      };

      class Off_A : public State  {
         public:
            Off_A(RelayController& pController) :State(pController){}
      };

      class Off_B : public State {
         public:
            Off_B(RelayController& pController) :State(pController){}
      };

      class On_A : public State {
         public:
            On_A(RelayController& pController) :State(pController){}
      };

      class On_B : public State {
         public:
            On_B(RelayController& pController) :State(pController){}
      };

      class ToggleAOn : public State  {
         public:
            ToggleAOn(RelayController& pController) :State(pController){}

            virtual void onEnter(){
               mController->mActionPort->relayAToOn(true);
               mController->mStateMachine.setTimer(RELAY_ENERGIZATION_DURATION);
            }

            virtual void evTimeUp(){
               mController->mActionPort->relayAToOn(false);
               if(mController->mQueryPort->loadOut()) {
                  mController->mStateMachine.nextState(mController->mOn_B);
               } else {
                  mController->mStateMachine.nextState(mController->mOff_B);
               }
            }
      };

      class ToggleAOff : public State  {
         public:
            ToggleAOff(RelayController& pController) :State(pController){}

            virtual void onEnter(){
               mController->mActionPort->relayAToOff(true);
               mController->mStateMachine.setTimer(RELAY_ENERGIZATION_DURATION);
            }

            virtual void evTimeUp(){
               mController->mActionPort->relayAToOff(false);
               mController->mStateMachine.nextState(mController->mToggleBOn);
            }
      };

      class ToggleBOn : public State  {
         public:
            ToggleBOn(RelayController& pController) :State(pController){}

            virtual void onEnter(){
               mController->mActionPort->relayBToOn(true);
               mController->mStateMachine.setTimer(RELAY_ENERGIZATION_DURATION);
            }

            virtual void evTimeUp(){
               mController->mActionPort->relayBToOn(false);
               if(mController->mQueryPort->loadOut()) {
                  mController->mStateMachine.nextState(mController->mOn_B);
               } else {
                  mController->mStateMachine.nextState(mController->mOff_B);
               }
            }
      };

      class ToggleBOff : public State  {
         public:
            ToggleBOff(RelayController& pController) :State(pController){}

            virtual void onEnter(){
               mController->mActionPort->relayBToOff(true);
               mController->mStateMachine.setTimer(RELAY_ENERGIZATION_DURATION);
            }

            virtual void evTimeUp(){
               mController->mActionPort->relayBToOff(false);
               mController->mStateMachine.nextState(mController->mToggleAOn);
            }
      };

      class ToggleLoadOn : public State  {
         public:
            ToggleLoadOn(RelayController& pController) :State(pController){}

            virtual void onEnter(){
               mController->mActionPort->relayLoadToOn(true);
               mController->mStateMachine.setTimer(RELAY_ENERGIZATION_DURATION);
            }

            virtual void evTimeUp(){
               mController->mActionPort->relayLoadToOn(false);
               if(mController->mQueryPort->loadASense() && mController->mQueryPort->loadBSense()) {
                  mController->mStateMachine.nextState(mController->mToggleAOn); // TODO decide on voltage
               } else if (mController->mQueryPort->loadASense()) {
                  mController->mStateMachine.nextState(mController->mOn_B);
               } else {
                  mController->mStateMachine.nextState(mController->mOn_A);
               }
            }
      };

      class ToggleLoadOff : public State  {
         public:
            ToggleLoadOff(RelayController& pController) :State(pController){}

            virtual void onEnter(){
               mController->mActionPort->relayLoadToOff(true);
               mController->mStateMachine.setTimer(RELAY_ENERGIZATION_DURATION);
            }

            virtual void evTimeUp(){
               mController->mActionPort->relayLoadToOff(false);
               if(mController->mQueryPort->loadASense() && mController->mQueryPort->loadBSense()) {
                  mController->mStateMachine.nextState(mController->mToggleAOn); // TODO decide on voltage
               } else if (mController->mQueryPort->loadASense()) {
                  mController->mStateMachine.nextState(mController->mOff_B);
               } else {
                  mController->mStateMachine.nextState(mController->mOff_A);
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
      Off_A mOff_A;
      Off_B mOff_B;
      On_A mOn_A;
      On_B mOn_B;
      ToggleAOn mToggleAOn;
      ToggleAOff mToggleAOff;
      ToggleBOn mToggleBOn;
      ToggleBOff mToggleBOff;
      ToggleLoadOn mToggleLoadOn;
      ToggleLoadOff mToggleLoadOff;

      Core::StateMachine<State> mStateMachine;

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
