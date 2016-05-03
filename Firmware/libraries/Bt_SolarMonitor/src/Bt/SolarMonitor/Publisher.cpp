//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Publisher
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Publisher.hpp"

#include <string.h>
#include <stdio.h>

namespace Bt {
namespace SolarMonitor {

namespace {

#define PREFIX "BSM/S/"

   Publisher::Topic sTopics[] = {
      PREFIX "PA/I",
      PREFIX "PA/V",
      PREFIX "PB/I",
      PREFIX "PB/V",
      PREFIX "BA/I",
      PREFIX "BA/V",
      PREFIX "BB/I",
      PREFIX "BB/V",
      PREFIX "L/I" ,
      PREFIX "L/V" ,
      PREFIX "C/I" ,
      PREFIX "C/V"
   };
}

Publisher::Publisher(I_MqttClient& pMqttClient) : mMqttClient(&pMqttClient) {

}

Publisher::~Publisher() {

}

void Publisher::begin() {
}

void Publisher::publish(const MeasurementRecord& pMeasurementRecord) {
   publish(&sTopics[0], pMeasurementRecord.panelA());
   publish(&sTopics[2], pMeasurementRecord.panelB());
   publish(&sTopics[4], pMeasurementRecord.batteryA());
   publish(&sTopics[6], pMeasurementRecord.batteryB());
   publish(&sTopics[8], pMeasurementRecord.load());
   publish(&sTopics[10], pMeasurementRecord.control());
}

void Publisher::publish(Topic pTopics[], const Measurement& pMeasurement) {
   publish(pTopics[0], pMeasurement.current());
   publish(pTopics[1], pMeasurement.voltage());
}

void Publisher::publish(Topic pTopic, float pValue) {
   static const size_t LOCAL_BUFFER_SIZE = 10;
   char buffer[LOCAL_BUFFER_SIZE] = {0};
   snprintf(buffer, LOCAL_BUFFER_SIZE, "%2.4f", pValue);
   mMqttClient->publish(pTopic, buffer);
}

} // namespace SolarMonitor
} // namespace Bt
