//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MainController
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MainController__hpp
#define INC__Bt_SolarMonitor_MainController__hpp

#include <Bt/Core/Wire.hpp>
#include <Bt_INA219.h>
#include "Bt/SolarMonitor/MeasureLoop.hpp"

namespace Bt {
namespace SolarMonitor {

class MainController
{
   public:
      MainController();
      ~MainController();

   private:
   	  // Constructor to prohibit copy construction.
      MainController(const MainController&);

      // Operator= to prohibit copy assignment
      MainController& operator=(const MainController&);

      Bt::Core::WireWrapper mWire;

      INA219 mSensorPanelA;
      INA219 mSensorPanelB;
      INA219 mSensorBatteryA;
      INA219 mSensorBatteryB;
      INA219 mSensorLoad;
      INA219 mSensorControl;

      MeasureLoop mMeasureLoop;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MainController__hpp
