//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::Net::Gprs::I_MobileTerminal
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Gprs_I_MobileTerminal__hpp
#define INC__Bt_Net_Gprs_I_MobileTerminal__hpp

#include <Bt/Core/Function.hpp>
#include "Bt/Net/Gprs/Return.hpp"


namespace Bt {
namespace Net {
namespace Gprs {

enum  ConnectionStatus  {
   IP_INITIAL,
   IP_START,
   IP_CONFIG,
   IP_GPRSACT,
   IP_STATUS,
   CONNECTING,
   CONNECT_OK,
   CLOSING,
   CLOSED,
   PDP_DEACT
};

class I_MobileTerminal {
   public:
      virtual ~I_MobileTerminal() {}

      virtual Return<bool> checkAtOk() = 0;
      virtual Return<void> disableEcho() = 0;
      virtual Return<void> checkAndSetPin(const char* pPin) = 0;
      virtual Return<bool> checkNetworkRegistration() = 0;
      virtual Return<int>  getRSSI() = 0;
      virtual Return<bool> checkGprsAttachment() = 0;
      virtual Return<void> bringUpWirelessConnection(const char* pApn, const char* pUser = nullptr, const char* pPassword = nullptr) = 0;
      virtual Return<void> connect(const char* pHostname, int pPort) = 0;
      virtual Return<void> close() =0 ;
      virtual Return<ConnectionStatus> getConnectionStatus() = 0;
      virtual Return<int> write(unsigned char* pBuffer, int pLen, int pTimeout) = 0;
      virtual Return<int> read(unsigned char* pBuffer, int pLen, int pTimeout) = 0;
      virtual Return<void> enableSleepMode() = 0;
      virtual Return<void> disableSleepMode() = 0;
      virtual Return<void> powerDown() = 0;











};

} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_I_MobileTerminal__hpp
