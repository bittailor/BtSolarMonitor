//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Bt/Core/Function.hpp"

using ::testing::Return;
using ::testing::Ref;

namespace Bt {
namespace Core {

namespace {
   class NoCopyArgument {
      public:
         NoCopyArgument(){}
      private:
         NoCopyArgument(const NoCopyArgument&);
         NoCopyArgument& operator=(const NoCopyArgument&);
   };

   class SimpleMock {
      public:
         MOCK_METHOD1(methodVoidInt, void(int));
         MOCK_METHOD1(anotherVoidInt, void(int));
         MOCK_METHOD1(methodIntInt, int(int));
         MOCK_METHOD1(methodVoidNoCopyArgument, void(NoCopyArgument&));
   };
}

TEST(FunctionTest, Empty) {
   Function<void(int)> callback;
   if(callback) {
      FAIL();
   }
}

TEST(FunctionTest, Equal) {
   testing::StrictMock<SimpleMock> simpleMock;

   Function<void(int)> callbackOne = Function<void(int)>::build<SimpleMock,&SimpleMock::methodVoidInt>(simpleMock);
   Function<void(int)> callbackTwo = Function<void(int)>::build<SimpleMock,&SimpleMock::methodVoidInt>(simpleMock);

   ASSERT_TRUE(callbackOne == callbackTwo);
}

TEST(FunctionTest, NotEqual_WhenNotSameObject) {
   testing::StrictMock<SimpleMock> simpleMockOne;
   testing::StrictMock<SimpleMock> simpleMockTwo;

   Function<void(int)> callbackOne = Function<void(int)>::build<SimpleMock,&SimpleMock::methodVoidInt>(simpleMockOne);
   Function<void(int)> callbackTwo = Function<void(int)>::build<SimpleMock,&SimpleMock::methodVoidInt>(simpleMockTwo);

   ASSERT_FALSE(callbackOne == callbackTwo);
}

TEST(FunctionTest, NotEqual_WhenNotSameMember) {
   testing::StrictMock<SimpleMock> simpleMock;

   Function<void(int)> callbackOne = Function<void(int)>::build<SimpleMock,&SimpleMock::methodVoidInt>(simpleMock);
   Function<void(int)> callbackTwo = Function<void(int)>::build<SimpleMock,&SimpleMock::anotherVoidInt>(simpleMock);

   ASSERT_FALSE(callbackOne == callbackTwo);
}

TEST(FunctionTest, VoidReturnOneArgument_CheckCall) {
   testing::StrictMock<SimpleMock> simpleMock;
   int argument = 9876;

   EXPECT_CALL(simpleMock, methodVoidInt(argument));

   Function<void(int)> callback = Function<void(int)>::build<SimpleMock,&SimpleMock::methodVoidInt>(simpleMock);
   callback(argument);
}

TEST(FunctionTest, VoidReturnOneNoCopyArgument_CheckCall) {
   testing::StrictMock<SimpleMock> simpleMock;
   NoCopyArgument argument;

   EXPECT_CALL(simpleMock, methodVoidNoCopyArgument(Ref(argument)));

   Function<void(NoCopyArgument&)> callback = Function<void(NoCopyArgument&)>::build<SimpleMock,&SimpleMock::methodVoidNoCopyArgument>(simpleMock);
   callback(argument);
}

TEST(FunctionTest, IntReturnOneArgument_CheckCall) {
   testing::StrictMock<SimpleMock> simpleMock;
   int returnValue = 1234;
   int argument = 9876;

   EXPECT_CALL(simpleMock, methodIntInt(argument))
      .WillOnce(Return(returnValue));;

   Function<int(int)> callback = Function<int(int)>::build<SimpleMock,&SimpleMock::methodIntInt>(simpleMock);
   int value = callback(argument);
   ASSERT_EQ(returnValue,value);
}

} // namespace Core
} // namespace Bt
