#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "include/randomvariable.h"

float ExpRandomGenerate()
{
	float variable, exponential;
	int precision = 1000;
	float coeficient = 1.1;
	
	srand(time(NULL));
	variable = (float) (rand()%precision+1)/precision;

	exponential = -log(variable)/coeficient;	

	return exponential;
}

float UniformRandomGenerate()
{
	float variable;
	int precision = 1000;
	
	srand(time(NULL));
	variable = (float) (rand()%precision+1)/precision;
	
	return variable;
}
