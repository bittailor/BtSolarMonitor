//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include "Bt/SolarMonitor/RelayController.hpp"

#include <gtest/gtest.h>
#include <Bt/Core/TimeGMock.hpp>
#include "Bt/SolarMonitor/LatchingRelayGMock.hpp"
#include "Bt/SolarMonitor/RelayControllerQueryPortGMock.hpp"
#include "Bt/SolarMonitor/PowerStateGMock.hpp"

namespace Bt {
namespace SolarMonitor {

using ::testing::Return;

class RelayControllerTest : public ::testing::Test {

   protected:

      RelayControllerTest()
      :mRelayController(mTimeGMock, mQueryPortMock, mRelayA, mRelayB, mRelayLoad, mPowerState) {
      }


      virtual void SetUp() {
      }

      virtual void TearDown() {

      }

      ::testing::NiceMock<Core::TimeGMock> mTimeGMock;
      ::testing::NiceMock<RelayControllerQueryPortGMock> mQueryPortMock;

      ::testing::StrictMock<LatchingRelayGMock> mRelayA;
      ::testing::StrictMock<LatchingRelayGMock> mRelayB;
      ::testing::StrictMock<LatchingRelayGMock> mRelayLoad;
      ::testing::StrictMock<PowerStateGMock> mPowerState;

      RelayController mRelayController;

};


TEST_F(RelayControllerTest, beginOff) {
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(false));

   EXPECT_CALL(mPowerState, state(I_PowerState::Off)).Times(1);
   mRelayController.begin();
}

TEST_F(RelayControllerTest, beginOff_ToggleOn_WithLoadOnAndBatteryABetter) {
   ON_CALL(mQueryPortMock, isBatteryABetter()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mPowerState,  state(I_PowerState::Off)).Times(1);
   mRelayController.begin();
   mRelayController.workcycle();

   EXPECT_CALL(mRelayA,  onCoil(true)).Times(1);
   mRelayController.toggleOnOff();

   EXPECT_CALL(mRelayA,  onCoil(false)).Times(1);
   EXPECT_CALL(mPowerState,  state(I_PowerState::OnA)).Times(1);
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(500));
   mRelayController.workcycle();
}

TEST_F(RelayControllerTest, beginOff_ToggleOn_WithLoadOnAndBatteryANotBetter) {
   ON_CALL(mQueryPortMock, isBatteryABetter()).WillByDefault(Return(false));
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mPowerState,  state(I_PowerState::Off)).Times(1);
   mRelayController.begin();
   mRelayController.workcycle();

   EXPECT_CALL(mRelayB,  onCoil(true)).Times(1);
   mRelayController.toggleOnOff();

   EXPECT_CALL(mRelayB,  onCoil(false)).Times(1);
   EXPECT_CALL(mPowerState,  state(I_PowerState::OnB)).Times(1);
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(500));
   mRelayController.workcycle();
}

TEST_F(RelayControllerTest, beginOff_ToggleOn_WithLoadOffAndBatteryABetter) {
   ON_CALL(mQueryPortMock, isBatteryABetter()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(false));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mPowerState,  state(I_PowerState::Off)).Times(1);
   mRelayController.begin();
   mRelayController.workcycle();

   EXPECT_CALL(mRelayA,  onCoil(true)).Times(1);
   mRelayController.toggleOnOff();

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(200));
   EXPECT_CALL(mRelayA,  onCoil(false)).Times(1);
   EXPECT_CALL(mRelayLoad,  onCoil(true)).Times(1);
   mRelayController.workcycle();

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(300));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(false));
   EXPECT_CALL(mRelayLoad,  onCoil(false)).Times(1);
   EXPECT_CALL(mPowerState,  state(I_PowerState::OnA)).Times(1);
   mRelayController.workcycle();

}

TEST_F(RelayControllerTest, beginOnA) {
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(false));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));

   EXPECT_CALL(mPowerState,  state(I_PowerState::OnA)).Times(1);
   mRelayController.begin();
}

TEST_F(RelayControllerTest, beginOnA_ButLoadIsOff) {
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(false));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(false));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mRelayLoad,  onCoil(true)).Times(1);
   mRelayController.begin();

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(200));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   EXPECT_CALL(mRelayLoad,  onCoil(false)).Times(1);
   EXPECT_CALL(mPowerState,  state(I_PowerState::OnA)).Times(1);
   mRelayController.workcycle();
}

TEST_F(RelayControllerTest, beginOnA_ToggleOff) {
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(false));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mPowerState,  state(I_PowerState::OnA)).Times(1);
   mRelayController.begin();

   EXPECT_CALL(mRelayA,  offCoil(true)).Times(1);
   mRelayController.toggleOnOff();

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(200));
   EXPECT_CALL(mRelayA,  offCoil(false)).Times(1);
   EXPECT_CALL(mPowerState,  state(I_PowerState::Off)).Times(1);
   mRelayController.workcycle();
}

TEST_F(RelayControllerTest, beginOnB_ToggleOff) {
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(false));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mPowerState,  state(I_PowerState::OnB)).Times(1);
   mRelayController.begin();

   EXPECT_CALL(mRelayB,  offCoil(true)).Times(1);
   mRelayController.toggleOnOff();

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(200));
   EXPECT_CALL(mRelayB,  offCoil(false)).Times(1);
   EXPECT_CALL(mPowerState,  state(I_PowerState::Off)).Times(1);
   mRelayController.workcycle();
}

TEST_F(RelayControllerTest, beginOnA_SwitchToB) {
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(false));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mPowerState,  state(I_PowerState::OnA)).Times(1);
   mRelayController.begin();

   EXPECT_CALL(mRelayA,  offCoil(true)).Times(1);
   mRelayController.toggleAB();

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(200));
   EXPECT_CALL(mRelayA,  offCoil(false)).Times(1);
   EXPECT_CALL(mRelayB,  onCoil(true)).Times(1);
   mRelayController.workcycle();

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(300));
   EXPECT_CALL(mRelayB,  onCoil(false)).Times(1);
   EXPECT_CALL(mPowerState,  state(I_PowerState::OnB)).Times(1);
   mRelayController.workcycle();

}

TEST_F(RelayControllerTest, beginOnB_SwitchToA) {
   ON_CALL(mQueryPortMock, loadASense()).WillByDefault(Return(true));
   ON_CALL(mQueryPortMock, loadBSense()).WillByDefault(Return(false));
   ON_CALL(mQueryPortMock, loadOut()).WillByDefault(Return(true));
   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(100));

   EXPECT_CALL(mPowerState,  state(I_PowerState::OnB)).Times(1);
   mRelayController.begin();

   EXPECT_CALL(mRelayB,  offCoil(true)).Times(1);
   mRelayController.toggleAB();

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(200));
   EXPECT_CALL(mRelayB,  offCoil(false)).Times(1);
   EXPECT_CALL(mRelayA,  onCoil(true)).Times(1);
   mRelayController.workcycle();

   ON_CALL(mTimeGMock, milliseconds()).WillByDefault(Return(300));
   EXPECT_CALL(mRelayA,  onCoil(false)).Times(1);
   EXPECT_CALL(mPowerState,  state(I_PowerState::OnA)).Times(1);
   mRelayController.workcycle();

}





} // namespace SolarMonitor
} // namespace Bt
