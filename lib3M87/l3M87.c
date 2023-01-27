/*
 * l3M87.c
 *
 *  Created on: 18.03.2015
 *      Author: drd
 */

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "l3M87.h"

int m3M87SetIp1(int fd, m3M87_Ip1_t *ip){
	return(errno = devctl (fd, M3M87_SET_IP1, ip, sizeof(m3M87_Ip1_t), NULL));
}

int m3M87GetIp1(int fd, m3M87_Ip1_t *ip){
	return(errno = devctl (fd, M3M87_GET_IP1, ip, sizeof(m3M87_Ip1_t), NULL));
}


int m3M87SetIp2(int fd, m3M87_Ip2_t *ip){
	return(errno = devctl (fd, M3M87_SET_IP2, ip, sizeof(m3M87_Ip2_t), NULL));
}

int m3M87GetIp2(int fd, m3M87_Ip2_t *ip){
	return(errno = devctl (fd, M3M87_GET_IP2, ip, sizeof(m3M87_Ip2_t), NULL));
}

int m3M87SetIp3(int fd, m3M87_Ip3_t *ip){
	return(errno = devctl (fd, M3M87_SET_IP3, ip, sizeof(m3M87_Ip3_t), NULL));
}

int m3M87GetIp3(int fd, m3M87_Ip3_t *ip){
	return(errno = devctl (fd, M3M87_GET_IP3, ip, sizeof(m3M87_Ip3_t), NULL));
}

int m3M87GetState(int fd, m3M87_State_t *ip){
	return(errno = devctl (fd, M3M87_GET_STATE, ip, sizeof(m3M87_State_t), NULL));
}
