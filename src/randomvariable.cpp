#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "include/randomvariable.h"



int RandomGenerate(int maxRange) {
	int variable;

	srand(time(NULL));
	variable = (int) (rand()%maxRange);

	return variable;

}


float ExpRandomGenerate(float coeficient)
{
	float variable, exponential;

	srand(time(NULL));
	variable = (float) ((rand()+1)%1000)/1000.0;

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
