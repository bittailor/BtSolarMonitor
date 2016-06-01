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
: mMeasureCallback(mTime,500,Bt::Core::Function<void()>::build<MainController,&MainController::measure>(*this))
, mYieldCallback(mTime,10*1000,Bt::Core::Function<void()>::build<MainController,&MainController::yield>(*this))
, mPublishCallback(mTime,/*10 * 60 * 1000*/ 2 * 60 * 1000,Bt::Core::Function<void()>::build<MainController,&MainController::publish>(*this))
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
, mMqttClient(mTime,mMainWorkcycle)
, mPublisher(mMqttClient)
, mRecordToPublish(
         Measurement(0,0),
         Measurement(0,0),
         Measurement(0,0),
         Measurement(0,0),
         Measurement(0,0),
         Measurement(0,0)){
   mMainWorkcycle.add(mMeasureCallback);
   mMainWorkcycle.add(mYieldCallback);
   mMainWorkcycle.add(mPublishCallback);
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
   mNotify.begin();
   mNokiaScreenOne.begin();
   mNokiaScreenTwo.begin();
   mIoSlave.begin();
   mMqttClient.begin();
   mMqttClient.connect();

}

//-------------------------------------------------------------------------------------------------

bool MainController::loop() {
   mMainWorkcycle.oneWorkcycle();



   if(mIoSlave.loop()) {
      mMeasureLoop.measure();
   }

   return true;
}

//-------------------------------------------------------------------------------------------------

void MainController::measure() {
   mMeasureLoop.measure();

}

//-------------------------------------------------------------------------------------------------


void MainController::yield() {
   if(!mMqttClient.isConnected()) {
      mMqttClient.yield(1);
   } else {
      mMqttClient.connect();
   }
}

//-------------------------------------------------------------------------------------------------

void MainController::publish() {

   if(!mMqttClient.isConnected()) {
      LOG("oops is disconnected lets reconnect");
      if(!mMqttClient.connect()) {
         LOG("oops oops reconnect failed let's try next time");
         return;
      }
   }

   LOG("");
   LOG("publish to cloud ...");
   mPublisher.publish(mRecordToPublish);
   LOG(" ... done publish to cloud");
}

//-------------------------------------------------------------------------------------------------

void MainController::OnMeasurementRecord(const MeasurementRecord& pRecord) {
   static uint32_t counter = 0;

   mRecordToPublish = pRecord;

   /*
   LOG("");
   LOG(counter++);
   log("PanelA:  ", pRecord.panelA());
   log("PanelB:  ", pRecord.panelB());
   log("BatteryA:", pRecord.batteryA());
   log("BatteryB:", pRecord.batteryB());
   log("Load:    ", pRecord.load());
   log("Control: ", pRecord.control());
   */

   I_PowerState::State powerState = mIoSlave.powerState();
   //LOG("PowerState: " << powerState);

   //bool connectionState =  mMqttClient.yield(1);
   //LOG("ConnectionState" << connectionState);

   mScreens.update(pRecord, powerState, true);


}

//-------------------------------------------------------------------------------------------------

void MainController::log(const char* pPrefix, const Measurement& pMeasurement) {
   LOG(pPrefix << " u = " << pMeasurement.voltage() << " i = " << pMeasurement.current());

}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
