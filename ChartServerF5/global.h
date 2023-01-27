#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

#include "chartLst.h"


#define EARTH_RADIUS 6371.0
#define NET_SCALE 20
#define NET_RADIUS 200.0
#define COMF = 10000000.0

extern char chartSrvCfgPath[100];

//#define tmpPath "/home/furke2/tmpChart/"
//#define tmpPath2 "/home/furke2/tmpChart2/"

extern char chartPath[100];
extern char layerPath[100];
extern char layerPath2[100];

extern char port_ChartArm1[100];
extern char port_ChartArm2[100];
extern char port_ChartServer[100];
extern char port_KKSstate[100];

extern struct listChartLst listChartLst;
extern struct listChartLst listChartLst2;

typedef struct{
	float course;
	double fi;
	double lambda;
}OurShipGeo; 

extern OurShipGeo ourShipGeo, oldOurShipGeo;
extern OurShipGeo ourShipGeo2, oldOurShipGeo2;

extern char ipKKSosn[100];
extern char ipKKSres[100];

extern time_t chartDate;

#endif /*GLOBAL_H_*/
