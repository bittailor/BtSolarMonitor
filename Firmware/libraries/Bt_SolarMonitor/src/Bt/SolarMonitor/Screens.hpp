//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Screens
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Screens__hpp
#define INC__Bt_SolarMonitor_Screens__hpp

#include <Bt/Core/PlacementPointer.hpp>

#include "Bt/SolarMonitor/I_Screen.hpp"
#include "Bt/SolarMonitor/MeasurementRecord.hpp"
#include "Bt/SolarMonitor/PowerState.hpp"


namespace Bt {
namespace SolarMonitor {

class Screens
{
   public:
      Screens(I_Screen& pOne, I_Screen& pTwo);
      ~Screens();

      void updateMeasurementRecord(const MeasurementRecord& pMeasurementRecord);
      void updatePowerState(PowerState::State pPowerState);
      void updateRSSI(int pRSSI);
      void updateGsmState(int pGsmState);
      void updateCounter(int pCounter);

   private:
   	  // Constructor to prohibit copy construction.
      Screens(const Screens&);

      // Operator= to prohibit copy assignment
      Screens& operator=(const Screens&);

      void updateScreens();
      void drawPanelCurrent(size_t pScreenIndex, float pCurrent);
      void drawBatteryVoltage(size_t pScreenIndex, float pVoltage);
      void drawPowerState(size_t pScreenIndex);
      void drawConnectionState(size_t pScreenIndex);
      void drawMeasureValue(double value, size_t pScreenIndex);

      I_Screen* mScreens[2];
      MeasurementRecord mMeasurementRecord;
      PowerState::State mPowerState;
      int mRSSI;
      int mGsmState;
      int mCounter;



};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Screens__hpp
