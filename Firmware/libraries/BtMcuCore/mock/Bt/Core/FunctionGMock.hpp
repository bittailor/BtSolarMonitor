//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#ifndef INC__Bt_Core_FunctionGMock__hpp
#define INC__Bt_Core_FunctionGMock__hpp

#include <gmock/gmock.h>

namespace Bt {
namespace Core {

template<typename R>
class FunctionGMock {
   public:

      MOCK_METHOD0_T(call, R());
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_FunctionGMock__hpp
