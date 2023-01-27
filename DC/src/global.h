#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <stdbool.h>

#include "docStructs.h"

#include <photon/PtText.h>
#include <photon/PtList.h>

#define USE_PANE_IN_PANESEL		1
#define NEW_FRAME_STRUCT

#define TMP_DIR_PATH "/tmp/log/"
#define TMP_LOG_PATH "/tmp/log/docTmp.log"

#ifdef NEW_FRAME_STRUCT
	///todo - move to libDoc (docStructs.h)
	#include <time.h>
	#include <string.h>
	#include <stdio.h>
	#include <stdint.h>
	//| MsgSize(8bit) | MsgTime(32bit) | MsgType(8bit) | MsgStruct(variable size) | ..... | MsgCount(32bit) | EOF


	#define DOC_TIME_t		uint32_t
	#define DOC_TYPE_t		uint8_t
	#define DOC_SIZE_t		uint8_t
	#define DOC_MSG_COUNT_t	uint32_t

	typedef struct DOC_FRAME_S{
		void  *data;
		DOC_TIME_t  time;
		DOC_TYPE_t  type;
		DOC_SIZE_t   size;
	}doc_frame_t;
	#define DOC_TIME_CONVERTER_OPT_NO_MS			0x1	//default time*100 + ms
	#define DOC_TIME_CONVERTER_OPT_FORMAT_NO_MS		0x2	//default format H:M:S.ms
	#define DOC_TIME_CONVERTER_OPT_FORMAT_ADD_DMY	0x4	//default format H:M:S.ms
	#define DOC_TIME_CONVERTER_OPT_FORMAT_ADD_ENDL	0x8	//default add '\0'
	void timeConverter(char* buffStr, time_t rawTime, uint16_t opts);

#endif

//typedef unsigned char   byte;
//typedef unsigned short   word;

struct fileList{
	int ind;
	char name[100];
	char date[100];
};

typedef struct
{
	char name[40];
	char path[40];
} Param;


struct message{
	time_t time;
	unsigned short type;
	char msg[256];
};

struct setupFlags{
	int f1[1];
	//
	int f2[3];
	//
	int f3[2];
	//
	int f4[6];
	//
	int f5[9];
};

#ifndef NEW_FRAME_STRUCT

// Фрэем из лога
struct frame{
	unsigned char show;			// Показывать или нет Фрейм (Используется при фильтрации)
	unsigned char size;			// тип сообщения
	char data[200];				// данные сообщения
};

#endif


struct pointCoord{
	float distance;
	float peleng;
	unsigned char pr_own;               // госпринадлежность точки,
};

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

struct pointTrassa{
	bool draw;
	time_t timeDraw;
  	int indChannel;								// индекс канала, используется для передачи
    byte nSource;                               // номер цели в нумерации 5П-27М или 0
    ObjType objType;						    // тип цели
    Own own;                                    // госпринадлежность цели
    bool bvz;									// признак наличия скорости по высоте
    bool bSlow;									// признак малоподвижной цели
    bool bHandEscort;							// признак па сопровождения
    bool bAutoEscort;							// признак захвата
    //bool bDoubleAccuracy;						// признак сопровождения с двойной точностью
    bool bPassive;								// признак помехопостановщика
    bool bReset;								// признак сброса цели
    //bool bExtrapol;								// признак экстраполированных данных
    bool bLoss;									// признак неустойчивого сопровождения
    //byte epr;									// 1-малая<1м2, 2-средняя 1-20м2, 4-большая >20м2
    float x;									// координата x, м или пеленг, если bPassive
    float y;									// координата y, м
    float z;									// координата z, м или ум, если bPassive
    float vx;				
    // проекция скорости на x или скорость изменения пеленга, м/с
    float vy;									// проекция скорости на y, м/с
    float vz;									// проекция скорости на z или скорость изменения ум, м/с
    float skoX;
    float skoY;
    float skoZ;
    float skoVx;
    float skoVy;
    float skoVz;
    /************ эти данные считаются отдельно ******************/
    
    float distance;								// дистанция наклонная, м
    float distanceHor;							// дистанция горизонтальная, м
    float peleng;								// пеленг, рад
    float um;    								// УМ, рад
    float course;								// курс цели, рад
    float vr;									// м/с радиальная скорость
    /***************************************************************/
    
    short iname;      // признак имитиранной цели! // идентификатор объекта, используется только в тренировочных режимах
    double tour;								// время наше внутреннее выработки ЦУ
    float tsigma;								// время выработки ЦУ сигма
    char bsigma;								// признак времени сигмы
    unsigned short t3k96;									// время выработки ЦУ 3K963, unsigned short
    char b3k;									// признак времени 3K963
};

struct PointTrassaServal{
    unsigned char draw;                           // номер цели в нумерации X или 0
	time_t timeDraw;
    unsigned char nServal;                       // номер цели в нумерации X или 0
    unsigned char nTelevik;                     // номер цели в нумерации X или 0
    unsigned char AC;                           // номер цели в нумерации X или 0
    ObjType objType;		   	    // тип цели
    unsigned char bFire;						    // тип цели
    unsigned char reserve0;						    // тип цели
    unsigned char reserve1;						    // тип цели
    unsigned char reserve2;						    // тип цели
    float distance;								// дистанция наклонная, м
    float peleng;								// пеленг, рад
    float course;								// курс цели, рад
} ;

struct PointTrassaX{
	bool draw;
	time_t timeDraw;
    unsigned short nX;                           // номер цели в нумерации X или 0
    ObjType objType;						    // тип цели
    Own own;                                    // госпринадлежность цели
    float distance;								// дистанция наклонная, м
    float peleng;								// пеленг, рад
    float course;								// курс цели, рад
};

struct pointArray{
	int count;
	int curPos;
	struct pointCoord pointCoord[2000];
};

struct pointDraw67{
	int count;
	int curPos; 
	char draw[100];
	time_t timeDraw[100];
	DocDev67Tgt dev67Tgt[100];
};

// Проверка точек для рисования
extern struct pointArray pointArray;	// следы трасс (отображаются точечками)
extern struct pointTrassa pointTrassa[200]; // Трассы (отображаются значками)
extern struct pointTrassa pointTelevik[100]; // Трассы (отображаются значками)
extern struct PointTrassaServal pointServal; // Трассы (отображаются значками)
extern struct PointTrassaX pointTrassa_X[300]; // Трассы (отображаются значками)
extern struct pointDraw67 pointDraw67;	// Отметки изделия 67
extern double courseShipView;			// Курс корабля для отрисовки

extern struct message *message;
//extern struct fileList fileList[12];
//extern int posSellect;
extern int fileSellect;
extern int readCount;
extern char filePath[100];
extern char pathPhoton[100];
extern char docPath[100];

extern PtListCallback_t 	selectedListStuct;


#define READ_COUNT 14
extern int logFile;
extern int readLogFileCount;

// Фрэймы ил Лог файла достаточно большой по размеру
#ifndef NEW_FRAME_STRUCT
	extern struct frame *frame;
#else
	extern doc_frame_t	*DFrame;
#endif

extern int showFrameCount;
extern unsigned int *showFrame;

// Документирование
extern int playSpeed;
// Настройка
extern struct setupFlags setupFlags;

// Отображение
extern double VIEW_SCALE;
extern char Helvetica18[MAX_FONT_TAG];

extern int TimerRepeat;

#endif /*GLOBAL_H_*/
