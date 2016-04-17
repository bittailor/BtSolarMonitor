//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Function
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Function__hpp
#define INC__Bt_Core_Function__hpp

namespace Bt {
namespace Core {

template <typename Signature> class Function;

// ----------------------------------------------------------------------------
// Macros
// ----------------------------------------------------------------------------

#define BT_CORE_FUNCTION_STD_OPERATORS                                        \
operator bool() const {                                                       \
  return mCaller != 0;                                                        \
}                                                                             \
bool operator!() const {                                                      \
  return !(operator bool());                                                  \
}                                                                             \
bool operator==(const Function& other) const {                                \
  return ((mObject == other.mObject) && (mCaller == other.mCaller));          \
}


// ----------------------------------------------------------------------------
// 0 Argument
// ----------------------------------------------------------------------------

template <typename TReturn> class Function<TReturn()> {
   public:
      Function(): mObject(nullptr), mCaller(nullptr){}

      template <typename TTarget, TReturn (TTarget::*TMethod)()>
      static Function<TReturn()> build(TTarget& pObject) {
         return Function<TReturn()>(&pObject, &methodStub<TTarget,TMethod>);
      }

      TReturn operator()() {
         return mCaller(mObject);
      }

      BT_CORE_FUNCTION_STD_OPERATORS

   private:
      typedef TReturn (*Caller)(void *);

      Function(void* pObject, Caller pCaller): mObject(pObject), mCaller(pCaller){}

      void* mObject;
      Caller mCaller;

      template <class TTarget, TReturn (TTarget::*TMethod)()>
      static TReturn methodStub(void* pObject) {
            TTarget* target = static_cast<TTarget*>(pObject);
            return (target->*TMethod)();
      }

};

// ----------------------------------------------------------------------------
// 1 Argument
// ----------------------------------------------------------------------------

template <typename TReturn, typename TArg1> class Function<TReturn(TArg1)> {
   public:
      Function(): mObject(nullptr), mCaller(nullptr){}

      template <typename TTarget, TReturn (TTarget::*TMethod)(TArg1)>
      static Function<TReturn(TArg1)> build(TTarget& pObject) {
         return Function<TReturn(TArg1)>(&pObject, &methodStub<TTarget,TMethod>);
      }

      TReturn operator()(TArg1 pArgument1) {
         return mCaller(mObject,pArgument1);
      }

      BT_CORE_FUNCTION_STD_OPERATORS

   private:
      typedef TReturn (*Caller)(void *, TArg1);

      Function(void* pObject, Caller pCaller): mObject(pObject), mCaller(pCaller){}

      void* mObject;
      Caller mCaller;

      template <class TTarget, TReturn (TTarget::*TMethod)(TArg1)>
      static TReturn methodStub(void* pObject, TArg1 pArgument1) {
            TTarget* target = static_cast<TTarget*>(pObject);
            return (target->*TMethod)(pArgument1);
      }

};



// ----------------------------------------------------------------------------
// 2 Arguments
// ----------------------------------------------------------------------------

template <typename TReturn, typename TArg1, typename TArg2> class Function<TReturn(TArg1,TArg2)> {
   public:
      Function(): mObject(nullptr), mCaller(nullptr){}

      template <typename TTarget, TReturn (TTarget::*TMethod)(TArg1,TArg2)>
      static Function<TReturn(TArg1,TArg2)> build(TTarget& pObject) {
         return Function<TReturn(TArg1,TArg2)>(&pObject, &methodStub<TTarget,TMethod>);
      }

      TReturn operator()(TArg1 pArgument1, TArg2 pArgument2) {
         return mCaller(mObject, pArgument1, pArgument2);
      }

      BT_CORE_FUNCTION_STD_OPERATORS

   private:
      typedef TReturn (*Caller)(void *, TArg1, TArg2);

      Function(void* pObject, Caller pCaller): mObject(pObject), mCaller(pCaller){}

      void* mObject;
      Caller mCaller;

      template <class TTarget, TReturn (TTarget::*TMethod)(TArg1,TArg2)>
      static TReturn methodStub(void* pObject, TArg1 pArgument1, TArg2 pArgument2) {
            TTarget* target = static_cast<TTarget*>(pObject);
            return (target->*TMethod)(pArgument1, pArgument2);
      }

};



// ----------------------------------------------------------------------------
// Macro cleanup
// ----------------------------------------------------------------------------

#undef BT_CORE_FUNCTION_STD_OPERATORS

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------


} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Function__hpp
