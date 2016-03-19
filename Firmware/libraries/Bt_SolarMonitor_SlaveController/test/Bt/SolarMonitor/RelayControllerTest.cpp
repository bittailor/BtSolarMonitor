//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/SolarMonitor/RelayController.hpp"
#include "Bt/SolarMonitor/RelayControllerActionPortGMock.hpp"
#include "Bt/SolarMonitor/RelayControllerQueryPortGMock.hpp"

namespace Bt {
namespace SolarMonitor {

class RelayControllerTest : public ::testing::Test {

   protected:

      RelayControllerTest()
      :mRelayController(mActionPortMock, mQueryPortMock) {

      }


      virtual void SetUp() {
      }

      virtual void TearDown() {

      }

      RelayControllerActionPortGMock mActionPortMock;
      RelayControllerQueryPortGMock mQueryPortMock;

      RelayController mRelayController;

};


TEST_F(RelayControllerTest, testcase) {
   EXPECT_EQ(1,1);
}


} // namespace SolarMonitor
} // namespace Bt
