//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::StaticStringBuilder
//
//*************************************************************************************************

#ifndef INC__Bt_Core_StaticStringBuilder__hpp
#define INC__Bt_Core_StaticStringBuilder__hpp

#include <Arduino.h>

namespace Bt {
namespace Core {

class StaticStringBuilder : public Print {
   public:
      StaticStringBuilder(char* pBuffer, size_t size);
      virtual size_t write(uint8_t pByte);
      void clear();

   private:
      // Constructor to prohibit copy construction.
      StaticStringBuilder(const StaticStringBuilder&);

      // Operator= to prohibit copy assignment
      StaticStringBuilder& operator=(const StaticStringBuilder&);

      char* mBuffer;
      size_t mCapacity;
      size_t mLength;
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_StaticStringBuilder__hpp
