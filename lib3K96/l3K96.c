/*
 * l3K96.c
 *
 *  Created on: 08.04.2015
 *      Author: 1
 */

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "l3K96.h"

IpMap3K96_t map;

int m3K96SetIp1(int fd, m3K96_Ip1_t *ip){
//	return(errno = devctl (fd, MSFERA_SET_IP1, ip, sizeof(mSfera_Ip1_t), NULL));
	int size =  0;
	lseek( fd, size, SEEK_SET );
	size = sizeof(*ip);
	if( size != write(fd,ip, size) )
	{
		printf("err write ip1 [%s]\n ",strerror( errno ));
	}

	printf ("SET_IP1: %d\n",sizeof(*ip)  );

	lseek( fd, 0, SEEK_SET );
	return(0);

}

int m3K96GetIp1(int fd, m3K96_Ip1_t *ip){
//	return(errno = devctl (fd, MSFERA_GET_IP1, ip, sizeof(mSfera_Ip1_t), NULL));
	int size =  0;
	lseek( fd, size, SEEK_SET );
	size = sizeof(*ip);
	if( size != read(fd,ip, size) )
	{
		printf("err read ip1 [%s]\n ",strerror( errno ));
	}

	printf ("GET_IP1: %d\n",sizeof(*ip)  );

	lseek( fd, 0, SEEK_SET );
	return(0);
}

int m3K96SetIp2(int fd, m3K96_Ip2_t *ip){
//	return(errno = devctl (fd, MSFERA_SET_IP2, ip, sizeof(mSfera_Ip2_t), NULL));
	int size =  sizeof(map.ip1);
	lseek( fd, size, SEEK_SET );
	size = sizeof(*ip);
	if( size != write(fd,ip, size) )
	{
		printf("err write ip2 [%s]\n ",strerror( errno ));
	}

	printf ("SET_IP2: %d\n",sizeof(*ip)  );

	lseek( fd, 0, SEEK_SET );
	return(0);
}

int m3K96GetIp2(int fd, m3K96_Ip2_t *ip){
//	return(errno = devctl (fd, MSFERA_GET_IP2, ip, sizeof(mSfera_Ip2_t), NULL));
	int size =  sizeof(map.ip1);
	lseek( fd, size, SEEK_SET );
	size = sizeof(*ip);
	if( size != read(fd,ip, size) )
	{
		printf("err read ip2 [%s]\n ",strerror( errno ));
	}

	printf ("GET_IP2: %d\n",sizeof(*ip)  );

	lseek( fd, 0, SEEK_SET );
	return(0);
}

int m3K96SetIp3(int fd, m3K96_Ip3_t *ip){
//	return(errno = devctl (fd, MSFERA_SET_IP3, ip, sizeof(mSfera_Ip3_t), NULL));
	int size =  sizeof(map.ip1)+ sizeof(map.ip2tgt);
	lseek( fd, size, SEEK_SET );
	size = sizeof(m3K96_Ip3_t)*M3K96_IP3_TGT_MAX_NUM;
	if( size != write(fd,ip, size) )
	{
		printf("err write ip3 [%s]\n ",strerror( errno ));
	}

	printf ("SET_IP3: %d\n",sizeof(*ip)  );

	lseek( fd, 0, SEEK_SET );
	return(0);
}

int m3K96GetIp3(int fd, m3K96_Ip3_t *ip){
//	return(errno = devctl (fd, MSFERA_GET_IP3, ip, sizeof(mSfera_Ip3_t), NULL));
	int size =  sizeof(map.ip1)+ sizeof(map.ip2tgt);
	lseek( fd, size, SEEK_SET );
	size = sizeof(m3K96_Ip3_t)*M3K96_IP3_TGT_MAX_NUM;
	if( size != read(fd,ip, size) )
	{
		printf("err read ip3 [%s]\n ",strerror( errno ));
	}

	printf ("GET_IP3: %d\n",sizeof(*ip)  );

	lseek( fd, 0, SEEK_SET );
	return(0);
}

int m3K96SetIp10(int fd, m3K96_Ip10_t *ip){
//	return(errno = devctl (fd, MSFERA_SET_IP5, ip, sizeof(mSfera_Ip5_t), NULL));

	int size =  sizeof(map.ip1)+ sizeof(map.ip2tgt)+ sizeof(map.cu);
	lseek( fd, size, SEEK_SET );
	size = sizeof(m3K96_Ip10_t);
	if( size != write(fd,ip, size) )
	{
		printf("err write ip10 [%s]\n ",strerror( errno ));
	}

	printf ("SET_IP10: %d\n",sizeof(*ip)  );

	lseek( fd, 0, SEEK_SET );
	return(0);
}

int m3K96GetIp10(int fd, m3K96_Ip10_t *ip){
//	return(errno = devctl (fd, MSFERA_GET_IP5, ip, sizeof(mSfera_Ip5_t), NULL));

	int size =  sizeof(map.ip1)+ sizeof(map.ip2tgt)+ sizeof(map.cu);
	lseek( fd, size, SEEK_SET );
	size = sizeof(m3K96_Ip10_t);
	if( size != read(fd,ip, size) )
	{
		printf("err read ip10 [%s]\n ",strerror( errno ));
	}

	printf ("GET_IP10: %d\n",sizeof(*ip)  );

	lseek( fd, 0, SEEK_SET );
	return(0);
}

int m3K96SetIp11(int fd, m3K96_Ip11_t *ip){
//	return(errno = devctl (fd, MSFERA_SET_IP6, ip, sizeof(mSfera_Ip6_t), NULL));

	int size =  sizeof(map.ip1)+ sizeof(map.ip2tgt)+ sizeof(map.cu)+ sizeof(map.ip10);
	lseek( fd, size, SEEK_SET );
	size = sizeof(m3K96_Ip11Tgt_t)*M3K96_IP11_MAX_NUM;
	if( size != write(fd,ip, size) )
	{
		printf("err write ip11 [%s]\n ",strerror( errno ));
	}

	printf ("SET_IP11: %d\n",sizeof(*ip)  );

	lseek( fd, 0, SEEK_SET );
	return(0);
}

int m3K96GetIp11(int fd, m3K96_Ip11_t *ip){
//	return(errno = devctl (fd, MSFERA_GET_IP6, ip, sizeof(mSfera_Ip6_t), NULL));

	int size =  sizeof(map.ip1)+ sizeof(map.ip2tgt)+ sizeof(map.cu)+ sizeof(map.ip10);
	lseek( fd, size, SEEK_SET );
	size = sizeof(m3K96_Ip11Tgt_t)*M3K96_IP11_MAX_NUM;
	if( size != read(fd,ip, size) )
	{
		printf("err read ip11 [%s]\n ",strerror( errno ));
	}

	printf ("GET_IP11: %d\n",sizeof(*ip)  );

	lseek( fd, 0, SEEK_SET );
	return(0);
}

int m3K96GetState(int fd, m3K96_State_t *ip){
//	return(errno = devctl (fd, MSFERA_GET_STATE, ip, sizeof(mSfera_State_t), NULL));

	int size =  sizeof(map.ip1)+ sizeof(map.ip2tgt)+ sizeof(map.cu)+ sizeof(map.ip10)+ sizeof(map.ip11tgt);
	lseek( fd, size, SEEK_SET );
	size = sizeof(*ip);
	if( size != read(fd,ip, size) )
	{
		printf("err read state [%s]\n ",strerror( errno ));
	}

	lseek( fd, 0, SEEK_SET );
	return(0);

}
