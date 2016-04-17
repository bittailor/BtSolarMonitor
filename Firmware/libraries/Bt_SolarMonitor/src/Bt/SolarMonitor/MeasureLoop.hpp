//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MeasureLoop
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MeasureLoop__hpp
#define INC__Bt_SolarMonitor_MeasureLoop__hpp

#include <Bt_INA219.h>
#include <Bt/Core/Function.hpp>

#include "Bt/SolarMonitor/MeasurementRecord.hpp"

namespace Bt {
namespace SolarMonitor {

class MeasureLoop
{
   public:
      typedef Bt::Core::Function<void(const MeasurementRecord&)> Callback;

      MeasureLoop(INA219& pSensorPanelA,
                  INA219& pSensorPanelB,
                  INA219& pSensorBatteryA,
                  INA219& pSensorBatteryB,
                  INA219& pSensorLoad,
                  INA219& pSensorControl);
      ~MeasureLoop();

      void measure(Callback pCallback);

   private:
      enum {NUMBER_OF_SENSORS = 6};

   	  // Constructor to prohibit copy construction.
      MeasureLoop(const MeasureLoop&);

      // Operator= to prohibit copy assignment
      MeasureLoop& operator=(const MeasureLoop&);

      INA219* mSensors[NUMBER_OF_SENSORS];
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MeasureLoop__hpp
