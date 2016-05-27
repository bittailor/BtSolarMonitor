//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MqttClient
//
//*************************************************************************************************

#include "Bt/Core/Logger.hpp"
#include "Bt/SolarMonitor/MqttClient.Platform.Arduino.hpp"

namespace Bt {
namespace SolarMonitor {

#define FONA_KEY 12
#define FONA_PS A0
#define FONA_RST 9
#define FONA_APN "gprs.swisscom.ch"


#if defined(ARDUINO_ARCH_SAMD)
 #define FONA_SERIAL Serial1
#elif defined(HAVE_HWSERIAL1) // TODO Franz workaround
 #define FONA_SERIAL Serial1
#else
 #define FONA_SERIAL Serial
#endif


//-------------------------------------------------------------------------------------------------

MqttClient::MqttClient(Core::I_Time& pTime, Core::I_Workcycle& pWorkcycle)
: mWorkcycle(&pWorkcycle)
, mOnOffKey(FONA_KEY)
, mReset(FONA_RST)
, mPowerState(FONA_PS)
, mMobileTerminal(FONA_SERIAL)
, mGprsModule(pTime, mOnOffKey, mReset, mPowerState, mMobileTerminal)
, mMqttClient(mGprsModule)
{

}

//-------------------------------------------------------------------------------------------------

MqttClient::~MqttClient() {

}

//-------------------------------------------------------------------------------------------------

void MqttClient::begin() {
   FONA_SERIAL.begin(115200);
   mWorkcycle->add(mMobileTerminal);
   mWorkcycle->add(mGprsModule);
   mOnOffKey.begin();
   mReset.begin();
   mPowerState.begin();
   mGprsModule.begin(*this);



}

//-------------------------------------------------------------------------------------------------

bool MqttClient::publish(const char* pTopicName, void* pPayload, size_t pPayloadlen, QoS pQos, bool pRetained) {
   MQTT::QoS qos;
   switch(pQos){
      case QoS::QOS0 : qos =  MQTT::QOS0; break;
      case QoS::QOS1 : qos =  MQTT::QOS1; break;
      case QoS::QOS2 : qos =  MQTT::QOS2; break;
   }

   return mMqttClient.publish(pTopicName, pPayload, pPayloadlen, qos, pRetained) == 0;
}

bool MqttClient::publish(const char* pTopicName, const char* pMessage, QoS pQos, bool pRetained) {
   LOG("MQTT publish " << pTopicName << " - " << pMessage << " [" << strlen(pMessage) << "]" );
   return publish(pTopicName, const_cast<char*>(pMessage), strlen(pMessage), pQos, pRetained);
}



//-------------------------------------------------------------------------------------------------

void MqttClient::onReady() {
   const char * url = "broker.shiftr.io";
   //const char * url = "iot.eclipse.org";

   while(int rc = mGprsModule.connect(url,1883) != 0) {
      LOG("tcp connect failed with " << rc);
   }
}

//-------------------------------------------------------------------------------------------------

void MqttClient::onConnected() {
   MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
   data.MQTTVersion = 3;
   data.clientID.cstring = (char*)"bt-resource-check";
   data.keepAliveInterval = 120;
   data.willFlag = true;
   data.will.topicName.cstring = (char*)"BSM/Status";
   data.will.message.cstring = (char*)"offline";
   data.will.retained = true;
   data.will.qos = MQTT::QOS1;
   data.username.cstring = (char*)"f64edae4";
   data.password.cstring = (char*)"eea9554c6e05c108";

   int rc = mMqttClient.connect(data);
   if (rc != 0)
   {
      LOG("connect returned"  << rc);
      return;
   }
   LOG("MQTT connected");

   publish("BSM/Status", "online", QoS::QOS1, "online");
}

//-------------------------------------------------------------------------------------------------

bool MqttClient::connect() {
}

//-------------------------------------------------------------------------------------------------

bool MqttClient::isConnected() {
   if(mGprsModule.isConnected()) {
      return mMqttClient.isConnected();
   }
   return false;
}

//-------------------------------------------------------------------------------------------------

bool MqttClient::yield(uint32_t pTimeoutInMilliseconds) {
   if(mGprsModule.isConnected()) {
      return mMqttClient.yield(pTimeoutInMilliseconds);
   }
   return false;
}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
