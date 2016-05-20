//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::MobileTerminal
//
//*************************************************************************************************

#include "Bt/Net/Gprs/MobileTerminal.hpp"

#include "Bt/Core/Logger.hpp"
#include "Bt/Core/StaticStringBuilder.hpp"

namespace Bt {
namespace Net {
namespace Gprs {

#define LOG_MT(msg) LOG(msg)
#define CMD_BUFFER_SIZE 256

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

MobileTerminal::MobileTerminal(Stream& pStream)
: mStream(&pStream)
, mLineReader()
, mIdle(*this)
, mWaitingForResponse(*this)
, mCurrentState(&mIdle) {

}

MobileTerminal::~MobileTerminal() {

}

uint32_t MobileTerminal::workcycle() {
   return FOREVER;
}

bool MobileTerminal::checkAtOk() {
   AtOkCommand command;
   return command.run(*this);
}

bool MobileTerminal::disableEcho() {
   DisableEchoCommand command;
   return command.run(*this);
}

bool MobileTerminal::checkAndSetPin(const char *pPin) {
   CheckAndSetPinCommand command;
   return command.run(*this, pPin);
}

bool MobileTerminal::checkNetworkRegistration() {
   CheckNetworkRegistration command;
   return command.run(*this);
}

bool MobileTerminal::checkGprsAttachment() {
   CheckGprsAttachment command;
   return command.run(*this);
}

bool MobileTerminal::startTaskAndSetAPN(const char* pApn, const char* pUser, const char* pPassword) {
   StartTaskAndSetAPN command;
   return command.run(*this, pApn, pUser, pPassword);
}

bool MobileTerminal::bringUpWirelessConnection() {
   BringUpWirelessConnection command;
   return command.run(*this);
}

bool MobileTerminal::getLocalIp() {
   GetLocalIp command;
   return command.run(*this);
}

void MobileTerminal::sendCommand(const char* pCommand) {
   flushInput();
   mCurrentState = &mWaitingForResponse;
   sendLine(pCommand);
}

void MobileTerminal::sendLine(const char* pLine) {
   LOG_MT("  ---> " << pLine );
   mStream->println(pLine);
}

const char* MobileTerminal::readLine(Bt::Core::Timer& pTimer) {
   const char* line = mLineReader.readLine(*mStream, pTimer);
   if( line == nullptr) {
      LOG_MT("  <-(TIMEOUT)- ");
   } else {
      LOG_MT("  <--- " << line);
   }
   return line;
}

void MobileTerminal::flushInput() {
   while(mStream->available()) {
      mStream->read();
   }
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

bool MobileTerminal::AtOkCommand::run(MobileTerminal& pTerminal) {
   Bt::Core::Timer timer(5000);
   pTerminal.sendCommand("AT");

   while(!timer.expired()){
      const char* line = pTerminal.readLine(timer);
      if(line == nullptr) {
         return false;
      }
      if (strcmp(line, "AT") == 0) {
         continue;
      }
      if (strcmp(line, "OK") == 0) {
         return true;
      }
      return false;
   }
   return false;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

bool MobileTerminal::DisableEchoCommand::run(MobileTerminal& pTerminal) {
   Bt::Core::Timer timer(5000);
   pTerminal.sendCommand("ATE0");

   while(!timer.expired()){
      const char* line = pTerminal.readLine(timer);
      if(line == nullptr) {
         return false;
      }
      if (strcmp(line, "ATE0") == 0) {
         continue;
      }
      if (strcmp(line, "OK") == 0) {
         return true;
      }
      return false;
   }
   return false;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

bool MobileTerminal::CheckAndSetPinCommand::run(MobileTerminal& pTerminal, const char *pPin) {
   Bt::Core::Timer timer(5000);
   pTerminal.sendCommand("AT+CPIN?");
   const char* prefix = "+CPIN: ";
   bool needPin = true;

   while(true){
      if(timer.expired()) {
         return false;
      }
      const char* line = pTerminal.readLine(timer);
      if(line == nullptr) {
         return false;
      }
      if(strncmp(prefix,line,strlen(prefix)) == 0) {
         if(strcmp(line+strlen(prefix),"SIM PIN") == 0) {
            needPin = true;
            continue;
         }
         if(strcmp(line+strlen(prefix),"READY") == 0) {
            needPin = false;
            continue;
         }
      }
      if (strcmp(line, "OK") == 0) {
         break;
      }
      return false;
   }

   if(!needPin) {
      return true;
   }

   char cmd[CMD_BUFFER_SIZE];
   Core::StaticStringBuilder builder(cmd,CMD_BUFFER_SIZE);
   builder.print("AT+CPIN=");
   builder.print(pPin);
   pTerminal.sendCommand(cmd);

   timer = 5000;

   while(true){
      if(timer.expired()) {
         return false;
      }
      const char* line = pTerminal.readLine(timer);
      if(line == nullptr) {
         return false;
      }

      if (strcmp(line, "+CPIN: READY") == 0) {
         return true;
      }
      if (strcmp(line, "OK") == 0) {
         continue;
      }
      return false;
   }

   return false;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

bool MobileTerminal::CheckNetworkRegistration::run(MobileTerminal& pTerminal) {
   Bt::Core::Timer timer(5000);
   pTerminal.sendCommand("AT+CREG?");

   bool networkRegistrationOk = false;

   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return false; }
      if(line == nullptr) {return false;}

      int creg;
      if(sscanf(line,"+CREG: %*d,%d", &creg) == 1) {
         if (creg == 1) {
            networkRegistrationOk = true;
         }
         continue;
      }
      if (strcmp(line, "OK") == 0) {
         return networkRegistrationOk;
      }
      return false;
   }
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

bool MobileTerminal::CheckGprsAttachment::run(MobileTerminal& pTerminal) {
   Bt::Core::Timer timer(5000);
   pTerminal.sendCommand("AT+CGATT?");

   bool gprsAttachmentOk = false;
   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return false; }
      if(line == nullptr) {return false;}

      if (strcmp(line, "+CGATT: 1") == 0) {
         gprsAttachmentOk = true;
         continue;
      }
      if (strcmp(line, "OK") == 0) {
         return gprsAttachmentOk;
      }

      return false;
   }
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

bool MobileTerminal::StartTaskAndSetAPN::run(MobileTerminal& pTerminal, const char* pApn, const char* pUser, const char* pPassword) {
   Bt::Core::Timer timer(5000);

   char cmd[CMD_BUFFER_SIZE];
   Core::StaticStringBuilder builder(cmd,CMD_BUFFER_SIZE);
   builder.print("AT+CSTT=\"");
   builder.print(pApn);
   if(pUser != nullptr) {
      builder.print("\",\"");
      builder.print(pUser);
   }
   if(pPassword != nullptr) {
      builder.print("\",\"");
      builder.print(pPassword);
   }
   builder.print("\"");
   pTerminal.sendCommand(cmd);

   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return false; }
      if(line == nullptr) {return false;}

      if (strcmp(line, "OK") == 0) {
         return true;
      }

      return false;
   }
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

bool MobileTerminal::BringUpWirelessConnection::run(MobileTerminal& pTerminal) {
   Bt::Core::Timer timer(85000);
   pTerminal.sendCommand("AT+CIICR");

   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return false; }
      if(line == nullptr) {return false;}

      if (strcmp(line, "OK") == 0) {
         return true;
      }

      return false;
   }
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

bool MobileTerminal::GetLocalIp::run(MobileTerminal& pTerminal) {
   Bt::Core::Timer timer(5000);
   pTerminal.sendCommand("AT+CIFSR");

   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return false; }
      if(line == nullptr) {return false;}

      int ip[4];
      if (sscanf(line, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]) == 4){
         return true;
      }

      return false;
   }
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------


} // namespace Gprs
} // namespace Net
} // namespace Bt


