#include <iostream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "include/globalfunctions.h"
#include "include/thread.h"
#include "include/workqueue.h"
#include "include/tcpacceptor.h"


#define FAIL -1


vector<hostData> hostVector;
ofstream hostFile;
ofstream resultFile;
int connectionNumber = 0;
int infectedNodes = 0;


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
      for(long unsigned i = 0;; i++)
      {

         ssize_t length;
         char hostRequestMessage[256];
         char serverResponseMessage[256];

         printf("thread %lu, loop %ld - waiting for item...\n",
            (long unsigned)self(), i);
         WorkItem* item = m_queue.remove();
         printf("thread %lu, loop %ld - got one item\n",
            (long unsigned)self(), i);

         //establishes connection with a host
         TCPStream* stream = item->getStream();
			char peerPortString[10];
			sprintf(peerPortString, "%d", stream->getPeerPort());
         length = stream->receiveMessage(hostRequestMessage, sizeof(hostRequestMessage));
         char hostPortString[10];
         strcpy(hostPortString, hostRequestMessage);
         cout << "host port - " << hostPortString << endl;

         //save host information
         hostVector.push_back(hostData());
         hostVector[connectionNumber].peerPortString = peerPortString;
         hostVector[connectionNumber].hostPortString = hostPortString;

         hostFile.open(NODES_FILENAME, std::ios_base::app);
			hostFile << connectionNumber << " " << peerPortString << " " << hostPortString << "\n";
			hostFile.close();


			printf("%d - %s %s\n", connectionNumber, peerPortString, hostPortString);

         connectionNumber++;
         //loop waiting for messages from host
         while ((length = stream->receiveMessage(hostRequestMessage, sizeof(hostRequestMessage))) > 0)
         {
            hostRequestMessage[length] = '\0';
        //    char peerPortString[10];
        //    sprintf(peerPortString, "%d",stream->getPeerPort());
            cout <<  "New message : " << peerPortString << " - " << hostRequestMessage << endl;

            //printf("%s\n", hostRequestMessage);

            if( (strcmp(hostRequestMessage, "REQUEST HOST")) == 0 && connectionNumber > 1) {
               printf("procurar host\n");
               //search randomic for a host that is not the actual peer
               int peerPort = searchHost(hostVector, peerPortString);
               cout << "PORTA ALEATORIA ENCONTRADA: " << peerPort << endl;
               memset(serverResponseMessage, 0, sizeof(serverResponseMessage));
               snprintf(serverResponseMessage, length, "%d", peerPort);
               cout << "TESTE " << serverResponseMessage << endl;
               stream->sendMessage(serverResponseMessage, length);
            }

            if( (strcmp(hostRequestMessage, "INFECTED"))  == 0 ){
               infectedNodes++;
               cout << "TOTAL INFECTED - "  << infectedNodes << endl;
               //saving some results
               resultFile.open(RESULT_FILENAME, std::ios_base::app);
               resultFile << time(NULL) << " " << infectedNodes << "\n";
               resultFile.close();

            }

            if( (strcmp(hostRequestMessage, "SUSCEPTIBLE")) == 0) {
               infectedNodes--;
               cout << "TOTAL INFECTED - "  << infectedNodes << endl;
               //saving some results
               resultFile.open(RESULT_FILENAME, std::ios_base::app);
      			resultFile << time(NULL) << " " << infectedNodes << "\n";
      			resultFile.close();

            }

            //strcat(serverResponseMessage, peerPortString);
            //strcat(serverResponseMessage, " - ");
            //strcat(serverResponseMessage, hostRequestMessage);
            //stream->sendMessage(serverResponseMessage, length);
         }

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
   }

   perror("Could not start the server\n");
   exit(-1);
}
