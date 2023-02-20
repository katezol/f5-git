#ifndef LRLI_H_
#define LRLI_H_

#define _RLI_DEV_NAME	"/dev/RLI"

typedef struct{
	unsigned short	NC		:8;
	unsigned short	pad0	:8;
	unsigned short	TSev_1;
	unsigned short	TSev_2;
	unsigned short	TIP		:4;
	unsigned short	PRGO	:2;
	unsigned short	pad1	:3;
	unsigned short	IMIT	:1;
	unsigned short	PN		:1;
	unsigned short	MPO		:1;
	unsigned short	SBR		:1;
	unsigned short	PNS		:1;
	unsigned short	PS		:1;
	unsigned short	SHP		:1;
	unsigned short	X;
	unsigned short	Y;
	unsigned short	H;
	unsigned short	tX;
	unsigned short	tY;
	unsigned short	tH;
	
	unsigned short	Vx		:10;
	unsigned short	tVx		:6;
	
	unsigned short	Vy		:10;
	unsigned short	tVy		:6;
	
	unsigned short	Vh		:10;
	unsigned short	tVh		:6;
	
}mRLI_Ip1Tgt_t;


/////////////////
/// -- IP1 -- ///
/////////////////
#define MRLI_IP1_TGT_NUM	2
typedef struct{
	mRLI_Ip1Tgt_t	tgt[MRLI_IP1_TGT_NUM];
}mRLI_Ip1_t;

typedef struct{
	unsigned short _5p10_pri	:1;
	unsigned short _5p10_res	:1;
	unsigned short _5p20_pri	:1;
	unsigned short _5p20_res	:1;
	unsigned short sigma_pri	:1;
	unsigned short sigma_res	:1;
}mRLI_ChState_t; 
///////////////////
/// -- IPMAP -- ///
///////////////////
typedef struct{ 
	mRLI_Ip1_t ip1;
	mRLI_ChState_t state; 
}IpMapRLI_t;

#endif /*LRLI_H_*/
