#include <stdio.h>
#include "global.h"
#include "config.h"
#include <string.h>

char chartPath[100];
char layerPath[100];
char layerPath2[100];
char port_ChartServer[100];
char port_KKSstate[100];
unsigned int bOnlyPing;


int loadConfig(void){
	char id[100], path[100];
	FILE *fp=NULL;
	
	strcpy(chartPath, "");
	strcpy(layerPath, "");
	strcpy(layerPath2, "");
	strcpy(port_ChartServer, "");
	strcpy(port_KKSstate, "");
	bOnlyPing = 0;
	
	fp = fopen(chartSrvCfgPath, "r");
	if(fp==NULL)
		return 0;
	while(!feof(fp)){
		fscanf(fp, "%s %s\n", id, path);
		if(!strcmp(id,"ChartPath"))
			strcpy(chartPath, path);
		else if(!strcmp(id, "LayerPath"))
			strcpy(layerPath, path);
		else if(!strcmp(id, "LayerPath2"))
			strcpy(layerPath2, path);
		else if(!strcmp(id,"ChartServer_port"))
			strcpy(port_ChartServer, path);	
		else if(!strcmp(id,"kksState_port"))
			strcpy(port_KKSstate, path);	
		else if(!strcmp(id,"bOnlyPing"))
			bOnlyPing = strtoul( path, NULL, 10 );	
	}
	
	if(!strcmp(layerPath, ""))
		return -1;
	if(!strcmp(layerPath2, ""))
		return -1;
	if(!strcmp(port_ChartServer, ""))
		return -1;
	if(!strcmp(chartPath, ""))
		return -1;
	if(!strcmp(port_KKSstate, ""))
		return -1;
	fclose(fp);
	return 1;
}

void outConfig(void)
{
	printf("\nVivod portov\n");
	printf("------------------------------------------------------\n");
	printf("port_ChartServer	-	%s\n", port_ChartServer);
	printf("chartPath		-	%s\n", chartPath);
	printf("layerPath	- 	%s\n", layerPath);
	printf("layerPath2	-	%s\n", layerPath2);
	printf("------------------------------------------------------\n\n");
}