#ifndef TCPCONNECTOR_H
#define TCPCONNECTOR_H

#include <netinet/in.h>
#include "tcpstream.h"

class TCPConnector
{
   public:
      TCPStream* connect( const char* server, int port);
   private:
      int resolveHostName(const char* hostname, struct in_addr* addr);
};


#endif
