//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Publisher
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Publisher.hpp"

#include <string.h>
#include <stdio.h>
#include "Bt/Core/Logger.hpp"
#include "Bt/Core/StaticStringBuilder.hpp"

namespace Bt {
namespace SolarMonitor {

namespace {

   #define TOPIC "/v2/feeds/838919833.json"

   typedef const char* FeedId;

   void addMeasurement(Print&  pBuilder, FeedId pFeedId, float pValue) {
      pBuilder.print(pFeedId);
      pBuilder.print(", ");
      pBuilder.println(pValue,4);
   }

   void addMeasurementRecord(Print&  pBuilder, FeedId pFeedIds[], const Measurement& pMeasurement) {
      addMeasurement(pBuilder, pFeedIds[0], pMeasurement.current());
      addMeasurement(pBuilder, pFeedIds[1], pMeasurement.voltage());
   }

   FeedId sFeeds[] = {
      "PanelA.I",
      "PanelA.V",
      "PanelA.I",
      "PanelA.V",
      "BatteryA.I",
      "BatteryA.V",
      "BatteryB.I",
      "BatteryB.V",
      "Load.I" ,
      "load.V" ,
      "Control.I" ,
      "Control.V"
   };
}

Publisher::Publisher(I_MqttClient& pMqttClient) : mMqttClient(&pMqttClient) {

}

Publisher::~Publisher() {

}

void Publisher::begin() {
}

void Publisher::publish(const MeasurementRecord& pMeasurementRecord) {

   if(!mMqttClient->isConnected()){
      LOG("!!! mqtt not connected => skip publish !!!");
   }


   char message[500] = {0};
   Bt::Core::StaticStringBuilder builder(message,sizeof(message)/sizeof(message[0]));

   addMeasurementRecord(builder, &sFeeds[0], pMeasurementRecord.panelA());
//   addMeasurementRecord(builder, &sFeeds[2], pMeasurementRecord.panelB());
//   addMeasurementRecord(builder, &sFeeds[4], pMeasurementRecord.batteryA());
//   addMeasurementRecord(builder, &sFeeds[6], pMeasurementRecord.batteryB());
//   addMeasurementRecord(builder, &sFeeds[8], pMeasurementRecord.load());
//   addMeasurementRecord(builder, &sFeeds[10], pMeasurementRecord.control());
   builder.print('\0');

   LOG("message length = " << strlen(message));
   LOG("message " << message);
   mMqttClient->publish(TOPIC, message);
   //mMqttClient->publish(TOPIC, "HELLO");
}


} // namespace SolarMonitor
} // namespace Bt
