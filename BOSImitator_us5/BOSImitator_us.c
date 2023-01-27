#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <termios.h>

#include "uit.h"
#include "abonents.h"
#include "BOSImitator_us.h"
#include "valueCoders.h"
#include "tempForModeling.h"


char  currentDir[100];
//---------------------------------------------------------


/*
bool createFormularTrassa(int i)
{
	if (memcmp(&rlsTargetsPVO[i], &oldrlsTargetsPVO[i],	sizeof(rlsTargetsPVO[i]))==0) return 0;  // информация не изменилась 
 	memset(&formular_trassa, 0, sizeof(formular_trassa));
 	formular_trassa.nType = FORMULAR_TRASSA; 		
   	formular_trassa.char0 = rlsTargetsPVO[i].bPassive;
   	formular_trassa.char0 |= (rlsTargetsPVO[i].bHandEscort << 1);
   	formular_trassa.char0 |= (rlsTargetsPVO[i].bLoss << 2);
   	formular_trassa.char0 |= (rlsTargetsPVO[i].bReset << 3);
   	formular_trassa.char0 |= (rlsTargetsPVO[i].bSlow << 4);
    	
   	if ( rlsTargetsPVO[i].own == owEnemy ) 
   		formular_trassa.char0 |= (1 << 5);
   	else if ( rlsTargetsPVO[i].own == owOur )
   		formular_trassa.char0 |= (1 << 6);
   	
   	formular_trassa.char0 |= (rlsTargetsPVO[i].bAutoEscort << 7);
    	
   	switch  (rlsTargetsPVO[i].objType) {
    		
   		case otUndefine:	formular_trassa.short1 = 0; break;
   		case otShip: 		formular_trassa.short1 = 1; break;
   		case otSurface:		formular_trassa.short1 = 1; break;
   		case otAir:			formular_trassa.short1 = (1 << 3); break;
   		case otTrap:		formular_trassa.short1 = 1 | (1 << 3); break;
   		case otPlane:		formular_trassa.short1 = 2 | (1 << 3); break;
   		case otPkr:			formular_trassa.short1 = 3 | (1 << 3); break;
   		case otPrr:			formular_trassa.short1 = (1 << 2)|(1 << 3); break;
   		case otHelicopter:	formular_trassa.short1 = 1 | (1 << 2)|(1 << 3); break;      
   		case otSat:			formular_trassa.short1 = 3 | (1 << 2)|(1 << 3); break;
   		
   		default:break;
   	}
   	 
   	formular_trassa.short1 |= (rlsTargetsPVO[i].bvz << 4);
   	formular_trassa.short1 |= (rlsTargetsPVO[i].nSource << 8);
    	
   	if (rlsTargetsPVO[i].bPassive) 
   	{
       	formular_trassa.word2 =	codeValue(rlsTargetsPVO[i].x, M_PI / (1 << 15), 0, 16);
       	formular_trassa.word3 = codeValue(rlsTargetsPVO[i].z, M_PI / (1 << 16), 1, 16);
   		
   	    formular_trassa.word5 = codeValue(rlsTargetsPVO[i].vx, 5, 1, 10) * 1000.0f;
   	    formular_trassa.word6 = codeValue(rlsTargetsPVO[i].vz, 5, 1, 10) * 1000.0f;
   	}	else 
   	{
       	formular_trassa.word2 =	codeValue(rlsTargetsPVO[i].x, 5, 1, 16);
       	
       	formular_trassa.word3 = codeValue(rlsTargetsPVO[i].z, 1, 0, 16);
   		formular_trassa.word3 |= (codeValue(rlsTargetsPVO[i].y, 5, 1,16) << 16);
   		
   		formular_trassa.word4 = codeValue(rlsTargetsPVO[i].skoY, 1, 0, 16);
   		formular_trassa.word4 |=(codeValue(rlsTargetsPVO[i].skoX, 1, 0, 16) << 16);
   		
   		formular_trassa.word5 = codeValue(rlsTargetsPVO[i].vx, 5, 1, 10);
   		formular_trassa.word5 |= (codeValue(rlsTargetsPVO[i].skoVx, 5, 0, 6) << 10);
   		formular_trassa.word5 |= (codeValue(rlsTargetsPVO[i].skoZ, 5, 0, 16) << 16);
    		
   		formular_trassa.word6 = codeValue(rlsTargetsPVO[i].vz, 5, 1, 10);
   		formular_trassa.word6 |= (codeValue(rlsTargetsPVO[i].skoVz, 5, 0,6) << 10);
   		formular_trassa.word6 |= (codeValue(rlsTargetsPVO[i].vy, 5, 1,10) << 16);
   		formular_trassa.word6 |= (codeValue(rlsTargetsPVO[i].skoVy, 5, 0,6) << 26);
   	  }
   	memcpy( &oldrlsTargetsPVO[i], &rlsTargetsPVO[i], sizeof(oldrlsTargetsPVO[i]) );
   	return 1;
}*/
//---------------------------------------------------------
void* readSettingsBosImitator_thread(void* arg)
{
	int fd, n;
	struct timespec timeRead, curr;
   
	fd = -1;
	clock_gettime( CLOCK_REALTIME, &timeRead) ;// 

		
	while (true)
	{
		    if (fd == -1)
		    {
		    	fd = open( port_BIsettings,  O_RDONLY);
		    	printf("Open port_BIsettings =%s fd=%d\n",port_BIsettings,fd);
		    	perror("error:\n");
		    	delay(1000);
				if (fd == -1)continue;
		    }		
            delay (1000);

		    n=read(fd,&nVariant,sizeof(nVariant));
		    if (n<sizeof(nVariant))
		    {
		    	clock_gettime( CLOCK_REALTIME, &curr) ;//
		    	if ((curr.tv_sec-timeRead.tv_sec) > 20) 
		    	{ 	close(fd);
		    		clock_gettime( CLOCK_REALTIME, &timeRead) ;//
		    		fd=-1;
		    	}
		    	delay (100);
		    	continue;		  
		    }
	  printf("READING: nVariant=%d\n",nVariant);
	  clock_gettime( CLOCK_REALTIME, &timeRead) ;// 
	  
	  if ((nVariant<1) | (nVariant>88)) 
		    	nVariant = 1;
	  
	  if (nVariant != nVariantOld)
	  {
		  printf("change nVariant=%d\n",nVariant);
		  nVariantOld = nVariant;
		  init_model(0);
	  }
    } // while
	close (fd);
	return NULL;
}

int main(int argc, char** argv )
{
	//int i;
	char s[100];
	
	fd_bid = -1;
	nVariant = 1;
	nVariantOld = 1;
	sprintf(currentDir,"%s",dirname(argv[0]));
	strcpy(s,currentDir);
	strcat(s,"/abonents5");
	printf( "%s %s\n", s, currentDir) ;
	definePorts(s);	 
	if (bBidLog)
		    {
		    	//write(0,'bBidLog',20);
				strcpy(s,currentDir);
		    	strcat(s,"/log/logBid");
		    	fd_bid_log = open( s,  O_RDWR | O_CREAT);
		    	sprintf(s,"open fd_bid_log =%d\n",fd_bid_log);
		    //	write(fd_bid_log,s,strlen(s));

		    }
    init_model(1);
   
 	if (pthread_create(NULL, NULL, readSettingsBosImitator_thread, NULL))  
 							     {};// чтение настроек
 	
    while (1) 
    {
    	    if (fd_bid <= 0)
     	    {
      	    	fd_bid = open( port_bid,  O_WRONLY);
      	    	if ((bBidLog) & (fd_bid_log>0))
      	    	{	
      	    		sprintf(s,"fd_bid <= 0 new open  fd_bid =%d\n",fd_bid);
      	    		//write(fd_bid_log,s,strlen(s));
      	    	}	
     	    	delay(1000);
     	    	continue;
     	    }	
      	    delay(1000);
   }
    close(fd_bid);
    close(fd_bid_log);
}
//---------------------------------------------------------