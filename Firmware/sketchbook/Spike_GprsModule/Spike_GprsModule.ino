#define FONA_KEY 12
#define FONA_PS A0
#define FONA_RST 9

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

#include "Spike_GprsModule_Settings.settings.hpp"

const size_t COMMAND_BUFFER_SIZE = 200;
char sCommandBuffer[COMMAND_BUFFER_SIZE] = {0};
bool sRunning = true;
Bt::Core::StaticStringBuilder sCommandBuilder(sCommandBuffer, COMMAND_BUFFER_SIZE);

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
Net::Gprs::GprsModule sGprsModule(sGprsModuleSettings, sTime,sOnOffKey,sReset,sPowerState,sMobileTerminal);
MQTT::Client<Net::Gprs::I_GprsClient, PahoTimer, 500> sMqttClient(sGprsModule);
Core::Workcycle sWorkcycle;


void handleAction(MQTT::MessageData& pMessageData) {
   MQTT::Message& message = pMessageData.message;
   char payload[message.payloadlen + 1];
   memcpy(payload,message.payload,message.payloadlen);
   payload[message.payloadlen] = 0;
   Serial.print("Message ");
   Serial.print(" arrived: qos ");
   Serial.print(message.qos);
   Serial.print(", retained ");
   Serial.print(message.retained);
   Serial.print(", dup ");
   Serial.print(message.dup);
   Serial.print(", packetid ");
   Serial.println(message.id);
   Serial.print("Payload ");
   Serial.println(payload);
}

class Listener : public Net::Gprs::GprsModule::I_Listener {
   public:
      virtual void stateChanged(Net::Gprs::GprsModule::State pState) {
         LOG("stateChanged " << static_cast<int>(pState));
      }

      virtual void onReady() {
         const char * url = MQTT_BROKER;

         int rc = sGprsModule.connect(url,1883);
         if(rc!= 0) {
            LOG("tcp connect failed with " << rc);
         }
      }

      virtual void onConnected() {
         MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
         data.MQTTVersion = 3;
         data.clientID.cstring = (char*)MQTT_CLIENT_ID;
         data.keepAliveInterval = 120;
         data.willFlag = true;
         data.will.topicName.cstring = (char*)"device/state";
         data.will.message.cstring = (char*)"offline";
         data.will.retained = true;
         data.will.qos = MQTT::QOS1;
         data.username.cstring = (char*)MQTT_USERNAME;
         data.password.cstring = (char*)MQTT_PASSWORD;

         int rc = sMqttClient.connect(data);
         if (rc != 0)
         {
            LOG("!!! mqtt connect failed "  << rc);
            sGprsModule.disconnect();
            return;
         }
         LOG("MQTT connected");
         sMqttClient.subscribe("spike/action", MQTT::QOS1, &handleAction);

         rc = publish("device/state","online",MQTT::QOS1,true);
         LOG("MQTT publish returned " << rc);
         mConnected = true;
      }

      virtual void onDisconnected() {
         mConnected = false;
      }

      void publishTime(){
         if(mConnected){
            sMobileTerminal.disableSleepMode();
            sMqttClient.yield(1);
            char msg[30];
            Core::StaticStringBuilder builder(msg,30);
            builder.print(millis());
            int rc = publish("spike/millis",msg, MQTT::QOS1);
            if(rc != 0) {
               LOG("!!! MQTT publish failed " << rc);
            } else {
               LOG("MQTT publish success ");
            }
            sMqttClient.yield(10);
            sMobileTerminal.enableSleepMode();

         }
      }

      int publish(const char* pTopicName, const char* pMessage, MQTT::QoS pQos, bool pRetained = false) {
         LOG("MQTT publish " << pTopicName << " - " << pMessage << " [" << strlen(pMessage) << "]" );
         return sMqttClient.publish(pTopicName, (void*)pMessage, strlen(pMessage), pQos, pRetained);
      }

      bool mConnected = false;

};

Listener sListener;

//Core::PeriodicCallback sYield(sTime,500,Core::Function<void()>::build<Listener,&Listener::yield>(sListener));
Core::PeriodicCallback sPublish(sTime,20000,Core::Function<void()>::build<Listener,&Listener::publishTime>(sListener));

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
   sWorkcycle.add(sPublish);

   sOnOffKey.begin();
   sReset.begin();
   sPowerState.begin();
   sGprsModule.begin(sListener);
}

void loop() {
   sWorkcycle.oneWorkcycle();
   //sMqttClient.yield(10);

   while(Serial.available()) {
      char c = Serial.read();
      if(c == '\n') {
         executeCommand(sCommandBuffer);
         sCommandBuilder.clear();
      } else {
         Serial.print(" ++ Append '");
         Serial.print(c);
         Serial.println("'");
         sCommandBuilder.print(c);
      }
   }

}

void executeCommand(const char* pCommand) {
   if(strcmp("s",pCommand)==0) {
      shutdown();
   }else if(strcmp("shutdown",pCommand)==0) {
      shutdown();
   } else {
      Serial.print("Unknown command: '");
      Serial.print(pCommand);
      Serial.println("'");
   }
}

void shutdown() {
   sRunning = false;
   Serial.println("start shutdown ...");
   sMqttClient.disconnect();
   sGprsModule.disconnect();
   Serial.println("... shutdown done");
   while (true) {
      Serial.println("Ready to reboot ...");
      delay(2000);
   }
}

