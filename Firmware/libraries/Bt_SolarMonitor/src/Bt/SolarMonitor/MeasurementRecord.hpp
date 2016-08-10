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
      MeasurementRecord(){
      }

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

      MeasurementRecord& operator+=(const MeasurementRecord& rhs) {
         mPanelA += rhs.mPanelA;
         mPanelB += rhs.mPanelB;
         mBatteryA += rhs.mBatteryA;
         mBatteryB += rhs.mBatteryB;
         mLoad += rhs.mLoad;
         mControl += rhs.mControl;
         return *this;
      }

      MeasurementRecord& operator/=(double rhs) {
         mPanelA /= rhs;
         mPanelB /= rhs;
         mBatteryA /= rhs;
         mBatteryB /= rhs;
         mLoad /= rhs;
         mControl /= rhs;
         return *this;
      }

      friend MeasurementRecord operator+(MeasurementRecord lhs,const MeasurementRecord& rhs)
      {
         lhs += rhs;
         return lhs;
      }

      friend MeasurementRecord operator/(MeasurementRecord lhs, double rhs)
      {
         lhs /= rhs;
         return lhs;
      }

   private:

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
