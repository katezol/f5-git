#include <stdlib.h>
#include "ltool.h"

int ltRandi(int min, int max){
	float r = min + (float)rand()*( (float)max - (float)min) / (float)RAND_MAX ;
	if(r < 0.0)
		return( r - 0.5 );
	return( r + 0.5 );	
}

float ltRandf(float min,float max){
	return( min + (float)rand()*(max-min) / (float)RAND_MAX  );
}