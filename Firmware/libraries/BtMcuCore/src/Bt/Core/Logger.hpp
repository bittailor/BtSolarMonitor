//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Logger
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Logger__hpp
#define INC__Bt_Core_Logger__hpp

 #include <Arduino.h>

#if ARDUINO

namespace Bt {
namespace Core {

enum _EndLineCode { endl };


} // namespace Core
} // namespace Bt

template<class T>
inline Print &operator <<(Print &stream, T arg) {
   stream.print(arg);
   return stream;
}

inline Print &operator <<(Print &stream, Bt::Core::_EndLineCode arg) {
   stream.println();
   return stream;
}

#define LOG(msg) \
    Serial << "BtLog:" << millis() << ">" << msg << Bt::Core::endl

#define ERROR(msg) \
    Serial << "BtError:" << millis() << ">" << msg << Bt::Core::endl

#else

#define LOG(msg) \
    std::cout << "BL>" << msg << std::endl

#define ERROR(msg) \
    std::cerr << "BE>" << msg << std::endl

#endif

#endif // INC__Bt_Core_Logger__hpp
