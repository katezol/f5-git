#ifndef L5P20_H_
#define L5P20_H_


#define _5P20_DEV_NAME	"/dev/5P20"

typedef struct{
	unsigned short		D;
	unsigned short		P;
	unsigned short		BNC;
}m5P20_Ip1Tgt1_t;

typedef struct{
	unsigned short		D;
	unsigned short		P;
}m5P20_Ip1Tgt_t;

typedef struct{
	unsigned short		NC		:8;
	unsigned short		pad		:4;
	unsigned short		RZ		:3;
	unsigned short		NI		:1;
	m5P20_Ip1Tgt1_t		tgt1;
	m5P20_Ip1Tgt_t		tgt2;
	m5P20_Ip1Tgt_t		tgt3;
}m5P20_Ip1Go_t;


/////////////////
/// -- IP1 -- ///
/////////////////
#define  M5P20_IP1_GO_NUM	3
typedef struct{
	unsigned short		RR		:3;
	unsigned short		RU		:2;
	unsigned short		KCgo	:2;
	unsigned short		SGO		:1;
	unsigned short		pad0	:8;
	m5P20_Ip1Go_t		go[M5P20_IP1_GO_NUM];	
}m5P20_Ip1_t;

/////////////////
/// -- IP2 -- ///
/////////////////
typedef struct{
	unsigned short		Kgo		:3;
	unsigned short		SS		:2;
	unsigned short		Ant		:3;
	unsigned short		RIP		:2;
	unsigned short		Izl		:1;
	unsigned short		Gdr		:1;
	unsigned short		PrST	:1;
	unsigned short		S		:1;
	unsigned short		RU		:2;
	
	unsigned short		Na		:5;
	unsigned short		SHKD	:3;
	unsigned short		TS		:2;
	unsigned short		MP		:3;
	unsigned short		pad0	:3;
	
	unsigned short		Pa;
	unsigned short		Pcs;
}m5P20_Ip2_t;


typedef struct{
	unsigned short		PrL		:1;
	unsigned short		PRGO	:2;
	unsigned short		TIP		:4;
	unsigned short		SBR		:1;
	unsigned short		NC		:8;
	
	unsigned short		KRNC	:8;
	unsigned short		pad0	:8;
	
	unsigned short		D;
	unsigned short		P;
	unsigned short		K;
	unsigned short		tD;
	unsigned short		tP;
	unsigned short		tK;
	
	unsigned short		V		:10;
	unsigned short		tV		:6;

	unsigned short		Vr		:10;
	unsigned short		tVr		:6;			
}m5P20_Ip3Tgt_t;

/////////////////
/// -- IP3 -- ///
/////////////////
#define  _M5P20_IP3_TGT_NUM	3
typedef struct{
	m5P20_Ip3Tgt_t		tgt[_M5P20_IP3_TGT_NUM];	
}m5P20_Ip3_t;


typedef struct{
	unsigned short		NC		:8;
	unsigned short		pad0	:2;
	unsigned short		PRZ		:1;
	unsigned short		pad1	:2;
	unsigned short		NI		:3;
	
	unsigned short		D;
	unsigned short		P;
	unsigned short		K;
	unsigned short		V;
}m5P20_Ip4Tgt_t;

/////////////////
/// -- IP4 -- ///
/////////////////
#define  M5P20_IP4_TGT_NUM	4
typedef struct{
	m5P20_Ip4Tgt_t		tgt[M5P20_IP4_TGT_NUM];	
}m5P20_Ip4_t;

typedef struct {
	unsigned short 		mch_pri_state		:1;
	unsigned short		mch_res_state		:1;
}m5P20_State_t;

#define M5P20_IP3_TGT_NUM	21
typedef struct{
	m5P20_Ip1_t		ip1;
	m5P20_Ip2_t		ip2;
	m5P20_Ip3Tgt_t	ip3[M5P20_IP3_TGT_NUM];
	m5P20_Ip4_t		ip4;
	m5P20_State_t	state;
}IpMap5P20_t;

int m5P20GetIp2(int fd, m5P20_Ip2_t *ip);
int m5P20GetIp3(int fd, m5P20_Ip3_t *ip);
int m5P20GetIp4(int fd, m5P20_Ip4_t *ip);

//////////////////////////
//--      ctrl        --//
//////////////////////////
#define M5P20_CMD 			0
#define M5P20_GET_IP2 		__DIOF(_DCMD_MISC, M5P20_CMD + 1, m5P20_Ip2_t)
#define M5P20_GET_IP3 		__DIOF(_DCMD_MISC, M5P20_CMD + 2, m5P20_Ip3_t)
#define M5P20_GET_IP4 		__DIOF(_DCMD_MISC, M5P20_CMD + 3, m5P20_Ip4_t)

#endif /*L5P20_H_*/
