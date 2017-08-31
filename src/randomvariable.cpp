#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "include/randomvariable.h"

float generateRandomVariable()
{
	float variable, exponential;
	int precision = 1000;
	float coeficient = 1.1;
	
	srand(time(NULL));
	variable = (float) (rand()%precision+1)/precision;

	exponential = -log(variable)/coeficient;	

	return exponential;
}
