#include <Wire.h>
#include <BtMcuCore.h>
#include <Bt_INA219.h>
#include <Bt/Core/Wire.hpp>
#include <Bt/Core/Logger.hpp>

#define NUMBER_OF_SENSORS 6

Bt::Core::WireWrapper sWire;

uint8_t addresses[NUMBER_OF_SENSORS] = {
         0x40,
         0x44,
         0x41,
         0x45,
         0x4C,
         0x48
};

const uint16_t sConfigurationPanel =
         INA219_BUS_VOLTAGE_RANGE_16V |
         INA219_GAIN_1_40MV |
         INA219_BADCRES_12BIT |
         INA219_SADCRES_12BIT_1S_532US |
         // INA219_MODE_SANDBVOLT_CONTINUOUS;
         INA219_MODE_POWERDOWN;

const uint16_t sConfigurationBattery =
         INA219_BUS_VOLTAGE_RANGE_16V |
         INA219_GAIN_2_80MV |
         INA219_BADCRES_12BIT |
         INA219_SADCRES_12BIT_1S_532US |
         // INA219_MODE_SANDBVOLT_CONTINUOUS;
         INA219_MODE_POWERDOWN;

const uint16_t sConfigurationLoad =
         INA219_BUS_VOLTAGE_RANGE_16V |
         INA219_GAIN_2_80MV |
         INA219_BADCRES_12BIT |
         INA219_SADCRES_12BIT_1S_532US |
         // INA219_MODE_SANDBVOLT_CONTINUOUS;
         INA219_MODE_POWERDOWN;

const uint16_t sConfigurationControl =
         INA219_BUS_VOLTAGE_RANGE_16V |
         INA219_GAIN_2_80MV |
         INA219_BADCRES_12BIT |
         INA219_SADCRES_12BIT_1S_532US |
         // INA219_MODE_SANDBVOLT_CONTINUOUS;
         INA219_MODE_POWERDOWN;


INA219 sensors[NUMBER_OF_SENSORS] = {
   {sWire,0x40,sConfigurationPanel,   32768,250},
   {sWire,0x44,sConfigurationPanel,   32768,250},
   {sWire,0x41,sConfigurationBattery, 16384,500},
   {sWire,0x45,sConfigurationBattery, 16384,500},
   {sWire,0x4C,sConfigurationLoad,    16384,500},
   {sWire,0x48,sConfigurationControl,  8192, 50}
};

void setup() {
   Wire.begin();
   pinMode(13, OUTPUT);
   digitalWrite(13, HIGH);
   //setupTimer();
   while (!Serial) {}
   Serial.begin(115200);
   Serial.println("** Spike Power Sensors BT**");
   digitalWrite(13, LOW);

   for (size_t i=0 ; i < NUMBER_OF_SENSORS ; i++) {
      sensors[i].begin();
      delay(200);
   }

   delay(1000);
}
void loop() {
   Serial.println("  -- loop --");
   /*
   for (size_t i=0 ; i < NUMBER_OF_SENSORS ; i++) {
      measure(sensors[i],i);
      delay(100);
   }
   */

   delay(1000);
}

void measure(INA219& sensor, uint8_t id){
   Serial.print(id);
   Serial.print(": ");

   float v = sensor.busVoltage();
   int16_t iRaw = sensor.rawCurrent();
   float i = sensor.current();
   float vs = sensor.shuntVoltage();

   Serial.print(" u = ");
   Serial.print(v);
   Serial.print(" i = ");
   Serial.print(i);
   Serial.print(" i(raw) = ");
   Serial.print(iRaw);
   Serial.print(" u(s) = ");
   Serial.print(vs);
   Serial.println();
}

