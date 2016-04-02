//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Timing
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Timing__hpp
#define INC__Bt_Core_Timing__hpp

#include <stdint.h>

namespace Bt {
namespace Core {

void delayInMilliseconds(uint32_t milliseconds);
void delayInMicroseconds(uint32_t microseconds);

uint32_t milliseconds();
uint32_t microseconds();

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Timing__hpp
