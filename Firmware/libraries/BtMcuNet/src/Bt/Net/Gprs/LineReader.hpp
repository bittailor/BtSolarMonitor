//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::LineReader
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Gprs_LineReader__hpp
#define INC__Bt_Net_Gprs_LineReader__hpp

#include <stddef.h>

#include <Bt/Core/Function.hpp>

namespace Bt {
namespace Net {
namespace Gprs {

template<int LINE_BUFFER_SIZE=256>
class LineReader
{
   public:
      static const char LF = 0x0A;
      static const char CR = 0x0D;

      LineReader(Bt::Core::Function<void(const char*)> pLineHandler)
      :mBuffer{'\0'}, mLength{0}, mLineHandler(pLineHandler){
      }

      LineReader(const LineReader&)=delete;
      LineReader& operator=(const LineReader&)=delete;

      ~LineReader(){}

      void consume(char pCharacter) {
         if(pCharacter == LF) {
            if(mLength > 0) {
               mLineHandler(mBuffer);
               mLength = 0;
               mBuffer[mLength] = '\0';
            }
            return;
         }
         if(pCharacter == CR) {
            return;
         }
         mBuffer[mLength++] = pCharacter;
         mBuffer[mLength] = '\0';
      }

   private:
      char mBuffer[LINE_BUFFER_SIZE];
      size_t mLength;
      Bt::Core::Function<void(const char*)> mLineHandler;
};

} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_LineReader__hpp
