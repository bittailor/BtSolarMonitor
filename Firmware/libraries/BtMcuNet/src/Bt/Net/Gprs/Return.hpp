//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::Return
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Gprs_Return__hpp
#define INC__Bt_Net_Gprs_Return__hpp

namespace Bt {
namespace Net {
namespace Gprs {

enum ReturnCode { RC_SUCCESS, RC_TIMEOUT, RC_ERROR, RC_FAILURE };

template<typename T>
class Return {
   public:
      Return(ReturnCode pState):mState(pState){}
      Return(T pValue):mState(RC_SUCCESS),mValue(pValue) {}
      Return(ReturnCode pState, T pValue):mState(pState),mValue(pValue) {}

      explicit operator bool() const { return mState == RC_SUCCESS; }

      ReturnCode state() { return mState; }
      T value() { return mValue; }

   private:
      ReturnCode mState;
      T mValue;
};

template<>
class Return<void> {
   public:
      Return(ReturnCode pState):mState(pState){}

      explicit operator bool() const { return mState == RC_SUCCESS; }

      ReturnCode state() { return mState; }

   private:
      ReturnCode mState;
};


} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_Return__hpp
