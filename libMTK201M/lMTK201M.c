#include <sys/types.h>
#include <unistd.h>
#include <devctl.h>
#include <errno.h>

#include "lMTK201M.h"

int mMTKGetIp3(int fd, mMTK_Ip3_t *ip){
	return(errno = devctl (fd, MMTK_GET_IP3, ip, sizeof(mMTK_Ip3_t), NULL));
}

int mMTKGetIp4(int fd, mMTK_Ip4Tgt_t *ip){
	return(errno = devctl (fd, MMTK_GET_IP4, ip, MTK_TARGET_NUM*sizeof(mMTK_Ip4Tgt_t), NULL));
}
