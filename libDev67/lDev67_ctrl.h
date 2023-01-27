#ifndef LDEV67_CTRL_H_
#define LDEV67_CTRL_H_

typedef struct{
	unsigned short	begin;
	unsigned short	end;
}d67strob_t;

//#define DEV67_TGT_TYPE_TGT		0
//#define DEV67_TGT_TYPE_ALARM	1
typedef struct{
	unsigned short 	type;
	Dev67IP_t		ip;
}d67sub_t;


#define DEV67_CMD_SET_ANT 		__DIOT(_DCMD_MISC,  0, unsigned short)
#define DEV67_CMD_ENABLE 		__DIOT(_DCMD_MISC,  1, D67Mode_t)
#define DEV67_CMD_SET_STROB		__DIOT(_DCMD_MISC,  2, d67strob_t)
#define DEV67_CMD_DISABLE 		__DIOT(_DCMD_MISC,  3, unsigned short)
#define DEV67_CMD_GET_STATE		__DIOF(_DCMD_MISC,  4, Dev67State_t)
#define DEV67_CMD_GET_TGT 		__DIOF(_DCMD_MISC,  5, d67sub_t)

/////////////////
/// -- IP1 -- ///
/////////////////
#define D67_IP1_ID		0
typedef struct{
	unsigned char 		pad0		:2;
	unsigned char 		B2			:1;
	unsigned char 		B1			:1;
	unsigned char 		C			:3;
	unsigned char 		one0		:1;

	unsigned char 		L			:2;
	unsigned char 		R			:3;
	unsigned char 		id			:3;
}mD67_Ip1_t;

/////////////////
/// -- IP2 -- ///
/////////////////
typedef struct{
	unsigned char 		zero0		:8;
}mD67_Ip2_t;

/////////////////
/// -- IP3 -- ///
/////////////////
#define D67_IP3_ID		1
typedef struct{
	unsigned char 		D_low		:7;
	unsigned char 		one0		:1;
	unsigned char 		D_hi		:5;
	unsigned char 		id			:3;
	
}mD67_Ip3_t;

/////////////////
/// -- IP4 -- ///
/////////////////
#define D67_IP4_ID		2
typedef struct{
	unsigned char 		D_low		:7;
	unsigned char 		one0		:1;
	unsigned char 		D_hi		:5;
	unsigned char 		id			:3;
}mD67_Ip4_t;

/////////////////
/// -- IP5 -- ///
/////////////////
typedef struct{
	unsigned char 		zero0		:2;
	unsigned char 		PR			:1;
	unsigned char 		A			:2;
	unsigned char 		one0		:1;
	unsigned char 		one1		:1;
	unsigned char 		zero1		:1;
}mD67_Ip5_t;

/////////////////
/// -- IP6 -- ///
/////////////////
#define D67_IP6_ID		0
typedef struct{
	unsigned char 		T_low		:7;
	unsigned char 		one0		:1;
	unsigned char 		T_hi		:3;
	unsigned char 		C			:1;
	unsigned char 		one1		:1;
	unsigned char 		id			:3;
}mD67_Ip6_t;

/////////////////
/// -- IP7 -- ///
/////////////////
#define D67_IP7_ID		1
typedef struct{
	unsigned char 		P_low		:7;
	unsigned char 		one0		:1;
	unsigned char 		P_hi		:3;
	unsigned char 		PN			:1;
	unsigned char 		B			:1;
	unsigned char 		id			:3;
}mD67_Ip7_t;

/////////////////
/// -- IP8 -- ///
/////////////////
#define D67_IP8_ID		2
typedef struct{
	unsigned char 		D_low		:7;
	unsigned char 		one0		:1;
	unsigned char 		D_hi		:5;
	unsigned char 		id			:3;
}mD67_Ip8_t;

/////////////////
/// -- IP9 -- ///
/////////////////
#define D67_IP9_ID		3
typedef struct{
	unsigned char 		N_low		:7;
	unsigned char 		one0		:1;
	unsigned char 		N_hi		:5;
	unsigned char 		id			:3;
}mD67_Ip9_t;

/////////////////
/// -- IP10 -- ///
/////////////////
typedef struct{
	unsigned char 		one0		:1;
	unsigned char 		one1		:1;
	unsigned char 		one2		:1;
	unsigned char 		C			:1;
	unsigned char 		zero0		:4;
}mD67_Ip10_t;

/////////////////
/// -- IP11 -- ///
/////////////////
#define D67_IP11_ID		0
typedef struct{
	unsigned char 		L			:2;
	unsigned char 		R			:3;
	unsigned char 		RR			:1;
	unsigned char 		PP			:1;
	unsigned char 		one0		:1;
	unsigned char 		zero0		:3;
	unsigned char 		C			:1;
	unsigned char 		zero1		:1;
	unsigned char		id			:3;
}mD67_Ip11_t;

typedef union{ 
	mD67_Ip1_t		ip1;
	mD67_Ip2_t		ip2;
	mD67_Ip3_t		ip3;
	mD67_Ip4_t		ip4;
	mD67_Ip5_t		ip5;
	mD67_Ip6_t		ip6;
	mD67_Ip7_t		ip7;
	mD67_Ip8_t		ip8;
	mD67_Ip9_t		ip9;
	mD67_Ip10_t		ip10;
	mD67_Ip11_t		ip11;
}mD67_IP_t;

#endif /*LDEV67_CTRL_H_*/
