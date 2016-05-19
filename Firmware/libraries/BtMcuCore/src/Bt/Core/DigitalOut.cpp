//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::DigitalOut
//
//*************************************************************************************************

#include "Bt/Core/DigitalOut.hpp"

#include <Arduino.h>

namespace Bt {
namespace Core {

DigitalOut::DigitalOut(uint8_t pPin) : mPin(pPin) {
   pinMode(mPin, OUTPUT);
}

DigitalOut::~DigitalOut() {

}

void DigitalOut::write(bool pValue) {
   digitalWrite(mPin,pValue ? HIGH : LOW);
}

} // namespace Core
} // namespace Bt

