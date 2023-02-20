#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "lMR123.h"

int mMR123SetIp1(int fd, mMR123_Ip1_t *ip){
	return(errno = devctl (fd, MMR123_SET_IP1, ip, sizeof(mMR123_Ip1_t), NULL));
}

int mMR123GetIp1(int fd, mMR123_Ip1_t *ip){
	return(errno = devctl (fd, MMR123_GET_IP1, ip, sizeof(mMR123_Ip1_t), NULL));
}


int mMR123SetIp2(int fd, mMR123_Ip2_t *ip){
	return(errno = devctl (fd, MMR123_SET_IP2, ip, sizeof(mMR123_Ip2_t), NULL));
}

int mMR123GetIp2(int fd, mMR123_Ip2_t *ip){
	return(errno = devctl (fd, MMR123_GET_IP2, ip, sizeof(mMR123_Ip2_t), NULL));
}

int mMR123GetState(int fd, mMR123_State_t *ip){
	return(errno = devctl (fd, MMR123_GET_STATE, ip, sizeof(mMR123_State_t), NULL));
}
