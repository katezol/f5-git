#ifndef LSIGMA_H_
#define LSIGMA_H_

#define _SIGMA_DEV_NAME		"/dev/SIGMA"


typedef struct{
	unsigned short	TS;
	unsigned short	DS;
	unsigned short	Sender;
	unsigned short	Receiver;
}eSigma_Head_t;


/////////////////////
/// -- IP Ctrl -- ///
/////////////////////
typedef struct{
	eSigma_Head_t	head;
	unsigned short 	num;	
}eSigma_CtrlIp_t;


typedef struct{
	unsigned short		SS			:2;
	unsigned short		RUAK		:2;
	unsigned short		RUK			:2;
	unsigned short		RU			:2;
	unsigned short		RAK			:1;
	unsigned short		RK			:1;
	unsigned short		RMT			:1;
	unsigned short		RUF			:2;
	unsigned short		pad0		:3;

	unsigned short		TTEK_low;
	unsigned short		TTEK_hi;
}eSigmaState_t;

typedef struct{
	unsigned short		NCgo		:8;
	unsigned short		pad1		:6;
	unsigned short		TCgo		:2;
	
	unsigned short		X;
	unsigned short		Y;
	unsigned short		H;
	unsigned short		Vx;
	unsigned short		Vy;
	unsigned short		Vh;
}eSigmaGO_t;

typedef struct{
	unsigned short		NKS		:6;
	unsigned short		NI		:3;
	unsigned short		NCks	:7;
	
	unsigned short		NCf		:8;
	unsigned short		pad2	:8;
	
	unsigned short		X;
	unsigned short		Y;
	unsigned short		Vx;
	unsigned short		Vy;
}eSigmaShip_t;
/////////////////
/// -- IP1 -- ///
/////////////////
typedef struct{
	eSigma_Head_t		head;
	unsigned short		numGO		:14;
	unsigned short 		one0		:1;
	unsigned short		zerro0		:1;
	
	unsigned short		numShip		:14;
	unsigned short		zero1		:1;
	unsigned short		one1		:1;
	
	eSigmaState_t		State;
}eSigma_Ip1_t;


typedef struct{
	unsigned short		PRC			:5;
	unsigned short		pad0		:1; 
	unsigned short		PRGO		:2;
	unsigned short		NCobs		:8;
	
	unsigned short		TC			:2;
	unsigned short		NI			:3;
	unsigned short		NCU			:2;
	unsigned short		pad2		:1;
	unsigned short		NCi			:8;
}eSigmaTGT_t;

typedef struct{
	unsigned short		TC			:2;
	unsigned short		pad3		:3;
	unsigned short		PRGO		:2;
	unsigned short		pad4		:1;
	unsigned short		NCrld		:8;
	
	unsigned short		X;
	unsigned short		Y;
	unsigned short		H;
	unsigned short		Vx;
	unsigned short		Vy;
	unsigned short		Vh;
}eSigmaTGTHel_t;
/////////////////
/// -- IP2 -- ///
/////////////////

typedef struct{
	eSigma_Head_t		head;
	unsigned short		numTgt		:14;
	unsigned short 		one0		:1;
	unsigned short		zero0		:1;
	
	unsigned short		numHel		:14;
	unsigned short		zero1		:1;
	unsigned short		one1		:1;
}eSigma_Ip2_t; 


/////////////////
/// -- IP3 -- ///
/////////////////
typedef struct{
	eSigma_Head_t		head;
	unsigned short		RU		:2;
	unsigned short		SGO		:1;
	unsigned short		IZL		:1;
	unsigned short		RR		:2;
	unsigned short		pad0	:2;
	unsigned short		KSC		:8;
	
	unsigned short		RO		:3;
	unsigned short		pad1	:13;
}eSigma_Ip3_t; 


typedef struct{
	unsigned short ch1	:1;
	unsigned short ch2	:1;
}eSigma_ChState_t; 
/////////////////
/// -- MAP -- ///
/////////////////
#define _SIGMA_MAX_GO		3
#define _SIGMA_MAX_SHIP		32
#define _SIGMA_MAX_TGT		10
#define _SIGMA_MAX_TGTH		40

typedef struct{
	eSigmaState_t 		SigmaState;
	eSigmaGO_t			GO[_SIGMA_MAX_GO];
	eSigmaShip_t		Ship[_SIGMA_MAX_SHIP];
	eSigmaTGT_t			Tgt[_SIGMA_MAX_TGT];
	eSigmaTGTHel_t		TgtH[_SIGMA_MAX_TGTH];
	eSigma_Ip3_t		_5P27State;
	eSigma_ChState_t	chstate;
}IpMapSigma_t;
#endif /*LSIGMA_H_*/
