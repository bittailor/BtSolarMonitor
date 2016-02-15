#include <Wire.h>

#define CMD_READ_REQUEST_COUNTER  0x01 
#define CMD_DATA_LOOP_BACK  0x02 

#define LOOP_LED 6
#define ERROR_LED 5


void setup() {
     pinMode(ERROR_LED, OUTPUT);
     pinMode(LOOP_LED, OUTPUT);
     
     digitalWrite(ERROR_LED, HIGH);
     digitalWrite(LOOP_LED, HIGH);
     
     delay(500);
     digitalWrite(ERROR_LED, LOW);
     
     Wire.begin(0x8); 
     Wire.onReceive(receiveEvent);
     Wire.onRequest(requestEvent);
}

void loop() {
  static bool loopLed = true;
  digitalWrite(LOOP_LED, loopLed);
  delay(250);
  loopLed = !loopLed; 
}

const size_t sBufferLength = BUFFER_LENGTH;

volatile uint8_t sCommandBuffer = 0;
volatile uint32_t sRequestCounter = 0;

volatile uint8_t sBuffer[sBufferLength] = {}; 
volatile int sCurrentBufferSize = -1;

void receiveEvent(int numBytes) {
    if(numBytes < 1 || (numBytes+1) > sBufferLength) {
        digitalWrite(ERROR_LED, HIGH);
        return;
    }
    
    uint8_t sCommandBuffer = Wire.read();
    
    switch(sCommandBuffer) {
        case CMD_READ_REQUEST_COUNTER : {
            if(numBytes != 1) {
                digitalWrite(ERROR_LED, HIGH);    
            }
            return;    
        }break;
        case CMD_DATA_LOOP_BACK : {
            int readSize = numBytes-1;
            for (size_t i = 0; i < readSize; i++) {
                sBuffer[i] = Wire.read();   
            }
            sCurrentBufferSize = readSize;    
        }break;
        default : {
            digitalWrite(ERROR_LED, HIGH);
            return;    
        }    
    }  
}


void requestEvent() {
    uint8_t command = sCommandBuffer;
    sCommandBuffer = 0x0;
    switch(sCommandBuffer) {    
        case CMD_READ_REQUEST_COUNTER : {
            Wire.write((const uint8_t *)&sRequestCounter, sizeof(sRequestCounter));    
        }break;
        case CMD_DATA_LOOP_BACK : {
            if (sCurrentBufferSize < 0)
            {
                digitalWrite(ERROR_LED, HIGH);
                return;       
            }           
            Wire.write((const uint8_t *)sBuffer, sCurrentBufferSize);
            sCurrentBufferSize = -1;
              
        }break;
        default : {
            digitalWrite(ERROR_LED, HIGH);
            return;   
        }    
    }
    sRequestCounter++;
}
