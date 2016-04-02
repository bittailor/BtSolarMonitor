//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::SolarMonitor::I_LatchingRelay
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_I_LatchingRelay__hpp
#define INC__Bt_SolarMonitor_I_LatchingRelay__hpp

namespace Bt {
namespace SolarMonitor {

class I_LatchingRelay {
   public:
      virtual ~I_LatchingRelay() {}

      virtual void onCoil(bool pValue) = 0;
      virtual void offCoil(bool pValue) = 0;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_I_LatchingRelay__hpp
