//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::SolarMonitor::I_Publisher
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_I_Publisher__hpp
#define INC__Bt_SolarMonitor_I_Publisher__hpp

#include "Bt/SolarMonitor/MeasurementRecord.hpp"

namespace Bt {
namespace SolarMonitor {

class I_Publisher {
   public:
      virtual ~I_Publisher() {}

      virtual void publish(const MeasurementRecord& pMeasurementRecord) = 0;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_I_Publisher__hpp
