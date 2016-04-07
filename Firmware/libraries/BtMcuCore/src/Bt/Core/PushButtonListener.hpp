//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::PushButtonListener
//
//*************************************************************************************************

#ifndef INC__Bt_Core_PushButtonListener__hpp
#define INC__Bt_Core_PushButtonListener__hpp

#include "Bt/Core/I_PushButton.hpp"

namespace Bt {
namespace Core {

template<typename T>
class PushButtonListener : public I_PushButton::I_Listener
{
   public:
      typedef void (T::*Callback)();

      PushButtonListener(T& pObject, Callback pPressed, Callback pReleased = nullptr)
      : mObject(&pObject), mPressed(pPressed), mReleased(pReleased){
      }

      virtual void pressed() {
         if(mPressed) {
            (mObject->*mPressed)();
         }
      }

      virtual void released() {
         if(mReleased) {
            (mObject->*mReleased)();
         }
      }

   private:
   	  // Constructor to prohibit copy construction.
      PushButtonListener(const PushButtonListener&);

      // Operator= to prohibit copy assignment
      PushButtonListener& operator=(const PushButtonListener&);

      T* mObject;
      Callback mPressed;
      Callback mReleased;

};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_PushButtonListener__hpp
