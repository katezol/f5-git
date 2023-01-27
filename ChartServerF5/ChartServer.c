#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "global.h"
#include "config.h"
#include "connectionThread.h"
#include "chartRead.h"
#include "read_write.h"
#include "ping.h"

extern unsigned int bOnlyPing;

int init(void);			// Инициализация сервера
void connect(void);		// Установка соединения с ККС
void readChart1();	// Разбор карт для Клиента-1
void* readChart2(void* data);	// Разбор карт для Клиента-2
extern void sendKKSstate(void* data2);

OurShipGeo ourShipGeo = {0.0, 95.0, 0.0}, oldOurShipGeo = {0.0, 0.0, 0.0};
OurShipGeo ourShipGeo2 = {0.0, 95.0, 0.0}, oldOurShipGeo2 = {0.0, 0.0, 0.0};

time_t chartDate = 0;
time_t chartDate2 = 0;
char chartSrvCfgPath[100];
char currentDir[100];
char tmpPath[100];
char tmpPath2[100];



int main(int argc, char *argv[]) {
	char dop[100];
	sprintf(currentDir,"%s",dirname(argv[0]));
	if(argc>1){
		strcpy(chartSrvCfgPath, argv[1]);
		strcpy(dop,chartSrvCfgPath);
		sprintf(currentDir,"%s",dirname(dop));
	}else{
		strcpy(chartSrvCfgPath,currentDir);
		strcat(chartSrvCfgPath, "/ChartPath.cfg");
	}
	strcpy(tmpPath,currentDir);
	strcat(tmpPath,"/tmpChart/");
	strcpy(tmpPath2,currentDir);
	strcat(tmpPath2,"/tmpChart2/");
	
	strcpy(chartConnectionCfgPath,currentDir);
	strcat(chartConnectionCfgPath,"/chartConnection.cfg");
	
	if (init()!=0) return -1;
	// Запуск потоков отслежки каналов картасервера
	connect();
	pthread_create(NULL, NULL, &sendKKSstate, NULL); // отправка состояния в УС

//	pthread_create(NULL, NULL, &readChart2, NULL);	// Поток четния карт АРМ-2 tempcomm
	//ourShipGeo.fi = 42;
	//ourShipGeo.lambda = 131; // temptemp
	readChart1();

	return EXIT_SUCCESS;
}

int init(void){
	int err;
	char path[256];
	// Загрузка конфига
	strcpy(path, "mkdir ");
	strcat(path, tmpPath);
	system(path);
	strcpy(path, "mkdir ");
	strcat(path, tmpPath2);
	system(path);
	err = loadConfig();
	switch(err){
		case 0: 	printf("Cannot find configure file: %s\n", chartSrvCfgPath);
					return -1;
		case -1:	printf("Port Name fail ... \n");
					return -1;
		default:	//outConfig();
					break;
	}
	err = read_ConnectionCfg();
	if(err==0){
		printf("Cannot find mount config file: %s\n", chartConnectionCfgPath);
		return -2;
	}
	return 0;
}

void connect(void)
{
	if (bOnlyPing == 0)
	{	
	
		pthread_create(NULL, NULL, &connectionThread, NULL);	/* монтирование кналов*/
															//	ККС на лету */
		pthread_create(NULL, NULL, &readThread, NULL);			// Чтение координат корабля
	}
	pthread_create(NULL, NULL, &pingLoop, NULL);			// Проверка состояния ККС
}



void readChart1(){
	FILE *fp;
	char path[100];
	int _test = -1;
	while(_test == -1){
		if(ourShipGeo.fi==95.0){
			delay(100);
			continue;
		}
		memcpy(&oldOurShipGeo, &ourShipGeo, sizeof(ourShipGeo));
		_test = generateChart(ourShipGeo.fi, ourShipGeo.lambda, layerPath, tmpPath, &listChartLst);
		if(_test!=-1){
			strcpy(path, layerPath);
			strcat(path, "/");
			strcat(path, "TIMECREATE");
			fp = fopen(path, "wb");
			chartDate = time(NULL);
			fwrite(&chartDate, sizeof(time_t),1, fp);
			//printf("Time create = %d\n", chartDate);
			fclose(fp);
		}
		delay(500);
	}
	while(1){
		delay(1000);
		if(testRegion(ourShipGeo, oldOurShipGeo)>200){
			_test = -1;
			//printf("New region 1\n");
			memcpy(&oldOurShipGeo, &ourShipGeo, sizeof(ourShipGeo));
			while(_test == -1){
				_test = generateChart(ourShipGeo.fi, ourShipGeo.lambda, layerPath, tmpPath, &listChartLst);
				if(_test!=-1){
					strcpy(path, layerPath);
					strcat(path, "/");
					strcat(path, "TIMECREATE");
					fp = fopen(path, "wb");
					chartDate = time(NULL);
					fwrite(&chartDate, sizeof(time_t),1, fp);
					fclose(fp);
				}
				delay(500);
			}
		}
	}
}


void* readChart2(void* data){
	FILE *fp;
	char path[100];
	int _test = -1;
	while(_test == -1){
		if(ourShipGeo2.fi==95.0){
			delay(100);
			continue;
		}
		memcpy(&oldOurShipGeo2, &ourShipGeo2, sizeof(ourShipGeo2));
		_test = generateChart(ourShipGeo2.fi, ourShipGeo2.lambda, layerPath2, tmpPath2, &listChartLst2);
		if(_test!=-1){
			strcpy(path, layerPath2);
			strcat(path, "/");
			strcat(path, "TIMECREATE");
			fp = fopen(path, "wb");
			chartDate2 = time(NULL);
			//printf("Time2 = %d\n", chartDate2);
			fwrite(&chartDate2, sizeof(time_t), 1, fp);
			fclose(fp);
		}
		delay(500);
	}
	while(1){
		delay(1000);
		//printf("ARM2: (%f, %f)\n", oldOurShipGeo2.fi, oldOurShipGeo.lambda);
		if(testRegion(ourShipGeo2, oldOurShipGeo2)>200){
			_test = -1;
			//printf("new Reg\n");
			memcpy(&oldOurShipGeo2, &ourShipGeo2, sizeof(ourShipGeo2));
			while(_test == -1){
				_test = generateChart(ourShipGeo2.fi, ourShipGeo2.lambda, layerPath2, tmpPath2, &listChartLst2);
				if(_test!=-1){
					strcpy(path, layerPath2);
					strcat(path, "/");
					strcat(path, "TIMECREATE");
					fp = fopen(path, "wb");
					chartDate2 = time(NULL);
					fwrite(&chartDate2, sizeof(time_t), 1, fp);
					fclose(fp);
				}
				delay(500);
			}
		}
	}
}
