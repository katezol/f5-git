#ifndef LSFERA_H_
#define LSFERA_H_

#include <devctl.h>
#define _SFERA_DEV_NAME	"/dev/Sfera"

// 5П-27М --> IP1 --> ОЭС "Сфера-05"//
// Команда модулям обнаружения!!!
// Значения полей: ПА = 1(0001);
#define SFERA_MODUL_NUM 4
#define SFERA_TARGET_NUM 8
#define SFERA_TARGETOBN_NUM 60


typedef struct{
	unsigned short		typeTarget		: 2;//0-неопр, 1-надв, 2-возд, 3-берег
	unsigned short		reserve0		: 1;
	unsigned short		typeCommand		: 3;//1- сектор поиска 2-направление поиска,  3-целепоказ
	unsigned short		reserve1		: 2; 
	unsigned short		GO 				: 4;//1-своя 2-чужая
	unsigned short		istCU 			: 4;//1-RLK-s 2-РЛС-х 4-Сфера 5 МР-231 6-МР231-3
	unsigned short		nIstCU 			: 16;// номер цели в нумерации источника ЦУ
	unsigned short		UMmin			: 16; //-90..90, цена 14го разряда 90 в доп.коде, для НП-бис, для ЦП-УМ цели
	unsigned short		UMmax			: 16;//-90..90, цена 14го разряда 90 в доп.коде только для СП
	unsigned short		pelengMin 		: 16;//0..360 цена ст р 180 
	unsigned short		pelengMax 		: 16;//0..360 цена ст р 180 
}mSfera_Ip1Modul_t;


typedef struct{
	unsigned short		OT			:1;	// признак отказа изделия 5П-27М:
						// 0 - нет отказа;
						// 1 - есть отказ.
	unsigned short		RU	 		:2;	// режим управления изд. ОЭС, установленный в изд 5П-27М:
						//01   -"Централизованный";
						//10   -"Резервный";
						//00,11-"Автономный".
	unsigned short		RR  		:2;	// режим работы:
						//01    - боевой режим;
						//10    - боевой режим по имитированной обстановке;
						//00,11 - режим контроля
	unsigned short		pad0 		:3;// 0
	unsigned short		numberIP3	:4;// количество ИП3
	unsigned short		pad1 		:4;// 0

	mSfera_Ip1Modul_t tgt[SFERA_MODUL_NUM];
}mSfera_Ip1_t;

//////////////////////////////////////////////////////////////////////
// 5П-27М --> IP2 --> ОЭС "Сфера-05"//
// Команды целепоказ для модулей сопровождения;
// Значения полей: ПА = 2(0010), 3;
typedef struct{
	unsigned short		typeTarget		: 2;//0-неопр, 1-надв, 2-возд, 3-берег 4-верт
	unsigned short		pad0 			: 1;
	unsigned short		bPeleng			: 1;//признак передачи пеленга
	unsigned short		bUM 			: 1;//пизнак передачи УМ 
	unsigned short		bDistance 		: 1;//признак передачи Дистанции
	unsigned short		naznCU			: 2;//1-в интересах МУС, 2-в интересах ЗРК, 0,3-неопр
	unsigned short		GO 				: 4;//1-своя 2-чужая
	unsigned short		istCU 			: 4;//1-RLK-s 2-РЛС-х 4-Сфера 5 МР-231 6-МР231-3
	unsigned short		nIstCU 			: 16;// номер цели в нумерации источника ЦУ
	unsigned short		peleng 			: 16;//0..360 цена ст р 180 
	unsigned short		UM				: 16; //-90..90, цена 14го разряда 90 в доп.коде, для НП-бис, для ЦП-УМ цели
	unsigned short		distance 		: 16;//0..256км, ЦСР 128км
	unsigned short		pad1 			: 16;
}mSfera_Ip2Target_t;

///////////////////////////////////////////////////
// 5П-27М --> IP3 --> ОЭС "Сфера-05"//
// Формуляр результатов ГО;
// Значения полей: ПА = 4,5

typedef struct{
	
	unsigned short		pad0			:8;	// 
	unsigned short		process			: 4;// 0-ГО не произв, 1-производится, 2-результат
	unsigned short		GO 				: 4;//0-неоп 1-своя 2-чужая
	unsigned short		nSfera			: 16;	// номер цели (1-255 или 0) в нумерации изд. ОЭС,
	unsigned short		P;				// пеленг (курсовой угол) цели - диапазон 360град.
										// цена старшего разряда при передаче пеленгов (курсовых углов) цели 180град.
	unsigned short		D;				// дальность. Цена СР 128 км
}mSfera_ResGO_t;

///////////////////////////////////////////////////
// ОЭС "Сфера-05" --> IP4 --> 5П-27М//
// Формуляр состояния;
// Значения полей: ПА = 1(0001);

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
	unsigned short		obm_oem1 		:1; // признак наличия связи с м.обзора
	unsigned short		obm_oem2 		:1;// признак наличия связи с м.обзора
	unsigned short		obm_oem3 		:1;// признак наличия связи с м.обзора
	unsigned short		obm_oem4 		:1;// признак наличия связи с м.обзора
	unsigned short		pad2 			:2;
	
	unsigned short		tv_oem1 		:1; // признак наличия связи с тепловизором м.обзора
	unsigned short		tv_oem2 		:1;// признак наличия связи с тепловизором м.обзора
	unsigned short		tv_oem3 		:1;// признак наличия связи с тепловизором м.обзора
	unsigned short		tv_oem4 		:1;// признак наличия связи с тепловизором м.обзора
	unsigned short		o_oem1 			:1; // признак исправности оптики м.обзора
	unsigned short		o_oem2 			:1;// признак исправности оптики м.обзора
	unsigned short		o_oem3 			:1;// признак исправности оптики м.обзора
	unsigned short		o_oem4 			:1;// признак исправности оптики оем м.обзора
	unsigned short		reg_oem1 		:2; // режим работы м.обзора 0-отказ 1-ручн 2-рабочий 3-контроль
	unsigned short		reg_oem2 		:2;// режим работы м.обзора
	unsigned short		reg_oem3 		:2;// режим работы м.обзора
	unsigned short		reg_oem4 		:2;// режим работы м.обзора

	unsigned short		ld11 		:1; // испр лев ЛД 1 м.сопр
	unsigned short		ld12 		:1;// испр прав ЛД 1 м сопр
	unsigned short		ld21 		:1;// 
	unsigned short		ld22 		:1;// 
	unsigned short		ld31 		:1; //
	unsigned short		ld32 		:1;// 
	unsigned short		ld41 		:1;// 
	unsigned short		ld42 		:1;// 
	unsigned short		pad3 		:8;

	unsigned short		reg_sopr11 		:2; // режим работы м.сопр 0-отказ 1-ручн 2-рабочий 3-контроль
	unsigned short		reg_sopr12 		:2; // режим работы м.сопр 0-отказ 1-ручн 2-рабочий 3-контроль
	unsigned short		reg_sopr21 		:2;// режим работы м.сопр
	unsigned short		reg_sopr22 		:2; // режим работы м.сопр 0-отказ 1-ручн 2-рабочий 3-контроль
	unsigned short		reg_sopr31 		:2;// режим работы м.сопр
	unsigned short		reg_sopr32 		:2; // режим работы м.сопр 0-отказ 1-ручн 2-рабочий 3-контроль
	unsigned short		reg_sopr41 		:2;// режим работы м.сопр
	unsigned short		reg_sopr42 		:2; // режим работы м.сопр 0-отказ 1-ручн 2-рабочий 3-контроль
	
	unsigned short		obzor_oem1 		:1; // признак выполнения обзова м.обнаружения
	unsigned short		obzor_oem2 		:1; // признак выполнения обзова м.обнаружения
	unsigned short		obzor_oem3 		:1; // признак выполнения обзова м.обнаружения
	unsigned short		obzor_oem4 		:1; // признак выполнения обзова м.обнаружения
	unsigned short		pad5 			:4;

	unsigned short		bDistance11 	:1; // признак изменрения дистанции
	unsigned short		bDistance12 	:1; // признак изменрения дистанции
	unsigned short		bDistance21 	:1; // признак изменрения дистанции
	unsigned short		bDistance22 	:1; // признак изменрения дистанции
	unsigned short		bDistance31 	:1; // признак изменрения дистанции
	unsigned short		bDistance32 	:1; // признак изменрения дистанции
	unsigned short		bDistance41 	:1; // признак изменрения дистанции
	unsigned short		bDistance42 	:1; // признак изменрения дистанции

	unsigned short		peleng1 		:16; // 180/2 15 м.обнаружения
	unsigned short		angleHor1 		:16; // 180/2 15
	unsigned short		UM1				:16; //-90..90, цена 14го разряда 90 в доп.коде, для НП-бис, для ЦП-УМ цели
	unsigned short		angleVer1 		:16; // 180/2 15

	unsigned short		peleng2 		:16; // 180/2 15 м.обнаружения
	unsigned short		angleHor2 		:16; // 180/2 15
	unsigned short		UM2				:16; //-90..90, цена 14го разряда 90 в доп.коде, для НП-бис, для ЦП-УМ цели
	unsigned short		angleVer2 		:16; // 180/2 15

	unsigned short		peleng3 		:16; // 180/2 15 м.обнаружения
	unsigned short		angleHor3 		:16; // 180/2 15
	unsigned short		UM3				:16; //-90..90, цена 14го разряда 90 в доп.коде, для НП-бис, для ЦП-УМ цели
	unsigned short		angleVer3 		:16; // 180/2 15
	
	unsigned short		peleng4 		:16; // 180/2 15 м.обнаружения
	unsigned short		angleHor4 		:16; // 180/2 15
	unsigned short		UM4				:16; //-90..90, цена 14го разряда 90 в доп.коде, для НП-бис, для ЦП-УМ цели
	unsigned short		angleVer4 		:16; // 180/2 15

	unsigned short		numberSfera[8]; // номера целей, которые сопрровождаются модулями сопровождениями (лев, пр, лев..)
}mSfera_Ip4_t;

//////////////////////////////////////////////////////////////////////
// ОЭС "Сфера-05" --> IP5 --> 5П-27М//
// Формуляр сопровождаемых целей;
// Значения полей: ПА = 2(0010) - для первых 3 целей;
// 				   ПА = 3(0011) - для целей 4-6;
// 				   ПА = 4(0011) - для целей 7-8;
// Максимальное число сопровождаемых целей - восемь.

typedef struct{
	
	unsigned short		nSfera		: 16;// номер цели в нумерации Cферы
	unsigned short		typeTarget	:2;	// тип цели,установленный в изделии ОЭС:
						// 00 - тип не определен;
						// 01 - надводный;
						// 10 - воздушный;
						// 11 - береговой.
	unsigned short 		pad0		:1;
	unsigned short		bVPeleng			: 1;//признак ненулевого ВИП
	unsigned short		bVUM 			: 1;//признак ненулевой скорости изм УМ 
	unsigned short		bVDistance 		: 1;//признак ненулевого ВИП
	unsigned short		naznCU			: 2;//1-в интересах МУС, 2-в интересах ЗРК, 0,3-неопр
	unsigned short		istCU 			: 4;//1-RLK-s 2-РЛС-х 4-Сфера 5 МР-231 6-МР231-3
	unsigned short		GO 				: 4;//1-своя 2-чужая
	
	unsigned short		nIstCU 			: 16;// номер цели в нумерации источника ЦУ
	unsigned short		peleng 			: 16;//0..360 цена ст р 180 
	unsigned short		UM				: 16; //-90..90, цена 14го разряда 90 в доп.коде, для НП-бис, для ЦП-УМ цели
	unsigned short		distance 		: 16;//0..256км, ЦСР 128км
	unsigned short		vPeleng			: 16;//ВИП 15р - знак, цмр 512/2 14
	unsigned short		vUM 			: 16;//скорость изм УМ 
	unsigned short		vDistance 		: 16;//ВИП
}mSfera_Ip5Target_t;

//////////////////////////////////////////////////////////////////////
// ОЭС "Сфера-05" --> IP6 --> 5П-27М//
// Формуляр обнаруженных целей;
// Значения полей: ПА = 5 - 13, по 7 целней на ПА
// Максимальное число обнаруженных целей - шестьдесят.
// Цели распределяются по подадресам, начиная с подадреса 5, и занимают подадреса в порядке
// возрастания их номеров в зависимости от количества объектов.

typedef struct{
	unsigned short		nSfera		: 16;// номер цели в нумерации Cферы
	unsigned short		TIP			:2;	// тип цели,установленный в изделии ОЭС:
						// 00 - тип не определен;
						// 01 - надводный;
						// 10 - воздушный;
						// 11 - береговой.
	unsigned short 		pad0		:14;
	unsigned short		P;				// пеленг (курсовой угол) цели - диапазон 360град.
										// цена старшего разряда при передаче пеленгов (курсовых углов) цели 180град.

	unsigned short		UM;	   			// угол места цели - диапазон от -90 до +90 град.,
										// отрицательные значения передаются в дополнительном коде,
										// старший (15) разряд знаковый, цена 14 разряда 90град.

}mSfera_Ip6Target_t;


///////////////
//-- State --//
///////////////
/**Состояние каналов*/
typedef struct {
	unsigned short 		pri_state		:1;///< основной
	unsigned short		res_state		:1;///< резервный
}mSfera_State_t;

///////////////
//--  MAP  --//
///////////////
#define _SFERA_NUM_OF_IP 7
/**Карта памати  */
typedef struct{
	mSfera_Ip1_t 	 ip1;	/// СС и команды модулям обнаружения
	mSfera_Ip2Target_t 	 ip2[SFERA_TARGET_NUM];	/////< команды модулям сопровождения
	mSfera_ResGO_t 	 ip3[SFERA_TARGET_NUM];	///< РЛК-S   --> результаты ГО --> ОЭС "Сфера-05"
	mSfera_Ip4_t 	 ip4;	///< ОЭС "Сфера-05" --> СС
	mSfera_Ip5Target_t 	 ip5[SFERA_TARGET_NUM];	///< ОЭС "Сфера-05" --> формудяры целей сопр
	mSfera_Ip6Target_t 	 ip6[SFERA_TARGETOBN_NUM];	///< ОЭС "Сфера-05" --> формудяры целей обн
	mSfera_State_t state;	///< РЛК-S-5П-27М
}IpMapSfera_t;

////////////////////////
//--    Sfera API    --//

typedef struct{
	mSfera_Ip2Target_t 	 ip2[SFERA_TARGET_NUM];	/////< команды модулям сопровождения
}mSfera_Ip2AllTargets;

typedef struct{
	mSfera_ResGO_t 	 ip3[SFERA_TARGET_NUM];	///< РЛК-S   --> результаты ГО --> ОЭС "Сфера-05"
}mSfera_Ip3AllTargets;

typedef struct{
	mSfera_Ip6Target_t 	 ip6[SFERA_TARGETOBN_NUM];	///< ОЭС "Сфера-05" --> формудяры целей обн
}mSfera_Ip6AllTargets;

typedef struct{
	mSfera_Ip5Target_t 	 ip5[SFERA_TARGET_NUM];	///< ОЭС "Сфера-05" --> формудяры целей сопр
}mSfera_Ip5AllTargets;


////////////////////////
/**
Посылает ИП1 в устройство
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП1
@return  EOK в случае успеха иначе статус ошибки*/
int mSferaSetIp1(int fd, mSfera_Ip1_t *ip);
/**
Принимает ИП1 от устройства
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП1
@return  EOK в случае успеха иначе статус ошибки*/
int mSferaGetIp1(int fd, mSfera_Ip1_t *ip);
/**
Посылает ИП2 в устройство
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП2
@return  EOK в случае успеха иначе статус ошибки*/
int mSferaSetIp2(int fd, mSfera_Ip2AllTargets *ip);
/**
Принимает ИП2 от устройства
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП2
@return  EOK в случае успеха иначе статус ошибки*/
int mSferaGetIp2(int fd, mSfera_Ip2AllTargets *ip);
/**
Посылает ИП3 в устройство
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП3
@return  EOK в случае успеха иначе статус ошибки*/
int mSferaSetIp3(int fd, mSfera_Ip3AllTargets *ip);
/**
Принимает ИП3 от устройства
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП3
@return  EOK в случае успеха иначе статус ошибки*/
int mSferaGetIp3(int fd, mSfera_Ip3AllTargets *ip);
/**
Посылает ИП4 в устройство
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП3
@return  EOK в случае успеха иначе статус ошибки*/
int mSferaSetIp4(int fd, mSfera_Ip4_t *ip);
/**
Принимает ИП4 от устройства
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП3
@return  EOK в случае успеха иначе статус ошибки*/
int mSferaGetIp4(int fd, mSfera_Ip4_t *ip);
/**
Посылает ИП5 в устройство
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП3
@return  EOK в случае успеха иначе статус ошибки*/
int mSferaSetIp5(int fd, mSfera_Ip5AllTargets *ip);
/**
Принимает ИП5 от устройства
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП3
@return  EOK в случае успеха иначе статус ошибки*/
int mSferaGetIp5(int fd, mSfera_Ip5AllTargets *ip);
/**
Посылает ИП6 в устройство
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП3
@return  EOK в случае успеха иначе статус ошибки*/
int mSferaSetIp6(int fd, mSfera_Ip6AllTargets *ip);
/**
Принимает ИП6 от устройства
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП3
@return  EOK в случае успеха иначе статус ошибки*/
int mSferaGetIp6(int fd, mSfera_Ip6AllTargets *ip);
/**
Посылает State в устройство
@param  fd - дескриптор устпойства
@param   *ip  указатель на State
@return  EOK в случае успеха иначе статус ошибки*/
int mSferaGetState(int fd, mSfera_State_t *ip);

//////////////////////////
//--      ctrl        --//
//////////////////////////





#define MSFERA_CMD 		0
#define MSFERA_SET_IP1 			__DIOT(_DCMD_MISC,  MSFERA_CMD + 1 , mSfera_Ip1_t)
#define MSFERA_GET_IP1 			__DIOF(_DCMD_MISC,  MSFERA_CMD + 2 , mSfera_Ip1_t)
#define MSFERA_SET_IP2			__DIOT(_DCMD_MISC,  MSFERA_CMD + 3 , mSfera_Ip2AllTargets)
#define MSFERA_GET_IP2			__DIOF(_DCMD_MISC,  MSFERA_CMD + 4 , mSfera_Ip2AllTargets)
#define MSFERA_SET_IP3 			__DIOT(_DCMD_MISC,  MSFERA_CMD + 5 , mSfera_Ip3AllTargets)
#define MSFERA_GET_IP3 			__DIOF(_DCMD_MISC,  MSFERA_CMD + 6 , mSfera_Ip3AllTargets)
#define MSFERA_SET_IP4 			__DIOT(_DCMD_MISC,  MSFERA_CMD + 7 , mSfera_Ip4_t)
#define MSFERA_GET_IP4 			__DIOF(_DCMD_MISC,  MSFERA_CMD + 8 , mSfera_Ip4_t)
#define MSFERA_SET_IP5 			__DIOT(_DCMD_MISC,  MSFERA_CMD + 9 , mSfera_Ip5AllTargets)
#define MSFERA_GET_IP5 			__DIOF(_DCMD_MISC,  MSFERA_CMD + 10, mSfera_Ip5AllTargets)
#define MSFERA_SET_IP6 			__DIOT(_DCMD_MISC,  MSFERA_CMD + 11, mSfera_Ip6AllTargets)
#define MSFERA_GET_IP6 			__DIOF(_DCMD_MISC,  MSFERA_CMD + 12, mSfera_Ip6AllTargets)
#define MSFERA_GET_STATE 		__DIOF(_DCMD_MISC,  MSFERA_CMD + 13, mSfera_State_t)
#endif /* LSFERA_H_ */
