//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::AveragingMeasurementRecord
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_AveragingMeasurementRecord__hpp
#define INC__Bt_SolarMonitor_AveragingMeasurementRecord__hpp

#include <stddef.h>
#include <Bt/SolarMonitor/MeasurementRecord.hpp>

namespace Bt {
namespace SolarMonitor {

class AveragingMeasurementRecord
{
   public:
      AveragingMeasurementRecord();
      ~AveragingMeasurementRecord();

      void add(const MeasurementRecord& pMeasurementRecord);
      MeasurementRecord averageAndClear();

   private:
   	  // Constructor to prohibit copy construction.
      AveragingMeasurementRecord(const AveragingMeasurementRecord&);

      // Operator= to prohibit copy assignment
      AveragingMeasurementRecord& operator=(const AveragingMeasurementRecord&);

      void clear();
      MeasurementRecord average();

      size_t mCounter;
      MeasurementRecord mSum;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_AveragingMeasurementRecord__hpp
