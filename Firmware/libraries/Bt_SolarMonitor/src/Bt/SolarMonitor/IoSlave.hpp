//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::IoSlave
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_IoSlave__hpp
#define INC__Bt_SolarMonitor_IoSlave__hpp

#include "Bt/Core/I_Wire.hpp"

#include "Bt/SolarMonitor/I_IoSlave.hpp"

namespace Bt {
namespace SolarMonitor {

class IoSlave : public I_IoSlave
{
   public:
      IoSlave(Core::I_Wire& pWire);
      ~IoSlave();

      virtual I_PowerState::State powerState();

   private:
   	  // Constructor to prohibit copy construction.
      IoSlave(const IoSlave&);

      // Operator= to prohibit copy assignment
      IoSlave& operator=(const IoSlave&);

      Core::I_Wire* mWire;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_IoSlave__hpp
