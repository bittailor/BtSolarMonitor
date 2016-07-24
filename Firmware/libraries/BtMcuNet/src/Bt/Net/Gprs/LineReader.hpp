//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::LineReader
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Gprs_LineReader__hpp
#define INC__Bt_Net_Gprs_LineReader__hpp

#include <stddef.h>
#include <ctype.h>
#include <Arduino.h>
#include <Bt/Core/Timer.hpp>
#include <Bt/Core/Logger.hpp>
#include "Bt/Net/Gprs/Return.hpp"


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

      Return<const char*> readLine(Stream& pStream, Bt::Core::Timer& pTimer) {
         mLength = 0;
         mBuffer[mLength] = '\0';
         while(true){
            while (pStream.available()) {
               int character = pStream.read();
               if(character == LF || character > 0x7F) {
                  if(mLength == 0) {
                     continue;
                  }
                  if(onlyWhitespaceInBuffer()){
                     LOG("   !!!! LR - Skip Whitespace Buffer  ");
                     continue;
                  }
                  //LOG("     << LR length " << mLength);
                  return mBuffer;
               }
               if(character == CR) {
                  continue;
               }
               if(character <= 0) {
                  continue;
               }
               if(character > 0x7F) {
                  continue;
               }
               //LOG("     << LR ["<< mLength<< "] = " << character);
               mBuffer[mLength++] = character;
               mBuffer[mLength] = '\0';
            }
            if(pTimer.expired()) {
               if(mLength > 0) {
                  LOG("   !!!! LR - Timeout loose: " << mBuffer);
               }
               return RC_TIMEOUT;
               return RC_TIMEOUT;
            }
         }
      }

      Return<const char*> readPrompt(Stream& pStream, Bt::Core::Timer& pTimer) {
         mLength = 0;
         mBuffer[mLength] = '\0';
         while(true){
            if(pTimer.expired()) {
               if(mLength > 0) {
                  LOG("   !!!! LR - Timeout loose: " << mBuffer);
               }
               return RC_TIMEOUT;
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
               //LOG("     << LR ["<< mLength<< "] = " << character);
               mBuffer[mLength++] = character;
               mBuffer[mLength] = '\0';
               if( strcmp(mBuffer,"> ") == 0) {
                  return mBuffer;
               }
            }
         }
      }

   private:
      bool onlyWhitespaceInBuffer() {
         for(int i=0 ; i < mLength ; i++) {
            if(isspace(mBuffer[i]) == 0) {
               return false;
            }
         }
         return true;
      }


      char mBuffer[LINE_BUFFER_SIZE];
      size_t mLength;
};

} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_LineReader__hpp
