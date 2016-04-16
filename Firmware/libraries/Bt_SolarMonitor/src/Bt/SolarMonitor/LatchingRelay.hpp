//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::LatchingRelay
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_LatchingRelay__hpp
#define INC__Bt_SolarMonitor_LatchingRelay__hpp

#include <Bt/Core/I_DigitalOut.hpp>
#include "Bt/SolarMonitor/I_LatchingRelay.hpp"


namespace Bt {
namespace SolarMonitor {

class LatchingRelay : public I_LatchingRelay
{
   public:
      LatchingRelay(Core::I_DigitalOut& pOff, Core::I_DigitalOut& pOn);
      ~LatchingRelay();

      virtual void onCoil(bool pValue);
      virtual void offCoil(bool pValue);

   private:
   	  // Constructor to prohibit copy construction.
      LatchingRelay(const LatchingRelay&);

      // Operator= to prohibit copy assignment
      LatchingRelay& operator=(const LatchingRelay&);

      Core::I_DigitalOut* mOff;
      Core::I_DigitalOut* mOn;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_LatchingRelay__hpp
