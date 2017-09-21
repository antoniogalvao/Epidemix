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

#define MI_COEFICIENT 1
#define LAMBDA_COEFICIENT 1
#define GAMA_COEFICIENT 1.1
#define EXP_COEFICIENT 1.1

#define GLOBAL_SERVER_IP "localhost"
#define GLOBAL_SERVER_PORT 2000
#define LOCALHOST "localhost"

#define TIME_COEFICIENT 10000000


#define MESSAGE_LENGTH 256


static enum {susceptible, infected} hostStatus;



class InfectNear : public Thread
{
	int m_number;
	TCPStream* m_globalServerStream;

public:
	InfectNear(int number, TCPStream* globalServerStream):
		m_number(number), m_globalServerStream(globalServerStream) {}

	void* run()
   {
		TCPConnector* connector = new TCPConnector;
		char responseMessage[MESSAGE_LENGTH];
		ssize_t responseMessageLength;

      for(;;)
      {
			usleep(TIME_COEFICIENT*ExpRandomGenerate(GAMA_COEFICIENT));
			if(hostStatus == infected) {
				//search for a host to connect
				m_globalServerStream->sendMessage("REQUEST HOST", MESSAGE_LENGTH);
				responseMessageLength = m_globalServerStream->receiveMessage(responseMessage, MESSAGE_LENGTH);
				responseMessage[responseMessageLength] = '\0';
				cout << "received - " << responseMessage << endl;

				int foundedHostPort = atoi(responseMessage);

				//connecting to the founded host
				TCPStream* stream = connector->connect(LOCALHOST, foundedHostPort);
				if(stream) {
					cout << "InfectNear - connection established" << endl;
				}
				else{
					cout << "InfectNear - error conection" << endl;
				}
				delete stream;
			}
      }
      return NULL;
   }
};


class SelfHeal : public Thread
{
	int m_number;
	TCPStream* m_globalServerStream;
public:
	SelfHeal(int number, TCPStream* globalServerStream):
		m_number(number), m_globalServerStream(globalServerStream) {}

   void* run()
   {
      for(;;)
      {
			if(hostStatus == infected){
				usleep(TIME_COEFICIENT*ExpRandomGenerate(MI_COEFICIENT));
				hostStatus = susceptible;
				m_globalServerStream->sendMessage("SELF HEAL", MESSAGE_LENGTH);
				m_globalServerStream->sendMessage("SUSCEPTIBLE", MESSAGE_LENGTH);
				cout << time(NULL) << " - selfheal - susceptible" << endl;
			}
      }
      return NULL;
   }
};

class ExogenousInfect : public Thread
{
	int m_number;
	TCPStream* m_globalServerStream;
public:
	ExogenousInfect (int number, TCPStream* globalServerStream):
		m_number(number), m_globalServerStream(globalServerStream)  {}

   void* run()
   {
      for(;;)
      {/*
			usleep(TIME_COEFICIENT*10*ExpRandomGenerate(LAMBDA_COEFICIENT));
			if(hostStatus == susceptible) {
				hostStatus = infected;
				m_globalServerStream->sendMessage("EXOGENOUS", MESSAGE_LENGTH);
				m_globalServerStream ->sendMessage("INFECTED", MESSAGE_LENGTH);
				cout << time(NULL) << " - exogenous - infected" << endl;
      	}
		*/
		}
      return NULL;
   }
};


class EndogenousInfect : public Thread
{
	int m_number;
	int m_hostPort;
	TCPStream* m_globalServerStream;
public:
	EndogenousInfect(int number, int hostPort,TCPStream* globalServerStream):
		m_number(number), m_hostPort(hostPort), m_globalServerStream(globalServerStream) {}

   void* run()
   {
		TCPAcceptor* acceptor = new TCPAcceptor(LOCALHOST, m_hostPort);
		acceptor->start();
		cout << "EndogenousInfect - Acceptor created" << endl;
      for(;;)
      {
			if(hostStatus == susceptible) {
				TCPStream* stream = acceptor->accept();
				if(stream) {
					printf("EndogenousInfect - connection established\n");
					hostStatus = infected;	
					m_globalServerStream->sendMessage("ENDOGENOUS", MESSAGE_LENGTH);
					m_globalServerStream->sendMessage("INFECTED", MESSAGE_LENGTH);
					cout << time(NULL) <<  " - endogenous - infected" << endl;
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

	//setting the host default status
	hostStatus = infected;

	//Connecting to the Global Server
	TCPConnector* connector = new TCPConnector;
	TCPStream* globalServerStream = connector->connect(GLOBAL_SERVER_IP, GLOBAL_SERVER_PORT);
	if(globalServerStream){
		cout << "Connected with Global Server\n";
		char requestMessage[MESSAGE_LENGTH];
		//char responseMessage[MESSAGE_LENGTH];
		//ssize_t responseMessageLength;
		snprintf(requestMessage,MESSAGE_LENGTH,"%d",hostPort);
		globalServerStream->sendMessage(requestMessage, MESSAGE_LENGTH);
	}
	else{
		perror("Error connecting to Global Server");
		exit(1);
	}




   // Create the threads
	InfectNear* infectNear = new InfectNear(1, globalServerStream);
	if(!infectNear) {
		printf("Could not create InfectNear");
		exit(1);
	}


	SelfHeal* selfHeal = new SelfHeal(2, globalServerStream);
	if(!selfHeal) {
		printf("Could not create SelfHeal");
		exit(1);
	}


	ExogenousInfect* exogenousInfect = new ExogenousInfect(3, globalServerStream);
	if(!exogenousInfect) {
		printf("Could not create ExogenousInfect");
		exit(1);
	}


	EndogenousInfect* endogenousInfect = new EndogenousInfect(4, hostPort, globalServerStream);
	if(!endogenousInfect) {
		printf("Could not create EndogenousInfect");
		exit(1);
	}


	//starting the threads
	infectNear->start();
	selfHeal->start();
	exogenousInfect->start();
	endogenousInfect->start();


	// infinite loop program
	while(true){}



   perror("Could not start the server\n");

	delete globalServerStream;
   exit(-1);
}
