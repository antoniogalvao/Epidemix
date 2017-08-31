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


static enum {susceptible, infected} hostStatus;



class InfectNear : public Thread
{
	int m_number;
public:
	InfectNear(int number): m_number(number) {}

	void* run()
   {
		//TCPConnector* connector = new TCPConnector;

      for(int i = 0;; i++)
      {
      	//printf("InfectNear - thread %lu, number %d, loop %d - waiting for item...\n",
         //   (long unsigned)self(),m_number, i);
			/*usleep(100000*ExpRandomGenerate());
			if(hostStatus == infected) {
				TCPStream* stream = connector->connect("localhost", 2000);
				if(stream) {
					printf("InfectNear - connection established\n");
				}
				else{
					printf("InfectNear - error connection\n");
				}
				delete stream;
			}*/

      }
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
				//printf("%ld - selfheal - susceptible\n", time(NULL));
				//hostStatus = susceptible;
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
      	//printf("ExogenousInfect - thread %lu, number %d, loop %d - waiting for item...\n",
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
public:
	EndogenousInfect(int number): m_number(number) {}

   void* run()
   {
		/*TCPAcceptor* acceptor = new TCPAcceptor("localhost", 3000);
		acceptor->start();
      for(int i = 0;; i++)
      {
      	//printf("EndogenousInfect - thread %lu, number %d, loop %d - waiting for item...\n",
         //   (long unsigned)self(),m_number, i);
			//usleep(10000000);
			if(hostStatus == susceptible) {
				TCPStream* stream = acceptor->accept();
				if(stream) {
					printf("EndogenousInfect - connection established\n");
				}
				else{
					printf("EndogenousInfect - error connection\n");
				}
				delete stream;
			}
		}*/
      return NULL;
	}
};


int main(int argc, char** argv)
{
   /*if(argc != 3)
   {
      printf("Usage: %s <ip> <port>\n", argv[0]);
      exit(1);
   }*/

	hostStatus = infected;

   //create the threads
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

	EndogenousInfect* endogenousInfect = new EndogenousInfect(4);
	if(!endogenousInfect) {
		printf("Could not create EndogenousInfect");
		exit(1);
	}
	endogenousInfect->start();

	TCPConnector* connector = new TCPConnector;
	connector->connect("localhost", 2000);


	while(true){
	}

  /* TCPAcceptor* acceptor = NULL;
   acceptor = new TCPAcceptor(argv[1], atoi(argv[2]));
   if(!acceptor || acceptor->start() > 0){
      printf("Could not create an connection acceptor\n");
      exit(1);
   }*/

   //cout << "Server online" << endl;


   /*while(1)
   {
      TCPStream* connection = acceptor->accept();
      std::cout << "Connection: " << connection->getPeerIP() << ":" << connection->getPeerPort() << std::endl;

      if(!connection){
         printf("Could not accept a connection\n");
         continue;
      }
   }*/

   //perror("Could not start the server\n");
   //exit(-1);
}
