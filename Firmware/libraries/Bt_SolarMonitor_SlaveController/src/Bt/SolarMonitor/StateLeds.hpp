//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::StateLeds
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_StateLeds__hpp
#define INC__Bt_SolarMonitor_StateLeds__hpp

#include "Bt/Core/I_DigitalOut.hpp"
#include "Bt/SolarMonitor/I_RelayController.hpp"

namespace Bt {
namespace SolarMonitor {

class StateLeds
{
   public:
      StateLeds(Core::I_DigitalOut& pLedA, Core::I_DigitalOut& pLedB);
      ~StateLeds();

      void show(I_RelayController::PublicState pState);

   private:
   	  // Constructor to prohibit copy construction.
      StateLeds(const StateLeds&);

      // Operator= to prohibit copy assignment
      StateLeds& operator=(const StateLeds&);

      Core::I_DigitalOut* mLedA;
      Core::I_DigitalOut* mLedB;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_StateLeds__hpp
