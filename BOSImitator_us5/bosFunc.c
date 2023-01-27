#include <bosFunc.h>
/* Ретранслятор целей-видит между двумя пеленгами все!
 */
#include <stdio.h>
#include <mathUtils.h>
#include <math.h>
extern float peleng, pelengOld, azimut;

/* Ретранслятор целей-видит между двумя пеленгами все!
 */
int getVisualMarksBosImitator(
            int sizeCoordinats,
            ObjectData  * pobjectDatas[],		// массив координат
            Coordinat  * pourShip,
            float pelengAntennaOld,
            float pelengAntenna,
            int nVisualMarks,
            VisualMark visualMarks[]
            )
{
	int i, size;
	float distance, peleng, distanceHor, um;
	
	for (i = 0; i < nVisualMarks; ++i) 
	{
		memset(&visualMarks[i],0,sizeof(visualMarks[i]));
		visualMarks[i].iname = -1;	
	}
	
	size =0;
    for (i = 0; i < sizeCoordinats; ++i) 
	{
		if (pobjectDatas[i]->iname < 0) continue;
		if (pobjectDatas[i]->iname == pourShip->iname) continue;
		distance = sqrtf ( powf(pobjectDatas[i]->y-pourShip->y,2) + powf(pobjectDatas[i]->x-pourShip->x,2) +
                           powf(pobjectDatas[i]->z-pourShip->z,2) );
		peleng   = getPeleng(pobjectDatas[i]->y - pourShip->y, pobjectDatas[i]->x - pourShip->x);
		if (!isAngleInSector(peleng, pelengAntennaOld,pelengAntenna)) continue;
	    distanceHor= hypotf (pobjectDatas[i]->y - pourShip->y, pobjectDatas[i]->x - pourShip->x);		
		um         = getPeleng(pobjectDatas[i]->z - pourShip->z, distanceHor);
		
		visualMarks[size].distance = distance;
		visualMarks[size].distanceHor = distanceHor;		
		visualMarks[size].peleng = peleng;
		visualMarks[size].um = um;		
		visualMarks[size].objType = pobjectDatas[i]->objType;
		visualMarks[size].own = pobjectDatas[i]->own;		
		visualMarks[size].z = pobjectDatas[i]->z;
		visualMarks[size].iname = pobjectDatas[i]->iname;//size;
		size ++;
		if (size>=nVisualMarks) return size;								
	}	
	return size;
}	//getVisualMarksBosImitator


void getChannelsBosImitator(
            int sizeCoordinats, 	
            ObjectData  * pobjectDatas[],
            Coordinat  * pourShip,            	
            int sizeTargets,
            RlsTarget  prlsTargets[],            
            float currTime,
            float timeLoss,
            float pelengOld,
            float peleng,
            int nVisualMarks,
            VisualMark visualMarks[]) 	
{
	int size =0;
	int i, nMark, nRlsTarget;
	
	size =  getVisualMarksBosImitator(
            sizeCoordinats,
            pobjectDatas,		// массив координат
            pourShip,
            pelengOld,
            peleng,
            nVisualMarks,
            visualMarks
            );
  
    for (i = 0; i < size; ++i) 
    {  // ищем ранее сопровождавшиеся цели
		nRlsTarget = getRlsTargetByName(visualMarks[i].iname,
		                  sizeTargets,prlsTargets); // специально для тренировочных режимов
		if (nRlsTarget == -1) continue;
		prlsTargets[nRlsTarget].objType = visualMarks[i].objType;
		prlsTargets[nRlsTarget].x = 	 visualMarks[i].distance * 
				                                   cosf (visualMarks[i].peleng) * 
				                                   cosf (visualMarks[i].um);
		prlsTargets[nRlsTarget].y =      visualMarks[i].distance * 
				                                   sinf (visualMarks[i].peleng) * 
				                                   cosf (visualMarks[i].um);
		prlsTargets[nRlsTarget].z = visualMarks[i].z;
		prlsTargets[nRlsTarget].distance = visualMarks[i].distance;
		prlsTargets[nRlsTarget].distanceHor = visualMarks[i].distanceHor;
		prlsTargets[nRlsTarget].peleng      = visualMarks[i].peleng;
    	prlsTargets[nRlsTarget].um          = visualMarks[i].um;
		prlsTargets[nRlsTarget].tour = currTime; // без маркеров с хранением производные взять неоткуда
		
		if (pobjectDatas[visualMarks[i].iname]->iname == visualMarks[i].iname)
		{
			prlsTargets[nRlsTarget].vx          = pobjectDatas[visualMarks[i].iname]->v*
                                                  cos(pobjectDatas[visualMarks[i].iname]->course)*
                                                  cos(pobjectDatas[visualMarks[i].iname]->pitch);                                                  			
			prlsTargets[nRlsTarget].vy          = pobjectDatas[visualMarks[i].iname]->v*
                                                  sin(pobjectDatas[visualMarks[i].iname]->course)*
                                                  cos(pobjectDatas[visualMarks[i].iname]->pitch);                                                  
			prlsTargets[nRlsTarget].vz          = pobjectDatas[visualMarks[i].iname]->iname*
                                                  sin(pobjectDatas[visualMarks[i].iname]->pitch);			
			prlsTargets[nRlsTarget].vr          = pobjectDatas[visualMarks[i].iname]->v;						
			prlsTargets[nRlsTarget].course      = pobjectDatas[visualMarks[i].iname]->course;
		}	
		visualMarks[i].iname = -1;		
		
		//if (poldRlsTargets[nRlsTarget].nSource > 0) 
		  //   getPdc(&prlsTargets[nRlsTarget],&poldRlsTargets[nRlsTarget]);
	}
	
	for (i = 0; i < sizeTargets; ++i) 
	{ 
		nRlsTarget = getFreeRlsTargets(sizeTargets,prlsTargets);
		if (nRlsTarget == -1) break; // все каналы уже заняты информацией
		nMark = getVisualMarksByMinDistance(size,visualMarks);
		if (nMark == -1) break; // нет больше нераспределенных по каналам отметок
		deleteChannel(&prlsTargets[nRlsTarget],NULL,nRlsTarget);
		prlsTargets[nRlsTarget].nSource = nRlsTarget+1;
		prlsTargets[nRlsTarget].iname = visualMarks[nMark].iname;		
		prlsTargets[nRlsTarget].objType = visualMarks[nMark].objType;
		prlsTargets[nRlsTarget].own = visualMarks[nMark].own;		
		prlsTargets[nRlsTarget].x =  visualMarks[nMark].distance * 
				                                   cosf (visualMarks[nMark].peleng) * 
				                                   cosf (visualMarks[nMark].um);
		prlsTargets[nRlsTarget].y =  visualMarks[nMark].distance * 
				                                   sinf (visualMarks[nMark].peleng) * 
				                                   cosf (visualMarks[nMark].um);
		prlsTargets[nRlsTarget].z = visualMarks[nMark].z;
		prlsTargets[nRlsTarget].distance = visualMarks[nMark].distance;
		prlsTargets[nRlsTarget].distanceHor = visualMarks[nMark].distanceHor;
		prlsTargets[nRlsTarget].peleng      = visualMarks[nMark].peleng;
    	prlsTargets[nRlsTarget].um          = visualMarks[nMark].um;
		if (pobjectDatas[visualMarks[nMark].iname]->iname == visualMarks[nMark].iname)
		{
			prlsTargets[nRlsTarget].vx          = pobjectDatas[visualMarks[nMark].iname]->v*
                                                  cos(pobjectDatas[visualMarks[nMark].iname]->course)*
                                                  cos(pobjectDatas[visualMarks[nMark].iname]->pitch);                                                  			
			prlsTargets[nRlsTarget].vy          = pobjectDatas[visualMarks[nMark].iname]->v*
                                                  sin(pobjectDatas[visualMarks[nMark].iname]->course)*
                                                  cos(pobjectDatas[visualMarks[nMark].iname]->pitch);                                                  
			prlsTargets[nRlsTarget].vz          = pobjectDatas[visualMarks[nMark].iname]->iname*
                                                  sin(pobjectDatas[visualMarks[nMark].iname]->pitch);			
			prlsTargets[nRlsTarget].vr          = pobjectDatas[visualMarks[nMark].iname]->v;						
			prlsTargets[nRlsTarget].course      = pobjectDatas[visualMarks[nMark].iname]->course;
		}	
		prlsTargets[nRlsTarget].tour = currTime; 
		visualMarks[nMark].iname = -1;												              
	}
	for (i = 0; i < sizeTargets; ++i) 
	{               
		if (prlsTargets[i].nSource <= 0) continue;
		if ((currTime - prlsTargets[i].tour) > timeLoss/3)
				 prlsTargets[i].bLoss = true;  // удаление устаревших каналов
		if ((currTime - prlsTargets[i].tour) > timeLoss)
		  deleteChannel (&prlsTargets[i], NULL,i);  // удаление устаревших каналов
	}
	
} // getChannelsBosImitator
