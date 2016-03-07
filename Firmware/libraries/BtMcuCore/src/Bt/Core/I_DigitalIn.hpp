//*************************************************************************************************
//
//  BITTAILOR.CH - Firmware
//
//*************************************************************************************************

#ifndef INC__Bt_Core_I_DigitalIn__hpp
#define INC__Bt_Core_I_DigitalIn__hpp

namespace Bt {
namespace Core {

class I_DigitalIn
{
   public:
      virtual ~I_DigitalIn() {}
      virtual bool read() = 0;
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_I_DigitalIn__hpp
