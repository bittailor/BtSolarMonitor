#include <Wire.h>
#include <BtMcuCore.h>
#include <Bt/Core/Wire.hpp>
#include <Bt/Core/Logger.hpp>

#define INA219_REGISTER_CONFIGURATION     (0x00)

#define INA219_BUS_VOLTAGE_RANGE_16V     (0x0000)  // 0-16V Range
#define INA219_BUS_VOLTAGE_RANGE_32V     (0x2000)  // 0-32V Range

#define INA219_GAIN_1_40MV               (0x0000)  // Gain 1, 40mV Range
#define INA219_GAIN_2_80MV               (0x0800)  // Gain 2, 80mV Range
#define INA219_GAIN_4_160MV              (0x1000)  // Gain 4, 160mV Range
#define INA219_GAIN_8_320MV              (0x1800)  // Gain 8, 320mV Range

#define INA219_BADCRES_9BIT              (0x0080)  // 9-bit bus res = 0..511
#define INA219_BADCRES_10BIT             (0x0100)  // 10-bit bus res = 0..1023
#define INA219_BADCRES_11BIT             (0x0200)  // 11-bit bus res = 0..2047
#define INA219_BADCRES_12BIT             (0x0400)  // 12-bit bus res = 0..4097

#define INA219_SADCRES_9BIT_1S_84US      (0x0000)  // 1 x 9-bit shunt sample
#define INA219_SADCRES_10BIT_1S_148US    (0x0008)  // 1 x 10-bit shunt sample
#define INA219_SADCRES_11BIT_1S_276US    (0x0010)  // 1 x 11-bit shunt sample
#define INA219_SADCRES_12BIT_1S_532US    (0x0018)  // 1 x 12-bit shunt sample
#define INA219_SADCRES_12BIT_2S_1060US   (0x0048)  // 2 x 12-bit shunt samples averaged together
#define INA219_SADCRES_12BIT_4S_2130US   (0x0050)  // 4 x 12-bit shunt samples averaged together
#define INA219_SADCRES_12BIT_8S_4260US   (0x0058)  // 8 x 12-bit shunt samples averaged together
#define INA219_SADCRES_12BIT_16S_8510US  (0x0060)  // 16 x 12-bit shunt samples averaged together
#define INA219_SADCRES_12BIT_32S_17MS    (0x0068)  // 32 x 12-bit shunt samples averaged together
#define INA219_SADCRES_12BIT_64S_34MS    (0x0070)  // 64 x 12-bit shunt samples averaged together
#define INA219_SADCRES_12BIT_128S_69MS   (0x0078)  // 128 x 12-bit shunt samples averaged together

#define INA219_MODE_POWERDOWN            (0x0000)
#define INA219_MODE_SVOLT_TRIGGERED      (0x0001)
#define INA219_MODE_BVOLT_TRIGGERED      (0x0002)
#define INA219_MODE_SANDBVOLT_TRIGGERED  (0x0003)
#define INA219_MODE_ADCOFF               (0x0004)
#define INA219_MODE_SVOLT_CONTINUOUS     (0x0005)
#define INA219_MODE_BVOLT_CONTINUOUS     (0x0006)
#define INA219_MODE_SANDBVOLT_CONTINUOUS (0x0007)

#define ADDRESS 0x44

void writeRegister(uint8_t pRegister, uint16_t pValue)
{
   Wire.beginTransmission(ADDRESS);
   Wire.write(pRegister);                  // Register
   Wire.write((pValue >> 8) & 0xFF);       // Upper 8-bits
   Wire.write(pValue & 0xFF);              // Lower 8-bits
   Wire.endTransmission();
}

uint16_t readRegister(uint8_t pRegister)
{

   Wire.beginTransmission(ADDRESS);
   Wire.write(pRegister);                  // Register
   Wire.endTransmission();

   //delay(700); // Max 12-bit conversion time is 586us per sample

   Wire.requestFrom(ADDRESS, sizeof(uint16_t));
   return ((Wire.read() << 8) | Wire.read());
}

void setup() {
   Wire.begin();
   while (!Serial) {}
   Serial.begin(115200);
   Serial.println("** Spike Power Sensors BT**");
   delay(1000);
}
void loop() {
   uint16_t configurationRegister =  
         INA219_BUS_VOLTAGE_RANGE_16V |
         INA219_GAIN_1_40MV |
         INA219_BADCRES_12BIT |
         INA219_SADCRES_12BIT_1S_532US |
         INA219_MODE_SANDBVOLT_CONTINUOUS;
  
   LOG("write ConfigurationRegister " << configurationRegister);
   writeRegister(INA219_REGISTER_CONFIGURATION, configurationRegister);
   LOG("read ConfigurationRegister " << readRegister(INA219_REGISTER_CONFIGURATION));

   delay(1000);
}

