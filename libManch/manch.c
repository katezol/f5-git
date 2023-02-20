#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <devctl.h>

#include <ManchCtl.h>
#include <lmanch.h>


int	BcTest(int fd){
	return(errno = devctl (fd, MR_CMD_TEST, NULL, 0, NULL));
}

//////////////////////
//        BC        //
//////////////////////
int	BcOpen(const char* dev){
	int fd = 0;
	fd = open(dev,O_RDWR);
	if(-1 != fd){
		errno = devctl (fd, MR_CMD_BC_CONFIG,NULL, 0, NULL);
		if(EOK != errno)
			return(-1);
		return(fd);
	}
	return(-1);
}

int	BcClose(int fd){
	return(close(fd));
}	

int	BcSetErrMask(int fd, BcErrMask_t* mask){
	return(errno = devctl (fd, MR_CMD_BC_SET_ERR_MASK, mask, sizeof(BcErrMask_t), NULL));
}

int	BcSetChannel(int fd, unsigned short ch){
	return(errno = devctl (fd, MR_CMD_BC_SET_CHANNEL, &ch, sizeof(unsigned short), NULL));
}

int	BcSetAddress(int fd, unsigned short addr){
	return(errno = devctl (fd, MR_CMD_BC_SET_ADDRESS, &addr, sizeof(unsigned short), NULL));
}

int	BcSetSubAddress(int fd, unsigned short subaddr){
	return(errno = devctl (fd, MR_CMD_BC_SET_SUBADDRESS, &subaddr, sizeof(unsigned short), NULL));
}

int BcCmd(int fd, unsigned short cmd){
	return(errno = devctl (fd, MR_CMD_BC_CMD_RULE, &cmd, sizeof(unsigned short), NULL));
}

int BcCmdWD(int fd, unsigned short cmd, unsigned short *data){
	unsigned short _data[2] = {cmd, *data};
	return(errno = devctl (fd, MR_CMD_BC_CMD_RULE_WD, _data, 2*sizeof(unsigned short), NULL));
}
int BcCmdRD(int fd, unsigned short cmd, unsigned short *data){
	unsigned short _data[2] = {cmd, NULL};
	errno = devctl (fd, MR_CMD_BC_CMD_RULE_RD, _data, 2*sizeof(unsigned short), NULL);
	memcpy(data,&_data[1],sizeof(unsigned short));
	return(errno);
}

int BcWrite(int fd, void* data, unsigned short size){
	return(errno = devctl (fd, MR_CMD_BC_CMD_WRITE, data, size, NULL));
}

int BcRead(int fd, void* data, unsigned short size){
	return(errno = devctl (fd, MR_CMD_BC_CMD_READ, data, size, NULL));
}

int BcGetAW(int fd, BcAnswerWord_t* aw){
	return(errno = devctl (fd, MR_CMD_BC_GET_AW, aw, sizeof(BcAnswerWord_t), NULL));
}
int BcGetSW(int fd, BcStateWord_t* sw){
	return(errno = devctl (fd, MR_CMD_BC_GET_SW, sw, sizeof(BcStateWord_t), NULL));
}


BcFrame_t BcFrameCreate(){
	return(malloc(2*BC_RAM_SIZE));
}

int BcFrameStart(int fd, BcFrame_t cc){
	errno = devctl (fd, MR_CMD_BC_FRAME, cc, 2*cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE], NULL);
	return(errno);
}

int BcFrameReset(BcFrame_t cc){
	memset(cc,0,BC_RAM_SIZE);
	cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE] = BC_FRAME_MEM_HEAD_LEN;
	return(EOK);
}

int BcFrameCmd(BcFrame_t cc,unsigned short addr, unsigned short cmd){
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = 4;
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = BC_FRAME_CMD_TYPE_RULE;
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = addr;
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = cmd;
	cc[BC_FRAME_MEM_ADDR_CMD_CNT]++;
	if(BC_RAM_SIZE <= cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]){
		return(errno = ENOMEM);
	}
	return(EOK);
}

int BcFrameCmdWD(BcFrame_t cc, unsigned short addr, unsigned short cmd, unsigned short* data){
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = 5;
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = BC_FRAME_CMD_TYPE_RULE_WD;
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = addr;
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = cmd;
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = *data;
	cc[BC_FRAME_MEM_ADDR_CMD_CNT]++;
	if(BC_RAM_SIZE <= cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]){
		return(errno = ENOMEM);
	}
	return(EOK);

}

int BcFrameWrite(BcFrame_t cc, unsigned short addr, unsigned short subaddr, void* data, unsigned short size){
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = (size+1)/2 + 1+1+1+1+1;
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = BC_FRAME_CMD_TYPE_DATA_WD;
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = addr;
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = subaddr;
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = (size+1)/2;
	memcpy(&cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]],data,size);
	cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE] += (size+1)/2;
	cc[BC_FRAME_MEM_ADDR_CMD_CNT]++;
	if(BC_RAM_SIZE <= cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]){
		return(errno = ENOMEM);
	}
	return(EOK);

}

int BcFrameDelay(BcFrame_t cc, unsigned long delay){
	delay /=10;
	if(!delay )
		delay = 1;
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = 1+1+1;
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = BC_FRAME_CMD_TYPE_DELAY;
	cc[cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]++] = (unsigned short)delay;
	cc[BC_FRAME_MEM_ADDR_CMD_CNT]++;
	if(BC_RAM_SIZE <= cc[BC_FRAME_MEM_ADDR_TOTAL_SIZE]){
		return(errno = ENOMEM);
	}
	return(EOK);

}

BcFrame_t BcFrameDestroy(BcFrame_t cc){
	free(cc);
	return(NULL);
}


//////////////////////
//        RT        //
//////////////////////
int RtOpen(const char* dev){
	int fd = 0;
	fd = open(dev,O_RDWR);
	if(-1 != fd){
		errno = devctl (fd, MR_CMD_RT_CONFIG, NULL, 0, NULL);
		if(EOK != errno)
			return(-1);
		return(fd);
	}
	return(-1);
}

int	RtClose(int fd){
	return(close(fd));
}

int	RtSetAddress(int fd, unsigned short addr){
	return(errno = devctl (fd, MR_CMD_RT_SET_ADDRESS, &addr, sizeof(unsigned short), NULL));
}

int RtAllowCmd(int fd, unsigned short cmd){
	return(errno = devctl (fd, MR_CMD_RT_ALLOW_CMD, &cmd, sizeof(unsigned short), NULL));
}

int RtSetTimeOut(int fd, unsigned long delay){
	return(errno = devctl (fd, MR_CMD_RT_ADD_DELAY, &delay, sizeof(delay), NULL));
}

int RtWriteCmdData(int fd, unsigned short cmd, unsigned short data){
	unsigned short _data[2] = {cmd, data};
	return(errno = devctl (fd, MR_CMD_RT_CMD_DATA, _data, 2*sizeof(unsigned short), NULL));
}

int RtAllowSubAddress(int fd, unsigned short subaddr, unsigned short read_size, unsigned short write_size){
	unsigned short _data[3] = {subaddr, read_size, write_size};
	return(errno = devctl (fd, MR_CMD_RT_ALLOW_SA, _data, 3*sizeof(unsigned short), NULL));
}

int RtAllowSubAddressNoShift(int fd, unsigned short subaddr){
	unsigned short _data[1] = {subaddr};
	return(errno = devctl (fd, MR_CMD_RT_ALLOW_SA_NOSHIFT, _data, sizeof(unsigned short), NULL));
}

int	RtSetSubAddress(int fd, unsigned short subaddr){
	return(errno = devctl (fd, MR_CMD_RT_SET_SUBADDRESS, &subaddr, sizeof(unsigned short), NULL));
}

int RtEnable(int fd){
	return(errno = devctl (fd, MR_CMD_RT_ENABLE, NULL, 0, NULL));
}

int RtWrite(int fd, void* data, unsigned short size){
	return(errno = devctl (fd, MR_CMD_RT_WRITE_TO_SA, data, size, NULL));
}

int RtRead(int fd, void* data, unsigned short size){
	return(errno = devctl (fd, MR_CMD_RT_READ_FROM_SA, data, size, NULL));
}

int RtWaitEvent(int fd, RtEvent_t* event){
	return(errno = devctl (fd, MR_CMD_RT_WAIT_EVENT, event, sizeof(RtEvent_t), NULL));
}
