#ifndef LMR231_H_
#define LMR231_H_

typedef struct{
	unsigned short 	num;		//4 ����� ����
	float			dist;		//5  [mili] ���� �� ����
	float			pel;		//6  [grad] ��� �� ����
	char			pel_ok;		//7 ������� ��������� �������
	float			speed;		//8  [uzl] ��������
	float			cur;		//9  [grad] ���� ����
	char			cur_ok;		//10 ������� ��������� �����
	float			dist_to;	//11 [mili] ��������� �� ����� ���������
	float			time_to;	//12 [min] ����� �� ����� ���������
	char			dist_type;	//13 ������� ��������� ��������� � �����
	char			tgt_type;	//14 ������� ����������� ����
	char			tgt_status;	//15 ������ ����
	char			pad;		//16 ������ ����
	unsigned long	north_time;	//17 [ms] ����� ����� ���. �����  � ������ ����
	char 			tgt_auto;	//18 ������� �����������������
	unsigned short	cs;			//19 ����������� ����� 
}mr231Tgt_t;

typedef struct{
	float 	cur; 			//1 ����
	char 	cur_ok;			//2 ��������
	float 	cur_mag;		//3 ���� ���������
	char 	cur_mag_ok;		//4 ��������
	float 	speed; 			//5 [uzl] ��������
	char  	speed_type;		//6 ����
	float 	speed_km; 		//7 [km/h] �������� � ��
	char 	speed_km_type;	//8 ��/�
	unsigned short cs;		//9
}mr231Ship_t;

typedef struct{
	float 	start_dist_1;	//1  ��������� ���������� 1
	float	start_pel;		//2  ��������� ������
	float	cyc_dist_1;		//3  ��������� ���� ���������
	float	pel_1;			//4  [grad] ������ 1
	float	start_dist_2;	//5  ��������� ���������� 2
	float	start_pel_2;	//6  ��������� ������ 2
	float	dist;			//7  VRM2 ���������
	float   grad;			//8  EBL2 �������
	float	dist_m;			//9  ��������� �� ������ �������
	float	pel_m;			//10 [grad] ������
	float	dist_step;		//11 ����� ���������
	char	dist_type;		//12 ������� ��������� ����������
	char	disp_pos;		//13 ���������� �������
	char	mode;			//14 ����� ������ ����
	unsigned short	cs;		//15
}mr231State_t;


typedef union{
	mr231Tgt_t		tgt;
	mr231Ship_t		ship;
	mr231State_t	state;
}mr231IP_t;

#define MR231_TYPE_1	1
#define MR231_TYPE_3	2

#define MR231_IPTYPE_ERR	0
#define MR231_IPTYPE_TGT	1
#define MR231_IPTYPE_SHIP	2
#define MR231_IPTYPE_STATE	3
int mr231Open(char* devmame);
//return one of define and full ip if no err
#define MR231_DATA_BLOK_READ_SIZE	1
#define MR231_WAIT_DELAY			100
int mr231ReadIp(int fd, mr231IP_t* ip, int type);

#endif /*LMR231_H_*/
