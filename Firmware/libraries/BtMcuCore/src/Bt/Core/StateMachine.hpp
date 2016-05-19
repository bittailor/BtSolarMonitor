//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::StateMachine
//
//*************************************************************************************************

#ifndef INC__Bt_Core_StateMachine__hpp
#define INC__Bt_Core_StateMachine__hpp

#include <stdint.h>
#include "Bt/Core/I_Time.hpp"
#include "Bt/Core/I_Runnable.hpp"
#include "Bt/Core/Logger.hpp"

namespace Bt {
namespace Core {

template<typename State, typename Implementation>
class StateMachine : public Bt::Core::I_Runnable
{
   public:

      class StateBase : public State {
         public:
            StateBase(Implementation& pImplementation):mController(&pImplementation){}
            virtual void onEnter(){}
            virtual void onExit(){}
            virtual void timeUp(){}
            virtual const char* name() = 0;

         protected:
            Implementation* mController;
      };

      StateMachine(I_Time& pTime):mTime(&pTime), mTimerActive(false), mStartTime(0), mInterval(0) {
      }

      ~StateMachine() {

      }

      virtual uint32_t workcycle() {
         if(!mTimerActive) {
            return FOREVER;
         }

         uint32_t diff = mTime->milliseconds() - mStartTime;
         if (diff >= mInterval) {
            mTimerActive = false;
            mInterval = 0;
            mStartTime = 0;
            mCurrentState->timeUp();
            return IMMEDIATELY;
         }

         return mInterval - diff;
      }

   protected:

      void handle(void (State::*pEvent)()) {
         (mCurrentState->*pEvent)();
      }

      template<typename Arg>
      void handle(void (State::*pEvent)(Arg),Arg pArg) {
         (mCurrentState->*pEvent)(pArg);
      }


      void nextState(StateBase& pState) {
         LOG( name() << ": " << mCurrentState->name() << " => " << pState.name());
         mCurrentState->onExit();
         mCurrentState = &pState;
         mCurrentState->onEnter();
      }

      void setTimer(uint32_t pInterval) {
         mTimerActive = true;
         mStartTime = mTime->milliseconds();
         mInterval = pInterval;
      }

      void resetTimer() {
         mTimerActive = false;
         mStartTime = 0;
         mInterval = 0;
      }

      void init(StateBase& initialState) {
         mCurrentState = &initialState;
         mCurrentState->onEnter();
      }

      virtual const char* name() =0;

   private:
      I_Time* mTime;
      StateBase* mCurrentState;
      bool mTimerActive;
      uint32_t mStartTime;
      uint32_t mInterval;

};


} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_StateMachine__hpp
