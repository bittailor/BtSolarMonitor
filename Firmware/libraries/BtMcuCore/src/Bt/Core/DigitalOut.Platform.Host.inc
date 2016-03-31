//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::DigitalOut
//
//*************************************************************************************************

#include "Bt/Core/DigitalOut.hpp"

#include "Bt/Core/Logger.hpp"

namespace Bt {
namespace Core {

DigitalOut::DigitalOut(uint8_t pPin) : mPin(pPin) {
   LOG("HOST-IO>" << mPin << " << mode OUTPUT" );
}

DigitalOut::~DigitalOut() {

}

void DigitalOut::write(bool pValue) {
   LOG("HOST-IO>" << mPin << " write " << pValue );
}

} // namespace Core
} // namespace Bt
