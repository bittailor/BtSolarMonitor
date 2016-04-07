//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::Core::I_PushButton
//
//*************************************************************************************************

#ifndef INC__Bt_Core_I_PushButton__hpp
#define INC__Bt_Core_I_PushButton__hpp

namespace Bt {
namespace Core {

class I_PushButton {
   public:
      class I_Listener {
         public:
            virtual ~I_Listener() {}
            virtual void pressed()=0;
            virtual void released()=0;
      };

      class State {
         public:
         virtual ~State() {}
         virtual void change(){}
      };
      typedef void (State::*Event)();

      virtual ~I_PushButton() {}

      virtual void handle(Event pEvent) = 0;

};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_I_PushButton__hpp
