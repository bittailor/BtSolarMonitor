//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::MobileTerminal
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Gprs_MobileTerminal__hpp
#define INC__Bt_Net_Gprs_MobileTerminal__hpp

#include <Arduino.h>
#include <Bt/Core/I_Runnable.hpp>
#include <Bt/Core/Function.hpp>
#include <Bt/Core/Timer.hpp>
#include <Bt/Net/Gprs/I_MobileTerminal.hpp>
#include <Bt/Net/Gprs/LineReader.hpp>

namespace Bt {
namespace Net {
namespace Gprs {

class MobileTerminal : public I_MobileTerminal, public Bt::Core::I_Runnable
{
   public:
      MobileTerminal(Stream& pStream);
      virtual ~MobileTerminal();
      MobileTerminal(const MobileTerminal&) = delete;
      MobileTerminal& operator=(const MobileTerminal&) = delete;

      virtual bool checkAtOk();
      virtual bool disableEcho();
      virtual bool checkAndSetPin(const char *pPin);
      virtual bool checkNetworkRegistration();
      virtual bool checkGprsAttachment();

      virtual uint32_t workcycle();

   private:

      class State {
         public:
            State(MobileTerminal& pContext) : mContext(&pContext) {}
            virtual ~State(){}
            virtual uint32_t delay() = 0;
         protected:
            MobileTerminal* mContext;
      };

      class Idle : public State {
         public:
            Idle(MobileTerminal& pContext) : State(pContext) {}
            virtual uint32_t delay() {return 500;}

      };

      class WaitingForResponse : public State {
         public:
            WaitingForResponse(MobileTerminal& pContext) : State(pContext) {}
            virtual uint32_t delay() {return Bt::Core::I_Runnable::IMMEDIATELY;}
      };

//      class Command {
//         public:
//            void run(MobileTerminal& pTerminal);
//      };

      class AtOkCommand {
         public:
            bool run(MobileTerminal& pTerminal);
         private:
      };

      class DisableEchoCommand {
         public:
            bool run(MobileTerminal& pTerminal);
         private:
      };

      class CheckAndSetPinCommand {
         public:
            bool run(MobileTerminal& pTerminal, const char *pPin);
         private:
      };

      class CheckNetworkRegistration {
         public:
            bool run(MobileTerminal& pTerminal);
         private:
      };

      class CheckGprsAttachment {
         public:
            bool run(MobileTerminal& pTerminal);
         private:
      };


      void sendCommand(const char* pCommand);
      void sendLine(const char* pLine);
      const char* readLine(Bt::Core::Timer& pTimer);
      void flushInput();

      Stream* mStream;
      LineReader<> mLineReader;
      Idle mIdle;
      WaitingForResponse mWaitingForResponse;
      State* mCurrentState;

};

} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_MobileTerminal__hpp
