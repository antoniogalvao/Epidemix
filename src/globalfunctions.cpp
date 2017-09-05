#include "include/globalfunctions.h"
#include "include/randomvariable.h"


//iostream is used only for debugging
#include <iostream>
#include <stdlib.h>

using namespace std;


//the good practice is call this function only with the system have more than 1 host
int searchHost(const vector<hostData> & hostVector, char* peerPortString) {
   cout << "comecando busca" << endl;
   int index = RandomGenerate(hostVector.size());
   cout << index << endl;
   //this loop doesnt check if the system have only one node
   //can runs indefinitely
   /*while(hostVector[index].peerPortString == peerPortString) {
      index = RandomGenerate(hostVector.size());
   }*/

   int hostPort = atoi(hostVector[index].hostPortString.c_str());


   return hostPort;
}
