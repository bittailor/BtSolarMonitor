//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::AtParser
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Gprs_AtParser__hpp
#define INC__Bt_Net_Gprs_AtParser__hpp

#include <Stream.h>
#include <Bt/Core/StateMachine.hpp>
#include <Bt/Net/Gprs/I_AtParser.hpp>

namespace Bt {
namespace Net {
namespace Gprs {

class AtParser : public Core::I_Runnable
{
   public:
      AtParser();
      ~AtParser();


      virtual uint32_t workcycle();


   private:
   	  // Constructor to prohibit copy construction.
      AtParser(const AtParser&);

      // Operator= to prohibit copy assignment
      AtParser& operator=(const AtParser&);
      /*
      class

      class Idlde




      Stream* mStream;
      */

};

} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_AtParser__hpp
