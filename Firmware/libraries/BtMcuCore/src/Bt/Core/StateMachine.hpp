//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::StateMachine
//
//*************************************************************************************************

#ifndef INC__Bt_Core_StateMachine__hpp
#define INC__Bt_Core_StateMachine__hpp

#include <cstdint>

namespace Bt {
namespace Core {

template<typename State>
class StateMachine
{
   public:

      StateMachine(State& initialState) : mCurrentState(&initialState) {
         mCurrentState->onEnter();
      }

      ~StateMachine(){

      }

      void nextState(State& pState) {
         mCurrentState->onExit();
         mCurrentState = &pState;
         mCurrentState->onEnter();
      }

      void setTimer(uint32_t) {

      }

   private:
      State* mCurrentState;


};


} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_StateMachine__hpp
