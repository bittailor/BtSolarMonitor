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
                         INA219& pSensorControl,
                         Callback pCallback)
: mSensors{&pSensorPanelA,
           &pSensorPanelB,
           &pSensorBatteryA,
           &pSensorBatteryB,
           &pSensorLoad,
           &pSensorControl}
, mCallback(pCallback){

}

//-------------------------------------------------------------------------------------------------

MeasureLoop::~MeasureLoop() {

}

//-------------------------------------------------------------------------------------------------

void MeasureLoop::measure() {
   Measurement measurements[NUMBER_OF_SENSORS];
   for (int i = 0; i < NUMBER_OF_SENSORS; ++i) {
      measurements[i] = Measurement(mSensors[i]->current(),mSensors[i]->busVoltage());
   }

   mCallback(MeasurementRecord(measurements[0],
                               measurements[1],
                               measurements[2],
                               measurements[3],
                               measurements[4],
                               measurements[5]));
}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
