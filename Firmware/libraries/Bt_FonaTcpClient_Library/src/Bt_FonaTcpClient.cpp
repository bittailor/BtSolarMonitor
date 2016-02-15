#include <Bt_FonaTcpClient.h>


Bt_FonaTcpClient::Bt_FonaTcpClient(Adafruit_FONA& pFona) : mFona(&pFona){    
}

int Bt_FonaTcpClient::connect(IPAddress ip, uint16_t port){
    char host[32] = {0};    
    mFona->TCPconnect(host, port); 
    for (int i =0; i < 3; i++)
    {
        uint8_t part = ip[i];
        itoa(part, host + strlen(host), 10);
        host[strlen(host)] = '.';
        host[strlen(host) + 1] = 0;
    }
    uint8_t part = ip[3];
    itoa(part, host+strlen(host), 10);
    return mFona->TCPconnect(host, port);
}

int Bt_FonaTcpClient::connect(const char* host, uint16_t port){
    return mFona->TCPconnect(host,port);   
}

size_t Bt_FonaTcpClient::write(uint8_t data){
    char c = data;
    return mFona->TCPsend(&c,1);      
}

size_t Bt_FonaTcpClient::write(const uint8_t *buf, size_t length){
    const char* buffer = reinterpret_cast<const char *>(buf);
    return mFona->TCPsend(buffer,length);    
}

int Bt_FonaTcpClient::available(){
    return mFona->TCPavailable();     
}

int Bt_FonaTcpClient::read(){
    uint8_t c;
    if(mFona->TCPread(&c,1) == 1){
        return c;    
    }    
    return -1;
}

int Bt_FonaTcpClient::read(uint8_t *buf, size_t length){
    return mFona->TCPread(buf,length);    
}

int Bt_FonaTcpClient::peek(){
    return -1; //TODO peak !    
}

void Bt_FonaTcpClient::flush(){
    // no buffering => flush ?    
}

void Bt_FonaTcpClient::stop(){
    mFona->TCPclose();    
}

uint8_t Bt_FonaTcpClient::connected(){
    return mFona->TCPconnected();        
}

Bt_FonaTcpClient::operator bool(){
    return mFona->TCPconnected();       
}


