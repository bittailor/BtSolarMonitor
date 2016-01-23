#include <Countdown.h>
#include <IPStack.h>
#include <MQTTClient.h>
#include <Adafruit_FONA.h>
#include <Bt_FonaTcpClient.h>

#define FONA_RST 5

Adafruit_FONA sFona(FONA_RST);
Bt_FonaTcpClient sTcpClient(sFona);
IPStack sIPStack(sTcpClient);
MQTT::Client<IPStack, Countdown, 50, 1> sMqttClient(sIPStack);


void setup() {                
}

void loop() {
}
