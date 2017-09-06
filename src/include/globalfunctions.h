#ifndef GLOBAL_FUNCTIONS_H
#define GLOBAL_FUNCTIONS_H

#include <vector>
#include <string>

#define NODES_FILENAME "nodes.txt"
#define RESULT_FILENAME "result.txt"

typedef struct {
   std::string peerPortString;
   std::string hostPortString;
} hostData;



int searchHost(const std::vector<hostData> & hostVector, char* peerPortString);


#endif
