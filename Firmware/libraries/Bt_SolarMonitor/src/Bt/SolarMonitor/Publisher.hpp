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
      typedef const char* Topic;

      Publisher(I_MqttClient& pMqttClient);
      ~Publisher();

      void begin();

      virtual void publish(const MeasurementRecord& pMeasurementRecord);

   private:

   	// Constructor to prohibit copy construction.
      Publisher(const Publisher&);

      // Operator= to prohibit copy assignment
      Publisher& operator=(const Publisher&);

      void publish(Topic pTopics[], const Measurement& pMeasurement);
      void publish(Topic pTopic, float pValue);

      I_MqttClient* mMqttClient;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Publisher__hpp
