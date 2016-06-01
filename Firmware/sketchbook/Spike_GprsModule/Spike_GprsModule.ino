#define FONA_KEY 12
#define FONA_PS A0
#define FONA_RST 9
#define FONA_APN "gprs.swisscom.ch"

#include <BtMcuCore.h>
#include <BtMcuNet.h>
#include <MQTTClient.h>

#include <Bt/Core/Logger.hpp>
#include <Bt/Core/Time.hpp>
#include <Bt/Core/DigitalOut.hpp>
#include <Bt/Core/DigitalIn.hpp>
#include <Bt/Core/Workcycle.hpp>
#include <Bt/Core/StaticStringBuilder.hpp>
#include <Bt/Core/PeriodicCallback.hpp>
#include <Bt/Net/Gprs/MobileTerminal.hpp>
#include <Bt/Net/Gprs/GprsModule.hpp>

using namespace Bt;

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


Core::Time sTime;
Core::DigitalOut sOnOffKey(FONA_KEY);
Core::DigitalOut sReset(FONA_RST);
Core::DigitalIn sPowerState(FONA_PS);
Net::Gprs::MobileTerminal sMobileTerminal(Serial1);

Net::Gprs::GprsModule sGprsModule(sTime,sOnOffKey,sReset,sPowerState,sMobileTerminal);
MQTT::Client<Net::Gprs::I_GprsClient, PahoTimer, 500> sMqttClient(sGprsModule);
Core::Workcycle sWorkcycle;



class Listener : public Net::Gprs::GprsModule::I_Listener {
   public:
      virtual void onReady() {
         //const char * url = "broker.shiftr.io";
         const char * url = "iot.eclipse.org";

         int rc = sGprsModule.connect(url,1883);
         if(rc!= 0) {
            LOG("tcp connect failed with " << rc);
         }
      }

      virtual void onConnected() {
         MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
         data.MQTTVersion = 3;
         data.clientID.cstring = (char*)"bt-resource-check";
         data.keepAliveInterval = 120;
         data.willFlag = true;
         data.will.topicName.cstring = (char*)"BT/GprsModule/Status";
         data.will.message.cstring = (char*)"offline";
         data.will.retained = true;
         data.will.qos = MQTT::QOS0;
         //data.username.cstring = (char*)"f64edae4";
         //data.password.cstring = (char*)"eea9554c6e05c108";


         int rc = sMqttClient.connect(data);
         if (rc != 0)
         {
            LOG("mqtt connect returned "  << rc);
            return;
         }
         LOG("MQTT connected");

         rc = publish("BT/GprsModule/Status","online",MQTT::QOS0,true);
         LOG("MQTT publish returned " << rc);
         mConnected = true;
      }

      void yield(){
         bool gprsConnected = sGprsModule.isConnected();
         bool mqttConnected = sMqttClient.isConnected();
         LOG("GPRS connected "  << gprsConnected);
         LOG("MQTT Connected "  << mqttConnected);

         if(gprsConnected && mqttConnected) {
            int rc = sMqttClient.yield(10);
            return;
         }

         if(!gprsConnected) {
            onReady();
            return;
         }

         if(!mqttConnected) {
            onConnected();
            return;
         }
      }

      void publishTime(){
         if(mConnected){
            sMqttClient.yield(1);
            char msg[30];
            Core::StaticStringBuilder builder(msg,30);
            builder.print(millis());
            int rc = publish("BT/GprsModule/Msg",msg, MQTT::QOS0);
            if(rc != 0) {
               LOG("!!!");
               LOG("!!! MQTT publish failed " << rc);
               LOG("!!!");
            }

         }
      }

      int publish(const char* pTopicName, const char* pMessage, MQTT::QoS pQos, bool pRetained = false) {
         LOG("MQTT publish " << pTopicName << " - " << pMessage << " [" << strlen(pMessage) << "]" );
         return sMqttClient.publish(pTopicName, (void*)pMessage, strlen(pMessage), pQos, pRetained);
      }

      bool mConnected = false;

};

Listener sListener;

Core::PeriodicCallback sYield(sTime,500,Core::Function<void()>::build<Listener,&Listener::yield>(sListener));
Core::PeriodicCallback sPublish(sTime,5000,Core::Function<void()>::build<Listener,&Listener::publishTime>(sListener));

void setup() {
   pinMode(13, OUTPUT);
   digitalWrite(13, HIGH);
   while (!Serial) {}
   Serial.begin(115200);
   Serial.println(F("** Spike GPRS Module**"));
   Serial1.begin(115200);
   digitalWrite(13, LOW);
   delay(1000);


   sWorkcycle.add(sGprsModule);
   sWorkcycle.add(sYield);
   sWorkcycle.add(sPublish);

   sOnOffKey.begin();
   sReset.begin();
   sPowerState.begin();
   sGprsModule.begin(sListener);
}



void loop() {
   sWorkcycle.oneWorkcycle();
}
