//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Measurement
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Measurement__hpp
#define INC__Bt_SolarMonitor_Measurement__hpp

#include <math.h>

namespace Bt {
namespace SolarMonitor {

class Measurement
{
   public:
      Measurement() : mCurrent(0), mVoltage(0) {
      }

      Measurement(float pCurrent, float pVoltage) : mCurrent(pCurrent), mVoltage(pVoltage) {
      }

      float current() const { return mCurrent; }
      float voltage() const { return mVoltage; }

      Measurement& operator+=(const Measurement& rhs) {
         mCurrent += rhs.mCurrent;
         mVoltage += rhs.mVoltage;
         return *this;
      }

      Measurement& operator/=(double rhs) {
         mCurrent /= rhs;
         mVoltage /= rhs;
         return *this;
      }

   private:

      float mCurrent;
      float mVoltage;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Measurement__hpp
