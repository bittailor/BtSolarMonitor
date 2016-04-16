//*************************************************************************************************
//
//  BITTAILOR.CH - INA219
//
//*************************************************************************************************

#include "Bt_INA219.h"

#include <Bt/Core/Timing.hpp>
#include <Bt/Core/Logger.hpp>

#define INA219_REGISTER_CONFIGURATION     (0x00)
#define INA219_REGISTER_SHUNTVOLTAGE      (0x01)
#define INA219_REGISTER_BUSVOLTAGE        (0x02)
#define INA219_REGISTER_POWER             (0x03)
#define INA219_REGISTER_CURRENT           (0x04)
#define INA219_REGISTER_CALIBRATION       (0x05)


INA219::INA219(Bt::Core::I_Wire& pWire, uint8_t pAddress, uint16_t pConfigurationRegister, uint16_t pCalibrationRegister, uint16_t pCurrentLSB)
: mWire(&pWire)
, mAddress(pAddress)
, mConfigurationRegister(pConfigurationRegister)
, mCalibrationRegister(pCalibrationRegister)
, mCurrentLSB(pCurrentLSB) {

}

INA219::~INA219() {

}

void INA219::begin() {
   LOG("write ConfigurationRegister " << mConfigurationRegister);
   writeRegister(INA219_REGISTER_CONFIGURATION, mConfigurationRegister);
   LOG("read ConfigurationRegister " << readRegister(INA219_REGISTER_CONFIGURATION));

   LOG("write CalibrationRegister " << mCalibrationRegister);
   writeRegister(INA219_REGISTER_CALIBRATION, mCalibrationRegister);
   LOG("read CalibrationRegister " << readRegister(INA219_REGISTER_CALIBRATION));

}

float INA219::busVoltage() {
   return rawBusVoltage() * 0.001;
}

float INA219::shuntVoltage() {
   return rawShuntVoltage() * 0.01;
}

float INA219::current() {
   float valueDec = rawCurrent();
   return valueDec / ( 1000 / mCurrentLSB );
}

int16_t INA219::rawBusVoltage() {
   uint16_t value = readRegister(INA219_REGISTER_BUSVOLTAGE);
   // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
   return (int16_t)((value >> 3) * 4);
}

int16_t INA219::rawShuntVoltage() {
   return readRegister(INA219_REGISTER_SHUNTVOLTAGE);
}

int16_t INA219::rawCurrent() {
   return readRegister(INA219_REGISTER_CURRENT);
}

void INA219::writeRegister(uint8_t pRegister, uint16_t pValue)
{
   mWire->beginTransmission(mAddress);
   mWire->write(pRegister);                  // Register
   mWire->write((pValue >> 8) & 0xFF);       // Upper 8-bits
   mWire->write(pValue & 0xFF);              // Lower 8-bits
   mWire->endTransmission();
}

uint16_t INA219::readRegister(uint8_t pRegister)
{

   mWire->beginTransmission(mAddress);
   mWire->write(pRegister);                  // Register
   mWire->endTransmission();

   Bt::Core::delayInMicroseconds(700); // Max 12-bit conversion time is 586us per sample

   mWire->requestFrom(mAddress, sizeof(uint16_t));
   return ((mWire->read() << 8) | mWire->read());
}


/*

void Adafruit_INA219::setCalibration_load(void) {

  ina219_calValue = 32768;

  ina219_currentDivider_mA = 4;  // Current LSB = 50uA per bit (1000/50 = 20)
  Serial.print("cDiv = ");Serial.println(ina219_currentDivider_mA);
  ina219_powerDivider_mW = 1;     // Power LSB = 1mW per bit

  // Set Calibration register to 'Cal' calculated above
  wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);

  // Set Config register to take into account the settings above
  uint16_t config = INA219_CONFIG_BVOLTAGERANGE_16V |
                    INA219_CONFIG_GAIN_1_40MV |
                    INA219_CONFIG_BADCRES_12BIT |
                    INA219_CONFIG_SADCRES_12BIT_1S_532US |
                    INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  wireWriteRegister(INA219_REG_CONFIG, config);
}



 */





