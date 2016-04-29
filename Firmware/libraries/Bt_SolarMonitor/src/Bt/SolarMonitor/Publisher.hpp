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
      void loop();

      virtual void publish(const MeasurementRecord& pMeasurementRecord);

   private:
   	  // Constructor to prohibit copy construction.
      Publisher(const Publisher&);

      // Operator= to prohibit copy assignment
      Publisher& operator=(const Publisher&);

      I_MqttClient* mMqttClient;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Publisher__hpp
