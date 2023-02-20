#ifndef LMTK201M_H_
#define LMTK201M_H_

#define _MTK_DEV_NAME	"/dev/MTK"
#define _IMMTK_DEV_NAME	"/dev/IMMTK"

#define MTK_TARGET_NUM 8

/////////////////
/// -- IP1 -- ///
/////////////////
typedef struct{
	unsigned short		typeTarget		: 3;//0-неопр, 1-надв, 2-возд, 3-берег 4-верт
	unsigned short		bNewCommand 	: 1;//новизна команды "НП"
	unsigned short		typeCommand		: 2;//1-направление поиска, 2- сектор поиска 3-целепоказ
	unsigned short		bInPeleng		: 1;//1- в пеленгах, 
	unsigned short		typeCamera 		: 3;//
	unsigned short		GO 				: 2;//1-своя 2-чужая
	unsigned short		istCU 			: 4;//1-RLK-s 2-РЛС-х 4-МТК 5 МР-231 6-МР231-3
	unsigned short		nIstCU 			: 16;// номер цели в нумерации источника ЦУ
	unsigned short		peleng			: 16; //0..360 цена ст р 180
	unsigned short		UM 				: 16;//-90..90, цена мл р 90 / 2_15
	unsigned short		sectorHor 		: 16;//0..360 цена ст р 180 только для СП
	unsigned short		sectorVer 		: 16;//цена мл р 90 / 2_15 только для СП
	unsigned short		distance 		: 16;//цена мл р 5м
	unsigned short		reserve2		: 16;//
	
}mMTK_Ip1Modul_t;

typedef struct{
	unsigned short		activeChannel	: 2; // 1-осн 2-рез, заполняется в драйвере
	unsigned short		bHigh			: 1;// включение высокого 
	unsigned short		speedAnt		: 3;// скорость вращения антенны 1-3об мин 2 - 6, 4 - 30.
	unsigned short		RO				: 4;// 1-РО1, 3-РО5, 5-РО2, 10-РО32, 11-РО31, 12-РО33, 14 РКИ
	unsigned short		RR				: 3;// 1-боевой, 2-боевой И, 3-режим контроля 
	unsigned short		RU				: 3;// 1-Автоном, 2 - ЦР, 3-РР-ВЦ, 4-РР НЦ, 
	unsigned short		numberIP2		: 4; // количество ИП2, от 0 до 8
	unsigned short		reserve2		: 12; // количество ИП2, от 0 до 8
	unsigned short		kuAnt			: 16;// КУ антенны, экстраполируется в драйвере 0..360 360/ 2_16
	mMTK_Ip1Modul_t modul[2];
}mMTK_Ip1_t;

typedef struct{
	unsigned short		pad0			: 8;	// 
	unsigned short		process			: 4;// 0-ГО не произв, 1-производится, 2-результат
	unsigned short		GO 				: 4;//0-неоп 1-своя 2-чужая
	unsigned short		nMTK			: 16;	// номер цели (1-255 или 0) в нумерации изд. ОЭС,
	unsigned short		t1;// время выработки данных по цели,  мс от начала суток, нужно брать из ИП4
	unsigned short		t2;// время выработки данных по цели,  мс от начала суток
	unsigned short		P;				// пеленг (курсовой угол) цели - диапазон 360град.
										// цена старшего разряда при передаче пеленгов (курсовых углов) цели 180град.
	unsigned short		D;				// дальность. Цена СР 128 км
}mMTK_Ip2ResGoTgt_t;

/////////////////
/// -- IP2 -- ///
/////////////////
typedef struct{
	mMTK_Ip2ResGoTgt_t tgt[MTK_TARGET_NUM];
}mMTK_Ip2_t;





/////////////////
/// -- IP3 -- ///
/////////////////
typedef struct{
	unsigned short		bOn			:2;//0-отключен, 1-вкл
	unsigned short		bInPeleng	:1;// 1- в пеленгах
	unsigned short		bTPV		:1;// активный канал 0-ТВ, 1-ТПВ
	unsigned short		typeCamera	:2;//1-узкое, 2-среднее, 3-широкое
	unsigned short		typeCommand	:4;//0-нет команды 1-НП, 2-СП, 3-ЦП, 4-другая команда  
	unsigned short		sostTV		:2;// 0-откл 1-отказ 2-работа
	unsigned short		sostTPV		:2;// 0-откл 1-отказ 2-работа
	unsigned short		sostLD		:2;// 0-откл 1-отказ 2-работа
	unsigned short		RU			:3; // 0-MTK, 1-RLK-s, 2 - Sigma, 3-SU, 4-MUS, 5-other
	unsigned short		istCU		: 4;////1-RLK-s 2-РЛС-х 4-МТК 5 МР-231 6-МР231-3
	unsigned short		pad0		: 2;//1-своя 2-чужая
	unsigned short		pad1		: 7;//
	unsigned short		nIstCU		: 16;// номер цели в нумерации источника ЦУ
	unsigned short		peleng		:16; // 0..360 цена ст р 180
	unsigned short		um			:16; // //-90..90, цена мл р 90 / 2_15
}mMTK_Ip3_OneModul_t;

typedef struct{
	unsigned short		RU			: 5;//1-автоном 2-ЦР 3 РР-ВЦ 4 РР-НЦ
	unsigned short		RR			: 2;//0,1 - не задан 2- ОСУ, 3-ОМУС
	unsigned short		Regim		: 4;// 0-отказ 1-походный 2-дежурный 3-боевой 4-контроль 5-тренировка
	unsigned short		pad0		: 5;//
	unsigned short		numberIP4	: 5;//количество ип4 (до 8)
	unsigned short		pad1		: 3;
	unsigned short		autoGO		: 1;
	unsigned short		pad2		: 7;
	mMTK_Ip3_OneModul_t modul[2];//
}mMTK_Ip3_t;

typedef struct{
	unsigned short 		nMTK		: 4;
	unsigned short 		typeMTK		: 4;// 0- неопр 1-берег 2-надв 3-возд 4-вертолет
	unsigned short		bV			: 1;
	unsigned short		bCourse		: 1;
	unsigned short		bDistance	: 1;
	unsigned short		GO 			: 3;//1-своя 2-чужая
	unsigned short		bInPeleng	: 1;
	unsigned short		OP			: 1; //0-правый, 1-левый
	unsigned short		istGO 		: 4;//источник присвоения ГО 0-нет данных, 1-МТК
	unsigned short		pad2		: 12;
	unsigned short		t1;// время выработки данных по цели,  мс от начала суток
	unsigned short		t2;// время выработки данных по цели,  мс от начала суток
	unsigned short		peleng		: 16;//0..360 цена ст р 180
	unsigned short		UM			: 16; //-90..90, цена мл р 90 / 2_15
	unsigned short		distance	: 16; // цмр 5м
	unsigned short		course		: 16; // 0..360 цена ст р 180
	unsigned short		V			: 16;// ц cт р 512
}mMTK_Ip4Tgt_t;

/////////////////
/// -- IP4 -- ///
/////////////////
#define _MTK_IP4_TGT_MAX_NUM 2
typedef struct{
	mMTK_Ip4Tgt_t tgt[_MTK_IP4_TGT_MAX_NUM];
}mMTK_Ip4_t;


typedef struct {
	unsigned short 		mch_pri_state		:1;
	unsigned short		mch_res_state		:1;
}mMTK_State_t;
///////////////////
/// -- IPMap -- ///
///////////////////

typedef struct{
	mMTK_Ip1_t		ip1;
	mMTK_Ip2_t		ip2; 
	mMTK_Ip3_t		ip3;
	mMTK_Ip4Tgt_t 	ip4tgt[MTK_TARGET_NUM];
    mMTK_State_t    state;
}IpMapMTK_t;



int mMTKGetIp3(int fd, mMTK_Ip3_t *ip);
int mMTKGetIp4(int fd, mMTK_Ip4Tgt_t *ip);
int mMTKGetIp1(int fd, mMTK_Ip1_t *ip);
int mMTKGetIp2(int fd, mMTK_Ip2_t *ip);
int mMTKGetState(int fd, mMTK_State_t *state);


//////////////////////////
//--      ctrl        --//
//////////////////////////
#define MMTK_CMD 		0
#define MMTK_GET_IP3 			__DIOF(_DCMD_MISC, MMTK_CMD + 1, mMTK_Ip3_t)
#define MMTK_GET_IP4 			__DIOF(_DCMD_MISC, MMTK_CMD + 2, mMTK_Ip4Tgt_t)


#define MIMMTK_GET_IP1 			__DIOF(_DCMD_MISC, MMTK_CMD + 3, mMTK_Ip1_t)
#define MIMMTK_GET_IP2 			__DIOF(_DCMD_MISC, MMTK_CMD + 4, mMTK_Ip2_t)
#define MIMMTK_GET_STATE		__DIOF(_DCMD_MISC, MMTK_CMD + 5, mMTK_State_t)






#endif /*LMTK201M_H_*/
