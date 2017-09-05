#include "include/globalfunctions.h"
#include "include/randomvariable.h"

#include <stdlib.h>
#include <stdio.h>

using namespace std;

int searchHost(const vector<hostData> & hostVector, char* peerPortString) {
   printf("comecando busca\n");
   int index = RandomGenerate(hostVector.size());
   /*while(hostVector[index].peerPortString == peerPortString) {
      index = RandomGenerate(hostVector.size());
   }
   */
   int peerPort = atoi(peerPortString); //wrong the correct is hostVector[index].peerPortString.c_str()
   //need to search for the error in the conversion
   return peerPort;
}
