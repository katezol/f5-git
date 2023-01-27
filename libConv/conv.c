#include "lconv.h"

unsigned long use2data(float value, float step, int sign){
	unsigned long ret = 0;
	float s = 1.0;
	if(( value < step)&&( value >= -step))
		return(0);
	if( value < 0.0)
		s = -1.0;
	ret = (unsigned long)(value/step + s*0.5);
	if(sign){
		ret &= ((1<<sign)-1);
		if(value < 0.0)
			ret |= 1<<sign;
	}		
	return(ret);	
}

unsigned long use2dataDouble(double value, double step, int sign){
	unsigned long ret = 0;
	double s = 1.0;
	if(( value < step)&&( value >= -step))
		return(0);
	if( value < 0.0)
		s = -1.0;
	ret = (unsigned long)(value/step + s*0.5);
	if(sign){
		ret &= ((1<<sign)-1);
		if(value < 0.0)
			ret |= 1<<sign;
	}		
	return(ret);	
}

float data2use(unsigned long data, float step, int sign){
	if( (data>>sign) && sign ){
		int n = 8*sizeof(data) - (sign + 1);
		data |=  ( ( (unsigned long)1 << n ) -1) << sign;
		data |=  (unsigned long)1 << (8*sizeof(data)-1);
		return( (signed long)data*step );
	}	
	return(data * step);
}

float data2useDouble(unsigned long data, double step, int sign){
	if( (data>>sign) && sign ){
		int n = 8*sizeof(data) - (sign + 1);
		data |=  ( ( (unsigned long)1 << n ) -1) << sign;
		data |=  (unsigned long)1 << (8*sizeof(data)-1);
		return( (signed long)data*step );
	}	
	return(data * step);
}
