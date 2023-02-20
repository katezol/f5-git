/*
 * lSfera.c
 *
 *  Created on: 20.11.2014
 *      Author: 1
 */

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "lSfera.h"

IpMapSfera_t map;

int mSferaSetIp1(int fd, mSfera_Ip1_t *ip){
	return(errno = devctl (fd, MSFERA_SET_IP1, ip, sizeof(mSfera_Ip1_t), NULL));
}

int mSferaGetIp1(int fd, mSfera_Ip1_t *ip){
	return(errno = devctl (fd, MSFERA_GET_IP1, ip, sizeof(mSfera_Ip1_t), NULL));

}

int mSferaSetIp2(int fd, mSfera_Ip2AllTargets *ip){
	return(errno = devctl (fd, MSFERA_SET_IP2, ip, 
			sizeof(mSfera_Ip2AllTargets), NULL));
}

int mSferaGetIp2(int fd, mSfera_Ip2AllTargets *ip){
	return(errno = devctl (fd, MSFERA_GET_IP2, ip, 
			sizeof(mSfera_Ip2AllTargets), NULL));
}

int mSferaSetIp3(int fd, mSfera_Ip3AllTargets *ip){
	return(errno = devctl (fd, MSFERA_SET_IP3, ip, 
			sizeof(mSfera_Ip3AllTargets), NULL));
}

int mSferaGetIp3(int fd, mSfera_Ip3AllTargets *ip){
	return(errno = devctl (fd, MSFERA_GET_IP3, ip, 
			sizeof(mSfera_Ip3AllTargets), NULL));
}

int mSferaSetIp4(int fd, mSfera_Ip4_t *ip){
	return(errno = devctl (fd, MSFERA_SET_IP4, ip, sizeof(mSfera_Ip4_t), NULL));
}

int mSferaGetIp4(int fd, mSfera_Ip4_t *ip){
	return(errno = devctl (fd, MSFERA_GET_IP4, ip, sizeof(mSfera_Ip4_t), NULL));
}

int mSferaSetIp5(int fd, mSfera_Ip5AllTargets *ip){	
	return(errno = devctl (fd, MSFERA_SET_IP5, ip, sizeof(mSfera_Ip5AllTargets), NULL));
}

int mSferaGetIp5(int fd, mSfera_Ip5AllTargets *ip){
	return(errno = devctl (fd, MSFERA_GET_IP5, ip, sizeof(mSfera_Ip5AllTargets), NULL));
}

int mSferaSetIp6(int fd, mSfera_Ip6AllTargets *ip){
	return(errno = devctl (fd, MSFERA_SET_IP6, ip, 	sizeof(mSfera_Ip6AllTargets), NULL));
}

int mSferaGetIp6(int fd, mSfera_Ip6AllTargets *ip){
	return(errno = devctl (fd, MSFERA_GET_IP6, ip, 	sizeof(mSfera_Ip6AllTargets), NULL));
}


int mSferaGetState(int fd, mSfera_State_t *ip){
	return(errno = devctl (fd, MSFERA_GET_STATE, ip, sizeof(mSfera_State_t), NULL));

}

