#ifndef LDEV67_H_
#define LDEV67_H_



#define _DEV67_DEV_NAME "/dev/DEV67"

#define DEV_67_ANT_SPEED_60		0x3
#define DEV_67_ANT_SPEED_30		0x2
#define DEV_67_ANT_SPEED_6		0x1
#define DEV_67_ANT_SPEED_03		0x0
#define DEV_67_ANT_ACC_ON		0x1
#define DEV_67_ANT_ACC_OFF		0x0
int	dev67SetAnt(int fd,unsigned short speed,unsigned short acc);

#define DEV_67_MODE_MODE_1		0x1
#define DEV_67_MODE_MODE_2		0x2
#define DEV_67_MODE_MODE_4		0x4
#define DEV_67_MODE_MODE_OFF	0x0

#define DEV_67_MODE_LINE_AIR		0x2
#define DEV_67_MODE_LINE_SHIP		0x1

typedef struct{
	unsigned short 		lowtgt		:1;
	unsigned short 		varo		:1;
	unsigned short 		stnum		:3;
	unsigned short 		line		:2;
	unsigned short 		mode		:3;
	unsigned short		pad			:6;
}D67Mode_t;
int	dev67Enable(int fd,D67Mode_t* mode);
int	dev67Disable(int fd);

int	dev67SetStrob(int fd,float begin, float end);
//////////////////////////////////////////////////////////////////////////

#define DEV67_ERR_OK	0
#define DEV67_ERR_PAR	1
#define DEV67_ERR_ERR	2

typedef struct{
	unsigned short 	err		:8;
	unsigned short 	state	:1; // 1 - исправно
	unsigned short	PP		:1; //1 - признак поступления пелега от 5П-27
	unsigned short	RR		:1;	//1 - признак ручного режима
	unsigned short	mode	:3;  // 1-I, 2-II, 4-IV
	unsigned short	line	:2;// 1-корабль, 2-самолет	
}Dev67State_t;
int	dev67GetState(int fd,Dev67State_t* state);

typedef struct{
	unsigned short	B		:1;//,бедствие
	unsigned short	PN		:1;//,признак поступления номера цели (только в 4м режиме)
	unsigned short	num;//,номер цели (только в 4м режиме)
	float	pel;//градусы
	float	dist;// километры
}Dev67Tgt_t;

typedef struct{
	unsigned short	state67	:1;
	float			pel;
		
}Dev67Alarm_t;



typedef union{
	Dev67Tgt_t		tgt;
	Dev67Alarm_t	alarm;
}Dev67IP_t;
#define DEV67_IP_TYPE_ERR		0
#define DEV67_IP_TYPE_ALARM		1
#define DEV67_IP_TYPE_TGT		2
int	dev67GetIp(int fd,Dev67IP_t* ip);




#endif /*LDEV67_H_*/
