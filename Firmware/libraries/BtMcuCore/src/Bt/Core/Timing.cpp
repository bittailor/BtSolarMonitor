//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Timing
//
//*************************************************************************************************

#include "Bt/Core/Timing.hpp"

namespace Bt {
namespace Core {

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

} // namespace Core
} // namespace Bt

#ifdef ARDUINO
   #include "Bt/Core/Timing.Platform.Arduino.inc"
#else
   #include "Bt/Core/Timing.Platform.HostNative.inc"
#endif
