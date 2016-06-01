//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::MobileTerminal
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Gprs_MobileTerminal__hpp
#define INC__Bt_Net_Gprs_MobileTerminal__hpp

#include <Arduino.h>
#include <Bt/Core/I_Runnable.hpp>
#include <Bt/Core/Function.hpp>
#include <Bt/Core/Timer.hpp>
#include <Bt/Net/Gprs/I_MobileTerminal.hpp>
#include <Bt/Net/Gprs/LineReader.hpp>

namespace Bt {
namespace Net {
namespace Gprs {

class MobileTerminal : public I_MobileTerminal
{
   public:
      MobileTerminal(Stream& pStream);
      virtual ~MobileTerminal();
      MobileTerminal(const MobileTerminal&) = delete;
      MobileTerminal& operator=(const MobileTerminal&) = delete;

      virtual Return<bool> checkAtOk();
      virtual Return<void> disableEcho();
      virtual Return<void> checkAndSetPin(const char* pPin);
      virtual Return<bool> checkNetworkRegistration();
      virtual Return<bool> checkGprsAttachment();
      virtual Return<void> bringUpWirelessConnection(const char* pApn, const char* pUser, const char* pPassword);
      virtual Return<void> connect(const char* pHostname, int pPort);
      virtual Return<ConnectionStatus> getConnectionStatus();
      virtual Return<int> write(unsigned char* pBuffer, int pLen, int pTimeout);
      virtual Return<int> read(unsigned char* pBuffer, int pLen, int pTimeout);

   private:

      void sendCommand(const char* pCommand);
      void sendLine(const char* pLine);
      Return<const char*> readLine(Bt::Core::Timer& pTimer);
      Return<const char*> readPrompt(Bt::Core::Timer& pTimer);
      void flushInput();

      Stream* mStream;
      LineReader<> mLineReader;
};

} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_MobileTerminal__hpp
