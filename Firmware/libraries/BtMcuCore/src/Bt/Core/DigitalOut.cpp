//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::DigitalOut
//
//*************************************************************************************************

#include "Bt/Core/DigitalOut.hpp"

#ifdef ARDUINO
   #include "Bt/Core/DigitalOut.Platform.Arduino.inc"
#else
   #include "Bt/Core/DigitalOut.Platform.Host.inc"
#endif
