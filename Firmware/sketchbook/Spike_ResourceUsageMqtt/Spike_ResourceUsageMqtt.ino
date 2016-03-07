#include <ArduinoJson.h>
#include <MQTTClient.h>
#include <Adafruit_FONA.h>
#include <Bt_PahoWithFona.h>

#define FONA_RST 5
#define FONA_APN       "gprs.swisscom.ch"

#define MEASURE_TRIGGER 12
#define START_TRIGGER 11

Adafruit_FONA sFona(FONA_RST);
Bt_PahoNetworkFona sNetwork(sFona);
MQTT::Client<Bt_PahoNetworkFona, Bt_PahoTimer, 500, 1> sMqttClient(sNetwork);

MQTT::Message sMessage;

void setup() {
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    while (!Serial) {}
    Serial.begin(115200); 
    Serial.println("** Spike_ResourceUsageMqtt **");
    digitalWrite(13, LOW);  
     
    pinMode(13, OUTPUT);
    pinMode(MEASURE_TRIGGER, OUTPUT);
    pinMode(START_TRIGGER, INPUT_PULLUP);
    digitalWrite(MEASURE_TRIGGER, HIGH);
     
    digitalWrite(13, HIGH);
        
    Serial1.begin(9600);
    
    Serial.println("Connecting ...");
    connect();  
    Serial.println(" ... connected");
                    
}

void loop() {
        
    Serial.println("Start Measure");
    digitalWrite(MEASURE_TRIGGER, LOW);
    delay(2000);
     
    if(!connectMQTT()){
        
        digitalWrite(MEASURE_TRIGGER, HIGH);
        delay(2000);
        return; 
    } 
     
    for(int i = 0 ; i < 60 ; i++) {
        while(!sMqttClient.isConnected()){
            Serial.println("Ooops Reconnect");
            connectMQTT();    
        }
        sendData();
        sMqttClient.yield(60 * 1000);    
    }
 
    sMqttClient.disconnect();
    
    digitalWrite(MEASURE_TRIGGER, HIGH);
    Serial.println("End Measure");
    delay(2000);
    
    sNetwork.disconnect();
    
    
    while(true) {
           
    }
}


bool connect() {
    if(!connectGPRS()) {
        return false;
    }
}

bool connectGPRS() {
    if (! sFona.begin(Serial1)) {
        return false;
    }
    
    Serial1.println("AT+CMEE=2");
    sFona.unlockSIM("1210");
    
    while (sFona.getNetworkStatus() != 1) {
        delay(100);
    }
     
    sFona.setGPRSNetworkSettings(F(FONA_APN));
    
    if (!sFona.enableGPRS(true)) {
        return false;
    }

    while (sFona.GPRSstate() != 1) {
        delay(100);
    }
 
    return true;  
}

bool connectMQTT() {
    char hostname[] = "broker.shiftr.io";
    int port = 1883;

    
    int rc = sNetwork.connect(hostname, port);
    if (rc != 0)
    {
    }
    
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
    data.MQTTVersion = 3;
    data.clientID.cstring = (char*)"bt-resource-check";
    data.keepAliveInterval = 120;
    data.willFlag = true;
    data.will.topicName.cstring = (char*)"bt/resource/status";
    data.will.message.cstring = (char*)"offline";
    data.will.retained = true;
    data.will.qos = MQTT::QOS1;
    data.username.cstring = (char*)"f64edae4";
    data.password.cstring = (char*)"eea9554c6e05c108";
   
    
    rc = sMqttClient.connect(data);
    if (rc != 0)
    {
    }
    Serial.println("MQTT connected");
    
    rc = sMqttClient.subscribe("bt/resource/command", MQTT::QOS2, messageArrived);   
    if (rc != 0)
    {
        Serial.print("rc from MQTT subscribe is ");
        Serial.println(rc);
    }
    Serial.println("MQTT subscribed");
    return true;   
}

void messageArrived(MQTT::MessageData& md)
{
    static int sCounter = 0;    
    
    MQTT::Message& message = md.message;
    Serial.print(F("Message "));
    Serial.print(++sCounter);
    Serial.print(F(" arrived: qos "));
    Serial.print(message.qos);
    Serial.print(F(", retained "));
    Serial.print(message.retained);
    Serial.print(F(", dup "));
    Serial.print(message.dup);
    Serial.print(F(", packetid "));
    Serial.println(message.id);
    Serial.print(F("Payload "));
    Serial.println((char*)message.payload);
}

void sendData() {
    Serial.println("publish ...");
    sMqttClient.publish("bt/res/PA/I",  (void*)"123.45",6);
    sMqttClient.publish("bt/res/PA/V", (void*)"678.09",6); 
    sMqttClient.publish("bt/res/PB/I", (void*)"123.45",6); 
    sMqttClient.publish("bt/res/PB/V", (void*)"678.09",6);
    sMqttClient.publish("bt/res/BA/I", (void*)"123.45",6);
    sMqttClient.publish("bt/res/BA/V", (void*)"678.09",6); 
    sMqttClient.publish("bt/res/BB/I", (void*) "123.45",6); 
    sMqttClient.publish("bt/res/BB/V", (void*)"678.09",6);
    sMqttClient.publish("bt/res/L/I", (void*)"123.45",6); 
    sMqttClient.publish("bt/res/L/V", (void*)"678.09",6);
    Serial.println(" ... published");  
}

