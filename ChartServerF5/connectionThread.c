#include <stdio.h>
#include "connectionThread.h"
#include "global.h"
#include <stdlib.h>

struct connection connection;
void* connectionTest(void* data);

void fgetStringLine(FILE* fp, char* str){
	unsigned char tmp = 0;
	int count= 0;
	while(tmp!='\n'&&!feof(fp)){
		tmp = fgetc(fp);
		if(tmp!='\n'){
			str[count] = tmp;
			count++;
		}
	}
	str[count]='\0';
}

int read_ConnectionCfg(void)
{
	char buff[100];
	FILE *fp;

	fp = fopen(chartConnectionCfgPath, "rb");
	if(!fp)
		return 0;
	fgetStringLine(fp, ipKKSosn);
	fgetStringLine(fp, ipKKSres);
	fgetStringLine(fp, connection.mnt[0]);	// Подключение для основного канала
	fgetStringLine(fp, connection.mnt[1]);	// Подключение для резервного канала

	fclose(fp);
	return 1;
}

int _readFSCIFS;

void* connectionTest(void* data){
	while(1){
		delay(100);
		if(_readFSCIFS==1){
			_readFSCIFS = 2;
			delay(1000);
		}else if(_readFSCIFS==2){			//соединение fs-cifs не работает

			killFSCIFS;
		}
	}
}

void* connectionThread(void* data){
	FILE *fp;
	char path[256];
	_readFSCIFS = 0;
	connection._chanal = 0;
	system(connection.mnt[connection._chanal]);
	delay (500);
	// Запуск потока проверки чтения
	pthread_create(NULL,NULL,&connectionTest,NULL);
	strcpy(path, chartPath);
	strcat(path, "CHART.LST");
	while(1){
		delay(2000);
		fp = NULL;
		_readFSCIFS = 1;
		fp = fopen(path, "rb");
		_readFSCIFS = 0;
		if(fp==NULL){
			killFSCIFS;
			//printf("Connection error : chanal %d\n", connection._chanal);
			connection._chanal = (connection._chanal+1)&1;
			system(connection.mnt[connection._chanal]);		
		}else{
			printf("Connect OK : chanal %d\n", connection._chanal);
		}
		if(fp!=NULL)
			fclose(fp);
	}
}
