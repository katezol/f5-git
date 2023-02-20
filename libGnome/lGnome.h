#include <devctl.h>

/*
 * lGnome.h
 *
 *  Created on: 02.11.2011
 *      Author: drd
 */

#ifndef LGNOME_H_
#define LGNOME_H_

#define _GNOME_DEV_NAME	"/dev/Gnome"
#define _IMGNOME_DEV_NAME	"/dev/IMGnome"

typedef struct{
	unsigned short mon_un	:4;		///< месяц года,   единицы
	unsigned short mon_tn	:4;		///< месяц года,   десятки
	unsigned short year_un	:4;		///< год столетия, единицы
	unsigned short year_tn	:4;		///< год столетия, десятки

	unsigned short hg_un	:4;		///< час гринвичского времени, единицы
	unsigned short hg_tn	:4;		///< час гринвичского времени, десятки
	unsigned short day_un	:4;		///< число месяца,             единицы
	unsigned short day_tn	:4;		///< число месяца,			 десятки

	unsigned short hp_un	:4;		///< час поясного времени,	 единицы
	unsigned short hp_tn	:4;		///< час поясного времени,	 десятки
	unsigned short hm_un	:4;		///< час московского времени,	 единицы
	unsigned short hm_tn	:4;		///< час московского времени,	 десятки

	unsigned short sec_un	:4;		///< секунда,	  единицы
	unsigned short sec_tn	:4;		///< секунда,	  десятки
	unsigned short min_un	:4;		///< минута,	  единицы
	unsigned short min_tn	:4;		///< минута,	  десятки

	unsigned short D		:1;		/**< признак достоверности хронометрической информации:
				    1-информация достоверна;
				    0-информация недостоверна.*/
	unsigned short m5		:1;		/**< "1" означает, что сигнал "XT" следующего
				    информационного сообщения соответствует моменту появления
				    пятиминутного синхроимпульса (при этом 16 и 17 разряды также "1").*/
	unsigned short m1		:1;		/**< "1" означает, что сигнал "XT" следующего
	   			    информационного сообщения соответствует моменту появления
	   			    минутного синхроимпульса (при этом 16 разряд также "1").*/
	unsigned short s1		:1;		/**< "1" означает, что сигнал "XT" следующего
	   			    информационного сообщения соответствует моменту появления
	   			    секундного синхроимпульса.*/
	unsigned short fs_10	:4;		///< доли секунды, тысячные
	unsigned short fs_100	:4;		///< доли секунды, сотые
	unsigned short fs_1000	:4;		///< доли секунды, десятые

	unsigned short SMEG		:1;		/**< "1" означает, что в системе зафиксирован
				    отказ по смежной ЛПИ.*/
	unsigned short SVOI		:1;		/**< "1" означает, что в системе зафиксирован
	   			    отказ по ЛПИ своего канала.*/
	unsigned short REZ		:1;		/**< "1" означает, что в системе зафиксирован
	   			    отказ резерва, то есть отказ, не влекущий за собой прекращения
				    трансляции потребителям хронометрической информации.*/
	unsigned short pad		:13;	///< 0
}GnomeTime_t;

typedef struct{
	unsigned char	pri;
	unsigned char	res;
}GnomeState_t;

typedef struct{
	GnomeTime_t		gnome_time;
	GnomeState_t    state;
}IpMapGnome_t;



/**
Принимает ИП от устройства
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП
@return  EOK в случае успеха иначе статус ошибки*/
int GnomeGetTime(int fd, GnomeTime_t *ip);
/**
Посылает ИП в устройство
@param  fd - дескриптор устпойства
@param   *ip  указатель на ИП
@return  EOK в случае успеха иначе статус ошибки*/
int GnomeSetTime(int fd, GnomeTime_t *ip);
/**
Принимает State от устройства
@param  fd - дескриптор устпойства
@param   *ip  указатель на State
@return  EOK в случае успеха иначе статус ошибки*/
int GnomeGetState(int fd, GnomeState_t *ip);

#define GNOME_GET_TIME		__DIOF(_DCMD_MISC, 1, GnomeTime_t)
#define GNOME_SET_TIME		__DIOT(_DCMD_MISC, 2, GnomeState_t)
#define GNOME_GET_STATE		__DIOF(_DCMD_MISC, 3, GnomeState_t)

#define IMGNOME_SET_TIME		__DIOT(_DCMD_MISC, 4, GnomeTime_t)
#define IMGNOME_GET_STATE		__DIOF(_DCMD_MISC, 5, GnomeState_t)

#endif /* LGNOME_H_ */
