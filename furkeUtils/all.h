#ifndef ALL_H_
#define ALL_H_

#include <commonTypes.h>
#include <typesRls.h>
#define NALLCHANNELS  658  // для оптики отдельный канал не образуется 150+300+8
#define MAXTIMEXUSTALL  10 // максимальное время жизни отождествленной цели с нулевой нумерацией в X и S






typedef struct
{
	unsigned short nAll;                              	// номер отождествленной цели=indChannel+1 или 0 или 0
	unsigned char ExtrapolWithoutData;                  	// признак экстраполяции, появляется, когда данные от источника не обновляются,
    // а цель не сбросить, т.к. она в стр. канале
	unsigned char nFurke;                               	// номер цели в нумерации 5П-27М или 0
	unsigned short nOpt;                              	// номер цели в нумерации Sfera или 0
	unsigned short nX;                               		// номер цели в нумерации X или 0
    unsigned short bNew;									// признак завязки новой трассы
    unsigned char bPrc;									// приоритет сопровождения цели, 1-на проходе, 2-при1, 3-при2, 4-при3
    ObjType objType;						    // тип цели
    Own own;                                    // госпринадлежность цели
    float x;									// координата x, м или пеленг, если bPassive
    float y;									// координата y, м
    float z;									// координата z, м или ум, если bPassive
    float vx;				
    float vy;									// проекция скорости на y, м/с	
    float vz;									// проекция скорости на z или скорость изменения ум, м/с
    double tour;								// время наше внутреннее выработки последнего обноврения
} AllTarget;


/************* формуляр от  ЦУ_ЦР  к РМО-2 */
// Результат ПВО
typedef struct
 	{
 		int nAllSent;// имя канала на предыдущей посылке,
 		int nAll;// имя канала текущее, 0- признак готовности канала 		
 		 // несовпадение [indCvk].nRls!= nRls - причина отмены
 		int indCvk; // индекс в массиве CVK  текущий  или -1
 		unsigned char bInZone; // нахождение в зоне стрельбы
 		unsigned char stateTarget; // статус цели, который она имела при предыдущей отсылке,
 						  // исходящие команды
 						  // 1-ЦУ, 2-отмена,
 		unsigned char state; 		// Состояние обслуживания цели в стрельбовом канале, отклик по цели 
 							// 0-Отмена, нет ответа 1-ОТКАЗ      2-ЦУ_принимается  3-ЦУ Принято 
 			                // 4-ОГОНЬ  5-Поражение  6-Отмена Принята 		
 		float timeBeginCancel;// время выдачи отмены
 		float timeBeginCu;// время выдачи ЦУ 		
 	} Plan_CR_ALL;
 	
 	
 	
 	 typedef struct  // флаги необходимости отсылки информации абоненту
 	  	{
 		 	AllTarget allTargets[NALLCHANNELS];	
 	  		ICvkTarget icvk[NALLCHANNELS];
 	  	    float courseParameter[NALLCHANNELS];  					// курсовой параметр, м
 	  	    float flyTime[NALLCHANNELS];  							// подлетное время, с
 	  	    unsigned char RedutMeetMayZone[NALLCHANNELS];
 	  	    unsigned char ArtMeet100MayZone[NALLCHANNELS];
 	  	    unsigned char ArtMeet30_0_MayZone[NALLCHANNELS];
 	  	    unsigned char ArtMeet30_1_MayZone[NALLCHANNELS];
 	  		Plan_CR_ALL plan[8];
 	  		unsigned char bSuitable[8]; // готовность каналов к работе в резервном режиме  		
 	  	} PvoMapAll;

#endif /*ALL_H_*/
