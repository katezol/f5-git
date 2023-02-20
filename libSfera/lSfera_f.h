#ifndef LSFERA_F_H_
#define LSFERA_F_H_

#include <stdbool.h>
#include "lSfera.h"

typedef struct{
	unsigned char		typeTarget		;//0-неопр, 1-надв, 2-возд, 3-берег
	unsigned char		typeCommand		;// 1- сектор поиска,  2-направление поиска, 3-целепоказ
	unsigned char		GO 				;//1-своя 2-чужая
	unsigned char		istCU 			;//1-RLK-s 2-РЛС-х 4-Сфера 5 МР-231 6-МР231-3
	unsigned short		nIstCU 			;// номер цели в нумерации источника ЦУ
	float		UMmin; 
	float		UMmax;
	float		pelengMin; 
	float		pelengMax; 
}fSfera_Ip1Modul_t;


typedef struct{
	unsigned char		OT			;	// признак отказа изделия 5П-27М:
						// 0 - нет отказа;
						// 1 - есть отказ.
	unsigned char		RU	 		;	// режим управления изд. ОЭС, установленный в изд 5П-27М:
						//01   -"Централизованный";
						//10   -"Резервный";
						//00,11-"Автономный".
	unsigned char		RR  		;	// режим работы:
						//01    - боевой режим;
						//10    - боевой режим по имитированной обстановке;
						//00,11 - режим контроля
	unsigned char		numberIP3	;// количество ИП3
	fSfera_Ip1Modul_t tgt[SFERA_MODUL_NUM];
}fSfera_Ip1_t;

//////////////////////////////////////////////////////////////////////
// 5П-27М --> IP2 --> ОЭС "Сфера-05"//
// Формуляр сопровождения;
// Задание сопровождения целей производится при каждом цикле обмена в течение всего времени сопровождения заданных целей.

typedef struct{
	unsigned short		typeTarget;//0-неопр, 1-надв, 2-возд, 3-берег 4-верт
	unsigned short		naznCU;//1-в интересах МУС, 2-в интересах ЗРК, 0,3-неопр
	unsigned short		GO;//1-своя 2-чужая
	unsigned short		istCU;//1-RLK-s 2-РЛС-х 4-Сфера 5 МР-231 6-МР231-3
	unsigned short		nIstCU;// номер цели в нумерации источника ЦУ
	float	peleng;//рад 
	float	UM; //рад
	float	distance;//рад
}fSfera_Ip2Tgt;

///////////////////////////////////////////////////
// 5П-27М --> IP3 --> ОЭС "Сфера-05"//
// Формуляр результатов ГО;

typedef struct{
	unsigned short		process;// 0-ГО не произв, 1-производится, 2-результат
	unsigned short		GO;//0-неоп 1-своя 2-чужая
	unsigned short		nSfera;	// номер цели (1-255 или 0) в нумерации изд. ОЭС,
	float 		P;				// пеленг (курсовой угол) цели - диапазон 360град.
										// цена старшего разряда при передаче пеленгов (курсовых углов) цели 180град.
	float 		D;				// дальность. Цена СР 128 км
}fSfera_ResGO_t;



typedef struct{
	unsigned short		obm; // признак наличия связи с м.обзора
	unsigned short		tv; // признак наличия связи с тепловизором м.обзора
	unsigned short		o_oem; // признак исправности оптики м.обзора
	unsigned short		reg; // режим работы м.обзора 0-отказ 1-ручн 2-рабочий 3-контроль
	unsigned short		obzor; // признак выполнения обзора (занятости?)
	float		peleng; // рад бис обзора
	float		angleHor; // сектор обзора
	float		UM; //рад бис обзора
	float		angleVer; // сектор обзора
}fSfera_modulObn;

typedef struct{
	unsigned short		reg; // режим работы м.обзора 0-отказ 1-ручн 2-рабочий 3-контроль
	unsigned short		ld; // признак исправности дальномера
	unsigned short		numberSfera; // номер сопровождаемой цели
	unsigned short		bDistance; // признак измерения дистанции
}fSfera_modulSopr;

typedef struct{
	unsigned short		RU	 		:2;	// установленный режим управления изделием ОЭС:
						//01   -"Централизованный";
						//10   -"РР-ВЦ";
						//11   -"РР-НЦ";
						//00-"Автономный".
	unsigned short		pad0 		:1;
	unsigned short		RR  		:2;	// режим работы:
						//01    - боевой режим;
						//10    - боевой режим по имитированной обстановке;
						//11 - деж режим, в этом режиме ОЭС не отраб коанды
	unsigned short		pad1 		:11;

	unsigned short		KOC			:6; // количество обнаруженных целей (до 60),
										// информация о которых выдается изд. ОЭС в изд. 5П-27М в ИП7.
										// При КОЦ = 0 целевая информация отсутствует.
	unsigned short		KSC			:4; // количество сопровождаемых целей (до 8),
										// информация о которых выдается изд. ОЭС в изд. 5П-27М в ИП6.
										// При КСЦ = 0 целевая информация отсутствует.
	fSfera_modulObn     modulObn [SFERA_MODUL_NUM];
	fSfera_modulSopr     modulSopr [2*SFERA_MODUL_NUM];
}fSfera_Ip4_t;



//////////////////////////////////////////////////////////////////////
// ОЭС "Сфера-05" --> IP5 --> 5П-27М//
// Формуляр сопровождаемых целей;
// Максимальное число сопровождаемых целей - восемь.

typedef struct{
	unsigned short		nSfera;// номер цели в нумерации Cферы
	unsigned short		typeTarget;	// тип цели,установленный в изделии ОЭС:
						// 00 - тип не определен;
						// 01 - надводный;
						// 10 - воздушный;
						// 11 - береговой.
	unsigned short		naznCU;//1-в интересах МУС, 2-в интересах ЗРК, 0,3-неопр
	unsigned short		istCU;//1-RLK-s 2-РЛС-х 4-Сфера 5 МР-231 6-МР231-3
	unsigned short		GO;//1-своя 2-чужая
	
	unsigned short		nIstCU;// номер цели в нумерации источника ЦУ
	float		peleng;//рад 
	float		UM; //рад для НП-бис, для ЦП-УМ цели
	float		distance;//м
	float		vPeleng;//ВИП рад/сек
	float		vUM;//скорость изм УМ  рад/сек 
	float		vDistance;//ВИП м/c
}fSfera_Ip5Tgt;

//////////////////////////////////////////////////////////////////////
// ОЭС "Сфера-05" --> IP6 --> 5П-27М//
// Формуляр обнаруженных целей;
// Максимальное число обнаруженных целей - шестьдесят.
// Цели распределяются по подадресам, начиная с подадреса 0011, и занимают подадреса в порядке
// возрастания их номеров в зависимости от количества объектов.

typedef struct{
	unsigned short		nSfera;// номер цели в нумерации Cферы
	unsigned short		TIP;	// тип цели,установленный в изделии ОЭС:
						// 00 - тип не определен;
						// 01 - надводный;
						// 10 - воздушный;
						// 11 - береговой.
	float		P;				// рад
	float		UM;	   			// рак}fSfera_Ip6Tgt;
}fSfera_Ip6Tgt;

typedef struct{
	float		UM;				
	float		sectorHor;	   	
	float		sectorVer;	   	
	float		reserve[5];
}SferaUstForSektor;



/**
Преобразует  mSfera_Ip4_t в fSfera_Ip4
@param  *t_ip4   - структура mSfera_Ip4_t
@param  *t_data4 - структура fSfera_Ip4*/
//void conv_read_Sfera_IP4(mSfera_Ip4_t *t_ip4, fSferaа_Ip4    *t_data4);

//Преобразует  mSfera_Ip5Target_t в fSfera_Ip5Target_t
void conv_read_Sfera_IP5_oneTarget(mSfera_Ip5Target_t *mtgt_ip5,
		fSfera_Ip5Tgt   * ftgt_ip5);


//Преобразует  mSfera_Ip6Target_t в fSfera_Ip6Target_t
void conv_read_Sfera_IP6_OneTarget(mSfera_Ip6Target_t *mtgt_ip6, 
		fSfera_Ip6Tgt   * ftgt_ip6);

//Преобразует IP1_Sfera в fSfera_Ip1
void conv_write_Sfera_IP1(const fSfera_Ip1_t *t_data1, mSfera_Ip1_t *t_ip1);

//Преобразует IP2_Sfera в fSfera_Ip2
void conv_write_Sfera_IP2_OneTarget(const fSfera_Ip2Tgt *ftgt_ip2, mSfera_Ip2Target_t *mtgt_ip2);

//Преобразует IP3_Sfera в fSfera_Ip3
void conv_write_Sfera_IP3_OneTarget(const fSfera_ResGO_t *ftgt_ip3, mSfera_ResGO_t *mtgt_ip3);


#endif /* LSFERA_F_H_ */
