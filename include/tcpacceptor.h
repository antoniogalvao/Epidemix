#ifndef TCPACCEPTOR_H
#define TCPACCEPTOR_H

#include <string.h>
#include <netinet/in.h>
#include "tcpstream.h"

class TCPAcceptor
{
   int m_listeningSocketDescriptor;
   string m_address;
   int m_port;
   bool m_listening;

   public:
      TCPAcceptor(const char* address, int port);
      ~TCPAcceptor();
      int start();
      TCPStream* accept();
   private:
      TCPAcceptor() {}

};

#endif
