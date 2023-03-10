/*
 * l3K96_f.h
 *
 *  Created on: 26.01.2015
 *      Author: 1
 */

#ifndef L3K96_F_H_
#define L3K96_F_H_

#include <devctl.h>
#include <stdbool.h>
#include "l3K96.h"

///////////////////////////////////////////////////

// РЛК --> IP2 --> 3K96-3//
// Информация по сопровождаемым РЛК объектам (до 150)
// со временем привязки сглаженных (или экстраполированных) данных к системному
// времени изделия 3К96-3,
// - типы объектов,
// - ЭПР,
// - признаки ГО,
// - признаки сопровождения:
// 		- сопровождение с признаком приоритетного объекта,
// 		- сопровождение со стандартной точностью,
// - среднеквадратичные отклонения координат и составляющих вектора скорости объекта.
// Информация выдается во всех режимах управления 3К96-3 (ЦР,РР,АР).

typedef struct{
	bool 	bPassive;			// признак помехопостановщика:
			// 1 - помехопостановщик;
			// 0 - эхосигнал.
	int		autoEscort;			// признаки сопровождения:
			// 1 - автосопровождение с признаком приоритетной цели;
			// 2 - автосопровождение со стандартной точностью.
	bool 	opr	;				// отсутствие привязки ко времени 3К96-3:
			// 0 - данные привязаны;
			// 1 - данные не привязаны.
	bool 	bExtrapol;			// признак экстраполированных данных, выдается объекту,
					    		// сопровождаемому по "памяти":
			// 0 - ;
			// 1 - отсутствие данных по объекту, сопровождение по "памяти".
	bool 	bLoss; 				// признак неустойчивого сопровождения:
			// 1 - наличие признака;
			// 0 - нет.
	int     nRlk;				// номер объекта в нумерации РЛК (1-150)
	int		own;				// признак ГО (признаки ГО выдаются по трассовой информации РЛК):
			// 1 - СВ - свой;
			// 2 - ЧЖ - чужой;
			// 0 - НП - неопознанный.
	int     epr;			 	// признак ЭПР:
			// 1 - МРЦ - малоразмерная (<1 м^2);
			// 2 - СРЦ - среднеразмерная (1...20 м^2);
			// 3 - КРЦ - крупноразмерная (>20 м^2).
	bool    bSlow; 				// признак малоподвижного объекта:
			// 1 - наличие признака;
			// 0 - нет.
	int 	typeTarget; // 1-нц 2-вц 3-нлц 4-лц 5-пкр 6-прр 7-верт 8-пн
	// Сглаженные (или экстраполированные) данные:

	float 	x_f;				// по оси Х (север)(при bPassive = 0), диапазон -150...+150 км, ц.мл.р. 5м, 15р.-зн.
								// при bPassive = 1 b - азимут (пеленг) помехопостановщика, диапазон 0-360°, ц.мл.р. 360°/2^16

	float 	y_f;				// по оси Y (восток)(при bPassive = 0), диапазон -150...+150 км, ц.мл.р. 5м, 15р.-зн.
								// при bPassive = 1 не используется

	float 	z_f;				// по оси Z (высота)(при bPassive = 0), диапазон 0...50 км, ц.мл.р. 1м
								// при bPassive = 1 е - угол места помехопостановщика, диапазон -90...+90°, ц.мл.р. 180°/2^16, 15р.-зн.

	float 	skoVx_f; 			// среднеквадратичные отклонения составляющих вектора скорости объекта,
								// диапазон 0...+240 м/с, ц.мл.р. 5 м/с
	float 	vx_f;				// скорость изменения координаты по оси Х (север) , (при bPassive = 0),
								// диапазон -2500...+2500 м/с, ц.мл.р. 5 м/с, 15р.-зн.
								// при bPassive = 1 Vb - скорость изменения азимута (пеленга) помехопостановщика,
								// диапазон -2...+2 рад/с, ц.мл.р. 5мрад/с, 15р.-зн.

	float 	skoVy_f;			// среднеквадратичные отклонения составляющих вектора скорости объекта,
								// диапазон 0...+240 м/с, ц.мл.р. 5 м/с
	float 	vy_f;				// скорость изменения координаты по оси Y (восток), (при bPassive = 0),
								// диапазон -2500...+2500 м/с, ц.мл.р. 5 м/с, 15р.-зн.
								// при bPassive = 1 не используется

	float 	skoVz_f;			// среднеквадратичные отклонения составляющих вектора скорости объекта,
								// диапазон 0...+240 м/с, ц.мл.р. 5 м/с
	float 	vz_f;				// скорость изменения координаты по оси Z (высота), (при bPassive = 0),
								// диапазон -2500...+2500 м/с, ц.мл.р. 5 м/с, 15р.-зн.
								// при bPassive = 1 Ve - скорость изменения угла места помехопостановщика,
								// диапазон -2...+2 рад/с, ц.мл.р. 5мрад/с, 15р.-зн.

	float 	skoX_f;				// среднеквадратичные отклонения координат объекта,
								// диапазон 0...1000 м, ц.мл.р. 1 м

	float 	skoY_f;				// среднеквадратичные отклонения координат объекта,
								// диапазон 0...1000 м, ц.мл.р. 1 м

	float 	skoZ_f;				// среднеквадратичные отклонения координат объекта,
								// диапазон 0...1000 м, ц.мл.р. 1 м

	int	  	time_3k96;			// время привязки сглаженных (или экстраполированных) данных к системному времени 3К96-3
								// диапазон 0...86,4*10^6 мс, ц.мл.р. 1 мс,
								// действует при наличии признака ОПр = 0
}Target_IP2_3k96_tgt;


///////////////////////////////////////////////////

// РЛК --> IP3 --> 3K96-3//
// - команды ЦУ;
// - тип источника ЦУ;
// - информация по объектам, предназначенным для обслуживания изделием 3К96-3
// (определяется количеством БЗ, но не более 6).
// ИП3 выдается в режиме управления РР 3К96-3.

typedef struct{
	int     nX; 				// номер объекта в нумерации РЛС-Х 
	int 	nRlk; 				// номер объекта в нумерации РЛК (1-150)
	int 	nOpt; 				// номер объекта в нумерации Оптики
	int 	nEco; 				// номер объекта в нумерации Оптики

	int 	typeSource;	 		// тип источника ЦУ:
			// 5 - РЛС-Х;
			// 2 - ОЭС;
			// 4 - РЛК.
	int 	nF;				// номер формуляра ЦУ (1-6)
	int		autoEscort; 		// признаки сопровождения:
			// 1 - автосопровождение с признаком приоритетной цели;
			// 2 - автосопровождение со стандартной точностью.
	int 	opr; 				// отсутствие привязки данных ко времени 3К96-3:
			// 0 - данные привязаны;
			// 1 - данные не привязаны.
	bool 	bExtrapol; 			// признак экстраполированных данных, выдается по объекту,
								// сопровождаемому по "памяти":
			// 0 - ;
			// 1 - отсутствие данных по объекту, сопровождение по "памяти".
	bool 	bLoss; 				// признак неустойчивого сопровождения:
			// 1 - наличие признака;
			// 0 - нет.
	int     stateCommandCu; 	// признак обслуживания объекта (в РР):
			// 1 - принять ЦУ;
			// 2 - сбросить ЦУ;
			// 0 - отсутствие ЦУ.
	int 	nOES; 				// номер объекта в нумерации ОЭС (1-50)
	int		own; 				// признак ГО:
			// 1 - СВ - свой;
			// 2 - ЧЖ - чужой;
			// 0 - НП - неопознанный.
	bool 	bRefuseAccepted; 	// отказ от цели принят:
			// 1 - наличие признака;
			// 0 - отсутствие.
	int 	epr; 				// признак ЭПР:
			// 1 - МРЦ - малоразмерная (<1 м^2);
			// 2 - СРЦ - среднеразмерная (1...20 м^2);
			// 3 - КРЦ - крупноразмерная (>20 м^2).
	bool 	bSlow; 				// признак малоподвижного объекта:
			// 1 - наличие признака;
			// 0 - нет.
	int 	typeTarget; // 1-нц 2-вц 3-нлц 4-лц 5-пкр 6-прр 7-верт 8-пн
	// Сглаженные (или экстраполированные) значения координат и параметров движения
	// в прямоугольной системе координат:

	float 	x_f;				// по оси Х (север),  диапазон -150...+150 км, ц.мл.р. 5м, 15р.-зн.
	float 	y_f;				// по оси Y (восток), диапазон -150...+150 км, ц.мл.р. 5м, 15р.-зн.
	float 	z_f;				// по оси Z (высота), диапазон    0...  50 км, ц.мл.р. 1м

	float 	skoVx_f; 			// среднеквадратичные отклонения составляющих вектора скорости объекта,
								// диапазон 0...+240 м/с, ц.мл.р. 5 м/с
	float 	vx_f;				// скорость изменения координаты по оси Х (север),
								// диапазон -2500...+2500 м/с, ц.мл.р. 5 м/с, 15р.-зн.

	float 	skoVy_f; 			// среднеквадратичные отклонения составляющих вектора скорости объекта,
								// диапазон 0...+240 м/с, ц.мл.р. 5 м/с
	float 	vy_f;				// скорость изменения координаты по оси Y (восток),
								// диапазон -2500...+2500 м/с, ц.мл.р. 5 м/с, 15р.-зн.

	float 	skoVz_f; 			// среднеквадратичные отклонения составляющих вектора скорости объекта,
								// диапазон 0...+240 м/с, ц.мл.р. 5 м/с
	float 	vz_f;				// скорость изменения координаты по оси Z (высота),
								// диапазон -2500...+2500 м/с, ц.мл.р. 5 м/с, 15р.-зн.

	float 	skoX_f;				// среднеквадратичные отклонения координат объекта,
								// диапазон 0...1000 м, ц.мл.р. 1 м

	float 	skoY_f;				// среднеквадратичные отклонения координат объекта,
								// диапазон 0...1000 м, ц.мл.р. 1 м

	float 	skoZ_f;				// среднеквадратичные отклонения координат объекта,
								// диапазон 0...1000 м, ц.мл.р. 1 м

	int	time_3k96;				// время привязки сглаженных (или экстраполированных) данных к системному времени 3К96-3
								// диапазон 0...86,4*10^6 мс, ц.мл.р. 1 мс,
								// действует при наличии признака ОПр = 0

}Target_IP3_3k96_tgt;


///////////////////////////////////////////////////

// 3К96-3 --> IP11 --> РЛК//
// Команды в РЛК по объектам и информация о БД по объектам, обслуживаемым изделием 3К96-3 (по 1-6 объектам), включающая:
// - номер объекта в нумерации РЛК, ОЭС, РЛС-Х;
// - тип источника ЦУ;
// - признаки состояния целевого канала (ПСК):
//   	- обслуживание закончено;
//   	- ЦУ принимается;
//   	- пуск.
// - признак отказа от объекта;
// - признаки действий РЛК по данному объекту:
//   	- сопровождать;
//   	- сопровождать с признаком приоритетного объекта.
// Информация выдается во всех режимах управления 3К96-3 (ЦР,РР,АР).

typedef struct{
	int     nX; 				// номер объекта в нумерации РЛС-Х 
	int 	nRlk; 				// номер объекта в нумерации РЛК (1-150)
	int 	nOpt; 				// номер объекта в нумерации Оптики
	int 	nEco; 				// номер объекта нашего ЕЦО 
	unsigned short 	nSigma;		// номер объекта в нумерации КРС или СУ(ЦР/ РР-НЦ)	
	unsigned short 	prefixSigma;// префикс- тип цели КРС или СУ(ЦР/ РР-НЦ)
	//1-М надв 2-С подв 3-А возд 4-Б берег 5-Р РТО 6 Х ручн 7-Ю граф 8- У учебн 9- Г групп
	int 	typeSource;			// тип источника ЦУ:
			// 5 - РЛС-Х;
			// 2 - ОЭС;
			// 4 - РЛК;
			// 6 - КРС (выдается в ЦР).
	int 	nF; 				// номер формуляра ЦУ (1-6)
	bool 	bRefuse; 			// признак "Отказ от объекта" (в РР):
			// 1 - наличие признака;
			// 0 - нет.
	int 	pd; 				// признаки действий РЛК:
			// 01 - ЦС    - сопровождать;
			// 11 - ЦС-ПТ - сопровождать с признаком приоритетного объекта.
	int		stateChannel; 		// признак состояния целевого канала:
			// 00 - обслуживание закончено;
			// 01 - ЦУ принимается;
			// 10 - пуск;
			// 11 - радиокоррекция (при наличии ЛРК).
	float	T1;					// расчетное время точки встречи 1 изделия
								// диапазон 0...200 с, ц.мл.р. 0,1 с
	float 	x1_f;				// координата Х1 точки встречи 1 изделия (север),
								// диапазон -80...+80 км, ц.мл.р. 5 м, 15р.-зн.
	float 	y1_f;				// координата Y1 точки встречи 1 изделия (восток),
								// диапазон -80...+80 км, ц.мл.р. 5 м, 15р.-зн.
	float 	z1_f;				// координата Z1 точки встречи 1 изделия (высота),
								// диапазон 0...30 км, ц.мл.р. 1 м, 15р.-зн.
	float   T2;					// расчетное время точки встречи 2 изделия
								// диапазон 0...200 с, ц.мл.р. 0,1 с
	float 	x2_f;				// координата Х2 точки встречи 2 изделия (север),
								// диапазон -80...+80 км, ц.мл.р. 5 м, 15р.-зн.
	float 	y2_f;				// координата Y2 точки встречи 2 изделия (восток),
								// диапазон -80...+80 км, ц.мл.р. 5 м, 15р.-зн.
	float 	z2_f;				// координата Z2 точки встречи 2 изделия (высота),
								// диапазон 0...30 км, ц.мл.р. 1 м, 15р.-зн.

}Target_IP11_3k96_tgt;

/**
Преобразует  m3K96_Ip2_t в Target_IP2_3K96
@param  *t_ip2   - структура m3K96_Ip2_t
@param  *t_data2 - структура Target_IP2_3K96
@param  num_el  - количество элементов для преобразования*/
//void conv_read_3K96_IP2(m3K96_Ip2_t *t_ip2, Target_IP2_3K96    *t_data2, int num_el);

/**
Преобразует  m3K96_Ip3_t в Target_IP3_3K96
@param  *t_ip3   - структура m3K96_Ip3_t
@param  *t_data3 - структура Target_IP3_3K96
@param  num_el  - количество элементов для преобразования*/
//void conv_read_3K96_IP3(m3K96_Ip3_t *t_ip3, Target_IP3_3K96    *t_data3, int num_el);

/**
Преобразует  m3K96_Ip5_t в f3K96_Ip5
@param  *t_ip5   - структура m3K96_Ip5_t
@param  *t_data5 - структура f3K96_Ip5
@param  num_el  - количество элементов для преобразования*/

/*
void conv_read_3K96_IP5(m3K96_Ip5_t *t_ip5, f3K96_Ip5    *t_data5, int num_el);

/**
Преобразует  m3K96_Ip11_t в Target_IP11_3K96
@param  *t_ip11   - структура m3K96_Ip11_t
@param  *t_data11 - структура Target_IP11_3K96
@param  num_el    - количество элементов для преобразования*/
void conv_read_3K96_IP11_OneTarget(m3K96_Ip11Tgt_t *t_ip11, Target_IP11_3k96_tgt    *t_data11);

/**
Преобразует Target_IP1_3K96 в m3K96_Ip1_t
@param  *t_ip1   - структура m3K96_Ip1_t
@param  *t_data1 - структура Target_IP1_3K96
@param  num_el    - количество элементов для преобразования*/
//void conv_write_3K96_IP1(const Target_IP1_3K96 *t_data1, m3K96_Ip1_t *t_ip1,int num_el);

/**
Преобразует Target_IP2_3K96 в m3K96_Ip2_t
@param  *t_ip2   - структура m3K96_Ip2_t
@param  *t_data2 - структура Target_IP2_3K96
@param  num_el  - количество элементов для преобразования*/
void conv_write_3K96_IP2_OneTarget(const Target_IP2_3k96_tgt *t_data2, m3K96_Ip2Tgt_t *t_ip2);

/**
Преобразует Target_IP3_3K96 в m3K96_Ip3_t
@param  *t_ip3   - структура m3K96_Ip3_t
@param  *t_data3 - структура Target_IP3_3K96
@param  num_el  - количество элементов для преобразования*/
void conv_write_3K96_IP3_OneTarget(const Target_IP3_3k96_tgt *t_data3, m3K96_Ip3Tgt_t *t_ip3);

/**
Преобразует f3K96_Ip5 в m3K96_Ip5_t
@param  *t_ip5   - структура m3K96_Ip5_t
@param  *t_data5 - структура f3K96_Ip5
@param  num_el  - количество элементов для преобразования*/

/*

void conv_write_3K96_IP5(const f3K96_Ip5 *t_data5, m3K96_Ip5_t *t_ip5,int num_el);

/**
Преобразует Target_IP11_3K96 в m3K96_Ip11_t
@param  *t_ip11   - структура m3K96_Ip11_t
@param  *t_data11 - структура Target_IP11_3K96
@param  num_el    - количество элементов для преобразования*/
//void conv_write_3K96_IP11(const Target_IP11_3K96 *t_data11, m3K96_Ip11_t *t_ip11,int num_el);


#endif /* L3K96_F_H_ */
