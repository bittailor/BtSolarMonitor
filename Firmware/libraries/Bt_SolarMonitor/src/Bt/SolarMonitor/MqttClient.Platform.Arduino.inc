//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MqttClient
//
//*************************************************************************************************

#include "Bt/Core/Logger.hpp"
#include "Bt/SolarMonitor/MqttClient.Platform.Arduino.hpp"

namespace Bt {
namespace SolarMonitor {

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

MqttClient::MqttClient()
: mFona(FONA_RST)
, mNetwork(mFona)
, mMqttClient(mNetwork)
{

}

//-------------------------------------------------------------------------------------------------

MqttClient::~MqttClient() {

}

//-------------------------------------------------------------------------------------------------

void MqttClient::begin() {
   FONA_SERIAL.begin(9600);
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
   // LOG("MQTT publish " << pTopicName << " - " << pMessage << " [" << strlen(pMessage) << "]" );
   return publish(pTopicName, const_cast<char*>(pMessage), strlen(pMessage), pQos, pRetained);
}


//-------------------------------------------------------------------------------------------------

bool MqttClient::connect() {
   if(connectGPRS()){
      return connectMQTT();
   }
   LOG("GPRS failed !");
   return false;
}

//-------------------------------------------------------------------------------------------------

bool MqttClient::isConnected() {
   return mMqttClient.isConnected();
}

//-------------------------------------------------------------------------------------------------

bool MqttClient::yield(uint32_t pTimeoutInMilliseconds) {
   return mMqttClient.yield(pTimeoutInMilliseconds);
}

//-------------------------------------------------------------------------------------------------

bool MqttClient::connectGPRS() {
   if (! mFona.begin(FONA_SERIAL)) {
      LOG("Fona.begin failed !");
      return false;
   }

   FONA_SERIAL.println("AT+CMEE=2");
   mFona.unlockSIM("1210");

   while (mFona.getNetworkStatus() != 1) {
      delay(100);
   }

   mFona.setGPRSNetworkSettings(F(FONA_APN));

   if (!mFona.enableGPRS(true)) {
      LOG("Fona.enableGPRS failed !");
      return false;
   }

   while (mFona.GPRSstate() != 1) {
      delay(100);
   }

   return true;
}


//-------------------------------------------------------------------------------------------------

bool MqttClient::connectMQTT() {
    char hostname[] = "broker.shiftr.io";
    int port = 1883;


    int rc = mNetwork.connect(hostname, port);
    if (rc != 0)
    {
    }

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


    rc = mMqttClient.connect(data);
    if (rc != 0)
    {
       LOG("connect returned"  << rc);
    }
    LOG("MQTT connected");

    publish("BSM/Status","online",QOS1,"online");

    return true;
}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
