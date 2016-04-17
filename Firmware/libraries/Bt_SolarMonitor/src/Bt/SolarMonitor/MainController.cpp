//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MainController
//
//*************************************************************************************************

#include "Bt/SolarMonitor/MainController.hpp"

namespace Bt {
namespace SolarMonitor {

const uint16_t sConfigurationPanel =
         INA219_BUS_VOLTAGE_RANGE_16V |
         INA219_GAIN_1_40MV |
         INA219_BADCRES_12BIT |
         INA219_SADCRES_12BIT_1S_532US |
         INA219_MODE_SANDBVOLT_CONTINUOUS;

const uint16_t sConfigurationBattery =
         INA219_BUS_VOLTAGE_RANGE_16V |
         INA219_GAIN_2_80MV |
         INA219_BADCRES_12BIT |
         INA219_SADCRES_12BIT_1S_532US |
         INA219_MODE_SANDBVOLT_CONTINUOUS;

const uint16_t sConfigurationLoad =
         INA219_BUS_VOLTAGE_RANGE_16V |
         INA219_GAIN_2_80MV |
         INA219_BADCRES_12BIT |
         INA219_SADCRES_12BIT_1S_532US |
         INA219_MODE_SANDBVOLT_CONTINUOUS;

const uint16_t sConfigurationControl =
         INA219_BUS_VOLTAGE_RANGE_16V |
         INA219_GAIN_2_80MV |
         INA219_BADCRES_12BIT |
         INA219_SADCRES_12BIT_1S_532US |
         INA219_MODE_SANDBVOLT_CONTINUOUS;

//-------------------------------------------------------------------------------------------------

MainController::MainController()
:mSensorPanelA  (mWire,0x40,sConfigurationPanel,   32768,250)
,mSensorPanelB  (mWire,0x44,sConfigurationPanel,   32768,250)
,mSensorBatteryA(mWire,0x41,sConfigurationBattery, 16384,500)
,mSensorBatteryB(mWire,0x45,sConfigurationBattery, 16384,500)
,mSensorLoad    (mWire,0x4C,sConfigurationLoad,    16384,500)
,mSensorControl (mWire,0x48,sConfigurationControl,  8192, 50)
,mMeasureLoop(
   mSensorPanelA,
   mSensorPanelB,
   mSensorBatteryA,
   mSensorBatteryB,
   mSensorLoad,
   mSensorControl) {

}

//-------------------------------------------------------------------------------------------------

MainController::~MainController() {

}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
