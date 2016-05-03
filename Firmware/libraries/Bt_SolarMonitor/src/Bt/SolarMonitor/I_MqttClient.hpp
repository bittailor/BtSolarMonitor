//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::SolarMonitor::I_MqttClient
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_I_MqttClient__hpp
#define INC__Bt_SolarMonitor_I_MqttClient__hpp


#include <stddef.h>

namespace Bt {
namespace SolarMonitor {

class I_MqttClient {
   public:
      enum QoS { QOS0, QOS1, QOS2 };

      virtual ~I_MqttClient() {}
      virtual bool publish(const char* pTopicName, void* pPayload, size_t pPayloadlen, QoS pQos = QOS0, bool pRetained = false) = 0;
      virtual bool publish(const char* pTopicName, const char* pMessage, QoS pQos = QOS0, bool pRetained = false) = 0;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_I_MqttClient__hpp
