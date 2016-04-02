//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::SolarMonitor::IOSlavePins
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_IOSlavePins__hpp
#define INC__Bt_SolarMonitor_IOSlavePins__hpp

#ifdef ARDUINO
   #include <Arduino.h>
#else
   #define A0 14
   #define A1 15
   #define A2 16
   #define A3 17
   #define A4 18
   #define A5 19
   #define A6 20
   #define A7 21
#endif

// BT_SOLARMONITOR_IOSLAVE_PIN

#define BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_ON_OFF      2
#define BT_SOLARMONITOR_IOSLAVE_PIN_BUTTON_A_B         3

#define BT_SOLARMONITOR_IOSLAVE_PIN_Sense_L           A0
#define BT_SOLARMONITOR_IOSLAVE_PIN_Sense_A           A1
#define BT_SOLARMONITOR_IOSLAVE_PIN_Sense_B           A2

#define BT_SOLARMONITOR_IOSLAVE_PIN_LED_A             A7
#define BT_SOLARMONITOR_IOSLAVE_PIN_LED_B             A6

#define BT_SOLARMONITOR_IOSLAVE_PIN_RELAY_A_ON         5
#define BT_SOLARMONITOR_IOSLAVE_PIN_RELAY_A_OFF        6
#define BT_SOLARMONITOR_IOSLAVE_PIN_RELAY_B_ON         8
#define BT_SOLARMONITOR_IOSLAVE_PIN_RELAY_B_OFF        7
#define BT_SOLARMONITOR_IOSLAVE_PIN_RELAY_LOAD_ON      9
#define BT_SOLARMONITOR_IOSLAVE_PIN_RELAY_LOAD_OFF    10



#endif // INC__Bt_SolarMonitor_IOSlavePins__hpp
