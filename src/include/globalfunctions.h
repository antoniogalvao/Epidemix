#ifndef GLOBAL_FUNCTIONS_H
#define GLOBAL_FUNCTIONS_H

#include <vector>
#include <string>

#define FILENAME "example.txt"


typedef struct {
   std::string peerPortString;
   std::string hostPortString;
} hostData;



int searchHost(const std::vector<hostData> & hostVector, char* peerPortString);


#endif
