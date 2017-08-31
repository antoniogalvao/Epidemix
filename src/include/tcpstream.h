#ifndef TCPSTREAM_H
#define TCPSTREAM_H

#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

class TCPStream
{
   int m_peerSocketDescriptor;
   string m_peerIP;
   int m_peerPort;

   public:
      friend class TCPAcceptor;
      friend class TCPConnector;

      ~TCPStream();

      ssize_t sendMessage(const char* buffer, size_t lenght);
      ssize_t receiveMessage(char* buffer, size_t lenght);

      string getPeerIP();
      int getPeerPort();
      int getPeerSocketDescriptor();
   private:
      TCPStream(int sd, struct sockaddr_in* address);
      TCPStream();
      TCPStream(const TCPStream& stream);

};


#endif
