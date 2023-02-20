/*
 * lGnome.c
 *
 *  Created on: 02.11.2011
 *      Author: drd
 */

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include "lGnome.h"

int GnomeGetTime(int fd, GnomeTime_t *ip){
	return(errno = devctl(fd, GNOME_GET_TIME, ip, sizeof(GnomeTime_t), NULL));
}

int GnomeSetTime(int fd, GnomeTime_t *ip){
	return(errno = devctl(fd, GNOME_SET_TIME, ip, sizeof(GnomeTime_t), NULL));
}

int GnomeGetState(int fd, GnomeState_t *ip){
	return(errno = devctl(fd, GNOME_GET_STATE, ip, sizeof(GnomeState_t), NULL));
}
