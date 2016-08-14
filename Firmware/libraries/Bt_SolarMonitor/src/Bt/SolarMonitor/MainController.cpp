//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MainController
//
//*************************************************************************************************

#include "Bt/SolarMonitor/MainController.hpp"

#include <Arduino.h>

#include "Bt/Core/Logger.hpp"

#define FONA_KEY 12
#define FONA_PS A0
#define FONA_RST 9

#if defined(ARDUINO_ARCH_SAMD)
 #define FONA_SERIAL Serial1
#elif defined(HAVE_HWSERIAL1) // TODO Franz workaround
 #define FONA_SERIAL Serial1
#else
 #define FONA_SERIAL Serial
#endif

namespace Bt {
namespace SolarMonitor {

namespace {

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

Net::Gprs::GprsModule::Settings sGprsModuleSettings = {
         "1210",
         "gprs.swisscom.ch"
};

} // namespace

//-------------------------------------------------------------------------------------------------

MainController::MainController()
: mMeasureCallback(mTime,500,Bt::Core::Function<void()>::build<MainController,&MainController::measure>(*this))
, mYieldCallback(mTime, 2 * 1000,Bt::Core::Function<void()>::build<MainController,&MainController::yield>(*this))
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
          MeasureLoop::Callback::build<MainController,&MainController::onMeasurementRecord>(*this))
, mNokiaScreenOne(A5, A4, A3)
, mNokiaScreenTwo(A5, A2, A1)
, mScreens(mNokiaScreenOne, mNokiaScreenTwo)

, mOnOffKey(FONA_KEY)
, mReset(FONA_RST)
, mPowerState(FONA_PS)
, mMobileTerminal(FONA_SERIAL)
, mGprsModule(sGprsModuleSettings, mTime, mOnOffKey, mReset, mPowerState, mMobileTerminal)
, mMqttClient(mTime, mMainWorkcycle, mGprsModule)

, mPublisher(mMqttClient)
, mRecordToPublish() {
   mMainWorkcycle.add(mMeasureCallback);
   mMainWorkcycle.add(mYieldCallback);
   mMainWorkcycle.add(mPublishCallback);
   mMainWorkcycle.add(mGprsModule);
   mMqttClient.setListener(Bt::Core::Function<void(MqttClient::State)>::build<MainController,&MainController::mqttStateUpdate>(*this));
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

   FONA_SERIAL.begin(9600);
   mOnOffKey.begin();
   mReset.begin();
   mPowerState.begin();
   mGprsModule.begin(mMqttClient);
   mMqttClient.begin();
}

//-------------------------------------------------------------------------------------------------

bool MainController::loop() {
   mMainWorkcycle.oneWorkcycle();

   if(mIoSlave.loop()) {
      I_PowerState::State powerState = mIoSlave.powerState();
      mScreens.updatePowerState(powerState);
      mMeasureLoop.measure();
   }

   return true;
}

//-------------------------------------------------------------------------------------------------

void MainController::shutdown() {
   mMqttClient.shutdown();
}

//-------------------------------------------------------------------------------------------------

void MainController::measure() {
   mMeasureLoop.measure();

}

//-------------------------------------------------------------------------------------------------


void MainController::yield() {
   mScreens.updateRSSI(mGprsModule.getRSSI());
}

//-------------------------------------------------------------------------------------------------

void MainController::publish() {
   LOG("");
   LOG("publish to cloud ...");
   if(mPublisher.publish(mRecordToPublish.averageAndClear(),mReconnectCounter)){
      mSuccessfulPublishCounter++;
      mScreens.updateCounter(mSuccessfulPublishCounter);
   } else {
      mSuccessfulPublishCounter = 0;
   }
   mMqttClient.yield(10);
   LOG(" ... done publish to cloud");
}

//-------------------------------------------------------------------------------------------------


void MainController::mqttStateUpdate(MqttClient::State pState) {
   mScreens.updateGsmState(static_cast<int>(pState));
}

//-------------------------------------------------------------------------------------------------

void MainController::onMeasurementRecord(const MeasurementRecord& pRecord) {

   mRecordToPublish.add(pRecord);
   mScreens.updateMeasurementRecord(pRecord);
}

//-------------------------------------------------------------------------------------------------

void MainController::log(const char* pPrefix, const Measurement& pMeasurement) {
   LOG(pPrefix << " u = " << pMeasurement.voltage() << " i = " << pMeasurement.current());

}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
