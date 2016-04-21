//#include "MqttPublisher.h"
//
//#ifdef ARDUINO_SAMD_ZERO
//static char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
//  char fmt[20];
//  sprintf(fmt, "%%%d.%df", width, prec);
//  sprintf(sout, fmt, val);
//  return sout;
//}
//#endif
//
//MqttPublisher::MqttPublisher(MqttClient& pMqttClient):mMqttClient(pMqttClient) {
//
//}
//
//#define TOPIC_BASE "bittailor" "/feeds/"
//
//void MqttPublisher::publish(MeasurementRecord pRecords[], size_t pLength) {
//    char topic[MAX_TOPIC_LENGTH] = {0};
//    char payload[MAX_TOPIC_LENGTH] = {0};
//    int start = strlen(topic);
//
//    for(size_t i = 0 ; i < pLength; i++) {
//        strncpy(topic, TOPIC_BASE, MAX_TOPIC_LENGTH);
//        strncat(topic, pRecords[i].mTopic, MAX_TOPIC_LENGTH);
//        strncat(topic, "I", MAX_TOPIC_LENGTH);
//        dtostrf(pRecords[i].mCurrent, 0, 2, payload);
//        mMqttClient.publish(topic, payload, MQTT::QOS1);
//
//        strncpy(topic, TOPIC_BASE, MAX_TOPIC_LENGTH);
//        strncat(topic, pRecords[i].mTopic, MAX_TOPIC_LENGTH);
//        strncat(topic, "V", MAX_TOPIC_LENGTH);
//        dtostrf(pRecords[i].mVoltage, 0, 2, payload);
//        mMqttClient.publish(topic, payload, MQTT::QOS1);
//    }
//}
