#ifndef INC_MqttPublisher_H
#define INC_MqttPublisher_H

#include <Arduino.h>
#include <stddef.h>
#include <MQTTClient.h>
#include <Bt_PahoWithFona.h>
#include "I_Publisher.h"


typedef MQTT::Client<Bt_PahoNetworkFona, Bt_PahoTimer, 500, 1> MqttClient;

class MqttPublisher : I_Publisher {
public:
    MqttPublisher(MqttClient& pMqttClient);
       
    virtual void publish(MeasurementRecord pRecords[], size_t pLength);
       
private:
    MqttClient& mMqttClient;  
};


#endif 
