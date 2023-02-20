#include "ltool.h"

char ltCheckSumB(char *data, int size){
	char sum = 0;
	int i = 0;
	for(i = 0; i<size; i++)
		sum += data[i];
	return (sum);
}

char ltCheckSumBSh(char *data, int size){
	unsigned short sum = 0;
	int i = 0;
	for(i = 0; i<size; i++){
		sum += data[i];
		if(sum >> 0x10)
			sum = (sum&0xFFFF) + 1;
	}		
	return ((char)sum);
}

unsigned short ltCheckSumW(unsigned short *data, int size){
	unsigned short sum = 0;
	int i = 0;
	for(i = 0; i<size/2; i++)
		sum += data[i];
	return (sum);
}

unsigned short ltCheckSumWSh(unsigned short *data, int size){
	unsigned long sum = 0;
	int i = 0;
	for(i = 0; i<size/2; i++){
		sum += data[i];
		if(sum >> 0x10)
			sum = (sum&0xFFFF) + 1;
	}		
	return ((unsigned short)sum);
}

