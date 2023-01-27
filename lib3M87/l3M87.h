/*
 * l3M87.h
 *
 *  Created on: 18.03.2015
 *      Author: 1
 */

#ifndef L3M87_H_
#define L3M87_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <devctl.h>
#define _3M87_DEV_NAME	"/dev/_3M87"

#include <devctl.h>

// 5П-27М --> IP1 --> 3М87 (КМ)//
// Формуляр ЦУ.
// ПА = 1001.

typedef struct{
	unsigned short		Kk;			// курс корабля, цена ст.р. 180, частота обновления 1Гц

	unsigned short		RCU		:2;	// режим выдачи ЦУ, установленный в изделии 5П-27М
						// 00 - ЦР;
						// 01 - РР;
						// 10 - АР.
	unsigned short		CU		:1;	// ЦУ:
						// 1 - есть ЦУ;
						// 0 - нет  ЦУ.
	unsigned short		Izl		:1;
	unsigned short		RU		:2;	// РУ:
						// 00 - боевой;
						// 01 - боевой по имитированной обстановке;
						// 10 - контроль.
	unsigned short		RR		:3;	// режим работы изделия 5П-27М:
						// 000 - не определен;
						// 001 - режим 1. Скорость АП 60 об/мин;
						// 010 - режим 2. Скорость АП 30 об/мин;
						// 011 - режим 3. Скорость АП 3  об/мин;
						// 100 - режим 4. Скорость АП 6  об/мин.
	unsigned short		Ns		:1; // признак неисправности изделия 5П-27М:
						// 0 - изделие 5П-27М   исправно;
						// 1 - изделие 5П-27М неисправно.
	unsigned short		KFO		:6; // количество формуляров объектов ИП2, 0..20

	unsigned short		NO		:9;	// номер в нумерации 5П-27М (0..300)
	unsigned short		pad0	:1;	// 0
	unsigned short		Tip		:4; // тип цели:
						// 0 - не определен;
						// 1  - надводный;
						// 8  - воздушный (тип не определен);
						// 10 - самолет;
						// 11 - ПКР;
						// 12 - ПРР;
						// 13 - вертолет;
						// 15 - ВПЦ.
	unsigned short		GO		:2;	// признак госпринадлежности:
						// 00 - неопознанный;
						// 01 - чужой;
						// 10 - свой.

	unsigned short		PRNS	:3; // признак выдачи скоростей:
						// хх1 - Vx
						// х1х - Vy
						// 1хх - Vz
	unsigned short		PRNK	:3;	// признак выдачи координат:
						// хх1 - X
						// х1х - Y
						// 1хх - Z
	unsigned short		pad1	:8; // 0
	unsigned short		PCU		:1; // принять ЦУ, выдается до получения от КМ отказа от ЦУ или
									// до получения "ОП" объект поражен. Либо до выработки отмены "ОЦУ" изделием 5П-27М.
	unsigned short		OCU		:1; // отмена ЦУ, выдается до получения от КМ подтверждения отказа, но не более 1 сек.

	unsigned short		X;			// север , диапазон: -150км..+150км, ц.м.р. 5м

	unsigned short		Y;			// восток, диапазон: -150км..+150км, ц.м.р. 5м

	unsigned short		Z;			// горизонт, диапазон: 0..50км, ц.м.р. 1м

	unsigned short		tX;			// диапазон: 0..1000м, ц.м.р. 1м

	unsigned short		tY;			// диапазон: 0..1000м, ц.м.р. 1м

	unsigned short		tZ;			// диапазон: 0..1000м, ц.м.р. 1м

	unsigned short		tVx		:7;	// диапазон: 0..240м/с, ц.м.р. 5м/с
	unsigned short		Vx		:9; // диапазон: -1200м/с..+1200м/с, ц.м.р. 5м/с

	unsigned short		tVy		:7;	// диапазон: 0..240м/с, ц.м.р. 5м/с
	unsigned short		Vy		:9; // диапазон: -1200м/с..+1200м/с, ц.м.р. 5м/с

	unsigned short		tVz		:7;	// диапазон: 0..240м/с, ц.м.р. 5м/с
	unsigned short		Vz		:9; // диапазон: -1000м/с..+1000м/с, ц.м.р. 5м/с

}m3M87_Ip1_t;

// 5П-27М --> IP2 --> 3М87 (КМ)//
// Формуляр обстановки.
// ПА - (от 10010 до 11011) - всего 10 шт,передача до 20 целей.
// Информация выдается асинхронно, не реже одного раза за оборот антенны.
// Цели выдаются экстраполированными на момент выдачи.
// Информация выдается по 20-ти наиболее опасным целям в секторе БМ на
// дальности до 32 км относительно антенного поста, в т.ч. от других источников данных.

typedef struct{
//	unsigned short		pad0;		// 0

	unsigned short		NO		:9;	// номер в нумерации 5П-27М (0..300)
	unsigned short		pad1	:7;	// 0

	unsigned short		Tip		:4; // тип цели:
						// 0 - не определен;
						// 1  - надводный;
						// 8  - воздушный (тип не определен);
						// 10 - самолет;
						// 11 - ПКР;
						// 12 - ПРР;
						// 13 - вертолет;
						// 15 - ВПЦ.
	unsigned short		GO		:2;	// признак госпринадлежности:
						// 00 - неопознанный;
						// 01 - чужой;
						// 10 - свой.
	unsigned short		pad2	:5;	// 0
	unsigned short		MPO		:1;	// признак малоподвижного объекта
	unsigned short		Sbr		:1;	// признак сброса
	unsigned short		PNS		:1;	// признак неустойчивого сопровождения цели
	unsigned short		PA		:1;	// признак полуавтоматического сопровождения цели:
						// 1 - полуавтомат;
						// 0 - автомат.
	unsigned short		pad3	:1;	// 0

	unsigned short		X;			// север , диапазон: -150км..+150км, ц.м.р. 5м

	unsigned short		Y;			// восток, диапазон: -150км..+150км, ц.м.р. 5м

	unsigned short		Z;			// горизонт, диапазон: 0..50км, ц.м.р. 1м

	unsigned short		tX;			// диапазон: 0..1000м, ц.м.р. 1м

	unsigned short		tY;			// диапазон: 0..1000м, ц.м.р. 1м

	unsigned short		tZ;			// диапазон: 0..1000м, ц.м.р. 1м

	unsigned short		tVx		:7;	// диапазон: 0..240м/с, ц.м.р. 5м/с
	unsigned short		Vx		:9; // диапазон: -1200м/с..+1200м/с, ц.м.р. 5м/с

	unsigned short		tVy		:7;	// диапазон: 0..240м/с, ц.м.р. 5м/с
	unsigned short		Vy		:9; // диапазон: -1200м/с..+1200м/с, ц.м.р. 5м/с

	unsigned short		tVz		:7;	// диапазон: 0..240м/с, ц.м.р. 5м/с
	unsigned short		Vz		:9; // диапазон: -1000м/с..+1000м/с, ц.м.р. 5м/с

}m3M87_Ip2Tgt_t;

#define _3M87_IP2_TGT_NUM 2
#define _3M87_IP2_TGT_MAX_NUM 20

/*
typedef struct{
	unsigned short KK;
	m3M87_Ip2Tgt_t tgt[_3M87_IP2_TGT_NUM];
}m3M87_Ip2Single_t;
*/
typedef struct{
	m3M87_Ip2Tgt_t tgt[_3M87_IP2_TGT_MAX_NUM];
}m3M87_Ip2_t;



// 3М87 (КМ) --> IP3 --> 5П-27М//
// Формуляр состояния КМ и БМ изделия 3М-87 и состояние обработки ЦУ.
// ПА = 10001.

typedef struct{

	unsigned short		RCU		:2;	// РЦУ:
						// 00 - централизованный;
						// 01 - резервный режим (централизованный).
						// 10 - АР, автономный;
	unsigned short		CRR		:1;	// ручной режим целераспределения
	unsigned short		CRA		:1;	// автоматический режим целераспределения
									// в режимах Централизованный и Резервный (ЦР-2), КМ всегда выдает ЦРА=1
	unsigned short		TKM		:1; // режим Т
	unsigned short		KKM		:1;	// режим Р
	unsigned short		RKM		:1;	// признак режима Р
	unsigned short		PNS_KM	:2; // признак наличия сопровождения:
						// х1 - сл Сигмой
						// 1х - МТК
	unsigned short		pad0	:7;	// 0

	unsigned short		NO		:9;	// номер в нумерации 5П-27М (0..300)
	unsigned short		PZ		:1;	// перезарядка
	unsigned short		OP		:1;	// объект поражен
	unsigned short		OO		:1;	// объект обстреливается
	unsigned short		OS		:1;	// объект сопровождается
	unsigned short		CUP		:1;	// цу принимается
	unsigned short		GC		:1;	// готов к приему ЦУ
	unsigned short		OTK		:1;	// отказ от ЦУ

	unsigned short		NBM		:1;	// неисправность БМ
	unsigned short		GZR		:1;	// готовность ЗР
	unsigned short		GZA		:1;	// готовность ЗА
	unsigned short		RZR		:1;	// работа ЗР
	unsigned short		RZA		:1;	// работа ЗА
	unsigned short		K		:1;	// режим К в БМ
	unsigned short		D		:1;	// режим Д в БМ
	unsigned short		B		:1;	// режим Б в БМ
	unsigned short		O		:1;	//
	unsigned short		pad1	:7; // 0

	unsigned short		BZA		:7;	// боезапас ЗА, ц.ст.р. 640шт
	unsigned short		BZR		:4;	// боезапас ЗР, ц.ст.р. 8шт
	unsigned short		pad2	:5;	// 0

	unsigned short		Do;			// наклонная дальность БМ, ц.ст.р 25460м

	unsigned short		qap;		// курсовой угол электрической оси поста БМ при отсутствии признака
									// "цель сопровождается"; - курсовой угол цели при наличии признака "цель сопровождается",
									// ц.ст.р. 180, диапазон 0..360

	unsigned short		Ko_BM;		// курс объекта,ц.ст.р. 180

	unsigned short		Vog_BM;		// горизонтальная скорость объекта, ц.ст.р. 1000м/с

}m3M87_Ip3_t;

typedef struct{
	unsigned short pad;
}m3M87_State_t;

typedef struct{
	m3M87_Ip1_t ip1;
	m3M87_Ip2_t ip2;
	m3M87_Ip3_t ip3;
	m3M87_State_t  state;
}IpMap3M87_t;

////////////////////////
//--    3M87 API    --//
////////////////////////
/**
Посылает ИП1 в устройство
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП1
@return  EOK в случае успеха иначе статус ошибки*/
int m3M87SetIp1(int fd, m3M87_Ip1_t *ip);
/**
Принимает ИП1 от устройства
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП1
@return  EOK в случае успеха иначе статус ошибки*/
int m3M87GetIp1(int fd, m3M87_Ip1_t *ip);
/**
Посылает один объект ИП2 в устройство
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП2
@return  EOK в случае успеха иначе статус ошибки*/
int m3M87SetIp2(int fd, m3M87_Ip2_t *ip);
/**
Принимает ИП2 от устройства
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП2
@return  EOK в случае успеха иначе статус ошибки*/
int m3M87GetIp2(int fd, m3M87_Ip2_t *ip);
/**
Посылает ИП3 в устройство
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП3
@return  EOK в случае успеха иначе статус ошибки*/
int m3M87SetIp3(int fd, m3M87_Ip3_t *ip);
/**
Принимает ИП3 от устройства
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП3
@return  EOK в случае успеха иначе статус ошибки*/
int m3M87GetIp3(int fd, m3M87_Ip3_t *ip);
/**
Принимает State от устройства
@param  fd - дескриптор устпойства
@param   *ip  указатель на State
@return  EOK в случае успеха иначе статус ошибки*/
int m3M87GetState(int fd, m3M87_State_t *ip);

//////////////////////////
//--      ctrl        --//
//////////////////////////
#define M3M87_CMD 			0
#define M3M87_SET_IP1 		__DIOT(_DCMD_MISC,  M3M87_CMD + 1, m3M87_Ip1_t)
#define M3M87_GET_IP1 		__DIOF(_DCMD_MISC,  M3M87_CMD + 2, m3M87_Ip1_t)
#define M3M87_SET_IP2		__DIOT(_DCMD_MISC,  M3M87_CMD + 3, m3M87_Ip2_t)
#define M3M87_GET_IP2		__DIOF(_DCMD_MISC,  M3M87_CMD + 4, m3M87_Ip2_t)
#define M3M87_SET_IP3 		__DIOT(_DCMD_MISC,  M3M87_CMD + 5, m3M87_Ip3_t)
#define M3M87_GET_IP3 		__DIOF(_DCMD_MISC,  M3M87_CMD + 6, m3M87_Ip3_t)
#define M3M87_GET_STATE 	__DIOF(_DCMD_MISC,  M3M87_CMD + 7, m3M87_State_t)


#ifdef __cplusplus
}
#endif

#endif /* L3M87_H_ */
