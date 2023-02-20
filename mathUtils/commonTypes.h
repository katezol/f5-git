 /**
 * @file commonTypes.h
 * @short общие типы объектов
 */

#ifndef COMMONTYPES_H_
#define COMMONTYPES_H_

#include <stdbool.h>

//#define BYTE	unsigned char
#define BIT(x)	(1 << x)
#define BIT7			BIT(7)


typedef unsigned char   byte;
//typedef unsigned short   word;
typedef int             BOOL;
typedef unsigned short  WORD;
typedef unsigned int    DWORD;
typedef DWORD *         LPDWORD;
typedef void *          LPVOID;
typedef unsigned long   ULONG;
typedef int             LPOVERLAPPED;

#define LOWORD(dwValue) ((DWORD)dwValue & 0xFFFF)
#define HIWORD(dwValue) ((DWORD)dwValue >> 16)


 typedef struct Point3d
 {
 	float x;		// на север
 	float y;		// на восток
 	float z;		// вверх
 }Point3d; // описание 3-мерной точки

 typedef struct
 {
 	float x;
 	float y;
 }Point2d;
 
 typedef struct
 {
 	float distance;	//наклонная дальность
 	float peleng;	//пеленг
 	float elevation;//угол места
 }PolarPoint3d; //трехмерная точка в полярных координатах
 
 typedef struct
 {
 	float distance;
 	float peleng;
 }PolarPoint2d;
 
 typedef short ObjName; 	

 typedef struct 
 {
    ObjName iname;   // идентификатор (== -1 - признак того, что данные пустые)
 	float x;		// на север
 	float y;		// на восток
 	float z;		// вверх
 	float v;		// м/с
 	float course;	// рад, от оси Х (север) по часовой стрелке
 	float pitch;	// рад, от плоскости горизонта против часовой(вверх == +)
 } Coordinat; // Coordinat - описание координат объекта, истинное(поступает от гирокомпасов итд)
 
 typedef struct 
 {
 	float course;//курс-радианы
 	double fi;//широта-градусы
 	double lambda;//долгота-градусы
 }OurShipGeo; 

 
 typedef struct
 {
 	ObjName iname;
 	bool bNewz;
 	bool bNewv;
 	bool bNewCourse;
 	bool bNewPitch;
 	float z;
 	float v;
 	float course;
 	float pitch;
 } Control;


  
  typedef enum 
      {
         owUnknown,          //  = 0;  Госпринадлежность не определена
         owOur,              //  = 1;  Свой
         owEnemy,            //  = 2;  Чужой
         owNeitral          //  = 3;  Нейтральный*/
       } Own; //- описание госпринадлежности цели
       
  typedef enum 
   {
   	otUndefine,
   	otSurface,                      		// 1 надводная цель
   	otShip,                      			// 2 надводный корабль
   	otAir,									// 3 воздушная цель
   	otPlane,                      			// 4 самолет
   	otLft,									// 5 низколетящая цель
   	otFalseTarget,							// 6 ложная цель
   	otMissileCommon,              			// 7 просто ракета
   	otPkr,									// 8 ПКР
   	otPrr,									// 9 ПРР
   	otHelicopter,							// 10 вертолет
   	otPn,									// 11 помехоноситель
   	otSat,									// 12 внезапно появившаяся цель
   	otTrap									// 13 ловушка
   } ObjType; // - тип цели
   
   
 
 typedef struct
 {
    ObjName iname;   // идентификатор (== -1 - признак того, что данные пустые)
 	float x;		// на север
 	float y;		// на восток
 	float z;		// вверх
 	float v;		// м/с
 	float course;	// рад, от оси Х (север) по часовой стрелке
 	float pitch;	// рад, от плоскости горизонта против часовой(вверх == +)
 	ObjType objType;// тип объекта
 	float width;	// ширина, м
 	float height;	// высота, м
 	float length;	// длина, м
 	Own own;	// госпринадлежность
 }  ObjectData;
/*                     
 typedef struct 
     {
    	bool bCourse;            // курс выработан
        bool bPitch;        		// угол наклона траектории в вертикальной плоскости выработан
        bool bDistance;     		// дистанция выработана
        bool bPeleng;       		// пеленг выработан
        bool bUm;           		// УМ выработан
        bool bSKOPeleng;    		// СКО пеленга выработано
        bool bSKODistance;  		// СКО дистанции выработано
        bool bH;              	// высота выработана
        bool bExist;          	// ЦУ выработано
        bool bLft;              	// выработан (Low Flying Target)
        bool bSat ;              	// выработан (Suddenly Appearing Target)
     } ComplexTargetFlags; // - описание флагов сопровождаемой цели
    
 typedef struct  
 {
    byte nChannel;                                // номер канала
    byte idComplex;                               // имя комплекса
    byte nChannelComplex;
    ComplexTargetFlags flags;                    // флаги
    Own own;                                     // госпринадлежность цели
    ObjType objType;                              // тип цели
    float x, y, h;
    float peleng;
    float distance;                              // дистанция до цели, м
    float um; 									 // дистанция до цели, м
    float vx, vy, vh;
    float vr;                                    // скорость цели , м/с
    float xOur, yOur, hOur;                      // коор-ты точки отсчета , м  - нашего корабля
    float kDanger;     
    float course;                                // курс цели, рад
    float pitch;                                 // наклон траектории, рад
    float t;                                     //  время ЦУ
    float skoPeleng;                             // СКО пеленга, рад
    float skoDistance;                         // СКО дальности, м
   } ComplexTarget; // - описание сопровождаемой цели
*/   
typedef struct
{ 
   float rCirkHor ,rCirkVer;   	// радиусы поворота в вертикальной и горизонтальной плоскостях
   float wh,wv;           		// угловые скорости в вертикальной и горизонтальных плоскостях
   bool bConstCirk;				// радиусы циркуляции при изменении скорости не менять
   int napravH, napravV;  		// направления циркуляции в гор. и верт. плоскостях
   float maxGravitation;   		// максимальная перегрузка
   float zMax;   				// максимальная высота (глубина)
   float zMin;   				// минимальная высота  (глубина)
   float vMax;   				// максимальная скорость
   float vMin;   				// минимальная скорость
   float hWantedOld;			// старая высота
   float pitchMax;				// максимальный угол наклона траектория
   //bool bDownUp;				// признак изменения угла наклона в верт. плоскости
   float hFlyMarshDown;			// высота изменения направления циркуляции (точка перегиба)
} MobilityData;  // контекст объекта с маневрами в вертикальной и горизонтальных плоскостях
   
   

typedef struct
{ 
   float distance;   			// дистанция
   float distanceHor;  			// дистанция горизонтальная
   float peleng;   				// пеленг
   float um;   				    // высота
   ObjType objType;   			// тип объекта
   Own own;   			// тип объекта
   float z;   					// высота
   ObjName iname;				// имя
} VisualMark;  // описание видимой активной РЛС цели
   
   
   /* цель воздушная?*/
   bool isAir(ObjType objType);
   /* цель надводная?*/
   bool isSurface(ObjType objType);
   /* цель надвлдный корабль?*/
   bool isShip(ObjType objType);
   /* цель какая-нибудь ракета?*/
   bool isRocket(ObjType objType);
   /* цель надводная?*/
   bool isUnderwater(ObjType objType);
   /* определяет строковое значение типа цели?*/
   char * getStrObjType(ObjType objType);
   
/* заполняет контекст по типу объекта*/
	void setTypicalMobilityDataFromObjType (
              ObjType objType, 
              MobilityData * pmobilityData);
   

  
  #endif /*COMMONTYPES_H_*/
   
    
        
