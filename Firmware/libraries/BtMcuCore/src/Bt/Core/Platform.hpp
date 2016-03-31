//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Platform
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Platform__hpp
#define INC__Bt_Core_Platform__hpp

#if ARDUINO
 #include "Arduino.h"

template<class T>
inline Prin t &operator <<(Print &stream, T arg) {
   stream.print(arg);
   return stream;
}

inline Print &operator <<(Print &stream, _EndLineCode arg) {
   stream.println();
   return stream;
}

#define BT_PLATFORM_LOG_MESSAGE(prefix,message) Serial << millis() << F(prefix) << message << ::Bt::Core::endl ;

#else

#include <iostream>
#include <typeinfo>

#define LOG(msg) \
    std::cout << "BtLog>" << msg << std::endl


#endif


#endif // INC__Bt_Core_Platform__hpp
