#ifndef CHARTREAD_H_
#define CHARTREAD_H_

#include "global.h"

// Расстояние между точками
int testRegion(OurShipGeo p1, OurShipGeo p2);

int generateChart(double latitude, double longitude, char *dst, char *tP, struct listChartLst* lst);
// Сеть охватывающая карты в определенном радиусе действия 
void createGeoNet(double latitude, double longitude);

// Генерация карты 
	// Копирование файла
	int cp__(const char *src, const char *dst);	// -1 неудача при копировании
	// Нахождение координат в списке и составления списка карт данного региона
	int findCoord(double latitude, double longitude, char *tP, struct listChartLst *lst);


#endif /*CHARTREAD_H_*/