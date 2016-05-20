//*************************************************************************************************
//
//  BITTAILOR.CH - Firmware     
//
//*************************************************************************************************

#include "Bt/Core/DigitalIn.hpp"

#include <Arduino.h>

namespace Bt {
namespace Core {

DigitalIn::DigitalIn(uint8_t pPin) : mPin(pPin) {
}

DigitalIn::~DigitalIn() {

}

void DigitalIn::begin() {
   pinMode(mPin, INPUT_PULLUP);
}

bool DigitalIn::read() {
   return digitalRead(mPin);
}

} // namespace Core
} // namespace Bt
