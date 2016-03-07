#ifndef INC__Bt_Core_I_InterruptIn__hpp
#define INC__Bt_Core_I_InterruptIn__hpp

namespace Bt {
namespace Core {

class I_InterruptIn
{
   public:
      virtual ~I_InterruptIn() {}

      virtual bool read()=0;

};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_I_InterruptIn__hpp
