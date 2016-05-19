//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MqttClient
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MqttClient_Platform_Arduino__hpp
#define INC__Bt_SolarMonitor_MqttClient_Platform_Arduino__hpp

#include <Arduino.h>
#include <MQTTClient.h>
#include <Adafruit_FONA.h>
#include <Bt_PahoWithFona.h>

namespace Bt {
namespace SolarMonitor {

class MqttClient : public I_MqttClient
{
   public:
      MqttClient();
      ~MqttClient();

      void begin();

      virtual bool connect();
      virtual bool isConnected();

      virtual bool yield(uint32_t pTimeoutInMilliseconds);

      virtual bool publish(const char* pTopicName, void* pPayload, size_t pPayloadlen, QoS pQos = QOS0, bool pRetained = false);
      virtual bool publish(const char* pTopicName, const char* pMessage, QoS pQos = QOS0, bool pRetained = false);

   private:
   	  // Constructor to prohibit copy construction.
      MqttClient(const MqttClient&);

      // Operator= to prohibit copy assignment
      MqttClient& operator=(const MqttClient&);

      bool connectGPRS();
      bool connectMQTT();


      Adafruit_FONA mFona;
      Bt_PahoNetworkFona mNetwork;
      MQTT::Client<Bt_PahoNetworkFona, Bt_PahoTimer, 700, 1> mMqttClient;



};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MqttClient_Platform_Arduino__hpp
