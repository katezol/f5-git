#include "controlSum.h"

//returns 1, if controlSum equals calculated value
bool checkSumXOR(unsigned char *array, int length, unsigned char controlSum)
{
	if(countSumXOR(array, length) == controlSum)	return 1;
	return 0;
}

//Считает сумму XOR байтов массива
unsigned char countSumXOR(unsigned char * array, int length)
{
	int counter;
	unsigned char sum = 0x00;
	for(counter=0; counter < length; ++counter)
	{
		sum ^= array[counter];
	}
	return sum;
}