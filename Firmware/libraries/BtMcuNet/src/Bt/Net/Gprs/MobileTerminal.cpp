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

namespace {

static const char *const sUrcResponses[] = {
         "+CIPRXGET: 1,",    /* incoming socket data notification */
         "+FTPGET: 1,",      /* FTP state change notification */
         "+PDP: DEACT",      /* PDP disconnected */
         "+SAPBR 1: DEACT",  /* PDP disconnected (for SAPBR apps) */
         "*PSNWID: ",        /* AT+CLTS network name */
         "*PSUTTZ: ",        /* AT+CLTS time */
         "+CTZV: ",          /* AT+CLTS timezone */
         "DST: ",            /* AT+CLTS dst information */
         "+CIEV: ",          /* AT+CLTS undocumented indicator */
         "RDY",              /* Assorted crap on newer firmware releases. */
         "Call Ready",
         "SMS Ready",
         "NORMAL POWER DOWN",
         "UNDER-VOLTAGE POWER DOWN",
         "UNDER-VOLTAGE WARNNING",
         "OVER-VOLTAGE POWER DOWN",
         "OVER-VOLTAGE WARNNING",
         NULL
};



bool isUrcResponse(const char *line)
{
    for (int i=0; sUrcResponses[i] != NULL; i++) {
       if (!strncmp(line, sUrcResponses[i], strlen(sUrcResponses[i]))) {
          return true;
       }
    }
    return false;
}


}


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

bool MobileTerminal::bringUpWirelessConnection(const char* pApn, const char* pUser, const char* pPassword) {
   BringUpWirelessConnection command;
   return command.run(*this, pApn, pUser, pPassword);
}

int MobileTerminal::connect(const char* pHostname, int pPort) {
   Connect command;
   return command.run(*this, pHostname, pPort);
}

bool MobileTerminal::checkConnected() {
   CheckConnected command;
   return command.run(*this);
}

int MobileTerminal::write(unsigned char* pBuffer, int pLen, int pTimeout) {
   Write command;
   return command.run(*this, pBuffer, pLen, pTimeout);
}

int MobileTerminal::read(unsigned char* pBuffer, int pLen, int pTimeout) {
   Read command;
   return command.run(*this, pBuffer, pLen, pTimeout);
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
   while(true) {
      const char* line = mLineReader.readLine(*mStream, pTimer);
      if(line == nullptr) {
         LOG_MT("  <-(TIMEOUT)- ");
         return nullptr;
      }
      if(!isUrcResponse(line)) {
         LOG_MT("  <--- " << line);
         return line;
      }
      LOG_MT("  <-(ignore urc)-- " << line);
   }
}

const char* MobileTerminal::readPrompt(Bt::Core::Timer& pTimer) {
   const char* line = mLineReader.readPrompt(*mStream, pTimer);
   if( line == nullptr) {
      LOG_MT("  <-(TIMEOUT)- ");
   } else {
      LOG_MT("  <--- " << line);
   }
   return line;
}

void MobileTerminal::flushInput() {
   while(mStream->available()) {
      LOG_MT(" flush:" << (char)mStream->read());
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
         } else {
            networkRegistrationOk = false;
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

      if (strcmp(line, "+CGATT: 0") == 0) {
         gprsAttachmentOk = false;
         continue;
      }
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

bool MobileTerminal::BringUpWirelessConnection::run(MobileTerminal& pTerminal, const char* pApn, const char* pUser, const char* pPassword) {

   Bt::Core::Timer timer(2000);
   pTerminal.sendCommand("AT+CIPMUX=0");

   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return false; }
      if(line == nullptr) {return false;}

      if (strcmp(line, "OK") == 0) {
         break;
      }

      return false;
   }

   // ---

   timer = 2000;
   pTerminal.sendCommand("AT+CIPQSEND=1");
   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return false; }
      if(line == nullptr) {return false;}

      if (strcmp(line, "OK") == 0) {
         break;
      }

      return false;
   }

   // ---

   timer = 2000;
   pTerminal.sendCommand("AT+CIPRXGET=1");
   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return false; }
      if(line == nullptr) {return false;}

      if (strcmp(line, "OK") == 0) {
         break;
      }

      return false;
   }

   // ---

   timer = 5000;
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
         break;
      }

      return false;
   }

   // ---

   timer = 85000;

   pTerminal.sendCommand("AT+CIICR");

   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return false; }
      if(line == nullptr) { return false; }

      if (strcmp(line, "OK") == 0) {
         break;
      }

      return false;
   }

   // ---

   timer = 10000;
   pTerminal.sendCommand("AT+CIFSR");

   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return false; }
      if(line == nullptr) {return false;}

      int ip[4];
      if (sscanf(line, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]) == 4){
         break;
      }

      return false;
   }



   return true;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

int MobileTerminal::Connect::run(MobileTerminal& pTerminal, const char* pHostname, int pPort) {
   Bt::Core::Timer timer(160000);

   pTerminal.sendCommand("AT+CIPSTATUS");
   bool needsCipShut = false;
   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return -1; }
      if(line == nullptr) { return -1; }

      if (strcmp(line, "OK") == 0) {
         continue;
      }
      char state[30];
      if (sscanf(line, "STATE: %[^\t\n]", state) == 1) {
         LOG("State is " << state);
         if(strcmp(state, "IP INITIAL") || strcmp(state, "IP STATUS")){
            needsCipShut = false;
         }else {
            needsCipShut = true;
         }
         break;
      }
      return -1;
   }

   if(needsCipShut){
      pTerminal.sendCommand("AT+CIPSHUT");
      while(true){
         const char* line = pTerminal.readLine(timer);

         if(timer.expired()) { return -1; }
         if(line == nullptr) { return -1; }

         if (strcmp(line, "SHUT OK") == 0) {
            break;
         }
         return -1;
      }
   }

   char cmd[CMD_BUFFER_SIZE];
   Core::StaticStringBuilder builder(cmd,CMD_BUFFER_SIZE);
   builder.print("AT+CIPSTART=\"TCP\",\"");
   builder.print(pHostname);
   builder.print("\",\"");
   builder.print(pPort);
   builder.print("\"");
   pTerminal.sendCommand(cmd);

   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return -1; }
      if(line == nullptr) { return -1; }

      if (strcmp(line, "OK") == 0) {
         return 0;
      }
      return -1;
   }
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

bool MobileTerminal::CheckConnected::run(MobileTerminal& pTerminal) {
   Bt::Core::Timer timer(5000);

   pTerminal.sendCommand("AT+CIPSTATUS");

   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return false; }
      if(line == nullptr) { return false; }

      if (strcmp(line, "OK") == 0) {
         continue;
      }
      if (strcmp(line, "CONNECT OK") == 0) {
         continue;
      }
      char state[30];
      if (sscanf(line, "STATE: %[^\t\n]", state) == 1) {
         LOG("State is " << state);
         if(strcmp(state, "CONNECT OK") == 0) {
            return true;
         }
         return false;
      }
      return false;
   }
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

int MobileTerminal::Write::run(MobileTerminal& pTerminal, unsigned char* pBuffer, int pLen, int pTimeout) {
   LOG_MT("MobileTerminal::Write " << pLen << "...");

   Bt::Core::Timer timer(pTimeout);
   char cmd[CMD_BUFFER_SIZE];
   Core::StaticStringBuilder builder(cmd,CMD_BUFFER_SIZE);
   builder.print("AT+CIPSEND=");
   builder.print(pLen);
   pTerminal.sendCommand(cmd);

   while(true){
      const char* line = pTerminal.readPrompt(timer);

      if(timer.expired()) { return -1; }
      if(line == nullptr) { return -1; }

      if (strcmp(line, ">") == 0) {
         LOG_MT("have > send data " << pLen);
         break;
      }
      return -1;
   }
   pTerminal.flushInput();
   pTerminal.mStream->write(pBuffer,pLen);

   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return -1;}
      if(line == nullptr) { return -1;}

      int acceptedLen = 0;
      if (sscanf(line, "DATA ACCEPT:%d", &acceptedLen) == 1) {
         return acceptedLen;
      }
      return -1;
   }
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

int MobileTerminal::Read::run(MobileTerminal& pTerminal, unsigned char* pBuffer, int pLen, int pTimeout) {
   LOG_MT("MobileTerminal::Read plen=" << pLen << "  pTimeout=" << pTimeout << "...");



   if(pTimeout < 200 ) {
      pTimeout = 200;
      LOG_MT("increase timeout to " << pTimeout);
   }
   Bt::Core::Timer timer(pTimeout);


   pTerminal.sendCommand("AT+CIPRXGET=4");

   int available = 0;
   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return -1; }
      if(line == nullptr) { return -1; }

      if(sscanf(line,"+CIPRXGET: %*d,%d", &available) == 1) {
         LOG_MT("   available = " << available);
         continue;
      }

      if (strcmp(line, "OK") == 0) {
         if (available >= pLen) {
            LOG_MT("     => ok read " << pLen);
            break;
         }
         return -1;
      }
      return -1;
   }



   int confirmed = -1;
   char cmd[CMD_BUFFER_SIZE];
   Core::StaticStringBuilder builder(cmd,CMD_BUFFER_SIZE);
   builder.print("AT+CIPRXGET=2,");
   builder.print(pLen);
   pTerminal.sendCommand(cmd);

   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return false; }
      if(line == nullptr) {return false;}

      if(sscanf(line,"+CIPRXGET: %*d,%d,%*d", &confirmed) == 1) {
         LOG_MT("  confirmed = " << confirmed);
         if (confirmed >= pLen) {
            break;
         }
      }
      return -1;
   }

   LOG_MT("Read " << pLen << " bytes ...");
   pTerminal.mStream->readBytes(pBuffer,pLen);
   LOG_MT("... done");

   while(true){
      const char* line = pTerminal.readLine(timer);

      if(timer.expired()) { return false; }
      if(line == nullptr) {return false;}

      if (strcmp(line, "OK") == 0) {
         return pLen;
      }

      return -1;
   }

   return -1;

}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

} // namespace Gprs
} // namespace Net
} // namespace Bt


