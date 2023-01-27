#ifndef LCHARDASH_H_
#define LCHARDASH_H_

#define _CHARDASH_DEV_NAME	"/dev/CHARD"

typedef struct{
	unsigned short 	aAngles		:1;	
	unsigned short 	aK			:1;
	unsigned short 	aPU			:1;
	unsigned short 	aGEO		:1;
	unsigned short 	aV0			:1;
	unsigned short 	aV			:1;
	unsigned short 	ah			:1;
	unsigned short 	aVAngles	:1;
	unsigned short 	atS			:1;
	unsigned short 	aVK			:1;
	unsigned short 	mode		:2;
	unsigned short 	pad0		:4;
	
	unsigned short 	Psi_hi;
	unsigned short 	Psi_low;
	
	unsigned short 	Teta_hi;
	unsigned short 	Teta_low;
	
	unsigned short 	K_hi;
	unsigned short 	K_low;
	
	unsigned short 	VPsi;
	unsigned short 	VTeta;
	unsigned short 	VK;
	
	unsigned short 	Vn;
	unsigned short 	tVn;
	unsigned short 	tSn;

	unsigned short 	Ve;
	unsigned short 	tVe;
	unsigned short 	tSe;
	
	unsigned short 	tVz;
	unsigned short 	tSz;
	
	unsigned short 	p;

	unsigned short 	Fi_hi;
	unsigned short 	Fi_low;
	
	unsigned short 	Ld_hi;
	unsigned short 	Ld_low;
	
	unsigned short 	PU_hi;
	unsigned short 	PU_low;
	
	unsigned short 	V0;
	//
	
	unsigned short 	a;
	unsigned short 	b;
	unsigned short 	t;
	unsigned short 	h;
	
	unsigned short 	pad;
	unsigned short 	CS;
}mCHARD_Ip_t;

///////////////////
/// -- IPCH -- ///
///////////////////
typedef struct{
	unsigned short	ch1pri	:1;
	unsigned short	ch1res	:1;
	unsigned short	ch2pri	:1;
	unsigned short	ch2res	:1;
}mChardChState_t;

///////////////////
/// -- IPMAP -- ///
///////////////////
typedef struct{
	mCHARD_Ip_t 		ip;
	mChardChState_t		chstate;	
}IpMapChardash_t;

#endif /*LCHARDASH_H_*/
