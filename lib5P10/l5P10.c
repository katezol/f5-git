#include <sys/types.h>
#include <unistd.h>
#include <devctl.h>
#include <errno.h>

#include "l5P10.h"


int m5P10GetIp2(int fd, m5P10_Ip2_t *ip){
	return(errno = devctl (fd, M5P10_GET_IP2, ip, sizeof(m5P10_Ip2_t), NULL));
}

int m5P10GetIp3(int fd, m5P10_Ip3_t *ip){
	return(errno = devctl (fd, M5P10_GET_IP3, ip, M5P10_IP3_NUM * sizeof(m5P10_Ip3_t), NULL));
}