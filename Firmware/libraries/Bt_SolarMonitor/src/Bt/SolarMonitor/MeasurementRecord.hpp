//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MeasurementRecord
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MeasurementRecord__hpp
#define INC__Bt_SolarMonitor_MeasurementRecord__hpp

#include "Bt/SolarMonitor/Measurement.hpp"

namespace Bt {
namespace SolarMonitor {

class MeasurementRecord
{
   public:
      MeasurementRecord(Measurement pPanelA,
                        Measurement pPanelB,
                        Measurement pBatteryA,
                        Measurement pBatteryB,
                        Measurement pLoad,
                        Measurement pControl)
      :mPanelA(pPanelA)
      ,mPanelB(pPanelB)
      ,mBatteryA(pBatteryA)
      ,mBatteryB(pBatteryB)
      ,mLoad(pLoad)
      ,mControl(pControl) {
      }

      const Measurement& panelA() const {return mPanelA;}
      const Measurement& panelB() const {return mPanelB;}
      const Measurement& batteryA() const {return mBatteryA;}
      const Measurement& batteryB() const {return mBatteryB;}
      const Measurement& load() const {return mLoad;}
      const Measurement& control() const {return mControl;}

   private:
      // Constructor to prohibit copy construction.
      MeasurementRecord(const MeasurementRecord&);

      // Operator= to prohibit copy assignment
      MeasurementRecord& operator=(const MeasurementRecord&);

      Measurement mPanelA;
      Measurement mPanelB;
      Measurement mBatteryA;
      Measurement mBatteryB;
      Measurement mLoad;
      Measurement mControl;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MeasurementRecord__hpp
