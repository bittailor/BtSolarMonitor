//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MqttClient
//
//*************************************************************************************************

#include "Bt/Core/Logger.hpp"
#include "Bt/SolarMonitor/MqttClient.hpp"

namespace Bt {
namespace SolarMonitor {

//-------------------------------------------------------------------------------------------------

MqttClient::MqttClient(Settings pSettings, Core::I_Time& pTime, Core::I_Workcycle& pWorkcycle, Net::Gprs::I_GprsClient& pGprsModule)
: mSettings(pSettings)
, mShutdown(false)
, mConnectCounter(0)
, mWorkcycle(&pWorkcycle)
, mGprsModule(&pGprsModule)
, mMqttClient(*mGprsModule)
{

}

//-------------------------------------------------------------------------------------------------

MqttClient::~MqttClient() {

}

//-------------------------------------------------------------------------------------------------

void MqttClient::begin() {
}

//-------------------------------------------------------------------------------------------------

void MqttClient::shutdown() {
   mShutdown = true;
   disconnect();
}

//-------------------------------------------------------------------------------------------------

bool MqttClient::publish(const char* pTopicName, void* pPayload, size_t pPayloadlen, QoS pQos, bool pRetained) {
   if(mShutdown) {
      LOG("MqttClient::publish: Shutdown!");
      return false;
   }

   MQTT::QoS qos;
   switch(pQos){
      case QoS::QOS0 : qos =  MQTT::QOS0; break;
      case QoS::QOS1 : qos =  MQTT::QOS1; break;
      case QoS::QOS2 : qos =  MQTT::QOS2; break;
   }
   if(mMqttClient.publish(pTopicName, pPayload, pPayloadlen, qos, pRetained) == 0) {
      return true;
   }
   disconnect();
   return false;
}

bool MqttClient::publish(const char* pTopicName, const char* pMessage, QoS pQos, bool pRetained) {
   LOG("MQTT publish " << pTopicName << " - " << pMessage << " [" << strlen(pMessage) << "]" );
   return publish(pTopicName, const_cast<char*>(pMessage), strlen(pMessage), pQos, pRetained);
}



//-------------------------------------------------------------------------------------------------

void MqttClient::stateChanged(Net::Gprs::GprsModule::State pState) {

   switch(pState){
      case Net::Gprs::GprsModule::State::OFF                     : mState = State::OFF; break;
      case Net::Gprs::GprsModule::State::PIN_OK                  : mState = State::PIN_OK; break;
      case Net::Gprs::GprsModule::State::NETWORK_REGISTRATION_OK : mState = State::NETWORK_REGISTRATION_OK; break;
      case Net::Gprs::GprsModule::State::GPRS_READY              : mState = State::GPRS_READY; break;
      case Net::Gprs::GprsModule::State::TCP_CONNECTED           : mState = State::TCP_CONNECTED; break;
   }

   publishState();
}

//-------------------------------------------------------------------------------------------------

void MqttClient::publishState() {
   if(mState == State::TCP_CONNECTED) {
      if (mMqttClient.isConnected()) {
         mState = State::MQTT_CONNECTED;
      }
   }

   if(!mStateListener) {
      return;
   }
   mStateListener(mState);
}

//-------------------------------------------------------------------------------------------------

void MqttClient::onReady() {
   if(mShutdown) {
      LOG("MqttClient::onReady: Shutdown!");
      return;
   }

   LOG("MqttClient::onReady()");
   const char * url = mSettings.brokerAddress;

   if(int rc = mGprsModule->connect(url,1883) != 0) {
      LOG("tcp connect failed with " << rc);
   }
}

//-------------------------------------------------------------------------------------------------

void MqttClient::onConnected() {
   if(mShutdown) {
      LOG("MqttClient::onConnected: Shutdown!");
      return;
   }

   MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
   data.MQTTVersion = 3;
   data.clientID.cstring = (char*)mSettings.clientId;
   data.keepAliveInterval = 10 * 60;
   data.willFlag = true;
   data.will.topicName.cstring = (char*)"device/status";
   data.will.message.cstring = (char*)"0";
   data.will.retained = true;
   data.will.qos = MQTT::QOS1;
   data.username.cstring = (char*)mSettings.username;
   data.password.cstring = (char*)mSettings.password;

   int rc = mMqttClient.connect(data);
   if (rc != 0)
   {
      LOG("connect returned"  << rc);
      return;
   }
   mConnectCounter++;
   publishState();
   LOG("MQTT connected");
   publish("device/status", "1", QoS::QOS1, true);
}

//-------------------------------------------------------------------------------------------------

void MqttClient::onDisconnected() {
   mMqttClient.disconnect();
}

//-------------------------------------------------------------------------------------------------

void MqttClient::disconnect() {
   mMqttClient.disconnect();
   mGprsModule->disconnect();
}

//-------------------------------------------------------------------------------------------------

bool MqttClient::yield(uint32_t pTimeoutInMilliseconds) {
   if(mShutdown) {
      LOG("MqttClient::yield: Shutdown!");
      return false;
   }
   if(mGprsModule->isConnected()) {
      return mMqttClient.yield(pTimeoutInMilliseconds);
   }
   return false;
}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
