//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Wire
//
//*************************************************************************************************

#include "Bt/Core/Wire.hpp"

#ifdef ARDUINO
   #include "Bt/Core/Wire.Platform.Arduino.inc"
#else
   #include "Bt/Core/Wire.Platform.Host.inc"
#endif
