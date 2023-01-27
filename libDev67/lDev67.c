#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <devctl.h>

#include "lDev67.h"
#include "lDev67_ctrl.h"


int	dev67SetAnt(int fd,unsigned short speed,unsigned short acc){
	unsigned short value  = (speed&0x3) | ( (acc&0x1)<<2  );
	return(errno = devctl (fd, DEV67_CMD_SET_ANT, &value, sizeof(value), NULL));
}

int	dev67Enable(int fd,D67Mode_t* mode){
	return(errno = devctl (fd, DEV67_CMD_ENABLE, mode, sizeof(D67Mode_t), NULL));
}
int	dev67Disable(int fd){
	return(errno = devctl (fd, DEV67_CMD_DISABLE, NULL, 0, NULL));
}

int	dev67SetStrob(int fd,float begin, float end){
	d67strob_t st;
	st.begin 	= (unsigned short)( begin / (256.0/2048.0)  + 0.5); // begin/2^11
	st.end		= (unsigned short)( end / (256.0/2048.0) + 0.5); // begin/2^11
	return(errno = devctl (fd, DEV67_CMD_SET_STROB, &st, sizeof(d67strob_t), NULL));
}

int	dev67GetState(int fd,Dev67State_t* state){
	return(errno = devctl (fd, DEV67_CMD_GET_STATE, state, sizeof(Dev67State_t), NULL));
}

typedef struct{
	unsigned short 	type;
	Dev67IP_t		ip;
}Dev67Sub_t;

int	dev67GetIp(int fd,Dev67IP_t* ip){
	//Dev67Sub_t sub;
	d67sub_t sub;
	errno = devctl (fd, DEV67_CMD_GET_TGT, &sub, sizeof(d67sub_t), NULL);
	//*printf( "real error %s\n", strerror( errno ));
	if(EOK != errno )
		return(DEV67_IP_TYPE_ERR);
	//printf("type[%d]\n",sub.type);
	memcpy(ip, &(sub.ip),sizeof(Dev67IP_t));	
	return(sub.type);	
}
