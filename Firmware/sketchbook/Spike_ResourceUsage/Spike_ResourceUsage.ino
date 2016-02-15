#include <ArduinoJson.h>
#include <MQTTClient.h>
#include <Adafruit_FONA.h>
#include <Bt_PahoWithFona.h>

#define FONA_RST 5
#define FONA_APN       "gprs.swisscom.ch"

Adafruit_FONA sFona(FONA_RST);
Bt_PahoNetworkFona sNetwork(sFona);
MQTT::Client<Bt_PahoNetworkFona, Bt_PahoTimer, 500, 1> sMqttClient(sNetwork);

MQTT::Message sMessage;

void setup() {
    
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    Bt_PahoTimer t(5000);
    while (!Serial && !t.expired()) {}
    Serial.begin(115200); 
    Serial.println(F("** ResourceCheck **"));
    digitalWrite(13, LOW);            
    
    Serial1.begin(9600);
    
    connect();                  
}

void loop() {
    static unsigned long sLast = 0;
    static unsigned long sInterval = 10 * 60 * 1000;
    
    Serial.println("loop 1");
    
    if (!sMqttClient.isConnected()) {
        if (!connect()) {
            return;
        }   
    }
    
    Serial.println("loop 2");
    
    /*
    unsigned long delay = millis() - sLast;  
    if (delay < sInterval)
    {
        return;
    }
    */
    
    Serial.println("loop 3");

    for(int i = 0 ; i < 100 ; i++) {
        sendData();
        Serial.print("i ");Serial.println(i);
        sMqttClient.yield(100);    
    }
    
    char buf[20];
    strcpy(buf, "end");
    sMessage.qos = MQTT::QOS1;
    sMessage.retained = true;
    sMessage.dup = false;
    sMessage.payload = (void*)buf;
    sMessage.payloadlen = strlen(buf)+1;
    sMqttClient.publish("bt/resource/status", sMessage);
    
    sMqttClient.disconnect();
    
    sNetwork.disconnect();
    
    Serial.println("end"); 
    
    while(true) {
           
    }
}


bool connect() {
    Serial.println(F("connect ..."));
    if(!connectGPRS()) {
        return false;
    }
    if(!connectMQTT()){
        return false;
    }
  
    char buf[20];
    strcpy(buf, "online");
    sMessage.qos = MQTT::QOS1;
    sMessage.retained = true;
    sMessage.dup = false;
    sMessage.payload = (void*)buf;
    sMessage.payloadlen = strlen(buf)+1;
    sMqttClient.publish("bt/resource/status", sMessage);
}

bool connectGPRS() {
    if (! sFona.begin(Serial1)) {
        Serial.println(F("Couldn't find FONA"));
        return false;
    }
    
    Serial1.println("AT+CMEE=2");
    Serial.println("FONA is OK");
    sFona.unlockSIM("1210");
    Serial.println("Checking for network...");
    
    while (sFona.getNetworkStatus() != 1) {
        delay(100);
    }
     
    sFona.setGPRSNetworkSettings(F(FONA_APN));
    
    Serial.println("Enabling GPRS ...");
    if (!sFona.enableGPRS(true)) {
        Serial.println(F("Failed to turn GPRS on"));  
        return false;
    }

    Serial.println(F("Checking for GPRS..."));
    while (sFona.GPRSstate() != 1) {
        delay(100);
    }
 
    return true;  
}

bool connectMQTT() {
    char hostname[] = "iot.eclipse.org";
    int port = 1883;

    Serial.print("Connecting to ");
    Serial.print(hostname);
    Serial.print(":");
    Serial.println(port);
    
    int rc = sNetwork.connect(hostname, port);
    if (rc != 0)
    {
        Serial.print("rc from TCP connect is ");
        Serial.println(rc);
    }
    
    Serial.println("MQTT connecting");
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
    data.MQTTVersion = 3;
    data.clientID.cstring = (char*)"bt-resource-check";
    data.willFlag = true;
    data.will.topicName.cstring = (char*)"bt/resource/status";
    data.will.message.cstring = (char*)"offline";
    data.will.retained = true;
    data.will.qos = MQTT::QOS1;
   
    
    rc = sMqttClient.connect(data);
    if (rc != 0)
    {
        Serial.print("rc from MQTT connect is ");
        Serial.println(rc);
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
    
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    
    JsonObject& panelA = root.createNestedObject("Pa");
    panelA["I"] = 10234;
    panelA["U"] = 12100;
    
    JsonObject& batteryA = root.createNestedObject("Ba");
    batteryA["I"] = 12345;
    batteryA["U"] = 13100;
    
    JsonObject& panelB = root.createNestedObject("Pb");
    panelB["I"] = 22876;
    panelB["U"] = 11987;
    
    JsonObject& batteryB = root.createNestedObject("Ba");
    batteryB["I"] = 12345;
    batteryB["U"] = 13100;
    
    JsonObject& load = root.createNestedObject("L");
    load["I"] = 80000;
    load["U"] = 13100;
    
    char timeBuffer[101];
    sFona.getTime(timeBuffer,100);
    root["time"] = timeBuffer;

    char buffer[500];
    root.printTo(buffer, sizeof(buffer));
    
    sMessage.qos = MQTT::QOS0;
    sMessage.retained = false;
    sMessage.dup = false;
    sMessage.payload = (void*)buffer;
    sMessage.payloadlen = strlen(buffer)+1;
    Serial.print(F("publish "));Serial.println(buffer);
    int rc = sMqttClient.publish("bt/resource", sMessage);   
    if (rc != 0)
    {
        Serial.print("rc from MQTT publish is ");
        Serial.println(rc);
    }    
}

