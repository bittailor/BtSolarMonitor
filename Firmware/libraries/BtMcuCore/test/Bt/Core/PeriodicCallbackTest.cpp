//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Core/PeriodicCallback.hpp"
#include "Bt/Core/TimeGMock.hpp"
#include "Bt/Core/FunctionGMock.hpp"

using namespace testing;

namespace Bt {
namespace Core {

class PeriodicCallbackTest : public ::testing::Test {

   protected:
      PeriodicCallbackTest(): mCallback(Function<void()>::build<FunctionGMock<void>,&FunctionGMock<void>::call>(mCallbackMock)) {
      }

      virtual void SetUp() {
      }

      virtual void TearDown() {
         delete mPeriodicCallback;
      }

      StrictMock<FunctionGMock<void> > mCallbackMock;
      Function<void()> mCallback;
      TimeGMock mTime;
      PeriodicCallback* mPeriodicCallback;

};


// .WillOnce(Return(std::numeric_limits<uint32_t>::max() - 200 ))

TEST_F(PeriodicCallbackTest, SimplePeriodExact) {
   EXPECT_CALL(mTime,milliseconds())
      .WillOnce(Return(200))
      .WillOnce(Return(300));

   EXPECT_CALL(mCallbackMock,call()).Times(1);

   mPeriodicCallback = new PeriodicCallback(mTime,100,mCallback);
   uint32_t next = mPeriodicCallback->workcycle();
   EXPECT_EQ(100,next);
}

TEST_F(PeriodicCallbackTest, SimplePeriodNotYet) {
   EXPECT_CALL(mTime,milliseconds())
      .WillOnce(Return(200))
      .WillOnce(Return(299));

   EXPECT_CALL(mCallbackMock,call()).Times(0);

   mPeriodicCallback = new PeriodicCallback(mTime,100,mCallback);
   uint32_t next = mPeriodicCallback->workcycle();
   EXPECT_EQ(1,next);
}

TEST_F(PeriodicCallbackTest, SimplePeriodOverflowExact) {
   EXPECT_CALL(mTime,milliseconds())
      .WillOnce(Return(std::numeric_limits<uint32_t>::max() - 50))
      .WillOnce(Return(49));

   EXPECT_CALL(mCallbackMock,call()).Times(1);

   mPeriodicCallback = new PeriodicCallback(mTime,100,mCallback);
   uint32_t next = mPeriodicCallback->workcycle();
   EXPECT_EQ(100,next);
}

TEST_F(PeriodicCallbackTest, SimplePeriodOverflowNotYet) {
   EXPECT_CALL(mTime,milliseconds())
      .WillOnce(Return(std::numeric_limits<uint32_t>::max() - 50))
      .WillOnce(Return(40));

   EXPECT_CALL(mCallbackMock,call()).Times(0);

   mPeriodicCallback = new PeriodicCallback(mTime,100,mCallback);
   uint32_t next = mPeriodicCallback->workcycle();
   EXPECT_EQ(9,next);
}


} // namespace Core
} // namespace Bt
