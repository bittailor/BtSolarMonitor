#ifndef INC_Bt_PahoWithFona_H
#define INC_Bt_PahoWithFona_H

#include <Arduino.h>
#include <Adafruit_FONA.h>


class Bt_PahoTimer {
public:   
    Bt_PahoTimer() : mInterval(0), mStart(millis()) {  
    }

    Bt_PahoTimer(int ms) : mInterval(ms), mStart(millis()) { 
    }
 
    
    bool expired() {
        return (millis() - mStart) >= mInterval;
    }
    
    void countdown_ms(unsigned long ms) {
        mInterval = ms;
        mStart = millis();
    }
    void countdown(int seconds) {
        countdown_ms(((unsigned long)seconds) * 1000L);
    }
    
    int left_ms() {
        unsigned long delay = millis() - mStart;
        if(delay >= mInterval) {
            return 0;
        }
        return mInterval - delay;
    }
    
private:
    unsigned long mInterval;
    unsigned long mStart; 

};

class Bt_PahoNetworkFona {
public:    

    Bt_PahoNetworkFona(Adafruit_FONA& pFona): mFona(&pFona) {
    }

    int connect(char* hostname, int port) {
        return mFona->TCPconnect(hostname, port);
    }
    
    int read(unsigned char* buffer, int len, int timeout) {
        Bt_PahoTimer timer(timeout);
        int interval = timeout < 30 ? 2 : 10;
        
        while (mFona->TCPavailable() < len)
        {
            if(timer.expired()){
                return -1;
            }
            delay(interval);
        }
        return mFona->TCPread(buffer, len);
    }
    
    int write(unsigned char* buffer, int len, int timeout){
       Serial.print("FN.write - ");Serial.print(len);Serial.print(" :");
       if(mFona->TCPsend((char*)buffer, len)) {
          return len;
       }
       return 0;
    }
    
    int disconnect() {
        mFona->TCPclose();
        return 0;
    }

private:
    Adafruit_FONA* mFona;
};

#endif
