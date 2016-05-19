//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::GprsClient
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Gprs_GprsClient__hpp
#define INC__Bt_Net_Gprs_GprsClient__hpp

#include "Bt/Net/Gprs/I_GprsClient.hpp"

namespace Bt {
namespace Net {
namespace Gprs {

class GprsClient : public I_GprsClient
{
   public:
      GprsClient();
      ~GprsClient();

      int connect(char* pHostname, int pPort);
      int read(unsigned char* bBuffer, int len, int bTimeout);
      int write(unsigned char* bBuffer, int len, int bTimeout);
      int disconnect();

   private:
   	  // Constructor to prohibit copy construction.
      GprsClient(const GprsClient&);

      // Operator= to prohibit copy assignment
      GprsClient& operator=(const GprsClient&);
};

} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_GprsClient__hpp
