//*************************************************************************************************
//
//  BITTAILOR.CH - Firmware     
//
//*************************************************************************************************

#include "Bt/Core/DigitalIn.hpp"

#ifdef ARDUINO
   #include "Bt/Core/DigitalIn.Platform.Arduino.inc"
#else
   #include "Bt/Core/DigitalIn.Platform.Host.inc"
#endif
