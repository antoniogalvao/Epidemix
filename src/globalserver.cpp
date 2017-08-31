#include <iostream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "include/thread.h"
#include "include/workqueue.h"
#include "include/tcpacceptor.h"


#define FAIL -1

ofstream myfile;


class WorkItem
{
   TCPStream* m_stream;
public:
   WorkItem(TCPStream* stream): m_stream(stream) {}
   ~WorkItem() {delete m_stream;}

   TCPStream* getStream() {return m_stream;}
};

class ConnectionHandler : public Thread
{
   WorkQueue<WorkItem*>& m_queue;
public:
   ConnectionHandler(WorkQueue<WorkItem*>& queue) : m_queue(queue) {}

   void* run()
   {
      for(int i = 0;; i++)
      {
         printf("thread %lu, loop %d - waiting for item...\n",
            (long unsigned)self(), i);
         WorkItem* item = m_queue.remove();
         printf("thread %lu, loop %d - got one item\n",
            (long unsigned)self(), i);


         TCPStream* stream = item->getStream();
			char peerPortString[10];
			sprintf(peerPortString, "%d", stream->getPeerPort());
			myfile.open("example.txt", std::ios_base::app);
			myfile << "1 - " << peerPortString << "\n";
			myfile.close();
			printf("1 - %s", peerPortString);

         //ssize_t length;
         //char line[256];

         /*while ((length = stream->receiveMessage(line, sizeof(line))) > 0)
         {
            line[length] = 0;
        //    char peerPortString[10];
        //    sprintf(peerPortString, "%d",stream->getPeerPort());
            printf("%s - %s\n", peerPortString, line);
            for(unsigned i = 0; i < sizeof(line); i++)
            {
               line[i] = toupper(line[i]);
            }
            char serverResponseMessage[512];
            strcat(serverResponseMessage, peerPortString);
            strcat(serverResponseMessage, " - ");
            strcat(serverResponseMessage, line);
            stream->sendMessage(serverResponseMessage, length);

         }*/

         delete stream;
      }
      return NULL;
   }
};



int main(int argc, char** argv)
{
   if(argc != 4)
   {
      printf("Usage: %s <ip> <port> <number-workers>\n", argv[0]);
      exit(1);
   }

   //create the queue and consumer(worker) threads
   int workers = atoi(argv[3]);
   WorkQueue<WorkItem*> queue;
   for (int i = 0; i < workers; i++)
   {
      ConnectionHandler* handler = new ConnectionHandler(queue);
      if(!handler) {
         printf("Could not create ConnectionHandler %d\n", i);
         exit(1);
      }
      handler->start();
   }

   //Create an acceptor then start listening for connections
   WorkItem* item;
   //TCPStream* stream = NULL;
   TCPAcceptor* acceptor = NULL;
   acceptor = new TCPAcceptor(argv[1], atoi(argv[2]));
   if(!acceptor || acceptor->start() > 0){
      printf("Could not create an connection acceptor\n");
      exit(1);
   }

   cout << "Server online" << endl;


   while(1)
   {
      TCPStream* connection = acceptor->accept();
      std::cout << "Connection: " << connection->getPeerIP() << ":" << connection->getPeerPort() << std::endl;

      if(!connection){
         printf("Could not accept a connection\n");
         continue;
      }
      item = new WorkItem(connection);
      if(!item)
      {
         printf("Could not create work item \n");
         continue;
      }
      queue.add(item);
      //echo server message for all clients
   }

   perror("Could not start the server\n");
   exit(-1);
}
