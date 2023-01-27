#ifndef DOCSRUCTS_H_
#define DOCSRUCTS_H_

#include <time.h>
#include <stdint.h>

//#include "filter.h"

#define byte unsigned char
#define bool  char
#define word unsigned short


typedef struct{
	unsigned char nType;    // FORMULAR_TRASSA ( 15 )
	unsigned int ShP 	: 1;// ШП - признак помехлпостановщикаchar0; (пассивная, активная)
	unsigned int PA 	: 1;// ПА - признак сопровождения в полуавтоматическом режимепомехлпостановщикаchar0;
	unsigned int PNS 	: 1;// ПНС - признак неустойстового сопровождения
	unsigned int SBR 	: 1;// СБР - признак сброса
	unsigned int MPO 	: 1;// МПО - признак малоподвижного объекта
/*f*/	unsigned int SV 	: 2;// СВ  - 0 -неопозна 1-чужая, 2-своя
	unsigned int PZ     : 1;// ПЗ  - признак захвата
/*f*/	unsigned int TC     : 4;// ТЦ - тип цели 0-неопр, 1-надв, 8-возд неопр, 9-ловушка, 10-самолет,
        						// 11-ПКР, 12-ПРР, 13-ВЕРТ, 15-ВПЦ;
	unsigned int bVz    : 1;// Пн - признак наличия скорости по высоте
	unsigned int NEW   : 1; //Признак новой трассы
	unsigned int bKT   : 1; //Признак привязки КТ
	unsigned int PRC   : 1; //Признак приоритетной цели
/*f*/	unsigned int nSource: 8;// НТномер трассы
	
	unsigned long t;		// ТЭ - время измерения или эквтраполяции
	
	unsigned int X 		: 16;// X или пеленг;
	unsigned int TNO 	: 16;// ТНО - текущий номер обзора
	
	unsigned int H 		: 16;// H  - высота или УМ
	unsigned int Y 		: 16;// Y
	
	unsigned int skoY 	: 16;// СКОУ
	unsigned int skoX 	: 16;// СКОХ
	
	unsigned int Vx 	: 10;// Vx или Vпеленг
	unsigned int skoVx 	: 6;// СКО Vx
	unsigned int skoH 	: 16;// СКО Н
	
	unsigned int Vh 	: 10;// Vh или Vум
	unsigned int skoVh 	: 6;// СКО Vh
	unsigned int Vy 	: 10;// Vy
	unsigned int skoVy 	: 6;// СКО Vy
}DocFormularTrassa; // Формуляр трасса 2

typedef struct{ // тип для  ИП3 из Пумы
	int indPuma; // 0-3 индекс канала ПУМЫ
/*f*/	byte nPuma; // НЦ в нумерации ПУМЫ (1-63) или 0
/*f*/	word nRls; // НЦ в нумерации РЛС (1-999) или 0
/*f*/	byte prn; // 1-чужая, 2-своя
		byte typeSource; // 0-неопр, 16-МТК(ПБ), 17-МТК(ЛБ),  19-Монумент, 21-РЛК, >29-Пума
		bool imc; // признак имитированной цели
/*f*/	byte objType; //0-неопр, 1-ВЦ, 3,4-МЦ(НЦ),11-вертолет, 12-самолет(СМЛ), 13-ПРР, 14-ПКР, 15-ВПЦ
	byte nau[3]; //назначение в РЭСУ цели:  0-не назначена, 1-цель основная, 2-цель резервная, 3-цель основная и резервная
	byte rrc; //0-нет работы,2-обнаружение, 3-захват, 4-АС,5-ИС, 6-ПА, 7-срыв сопровождения
	byte rsd;//разрешение на снятие данных ЦУ 1-да
	byte dcu;//доклад о работе по внешнему ЦУ 1-ЦУ принимается, 2-ЦУ принято, 3-срыв сопровождения цели
           // 4-отказ от приема ЦУ, 5-отмена ЦУ ринята
	byte ppc;// признак действия по цели 2-цель поражена
	byte fire[3];//1-веду огонь - на время замыкания педали
	byte sks; 	// 0-скорости в прямоугольная СК
	byte skk; 	// 0-прямоугольная СК, 1-полярная (П,УМ)
	bool bVx; 	// наличие Vx
	bool bVy; 	// наличие Vy
	bool bVh; 	// наличие Vh
	bool bx;  	// наличие x(П)
	bool by;  	// наличие y(УМ)
	bool bh;  	// наличие h
	byte sourceCu; // источник данных ЦУ 0-неопр, 1-данные источника, иначе-Пума
	float x;	// в м, или П (рад) - координаты биссектрисы заданного сектора
	float y;    // в м, или УМ (рад) - координаты биссектрисы заданного сектора
	float h;    // в м
	float vx;	// в м/с
	float vy;   // в м/с
	float vh; 	// в м/с
}DocIP3_Puma;

typedef struct 
{
	float course;//курс-радианы
	double fi;//широта-градусы
	double lambda;//долгота-градусы
}DocOurShipGeo; 

typedef struct 
{
	double currentTime;// текущее время УС, сек. от включения
	unsigned int timeSigma;//текущее корабельное время
	unsigned int timeBOS;// текущее время БОС, мс
	word time3K96;// текущее время Редута, мс
	word reserve0;
	float reserve1;
	float reserve2;
	float reserve3;
}DocTime;

typedef struct{
	float			pel; // ПЕЛЕНГ ТРЕВОГИ, рад
}DocDev67Alarm;

typedef struct{
	bool B;//,бедствие
	bool PN;//,признак поступления номера цели (только в 4м режиме)
	unsigned short	num;//,номер цели (только в 4м режиме)
	float	pel;// град
	float	dist;// км
	byte line;// 1,2,4
	byte mode;//1-КОРАБЛЬ 2-САМОЛЕТ
	word numberRLK;
}DocDev67Tgt; // цель обнаружена 67 изделием + отожденствлена РЛК

typedef struct 
{
	unsigned short b5P27	: 1;  
	unsigned short b3K96	: 1;
	unsigned short bSigma	: 1;
	unsigned short b5P20	: 1;
	unsigned short bMTK		: 1;
	unsigned short bSfera	: 1;
	unsigned short bX	: 1;
	unsigned short res	: 10;
} SourceGo;

typedef struct 
{
	word numberRlk;//// номер канала РЛС
	word numberX;// номер канала X
	word numberSfera;// номер канала Sfera
	SourceGo sourceGo;// истоник(и) заявки
	word parameter;//причина
}Doc67Zapros; //DI_SwitchOff67 выключение запросчика+сброс цели с опознавания
 // DI_NewZapros67новый запрос по цели
 // DI_DeleteZapros удаление запроса из очереди запросов
//  DI_Zapros67 произвести ГО в ПА режиме

typedef struct 
{// типа DI_ZaprosSkol67  запрос на опознавание по сколу
	  unsigned int lineSkol; // линия скола 1-корабль 2-самолет 
	  unsigned int modeSkol;  // режим скола 1-I, 2-II, 4-IV, 0- текущий  		    
	  float pelengSkol;// пеленг скола, рад
	  float distanceSkol;// дистанция наклонная скола, м	
	  float reserve[4];
}Doc67ZaprosSkol; //

typedef struct 
{
	word numberRlk;// номер канала РЛС
	unsigned int own: 3;// 1-свой 2-чужой 0-неопознанный
	unsigned int  bX: 5;// признак передачи цели в нумерации Х, 2-Сфера
}DocHandGO; // назначение признака ГП

typedef struct 
{
	word numberRlk;// номер канала РЛС
	word numberShootChannel;    // номер стрельбового канала
	word parameter; // причина
}DocTaskTarget;// задание цели

typedef struct 
{
	word numberFurke;// номер канала РЛС
	word numberX;// номер канала РЛС
	word numberEco;// номер канала РЛС
	word numberShootChannel;    // номер стрельбового канала
	word parameter; // причина
}DocTaskTargetAll;// задание цели

typedef struct 
{
	word numberShootChannel;// номер стрельбового канала
	word parameter; // причина
}DocNullShootChannel; // сброс канала
// DI_NullShootChannelOperator; // сброс канала оператором
// DI_NullShootChannel; // сброс канала,расшифровка причины

typedef struct 
{
	word numberShootChannel;// номер стрельбового канала
	word parameter; // причина
}DocResetShootChannel; // сброс канала
// DI_ResetShootChannel; // выставление признака сброс стрельбовому каналу


typedef struct 
{
/*f*/	unsigned int nSource: 8;// НТ номер трассы
	unsigned int reserve: 24;	
}DocSendRli; 
// DI_SendRli // цель выдана в канал РЛИ

typedef struct 
{
	unsigned int reserve: 32;	
}DocSendRedut; 
// DI_SendRedut // выдача пакета в Редут

typedef struct 
{
	word numberRlk;				// номер канала РЛR	
	word numberShootChannel;    // номер стрельбового канала	
	word RVV;	// 1-цель сопровождать с повышенной точность,0-отмена RVV
	word reserve;	
}DocRVVRedut; 
// DI_RVVRedut // запрос о сопровождении с повышенной точностью


typedef struct
{
        unsigned char nType;    // OPERATOR_DIRECTIVE ( 1 )
        unsigned int TC 	: 4;	//!< признак типа цели 0-неопр, 1-надв, 8-возд неопр, 9-ловушка, 10-самолет, 
		// 11-ПКР, 12-ПРР, 13-ВЕРТ, 15-ВПЦ;
        unsigned int NTC 	: 1;	 //!< признак назначения типа цели
        unsigned int PC 	: 1;	//!< признак назначения приоритетной цели
        unsigned int YST 	: 1;	//!< признак назначения юстировочных констант
        unsigned int NZZ 	: 1;	//!< признак зоны заз
        unsigned int OZZ 	: 1;    //!< признак отмены зоны заз
        unsigned int ONC 	: 1;	//!< отображение надводной цели
        unsigned int VA1 	: 1;	//!< признак первого ввода с переводом на автосопровождение
        unsigned int PA 	: 2;	//!< код режима полуавтоматического сопровождени 0-нет, 1-1В, 2-К
        unsigned int SBR 	: 1;	//!< признак сброса трассы с сопровождени
        unsigned int OPC 	: 1; 	//!< отмена назначения признака приоритетной цели
        unsigned int TRC 	: 1; 	//!< признак назначения режима РТЦ по трассе
        unsigned int NT3 	: 8;	//!< номер трассы или номер зоны заз
        
        unsigned int B1 	: 16;	//!< пеленг в сск начала сектора заз/пеленг кт пои/ юстировочная поправка по пеленгу при ЮСТ=1
        unsigned int TNO 	: 16;	//!< номер обзора
        unsigned int B2 	: 16;	//!< пеленг в сск конца сектора заз
        unsigned int Eps 	: 16; 	//!< угол места в сск/угол места кт пои/ юстировочная поправка по углу места при ЮСТ =1
        unsigned int R1 	: 16;	//!< дальность начала сектора заз в сск/дальность кт пои/ юстировочная поправка по дальности при ЮСТ =1
        unsigned int R2 	: 16;	//!< дальность конца сектора заз
        unsigned int Tlok 	: 32;	//!< время локации(ms)
 } DocOPERATOR_DIRECTIVE_STRUCT2;
typedef struct
{
        unsigned char nType;    // OPERATOR_DIRECTIVE ( 1 )
        unsigned int pad0	: 4;
        unsigned int typeMes: 7;    // typeMes=127, только в этом случае такая расшифровка
        
        unsigned int pad1	: 5;        
        unsigned int NT3 	: 8;	//!< номер трассы,по которой выдается высота
        
        unsigned int PRM 	: 1;	//!< признак назначения мирного времени
        unsigned int MIR 	: 1;	//!< признак мирного времени
        unsigned int PRNC 	: 1;	//!< признак установки кода для отображения НЦ
        unsigned int NC 	: 3;	//!< код порога для отображения НЦ 0-7
        unsigned int pad2 	: 9;	//!< пеленг в сск конца сектора заз
        unsigned int PRH 	: 1;	//!< признак установки высоты NT3
        unsigned int H 		: 16;	//!< высота, м NT3
        
        unsigned int pad3 	: 32;	//!< пеленг в сск конца сектора заз
        unsigned int pad4 	: 32;	//!< дальность начала сектора заз в сск/дальность кт пои/ юстировочная поправка по дальности при ЮСТ =1
        unsigned int pad5 	: 32;	//!< дальность конца сектора заз
} DocOPERATOR_DIRECTIVE_STRUCT3;

typedef struct
{
    unsigned char nType;    // ANSWER_BOS_OPERATOR ( 10 )
    unsigned int pad0	: 5;	
    unsigned int PC 	: 1;	//!< признак назначения приоритетной цели
    unsigned int YST 	: 1;	//!< признак назначения юстировочных констант
    unsigned int NZZ 	: 1;	//!< признак зоны заз
    unsigned int OZZ 	: 1;    //!< признак отмены зоны заз
    unsigned int pad1 	: 3;	//
    unsigned int refuseVA1 	: 1;	//отказ от корректуры
    unsigned int error 	: 1;	//!< ошибка в директиве оператора
    unsigned int OPC 	: 1; 	//!< отмена назначения признака приоритетной цели
    unsigned int pad2 	: 1; 	//
    unsigned int NT3 	: 8;	//!< номер трассы или номер зоны заз
 
    unsigned int B1 	: 16;	//!< пеленг в сск начала сектора заз/юстировочная поправка по пеленгу при ЮСТ=1
    unsigned int B2 	: 16;	//!< пеленг в сск конца сектора заз
    unsigned int R1 	: 16;	//!< дальность начала сектора заз в сск/юстировочная поправка по дальности при ЮСТ =1
    unsigned int R2 	: 16;	//!< дальность конца сектора заз
} DocANSWER_BOS_OPERATOR_STRUCT2;
typedef struct
{
	unsigned char nType;    // ANSWER_BOS_OPERATOR ( 10 )
	unsigned int errorCode	: 4; // код ошибки в директиве оператора, 0-нет ошибки
	unsigned int typeMes: 7;    // typeMes=127, только в этом случае такая расшифровка
	unsigned int pad1	: 5;        
	unsigned int NT3 	: 8;	//!< номер трассы,по которой выдается высота
 
	unsigned int PRM 	: 1;	//!< признак назначения мирного времени
	unsigned int MIR 	: 1;	//!< признак мирного времени
	unsigned int PRNC 	: 1;	//!< признак установки кода для отображения НЦ
	unsigned int NC 	: 3;	//!< код порога для отображения НЦ 0-7
	unsigned int pad2 	: 9;	//!< пеленг в сск конца сектора заз
	unsigned int PRH 	: 1;	//!< признак установки высоты NT3
	unsigned int H 		: 16;	//!< высота, м NT3
 
	unsigned int pad3 	: 32;	//
} DocANSWER_BOS_OPERATOR_STRUCT3;


typedef struct
{
        unsigned char nType;    // CODOGRAMM_ADAPTATION_CONTROL ( 3 )
        unsigned int    bBR				: 1;           //!< директива БР
        unsigned int    bDR				: 1;           //!< директива ДР
        unsigned int    bRR				: 1;           //!< директива РР
        unsigned int    bISH				: 1;           //!< директива ИСх
        unsigned int    NRO				: 4;           //!< номер режима обзора
        unsigned int    bRO				: 1;           //!< признак назначения режима
        unsigned int    bLCHM			: 1; 		   //!< признак переключения наклона лчм
        unsigned int    bACBF			: 1;           //!< признак включения ACBF
        unsigned int    bADBF			: 1;           //!< признак включения АдBF
        unsigned int    bRVF				: 1;           //!< признак ручного выбора частоты
        unsigned int    bVRSH			: 1;           //!< признак передачи значения скорости
        unsigned int    bRCH				: 1;           //!< признака назначения разрешенных частот
        unsigned int    NCHRV			: 5;           //!< номер частоты при ручном выборе
        unsigned int    VR60			: 1;		   //!< скорость вращения АПУ 1 об/мин
        unsigned int    VR30			: 1;           //!< скорость вращения АПУ 30 об/мин
        unsigned int    VR3				: 1;           //!< скорость вращения АПУ 20 об/мин
        unsigned int    VR6				: 1;           //!< скорость вращения АПУ 6 об/мин
        /* ***dWord*** */
        unsigned int    f              : 32;           //!< номер разрешенной частоты
        /* ***dWord*** */
        unsigned int    Dkgk       : 14;          //!< дальность КгК по кругу (изменение значения ДКгК, назначенного по умолчанию при начальном включении)
        unsigned int    Dvaru      : 12;          //!< дальность действия ВАРУ по кругу (изменение значения ДВАРУ, назначенного по умолчанию)
        unsigned int    bDkgk            : 1;           //!< признак изменения дальности КгК по кругу
        unsigned int    bDvaru           : 1;           //!< признак изменения дальности ВАРУ по кругу
        unsigned int    bMDFA			: 1; 			// Признак изменения режима МДФА
        unsigned int    MDFA			: 1; 			// Признак режима МДФА
        unsigned int    LCHM			: 1; 			//!< признак изменения наклона лчм
        unsigned int    AOGP			: 1;			//признак автоматического режима определения ГО        
} DocCODOGRAMM_ADAPTATION_CONTROL_STRUCT;

typedef struct
{
        unsigned char nType;    // ANSWER_ADAPTATION_CONTROL ( 17 )
		unsigned int    bBR                 : 1;           //!< Директива БР
		unsigned int    bDR                 : 1;           //!< Директива ДР
		unsigned int    bRR                 : 1;           //!< Директива РР
		unsigned int    bISCH            	: 1;           //!< Директива ИСХ
        unsigned int    NRO					: 4;           //!< номер режима обзора
        unsigned int    bRO					: 1;           //!< признак назначения режима
        unsigned int    bLCHM				: 1; 		   //!< признак переключения наклона лчм
        unsigned int    bACBF				: 1;           //!< признак включения ACBF
        unsigned int    bADBF				: 1;           //!< признак включения АдBF
        unsigned int    bRVF					: 1;           //!< признак ручного выбора частоты
        unsigned int    bVRSH				: 1;           //!< признак передачи значения скорости
        unsigned int    bRCH					: 1;           //!< признака назначения разрешенных частот
        unsigned int    NCHRV				: 5;           //!< номер частоты при ручном выборе
        unsigned int    VR60				: 1;		   //!< скорость вращения АПУ 1 об/мин
        unsigned int    VR30				: 1;           //!< скорость вращения АПУ 30 об/мин
        unsigned int    VR3					: 1;           //!< скорость вращения АПУ 20 об/мин
        unsigned int    VR6					: 1;           //!< скорость вращения АПУ 6 об/мин
        unsigned long 	f;	//
		unsigned int 	Dkgk 					: 14;
		unsigned int 	Dvaru					: 12;
		unsigned int 	bDkgk 					: 1;
		unsigned int 	bDvaru					: 1;
		unsigned int 	bMDFA					: 1;
		unsigned int 	MDFA 					: 1;
		unsigned int 	LCHM 					: 1;
		unsigned int 	reserve0				: 1;       
} DocANSWER_ADAPTATION_CONTROL_STRUCT;


typedef struct
{
        unsigned char 	nType;    // CODOGRAMM_GENERAL_MODES ( 6 )
       	unsigned int    reserve0			: 1;
        unsigned int    bInitial            : 1;           //!<  ИСХ
        unsigned int    bBR                 : 1;           //!<  BR        
        unsigned int    bDR                 : 1;           //!<  ДР
       	unsigned int    bRR                 : 1;           //!<  РР     
       	unsigned int 	ACBF 				: 1;
       	unsigned int 	AdBP 				: 1;
       	unsigned int 	LCHM 				: 1;
       	unsigned int 	NRO 				: 4;
       	unsigned int 	reserve1 			: 1;
       	unsigned int 	high 				: 1;
       	unsigned int 	reserve2 			: 1;
       	unsigned int 	NCHRV 				: 5;
       	unsigned int 	speed60 			: 1;
       	unsigned int 	speed30 			: 1;
       	unsigned int 	speed3 				: 1;
       	unsigned int 	speed6 				: 1;
         unsigned int    bF32                 : 1;           //!< признак разрешенной частоты
         unsigned int    bF31                 : 1;
         unsigned int    bF30                 : 1;
         unsigned int    bF29                 : 1;
         unsigned int    bF28                 : 1;
         unsigned int    bF27                 : 1;
         unsigned int    bF26                 : 1;
         unsigned int    bF25                 : 1;
         unsigned int    bF24                 : 1;
         unsigned int    bF23                 : 1;
         unsigned int    bF22                 : 1;
         unsigned int    bF21                 : 1;
         unsigned int    bF20                 : 1;
         unsigned int    bF19                 : 1;
         unsigned int    bF18                 : 1;
         unsigned int    bF17                 : 1;
         unsigned int    bF16                 : 1;
         unsigned int    bF15                 : 1;
         unsigned int    bF14                 : 1;
         unsigned int    bF13                 : 1;
         unsigned int    bF12                 : 1;
         unsigned int    bF11                 : 1;
         unsigned int    bF10                 : 1;
         unsigned int    bF9                  : 1;
         unsigned int    bF8                  : 1;
         unsigned int    bF7                  : 1;
         unsigned int    bF6                  : 1;
         unsigned int    bF5                  : 1;
         unsigned int    bF4                  : 1;
         unsigned int    bF3                  : 1;
         unsigned int    bF2                  : 1;
         unsigned int    bF1                  : 1;
         unsigned int     nRegulateAzimuth     : 10;          //!< юстировочная поправка по пеленгу
         unsigned int     nRegulateElevation   :  7;          //!< юстировочная поправка по углу места
         unsigned int     nPorogUpwater        :  3;          //!< порог отображения надводной цели
         unsigned int     nRegulateDistance    : 12;          //!< юстировочная поправка по дальности
         unsigned int     nAzimuthZI11         : 14;          //!< информация по секторам запрета излучени
         unsigned int     nAzimuthZI21         : 14;
         unsigned int     nElevetionZI1        : 4;
         unsigned int     nAzimuthZI12         : 14;          //!< информация по секторам запрета излучени
         unsigned int     nAzimuthZI22         : 14;
         unsigned int     nElevetionZI2        : 4;
         unsigned int     nAzimuthZI13         : 14;
         unsigned int     nAzimuthZI23         : 14;
         unsigned int     nElevetionZI3        : 4;
         unsigned int     nAzimuthZI14         : 14;
         unsigned int     nAzimuthZI24         : 14;
         unsigned int     nElevetionZI4        : 4;
         unsigned int     nAzimuthZI15         : 14;
         unsigned int     nAzimuthZI25         : 14;
         unsigned int     nElevetionZI5        : 4;

         unsigned int     nAzimuthZAZ11        : 16;      //!< информация по секторам заз
         unsigned int     nAzimuthZAZ21        : 16;
         unsigned int     nDistanceZAZ11       : 16;
         unsigned int     nDistanceZAZ21       : 16;
         unsigned int     nAzimuthZAZ12        : 16;
         unsigned int     nAzimuthZAZ22        : 16;
         unsigned int     nDistanceZAZ12       : 16;
         unsigned int     nDistanceZAZ22       : 16;
         unsigned int     nAzimuthZAZ13        : 16;
         unsigned int     nAzimuthZAZ23        : 16;
         unsigned int     nDistanceZAZ13       : 16;
         unsigned int     nDistanceZAZ23       : 16;
         unsigned int     nAzimuthZAZ14        : 16;
         unsigned int     nAzimuthZAZ24        : 16;
         unsigned int     nDistanceZAZ14       : 16;
         unsigned int     nDistanceZAZ24       : 16;
         unsigned int     nAzimuthZAZ15        : 16;
         unsigned int     nAzimuthZAZ25        : 16;
         unsigned int     nDistanceZAZ15       : 16;
         unsigned int     nDistanceZAZ25       : 16;

         unsigned int     nAzimuthNoise11      : 9;          //!< информация по секторам СПЗ, сектора ЗАЗ при перезапуске не восстанавливаютс
         unsigned int     nAzimuthNoise21      : 9;
         unsigned int     nNumberTreshold1     : 4;
         unsigned int    bThreshold1          : 1;
         unsigned int    bVARU1               : 1;
         unsigned int    bKgK1                : 1;
         unsigned int    bAMDF1               : 1;
         unsigned int    bMO1                 : 1;
         unsigned int    bVOGR1               : 1;
         unsigned int    bAKP1                : 1;
         unsigned int    bNumberVOGR1         : 3;
         unsigned int     nNoiseDistance11     : 14;
         unsigned int     nNoiseDistance21     : 14;
         unsigned int    bReserved4           : 1;
         unsigned int    bNoiseElevetion1     : 3;

         unsigned int     nAzimuthNoise12      : 9;          //!<
         unsigned int     nAzimuthNoise22      : 9;
         unsigned int     nNumberTreshold2     : 4;
         unsigned int    bThreshold2          : 1;
         unsigned int    bVARU2               : 1;
         unsigned int    bKgK2                : 1;
         unsigned int    bAMDF2               : 1;
         unsigned int    bMO2                 : 1;
         unsigned int    bVOGR2               : 1;
         unsigned int    bAKP2                : 1;
         unsigned int    bNumberVOGR2         : 3;
         unsigned int     nNoiseDistance12     : 14;
         unsigned int     nNoiseDistance22     : 14;
         unsigned int    bReserved12           : 1;
         unsigned int    bNoiseElevetion2     : 3;

         unsigned int     nAzimuthNoise13      : 9;          //!<
         unsigned int     nAzimuthNoise23      : 9;
         unsigned int     nNumberTreshold3     : 4;
         unsigned int    bThreshold3          : 1;
         unsigned int    bVARU3               : 1;
         unsigned int    bKgK3                : 1;
         unsigned int    bAMDF3               : 1;
         unsigned int    bMO3                 : 1;
         unsigned int    bVOGR3               : 1;
         unsigned int    bAKP3                : 1;
         unsigned int    bNumberVOGR3         : 3;
         unsigned int     nNoiseDistance13     : 14;
         unsigned int     nNoiseDistance23     : 14;
         unsigned int    bReserved13           : 1;
         unsigned int    bNoiseElevetion3     : 3;

         unsigned int     nAzimuthNoise14      : 9;          //!<
         unsigned int     nAzimuthNoise24      : 9;
         unsigned int     nNumberTreshold4     : 4;
         unsigned int    bThreshold4          : 1;
         unsigned int    bVARU4               : 1;
         unsigned int    bKgK4                : 1;
         unsigned int    bAMDF4               : 1;
         unsigned int    bMO4                 : 1;
         unsigned int    bVOGR4               : 1;
         unsigned int    bAKP4                : 1;
         unsigned int    bNumberVOGR4         : 3;
         unsigned int     nNoiseDistance14     : 14;
         unsigned int     nNoiseDistance24     : 14;
         unsigned int    bReserved14           : 1;
         unsigned int    bNoiseElevetion4     : 3;

         unsigned int     nAzimuthNoise15      : 9;          //!<
         unsigned int     nAzimuthNoise25      : 9;
         unsigned int     nNumberTreshold5     : 4;
         unsigned int    bThreshold5          : 1;
         unsigned int    bVARU5               : 1;
         unsigned int    bKgK5                : 1;
         unsigned int    bAMDF5               : 1;
         unsigned int    bMO5                 : 1;
         unsigned int    bVOGR5               : 1;
         unsigned int    bAKP5                : 1;
         unsigned int    bNumberVOGR5         : 3;
         unsigned int     nNoiseDistance15     : 14;
         unsigned int     nNoiseDistance25     : 14;
         unsigned int    bReserved15           : 1;
         unsigned int    bNoiseElevetion5     : 3;

         unsigned int     nAzimuthNoise16      : 9;          //!<
         unsigned int     nAzimuthNoise26      : 9;
         unsigned int     nNumberTreshold6     : 4;
         unsigned int    bThreshold6          : 1;
         unsigned int    bVARU6               : 1;
         unsigned int    bKgK6                : 1;
         unsigned int    bAMDF6               : 1;
         unsigned int    bMO6                 : 1;
         unsigned int    bVOGR6               : 1;
         unsigned int    bAKP6                : 1;
         unsigned int    bNumberVOGR6         : 3;
         unsigned int     nNoiseDistance16     : 14;
         unsigned int     nNoiseDistance26     : 14;
         unsigned int    bReserved16           : 1;
         unsigned int    bNoiseElevetion6     : 3;

         unsigned int     nAzimuthNoise17      : 9;          //!<
         unsigned int     nAzimuthNoise27      : 9;
         unsigned int     nNumberTreshold7     : 4;
         unsigned int    bThreshold7          : 1;
         unsigned int    bVARU7               : 1;
         unsigned int    bKgK7                : 1;
         unsigned int    bAMDF7               : 1;
         unsigned int    bMO7                 : 1;
         unsigned int    bVOGR7               : 1;
         unsigned int    bAKP7                : 1;
         unsigned int    bNumberVOGR7         : 3;
         unsigned int     nNoiseDistance17     : 14;
         unsigned int     nNoiseDistance27     : 14;
         unsigned int    bReserved17           : 1;
         unsigned int    bNoiseElevetion7     : 3;

         unsigned int     nAzimuthNoise18      : 9;          //!<
         unsigned int     nAzimuthNoise28      : 9;
         unsigned int     nNumberTreshold8     : 4;
         unsigned int    bThreshold8          : 1;
         unsigned int    bVARU8               : 1;
         unsigned int    bKgK8                : 1;
         unsigned int    bAMDF8               : 1;
         unsigned int    bMO8                 : 1;
         unsigned int    bVOGR8               : 1;
         unsigned int    bAKP8                : 1;
         unsigned int    bNumberVOGR8         : 3;
         unsigned int     nNoiseDistance18     : 14;
         unsigned int     nNoiseDistance28     : 14;
         unsigned int    bReserved18           : 1;
         unsigned int    bNoiseElevetion8     : 3;

         unsigned int     nAzimuthNoise19      : 9;          //!<
         unsigned int     nAzimuthNoise29      : 9;
         unsigned int     nNumberTreshold9     : 4;
         unsigned int    bThreshold9          : 1;
         unsigned int    bVARU9               : 1;
         unsigned int    bKgK9                : 1;
         unsigned int    bAMDF9               : 1;
         unsigned int    bMO9                 : 1;
         unsigned int    bVOGR9               : 1;
         unsigned int    bAKP9                : 1;
         unsigned int    bNumberVOGR9         : 3;
         unsigned int     nNoiseDistance19     : 14;
         unsigned int     nNoiseDistance29     : 14;
         unsigned int    bReserved19           : 1;
         unsigned int    bNoiseElevetion9     : 3;

         unsigned int     nAzimuthNoise110      : 9;          //!<
         unsigned int     nAzimuthNoise210      : 9;
         unsigned int     nNumberTreshold10     : 4;
         unsigned int    bThreshold10          : 1;
         unsigned int    bVARU10               : 1;
         unsigned int    bKgK10                : 1;
         unsigned int    bAMDF10               : 1;
         unsigned int    bMO10                 : 1;
         unsigned int    bVOGR10               : 1;
         unsigned int    bAKP10                : 1;
         unsigned int    bNumberVOGR10         : 3;
         unsigned int     nNoiseDistance110     : 14;
         unsigned int     nNoiseDistance210     : 14;
         unsigned int    bReserved110           : 1;
         unsigned int    bNoiseElevetion10     : 3;

         unsigned int     nAzimuthNoise111      : 9;          //!<
         unsigned int     nAzimuthNoise211      : 9;
         unsigned int     nNumberTreshold11     : 4;
         unsigned int    bThreshold11          : 1;
         unsigned int    bVARU11               : 1;
         unsigned int    bKgK11                : 1;
         unsigned int    bAMDF11               : 1;
         unsigned int    bMO11                 : 1;
         unsigned int    bVOGR11               : 1;
         unsigned int    bAKP11                : 1;
         unsigned int    bNumberVOGR11         : 3;
         unsigned int     nNoiseDistance111     : 14;
         unsigned int     nNoiseDistance211     : 14;
         unsigned int    bReserved111           : 1;
         unsigned int    bNoiseElevetion11     : 3;

         unsigned int     nAzimuthNoise112      : 9;          //!<
         unsigned int     nAzimuthNoise212      : 9;
         unsigned int     nNumberTreshold12     : 4;
         unsigned int    bThreshold12          : 1;
         unsigned int    bVARU12               : 1;
         unsigned int    bKgK12                : 1;
         unsigned int    bAMDF12               : 1;
         unsigned int    bMO12                 : 1;
         unsigned int    bVOGR12               : 1;
         unsigned int    bAKP12                : 1;
         unsigned int    bNumberVOGR12         : 3;
         unsigned int     nNoiseDistance112     : 14;
         unsigned int     nNoiseDistance212     : 14;
         unsigned int    bReserved112           : 1;
         unsigned int    bNoiseElevetion12     : 3;

         unsigned int     nAzimuthNoise113      : 9;          //!<
         unsigned int     nAzimuthNoise213      : 9;
         unsigned int     nNumberTreshold13     : 4;
         unsigned int    bThreshold13          : 1;
         unsigned int    bVARU13               : 1;
         unsigned int    bKgK13                : 1;
         unsigned int    bAMDF13               : 1;
         unsigned int    bMO13                 : 1;
         unsigned int    bVOGR13               : 1;
         unsigned int    bAKP13                : 1;
         unsigned int    bNumberVOGR13         : 3;
         unsigned int     nNoiseDistance113     : 14;
         unsigned int     nNoiseDistance213     : 14;
         unsigned int    bReserved113           : 1;
         unsigned int    bNoiseElevetion13     : 3;

         unsigned int     nAzimuthNoise114      : 9;          //!<
         unsigned int     nAzimuthNoise214      : 9;
         unsigned int     nNumberTreshold14     : 4;
         unsigned int    bThreshold14          : 1;
         unsigned int    bVARU14               : 1;
         unsigned int    bKgK14                : 1;
         unsigned int    bAMDF14               : 1;
         unsigned int    bMO14                 : 1;
         unsigned int    bVOGR14               : 1;
         unsigned int    bAKP14                : 1;
         unsigned int    bNumberVOGR14         : 3;
         unsigned int     nNoiseDistance114     : 14;
         unsigned int     nNoiseDistance214     : 14;
         unsigned int    bReserved114           : 1;
         unsigned int    bNoiseElevetion14     : 3;

         unsigned int     nAzimuthNoise115      : 9;          //!<
         unsigned int     nAzimuthNoise215      : 9;
         unsigned int     nNumberTreshold15     : 4;
         unsigned int    bThreshold15          : 1;
         unsigned int    bVARU15               : 1;
         unsigned int    bKgK15                : 1;
         unsigned int    bAMDF15               : 1;
         unsigned int    bMO15                 : 1;
         unsigned int    bVOGR15               : 1;
         unsigned int    bAKP15                : 1;
         unsigned int    bNumberVOGR15         : 3;
         unsigned int     nNoiseDistance115     : 14;
         unsigned int     nNoiseDistance215     : 14;
         unsigned int    bReserved115           : 1;
         unsigned int    bNoiseElevetion15     : 3;

         unsigned int    nReserved111          : 32;          //!<

         unsigned int     nDistanceKgK         : 14;
         unsigned int     nDistanceVARU        : 12;
         unsigned int    bReserved2           : 3;
         unsigned int    bMDFA                : 1;
         unsigned int    bReserved3           : 2;       	
       	
       	
} DocCODOGRAMM_GENERAL_MODES_STRUCT;

typedef struct
{
        unsigned char nType;    // CODGRAMM_KKSUSRMOPVO( 60)
        unsigned int    bRmo2Suitable 	: 1;
        unsigned int    bPvoCombat    	: 1;
        unsigned int    bPvoTrain     	: 1;
        unsigned int    bReserve1     	: 1;
        unsigned int    bKKSSuitable  	: 1;
        unsigned int    bKKSReady     	: 1;
        unsigned int    bReserve2     	: 1;
        unsigned int    bBosConnected 	: 1;
        unsigned int    bBosNav     	: 1;
        unsigned int    bPavConnected  	: 1;
        unsigned int    Reserve4     	: 2;
        unsigned int    b5P1Suitable 	: 1;
        unsigned int    b5P2Suitable 	: 1;
        unsigned int    bMDVVSuitable 	: 1;
        unsigned int    bCombatI     	: 1;        
        unsigned int 	Reserve6        : 8;
        unsigned int    temp5P1  		: 8;
        unsigned int    temp5P2 		: 8;        
        unsigned int    Reserve7 		: 8;
        unsigned int    Reserve8 		: 8;
        
}DocCODGRAMM_KKSUSRMOPVO_STRUCT;


typedef struct  // тип, описывающий состояние 5П-27М
{
    unsigned int    bCombatI     	: 1;
    unsigned int    stateAutonom   	: 2;  // 0-ЦР, 1-РР, 2-АР
    unsigned int    bPvoCombat    	: 1;
    unsigned int    bPvoTrain     	: 1;
    unsigned int    bAutoCr     	: 1;
    unsigned int    vAntenna     	: 6;//0,3,6,30
    unsigned int    bHigh     		: 1;
    unsigned int    b67     		: 1;
    unsigned int    bAutoGo     	: 1;
   unsigned int    	bBosUprConnected: 1;
   unsigned int    	RO     			: 4;   
   unsigned int    	bBosADUConnected: 1;
   unsigned int    	bBosPOIConnected: 1;
   unsigned int    	bBosVOIConnected: 1;   
   unsigned int    	bBosFDKConnected: 1;   
   unsigned int    	bBosREGConnected: 1;   
   unsigned int    	bBosNavConnected: 1;
   unsigned int    	bPavConnected  	: 1;
   unsigned int    	reserve0  		: 5;
   unsigned char   	reserve1[8];
} DocMainDoc;	



typedef struct
{
        unsigned char nType;    // PAV_OK( 51)
        unsigned int    bRotarySpeed_60     : 1;           //!< Скорость вращения АПУ 60 об/мин
		unsigned int    bRotarySpeed_30     : 1;           //!< Скорость вращения АПУ 30 об/мин
		unsigned int    bRotarySpeed_3      : 1;           //!< Скорость вращения АПУ 3 об/мин
		unsigned int    bRotarySpeed_6      : 1;           //!< Скорость вращения АПУ 6 об/мин
		unsigned int    bStop               : 1;           //!< Стоп
		unsigned int    bReserve1           : 1;           // резерв
		unsigned int    bAvar               : 1;           //!< Аварийный стоп
		unsigned int    bReserve2           : 1;           // резерв
		unsigned int    bPVK              	: 1;           // признак выполнения команды
		unsigned int    bIK          		: 1;		   // команда исполнена
		unsigned int    bReady         		: 1;		   // Готовность ПАВ
		unsigned int    bErrOper           	: 1;		   //ошибка оператора 
		unsigned int    bHand           	: 1;		   //ручное управление механизмами ПАВ
		unsigned int    bBlVrash           	: 1;		   //Включение блокировки вращения
		unsigned int    bOtkCp           	: 1;		   //отказ центрального процессора
		unsigned int    bSKO           		: 1;		   //Сбой канала обмена
		unsigned int    bPBU           : 1;		   //Признак блокировки управления ПАВ
		unsigned int    bOtkBuPav           : 1;		   //отказ блока управления в части ПАВ
		unsigned int    bOtkMehPav           : 1;		   //отказ механизма ПАВ
		unsigned int    bVklGrupB           : 1;		   //Включено питание групп, выпрямит
		unsigned int    bOtk13           : 1;		   //Отказ вентил. 1... 3 теплообменника
		unsigned int    bVklVum           : 1;		   //Включено +27 В ВУМ
		unsigned int    bOtk47           : 1;		   //Отказ вентил. 4... 7 теплообменника
		unsigned int    bOtkVent           : 1;		   //Отказ вентиляторов контейнера
		unsigned int    bVklPav           : 1;		   //Включен ВИП ПАВ по цепи +27 В
		unsigned int    bVklVent           : 1;		   //Включение вентиляторов контейнера
		unsigned int    bVklBos           : 1;		   //Включено электропитание БОС
		unsigned int    bVklVip           : 1;		   //Включено электропитание ВИП
		unsigned int    bOhlVum1           : 1;		   //Включено охлаждение ВУМ1
		unsigned int    bOhlVum2           : 1;		   //Включено охлаждение ВУМ2
		unsigned int    bReserve3		   : 2;        // 
        unsigned char   char0;
        unsigned short short1;
        
}DocPAV_OK_STRUCT;

typedef struct{ // заголовок сообщения
		unsigned int nType	: 16;    // тип сообщения
        unsigned int iMes 	: 16;    // циклический номер сообщения
        unsigned int timeUs_s; 		 // время уса от 1 янв 1970, секунды 
        unsigned int timeUs_mks; 	 // время уса от начала секунды, мкс
        unsigned int idSender : 16; // id получателя
        unsigned int idReceiver : 16; // id отправителя, 1-СУ, 11-РЛС-Х, 12-5П27
        unsigned int numSegments : 8; // количество сегментов в сообщение, всегда 1        
        unsigned int pad0 : 8; //
        unsigned int size_Inf : 16; // размер информационной части сообщения
        
} DocHeader_su;

typedef struct
{ // команда на допоиск в заданном стробе
	//COMMANDX_STROB_CODE 5
    	unsigned int iRequest	: 16;   // номер запроса
        unsigned int strobPel 	: 16;	// строб по пеленгу 0..360, 180/2,15
        unsigned int pad0		: 32;	// ТЭ - время измерения или эквтраполяции, время 5П27 с начала суток, мс
        unsigned int strobUm 	: 16;	// строб по УМ -90..90, 180/2,16
        unsigned int strobDist 	: 16;	// строб по дальности 0..250km, 10m
        unsigned int peleng 	: 16;	// пеленг 0..360, 180/2,15
        unsigned int pad1		: 16;	//
        unsigned int um 		: 16;	// УМ -90..90, 180/2,16
        unsigned int distance 	: 16;	// дальность 0..250km, 10m
} DocCommandX_Strob;
typedef struct
{
	// команда на изменение приоритета сопровождения 
	// COMMANDX_CHANGEPRI_CODE 7	
		unsigned int iRequest	: 16;   // номер запроса
		unsigned int pad0		: 16;	//
        unsigned long pad1		;		// время выдачи запроса 5П27 с начала суток, мс
		unsigned int pad2		: 8;	//
		unsigned int pad3		: 8;	// 
		unsigned int nX			: 9;	// номер цели в нумерации X
		unsigned int pad4		: 2;	// 
		unsigned int PR			: 2;	// 0-отмена ЦУ 2-АС 3-ПС
		unsigned int pad5		: 3;	//
        unsigned int pad111[6];
} DocCommandX_ChangePri;

typedef struct
 { // ТС=8 результат обработки в РЛС-Х команд ТС=5, ТС=71, ТС=7 - или сброс цели
	 // RESULT_COMMANDX_CODE 8
	   unsigned int iRequest	: 16;   // номер запроса
   unsigned int pad0		: 5;
   unsigned int error		: 4;	// // код ошибки, выдается только при РО=0, 0-нет инфор, 1-отсутств связи с Х, 2-неиспр Х,
   									// 3-отсутствие тех возм 4-наруш приоритета, невозможность выполнения "сброс ЦУ"
   unsigned int RO			: 2;    // 0-запрос отклонен 1-ИС05-есть захват Nц целей 2-цель сброшена 3-цель  отсутствует (ИС05)
   unsigned int numberTargets :5;   // количество целей в стробе 0-10
   unsigned short nX[10]	;       // номера целей в заданном стробе
 } DocResult_CommandX;
 

	typedef struct{
		unsigned short		nType;		//< идентификатор пакета 2000
		unsigned short		Ns;			///< № сообщения
		unsigned short		RTrr1;		///< разрешенные рабочие точки
		unsigned short		RTrr2;		///< разрешенные рабочие точки
		unsigned short		RTrk1;		///< рекомендованные рабочие точки
		unsigned short		RTrk2;		///< рекомендованные рабочие точки
		unsigned short		NSzi;		///  < начало границы сектора ЗИ 0.1 гр
		unsigned short		OSzi;		//  < окончание границы сектора ЗИ  0.1 гр
		unsigned short		HiOff;		//  < команда "ЗИ"
		unsigned short		CRC;		//
	}DocEMS_SU;
	
   	
  	typedef struct{
   		unsigned short		nType;		//< идентификатор пакета 2001
   		unsigned short		Ns;			///< № сообщения
  		unsigned short		PKS;		//1 - исправно;2 - работ 3 - неисправно
   		unsigned short		RR;// Режим работы  с ЭМС 1-ручн, 2-ПА, 3-автомат
   		unsigned short		EKV;		// < признак "Эквивалент":
   		unsigned short		Prt;		// < признак подтверждения установки рабочих точек:
 //  	0 - РРТ не установлены;1 - РРТ установлены;2 - РРТ устанавливаются;	3 - отказ от приема (признак невозможности принятия команды на установку РРТ). */
   		unsigned short		RTrr1;		///< разрешенные рабочие точки
   		unsigned short		RTrr2;		///< разрешенные рабочие точки
   		unsigned short		RTip1;		///< используемые рабочие точки
   		unsigned short		RTip2;		///< используемые рабочие точки
   		unsigned short		Pzs;		// < признак подтверждения установки границ сектора запрета излучения:
 //  0 - СЗИ не установлены;1 - устанавливаются;2 - отказ от приема. 
   		unsigned short		SecStart;	///  < начало границы сектора запрета излучения
   		unsigned short		SecEnd;		///  < окончание границы сектора запрета излучения
   		unsigned short		HiOffSet;	///  прзнак выполнения команды запрет ЗИ
   		unsigned short		CRC;		// < контрольная сумма
    	}DocEMS_FURKE;
         
         typedef struct{ 
         	// тип, описывающий координатную информацию цели СУ ИП1 nType = 1
        	// TARGETX_CODE 1 
            unsigned int nn 		:16; 		 // номер объекта в нумерации источника
            unsigned int sSource	:8; 	 // тип источника информации 1-СУ 11 РЛС-Х
            unsigned int typeTarget	:8; 	 // тип цели
            unsigned int pri		:8; 	 //приоритет сопровождения цели 1-СНП, 2-пр1(АС), 3-пр2(ПС) 4-пр3(СС)
            unsigned int pad0		:8; 	 // 
            unsigned int bnonImit	:2; 	 // признак неимитированной цели, 1-реальная цель 2-ручн
            unsigned int GO 		:3; 	 // ГО, 0-неопозн, 1-чужой, 2-свой
            unsigned int bComplex	:1; 	 // признак комплексирования
            unsigned int SKK 		:1; 	 // 0-система координат - сферическая 1-геодезическая
            unsigned int SKS 		:1; 	 // 0-скорость в сферических 1-в прямоугольных коорд
            unsigned int bMonevrHor	:1;  // признак меневра по курсу
            unsigned int bMonevrV	:1; 	 // признак меневра по скорости
            unsigned int bMonevrH	:1; 	 // признак меневра по высоте
            unsigned int bTime_Pr	:1; 	 // время выработки данных 0-на мом обнаруж, 1-на время выдачи
            unsigned int pad1		:4; 	 // 
            
            unsigned int 	timeSu_s	:32; 		 // время выработки данных, СУ от 1 янв 1970, секунды 
            unsigned int 	timeSu_mks	:32; 	 // время выработки данных, СУ от начала с, мкс

         	unsigned int	bPeleng			:1;	// наличие данных по пеленгу(долготе)
         	unsigned int	bDistance		:1;	// наличие данных по дальности (высоте)
         	unsigned int	bUm				:1;	// наличие данных по УМ (широте)
         	unsigned int	bVx				:1;	// наличие данных об изменении пеленга (Vx) 
         	unsigned int	bVy				:1;	// наличие данных об изменении дистанции (Vy) 
         	unsigned int	bVz				:1;	// наличие данных об изменении УМ (Vz) 
         	unsigned int	bskoPeleng		:1;	// наличие данных СКО по пеленгу(долготе)
         	unsigned int	bskoDistance	:1;	// наличие данных СКО по дальности (высоте)
         	unsigned int	bskoUm			:1;	// наличие данных СКО по УМ (широте)
         	unsigned int	bskoVx			:1;	// наличие данных СКО об изменении пеленга (Vx) 
         	unsigned int	bskoVy			:1;	// наличие данных СКО об изменении дистанции (Vy) 
         	unsigned int	bskoVz			:1;	// наличие данных СКО об изменении УМ (Vz)
         	unsigned int 	pad3			:4; 
         	
         	unsigned int	peleng_low		:16;	// пеленг(долгота)  360/pow(2,32) (180/pow(2,31))
         	unsigned int	peleng_hi		:16;	// пеленг(долгота)  360/pow(2,32) (180/pow(2,31))
         	unsigned int	um_low			:16;	// УМ (широта) 180/pow(2,31)
         	unsigned int	um_hi			:16;	// УМ (широта) 180/pow(2,31)
         	unsigned int	distance_low		:16;	// дальность (высота) 0,05m
         	unsigned int	distance_hi	:16;	// дальность (высота) 0,05m
         	unsigned int	skoPeleng_low	:16;	// СКО пеленга(долготы)180/pow(2,32)
         	unsigned int	skoPeleng_hi	:16;	// СКО пеленга(долготы)180/pow(2,32)
         	unsigned int	skoUm_low		:16;	// СКО УМ (широты) 180/pow(2,32)
         	unsigned int	skoUm_hi		:16;	// СКО УМ (широты) 180/pow(2,32)
         	unsigned int	skoDistance	:16;	// СКО дальности (высоты) 0,5м
         	unsigned int	Vx			:16;	// изменение пеленга (Vx)  0,01град/с (0,25м/с) север
         	unsigned int	Vy			:16;	// изменение дистанции (Vy) 0,01град/с (0,25м/с)
         	unsigned int	Vz			:16;	// изменение УМ (Vz)  0,5град/с (0,25м/с)
         	unsigned int	skoVx		:16;	// СКО изменения пеленга (Vx) 0,01град/с (0,01м/с)
         	unsigned int	skoVy		:16;	// СКО изменения дистанции (Vy) 0,5м/с (0,25м/с) 
         	unsigned int	skoVz		:16;	// СКО изменения УМ (Vz) 0,01град/с (0,01м/с)
          } DocTargetX_m;
          
          typedef struct{ 
          	// тип, описывающий координатную информацию цели СУ ИП1 nType = 1
             unsigned short nn; 		 // номер объекта в нумерации источника
             unsigned char sSource; 	 // тип источника информации 1-СУ 11 РЛС-Х
             unsigned char SKK; 	 // 0-система координат - сферическая 1-геодезическая
             unsigned char SKS; 	 // 0-скорость в сферических 1-в прямоугольных коорд
             unsigned char bMonevrHor;  // признак меневра по курсу
             unsigned char bMonevrV; 	 // признак меневра по скорости
             unsigned char bMonevrH; 	 // признак меневра по высоте
             double timeSu_d; 		 // время выработки данных, СУ от 1 янв 1970, секунды
          	unsigned char	bPeleng;	// наличие данных по пеленгу(долготе)
          	unsigned char	bDistance;	// наличие данных по дальности (высоте)
          	unsigned char	bUm;	// наличие данных по УМ (широте)
          	unsigned char	bVx;	// наличие данных об изменении пеленга (Vx)
          	unsigned char	bVy;	// наличие данных об изменении дистанции (Vy) 
          	unsigned char	bVz;	// наличие данных об изменении УМ (Vz) 
          	
          	unsigned char	bskoPeleng;	// наличие данных СКО по пеленгу(долготе)
          	unsigned char	bskoDistance;	// наличие данных СКО по дальности (высоте)
          	unsigned char	bskoUm;	// наличие данных СКО по УМ (широте)
          	unsigned char	bskoVx;	// наличие данных СКО об изменении пеленга (Vx) 
          	unsigned char	bskoVy;	// наличие данных СКО об изменении дистанции (Vy) 
          	unsigned char	bskoVz;	// наличие данных СКО об изменении УМ (Vz)
          	float peleng;	// пеленг(долгота) рад/град
          	float um;	// УМ (широта) рад/град
          	float distance;	// дальность (высота), m
          	float skoPeleng;	// СКО пеленга(долготы)рад/град
          	float skoUm;	// СКО УМ (широты) 180/pow(2,32)
          	float skoDistance;	// СКО дальности (высоты)м
          	float Vx;	// изменение пеленга (Vx)  рад/с (м/с) север
          	float Vz;	// изменение УМ (Vz)  рад/с (м/с) 
          	float Vy;	// изменение дистанции (Vy) град/с (м/с) восток
          	float skoVx;	// СКО изменения пеленга (Vx) рад/с (м/с)
          	float skoVy;	// СКО изменения дистанции (Vy) м/с 
          	float skoVz;	// СКО изменения УМ (Vz) рад/с (м/с)
            unsigned char bnonImit;	// признак неимитированной цели, 1-реальная цель
            unsigned char GO; 	 	// ГО, 0-неопозн, 1-чужой, 2-свой
            unsigned char bComplex; // признак комплексирования  
            //unsigned char typeTarget; 	// тип цели
            unsigned char typeTarget; 	// тип цели -реально это такой же тип, как в трассах Ф, в конвертере делается преобразование
            unsigned char pri; 	 	// приоритет сопровождения цели, 1-на проходе, 2-при1, 3-при2, 4-при3
           } DocTargetX_f;     
           
           
           typedef struct
           {// DI_TARGETTELEVIK
                   unsigned int pad0	: 10;    // 
                   unsigned int bPNS 	: 1;// ПНС - признак неустойстового сопровождения
                   unsigned int bReset 	: 1;// СБР - признак сброса
                   unsigned int pad1	: 4;    // 
                   unsigned int typeTarget     : 4;// ТЦ - тип цели 0-возд неопр, 1-надв, 8-возд малор, 10-самолет, 
                   						// 11-ПКР, 12-ПРР, 13-ВЕРТ, 
                   unsigned int pad2	: 1;    // 
                   unsigned int bNew 	: 1;// прзнак новой трассы
                   unsigned int pad3	: 1;    // 
                   unsigned int bPriority 	: 1;// прзнак приоритетной цели
                   unsigned int nSource: 8;    // номер цели
                   unsigned int pad4;    // 
                   unsigned int t;		// ТЭ - время измерения или эквтраполяции
                   float x; // север, м
                   float y; // восток, м
                   float h; // зенит, м
                   float vx; // север, м
                   float vy; // восток, м
                   float vh; // зенит, м
           } DocTargetTelevik;
           
           typedef struct
           {// DI_TARGETSERVAL
       	 	unsigned int  dcu : 4;//доклад о работе по внешнему ЦУ 1-ЦУ принимается, 2-ЦУ принято, 3-отказ ЦНЗ
       	            // 4-отказ, недействительная, 5-отмена ЦУ ринята 
       	 	unsigned int  ppc : 2;// признак поражения цели 1-цель поражена
       	 	unsigned int naznyAU : 1;//1-веду огонь - на время замыкания педали
       	 	unsigned int fire : 1;//1-веду огонь - на время замыкания педали
       	 	unsigned int  rrc : 4; //0-нет работы,1- работа по ЦУ 2-АС 
               unsigned int pad0	: 4;    // 
       	 	unsigned int typeTarget     : 4;// ТЦ - тип цели 0-возд неопр, 1-надв, 8-возд малор, 10-самолет, 
                						// 11-ПКР, 12-ПРР, 13-ВЕРТ, 
               unsigned int pad1	: 4;    // 
       	 	unsigned int nTelevik: 8;    // номер цели
       	 	
                unsigned int nModul: 8;    // номер модуля (по которому отсчет координат) 1-10 или 0 
                unsigned int pad3	: 8;    // 
                unsigned int bku : 1; // наличие данных
                unsigned int bdistance: 1; // наличие данных
                unsigned int bum: 1; // наличие данных
                unsigned int bvx: 1; // наличие данных
                unsigned int bvy: 1; // наличие данных
                unsigned int bvh: 1; // наличие данных
                unsigned int pad4	: 2;    // 
                unsigned int nServal: 8;    // номер цели в нумерации Сервала
                unsigned int t;		// ТЭ - время измерения или эквтраполяции
                float ku; // рад
                float distance; //  м
                float um; // рад
                float vx; // нормаль, м, на приеме- пересчет
                float vy; // перпенд нормали м, на приеме- пересчет
                float vh; // зенит, м
           } DocTargetServal;
           
           typedef struct  // тип, описывающий состояние модуля Сервала
           {//
           	unsigned int stateComplex :8; 		// 0-нет ГС, 1 - вкл/откл ГС, 2 -ГС, нет упр, 3-ГС, автон, 4 - ГС, совм, ведущий, 5-ГС, совм, ведом
           	unsigned int error : 1; // 1 - сиcтема неиправна
           	unsigned int AC : 1; // 1 - автосопровождение
           	unsigned int CS : 1; // 1 - ЦС заблокированна
           	unsigned int readyAU : 1; // 1 - готовность АУ
           	unsigned int fire : 1; // 1 - ВО
           	unsigned int bBM : 1; // признак боевого модуля, иначе - оптика
           	unsigned int padO :10;//
           	unsigned int nServal :8;	// номер цели в нумерации Сервал
              float courseAngle;	// рад
              float um;	// рад
           	float xVynos;	// вынос по х
          	float yVynos;	// вынос по y
          	float hVynos;	// вынос по h
        	float deltaKU;	// поправка по курсовому углу
            float deltaUM;	// поправка по углу места
              float pelengMin;	// рад
              float pelengMax;	// рад
              float UMMin;	// рад
              float UMMax;	// рад
           } DocOneModulServal;

           typedef struct
           { 
                   float pelengMin;	// рад
                   float pelengMax;	// рад
                   float distanceMin;	// m
                   float distanceMax;	// m
                   unsigned int kodUM		: 8;	
                   unsigned int pad		: 24;	
           } DocSectorWithKodUM;
           
           typedef struct  
           { 
           	float z1;							// максимальная высота поражения, м
           	float z2;							// высота поражения, м
           	float z3;							// минимальная высота поражения, м
           	float d1;							// максимальная дистанция поражения, м
           	float d2;							// дистанция поражения, м
           	float d3;							// минимальная дистанция поражения, м
           	float courseParameterMaxPlane; 		// максимальный курсовой параметр по самолету, м
           	float courseParameterMaxPkr;		// максимальный курсовой параметр по ракете, м
           	float courseAngleMax;  				// абс.значение максимального курсового угла, рад
           } DocZoneZos; //- описание зоны поражения ЗОС

           
           typedef struct  // тип, описывающий состояние Сервала
           {//STATESERVAL_CODE 61
           	unsigned int stateComplex :8; 		// режим работы устройства 0-нет данных 1-автоном 2-совместн
           	unsigned int error : 1; // 1 - сиcтема неиправна
           	unsigned int padO :15;//
           	unsigned int numberModul :8;	// количество модулей (0..10)
           	DocOneModulServal modul[10];
           } DocStateServal;
           
           typedef struct  // тип, описывающий состояние Телевика
           {//STATETELEVIK_CODE 78
           	unsigned int bConnectPristavka; 	
           	unsigned int bConnectVoi;
           	unsigned int bRecord;
           	unsigned int nVar;	
           	unsigned int bAutoCr;
            unsigned int  bImitTarget;
            int indImitTarget; 
            unsigned int  bImitTargetPristavka;
            int indImitTargetPristavka; 
            unsigned int  speedAnt; // скорость вращения антенны об/мин
            unsigned int  bAccel; // признак разгона-торможения
        	unsigned int reserve[12];
           	unsigned int stateComplex; 		// режим функ-я  0-боевой 1-деж 2-реглам 3-функ кон 4-боевой и
           	unsigned int RO;				// режим обзора, 0-нет, 1-круговой, 2-в секторах
           	unsigned int numberTargets :8;	// количество сопровождаемых целей 0-100
           	float xVynosTelevik ;	// вынос по х
           	float yVynosTelevik ;	// вынос по y
           	float hVynosTelevik ;	// вынос по h
        	float deltaKU;	// вынос по KU
        	float deltaUM ;	// вынос по um
           	DocSectorWithKodUM workSector; //рабочий сектор Телевика
           	DocSectorWithKodUM targetSector; // сектор выдачи ИС15
           	DocSectorWithKodUM crSector;  //сектор выдачи ИС211
           	DocZoneZos zosSector;  //сектор выдачи ИС211
           } DocStateGlobalTelevik;
           
           
           typedef struct
           {// TARGETPRISTAVKA_CODE 80
               unsigned int pad0	: 10;    // 
               unsigned int bPNS 	: 1;// ПНС - признак неустойстового сопровождения
               unsigned int bReset : 1;// СБР - признак сброса
               unsigned int bValid	: 1;   // признак валидности данных (наличие скорости?) 
               unsigned int pad1	: 8;    // 
               unsigned int bNew 	: 1;// прзнак новой трассы
               unsigned int pad2	: 1;    // 
               unsigned int bPriority 	: 1;// прзнак приоритетной цели
               unsigned int pad3: 8;    // 
               unsigned int nSource: 16;    // номер цели
               unsigned int pad4: 16;    // 
               unsigned int time_ms_low; 		 // время уса от 1 янв 1970, милисекунды 
               unsigned int time_ms_hi; 		 // время уса от 1 янв 1970, милисекунды 
               
      //         unsigned long t;		// ТЭ - время измерения или экcтраполяции, ms
               float distance; // наклонная, м
               float peleng; // 
               float reserve0; // 
               float course; //
               float speed; // 
               float reserve1; // 
           } DocTargetPristavka;
           typedef struct
           { 
                   float pelengMin;	// рад
                   float pelengMax;	// рад
                   float umMin;	// рад
                   float umMax;	// рад
                   float distanceMin;	// m
                   float distanceMax;	// m
           } DocSectorWithUm; 


           typedef struct  // тип, описывающий состояние Приставки
           {////STATEPRISTAVKA_CODE 61
          		unsigned int stateComplex :8; 		// режим функ-я  0-боевой 1-деж 2-реглам 3-функ кон 4-боевой и
          		unsigned int pp :8; 		// пеленг поступает
          		unsigned int high :8; 		// скорость вращения антенны: 0,1,3,6,20,30,60 об/мин
          		unsigned int RO :8; 		// признак разгона/торможения
          		unsigned int pad0 : 32; //
          		DocSectorWithUm sector;
           } DocStatePristavka;
           typedef struct  // тип, описывающий состояние Истры
           {//STATEISTRA_CODE 78
           	unsigned int bConnectServal; 	
           	unsigned int bConnectVoi;
           	unsigned int bRecord;
           	unsigned int nVar;	
           	unsigned int bAutoCr;
               unsigned int  bImitTarget;
               int indImitTarget; 
           	unsigned int reserve[16];
           	unsigned int stateComplex; 		// режим функ-я  0-боевой 1-деж 2-реглам 3-функ кон 4-боевой и
           	unsigned int RO;				// режим обзора, 0-нет, 1-круговой, 2-в секторах
           	unsigned int numberTargets :8;	// количество сопровождаемых целей 0-100
           	float xVynosIstra ;	// вынос по х
           	float yVynosIstra ;	// вынос по y
           	float hVynosIstra ;	// вынос по h
           	float deltaKU;	// вынос по KU
           	float deltaUM ;	// вынос по um
           } DocStateGlobalIstra;           
           
           
           

           
	

char * getStrMsg(word msgCode, word parameter,char * str_parameter);
char * getStrNumberShootChannel(word numberShootChannel, int shipType);


int createDoc(void);
void startDoc(struct timespec timeStartDoc);
void stopDoc(void);
int saveDoc(char* savePath, char* saveDir2);
int testFile(char *s);

int getTypeSize(int type);
void sendDocMsg(unsigned char type, void *p); 
void parseDocMsg(unsigned char type, void *p, char* buff,int bParamPD,int bParamVr, int shipType);
void parseDocMsg2Txt(uint32_t time, uint8_t type, void *pMsg, char* buff,int bParamPD,int bParamVr, int shipType);

float docDo2pi(float value);
double docDecodeValue(unsigned short wrd , double LOB_worth, int sign, int size);

 int DocconvSuTypeTargetToObjType(int typeTarget);
 int convRead_DocTargetX(DocTargetX_f * targetX_f, DocTargetX_m * targetX_m);

#endif /*DOCSRUCTS_H_*/
