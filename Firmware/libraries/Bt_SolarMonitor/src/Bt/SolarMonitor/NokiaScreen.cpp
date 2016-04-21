//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::NokiaScreen
//
//*************************************************************************************************

#include "Bt/SolarMonitor/NokiaScreen.hpp"

#ifdef ARDUINO
   #include "Bt/SolarMonitor/NokiaScreen.Platform.Arduino.inc"
#else
   #include "Bt/SolarMonitor/NokiaScreen.Platform.Host.inc"
#endif
