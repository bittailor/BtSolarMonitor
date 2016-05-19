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

      virtual bool checkAtOk(Core::Function<void(bool)> pCallback);

      virtual uint32_t workcycle();

   private:

      class State {
         public:
            State(MobileTerminal& pContext) : mContext(&pContext) {}
            virtual ~State(){}
            virtual void consume(char c) = 0;
            virtual uint32_t delay() = 0;
         protected:
            MobileTerminal* mContext;
      };

      class Idle : public State {
         public:
            Idle(MobileTerminal& pContext) : State(pContext) {}
            virtual void consume(char c) {mContext->mLineReader.consume(c);}
            virtual uint32_t delay() {return 500;}

      };

      class WaitingForResponse : public State {
         public:
            WaitingForResponse(MobileTerminal& pContext) : State(pContext) {}
            virtual void consume(char c) {mContext->mLineReader.consume(c);}
            virtual uint32_t delay() {return Bt::Core::I_Runnable::IMMEDIATELY;}
      };

      class Command {
         public:
            virtual ~Command(){}
            virtual bool handleLine(const char* pLine) = 0;
      };

      class AtOkCommand : public Command {
         public:
            void run(MobileTerminal& pTerminal, Core::Function<void(bool)> pCallback);
            virtual bool handleLine(const char* pLine);
         private:
            Core::Function<void(bool)> mCallback;
      };

      void sendCommand(const char* pCommand);
      void sendLine(const char* pLine);
      void handleLine(const char* pLine);

      void flushInput();

      Stream* mStream;
      LineReader<> mLineReader;
      Idle mIdle;
      WaitingForResponse mWaitingForResponse;
      State* mCurrentState;

      AtOkCommand mAtOkCommand;

      Command* mCurrentCommand;



};

} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_MobileTerminal__hpp
