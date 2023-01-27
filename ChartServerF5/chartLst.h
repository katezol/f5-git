#ifndef CHARTLST_H_
#define CHARTLST_H_

#include <stdio.h>

struct chartLst{
	char date[15];
	char name[15];
	float latMax;
	float latMin;
	float lonMax;
	float lonMin;
	float scale;
};

FILE* openChartLst(const char* path);
int readChartLst(FILE* fp, struct chartLst* tmpChartLst);

struct listChartLst{
	int count;
	struct chartLst chartLst[200];
};

void initListChartLst(struct listChartLst* p);
void addChartLst(const struct chartLst* chartLst, struct listChartLst* p);


#endif /*CHARTLST_H_*/
