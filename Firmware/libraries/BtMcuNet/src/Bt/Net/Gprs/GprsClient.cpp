//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::GprsClient
//
//*************************************************************************************************

#include "Bt/Net/Gprs/GprsClient.hpp"

namespace Bt {
namespace Net {
namespace Gprs {


//-------------------------------------------------------------------------------------------------

GprsClient::GprsClient() {

}

//-------------------------------------------------------------------------------------------------

GprsClient::~GprsClient() {

}

//-------------------------------------------------------------------------------------------------

int GprsClient::connect(char* pHostname, int pPort) {
   return -1;
}

//-------------------------------------------------------------------------------------------------

int GprsClient::read(unsigned char* bBuffer, int len, int bTimeout) {
   return -1;
}

//-------------------------------------------------------------------------------------------------

int GprsClient::write(unsigned char* bBuffer, int len, int bTimeout) {
   return -1;
}

//-------------------------------------------------------------------------------------------------

int GprsClient::disconnect() {
   return -1;
}

//-------------------------------------------------------------------------------------------------
} // namespace Gprs
} // namespace Net
} // namespace Bt