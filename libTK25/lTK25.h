#ifndef LTK25_H_
#define LTK25_H_

#define _TK25_DEV_NAME	"/dev/TK25"

typedef struct{
	unsigned short		NCf		:16;
	unsigned short		TIP		:4;
	unsigned short		IST		:4;
	unsigned short		pad		:8;

	
	unsigned short		X;
	unsigned short		Y;
	unsigned short		H;
	unsigned short		Vx;
	unsigned short		Vy;
	unsigned short		Vh;
}mTK25_Ip1Tgt_t;

/////////////////
/// -- IP1 -- ///
/////////////////
#define _TK25_IP1_TGT_MAX_NUM 3
typedef struct{
	unsigned short		RR		:2;
	unsigned short		pad0	:4;
	unsigned short		RV		:2;
	unsigned short		KFurke	:8;
	
	unsigned short		NK		:2;
	unsigned short		IZL		:1;
	unsigned short		KK		:1;
	unsigned short		RVI		:3;
	unsigned short		pad1	:3;
	unsigned short		KObstr	:6;

	mTK25_Ip1Tgt_t	tgt[_TK25_IP1_TGT_MAX_NUM];
	
}mTK25_Ip1_t;

typedef struct{
	unsigned short		TIP			:4;
	unsigned short		PRGO		:2;
	unsigned short		SHP			:1;
	unsigned short		pad0		:1;
	unsigned short		NC			:8;

	unsigned short		X;
	unsigned short		Y;
	unsigned short		H;
	unsigned short		Vx;
	unsigned short		Vy;
	unsigned short		Vh;			
}mTK25_Ip2Tgt_t;

/////////////////
/// -- IP2 -- ///
/////////////////
#define _TK25_IP2_TGT_MAX_NUM 4
typedef struct{
	mTK25_Ip2Tgt_t tgt[_TK25_IP2_TGT_MAX_NUM];
}mTK25_Ip2_t;


/////////////////
/// -- IP3 -- ///
/////////////////
typedef struct{
	unsigned short		SZIB	:16;
	unsigned short		SZIE	:16;
}mTK25_Ip3_OneSZI_t;


typedef struct{
	unsigned short		KC		:8;  // 1- ПрБортНос, 2- ПрБортКорма, 3 -ЛБортНос 4- ЛБКорма
	unsigned short		RVI		:3;
	unsigned short		pad0	:5;

	unsigned short		SS	:2;  // состояние РЭП: 0-тренаж, 1-работа, 2-контроль, 3-отказ
	unsigned short		SActive2:3; // состояние активного канала 1-готов, 2-контроль 3-отказ
	unsigned short		pad2	:1;
	unsigned short		SObn1	:2;// состояние канала обнар 1-готов, 2-контроль 3-отказ
	unsigned short		SActive1:3;
	unsigned short		pad3	:1;
	unsigned short		SObn2	:2;
	unsigned short		Pom2	:1; // признак постановки помехи
	unsigned short		Pom1	:1;

	unsigned short		pad4	:2;
	unsigned short		SActive4:3;
	unsigned short		pad5	:1;
	unsigned short		SObn3	:2;
	unsigned short		SActive3:3;
	unsigned short		pad6	:1;
	unsigned short		SObn4	:2;
	unsigned short		Pom4	:1;
	unsigned short		Pom3	:1;
	unsigned short		ShellWithoutOs_PU4		:4;
	unsigned short		ShellWithoutOs_PU3		:4;
	unsigned short		ShellWithoutOs_PU2		:4;
	unsigned short		ShellWithoutOs_PU1		:4;
	unsigned short		ShellWithOs_PU4		:4;
	unsigned short		ShellWithOs_PU3		:4;
	unsigned short		ShellWithOs_PU2		:4;
	unsigned short		ShellWithOs_PU1		:4;
	unsigned short		PelPomehi  [4]; // пеленга помехи 1- ПрБортНос, 2- ПрБортКорма, 3 -ЛБортНос 4- ЛБКорма
	unsigned short		szi_count: 4;
	unsigned short		pad7	: 12;  
}mTK25_Ip3_t;

//typedef struct{
//	mTK25_Ip3_OneSZI_t SZI[15];
//}mTK25_Ip3_DopSZI_t;


/////////////////
/// -- IP4 -- ///
/////////////////
#define _TK25_IP4_TGT_MAX_NUM 10
typedef struct{
	unsigned short		nFurke		:8;
	unsigned short		range		:3;
	unsigned short		radarType	:4;
	unsigned short		work		:1;
	
	unsigned short		n	:16;
	unsigned short		pad		:3;
	unsigned short		CP_4		:1;
	unsigned short		CP_3		:1;
	unsigned short		CP_2		:1;
	unsigned short		CP_1		:1;
	unsigned short		mode		:1;
	unsigned short		peleng		:8;			
}mTK25_Ip4Tgt_t;

typedef struct{
	mTK25_Ip4Tgt_t tgt[_TK25_IP4_TGT_MAX_NUM];
}mTK25_Ip4_t;

typedef struct {
	unsigned short 		mch_pri_state		:1;
	unsigned short		mch_res_state		:1;
}mTK25_State_t;


///////////////////
/// -- IPMap -- ///
///////////////////
#define TK25_IP1_TGT_MAX_NUM 		_TK25_IP1_TGT_MAX_NUM
#define TK25_IP2_TGT_MAX_NUM 		150
#define TK25_IP4_TGT_MAX_NUM		255
typedef struct{
	mTK25_Ip1_t		ip1[3];
	mTK25_Ip2Tgt_t 	ip2tgt[TK25_IP2_TGT_MAX_NUM];
	mTK25_Ip3_t		ip3;
	mTK25_Ip3_OneSZI_t	ip3_SZI[20];
	mTK25_Ip4Tgt_t 	ip4tgt[TK25_IP4_TGT_MAX_NUM];
	mTK25_State_t	state;
}IpMapTK25_t;

#endif /*LTK25_H_*/
