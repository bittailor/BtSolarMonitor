//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/SolarMonitor/Publisher.hpp"
#include "Bt/SolarMonitor/MqttClientGMock.hpp"


using namespace testing;

namespace Bt {
namespace SolarMonitor {

class PublisherTest : public ::testing::Test {

   protected:

      PublisherTest():mPublisher(mMqttClient) {
      }

      virtual void SetUp() {
      }

      virtual void TearDown() {

      }

      StrictMock<MqttClientGMock> mMqttClient;
      Publisher mPublisher;

};


TEST_F(PublisherTest, publishAMeasurementRecord) {

   MeasurementRecord measurementRecord(
            Measurement(1.1,11.11),
            Measurement(2.2,22.22),
            Measurement(3.3,33.33),
            Measurement(4.4,44.44),
            Measurement(5.5,55.55),
            Measurement(6.6,66.66)
   );

   EXPECT_CALL(mMqttClient,  publish(StrEq("BSM/S/PA/I"),StrEq("1.1000"),I_MqttClient::QOS0,false)).Times(1);
   EXPECT_CALL(mMqttClient,  publish(StrEq("BSM/S/PB/I"),StrEq("2.2000"),I_MqttClient::QOS0,false)).Times(1);
   EXPECT_CALL(mMqttClient,  publish(StrEq("BSM/S/BA/I"),StrEq("3.3000"),I_MqttClient::QOS0,false)).Times(1);
   EXPECT_CALL(mMqttClient,  publish(StrEq("BSM/S/BB/I"),StrEq("4.4000"),I_MqttClient::QOS0,false)).Times(1);
   EXPECT_CALL(mMqttClient,  publish(StrEq("BSM/S/L/I"),StrEq("5.5000"),I_MqttClient::QOS0,false)).Times(1);
   EXPECT_CALL(mMqttClient,  publish(StrEq("BSM/S/C/I"),StrEq("6.6000"),I_MqttClient::QOS0,false)).Times(1);

   EXPECT_CALL(mMqttClient,  publish(StrEq("BSM/S/PA/V"),StrEq("11.1100"),I_MqttClient::QOS0,false)).Times(1);
   EXPECT_CALL(mMqttClient,  publish(StrEq("BSM/S/PB/V"),StrEq("22.2200"),I_MqttClient::QOS0,false)).Times(1);
   EXPECT_CALL(mMqttClient,  publish(StrEq("BSM/S/BA/V"),StrEq("33.3300"),I_MqttClient::QOS0,false)).Times(1);
   EXPECT_CALL(mMqttClient,  publish(StrEq("BSM/S/BB/V"),StrEq("44.4400"),I_MqttClient::QOS0,false)).Times(1);
   EXPECT_CALL(mMqttClient,  publish(StrEq("BSM/S/L/V"),StrEq("55.5500"),I_MqttClient::QOS0,false)).Times(1);
   EXPECT_CALL(mMqttClient,  publish(StrEq("BSM/S/C/V"),StrEq("66.6600"),I_MqttClient::QOS0,false)).Times(1);


   mPublisher.publish(measurementRecord);

}


} // namespace SolarMonitor
} // namespace Bt
