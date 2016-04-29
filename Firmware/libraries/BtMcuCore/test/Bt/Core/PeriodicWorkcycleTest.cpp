//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Core/PeriodicWorkcycle.hpp"
#include "Bt/Core/TimeGMock.hpp"
#include "Bt/Core/RunnableGMock.hpp"

using namespace testing;

namespace Bt {
namespace Core {

class PeriodicWorkcycleTest : public ::testing::Test {

   protected:

      PeriodicWorkcycleTest(): mPeriodicWorkcycle(1000,mTime) {

      }

      virtual void SetUp() {
      }

      virtual void TearDown() {

      }

      TimeGMock mTime;
      PeriodicWorkcycle mPeriodicWorkcycle;

};


TEST_F(PeriodicWorkcycleTest, PeriodicWorkcycle) {
   StrictMock<RunnableGMock> runnable;

   EXPECT_CALL(runnable,workcycle()).Times(3);

   EXPECT_CALL(mTime,microseconds())
      .WillOnce(Return(77466))
      .WillOnce(Return(77999))
      .WillOnce(Return(78010))
      .WillOnce(Return(78999))
      .WillOnce(Return(79000))
      ;

   mPeriodicWorkcycle.add(runnable);
   mPeriodicWorkcycle.workcycle();
   mPeriodicWorkcycle.workcycle();
   mPeriodicWorkcycle.workcycle();
   mPeriodicWorkcycle.workcycle();
   mPeriodicWorkcycle.workcycle();

}

TEST_F(PeriodicWorkcycleTest, PeriodicWorkcycleOverflowCall) {
   StrictMock<RunnableGMock> runnable;

   EXPECT_CALL(runnable,workcycle()).Times(1);

   EXPECT_CALL(mTime,microseconds())
      .WillOnce(Return(std::numeric_limits<uint32_t>::max() - 200 ))
      .WillOnce(Return(200))
      .WillOnce(Return(900))
      ;

   mPeriodicWorkcycle.add(runnable);
   mPeriodicWorkcycle.workcycle();
   mPeriodicWorkcycle.workcycle();
   mPeriodicWorkcycle.workcycle();

}

TEST_F(PeriodicWorkcycleTest, PeriodicWorkcycleOverflowNoCall) {
   StrictMock<RunnableGMock> runnable;

   EXPECT_CALL(runnable,workcycle()).Times(1);

   EXPECT_CALL(mTime,microseconds())
      .WillOnce(Return(std::numeric_limits<uint32_t>::max() - 200 ))
      .WillOnce(Return(200))
      .WillOnce(Return(700))
      ;

   mPeriodicWorkcycle.add(runnable);
   mPeriodicWorkcycle.workcycle();
   mPeriodicWorkcycle.workcycle();
   mPeriodicWorkcycle.workcycle();
}


} // namespace Core
} // namespace Bt
