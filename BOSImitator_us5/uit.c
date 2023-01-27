#include <uit.h>
#include <stdio.h>
#include <math.h>
#include <mathutils.h>
#include <mobility.h>
#include <modeling.h>
#include <time.h>


int 
	readCommandUit(char * fileName)
{
	char str_buf[100];
    FILE* f;
    int i,j;
    int nTargets, nManever;
    int indexCommand = -1;
    int name, param[4],ipriv, itype;
              
    f = fopen( fileName, "r" );
	memset(commandUit,0,sizeof(commandUit));    
    if( f == NULL )  return indexCommand;
    if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;
    sscanf(str_buf, "%lf\n", &timeModeling);
    if (timeModeling <1) return indexCommand;
    if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;    
    sscanf(str_buf, "%lf %lf\n", &centerfi, &centerlambda);
    if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;
    sscanf(str_buf, "%i\n", &nTargets);
    if (nTargets <1) return indexCommand;
    if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;
    sscanf(str_buf, "%i\n", &param[0]);
    indOurShip = param[0];
    
    for (i = 0; i < nTargets; ++i) 
    {
        if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;    	
    	++indexCommand; 
    	if (indexCommand >= NCOMMANDUIT) return indexCommand;
    	memset(&commandUit[indexCommand],0,sizeof(CommandUit));    
    	if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;
    	sscanf(str_buf, "%i\n", &name); // имя объекта
    	commandUit[indexCommand].ind = name;                
    	if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;
    	sscanf(str_buf, "%i\n", &param[0]); 
    	if ((param[0]<0)|(param[0]>13))  return indexCommand;
    	commandUit[indexCommand].bNewz  = (short) param[0]; // !!
    	if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;    	
    	sscanf(str_buf, "%i %f\n", &param[0],&commandUit[indexCommand].y); //ГО и мощность помехопостановщика 
    	if ((param[0]<0)|(param[0]>4))  return indexCommand;
    	commandUit[indexCommand].bNewv = (short) param[0];
    	if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;    	
    	sscanf(str_buf, "%lf %i\n", &commandUit[indexCommand].time, &ipriv);
	    commandUit[indexCommand].type = binit;    	
        commandUit[indexCommand].indPriv = -1;   	
    	if (ipriv == -1)
    	{
    		if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;
    		sscanf(str_buf, "%f %f %f\n", &commandUit[indexCommand].x,
	    	       &commandUit[indexCommand].y,&commandUit[indexCommand].z);
    		if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;
    		sscanf(str_buf, "%f %f %f\n", &commandUit[indexCommand].v,
    	    	   &commandUit[indexCommand].course,&commandUit[indexCommand].pitch);
    	    commandUit[indexCommand].course = commandUit[indexCommand].course*M_PI/180;
    	    commandUit[indexCommand].pitch = commandUit[indexCommand].pitch*M_PI/180;    	    	   
        
    	} else  
    	{  	
    		commandUit[indexCommand].indPriv = ipriv;
    	}	
    	if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;    	
    	sscanf(str_buf, "%i \n", &nManever);
    	for (j = 0; j < nManever; ++j) 
    	{
	       	++indexCommand;
    		if (indexCommand >= NCOMMANDUIT) return indexCommand;	       	
    	   	memset(&commandUit[indexCommand],0,sizeof(CommandUit));
        	commandUit[indexCommand].ind = name;       
            commandUit[indexCommand].indPriv = -1;        		           	
    		if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;
	    	sscanf(str_buf, "%lf %i %i %i %i %f\n", &commandUit[indexCommand].time,
	    	                                     &itype,  // 1 - bControl, 2-ChangePosition, 3-ChangePdc
	    	                                     &ipriv,  // -1 - нет привязки маневра, координаты передаются далее 	    	                                     	    	                                     
	    	                                              // >=0 - номер объекта, на который поворачивать или прицеливать 
	    	                                     &param[0], // >0 - маневр в точку встречи, только для ipriv >= 0 
	    	                                     &param[1],
	    	                                                //  1 - признак передачи новой высоты,
	    	                                                // -1 - поворот на высоту привязки
	    	                                                //  0 - высота не изменяется 
	    	                                     &commandUit[indexCommand].z);// новая высота,
            commandUit[indexCommand].indTarget = ipriv;	    	                                       
            if (itype ==1) commandUit[indexCommand].type = bcontrol;
              else if (itype ==2) commandUit[indexCommand].type = bchangePosition;
                   else if (itype ==3) commandUit[indexCommand].type = bchangePdc;   
                        else return indexCommand;
            commandUit[indexCommand].bMeetPoint = param[0]>0;                        
    		if (ipriv == -1)
    		{
    			if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;
    			sscanf(str_buf, "%i %i %i %i\n", &param[0], &param[1], &param[2], &param[3]);
                commandUit[indexCommand].bNewz = (param[0] == 1);
                commandUit[indexCommand].bNewv = (param[1] == 1);
                commandUit[indexCommand].bNewCourse = param[2];
                commandUit[indexCommand].bNewPitch = (param[3] == 1);
    			if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;
	    		sscanf(str_buf, "%f %f %f %f\n", &commandUit[indexCommand].z, &commandUit[indexCommand].v,
    			    &commandUit[indexCommand].course,&commandUit[indexCommand].pitch);
    			commandUit[indexCommand].course = commandUit[indexCommand].course*M_PI/180;
    			commandUit[indexCommand].pitch = commandUit[indexCommand].pitch*M_PI/180;    			    
    		}else
       		{	       			            
                commandUit[indexCommand].bNewz = param[1];       			            
    		}
    		
    	} // for j - чтение по маневрам
       	++indexCommand;
    	if (indexCommand >= NCOMMANDUIT) return indexCommand;       	
   	   	memset(&commandUit[indexCommand],0,sizeof(CommandUit));
       	commandUit[indexCommand].ind = name;       	           	
    	if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) return indexCommand;
	    sscanf(str_buf, "%lf\n", &commandUit[indexCommand].time);
	    commandUit[indexCommand].type = bkill;
    } // for i - чтение по объектам
    return indexCommand+1;	    
}	 



void fill_control_uit(double oldTime, double currTime)
{
	int i, ind, npriv;
	float course;
	float timeMeet;
	Coordinat meetPoint;
	for (i = 0; i < nCommandUitCurr; ++i) 
	{
		if ((commandUit[i].time < oldTime) | (commandUit[i].time >  currTime)) continue;
		ind = commandUit[i].ind;
		if ((ind < 0) | (ind >= NRLSCHANNELS)) continue;		
		if (commandUit[i].type == binit)
		{
			deleteTarget(ind);
			
			pobjectDatas[ind]->iname = ind;
			pobjectDatas[ind]->objType = (ObjType) commandUit[i].bNewz;
			pobjectDatas[ind]->own     = (Own) commandUit[i].bNewv;
			pobjectDatas[ind]->width   = (float) 50;
			if (commandUit[i].indPriv >=0)
			{			
				npriv = commandUit[i].indPriv;
				if ((npriv >=0) & (npriv < NRLSCHANNELS) &
				    (pobjectDatas[npriv]->iname>= 0))
				    {
						pobjectDatas[ind]->x = pobjectDatas[npriv]->x;
						pobjectDatas[ind]->y = pobjectDatas[npriv]->y;
						pobjectDatas[ind]->z = pobjectDatas[npriv]->z;						
						pobjectDatas[ind]->v = pobjectDatas[npriv]->v;
						pobjectDatas[ind]->course = pobjectDatas[npriv]->course;
						pobjectDatas[ind]->pitch  = pobjectDatas[npriv]->pitch;						
				    }	
			} else
			{	    
				pobjectDatas[ind]->x = commandUit[i].x;
				pobjectDatas[ind]->y = commandUit[i].y;
				pobjectDatas[ind]->z = commandUit[i].z;				
				pobjectDatas[ind]->v = commandUit[i].v;
				pobjectDatas[ind]->course = commandUit[i].course;
				pobjectDatas[ind]->pitch  = commandUit[i].pitch;				
			}
 			pcontrols[ind]->iname = ind;		
			setTypicalMobilityDataFromObjType( pobjectDatas[ind]->objType, pmobilityDatas[ind]);
		} // bInit 
// NB!!!  Исполнение нового маневра прерывает исполнение старого!
// перед заполнением   		
	   if (commandUit[i].type == bcontrol)
		{
			memset(pcontrols[ind],sizeof(Control),0);// предварительная очистка
			pcontrols[ind]->iname = ind;// управляющего интерфейса
			if (commandUit[i].indTarget < 0) 
			{			
				pcontrols[ind]->bNewz = (commandUit[i].bNewz == 1);
				pcontrols[ind]->bNewv = (commandUit[i].bNewv == 1);
				pcontrols[ind]->bNewCourse = commandUit[i].bNewCourse;						
				pcontrols[ind]->bNewPitch = commandUit[i].bNewPitch;
				pcontrols[ind]->z = commandUit[i].z;
				pcontrols[ind]->v = commandUit[i].v;
				pcontrols[ind]->course = commandUit[i].course;						
				pcontrols[ind]->pitch = commandUit[i].pitch;
				continue;
			}
			npriv = rint(commandUit[i].indTarget);
			if ((npriv <0) |(npriv >= NRLSCHANNELS) |(pobjectDatas[npriv]->iname< 0)) continue;
			if (commandUit[i].bNewz != 0)
			{	pcontrols[ind]->bNewz = true;
				if (commandUit[i].bNewz == -1)	pcontrols[ind]->z = pobjectDatas[npriv]->z;
			 		                           else pcontrols[ind]->z = commandUit[i].z;
			} 	
			pcontrols[ind]->bNewCourse = true;
		    if (commandUit[i].bMeetPoint)
			{	// направление - ТВ
				timeMeet = countMeetingTime_od(// расчет точки встречи в случае, когда оружие движется равномерно прямолинейно
                 	           *pobjectDatas[ind],// координаты и скорость оружия (время на разворот игнорируется)
                   	           *pobjectDatas[npriv],  // координаты и параметры движения цели
                   	           &meetPoint);
	    		course = getPeleng(meetPoint.y - pobjectDatas[ind]->y,
	                               meetPoint.x - pobjectDatas[ind]->x );						
				pcontrols[ind]->course = course;
	         } else
			 { // направление - НМЦ
			   	course = getPeleng(pobjectDatas[npriv]->y - pobjectDatas[ind]->y,
			                       pobjectDatas[npriv]->x - pobjectDatas[ind]->x );						
				pcontrols[ind]->course = course;
			 }
		} // bControlOn 
		
	   if (commandUit[i].type == bchangePdc)
		{
			memset(pcontrols[ind],sizeof(Control),0);// предварительная очистка
			pcontrols[ind]->iname = ind;// управляющего интерфейса
			if (commandUit[i].indTarget <0) 
			{
				if (commandUit[i].bNewz == 1) 	pobjectDatas[ind]->z = commandUit[i].z;				
				if (commandUit[i].bNewv == 1) 	pobjectDatas[ind]->v = commandUit[i].v;
				if (commandUit[i].bNewCourse)	pobjectDatas[ind]->course = commandUit[i].course;
				if (commandUit[i].bNewPitch) 	pobjectDatas[ind]->pitch  = commandUit[i].pitch;				
				continue;
			}
			npriv = rint(commandUit[i].indTarget);
			if ((npriv <0) |(npriv >= NRLSCHANNELS) |(pobjectDatas[npriv]->iname< 0)) continue;
			if (commandUit[i].bNewz == -1)	pobjectDatas[ind]->z = pobjectDatas[npriv]->z; // высота таргет
			if (commandUit[i].bNewz ==  1)	pobjectDatas[ind]->z = commandUit[i].z;// высота из команды
		    if (commandUit[i].bMeetPoint)
			{	// направление - ТВ
				timeMeet = countMeetingTime_od(// расчет точки встречи в случае, когда оружие движется равномерно прямолинейно
                 	           *pobjectDatas[ind],// координаты и скорость оружия (время на разворот игнорируется)
                   	           *pobjectDatas[npriv],  // координаты и параметры движения цели
                   	           &meetPoint);
	    		course = getPeleng(meetPoint.y - pobjectDatas[ind]->y,
	                               meetPoint.x - pobjectDatas[ind]->x );						
				pobjectDatas[ind]->course = course;
	         } else
			 { // направление - НМЦ
			   	course = getPeleng(pobjectDatas[npriv]->y - pobjectDatas[ind]->y,
			                       pobjectDatas[npriv]->x - pobjectDatas[ind]->x );						
				pobjectDatas[ind]->course = course;
			 }
		} // bchangePdc
		
	    if (commandUit[i].type == bchangePosition)
		{ // внимательно!!
			memset(pcontrols[ind],sizeof(Control),0);// предварительная очистка
			pcontrols[ind]->iname = ind;// управляющего интерфейса
			if (commandUit[i].indTarget <0) 
			{
				if (commandUit[i].bNewz == 1) 	pobjectDatas[ind]->x = commandUit[i].z;				
				if (commandUit[i].bNewv == 1) 	pobjectDatas[ind]->y = commandUit[i].v;
				if (commandUit[i].bNewCourse) 	pobjectDatas[ind]->z = commandUit[i].course;
			}				
		} // bchangePosition
		
		if  (commandUit[i].type == bkill)
		{
			deleteTarget(ind);
		} // bkill
	} //for i
}	// fill_control_uit


void fill_new_uit(void)
{
	int i;
	struct timespec curr;
	for (i = 0; i < NRLSCHANNELS; ++i) 
	{
		deleteTarget(i);
		deleteChannel(&bid.rlsTargets[i], &oldBid.rlsTargets[i],i);		
	}
	if( clock_gettime( CLOCK_REALTIME, &curr) == -1 ) 
	{
    	return;
	}	
	timeBegin = curr.tv_sec*1.0+curr.tv_nsec*1.0/1000000000;
	oldCurrTime = 0.0;
	currTime = 0.0;
}	


void deleteTarget(int i)
{
	if ((i < 0)  | ( i >= NRLSCHANNELS)) return;
	memset(pobjectDatas[i], 0, sizeof(ObjectData));
	pobjectDatas[i]->iname = -1;
	memset(pmobilityDatas[i], 0, sizeof(MobilityData));
	memset(pcontrols[i], 0, sizeof(Control));
	pcontrols[i]->iname = -1;
}		




