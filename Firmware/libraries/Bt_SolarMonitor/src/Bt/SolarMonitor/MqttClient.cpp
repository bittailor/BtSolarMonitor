//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MqttClient
//
//*************************************************************************************************

#include "Bt/SolarMonitor/MqttClient.hpp"

#ifdef ARDUINO
   #include "Bt/SolarMonitor/MqttClient.Platform.Arduino.inc"
#else
   #include "Bt/SolarMonitor/MqttClient.Platform.Host.inc"
#endif
