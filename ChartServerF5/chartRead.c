#include <stdio.h>
#include <string.h>

#include "chartRead.h"
#include "global.h"
#include "math.h"

extern char  currentDir[100];

struct point{
	double x;
	double y;
};

struct point matrix[NET_SCALE][NET_SCALE];
// Перевод из градусов в радианы  Формат записи x.yy {x-градусы ... }

double deg2rad(double deg){
	double rad;
	rad = M_PI/180.0;
	rad = rad*deg;
	return rad;
}

int testRegion(OurShipGeo p1, OurShipGeo p2)
{
	int radEarth = (int)EARTH_RADIUS;
	int length = 0;
	double d;
	double p1Lat = deg2rad(p1.fi);
	double p1Long = deg2rad(p1.lambda);
	double p2Lat = deg2rad(p2.fi);
	double p2Long = deg2rad(p2.lambda);
	d = cos(p1Lat)*cos(p2Lat)*cos(p1Long-p2Long);
	d += sin(p1Lat)*sin(p2Lat);
	d = acos(d);
	length = d*radEarth;
	return length;
}



void createGeoNet(double latitude, double longitude){
	int i,j;				
	double yMax, yMin, xMax, xMin; // ограничения на параллели и мередианы
	double yStep, xStep;		// шаг сетки 
	double gradKm;				// градус/км
	double yPoint, xPoint;

	//
	gradKm = 360.0/(2*M_PI*EARTH_RADIUS);   
	yMax = latitude + NET_RADIUS*gradKm;
	yMin = latitude - NET_RADIUS*gradKm;
	yStep = (yMax - yMin)/(double)NET_SCALE;
	
	for(i=0; i<NET_SCALE; i++){
		yPoint = yMin + i*yStep;
		if (yPoint>85.0) 
			yPoint = 85.0;
		else if (yPoint<-85.0) 
			yPoint = -85.0; 
		gradKm = 360.0/(2*M_PI*(EARTH_RADIUS*cos(deg2rad(yPoint))));
//		xMax = longitude + 200.0*gradKm;
//		xMin = longitude - 200.0*gradKm;
		xMax = longitude + 200.0*gradKm;
		xMin = longitude - 200.0*gradKm;
		xStep = (xMax - xMin)/(double)NET_SCALE;
		
		for(j=0; j<NET_SCALE; j++){
			xPoint = xMin + j*xStep;
			if(xPoint>180.0){
				xPoint -= 360;
			}else if(xPoint<-180.0){
				xPoint += 360;
			}
			matrix[i][j].y = yPoint;
			matrix[i][j].x = xPoint;
		}
	}
}

void outputGeoNet(void){
	int i,j;
	for(i=0; i<NET_SCALE; i++){
		for(j=0; j<NET_SCALE; j++)
			printf("(%.4f;%.4f) ", (float)matrix[i][j].y, (float)matrix[i][j].x);
		printf("\n");
	}
}

//****************************************************//
int generateChart(double latitude, double longitude, char* dst, char *tP, struct listChartLst *lst){
	FILE *fp;
	int i, j;
	int temp = 1;
	char path[256];
	char path2[256];
	// Обнуление списка карт
	initListChartLst(lst);
	// Создание сети региона
	createGeoNet(latitude, longitude);
	//outputGeoNet();
	// Удаляем все старое
	
	strcpy(path, "rm -rf ");
	strcat(path, tP);
	strcat(path, "/*");
	system(path);
	strcpy(path, chartPath);
	strcat(path, "CHART.LST");
	strcpy(path2, tP);
	strcat(path2, "CHART.LST");
	// Копирование CHART.LST и проверка исправности соединения с ККС
	if(cp__(path, path2)==-1)
			return -1;
	// Ищем карты данного региона

	//for(i=0; i<NET_SCALE; i++)
		//for(j=0; j<NET_SCALE; j++){
		//	if(findCoord(matrix[i][j].y, matrix[i][j].x, tP, lst)==-1)
		//		return -1;
		//}
	if(findCoord(0.0, 0.0, tP, lst)==-1)
		return -1;
	
	//printf("List count = %d\n", lst->count);
	// Копируем карты с ККС на сервер

	for(i=0; i<lst->count; i++){
		strcpy(path, chartPath);
//		strcat(path, "S57/");
		strcat(path, lst->chartLst[i].name);
		strcat(path, ".000");
		strcpy(path2, tP);
		strcat(path2, lst->chartLst[i].name);
		strcat(path2, ".000");
		if(cp__(path ,path2)==-1)
			return -1;
	}
	// Генерируем слои

	// Разбиваем на слои и создаем дирректории
	//if (lst->count ==0) return -1;  // иначе не работает
	
	strcpy(path, dst);
	strcat(path, "/");
	strcat(path, "CHART_LAY.LST");
	fp = fopen(path, "wb");
	fwrite(&lst->count, sizeof(&lst->count), 1, fp);
	for(i=0; i<lst->count; i++){
		// создание дирректории

		strcpy(path, "mkdir ");			
		strcat(path, dst);
		strcat(path, "/");
		strcat(path, lst->chartLst[i].name);
		system(path);
		// создание слоев
		
		strcpy(path,currentDir);
		strcat(path, "/s57_converter ");
		strcat(path, tP);
		strcat(path, lst->chartLst[i].name);
		strcat(path, ".000 ");
		strcat(path, dst);
		strcat(path, "/");
		strcat(path, lst->chartLst[i].name);
		system(path);		
		// Запись в CHART_LAY.LST

		fwrite(lst->chartLst[i].date, 1, 10, fp);
		temp = lst->chartLst[i].scale;
		fwrite(&temp, sizeof(int), 1, fp);
		fwrite(lst->chartLst[i].name, 1, 8, fp);
		fwrite(&lst->chartLst[i].latMin, sizeof(float), 1, fp);
		fwrite(&lst->chartLst[i].latMax, sizeof(float), 1, fp);
		fwrite(&lst->chartLst[i].lonMin, sizeof(float), 1, fp);
		fwrite(&lst->chartLst[i].lonMax, sizeof(float), 1, fp);
	}	
	fclose(fp);
	
	return 0;
}

int cp__(const char* src, const char* dst){
	size_t sz;
	unsigned char ch;
	FILE *srcFile = NULL;
	FILE *dstFile = NULL;
	srcFile = fopen(src, "rb");
	dstFile = fopen(dst, "wb");
	if(srcFile == NULL)
	{	
		printf("srcFile ==NULL src=%s\n",src);
		return -1;
	}	
	if(dstFile == NULL)
	{	
		printf("dstFile ==NULL dst=%s\n",dst);
		return -1;
	}	
	while(!feof(srcFile)){
		sz = fread(&ch, 1, 1, srcFile);
		if(!feof(srcFile)){
			if(sz==0)
				return -1;
			fwrite(&ch, 1, 1, dstFile);
		}
	}
	fclose(srcFile);
	fclose(dstFile);
	return 0;
}
	
int findCoord(double latitude, double longitude, char *tP, struct listChartLst *lst){
	char path[256];
	double minLon=0.0, maxLon=0.0, minLat=0.0, maxLat=0.0;
	FILE *fp;
	struct chartLst tmpChartLst;
	strcpy(path, tP);
	strcat(path, "/CHART.LST");
	
	fp = openChartLst(path);
	if(fp==NULL)
		return -1;
	minLon = matrix[0][0].x;
	maxLon = matrix[0][19].x;
	maxLat = matrix[19][0].y;
	minLat = matrix[0][0].y;
	
	while(!feof(fp)){
		memset(&tmpChartLst,0,sizeof(tmpChartLst));
		if(readChartLst(fp, &tmpChartLst)==0)
			break;
		printf("findCoord(): %f, %f, %f, %f\n", tmpChartLst.lonMax, tmpChartLst.lonMin, tmpChartLst.latMax, tmpChartLst.latMin);
		if((abs(tmpChartLst.lonMax-tmpChartLst.lonMin)<60.0) | (abs(tmpChartLst.latMax-tmpChartLst.latMin)<60.0))
			{ printf("small map - ignore\n");
					continue;
			}		
//		if((minLon*60>tmpChartLst.lonMax)||(maxLon*60< tmpChartLst.lonMin))
//			continue;
	//	if((maxLat*60 < tmpChartLst.latMin)||(minLat*60 > tmpChartLst.latMax))
	//		continue;
		/*if(latitude>tmpChartLst.latMax||latitude<tmpChartLst.latMin||
				longitude>tmpChartLst.lonMax||longitude<tmpChartLst.lonMin)
			continue;*/
		else
			addChartLst(&tmpChartLst, lst);
	}
	fclose(fp);
	return 1;
}

