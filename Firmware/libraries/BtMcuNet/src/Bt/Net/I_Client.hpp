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

      virtual int connect(char* pHostname, int pPort)=0;
      virtual int read(unsigned char* bBuffer, int len, int bTimeout)=0;
      virtual int write(unsigned char* bBuffer, int len, int bTimeout)=0;
      virtual int disconnect()=0;

};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_I_Client__hpp
