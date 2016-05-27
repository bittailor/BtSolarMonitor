//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::Net::Gprs::I_MobileTerminal
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Gprs_I_MobileTerminal__hpp
#define INC__Bt_Net_Gprs_I_MobileTerminal__hpp

#include <Bt/Core/Function.hpp>

namespace Bt {
namespace Net {
namespace Gprs {

class I_MobileTerminal {
   public:
      virtual ~I_MobileTerminal() {}
      virtual bool checkAtOk() = 0;
      virtual bool disableEcho() = 0;
      virtual bool checkAndSetPin(const char* pPin) = 0;
      virtual bool checkNetworkRegistration() = 0;
      virtual bool checkGprsAttachment() = 0;
      virtual bool bringUpWirelessConnection(const char* pApn, const char* pUser = nullptr, const char* pPassword = nullptr) = 0;

      virtual int connect(const char* pHostname, int pPort) = 0;
      virtual bool checkConnected() = 0;
      virtual int write(unsigned char* pBuffer, int pLen, int pTimeout) = 0;
      virtual int read(unsigned char* pBuffer, int pLen, int pTimeout) = 0;

};

} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_I_MobileTerminal__hpp
