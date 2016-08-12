//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Publisher
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Publisher__hpp
#define INC__Bt_SolarMonitor_Publisher__hpp

#include "Bt/SolarMonitor/I_Publisher.hpp"

#include "Bt/SolarMonitor/I_MqttClient.hpp"

namespace Bt {
namespace SolarMonitor {

class Publisher : public I_Publisher
{
   public:


      Publisher(I_MqttClient& pMqttClient);
      ~Publisher();

      void begin();

      virtual bool publish(const MeasurementRecord& pMeasurementRecord, uint32_t pReconnectCounter);

   private:

   	// Constructor to prohibit copy construction.
      Publisher(const Publisher&);

      // Operator= to prohibit copy assignment
      Publisher& operator=(const Publisher&);

      I_MqttClient* mMqttClient;
      uint32_t mPublishCounter = 0;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Publisher__hpp
