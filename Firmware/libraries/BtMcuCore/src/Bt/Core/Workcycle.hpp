//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Workcycle
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Workcycle__hpp
#define INC__Bt_Core_Workcycle__hpp

#include "Bt/Core/I_Workcycle.hpp"

namespace Bt {
namespace Core {

class Workcycle : public I_Workcycle
{
   public:
      Workcycle();
      ~Workcycle();

      virtual void add(I_Runnable& iRunnable);
      virtual void remove(I_Runnable& iRunnable);

      uint32_t oneWorkcycle();

   private:
        // Constructor to prohibit copy construction.
      Workcycle(const Workcycle&);

      // Operator= to prohibit copy assignment
      Workcycle& operator=(const Workcycle&);


      typedef IntrusiveList<I_Runnable> Runnables;

      IntrusiveList<I_Runnable> mRunnables;
};


} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Workcycle__hpp
