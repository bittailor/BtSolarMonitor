//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MainController
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MainController__hpp
#define INC__Bt_SolarMonitor_MainController__hpp

#include <Bt/Core/Wire.hpp>
#include <Bt/Core/DigitalIn.hpp>
#include <Bt/Core/Time.hpp>
#include <Bt/Core/Workcycle.hpp>
#include <Bt/Core/PeriodicCallback.hpp>
#include <Bt_INA219.h>
#include "Bt/SolarMonitor/MeasureLoop.hpp"
#include "Bt/SolarMonitor/NokiaScreen.hpp"
#include "Bt/SolarMonitor/IoSlave.hpp"
#include "Bt/SolarMonitor/Screens.hpp"
#include "Bt/SolarMonitor/MqttClient.hpp"
#include "Bt/SolarMonitor/Publisher.hpp"

namespace Bt {
namespace SolarMonitor {

class MainController
{
   public:
      MainController();
      ~MainController();

      void begin();
      bool loop();

   private:
   	  // Constructor to prohibit copy construction.
      MainController(const MainController&);

      // Operator= to prohibit copy assignment
      MainController& operator=(const MainController&);

      void measure();
      void yield();
      void publish();

      void OnMeasurementRecord(const MeasurementRecord& pRecord);
      void log(const char* pPrefix, const Measurement& pMeasurement);

      Bt::Core::WireWrapper mWire;
      Bt::Core::Time mTime;

      Bt::Core::Workcycle mMainWorkcycle;
      Bt::Core::PeriodicCallback mMeasureCallback;
      Bt::Core::PeriodicCallback mYieldCallback;
      Bt::Core::PeriodicCallback mPublishCallback;

      INA219 mSensorPanelA;
      INA219 mSensorPanelB;
      INA219 mSensorBatteryA;
      INA219 mSensorBatteryB;
      INA219 mSensorLoad;
      INA219 mSensorControl;

      Core::DigitalIn mNotify;

      IoSlave mIoSlave;

      MeasureLoop mMeasureLoop;

      NokiaScreen mNokiaScreenOne;
      NokiaScreen mNokiaScreenTwo;

      Screens mScreens;

      MqttClient mMqttClient;
      Publisher mPublisher;

      MeasurementRecord mRecordToPublish;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MainController__hpp
