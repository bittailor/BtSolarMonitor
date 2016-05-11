//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MqttClientGMock__hpp
#define INC__Bt_SolarMonitor_MqttClientGMock__hpp

#include <gmock/gmock.h>

#include "Bt/SolarMonitor/I_MqttClient.hpp"

namespace Bt {
namespace SolarMonitor {

class MqttClientGMock : public I_MqttClient {
   public:
      MOCK_METHOD0(connect, bool());
      MOCK_METHOD0(isConnected, bool());
      MOCK_METHOD1(yield, bool(uint32_t));
      MOCK_METHOD5(publish, bool(const char*,void*,size_t,QoS,bool));
      MOCK_METHOD4(publish, bool(const char*,const char*,QoS,bool));
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MqttClientGMock__hpp
