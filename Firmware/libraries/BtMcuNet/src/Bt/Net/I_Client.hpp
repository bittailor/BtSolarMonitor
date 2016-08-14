//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::Net::I_Client
//
//*************************************************************************************************

#ifndef INC__Bt_Net_I_Client__hpp
#define INC__Bt_Net_I_Client__hpp

namespace Bt {
namespace Net {

class I_Client {
   public:
      virtual ~I_Client() {}
      virtual bool isConnected()=0;
      virtual int connect(const char* pHostname, int pPort)=0;
      virtual int read(unsigned char* pBuffer, int pLen, int pTimeout)=0;
      virtual int write(unsigned char* pBuffer, int pLen, int pTimeout)=0;
      virtual int disconnect()=0;

};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_I_Client__hpp
