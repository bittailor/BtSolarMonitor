//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MqttClient
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MqttClient__hpp
#define INC__Bt_SolarMonitor_MqttClient__hpp

#ifdef ARDUINO
   #include "Bt/SolarMonitor/MqttClient.Platform.Arduino.hpp"
#else
   #include "Bt/SolarMonitor/MqttClient.Platform.Host.hpp"
#endif


#endif // INC__Bt_SolarMonitor_MqttClient__hpp
