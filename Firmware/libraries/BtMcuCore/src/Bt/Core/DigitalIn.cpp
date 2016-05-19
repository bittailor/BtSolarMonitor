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
  pinMode(mPin, INPUT_PULLUP);
}

DigitalIn::~DigitalIn() {

}

bool DigitalIn::read() {
   return digitalRead(mPin);
}

} // namespace Core
} // namespace Bt
