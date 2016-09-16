//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Gprs::MobileTerminal
//
//*************************************************************************************************

#include "Bt/Net/Gprs/MobileTerminal.hpp"

#include "Bt/Core/Logger.hpp"
#include "Bt/Core/Timing.hpp"
#include "Bt/Core/StaticStringBuilder.hpp"

namespace Bt {
namespace Net {
namespace Gprs {

#define LOG_MT(msg)  LOG(msg)
#define CMD_BUFFER_SIZE 256
#define DEFAULT_QUERY_CMD_TIMEOUT 500
#define FIVE_SECOND_TIMEOUT 5000


namespace {

static const char *const sUrcResponses[] = {
         "+CIPRXGET: 1",    /* incoming socket data notification */
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
         nullptr
};

bool isUrcResponse(const char *line)
{
   for (int i=0; sUrcResponses[i] != nullptr; i++) {
      if (!strncmp(line, sUrcResponses[i], strlen(sUrcResponses[i]))) {
         return true;
      }
   }
   return false;
}

struct ConnectionStatusRecord {
      const char * text;
      ConnectionStatus state;
};

static ConnectionStatusRecord sConnectionStatusRecords[] = {
         {"IP INITIAL"       , IP_INITIAL},
         {"IP START"         , IP_START},
         {"IP CONFIG"        , IP_CONFIG},
         {"IP GPRSACT"       , IP_GPRSACT},
         {"IP STATUS"        , IP_STATUS},
         {"TCP CONNECTING"   , CONNECTING},
         {"UDP CONNECTING"   , CONNECTING},
         {"SERVER LISTENING" , CONNECTING},
         {"CONNECT OK"       , CONNECT_OK},
         {"TCP CLOSING"      , CLOSING},
         {"UDP CLOSING"      , CLOSING},
         {"TCP CLOSED"       , CLOSED},
         {"UDP CLOSED"       , CLOSED},
         {"PDP DEACT"        , PDP_DEACT},
         { nullptr           , IP_INITIAL}
};

Return<ConnectionStatus> covertFromString(const char * statusString) {
   for (int i=0; sConnectionStatusRecords[i].text != nullptr; i++) {
      if (strcmp(statusString, sConnectionStatusRecords[i].text) == 0) {
         return sConnectionStatusRecords[i].state;
      }
   }
   return ReturnCode::RC_ERROR;
}

#define checkLine(line)                              \
         if(line.state() != ReturnCode::RC_SUCCESS) {   \
            return line.state();                        \
         }                                              \
         if (strcmp(line.value(), "ERROR") == 0) {      \
            return ReturnCode::RC_ERROR;                \
         }


}



//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

MobileTerminal::MobileTerminal(Stream& pStream)
: mStream(&pStream)
, mLineReader() {

}

MobileTerminal::~MobileTerminal() {

}

Return<bool> MobileTerminal::checkAtOk() {
   Bt::Core::Timer timer(DEFAULT_QUERY_CMD_TIMEOUT);
   sendCommand("AT");
   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);

      if (strcmp(line.value(), "AT") == 0) {
         continue;
      }
      if (strcmp(line.value(), "OK") == 0) {
         return true;
      }
      return false;
   }
}

Return<void> MobileTerminal::disableEcho() {
   Bt::Core::Timer timer(DEFAULT_QUERY_CMD_TIMEOUT);
   sendCommand("ATE0");

   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);

      if (strcmp(line.value(), "ATE0") == 0) {
         continue;
      }
      if (strcmp(line.value(), "OK") == 0) {
         return ReturnCode::RC_SUCCESS;
      }
      return ReturnCode::RC_FAILURE;
   }
   return ReturnCode::RC_SUCCESS;
}

Return<void> MobileTerminal::checkAndSetPin(const char* pPin) {
   Bt::Core::Timer timer(FIVE_SECOND_TIMEOUT);
   sendCommand("AT+CPIN?");

   bool needPin = true;
   while(true) {
      Return<const char*> line = readLine(timer);
      checkLine(line);

      char code[30];
      if (sscanf(line.value(), "+CPIN: %[^\t\n]", code) == 1) {
         if(strcmp(code, "SIM PIN") == 0) {
            needPin = true;
            continue;
         }
         if(strcmp(code, "READY") == 0) {
            needPin = false;
            continue;
         }

      }
      if (strcmp(line.value(), "OK") == 0) {
         break;
      }

      return ReturnCode::RC_FAILURE;
   }

   if(!needPin) {
      return ReturnCode::RC_SUCCESS;
   }

   char cmd[CMD_BUFFER_SIZE];
   Core::StaticStringBuilder builder(cmd,CMD_BUFFER_SIZE);
   builder.print("AT+CPIN=");
   builder.print(pPin);
   sendCommand(cmd);

   timer = FIVE_SECOND_TIMEOUT;

   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);

      if (strcmp(line.value(), "+CPIN: READY") == 0) {
         return ReturnCode::RC_SUCCESS;
      }
      if (strcmp(line.value(), "OK") == 0) {
         continue;
      }
      return ReturnCode::RC_FAILURE;
   }
}

Return<bool> MobileTerminal::checkNetworkRegistration() {
   Bt::Core::Timer timer(DEFAULT_QUERY_CMD_TIMEOUT);
   sendCommand("AT+CREG?");

   bool networkRegistrationOk = false;
   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);

      int creg;
      if(sscanf(line.value(),"+CREG: %*d,%d", &creg) == 1) {
         if (creg == 1) {
            networkRegistrationOk = true;
         } else {
            networkRegistrationOk = false;
         }
         continue;
      }
      if (strcmp(line.value(), "OK") == 0) {
         return networkRegistrationOk;
      }
      return ReturnCode::RC_FAILURE;
   }
}

Return<int>  MobileTerminal::getRSSI() {
   Bt::Core::Timer timer(DEFAULT_QUERY_CMD_TIMEOUT);
   sendCommand("AT+CSQ");

   int rssi = -1;
   int ber = -1;
   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);
      if(sscanf(line.value(),"+CSQ: %d,%d", &rssi, &ber ) == 2) {
         continue;
      }
      if (strcmp(line.value(), "OK") == 0) {
         return rssi;
      }
      return ReturnCode::RC_FAILURE;
   }
}


Return<bool> MobileTerminal::checkGprsAttachment() {
   Bt::Core::Timer timer(DEFAULT_QUERY_CMD_TIMEOUT);
   sendCommand("AT+CGATT?");

   bool gprsAttachmentOk = false;
   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);

      if (strcmp(line.value(), "+CGATT: 0") == 0) {
         gprsAttachmentOk = false;
         continue;
      }
      if (strcmp(line.value(), "+CGATT: 1") == 0) {
         gprsAttachmentOk = true;
         continue;
      }
      if (strcmp(line.value(), "OK") == 0) {
         return gprsAttachmentOk;
      }
      return ReturnCode::RC_FAILURE;
   }
}


Return<void> MobileTerminal::bringUpWirelessConnection(const char* pApn, const char* pUser, const char* pPassword) {
   Bt::Core::Timer timer(DEFAULT_QUERY_CMD_TIMEOUT);
   sendCommand("AT+CIPMUX=0");
   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);

      if (strcmp(line.value(), "OK") == 0) {
         break;
      }
      return ReturnCode::RC_FAILURE;
   }

   // ---

   timer = DEFAULT_QUERY_CMD_TIMEOUT;
   sendCommand("AT+CIPQSEND=1");
   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);

      if (strcmp(line.value(), "OK") == 0) {
         break;
      }
      return ReturnCode::RC_FAILURE;
   }

   // ---

   timer = DEFAULT_QUERY_CMD_TIMEOUT;
   sendCommand("AT+CIPRXGET=1");
   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);

      if (strcmp(line.value(), "OK") == 0) {
         break;
      }
      return ReturnCode::RC_FAILURE;
   }

   // ---

   timer = DEFAULT_QUERY_CMD_TIMEOUT;
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
   sendCommand(cmd);
   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);

      if (strcmp(line.value(), "OK") == 0) {
         break;
      }
      return ReturnCode::RC_FAILURE;
   }

   // ---

   timer = 85000;
   sendCommand("AT+CIICR");
   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);

      if (strcmp(line.value(), "OK") == 0) {
         break;
      }
      return ReturnCode::RC_FAILURE;
   }

   // ---

   timer = FIVE_SECOND_TIMEOUT;
   sendCommand("AT+CIFSR");

   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);

      int ip[4];
      if (sscanf(line.value(), "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]) == 4){
         break;
      }

      return ReturnCode::RC_FAILURE;
   }

   return ReturnCode::RC_SUCCESS;
}

Return<void> MobileTerminal::connect(const char* pHostname, int pPort) {
   Return<ConnectionStatus> connectionStatus = getConnectionStatus();
   if(!connectionStatus) {
      return connectionStatus.state();
   }
   bool needsCipShut = true;
   if (connectionStatus.value() == ConnectionStatus::IP_INITIAL ||
            connectionStatus.value() == ConnectionStatus::IP_STATUS) {
      needsCipShut = false;
   }

   Core::Timer timer = 65000;
   if(needsCipShut){
      sendCommand("AT+CIPSHUT");
      while(true){
         Return<const char*> line = readLine(timer);
         checkLine(line);

         if (strcmp(line.value(), "SHUT OK") == 0) {
            break;
         }
         return ReturnCode::RC_FAILURE;
      }
   }

   timer = 5000;
   char cmd[CMD_BUFFER_SIZE];
   Core::StaticStringBuilder builder(cmd,CMD_BUFFER_SIZE);
   builder.print("AT+CIPSTART=\"TCP\",\"");
   builder.print(pHostname);
   builder.print("\",\"");
   builder.print(pPort);
   builder.print("\"");
   sendCommand(cmd);

   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);

      if (strcmp(line.value(), "OK") == 0) {
         return ReturnCode::RC_SUCCESS;
      }
      return ReturnCode::RC_FAILURE;
   }
}

Return<void> MobileTerminal::close() {
   Bt::Core::Timer timer(DEFAULT_QUERY_CMD_TIMEOUT);
   sendCommand("AT+CIPCLOSE=1");
   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);
      if (strcmp(line.value(), "CLOSE OK") == 0) {
         return ReturnCode::RC_SUCCESS;
      }
      return ReturnCode::RC_FAILURE;
   }
}

Return<ConnectionStatus> MobileTerminal::getConnectionStatus() {
   Bt::Core::Timer timer(DEFAULT_QUERY_CMD_TIMEOUT);
   sendCommand("AT+CIPSTATUS");

   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);

      if (strcmp(line.value(), "OK") == 0) {
         continue;
      }
      if (strcmp(line.value(), "CONNECT OK") == 0) {
         continue;
      }
      char state[30];
      if (sscanf(line.value(), "STATE: %[^\t\n]", state) == 1) {
         return covertFromString(state);
      }
      return ReturnCode::RC_FAILURE;
   }
}

Return<int> MobileTerminal::write(unsigned char* pBuffer, int pLen, int pTimeout) {
   LOG_MT("MobileTerminal::Write " << pLen << "...");
   Bt::Core::Timer writeTimer(pTimeout);


   Bt::Core::Timer timer(DEFAULT_QUERY_CMD_TIMEOUT);
   char cmd[CMD_BUFFER_SIZE];
   Core::StaticStringBuilder builder(cmd, CMD_BUFFER_SIZE);
   builder.print("AT+CIPSEND=");
   builder.print(pLen);
   sendCommand(cmd);

   while(true){
      Return<const char*> line= readPrompt(timer);
      checkLine(line);

      if (strcmp(line.value(), "> ") == 0) {
         LOG_MT("have > send data " << pLen);
         break;
      }
      return ReturnCode::RC_FAILURE;
   }
   flushInput();
   mStream->write(pBuffer,pLen);
   //mStream->readBytes(pBuffer,pLen);

   timer = DEFAULT_QUERY_CMD_TIMEOUT;
   int acceptedLen = 0;
   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);

      if (sscanf(line.value(), "DATA ACCEPT:%d", &acceptedLen) == 1) {
         break;
      }
      return ReturnCode::RC_FAILURE;
   }

   Core::delayInMilliseconds(100);

   while(true) {
      timer = DEFAULT_QUERY_CMD_TIMEOUT;
      Core::delayInMilliseconds(10);
      sendCommand("AT+CIPACK");
      Return<const char*> line = readLine(timer);
      if(line.state() == ReturnCode::RC_TIMEOUT) {
         if(writeTimer.expired()) {
            LOG_MT("CIPACK send check timeout");
            return ReturnCode::RC_TIMEOUT;
         }
         sendCommand("AT");
         flushInput();
         sendCommand("AT");
         flushInput();
         continue;
      }
      int txlen = 0;
      int acklen = 0;
      int nacklen = 0;
      if (sscanf(line.value(), "+CIPACK: %d,%d,%d", &txlen, &acklen , &nacklen ) != 3) {
         return ReturnCode::RC_FAILURE;
      }
      timer = DEFAULT_QUERY_CMD_TIMEOUT;
      line = readLine(timer);
      if(line.state() == ReturnCode::RC_TIMEOUT) {
         sendCommand("AT");
         flushInput();
         sendCommand("AT");
         flushInput();
         continue;
      }

      if (strcmp(line.value(), "OK") != 0) {
         return ReturnCode::RC_FAILURE;
      }

      LOG_MT("txlen=" << txlen << " acklen=" << acklen <<" nacklen=" << nacklen);

      if(nacklen == 0) {
         return acceptedLen;
      }

      if(writeTimer.expired()) {
         LOG_MT("CIPACK send check timeout");
         return ReturnCode::RC_TIMEOUT;
      }
   }
}

Return<int> MobileTerminal::read(unsigned char* pBuffer, int pLen, int pTimeout) {
   LOG_MT("MobileTerminal::Read plen=" << pLen << "  pTimeout=" << pTimeout << "...");
   Bt::Core::Timer readTimer(pTimeout);

   while(true) {
      Return<int> available = this->available();
      if (available && (available.value() >= pLen)) {
         break;
      }
      if(readTimer.expired()) {
         return ReturnCode::RC_TIMEOUT;
      }
      Core::delayInMilliseconds(100);
   }

   int confirmed = -1;
   char cmd[CMD_BUFFER_SIZE];
   Core::StaticStringBuilder builder(cmd,CMD_BUFFER_SIZE);
   builder.print("AT+CIPRXGET=2,");
   builder.print(pLen);
   sendCommand(cmd);
   Bt::Core::Timer timer(DEFAULT_QUERY_CMD_TIMEOUT);
   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);
      if(sscanf(line.value(),"+CIPRXGET: %*d,%d,%*d", &confirmed) == 1) {
         LOG_MT("  confirmed = " << confirmed);
         if (confirmed >= pLen) {
            break;
         }
      }
      return ReturnCode::RC_FAILURE;
   }

   LOG_MT("Read " << pLen << " bytes ...");
   mStream->readBytes(pBuffer,pLen);
   LOG_MT("... done");

   timer = DEFAULT_QUERY_CMD_TIMEOUT;
   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);

      if (strcmp(line.value(), "OK") == 0) {
         return pLen;
      }

      return ReturnCode::RC_FAILURE;
   }

   return ReturnCode::RC_FAILURE;
}

//-------------------------------------------------------------------------------------------------

Return<int> MobileTerminal::available() {
   Bt::Core::Timer readTimer(DEFAULT_QUERY_CMD_TIMEOUT);
   sendCommand("AT+CIPRXGET=4");
   int available = 0;
   while(true){
      Return<const char*> line = readLine(readTimer);
      checkLine(line);

      if(sscanf(line.value(),"+CIPRXGET: %*d,%d", &available) == 1) {
         LOG_MT("   available = " << available);
         continue;
      }

      if (strcmp(line.value(), "OK") == 0) {
         return available;
      }
      return ReturnCode::RC_FAILURE;
   }
}

//-------------------------------------------------------------------------------------------------

Return<void> MobileTerminal::enableSleepMode() {
   Bt::Core::Timer timer(DEFAULT_QUERY_CMD_TIMEOUT);
   sendCommand("AT+CSCLK=2");

   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);
      if (strcmp(line.value(), "OK") == 0) {
         return ReturnCode::RC_SUCCESS;
      }
      return ReturnCode::RC_FAILURE;
   }
}

//-------------------------------------------------------------------------------------------------

Return<void> MobileTerminal::disableSleepMode() {
   Bt::Core::Timer timer(DEFAULT_QUERY_CMD_TIMEOUT);
   sendCommand("AT");
   Core::delayInMilliseconds(100);
   sendCommand("AT+CSCLK=0");

   while(true){
      Return<const char*> line = readLine(timer);
      checkLine(line);
      if (strcmp(line.value(), "OK") == 0) {
         return ReturnCode::RC_SUCCESS;
      }
      return ReturnCode::RC_FAILURE;
   }
}

//-------------------------------------------------------------------------------------------------

void MobileTerminal::sendCommand(const char* pCommand) {
   flushInput();
   sendLine(pCommand);
   /*
   int retry = 0;
   while(retry < 2) {
      flushInput();
      sendLine(pCommand);
      Core::Timer timer(500);
      Return<const char*> line = readLine(timer);
      if(line.state() == ReturnCode::RC_SUCCESS) {
         if(strcmp(line.value(), pCommand) != 0) {
            LOG( " oops echo mismatch: '" << pCommand << "' [vs] '" <<  line.value() << "'");
         }
         return;
      }
      retry++;
      Core::delayInMilliseconds(20);
      mStream->println("");
      mStream->println("");
      mStream->println("");
      flushInput();
      Core::delayInMilliseconds(20);
      sendLine("AT");
      timer = 500;
      readLine(timer);
      timer = 500;
      readLine(timer);
      Core::delayInMilliseconds(20);
   }
   */
}

void MobileTerminal::sendLine(const char* pLine) {
   LOG_MT("  ---> " << pLine );
   mStream->println(pLine);
}

Return<const char*> MobileTerminal::readLine(Bt::Core::Timer& pTimer) {
   while(true) {
      Return<const char*> line = mLineReader.readLine(*mStream, pTimer);
      if(line.state() == ReturnCode::RC_TIMEOUT) {
         LOG_MT("  <-(TIMEOUT)- ");
         return line;
      }
      if(!isUrcResponse(line.value())) {
         LOG_MT("  <--- " << line.value());
         return line;
      }
      LOG_MT("  <-(ignore urc)-- " << line.value());
   }
}

Return<const char*> MobileTerminal::readPrompt(Bt::Core::Timer& pTimer) {
   Return<const char*> line = mLineReader.readPrompt(*mStream, pTimer);
   if( line.state() == ReturnCode::RC_TIMEOUT ) {
      LOG_MT("  <-(TIMEOUT)- ");
   } else {
      LOG_MT("  <--- " << line.value());
   }
   return line;
}

void MobileTerminal::flushInput() {
   uint16_t timeoutloop = 0;

   while (timeoutloop++ < 40) {
      while(mStream->available()) {
         char c = mStream->read();
         LOG_MT("  <-(F)- " << c);
         timeoutloop = 0;  // If char was received reset the timer
      }
      Bt::Core::delayInMilliseconds(1);
   }



   /*
   Core::Timer timer(10);
   Return<const char*> line = mLineReader.readLine(*mStream, timer);
   while(line.state() == ReturnCode::RC_SUCCESS) {
      LOG_MT("  <-(FLUSH)- " << line.value());
      line = mLineReader.readLine(*mStream, timer);
   }
   while(mStream->available()) {
      LOG_MT(" flush:" << (char)mStream->read());
   }
   */
}

//-------------------------------------------------------------------------------------------------

} // namespace Gprs
} // namespace Net
} // namespace Bt


