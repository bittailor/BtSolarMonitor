//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::StateMachine
//
//*************************************************************************************************

#ifndef INC__Bt_Core_StateMachine__hpp
#define INC__Bt_Core_StateMachine__hpp

#include <cstdint>
#include "Bt/Core/I_Time.hpp"
#include "Bt/Core/Logger.hpp"

namespace Bt {
namespace Core {

template<typename Interface, typename Implementation>
class StateMachine : public Interface
{
   public:
      typedef typename Interface::State State;
      typedef typename Interface::Event Event;

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

      StateMachine(I_Time& pTime):mTime(&pTime), mStartTime(0), mInterval(0) {
      }

      ~StateMachine() {

      }

      void loop() {
         if(mInterval == 0) {
            return;
         }
         if (mTime->milliseconds() - mStartTime >= mInterval) {
            mInterval = 0;
            mStartTime = 0;
            mCurrentState->timeUp();
         }
      }

      virtual void handle(Event pEvent) {
         (mCurrentState->*pEvent)();
      }

   protected:
      void nextState(StateBase& pState) {
         LOG( name() << ": " << mCurrentState->name() << " => " << pState.name());
         mCurrentState->onExit();
         mCurrentState = &pState;
         mCurrentState->onEnter();
      }

      void setTimer(uint32_t pInterval) {
         mStartTime = mTime->milliseconds();
         mInterval = pInterval;
      }

      void init(StateBase& initialState) {
         mCurrentState = &initialState;
         mCurrentState->onEnter();
      }

      virtual const char* name() =0;

   private:
      I_Time* mTime;
      StateBase* mCurrentState;
      uint32_t mStartTime;
      uint32_t mInterval;

};


} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_StateMachine__hpp
