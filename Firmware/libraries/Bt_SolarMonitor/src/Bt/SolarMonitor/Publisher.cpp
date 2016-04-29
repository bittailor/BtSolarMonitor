//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Publisher
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Publisher.hpp"

namespace Bt {
namespace SolarMonitor {

Publisher::Publisher(I_MqttClient& pMqttClient) : mMqttClient(&pMqttClient) {

}

Publisher::~Publisher() {

}

void Publisher::begin() {
}

void Publisher::loop() {

}

void Publisher::publish(const MeasurementRecord& pMeasurementRecord) {

}

} // namespace SolarMonitor
} // namespace Bt
