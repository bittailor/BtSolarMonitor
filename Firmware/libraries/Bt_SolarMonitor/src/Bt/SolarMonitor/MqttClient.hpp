//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MqttClient
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MqttClient__hpp
#define INC__Bt_SolarMonitor_MqttClient__hpp

#include <Bt/Core/Logger.hpp>

#include <Arduino.h>
#include <MQTTClient.h>

#include <Bt/Core/I_Time.hpp>
#include <Bt/Core/I_Workcycle.hpp>
#include <Bt/Core/Time.hpp>
#include <Bt/Core/DigitalOut.hpp>
#include <Bt/Core/DigitalIn.hpp>
#include <Bt/Core/Function.hpp>
#include <Bt/Core/Workcycle.hpp>
#include <Bt/Net/Gprs/MobileTerminal.hpp>
#include <Bt/Net/Gprs/GprsModule.hpp>

#include "Bt/SolarMonitor/I_MqttClient.hpp"


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
      enum class State : int {
         OFF = 0,
         PIN_OK = 1,
         NETWORK_REGISTRATION_OK = 2,
         GPRS_READY = 3,
         TCP_CONNECTED = 4,
         MQTT_CONNECTED = 5
      };

      MqttClient(Core::I_Time& pTime, Core::I_Workcycle& pWorkcycle);
      ~MqttClient();

      void begin();
      void shutdown();
      void setListener(Bt::Core::Function<void (State)> pStateListener){mStateListener = pStateListener;}

      virtual bool yield(uint32_t pTimeoutInMilliseconds);

      virtual bool publish(const char* pTopicName, void* pPayload, size_t pPayloadlen, QoS pQos = QOS0, bool pRetained = false);
      virtual bool publish(const char* pTopicName, const char* pMessage, QoS pQos = QOS0, bool pRetained = false);

      virtual void stateChanged(Net::Gprs::GprsModule::State pState);
      virtual void onReady();
      virtual void onConnected();
      virtual void onDisconnected();
      void publishState();

      // TODO hack inject
      Net::Gprs::GprsModule& gprsModule() {return mGprsModule;}


   private:
        // Constructor to prohibit copy construction.
      MqttClient(const MqttClient&);

      // Operator= to prohibit copy assignment
      MqttClient& operator=(const MqttClient&);

      void disconnect();

      bool mShutdown;
      State mState;
      Core::I_Workcycle* mWorkcycle;
      Core::DigitalOut mOnOffKey;
      Core::DigitalOut mReset;
      Core::DigitalIn mPowerState;
      Net::Gprs::MobileTerminal mMobileTerminal;

      Net::Gprs::GprsModule mGprsModule;
      MQTT::Client<Net::Gprs::I_GprsClient, PahoTimer, 500> mMqttClient;

      Bt::Core::Function<void (State)> mStateListener;

};

} // namespace SolarMonitor
} // namespace Bt


#endif // INC__Bt_SolarMonitor_MqttClient__hpp
