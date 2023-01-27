#include <sys/types.h>
#include <unistd.h>
#include <devctl.h>
#include <errno.h>

#include "l5P20.h"

int m5P20GetIp2(int fd, m5P20_Ip2_t *ip){
	return(errno = devctl (fd, M5P20_GET_IP2, ip, sizeof(m5P20_Ip2_t), NULL));
}

int m5P20GetIp3(int fd, m5P20_Ip3_t *ip){
	return(errno = devctl (fd, M5P20_GET_IP3, ip, sizeof(m5P20_Ip3_t), NULL));
}

int m5P20GetIp4(int fd, m5P20_Ip4_t *ip){
	return(errno = devctl (fd, M5P20_GET_IP4, ip, sizeof(m5P20_Ip4_t), NULL));
}
