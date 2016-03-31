//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Logger
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Logger__hpp
#define INC__Bt_Core_Logger__hpp

#include "Bt/Core/Platform.hpp"





// The available logging levels.
#define BT_LOG_LEVEL_NONE    0  // nothing is logged
#define BT_LOG_LEVEL_ERROR   1  // only errors are logged
#define BT_LOG_LEVEL_WARNING 2  // errors and warnings are logged
#define BT_LOG_LEVEL_INFO    3  // errors and warnings and info are logged
#define BT_LOG_LEVEL_DEBUG   4  // everything is logged

// Modify this to change the logging level. Reducing the logging level reduces the code size
// the strings are removed from the source code.
#define BT_LOGGING BT_LOG_LEVEL_DEBUG

//-------------------------------------------------------------------------------------------------
// logging detail stuff
//-------------------------------------------------------------------------------------------------

#if BT_LOGGING > BT_LOG_LEVEL_NONE

#define BT_LOG_MESSAGE(prefix,message) BT_PLATFORM_LOG_MESSAGE(prefix,message)

#else

#define BT_LOG_MESSAGE(prefix,message)

#endif


// -- BT_LOG_LEVEL_ERROR
#if BT_LOGGING >= BT_LOG_LEVEL_ERROR

#define BT_LOG_ERROR(message) BT_LOG_MESSAGE(" E: ",message)

#else

#define BT_LOG_ERROR(message)

#endif

// -- BT_LOG_LEVEL_WARNING
#if BT_LOGGING >= BT_LOG_LEVEL_WARNING

#define BT_LOG_WARNING(message) BT_LOG_MESSAGE(" W: ",message)

#else

#define BT_LOG_WARNING(message)

#endif

// -- BT_LOG_LEVEL_INFO
#if BT_LOGGING >= BT_LOG_LEVEL_INFO

#define BT_LOG_INFO(message) BT_LOG_MESSAGE(" I: ", message)

#else

#define BT_LOG_INFO(message)

#endif

// -- BT_LOG_LEVEL_DEBUG
#if BT_LOGGING >= BT_LOG_LEVEL_DEBUG

#define BT_LOG_DEBUG(message) BT_LOG_MESSAGE(" D: ", message)

#else

#define BT_LOG_DEBUG(message)

#endif


namespace Bt {
namespace Core {

enum _EndLineCode { endl };


} // namespace Core
} // namespace Bt




#endif // INC__Bt_Core_Logger__hpp
