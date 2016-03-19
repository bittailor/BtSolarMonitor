//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::SolarMonitor::I_RelayControllerQueryPort
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_I_RelayControllerQueryPort__hpp
#define INC__Bt_SolarMonitor_I_RelayControllerQueryPort__hpp

namespace Bt {
namespace SolarMonitor {

class I_RelayControllerQueryPort {
   public:
      virtual ~I_RelayControllerQueryPort() {}

      virtual bool loadASense()=0;
      virtual bool loadBSense()=0;
      virtual bool loadOut()=0;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_I_RelayControllerQueryPort__hpp
