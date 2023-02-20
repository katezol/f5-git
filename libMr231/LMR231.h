#ifndef LMR231_H_
#define LMR231_H_

typedef struct{
	unsigned short 	num;		//4 номер цели
	float			dist;		//5  [mili] дист до цели
	float			pel;		//6  [grad] пел на цель
	char			pel_ok;		//7 признак истинного пеленга
	float			speed;		//8  [uzl] скорость
	float			cur;		//9  [grad] курс цели
	char			cur_ok;		//10 признак истинного курса
	float			dist_to;	//11 [mili] дистанция до точки сближения
	float			time_to;	//12 [min] время до точки сближения
	char			dist_type;	//13 признак измерения дистанции в милях
	char			tgt_type;	//14 признак опознавания цели
	char			tgt_status;	//15 статус цели
	char			pad;		//16 пустое поле
	unsigned long	north_time;	//17 [ms] время между сиг. север  и зондом цели
	char 			tgt_auto;	//18 признак автосопровождения
	unsigned short	cs;			//19 контрольная сумма 
}mr231Tgt_t;

typedef struct{
	float 	cur; 			//1 курс
	char 	cur_ok;			//2 истинный
	float 	cur_mag;		//3 курс магнитный
	char 	cur_mag_ok;		//4 истинный
	float 	speed; 			//5 [uzl] скорость
	char  	speed_type;		//6 узлы
	float 	speed_km; 		//7 [km/h] скорость в км
	char 	speed_km_type;	//8 км/ч
	unsigned short cs;		//9
}mr231Ship_t;

typedef struct{
	float 	start_dist_1;	//1  начальное расстояние 1
	float	start_pel;		//2  начальный пеленг
	float	cyc_dist_1;		//3  подвижный курс дальности
	float	pel_1;			//4  [grad] пеленг 1
	float	start_dist_2;	//5  начальное расстояние 2
	float	start_pel_2;	//6  начальный пеленг 2
	float	dist;			//7  VRM2 дистанция
	float   grad;			//8  EBL2 градусы
	float	dist_m;			//9  дальность от своего корабля
	float	pel_m;			//10 [grad] пеленг
	float	dist_step;		//11 шкала дальности
	char	dist_type;		//12 единицы измерения расстояния
	char	disp_pos;		//13 ориентация дисплея
	char	mode;			//14 режим работы НРЛС
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
