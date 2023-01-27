#ifndef TEMPFORMODELING_H_
#define TEMPFORMODELING_H_
/* 
 * файл для организации:
 * 1. моделирования объектов
 * 2. моделирования работы РЛС
 */
 
#include <stdbool.h>
#include <typesRls.h>
#include <formular.h>
 
void init_model(int Firstnit);

//void fill_new(void);

void * modeling_thread(void *arg);

#define NVISUALMARKS 500

double timeModeling;

double currTime; 		// текущее время

Coordinat ourShip;
OurShipGeo ourShipGeo;
double centerFi, centerLambda;

double oldCurrTime; // начальная фаза
double timeBegin; // начальная фаза





float timeOneTurn;//время одного оборота антенны

ObjectData *pobjectDatas[NRLSCHANNELS];
Control *pcontrols[NRLSCHANNELS]; // управляющее воздействие
MobilityData *pmobilityDatas[NRLSCHANNELS];
ObjectData objectDatas[NRLSCHANNELS];
Control controls[NRLSCHANNELS]; // управляющее воздействие
MobilityData mobilityDatas[NRLSCHANNELS];
VisualMark visualMarks[NVISUALMARKS];
short indOurShip; // индекс своего корабля в массиве pobjectDatas или -1

BosImitatorData bid, oldBid; // для РМО-2, в боевом - из rlsTargetsBos
int nVariant,nVariantOld; // текущие и предыдущие настройка иммитатора
// RlsTarget oldrlsTargetsPVO[NRLSCHANNELS]; 
 

#endif /*TEMPFORMODELING_H_*/
