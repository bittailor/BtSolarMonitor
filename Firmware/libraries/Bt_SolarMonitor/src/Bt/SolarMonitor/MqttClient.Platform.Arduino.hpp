//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MqttClient
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MqttClient_Platform_Arduino__hpp
#define INC__Bt_SolarMonitor_MqttClient_Platform_Arduino__hpp

#include <Arduino.h>
#include <BtMcuCore.h>
#include <BtMcuNet.h>
#include <MQTTClient.h>

#include <Bt/Core/Logger.hpp>
#include <Bt/Core/Time.hpp>
#include <Bt/Core/DigitalOut.hpp>
#include <Bt/Core/DigitalIn.hpp>
#include <Bt/Core/Workcycle.hpp>
#include <Bt/Net/Gprs/MobileTerminal.hpp>
#include <Bt/Net/Gprs/GprsModule.hpp>


namespace Bt {
namespace SolarMonitor {

class PahoTimer {
   public:

      PahoTimer() : mInterval(0), mStart(millis()) {
      }

      PahoTimer(int ms) : mInterval(ms), mStart(millis()) {
      }


      bool expired() {
         return (millis() - mStart) >= mInterval;
      }

      void countdown_ms(unsigned long ms) {
         mInterval = ms;
         mStart = millis();
      }
      void countdown(int seconds) {
         countdown_ms(((unsigned long)seconds) * 1000L);
      }

      int left_ms() {
         unsigned long delay = millis() - mStart;
         if(delay >= mInterval) {
            return 0;
         }
         return mInterval - delay;
      }

   private:
      unsigned long mInterval;
      unsigned long mStart;

};

class MqttClient : public I_MqttClient , public Net::Gprs::GprsModule::I_Listener
{
   public:
      MqttClient(Core::I_Time& pTime, Core::I_Workcycle& pWorkcycle);
      ~MqttClient();

      void begin();

      virtual bool connect();
      virtual bool isConnected();

      virtual bool yield(uint32_t pTimeoutInMilliseconds);

      virtual bool publish(const char* pTopicName, void* pPayload, size_t pPayloadlen, QoS pQos = QOS0, bool pRetained = false);
      virtual bool publish(const char* pTopicName, const char* pMessage, QoS pQos = QOS0, bool pRetained = false);

      virtual void onReady();
      virtual void onConnected();

   private:
   	  // Constructor to prohibit copy construction.
      MqttClient(const MqttClient&);

      // Operator= to prohibit copy assignment
      MqttClient& operator=(const MqttClient&);

      bool connectGPRS();
      bool connectMQTT();

      Core::I_Workcycle* mWorkcycle;

      Core::DigitalOut mOnOffKey;
      Core::DigitalOut mReset;
      Core::DigitalIn mPowerState;
      Net::Gprs::MobileTerminal mMobileTerminal;

      Net::Gprs::GprsModule mGprsModule;
      MQTT::Client<Net::Gprs::I_GprsClient, PahoTimer, 500> mMqttClient;


};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MqttClient_Platform_Arduino__hpp
