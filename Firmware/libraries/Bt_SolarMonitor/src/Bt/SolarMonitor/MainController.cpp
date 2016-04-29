//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MainController
//
//*************************************************************************************************

#include "Bt/SolarMonitor/MainController.hpp"

#include "Bt/Core/Logger.hpp"
#include "Bt/SolarMonitor/IOPins.hpp"

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
: mMeasureWorkcycle(500,mTime)
, mPublishWorkcycle(10000,mTime)
, mSensorPanelA  (mWire,0x40,sConfigurationPanel,   32768,250)
, mSensorPanelB  (mWire,0x44,sConfigurationPanel,   32768,250)
, mSensorBatteryA(mWire,0x41,sConfigurationBattery, 16384,500)
, mSensorBatteryB(mWire,0x45,sConfigurationBattery, 16384,500)
, mSensorLoad    (mWire,0x4C,sConfigurationLoad,    16384,500)
, mSensorControl (mWire,0x48,sConfigurationControl,  8192, 50)
, mNotify(5)
, mIoSlave(mWire,mNotify)
, mMeasureLoop(
          mSensorPanelA,
          mSensorPanelB,
          mSensorBatteryA,
          mSensorBatteryB,
          mSensorLoad,
          mSensorControl,
          MeasureLoop::Callback::build<MainController,&MainController::OnMeasurementRecord>(*this))
, mNokiaScreenOne(A5, A4, A3)
, mNokiaScreenTwo(A5, A2, A1)
, mScreens(mNokiaScreenOne, mNokiaScreenTwo)
, mStartTime(0)
, mInterval(500) {
   mMainWorkcycle.add(mMeasureWorkcycle);
   mMainWorkcycle.add(mPublishWorkcycle);
}

//-------------------------------------------------------------------------------------------------

MainController::~MainController() {

}

//-------------------------------------------------------------------------------------------------

void MainController::begin() {
   mSensorPanelA.begin();
   mSensorPanelB.begin();
   mSensorBatteryA.begin();
   mSensorBatteryB.begin();
   mSensorLoad.begin();
   mSensorControl.begin();
   mNokiaScreenOne.begin();
   mNokiaScreenTwo.begin();
   mIoSlave.begin();

}

//-------------------------------------------------------------------------------------------------

bool MainController::loop() {
   mMainWorkcycle.oneWorkcycle();


   bool doMeasure = false;
   if (mTime.milliseconds() - mStartTime >= mInterval) {
      mStartTime = mTime.milliseconds();
      doMeasure = true;
   }

   if(mIoSlave.loop() || doMeasure) {
      mMeasureLoop.measure();
   }

   return true;
}

//-------------------------------------------------------------------------------------------------

void MainController::OnMeasurementRecord(const MeasurementRecord& pRecord) {
   static uint32_t counter = 0;
   LOG("");
   LOG(counter++);
   log("PanelA:  ", pRecord.panelA());
   log("PanelB:  ", pRecord.panelB());
   log("BatteryA:", pRecord.batteryA());
   log("BatteryB:", pRecord.batteryB());
   log("Load:    ", pRecord.load());
   log("Control: ", pRecord.control());

   I_PowerState::State powerState = mIoSlave.powerState();
   LOG("PowerState: " << powerState);
   mScreens.update(pRecord, powerState);



}

//-------------------------------------------------------------------------------------------------

void MainController::log(const char* pPrefix, const Measurement& pMeasurement) {
   LOG(pPrefix << " u = " << pMeasurement.voltage() << " i = " << pMeasurement.current());

}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
