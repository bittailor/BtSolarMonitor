//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Screens
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Screens__hpp
#define INC__Bt_SolarMonitor_Screens__hpp

#include "Bt/SolarMonitor/I_Screen.hpp"
#include "Bt/SolarMonitor/MeasurementRecord.hpp"
#include "Bt/SolarMonitor/PowerState.hpp"

namespace Bt {
namespace SolarMonitor {

class Screens
{
   public:
      Screens(I_Screen& pOne, I_Screen& pTwo);
      ~Screens();

      void update(const MeasurementRecord& pMeasurementRecord, PowerState::State pPowerState, bool pConnectionState);

   private:
   	  // Constructor to prohibit copy construction.
      Screens(const Screens&);

      // Operator= to prohibit copy assignment
      Screens& operator=(const Screens&);

      void printMeasureValue(double value, size_t pScreenIndex);

      I_Screen* mScreens[2];

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Screens__hpp
