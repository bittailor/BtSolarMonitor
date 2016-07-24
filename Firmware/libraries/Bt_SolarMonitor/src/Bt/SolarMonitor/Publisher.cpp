//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Publisher
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Publisher.hpp"

#include <string.h>
#include <stdio.h>
#include "Bt/Core/Logger.hpp"
#include "Bt/Core/Timing.hpp"
#include "Bt/Core/StaticStringBuilder.hpp"

namespace Bt {
namespace SolarMonitor {

namespace {

//   #define TOPIC "BT/BSM/v2/feeds/838919833.json"
// #define TOPIC "/v2/feeds/838919833.csv"
   #define TOPIC "BT/Solar/sens"

   typedef const char* FeedId;

   void addMeasurement(Print&  pBuilder, FeedId pFeedId, float pValue) {
      //pBuilder.print(pFeedId);
      //pBuilder.print(",");
      pBuilder.print(pValue,4);
   }

   void addMeasurementRecord(Print&  pBuilder, FeedId pFeedIds[], const Measurement& pMeasurement) {
      addMeasurement(pBuilder, pFeedIds[0], pMeasurement.current());
      pBuilder.print(";");
      addMeasurement(pBuilder, pFeedIds[1], pMeasurement.voltage());
   }

   FeedId sFeeds[] = {
      "PA.I",
      "PA.V",
      "PB.I",
      "PB.V",
      "BA.I",
      "BA.V",
      "BB.I",
      "BB.V",
      "L.I" ,
      "L.V" ,
      "C.I" ,
      "C.V"
   };
}

Publisher::Publisher(I_MqttClient& pMqttClient) : mMqttClient(&pMqttClient) {

}

Publisher::~Publisher() {

}

void Publisher::begin() {
}

void Publisher::publish(const MeasurementRecord& pMeasurementRecord, uint32_t pReconnectCounter) {

   if(!mMqttClient->isConnected()){
      LOG("!!! mqtt not connected => skip publish !!!");
   }


   char message[500] = {0};
   Bt::Core::StaticStringBuilder builder(message,sizeof(message)/sizeof(message[0]));

   addMeasurementRecord(builder, &sFeeds[0], pMeasurementRecord.panelA());
   builder.print(";");
   addMeasurementRecord(builder, &sFeeds[2], pMeasurementRecord.panelB());
   builder.print(";");
   addMeasurementRecord(builder, &sFeeds[4], pMeasurementRecord.batteryA());
   builder.print(";");
   addMeasurementRecord(builder, &sFeeds[6], pMeasurementRecord.batteryB());
   builder.print(";");
   addMeasurementRecord(builder, &sFeeds[8], pMeasurementRecord.load());
   builder.print(";");
   addMeasurementRecord(builder, &sFeeds[10], pMeasurementRecord.control());
   builder.print(";");
   //builder.print("T");
   //builder.print(",");
   builder.print(Core::milliseconds());
   builder.print(";");
   //builder.print("C");
   //builder.print(",");
   builder.print(mPublishCounter++);
   builder.print(";");
   //builder.print("R");
   //builder.print(",");
   builder.print(pReconnectCounter);





   LOG("message length = " << strlen(message));
   LOG("message " << message);
   mMqttClient->publish(TOPIC, message, I_MqttClient::QOS1);
   //mMqttClient->publish(TOPIC, "HELLO");
}


} // namespace SolarMonitor
} // namespace Bt
