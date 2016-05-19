//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Wire
//
//*************************************************************************************************

#include "Bt/Core/Wire.hpp"

#include <Arduino.h>
#include <Wire.h>

namespace Bt {
namespace Core {


WireWrapper::WireWrapper() {

}

WireWrapper::~WireWrapper() {

}

void WireWrapper::beginTransmission(uint8_t pAddress) {
   Wire.beginTransmission(pAddress);
}

uint8_t WireWrapper::endTransmission() {
   return Wire.endTransmission();
}

size_t WireWrapper::write(uint8_t pData) {
   return Wire.write(pData);
}

size_t WireWrapper::write(const uint8_t* pData, size_t pQuantity) {
   return Wire.write(pData, pQuantity);
}

uint8_t WireWrapper::requestFrom(uint8_t pAddress, size_t pQuantity) {
   return Wire.requestFrom(pAddress, pQuantity);
}

int WireWrapper::read() {
   return Wire.read();
}

} // namespace Core
} // namespace Bt

