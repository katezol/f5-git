#ifndef L5P10_H_
#define L5P10_H_

#define _5P10_DEV_NAME	"/dev/5P10"

/////////////////
/// -- IP1 -- ///
/////////////////
typedef struct{
	unsigned short		NS			:8;
	unsigned short		IBL			:4;
	unsigned short		ICO			:4;
	
	unsigned short		pad1		:4;
	unsigned short		MO			:1;
	unsigned short		MP			:1;
	unsigned short		NMOr		:1;
	unsigned short		pad2		:2;
	unsigned short		KCUr		:3;
	unsigned short		RCUr		:3;
	unsigned short		pad3		:1;
	
	unsigned short		pad4;
	
	unsigned short		NCcu		:16;

	unsigned short		ISTcu		:8;
	unsigned short		TIP			:7;
	unsigned short		IMC			:1;
	
	unsigned short		PCU			:3;
	unsigned short		ROO			:2;
	unsigned short		SCUr		:3;
	unsigned short		PRT0		:2;
	unsigned short		PRT1		:2;
	unsigned short		PRT2		:2;
	unsigned short		PRZS		:2;
	
	unsigned short		one1		:1;
	unsigned short		pad7		:2;
	unsigned short		PNK			:3;
	unsigned short		SKK			:2;
	unsigned short		PNS			:3;
	unsigned short		SKS			:1;
	unsigned short		pad8		:2;
	unsigned short		PRN			:2;
	
	unsigned short		X_hi;
	unsigned short		X_low;		
	unsigned short		Y_hi;
	unsigned short		Y_low;		
	unsigned short		H_hi;
	unsigned short		H_low;		
	
	unsigned short		Vx_hi;
	unsigned short		Vx_low;		
	unsigned short		Vy_hi;
	unsigned short		Vy_low;		
	unsigned short		Vh_hi;
	unsigned short		Vh_low;
	unsigned short		TSev_hi;
	unsigned short		TSev_low;
	/*
	float x;
	float y;
	float h;
	float Vx;
	float Vy;
	float Vh;*/
}m5P10_Ip1_t;

/////////////////
/// -- IP2 -- ///
/////////////////
typedef struct{
	
	unsigned short		NS			:8;
	unsigned short		pad0		:4;
	unsigned short		ICO			:4;
	
	unsigned short		RCUsu		:3;
	unsigned short		pad1		:2;
	unsigned short		KCUsu		:3;
	unsigned short		NMOr		:1;
	unsigned short		KCUr		:3;
	unsigned short		RCUr		:3;
	unsigned short		pad11		:1;
	
	unsigned short		GCU			:1;
	unsigned short		pad2		:3;
	unsigned short		ORRK		:2;
	unsigned short		pad3		:3;
	unsigned short		VNV			:1;
	unsigned short		EKV			:1;
	unsigned short		pad4		:3;
	unsigned short		RKSsu		:2;
	
	unsigned short		KSC			:4;
	unsigned short		PKRC		:4;
	unsigned short		pad5		:5;
	unsigned short		AU0OTKL		:1;
	unsigned short		AU1OTKL		:1;
	unsigned short		AU2OTKL		:1;
	
	unsigned short		KUap;
	unsigned short		UMap;
	unsigned short		pad6;
	
	unsigned short		Nzs_R		:8;
	unsigned short		TBZ			:3;
	unsigned short		CS0			:1;
	unsigned short		PN0			:1;
	unsigned short		Ris0		:1;
	unsigned short		Bl0			:1;
	unsigned short		GAU0		:1;
	
	unsigned short		Nzs_D		:8;
	unsigned short		Nofs		:8;
	
	unsigned short		GNau0;
	unsigned short		VNau0;
	
	unsigned short		pad7		:11;
	unsigned short		CS1			:1;
	unsigned short		PN1			:1;
	unsigned short		Ris1		:1;
	unsigned short		Bl1			:1;
	unsigned short		GAU1		:1;
	
	unsigned short		Nau1;		
	unsigned short		GNau1;
	unsigned short		VNau1;

	unsigned short		pad8		:11;
	unsigned short		CS2			:1;
	unsigned short		PN2			:1;
	unsigned short		Ris2		:1;
	unsigned short		Bl2			:1;
	unsigned short		GAU2		:1;
	
	unsigned short		Nau2;		
	unsigned short		GNau2;
	unsigned short		VNau2;
}m5P10_Ip2_t;

/////////////////
/// -- IP3 -- ///
/////////////////
typedef struct{
	unsigned short		NC			:8;
	unsigned short		IBL			:4;
	unsigned short		ICO			:4;
	
	unsigned short		NCcu		:16;

	unsigned short		ISTcu		:8;
	unsigned short		TIP			:7;
	unsigned short		IMC			:1;

	unsigned short		NCsu		:6;
	unsigned short		pad1		:8;
	unsigned short		PRN			:2;
	
	unsigned short		DCU			:3;
	unsigned short		RCD			:1;
	unsigned short		RRC			:4;
	unsigned short		NAU0		:2;
	unsigned short		NAU1		:2;
	unsigned short		NAU2		:2;
	unsigned short		pad2		:2;
	
	unsigned short		pad4		:2;
	unsigned short		VO0			:1;
	unsigned short		pad5		:3;
	unsigned short		VO1			:1;
	unsigned short		pad6		:3;
	unsigned short		VO2			:1;
	unsigned short		pad7		:3;
	unsigned short		PPC			:2;
	
	unsigned short		ISTdc		:3;
	unsigned short		PNK			:3;
	unsigned short		SKK			:2;
	unsigned short		PNS			:3;
	unsigned short		SKS			:1;
	unsigned short		pad8		:4;
	
	unsigned short		X_hi;
	unsigned short		X_low;
	unsigned short		Y_hi;
	unsigned short		Y_low;
	unsigned short		H_hi;
	unsigned short		H_low;
	unsigned short		Vx_hi;
	unsigned short		Vx_low;
	unsigned short		Vy_hi;
	unsigned short		Vy_low;
	unsigned short		Vh_hi;
	unsigned short		Vh_low;
	/*
	float x;
	float y;
	float h;
	float Vx;
	float Vy;
	float Vh;*/
}m5P10_Ip3_t;

/////////////////////
/// -- IP STATE -- ///
/////////////////////
#define M5P10_MCH_STATE_ON	1
#define M5P10_MCH_STATE_OFF	0
typedef struct{
	unsigned short 		mch_pri_state		:1;
	unsigned short		mch_res_state		:1;
}m5P10_State_t;

#define M5P10_IP1_NUM	2
#define M5P10_IP3_NUM	4
typedef struct{
	m5P10_Ip1_t 	ip1[M5P10_IP1_NUM];
	m5P10_Ip2_t 	ip2;
	m5P10_Ip3_t 	ip3[M5P10_IP3_NUM];
	m5P10_State_t	state;
}IpMap5P10_t;

int m5P10GetIp2(int fd, m5P10_Ip2_t *ip);
int m5P10GetIp3(int fd, m5P10_Ip3_t *ip);
//////////////////////////
//--      ctrl        --//
//////////////////////////
#define M5P10_CMD 		0
#define M5P10_GET_IP2 			__DIOF(_DCMD_MISC, M5P10_CMD + 1, m5P10_Ip2_t)
#define M5P10_GET_IP3 			__DIOF(_DCMD_MISC, M5P10_CMD + 2, m5P10_Ip3_t)

#endif /*L5P10_H_*/
