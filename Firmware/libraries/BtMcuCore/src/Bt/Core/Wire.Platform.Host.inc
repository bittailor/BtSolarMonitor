//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Wire
//
//*************************************************************************************************

#include "Bt/Core/Wire.hpp"

#include "Bt/Core/Logger.hpp"

namespace Bt {
namespace Core {


WireWrapper::WireWrapper() {

}

WireWrapper::~WireWrapper() {

}

void WireWrapper::beginTransmission(uint8_t pAddress) {
   LOG("HOST-Wire> beginTransmission " << pAddress);
}

uint8_t WireWrapper::endTransmission() {
   LOG("HOST-Wire> endTransmission ");
   return 1;
}

size_t WireWrapper::write(uint8_t pData) {
   LOG("HOST-Wire> write " << pData);
   return 1;
}

size_t WireWrapper::write(const uint8_t* pData, size_t pQuantity) {
   LOG("HOST-Wire> write " << pData << "," <<pQuantity );
   return 1;
}

uint8_t WireWrapper::requestFrom(uint8_t pAddress, size_t pQuantity) {
   LOG("HOST-Wire> requestFrom " << pAddress << " " << pQuantity);
   return pQuantity;
}

int WireWrapper::read() {
   LOG("HOST-Wire> read ");
   return 1;
}

} // namespace Core
} // namespace Bt
