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
      virtual bool checkAtOk(Core::Function<void(bool)> pCallback) = 0;
};

} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_I_MobileTerminal__hpp
