#include "chartLst.h"
#include <stdio.h>
#include <stdlib.h>

struct listChartLst listChartLst;
struct listChartLst listChartLst2;

FILE* openChartLst(const char* path){
	char tmp[100];
	FILE* fp=NULL;
	fp = fopen(path, "rb");
	if(fp==NULL)
	{
		perror(path);
		//perror(" opening failed\n");
		return fp;
	}
	fread(tmp, 1, 12, fp);
	return fp;
}

int readChartLst(FILE* fp, struct chartLst* tmpChartLst){
	float Scale;
	int szRead;
	
	//memset(tmpChartLst->name,0,strlen(tmpChartLst->name));
	//strcpy(&tmpChartLst->name,"");
	szRead = fread(tmpChartLst->name, 1, 9, fp);
	if(szRead==0) return 0;
	
	szRead = fread(tmpChartLst->name, 1, 9, fp);
	if(szRead==0) return 0;
//	memset(tmpChartLst->date,0,strlen(tmpChartLst->date));
//	tmpChartLst->date="";
	szRead = fread(tmpChartLst->date, 1, 11, fp);
	if(szRead==0) return 0;
	
	szRead = fread(&(tmpChartLst->latMin), sizeof(tmpChartLst->latMin), 1, fp);
	if(szRead==0) return 0;
	szRead = fread(&(tmpChartLst->latMax), sizeof(tmpChartLst->latMax), 1, fp);
	if(szRead==0) return 0;
	szRead = fread(&(tmpChartLst->lonMin), sizeof(tmpChartLst->lonMin), 1, fp);
	if(szRead==0) return 0;
	szRead = fread(&(tmpChartLst->lonMax), sizeof(tmpChartLst->lonMax), 1, fp);
	if(szRead==0) return 0;
	szRead = fread(&(tmpChartLst->scale), 4, 1, fp);
	if(szRead==0) return 0;
	
	return 1;
}

void initListChartLst(struct listChartLst *p){
	p->count = 0;
}

void addChartLst(const struct chartLst* chartLst, struct listChartLst* p){
	int i;
	if(p->count==199)					
		return;
	for(i=0; i<p->count; i++)
		if(!strcmp(p->chartLst[i].name, chartLst->name))
			return;
	memcpy(p->chartLst+p->count, chartLst, sizeof(struct chartLst));
	p->count++;
}
