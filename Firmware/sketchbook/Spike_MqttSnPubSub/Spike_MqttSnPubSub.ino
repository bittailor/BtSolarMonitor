/*
   Basic MQTT-SN example 
 
   - connects to an MQTT-SN gateway
   - reads a string from Serial and publishes the PUBLISH_TOPIC ("Bt/Ex/PubSub/Out") topic
   - subscribes to the SUBSCRIBE_TOPIC ("Bt/Ex/PubSub/In") topic and writes the reiceived data to the Serial 
*/

#include <SPI.h>
#include <BtMqttSn.h>

//-----

#define CLIENT_NODE_ID 1
#define GATEWAY_NODE_ID 0

#define CLIENT_ID "BtMqttPupSubExample"
#define PUBLISH_TOPIC "Bt/Ex/PubSub/Out"
#define SUBSCRIBE_TOPIC "Bt/Ex/PubSub/In"

//-----


class SerialPacketSocket : public Bt::I_RfPacketSocket {

   public:
      SerialPacketSocket(Stream& iStream): mStream(&iStream) {
      }

      virtual bool send(uint8_t* iPayload, size_t iSize, uint8_t iNodeId ) {
         return mStream->write(iPayload,iSize) == iSize;
      }

      virtual int32_t receive(uint8_t* oPayload, size_t iMaxSize, uint8_t* oNodeId) {
         int32_t size = 0;
         while(mStream->available() > 0) {
            oPayload[size] = mStream->read();
            size++;
            if(size >= iMaxSize) {
               return size;
            }
         }
         return size;
      }

      virtual bool available() {
         return mStream->available() > 0;
      }

      virtual void suspend() {
      }

      virtual void resume() {
      }

   private:
      Stream* mStream;
};


SerialPacketSocket serialPacketSocket(Serial1);
Bt::MqttSnClient client(serialPacketSocket, GATEWAY_NODE_ID, CLIENT_ID, &subscribeCallback);




void setup() {
   Serial.begin(9600);
   Serial << endl << endl << endl << "*** MQTT-SN PubSub example ***" << endl;
   Serial << endl; 
   Serial << " - Node      = " << CLIENT_NODE_ID << endl;
   Serial << " - Pup-Topic = " << PUBLISH_TOPIC << endl;
   Serial << " - Sub-Topic = " << SUBSCRIBE_TOPIC << endl;
   Serial << "try connect ..." << endl;

   while (!client.connect(30)) {
      Serial << "... connect failed ..." << endl;
      delay(1000);   
      Serial << "... retry connect ..." << endl;
   }
  
   Serial << "... connected" << endl;

   Serial << "subscribe" << endl;

   client.subscribe(SUBSCRIBE_TOPIC);

   Serial << "out> <Start>" << endl;

   client.publish(PUBLISH_TOPIC,"<Start>");
   
}

void loop() {
   client.loop();     
   if (Serial.available()) {
      String out = readString();
      Serial << "out> " << out << endl;
      client.publish(PUBLISH_TOPIC,out.c_str());
      if (out == "<quit>") {
         quit();
      }
   }
}

void subscribeCallback(const char* iTopic, const char*  iData) {
  Serial << "in> " << iData << endl;    
}

String readString() {
  Serial.setTimeout(10);
  String string = Serial.readString();
  Serial.setTimeout(1000);
  return string;
}

void quit() {
   Serial << "<quit>" << endl; 
   client.disconnect();
   Serial << "Hit reboot" << endl; 
   while(true){}  
}
