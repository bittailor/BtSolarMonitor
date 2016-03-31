//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#ifndef INC__Bt_Core_I_DigitalOut__hpp
#define INC__Bt_Core_I_DigitalOut__hpp

namespace Bt {
namespace Core {

class I_DigitalOut {
   public:
      virtual ~I_DigitalOut() {}

      virtual void write(bool pValue) = 0;
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_I_DigitalOut__hpp
