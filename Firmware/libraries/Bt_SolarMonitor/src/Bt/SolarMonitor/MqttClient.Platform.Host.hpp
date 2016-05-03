//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MqttClient
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MqttClient_Platform_Host__hpp
#define INC__Bt_SolarMonitor_MqttClient_Platform_Host__hpp


namespace Bt {
namespace SolarMonitor {

class MqttClient
{
   public:
      MqttClient();
      ~MqttClient();

   private:
   	  // Constructor to prohibit copy construction.
      MqttClient(const MqttClient&);

      // Operator= to prohibit copy assignment
      MqttClient& operator=(const MqttClient&);
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MqttClient_Platform_Host__hpp
