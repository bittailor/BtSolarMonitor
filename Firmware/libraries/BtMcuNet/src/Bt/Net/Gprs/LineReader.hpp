//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::LineReader
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Gprs_LineReader__hpp
#define INC__Bt_Net_Gprs_LineReader__hpp

#include <stddef.h>
#include <Arduino.h>
#include <Bt/Core/Timer.hpp>

namespace Bt {
namespace Net {
namespace Gprs {

template<int LINE_BUFFER_SIZE=256>
class LineReader
{
   public:
      static const char LF = 0x0A;
      static const char CR = 0x0D;

      LineReader() :mBuffer{'\0'}, mLength{0} {
      }

      LineReader(const LineReader&)=delete;
      LineReader& operator=(const LineReader&)=delete;

      ~LineReader(){}

      const char* readLine(Stream& pStream, Bt::Core::Timer& pTimer) {
         mLength = 0;
         mBuffer[mLength] = '\0';
         while(true){
            if(pTimer.expired()) {
               return nullptr;
            }
            if (pStream.available()) {
               char character = pStream.read();
               if(character == LF) {
                  if(mLength > 0) {
                     return mBuffer;
                  }
                  continue;
               }
               if(character == CR) {
                  continue;
               }
               mBuffer[mLength++] = character;
               mBuffer[mLength] = '\0';
            }
         }
      }

   private:
      char mBuffer[LINE_BUFFER_SIZE];
      size_t mLength;
};

} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_LineReader__hpp
