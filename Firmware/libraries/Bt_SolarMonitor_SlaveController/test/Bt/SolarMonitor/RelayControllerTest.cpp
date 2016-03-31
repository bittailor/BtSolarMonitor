//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Core/TimeGMock.hpp"


#include "Bt/SolarMonitor/RelayController.hpp"
#include "Bt/SolarMonitor/RelayControllerActionPortGMock.hpp"
#include "Bt/SolarMonitor/RelayControllerQueryPortGMock.hpp"

namespace Bt {
namespace SolarMonitor {

using ::testing::Return;

class RelayControllerTest : public ::testing::Test {

   protected:

      RelayControllerTest()
      :mRelayController(mTimeGMock, mActionPortMock, mQueryPortMock) {
      }


      virtual void SetUp() {
      }

      virtual void TearDown() {

      }

      ::testing::NiceMock<Core::TimeGMock> mTimeGMock;
      ::testing::StrictMock<RelayControllerActionPortGMock> mActionPortMock;
      ::testing::NiceMock<RelayControllerQueryPortGMock> mQueryPortMock;

      RelayController mRelayController;

};


TEST_F(RelayControllerTest, beginOff) {
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(false));

   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::Off)).Times(1);
   mRelayController.begin();
}

TEST_F(RelayControllerTest, beginOff_ToggleOn_WithLoadOnAndBatteryABetter) {
   ON_CALL(mQueryPortMock, isBatteryABetter()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::Off)).Times(1);
   mRelayController.begin();
   mRelayController.loop();

   EXPECT_CALL(mActionPortMock,  relayAToOn(true)).Times(1);
   mRelayController.handle(&RelayController::State::toggleOnOff);

   EXPECT_CALL(mActionPortMock,  relayAToOn(false)).Times(1);
   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::OnA)).Times(1);
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(500));
   mRelayController.loop();
}

TEST_F(RelayControllerTest, beginOff_ToggleOn_WithLoadOnAndBatteryANotBetter) {
   ON_CALL(mQueryPortMock, isBatteryABetter()).WillByDefault(Return(false));
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::Off)).Times(1);
   mRelayController.begin();
   mRelayController.loop();

   EXPECT_CALL(mActionPortMock,  relayBToOn(true)).Times(1);
   mRelayController.handle(&RelayController::State::toggleOnOff);

   EXPECT_CALL(mActionPortMock,  relayBToOn(false)).Times(1);
   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::OnB)).Times(1);
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(500));
   mRelayController.loop();
}

TEST_F(RelayControllerTest, beginOff_ToggleOn_WithLoadOffAndBatteryABetter) {
   ON_CALL(mQueryPortMock, isBatteryABetter()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(false));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::Off)).Times(1);
   mRelayController.begin();
   mRelayController.loop();

   EXPECT_CALL(mActionPortMock,  relayAToOn(true)).Times(1);
   mRelayController.handle(&RelayController::State::toggleOnOff);

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(200));
   EXPECT_CALL(mActionPortMock,  relayAToOn(false)).Times(1);
   EXPECT_CALL(mActionPortMock,  relayLoadToOn(true)).Times(1);
   mRelayController.loop();

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(300));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(false));
   EXPECT_CALL(mActionPortMock,  relayLoadToOn(false)).Times(1);
   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::OnA)).Times(1);
   mRelayController.loop();

}

TEST_F(RelayControllerTest, beginOnA) {
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(false));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));

   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::OnA)).Times(1);
   mRelayController.begin();
}

TEST_F(RelayControllerTest, beginOnA_ButLoadIsOff) {
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(false));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(false));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mActionPortMock,  relayLoadToOn(true)).Times(1);
   mRelayController.begin();

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(200));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   EXPECT_CALL(mActionPortMock,  relayLoadToOn(false)).Times(1);
   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::OnA)).Times(1);
   mRelayController.loop();
}

TEST_F(RelayControllerTest, beginOnA_ToggleOff) {
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(false));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::OnA)).Times(1);
   mRelayController.begin();

   EXPECT_CALL(mActionPortMock,  relayAToOff(true)).Times(1);
   mRelayController.handle(&RelayController::State::toggleOnOff);

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(200));
   EXPECT_CALL(mActionPortMock,  relayAToOff(false)).Times(1);
   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::Off)).Times(1);
   mRelayController.loop();
}

TEST_F(RelayControllerTest, beginOnB_ToggleOff) {
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(false));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::OnB)).Times(1);
   mRelayController.begin();

   EXPECT_CALL(mActionPortMock,  relayBToOff(true)).Times(1);
   mRelayController.handle(&RelayController::State::toggleOnOff);

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(200));
   EXPECT_CALL(mActionPortMock,  relayBToOff(false)).Times(1);
   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::Off)).Times(1);
   mRelayController.loop();
}

TEST_F(RelayControllerTest, beginOnA_SwitchToB) {
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(false));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::OnA)).Times(1);
   mRelayController.begin();

   EXPECT_CALL(mActionPortMock,  relayAToOff(true)).Times(1);
   mRelayController.handle(&RelayController::State::toggleAB);

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(200));
   EXPECT_CALL(mActionPortMock,  relayAToOff(false)).Times(1);
   EXPECT_CALL(mActionPortMock,  relayBToOn(true)).Times(1);
   mRelayController.loop();

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(300));
   EXPECT_CALL(mActionPortMock,  relayBToOn(false)).Times(1);
   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::OnB)).Times(1);
   mRelayController.loop();

}

TEST_F(RelayControllerTest, beginOnB_SwitchToA) {
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(false));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::OnB)).Times(1);
   mRelayController.begin();

   EXPECT_CALL(mActionPortMock,  relayBToOff(true)).Times(1);
   mRelayController.handle(&RelayController::State::toggleAB);

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(200));
   EXPECT_CALL(mActionPortMock,  relayBToOff(false)).Times(1);
   EXPECT_CALL(mActionPortMock,  relayAToOn(true)).Times(1);
   mRelayController.loop();

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(300));
   EXPECT_CALL(mActionPortMock,  relayAToOn(false)).Times(1);
   EXPECT_CALL(mActionPortMock,  publicState(I_RelayController::OnA)).Times(1);
   mRelayController.loop();

}





} // namespace SolarMonitor
} // namespace Bt
