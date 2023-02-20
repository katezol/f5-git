
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#include <devctl.h>
#include <sys/dcmd_chr.h>

#include "LMR231.h"

#define MR231_HEAD_SIZE		7
#define MR231_TGT_SIZE		67
#define MR231_SHIP_SIZE		35
#define MR231_STATE_SIZE	61
#define MR231_BUF_SIZE		256

#define MR231_START_HEAD	'$'
#define MR231_HEAD_TGT		"TTM,"
#define MR231_HEAD_SHIP		"VHW,"
#define MR231_HEAD_STATE	"RSD,"


int mr231ParseTgt(int fd, mr231Tgt_t* ip, int type, unsigned short cs);			
int mr231ParseShip(int fd, mr231Ship_t* ip, unsigned short cs);
int mr231ParseState(int fd, mr231State_t* ip, unsigned short cs);
//int getBuf(int fd, char* buf, int* cs);
char* msgshift(char* msg);


int mr231Open(char* devname){
	speed_t speed = 19200;
	struct termios termios_p;
	int fd = open( devname, O_RDWR | O_NOCTTY );
    if(-1 == fd ){
    	printf("Error open [%s] %s\n", devname, strerror (errno));
    	return( -1 );
    }	
	if(-1 == tcflush( fd,TCIOFLUSH ))
		return(-1);
    if( -1 == tcgetattr( fd, &termios_p ) )
      return( -1 );
    
    // set speed
    cfsetispeed(&termios_p, speed);
    cfsetospeed(&termios_p, speed);
	
    // control mode flags
    termios_p.c_cflag |= (CLOCAL | CREAD); // MustBe
	// set 8N1
    termios_p.c_cflag &= ~PARENB;
	termios_p.c_cflag &= ~CSTOPB;
	termios_p.c_cflag &= ~CSIZE;
	termios_p.c_cflag |= CS8;
	//local mode flags
    // set raw 
    termios_p.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    // input mode flags
    // turn off software control
    termios_p.c_iflag &= ~(IXON | IXOFF | IXANY);
    // output mode flags
    // set raw output
    termios_p.c_oflag &= ~OPOST;	    
    // control characters
    termios_p.c_cc[VMIN] = MR231_DATA_BLOK_READ_SIZE;
    termios_p.c_cc[VTIME] = 0;
   	if(-1 == tcsetattr( fd, TCSANOW, &termios_p ) )
   		return(-1);
    return( fd );	
}
//return one of define and full ip if no err

int mr231ReadIp(int fd, mr231IP_t* ip, int type){
	unsigned short head_size = (MR231_HEAD_SIZE - 1 ) ;
	//int read_size;
	char head[head_size];
	char start = 0;
	unsigned short cs = 0;
	int i = 0;
	int ret = MR231_IPTYPE_ERR;
	//char sub = 0;
	while(MR231_START_HEAD != start){
		if( 1 != read(fd,&start,1)){
			printf("Error read() %s\n", strerror (errno));
			return(MR231_IPTYPE_ERR);
		}
		cs = start;
	}	
	
	for(i=0;i<head_size;i++){
		if( -1 == read(fd, &head[i], 1) ){
			printf("Error read() %s\n", strerror (errno));
			return(MR231_IPTYPE_ERR);
		}
	//	printf("read head[%d]=%s",i,head[i]);
		cs ^= head[i];
	}	
	//printf("\n");
	head_size -=2;
	if(!strncmp(&head[2],MR231_HEAD_TGT, head_size)){
		//printf("begin read Tgt\n");
		ret = mr231ParseTgt(fd, &(ip->tgt), type, cs);			
	}

	if(!strncmp(&head[2],MR231_HEAD_SHIP, head_size)){
		//printf("begin read ship\n");
		ret = mr231ParseShip(fd, &(ip->ship), cs);
	}

	if(!strncmp(&head[2],MR231_HEAD_STATE, head_size)){
		//printf("begin read state\n");
		ret = mr231ParseState(fd,&(ip->state), cs);
	}
	return(ret);
	
}

int mr231ParseTgt(int fd, mr231Tgt_t* tgt, int type, unsigned short cs){
	char buf[MR231_BUF_SIZE];
	char* msg = buf;
	int i = 0;
	int tr = 0;
	for(i=0;i<MR231_BUF_SIZE;i++){
		if( -1 == read(fd, &msg[i], 1) ){
			printf("Error read()  %s\n", strerror (errno));
			return(MR231_IPTYPE_ERR);
		}	
		if((0xA == msg[i])||(0xD == msg[i])||(0x0 == msg[i])||(i==(MR231_BUF_SIZE-1))){ //<LF> or <CR> or <NULL>
			msg[i] = 0;
			break;
		}
		if(!tr)	
			cs ^= msg[i];	
		if('*' == msg[i])
			tr = 1;
	}
	memset(tgt,0,sizeof(mr231Tgt_t));
	//printf("mr231ParseTgt:\n");
	//printf(msg);
	//printf("\n");
	
	if (','!= *(msg) ){
		tgt->num = atoi(msg);
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		tgt->dist = atof(msg);
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		tgt->pel = atof(msg);
		msg = msgshift(msg);
	}

	if (','!= *(++msg) ){
		tgt->pel_ok = *msg;
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		tgt->speed = atof(msg);
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		tgt->cur = atof(msg);
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		tgt->cur_ok = *msg;
		msg = msgshift(msg);
	}

	if (','!= *(++msg) ){
		tgt->dist_to = atof(msg);
		msg = msgshift(msg);
	}

	if (','!= *(++msg) ){
		tgt->time_to = atof(msg);
		msg = msgshift(msg);
	}

	if (','!= *(++msg) ){
		tgt->dist_type = *msg;
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		tgt->tgt_type = *msg;
		msg = msgshift(msg);
	}

	if (','!= *(++msg) ){
		tgt->tgt_status = *msg;
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		tgt->pad = *msg;
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		tgt->north_time = atol(msg);
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		tgt->tgt_auto = *msg;
		msg = msgshift(msg);
	}
	tgt->cs = atoh(++msg);
	//////////////////////
	return(MR231_IPTYPE_TGT);
}			



int mr231ParseShip(int fd, mr231Ship_t* ship, unsigned short cs){
	char buf[MR231_BUF_SIZE];
	char* msg = buf;
	int i = 0;
	int tr = 0;
	for(i=0;i<MR231_BUF_SIZE;i++){
		if( -1 == read(fd, &msg[i], 1) ){
			printf("Error read()  %s\n", strerror (errno));
			return(MR231_IPTYPE_ERR);
		}	
		if((0xA == msg[i])||(0xD == msg[i])||(0x0 == msg[i])||(i==(MR231_BUF_SIZE-1))){ //<LF> or <CR> or <NULL>
			msg[i] = 0;
			break;
		}
		if(!tr)	
			cs ^= msg[i];	
		if('*' == msg[i])
			tr = 1;
	}
	memset(ship,0,sizeof(mr231Ship_t));
	//printf("mr231ParseShip:\n");
	//printf(msg);
	//printf("\n");
	if (','!= *(msg) ){
		ship->cur = atof(msg);
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		ship->cur_ok = *msg;
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		ship->cur_mag = atof(msg);
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		ship->cur_mag_ok = *msg;
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		ship->speed = atof(msg);
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		ship->speed_type = *msg;
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		ship->speed_km = atof(msg);
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		ship->speed_km_type = *msg;
		msg = msgshift(msg);
	}
	ship->cs = atoh(++msg);
	//printf("SHIP  cs[%d] from[%d]\n",cs,ship->cs);
	return(MR231_IPTYPE_SHIP);
}

int mr231ParseState(int fd, mr231State_t* state, unsigned short cs){
	char buf[MR231_BUF_SIZE];
	char* msg = buf;
	int i = 0;
	int tr = 0;
	for(i=0;i<MR231_BUF_SIZE;i++){
		if( -1 == read(fd, &msg[i], 1) ){
			printf("Error read()  %s\n", strerror (errno));
			return(MR231_IPTYPE_ERR);
		}	
		if((0xA == msg[i])||(0xD == msg[i])||(0x0 == msg[i])||(i==(MR231_BUF_SIZE-1))){ //<LF> or <CR> or <NULL>
			msg[i] = 0;
			break;
		}
		if(!tr)	
			cs ^= msg[i];	
		if('*' == msg[i])
			tr = 1;
	}	
	memset(state,0,sizeof(mr231State_t));
	//printf("mr231ParseState:\n");
	//printf(msg);
	//printf("\n");
	if (','!= *(msg) ){
		state->start_dist_1 = atof(msg);
		msg = msgshift(msg);
	}	
	if (','!= *(++msg) ){
		state->start_pel = atof(msg);
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		state->cyc_dist_1 = atof(msg);
		msg = msgshift(msg);
	}	
	if (','!= *(++msg) ){
		state->pel_1 = atof(msg);
		msg = msgshift(msg);
	}
	if (','!= *(++msg) ){
		state->start_dist_2 = atof(msg);
		msg = msgshift(msg);
	}	
	if (','!= *(++msg) ){
		state->start_pel_2 = atof(msg);
		msg = msgshift(msg);
	}	
	if (','!= *(++msg) ){
		state->dist = atof(msg);
		msg = msgshift(msg);
	}	
	if (','!= *(++msg) ){
		state->grad = atof(msg);
		msg = msgshift(msg);
	}	
	if (','!= *(++msg) ){
		state->dist_m = atof(msg);
		msg = msgshift(msg);
	}	
	if (','!= *(++msg) ){
		state->pel_m = atof(msg);
		msg = msgshift(msg);
	}	
	if (','!= *(++msg) ){
		state->dist_step = atof(msg);
		msg = msgshift(msg);
	}	
	if (','!= *(++msg) ){
		state->dist_type = *(msg);
		msg = msgshift(msg);	
	}	
	if (','!= *(++msg) ){
		state->disp_pos = *(msg);
		msg = msgshift(msg);
	}	
	if (','!= *(++msg) ){
		state->mode = *(msg);
		msg = msgshift(msg);
	}	
	state->cs = atoh(++msg);
	//printf("STATE cs[%d] from[%d]\n",cs,state->cs);
	return(MR231_IPTYPE_STATE);
}
char* msgshift(char* msg){
	while(msg++){
		if( (',' == *msg)||('*' == *msg))
			break;
	}
	return(msg);
}	
//////////////
/*
int getBuf(int fd, char* buf, int* cs){
	int i = 0;
	char  c;
	int bsize = 32;
	for(i=0;i<bsize;i++){
		buf[i] = '\0';
		if( -1 == read(fd, &c, 1)) )
			return(0);
		(*cs) ^= c;
		if((c != ',')||(c != '*'))
			break;
		buf[i] = c;
	}
	if(i>= bsize)
		return(0);
	return(1);
}
*/		
