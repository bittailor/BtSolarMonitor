//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MeasureLoop
//
//*************************************************************************************************

#include "Bt/SolarMonitor/MeasureLoop.hpp"

#include "Bt/SolarMonitor/Measurement.hpp"

namespace Bt {
namespace SolarMonitor {


//-------------------------------------------------------------------------------------------------

MeasureLoop::MeasureLoop(INA219& pSensorPanelA,
                         INA219& pSensorPanelB,
                         INA219& pSensorBatteryA,
                         INA219& pSensorBatteryB,
                         INA219& pSensorLoad,
                         INA219& pSensorControl)
: mSensors{&pSensorPanelA,
           &pSensorPanelB,
           &pSensorBatteryA,
           &pSensorBatteryB,
           &pSensorLoad,
           &pSensorControl}{

}

//-------------------------------------------------------------------------------------------------

MeasureLoop::~MeasureLoop() {

}

//-------------------------------------------------------------------------------------------------

void MeasureLoop::measure(Callback pCallback) {
   Measurement measurements[NUMBER_OF_SENSORS];
   for (int i = 0; i < NUMBER_OF_SENSORS; ++i) {
      measurements[i] = Measurement(mSensors[i]->current(),mSensors[i]->busVoltage());
   }

   pCallback(MeasurementRecord(measurements[0],
                               measurements[1],
                               measurements[2],
                               measurements[3],
                               measurements[4],
                               measurements[5]));
}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
