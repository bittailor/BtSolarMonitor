//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::AveragingMeasurementRecord
//
//*************************************************************************************************

#include "Bt/SolarMonitor/AveragingMeasurementRecord.hpp"

namespace Bt {
namespace SolarMonitor {


//-------------------------------------------------------------------------------------------------

AveragingMeasurementRecord::AveragingMeasurementRecord()
: mCounter(0)
, mSum() {

}

//-------------------------------------------------------------------------------------------------

AveragingMeasurementRecord::~AveragingMeasurementRecord() {

}

//-------------------------------------------------------------------------------------------------

void AveragingMeasurementRecord::add(const MeasurementRecord& pMeasurementRecord) {
   mSum += pMeasurementRecord;
   mCounter++;
}

//-------------------------------------------------------------------------------------------------

void AveragingMeasurementRecord::clear() {
   mCounter = 0;
   mSum = MeasurementRecord();
}


//-------------------------------------------------------------------------------------------------

MeasurementRecord AveragingMeasurementRecord::average() {
   if(mCounter==0) {
      return MeasurementRecord();
   }
   return mSum / mCounter;
}

//-------------------------------------------------------------------------------------------------

MeasurementRecord AveragingMeasurementRecord::averageAndClear() {
   MeasurementRecord averageValue = average();
   clear();
   return averageValue;
}

//-------------------------------------------------------------------------------------------------

} // namespace SolarMonitor
} // namespace Bt
