#include "include/tcpacceptor.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define BACKLOG 10

TCPAcceptor::TCPAcceptor(const char* address, int port)
   :m_listeningSocketDescriptor(0), m_address(address), m_port(port), m_listening(false){}


TCPAcceptor::~TCPAcceptor()
{
   if( m_listeningSocketDescriptor > 0)
   {
      close(m_listeningSocketDescriptor);
   }
}

int TCPAcceptor::start()
{
   if(m_listening == true)
   {
      return 0;
   }

   m_listeningSocketDescriptor = socket(PF_INET, SOCK_STREAM, 0);

   struct sockaddr_in address;
   memset(&address, 0, sizeof(address));
   address.sin_family = PF_INET;
   address.sin_port = htons(m_port);
   if(m_address.size() > 0)
   {
      inet_pton(PF_INET, m_address.c_str(), &(address.sin_addr));
   }
   else
   {
      address.sin_addr.s_addr = INADDR_ANY;
   }

   int optvalue = 1;
   setsockopt(m_listeningSocketDescriptor, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue));

   int result = bind(m_listeningSocketDescriptor, (struct sockaddr*)&address, sizeof(address));
   if(result != 0)
   {
      perror("bind() failed");
      return result;
   }
   result = listen(m_listeningSocketDescriptor, BACKLOG);
   if (result != 0)
   {
      perror("listen() failed");
      return result;
   }

   m_listening = true;
   return result;

}


TCPStream* TCPAcceptor::accept()
{
   if(m_listening == false)
   {
      return NULL;
   }

   struct sockaddr_in address;
   socklen_t length = sizeof(address);
   memset(&address, 0, sizeof(address));
   int sd = ::accept(m_listeningSocketDescriptor, (struct sockaddr*)&address, &length);
   if(sd < 0)
   {
      perror("accept() failed");
      return NULL;
   }
   return new TCPStream(sd, &address);
}
