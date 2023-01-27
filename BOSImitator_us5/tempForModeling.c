#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <mathutils.h>
#include <modeling.h>
#include <tempForModeling.h>
#include <uit.h>
#include <messages.h>
//#include <winComplex.h>

#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <abonents.h>
#include <bosFunc.h>
#include <valueCoders.h>
#include <fcntl.h>
#define SPEED_ANTENNA 30

float peleng, pelengOld, azimut;
extern int fd_bid;
extern int fd_bid_log;
extern char  currentDir[100];

void * modeling_thread(void *arg)
{
	struct timespec curr;
	int size;
	static char s[200];
	double tempTime; 
	
	
	
	peleng = 0;
	pelengOld = 0;
	azimut = 0;
	while (true) 
	{
		delay(50);
	    //continue;
	  	if( clock_gettime( CLOCK_REALTIME, &curr) == -1 ) return NULL;
	  	currTime = curr.tv_sec*1.0+curr.tv_nsec*1.0/1000000000 - timeBegin;  /**/
	  	if (bBidLog)
	  	{	
	  		if (fd_bid_log >0)
	  		{
	  			sprintf(s, "beginWhile = %f6.3\n", (float)currTime);    		
	  			//write(fd_bid_log,s,strlen(s));
	  		}
	  	}	

		if (currTime >= timeModeling) 
		{

			fill_new_uit(); // кончено время
//		    for (i = 0; i < NRLSCHANNELS; ++i){	sendIIp(idRlsTarget,i,0);}			
		} else
		{
		 	fill_control_uit(oldCurrTime,currTime);
		}    
		
		movingCoordinats(NRLSCHANNELS, 		// размер массива
            			pmobilityDatas,		// массив контекстов
            			pcontrols,			// массив управляющих данных             
            			pobjectDatas,		// массив координат
            			(float) (currTime - oldCurrTime)); 	
		if (bBidLog)
		{	
			if( clock_gettime( CLOCK_REALTIME, &curr) == -1 ) return NULL;
			tempTime = curr.tv_sec*1.0+curr.tv_nsec*1.0/1000000000 - timeBegin;  /**/
		    	if (fd_bid_log >0)
		    	{
		    		sprintf(s, "afterMoving curr = %f delta = %f\n",
		    				(float)(tempTime),
		    				(float)(tempTime-currTime));    		
		    		//write(fd_bid_log,s,strlen(s));
		    	}	    		 
		}
				
            								// прошедшее время
/*            								          printf("hwc> FLAGS (%08x)\n", bus->flags);
		/*
		 *  printf("hwc> FLAGS (%08x)\n", bus->flags);
		 *  порисовать
		*/
		if ((indOurShip >= 0) & (indOurShip < NRLSCHANNELS))
		{
			ourShip.iname = pobjectDatas[indOurShip]->iname;
			ourShip.x = pobjectDatas[indOurShip]->x;
			ourShip.y = pobjectDatas[indOurShip]->y;
			ourShip.z = pobjectDatas[indOurShip]->z;
			ourShip.v = pobjectDatas[indOurShip]->v;
			ourShip.course = pobjectDatas[indOurShip]->course;
			ourShip.pitch = pobjectDatas[indOurShip]->pitch;
  			ourShipGeo.fi = centerfi+pobjectDatas[indOurShip]->x/60/1852.4;			
  			ourShipGeo.lambda = pobjectDatas[indOurShip]->y/cosf(centerfi/180*M_PI)/60/1852.4+centerlambda;
  			ourShipGeo.course = ourShip.course; 
		}	
		azimut = do2pi(azimut+(currTime - oldCurrTime) / 60 * SPEED_ANTENNA * 2*M_PI);	
		peleng = do2pi(azimut + ourShipGeo.course);
		
		getChannelsBosImitator(
            NRLSCHANNELS, 	
            pobjectDatas,
            &ourShip,            	
            NRLSCHANNELS,
            bid.rlsTargets,	            
            (float)currTime,
            (float)6,//timeOneTurn*CYCLYLOSSTARGET,
            (float)pelengOld,
            (float)peleng,
            NVISUALMARKS,
            visualMarks);
		
		if (bBidLog)
		{	
			if( clock_gettime( CLOCK_REALTIME, &curr) == -1 ) return NULL;
			tempTime = curr.tv_sec*1.0+curr.tv_nsec*1.0/1000000000 - timeBegin;  /**/
		    	if (fd_bid_log >0)
		    	{
		    		sprintf(s, "afterGetChannel curr = %f delta = %f\n", 
		    				(float)(tempTime),
		    				(float)(tempTime-currTime));    		
		    		//write(fd_bid_log,s,strlen(s));
		    	}	    		 
		}	

       
       
       bid.realPeleng.nType = REAL_PELENG;
       bid.realPeleng.viewNumber = 1;
       bid.realPeleng.currentTime = currTime*1000;
       bid.realPeleng.rotation = codeValue(azimut, M_PI/(1<<15), 0, 16);
       bid.realPeleng.peleng = codeValue(peleng, M_PI/(1<<15), 0, 16);
       bid.ourShipGeo = ourShipGeo;
       bid.ourShipCoordinat = ourShip;
       
       if (memcmp(&bid, &oldBid, sizeof(bid))!=0)   // информация не изменилась 
	    { 	
	    	//printf("write bid currTime=%ld\n",bid.realPeleng.currentTime);
	    	if (fd_bid >0)
	    	{	
	    		size =  write(fd_bid,&bid, sizeof(bid));
	    		if (size != sizeof(bid))
	    		{
	    			close(fd_bid);
	    			fd_bid = -1;
	    			if (fd_bid_log >0)
	    			{
	    				sprintf(s, "error Write BosImitator_us fd closed \n");    		
	    				write(fd_bid_log,s,strlen(s));
	    			}	    			
	    		} else
	    		if (fd_bid_log >0)
	    			    	{
	    			    		sprintf(s, "SUCCESS WRITE\n");    		
	    			    		//write(fd_bid_log,s,strlen(s));
	    			    	}	   	    		
	    	}	 
	    	
	    	if (fd_bid_log >0)
	    	{
	    		sprintf(s, "currTime = %f diffTime = %f\n", (float)currTime,(float) (currTime - oldCurrTime));    		
	    		//write(fd_bid_log,s,strlen(s));
	    	}	    		 
	    	memcpy(&oldBid, &bid, sizeof(bid));
    	}
       oldCurrTime = currTime;
       pelengOld = peleng;
       			
     /*  if (fd_realPeleng_stoika_main > 0)
    	    size = write(fd_realPeleng_stoika_main,&real_peleng,sizeof(real_peleng));
       */       
//        for (i = 0; i < NRLSCHANNELS; ++i)
//         {	if (memcmp(&rlsTargetsPVO[i], &oldrlsTargetsPVO[i],
//         	    sizeof(rlsTargetsPVO[i]))!=0)
//         	    { /*  sendIIp(idRlsTarget,i,0);*/ }
//         }

	} // while (TRUE)
}// modeling_thread

void init_model(int Firstnit)
{ 
	int i;
	char s[100],ss[100];
	for (i = 0; i < NRLSCHANNELS; ++i)
	{  
		memset(&ourShip,0,sizeof(ourShip)); // здесь только выделение памяти
		memset(&ourShipGeo,0,sizeof(ourShipGeo)); // здесь только выделение памяти		
		centerFi = 0;
		centerLambda =0;
		if (Firstnit)
		{
			pobjectDatas[i] = &objectDatas[i];
			pmobilityDatas[i] = &mobilityDatas[i];
			pcontrols[i] = &controls[i];
		}	
	}
	timeModeling = 200;
    timeOneTurn = 2.5; // время одного оборота
    indOurShip = -1;
    
	fill_new_uit(); // очистка каналов и запоминание времени 
	//for (i = 0; i < NRLSCHANNELS; ++i){	sendIIp(idRlsTarget,i,0);}	
	
	nVarPvoWork = nVariant;  
	strcpy(s,currentDir);
	sprintf(ss,"/training/model_%d.var",nVarPvoWork);
	strcat(s,ss);
	printf(s);
	
	if ((bBidLog) & (fd_bid_log>0))
	      	    	{	
	      	    		write(fd_bid_log,s,strlen(s));
	      	    	}	
	
    nCommandUitCurr = readCommandUit(s);
	if (Firstnit)
	{
		if (pthread_create(NULL, NULL,modeling_thread, NULL))
	       {};
	}	
}	





 


