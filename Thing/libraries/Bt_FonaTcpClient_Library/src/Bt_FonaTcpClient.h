#ifndef INC_Bt_FonaTcpClient_H
#define INC_Bt_FonaTcpClient_H

#include <Arduino.h>

class Bt_FonaTcpClient : public Client {

public:
    Bt_FonaTcpClient(Adafruit_FONA& pFona);
    virtual int connect(IPAddress ip, uint16_t port);
    virtual int connect(const char* host, uint16_t port);
    virtual size_t write(uint8_t data);
    virtual size_t write(const uint8_t *buf, size_t length);
    virtual int available();
    virtual int read();
    virtual int read(uint8_t *buf, size_t length);
    virtual int peek();
    virtual void flush();
    virtual void stop();
    virtual uint8_t connected();
    virtual operator bool();
};

#endif