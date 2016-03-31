//*************************************************************************************************
//
//  BITTAILOR.CH - Firmware     
//
//*************************************************************************************************

#include "Bt/Core/DigitalIn.hpp"

#include "Bt/Core/Logger.hpp"

namespace Bt {
namespace Core {

DigitalIn::DigitalIn(uint8_t pPin) : mPin(pPin) {
   LOG("HOST-IO>" << mPin << " << mode INPUT_PULLUP" );
}

DigitalIn::~DigitalIn() {

}

bool DigitalIn::read() {
   LOG("HOST-IO>" << mPin << " read " << true );
   return true;
}

} // namespace Core
} // namespace Bt
