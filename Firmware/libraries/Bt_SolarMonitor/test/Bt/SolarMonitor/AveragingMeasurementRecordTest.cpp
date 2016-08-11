//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/SolarMonitor/AveragingMeasurementRecord.hpp"

namespace Bt {
namespace SolarMonitor {

TEST(AveragingMeasurementRecordTest, averageSimpleOverAll) {
   AveragingMeasurementRecord mAveragingMeasurementRecord;

   MeasurementRecord record{{1,10},{2,20},{3,30},{4,40},{5,50},{6,60}};
   mAveragingMeasurementRecord.add(record);
   mAveragingMeasurementRecord.add(record);
   mAveragingMeasurementRecord.add(record);
   mAveragingMeasurementRecord.add(record);
   mAveragingMeasurementRecord.add(record);
   mAveragingMeasurementRecord.add(record);
   MeasurementRecord average = mAveragingMeasurementRecord.averageAndClear();

   EXPECT_FLOAT_EQ(record.panelA().current(),average.panelA().current());
   EXPECT_FLOAT_EQ(record.panelA().voltage(),average.panelA().voltage());

   EXPECT_FLOAT_EQ(record.panelB().current(),average.panelB().current());
   EXPECT_FLOAT_EQ(record.panelB().voltage(),average.panelB().voltage());

   EXPECT_FLOAT_EQ(record.batteryA().current(),average.batteryA().current());
   EXPECT_FLOAT_EQ(record.batteryA().voltage(),average.batteryA().voltage());

   EXPECT_FLOAT_EQ(record.batteryB().current(),average.batteryB().current());
   EXPECT_FLOAT_EQ(record.batteryB().voltage(),average.batteryB().voltage());

   EXPECT_FLOAT_EQ(record.load().current(),average.load().current());
   EXPECT_FLOAT_EQ(record.load().voltage(),average.load().voltage());

   EXPECT_FLOAT_EQ(record.control().current(),average.control().current());
   EXPECT_FLOAT_EQ(record.control().voltage(),average.control().voltage());
}

TEST(AveragingMeasurementRecordTest, average) {
   AveragingMeasurementRecord mAveragingMeasurementRecord;

   int counts = 2 * 60 * 2;
   float sum = (counts*(counts + 1))/2;
   float expectedAverage = sum / counts;

   for (int i = 1; i <= counts; ++i) {
      float d = i;
      mAveragingMeasurementRecord.add(MeasurementRecord{{d,d},{d,d},{d,d},{d,d},{d,d},{d,d}});
   }

   MeasurementRecord average = mAveragingMeasurementRecord.averageAndClear();

   EXPECT_FLOAT_EQ(expectedAverage,average.panelA().current());

}

TEST(AveragingMeasurementRecordTest, clear) {
   AveragingMeasurementRecord mAveragingMeasurementRecord;

   int counts = 2 * 60 * 2;

   for (int i = 1; i <= counts; ++i) {
      float d = i;
      mAveragingMeasurementRecord.add(MeasurementRecord{{d,d},{d,d},{d,d},{d,d},{d,d},{d,d}});
   }

   mAveragingMeasurementRecord.averageAndClear();
   MeasurementRecord average = mAveragingMeasurementRecord.averageAndClear();

   EXPECT_FLOAT_EQ(0,average.panelA().current());
}


} // namespace SolarMonitor
} // namespace Bt
