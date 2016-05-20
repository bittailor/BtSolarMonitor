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
}

DigitalOut::~DigitalOut() {

}

void DigitalOut::begin() {
   pinMode(mPin, OUTPUT);
}

void DigitalOut::write(bool pValue) {
   pinMode(mPin, OUTPUT);
   digitalWrite(mPin,pValue ? HIGH : LOW);
}

} // namespace Core
} // namespace Bt

