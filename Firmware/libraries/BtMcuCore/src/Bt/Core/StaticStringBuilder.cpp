//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::StaticStringBuilder
//
//*************************************************************************************************

#include "Bt/Core/StaticStringBuilder.hpp"

namespace Bt {
namespace Core {


//-------------------------------------------------------------------------------------------------

StaticStringBuilder::StaticStringBuilder(char* pBuffer, size_t pSize)
: mBuffer(pBuffer), mCapacity(pSize - 1), mLength(0) {
   mBuffer[0] = '\0';
}

//-------------------------------------------------------------------------------------------------

size_t StaticStringBuilder::write(uint8_t pByte) {
   if (mLength >= mCapacity) {
      return 0;
   }
   mBuffer[mLength++] = pByte;
   mBuffer[mLength] = '\0';
   return 1;
}

//-------------------------------------------------------------------------------------------------

} // namespace Core
} // namespace Bt
