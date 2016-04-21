#include <Wire.h>
#include <BtMcuCore.h>
#include <Bt_INA219.h>
#include <Bt/Core/Wire.hpp>
#include <Bt/Core/Logger.hpp>
#include <RTCZero.h>


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
         INA219_MODE_SANDBVOLT_CONTINUOUS;

const uint16_t sConfigurationBattery =
         INA219_BUS_VOLTAGE_RANGE_16V |
         INA219_GAIN_2_80MV |
         INA219_BADCRES_12BIT |
         INA219_SADCRES_12BIT_1S_532US |
         // INA219_MODE_SANDBVOLT_CONTINUOUS;
         INA219_MODE_SANDBVOLT_CONTINUOUS;

const uint16_t sConfigurationLoad =
         INA219_BUS_VOLTAGE_RANGE_16V |
         INA219_GAIN_2_80MV |
         INA219_BADCRES_12BIT |
         INA219_SADCRES_12BIT_1S_532US |
         // INA219_MODE_SANDBVOLT_CONTINUOUS;
         INA219_MODE_SANDBVOLT_CONTINUOUS;

const uint16_t sConfigurationControl =
         INA219_BUS_VOLTAGE_RANGE_16V |
         INA219_GAIN_2_80MV |
         INA219_BADCRES_12BIT |
         INA219_SADCRES_12BIT_1S_532US |
         // INA219_MODE_SANDBVOLT_CONTINUOUS;
         INA219_MODE_SANDBVOLT_CONTINUOUS;


INA219 sensors[NUMBER_OF_SENSORS] = {
   {sWire,0x40,sConfigurationPanel,   32768,250},
   {sWire,0x44,sConfigurationPanel,   32768,250},
   {sWire,0x41,sConfigurationBattery, 16384,500},
   {sWire,0x45,sConfigurationBattery, 16384,500},
   {sWire,0x4C,sConfigurationLoad,    16384,500},
   {sWire,0x48,sConfigurationControl,  8192, 50}
};

RTCZero sRtc;


void setup() {
   Wire.begin();
   sRtc.begin();
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
   for (size_t i=0 ; i < NUMBER_OF_SENSORS ; i++) {
      measure(sensors[i],i);
      delay(100);
   }

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

// --------

/*
void setupTimer() {
   // Set up the generic clock (GCLK4) used to clock timers
  REG_GCLK_GENDIV = GCLK_GENDIV_DIV(3) |          // Divide the 48MHz clock source by divisor 3: 48MHz/3=16MHz
                    GCLK_GENDIV_ID(4);            // Select Generic Clock (GCLK) 4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
                     GCLK_GENCTRL_GENEN |         // Enable GCLK4
                     GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
                     GCLK_GENCTRL_ID(4);          // Select GCLK4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  // Feed GCLK4 to TC4 and TC5
  REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TC4 and TC5
                     GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
                     GCLK_CLKCTRL_ID_TC4_TC5;     // Feed the GCLK4 to TC4 and TC5
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  REG_TC4_COUNT8_CC0 = 0x55;                      // Set the TC4 CC0 register to some arbitary value
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
  REG_TC4_COUNT8_CC1 = 0xAA;                      // Set the TC4 CC1 register to some arbitary value
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
  REG_TC4_COUNT8_PER = 0xFF;                      // Set the PER (period) register to its maximum value
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

  //NVIC_DisableIRQ(TC4_IRQn);
  //NVIC_ClearPendingIRQ(TC4_IRQn);
  NVIC_SetPriority(TC4_IRQn, 0);    // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest)
  NVIC_EnableIRQ(TC4_IRQn);         // Connect TC4 to Nested Vector Interrupt Controller (NVIC)

  REG_TC4_INTFLAG |= TC_INTFLAG_MC1 | TC_INTFLAG_MC0 | TC_INTFLAG_OVF;        // Clear the interrupt flags
  REG_TC4_INTENSET = TC_INTENSET_MC1 | TC_INTENSET_MC0 | TC_INTENSET_OVF;     // Enable TC4 interrupts
  // REG_TC4_INTENCLR = TC_INTENCLR_MC1 | TC_INTENCLR_MC0 | TC_INTENCLR_OVF;     // Disable TC4 interrupts

  REG_TC4_CTRLA |= TC_CTRLA_PRESCALER_DIV64 |     // Set prescaler to 64, 16MHz/64 = 256kHz
                   TC_CTRLA_MODE_COUNT8 |         // Set the counter to 8-bit mode
                   TC_CTRLA_ENABLE;               // Enable TC4
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
}


void TC4_Handler()                              // Interrupt Service Routine (ISR) for timer TC4
{
  // Check for overflow (OVF) interrupt
  if (TC4->COUNT8.INTFLAG.bit.OVF && TC4->COUNT8.INTENSET.bit.OVF)
  {
    // Put your timer overflow (OVF) code here:
    //Serial.println("OVF");

    REG_TC4_INTFLAG = TC_INTFLAG_OVF;         // Clear the OVF interrupt flag
  }

  // Check for match counter 0 (MC0) interrupt
  if (TC4->COUNT8.INTFLAG.bit.MC0 && TC4->COUNT8.INTENSET.bit.MC0)
  {
    // Put your counter compare 0 (CC0) code here:
    // ...
    //Serial.println("CC0");
    REG_TC4_INTFLAG = TC_INTFLAG_MC0;         // Clear the MC0 interrupt flag
  }

  // Check for match counter 1 (MC1) interrupt
  if (TC4->COUNT8.INTFLAG.bit.MC1 && TC4->COUNT8.INTENSET.bit.MC1)
  {
    // Put your counter compare 1 (CC1) code here:
    // ...
    //Serial.println("CC1");
    REG_TC4_INTFLAG = TC_INTFLAG_MC1;        // Clear the MC1 interrupt flag
  }
}
*/

