#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "include/thread.h"
#include "include/workqueue.h"
#include "include/tcpacceptor.h"


#define FAIL -1


static enum {susceptible, infected} hostStatus;



class InfectNear : public Thread
{
	int m_number;
public:
	InfectNear(int number): m_number(number) {}

	void* run()
   {
      for(int i = 0;; i++)
      {
      	printf("InfectNear - thread %lu, number %d, loop %d - waiting for item...\n",
            (long unsigned)self(),m_number, i);
			usleep(10000000);
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
				printf("infected\n");
				usleep(5000000);
				printf("susceptible\n");
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
      	printf("ExogenousInfect - thread %lu, number %d, loop %d - waiting for item...\n",
            (long unsigned)self(),m_number, i);
			usleep(10000000);
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
      for(int i = 0;; i++)
      {
      	printf("EndogenousInfect - thread %lu, number %d, loop %d - waiting for item...\n",
            (long unsigned)self(),m_number, i);
			usleep(10000000);
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



   TCPAcceptor* acceptor = NULL;
   acceptor = new TCPAcceptor(argv[1], atoi(argv[2]));
   if(!acceptor || acceptor->start() > 0){
      printf("Could not create an connection acceptor\n");
      exit(1);
   }

   //cout << "Server online" << endl;


   while(1)
   {
      TCPStream* connection = acceptor->accept();
      std::cout << "Connection: " << connection->getPeerIP() << ":" << connection->getPeerPort() << std::endl;

      if(!connection){
         printf("Could not accept a connection\n");
         continue;
      }
   }

   perror("Could not start the server\n");
   exit(-1);
}
