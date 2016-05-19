//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::MobileTerminal
//
//*************************************************************************************************

#include "Bt/Net/Gprs/MobileTerminal.hpp"

#include "Bt/Core/Logger.hpp"

namespace Bt {
namespace Net {
namespace Gprs {

#define LOG_MT(msg) LOG(msg)

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

MobileTerminal::MobileTerminal(Stream& pStream)
: mStream(&pStream)
, mLineReader(Bt::Core::Function<void(const char*)>::build<MobileTerminal,&MobileTerminal::handleLine>(*this))
, mIdle(*this)
, mWaitingForResponse(*this)
, mCurrentState(&mIdle)
, mCurrentCommand(nullptr) {

}

MobileTerminal::~MobileTerminal() {

}

uint32_t MobileTerminal::workcycle() {
   if(mCurrentCommand != nullptr)

   while(mStream->available() > 0) {
      int byte = mStream->read();
      mCurrentState->consume(byte);
   }

   return mCurrentState->delay();
}

bool MobileTerminal::checkAtOk(Core::Function<void(bool)> pCallback) {
   if(mCurrentCommand != nullptr) {
      ERROR("already a command running");
      //return false;
   }
   mCurrentCommand = &mAtOkCommand;
   mAtOkCommand.run(*this, pCallback);
   return true;
}

void MobileTerminal::sendCommand(const char* pCommand) {
   flushInput();
   mCurrentState = &mWaitingForResponse;
   sendLine(pCommand);
}

void MobileTerminal::sendLine(const char* pLine) {
   LOG_MT("  ---> " << pLine );
   mStream->println(pLine);
}

void MobileTerminal::handleLine(const char* pLine) {
   LOG_MT("  <--- " << pLine );
   if(mCurrentCommand != nullptr) {
      bool done = mCurrentCommand->handleLine(pLine);
      if(done) {
         mCurrentCommand = nullptr;
      }
   }
}

void MobileTerminal::flushInput() {
   while(mStream->available()) {
      mStream->read();
   }
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void MobileTerminal::AtOkCommand::run(MobileTerminal& pTerminal, Core::Function<void(bool)> pCallback) {
   mCallback = pCallback;
   pTerminal.sendCommand("AT");
}

bool MobileTerminal::AtOkCommand::handleLine(const char* pLine) {
   if (strcmp(pLine, "AT") == 0) {
      return false;
   }
   if (strcmp(pLine, "OK") == 0) {
      mCallback(true);
   }
   mCallback(false);
   return true;
}



//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------


} // namespace Gprs
} // namespace Net
} // namespace Bt


