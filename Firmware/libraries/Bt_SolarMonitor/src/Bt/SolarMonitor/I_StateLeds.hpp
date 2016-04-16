//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::SolarMonitor::I_StateLeds
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_I_StateLeds__hpp
#define INC__Bt_SolarMonitor_I_StateLeds__hpp

namespace Bt {
namespace SolarMonitor {

class I_StateLeds {
   public:
      enum State {Off, OnA, OnB};

      virtual ~I_StateLeds() {}

      virtual void show(State pState)=0;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_I_StateLeds__hpp
