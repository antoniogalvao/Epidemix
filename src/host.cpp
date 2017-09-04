#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "include/thread.h"
#include "include/workqueue.h"
#include "include/tcpconnector.h"
#include "include/tcpacceptor.h"
#include "include/randomvariable.h"

#define FAIL -1

#define GLOBAL_SERVER_IP "localhost"
#define GLOBAL_SERVER_PORT 2000

#define MESSAGE_LENGTH 256


static enum {susceptible, infected} hostStatus;



class InfectNear : public Thread
{
	int m_number;
public:
	InfectNear(int number): m_number(number) {}

	void* run()
   {
		//TCPConnector* connector = new TCPConnector;

      /*for(int i = 0;; i++)
      {
      	//printf("InfectNear - thread %lu, number %d, loop %d - waiting for item...\n",
         //   (long unsigned)self(),m_number, i);
			usleep(100000*ExpRandomGenerate());
			if(hostStatus == infected) {
				TCPStream* stream = connector->connect("localhost", 2000);
				if(stream) {
					printf("InfectNear - connection established\n");
				}
				else{
					printf("InfectNear - error connon eection\n");
				}
				delete stream;
			}

      }*/

      return NULL;
   }
};


class SelfHeal : public Thread
{
	int m_number;
public:
	SelfHeal(int number): m_number(number) {}

   void* run()
   {
      for(int i = 0;; i++)
      {
      	//printf("SelfHeal - thread %lu, number %d, loop %d - waiting for item...\n",
            //(long unsigned)self(),m_number, i);
			if(hostStatus == infected){
				printf("%ld - selfheal - infected\n", time(NULL));
				usleep(1000000*ExpRandomGenerate());
				printf("%ld - selfheal - susceptible\n", time(NULL));
				hostStatus = susceptible;
			}
      }
      return NULL;
   }
};

class ExogenousInfect : public Thread
{
	int m_number;
public:
	ExogenousInfect(int number): m_number(number) {}

   void* run()
   {
      for(int i = 0;; i++)
      {
      	//p'rintf("ExogenousInfect - thread %lu, number %d, loop %d - waiting for item...\n",
         //   (long unsigned)self(),m_number, i);
			/*if(hostStatus == susceptible) {
				printf("%ld - exogenous - susceptible\n", time(NULL));
				hostStatus = infected;
				printf("%ld - exogenous - infected\n", time(NULL));
				usleep(1000000*ExpRandomGenerate());
      	}*/
		}
      return NULL;
   }
};

class EndogenousInfect : public Thread
{
	int m_number;
	int m_hostPort;
public:
	EndogenousInfect(int number, int hostPort)
		:m_number(number), m_hostPort(hostPort) {}

   void* run()
   {
		TCPAcceptor* acceptor = new TCPAcceptor("localhost", m_hostPort);
		acceptor->start();
		cout << "EndogenousInfect - Acceptor created" << endl;
      for(int i = 0;; i++)
      {
      	//printf("EndogenousInfect - thread %lu, number %d, loop %d - waiting for item...\n",
         //   (long unsigned)self(),m_number, i);
			//usleep(10000000);
			if(hostStatus == susceptible) {
				TCPStream* stream = acceptor->accept();
				if(stream) {
					printf("EndogenousInfect - connection established\n");
					cout << "EndogenousInfect - infected" << endl;
					hostStatus = infected;
				}
				else{
					printf("EndogenousInfect - error connection\n");
				}
				delete stream;
			}
		}
      return NULL;
	}
};


int main(int argc, char** argv)
{
   if(argc != 3)
   {
      printf("Usage: %s <ip> <port>\n", argv[0]);
      exit(1);
   }

	int hostPort = atoi(argv[2]);

	hostStatus = infected;

   // Create the threads
	InfectNear* infectNear = new InfectNear(1);
	if(!infectNear) {
		printf("Could not create InfectNear");
		exit(1);
	}
	infectNear->start();

	SelfHeal* selfHeal = new SelfHeal(2);
	if(!selfHeal) {
		printf("Could not create SelfHeal");
		exit(1);
	}
	selfHeal->start();

	ExogenousInfect* exogenousInfect = new ExogenousInfect(3);
	if(!exogenousInfect) {
		printf("Could not create ExogenousInfect");
		exit(1);
	}
	exogenousInfect->start();

	EndogenousInfect* endogenousInfect = new EndogenousInfect(4, hostPort);
	if(!endogenousInfect) {
		printf("Could not create EndogenousInfect");
		exit(1);
	}
	endogenousInfect->start();


	//Connecting to the Global Server
	TCPConnector* connector = new TCPConnector;
	TCPStream* globalServerStream = connector->connect(GLOBAL_SERVER_IP, GLOBAL_SERVER_PORT);
	if(globalServerStream){
		cout << "Connected with Global Server\n";

		char requestMessage[MESSAGE_LENGTH];
		char responseMessage[MESSAGE_LENGTH];
		ssize_t responseMessageLength;

		snprintf(requestMessage,MESSAGE_LENGTH,"%d",hostPort);
		globalServerStream->sendMessage(requestMessage, MESSAGE_LENGTH);


		globalServerStream->sendMessage("REQUEST HOST", MESSAGE_LENGTH);
		cout << "sent - " << requestMessage << endl;
		responseMessageLength = globalServerStream->receiveMessage(responseMessage, MESSAGE_LENGTH);
		responseMessage[responseMessageLength] = '\0';
		cout << "received - " << responseMessage << endl;

	}
	else{
		perror("Error connecting to Global Server");
	}



	// infinite loop program
	while(true){}



	// Create a TCPAcceptor

  /*TCPAcceptor* acceptor = NULL;
   acceptor = new TCPAcceptor(argv[1], atoi(argv[2]));
   if(!acceptor || acceptor->start() > 0){
      printf("Could not create an connection acceptor\n");
      exit(1);
   }


	while(1)
   {
      TCPStream* connection = acceptor->accept();
      std::cout << "Connection: " << connection->getPeerIP() << ":" << connection->getPeerPort() << std::endl;

      if(!connection){
         printf("Could not accept a connection\n");
         continue;
      }
   }*/

   perror("Could not start the server\n");

	delete globalServerStream;
   exit(-1);

}
