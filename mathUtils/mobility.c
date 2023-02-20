#include <math.h>
#include <stdlib.h>
#include <mathutils.h>

#include <commonTypes.h>
#include <maneverFunc.h>


 void moveStraight		(float dt,				// прошедшее время
 						Coordinat * pcoordinat
                   		)
{
	func_moveStraight(dt ,
	                  &pcoordinat->x,
	                  &pcoordinat->y, 
	                  &pcoordinat->z,
	                  pcoordinat->v,
	                  pcoordinat->course,
	                  pcoordinat->pitch	                  	                  
	                     );	
}  // moveStraight

 void moveStraight_od		(float dt,				// прошедшее время
 						ObjectData * pobjectData
                   		)
{
	func_moveStraight(dt ,
	                  &pobjectData->x,
	                  &pobjectData->y, 
	                  &pobjectData->z,
	                  pobjectData->v,
	                  pobjectData->course,
	                  pobjectData->pitch	                  	                  
	                     );	
}  // moveStraight_od



bool moveHturn	(	float dAngle, 				// угол поворота
                   		float dt, 					// прошедшее время
                   		MobilityData * pmobilityData,//
                   		Coordinat * pcoordinat		// 	 		
				)
{
	bool b;
	b = func_moveHturn (
					dAngle,
					dt,
					pmobilityData->wh,
					pmobilityData->rCirkHor,
					&pmobilityData->napravH,
					&pcoordinat->x,
					&pcoordinat->y,
					&pcoordinat->z,
					pcoordinat->v,
					&pcoordinat->course,
					pcoordinat->pitch);
	return b;																																													
}	// moveHturn


bool moveHturn_od	(	float dAngle, 				// угол поворота
                   		float dt, 					// прошедшее время
                   		MobilityData * pmobilityData,//
                   		ObjectData * pobjectData		// 	 		
				)
{
	bool b;
	b = func_moveHturn (
					dAngle,
					dt,
					pmobilityData->wh,
					pmobilityData->rCirkHor,
					&pmobilityData->napravH,
					&pobjectData->x,
					&pobjectData->y,
					&pobjectData->z,
					pobjectData->v,
					&pobjectData->course,
					pobjectData->pitch);
	return b;																																													
}	// moveHturn_od

					
  bool moveVturn	(	float dAngle, 				// угол поворота
                   		float dt, 					// прошедшее время
                   		MobilityData * pmobilityData,//
                   		Coordinat * pcoordinat		// 	 		
					)
{
	return func_moveVturn(
						dAngle,
						dt,
						pmobilityData->wv,
						pmobilityData->rCirkVer,
						&pmobilityData->napravV,
						&pcoordinat->x,
						&pcoordinat->y,
						&pcoordinat->z,
						pcoordinat->v,
						pcoordinat->course,
						&pcoordinat->pitch																																																
					);		
} // moveVturn

  bool moveVturn_od	(	float dAngle, 				// угол поворота
                   		float dt, 					// прошедшее время
                   		MobilityData * pmobilityData,//
                   		ObjectData * pobjectData		// 	 		
					)
{
	return func_moveVturn(
						dAngle,
						dt,
						pmobilityData->wv,
						pmobilityData->rCirkVer,
						&pmobilityData->napravV,
						&pobjectData->x,
						&pobjectData->y,
						&pobjectData->z,
						pobjectData->v,
						pobjectData->course,
						&pobjectData->pitch																																																
					);		
} // moveVturn_od	

	

/*
float moveCountFlyMarshDown( // расчет высоты изменения направления циркуляции
                    	float hWanted,         	// желаемая высота
                   		MobilityData * pmobilityData,//
                   		Coordinat * pcoordinat		// 	 		
      							)
{
	return func_moveCountFlyMarshDown(
						hWanted,
						pmobilityData->pitchMax,
						pmobilityData->rCirkVer,							
						pcoordinat->z,
						pcoordinat->pitch
						);
} // moveCountFlyMarshDown

float moveCountTimeTaxiUp(  // расчет времени на выполнение маневра AutoTaxiUp
                    	float hWanted,         	// желаемая высота
                   		MobilityData * pmobilityData,//
                   		Coordinat * pcoordinat,		//                   		
                        float * ptFirstCirculation,  // время на первую циркуляцию
                        float * ptLineMoving,           // время движения по прямой, если макс.диферент не достигается, то=0
                        float * ptSecondCirculation)   // время на вторую циркуляцию
{
	return func_moveCountTimeTaxiUp(
						hWanted,
						pmobilityData->pitchMax,
						pmobilityData->rCirkVer,
						pcoordinat->z,
						pcoordinat->v,						
						pcoordinat->pitch,
                        ptFirstCirculation,    // время на первую циркуляцию
                        ptLineMoving,          // время движения по прямой, если макс.диферент не достигается, то=0
                        ptSecondCirculation);  // время на вторую циркуляцию
						
						
							
} // moveCountTimeTaxiUp	                        


bool moveTaxiUp		(
                    	float hWanted,         	// желаемая высота
                    	float hFlyMarshDown,   	// высота изменения направления циркуляции
                    	float dt,				// прошедшее время
                   		MobilityData * pmobilityData,//
                   		Coordinat * pcoordinat		//                   		
					)
{
	return func_moveTaxiUp(
						hWanted,
						hFlyMarshDown,
						dt,
						pmobilityData->pitchMax,
						pmobilityData->rCirkVer,
						&pmobilityData->napravV,						
						&pcoordinat->x,
						&pcoordinat->y,
						&pcoordinat->z,
						pcoordinat->v,
						pcoordinat->course,
						&pcoordinat->pitch
						);																														
																									
} // moveTaxiUp

*/
bool moveAutoTaxiUp		( 
                    	float hWanted,         	// желаемая высота
                    	float dt,				// прошедшее время                    	
                   		MobilityData * pmobilityData,//
                   		Coordinat * pcoordinat		//                   		
                    	)
{
	return func_moveAutoTaxiUp(
						hWanted,
						&pmobilityData->hWantedOld,
						&pmobilityData->hFlyMarshDown,
						dt,
						pmobilityData->pitchMax,
						pmobilityData->rCirkVer,
						&pmobilityData->napravV,						
						&pcoordinat->x,
						&pcoordinat->y,
						&pcoordinat->z,
						pcoordinat->v,
						pcoordinat->course,
						&pcoordinat->pitch
						);
} // moveAutoTaxiUp

bool moveAutoTaxiUp_od		( 
                    	float hWanted,         	// желаемая высота
                    	float dt,				// прошедшее время                    	
                   		MobilityData * pmobilityData,//
                   		ObjectData * pobjectData		//                   		
                    	)
{
	return func_moveAutoTaxiUp(
						hWanted,
						&pmobilityData->hWantedOld,
						&pmobilityData->hFlyMarshDown,
						dt,
						pmobilityData->pitchMax,
						pmobilityData->rCirkVer,
						&pmobilityData->napravV,						
						&pobjectData->x,
						&pobjectData->y,
						&pobjectData->z,
						pobjectData->v,
						pobjectData->course,
						&pobjectData->pitch
						);
} // moveAutoTaxiUp_od


bool moveReCountV		(
						float vWanted,         // нужная скорость
						float bChangeRCirk,             // менять ли радиус циркуляции
                  		float accSpeed,              // ускорение для набора.потери скорости
                  		float accTurn,
                  		float dt,
                   		MobilityData * pmobilityData,//
                   		Coordinat * pcoordinat		//                   		
						)
{
	return func_moveReCountV(
					vWanted,
					bChangeRCirk,
					accSpeed,
					accTurn,
					dt,
					&pcoordinat->v,
					&pmobilityData->rCirkHor,
					&pmobilityData->wh,
					&pmobilityData->rCirkVer,
					&pmobilityData->wv
					);
} // moveReCountV

bool moveReCountV_od		(
						float vWanted,         // нужная скорость
						float bChangeRCirk,             // менять ли радиус циркуляции
                  		float accSpeed,              // ускорение для набора.потери скорости
                  		float accTurn,
                  		float dt,
                   		MobilityData * pmobilityData,//
                   		ObjectData * pobjectData		//                   		
						)
{
	return func_moveReCountV(
					vWanted,
					bChangeRCirk,
					accSpeed,
					accTurn,
					dt,
					&pobjectData->v,
					&pmobilityData->rCirkHor,
					&pmobilityData->wh,
					&pmobilityData->rCirkVer,
					&pmobilityData->wv
					);
} // moveReCountV_od


bool moveWithControl(
						float dt,
                   		MobilityData * pmobilityData,						
						Control * pcontrol,
						Coordinat * pcoordinat
					)
{
	bool bNewz, bNewCourse, bNewPitch;
	float dopAngle, dopdt;
	
	if (pcontrol->v > pmobilityData->vMax) pcontrol->v = pmobilityData->vMax;
	if (pcontrol->v < pmobilityData->vMin) pcontrol->v = pmobilityData->vMin;	 
	if (pcontrol->z > pmobilityData->zMax) pcontrol->z = pmobilityData->zMax;
	if (pcontrol->z < pmobilityData->zMin) pcontrol->z = pmobilityData->zMin;	 

	if ( (pcontrol->bNewv) & ( fabs(pcontrol->v - pcoordinat->v) > 0 ) )
	      moveReCountV (pcontrol->v, 
	                    !pmobilityData->bConstCirk,
	                    pmobilityData->maxGravitation,
	                    pmobilityData->maxGravitation,
	                    dt,
	                    pmobilityData,
	                    pcoordinat );
	bNewz      =  ( (pcontrol->bNewz) & (fabs (pcontrol->z - pcoordinat->z) >0 ) );
	bNewPitch  =  ( (pcontrol->bNewPitch) & (cos (pcontrol->pitch - pcoordinat->pitch) < cos (M_PI/180)) ) ;
	bNewCourse =  ( (pcontrol->bNewCourse) & (cos (pcontrol->course - pcoordinat->course) < cos (M_PI/180)) );
	if ((!bNewz) &(pcontrol->bNewz)) {pcoordinat->z = pcontrol->z;pcoordinat->pitch = 0;}
	if ((!bNewCourse) &(pcontrol->bNewCourse)) pcoordinat->course = pcontrol->course;
	if ((!bNewz)& (pcontrol->bNewPitch)) pcoordinat->pitch = pcontrol->pitch;				
	
	if ( (bNewCourse) | (bNewz) | (bNewPitch) )
	{ 
		if (bNewCourse)
		{
			dopAngle = pcontrol->course - pcoordinat->course;
			if (pmobilityData->wh > 0.001) dopdt = fabs(dopAngle/pmobilityData->wh);
				else dopdt = 10*dt;
			if ((bNewPitch) | (bNewz) )
			{
			        moveHturn (dopAngle, 
		    	               minf(dt/2, dopdt),
		        	           pmobilityData,
		            	       pcoordinat);
					dt = dt - minf(dt/2, dopdt);		            	       
			}        	       
				else
			{	 moveHturn   (dopAngle, 
		        	           dt,
		            	       pmobilityData,
		                	   pcoordinat);
		         dt =0;
			}            	   
		} // bNewCourse
		if (bNewPitch)
		{
			dopAngle = pcontrol->pitch - pcoordinat->pitch;
			moveVturn (dopAngle,
			           dt,
			           pmobilityData,
			           pcoordinat);
			dt = 0;           
		} // bNewPitch		
		if (bNewz)
		{
			moveAutoTaxiUp(
						pcontrol->z,
						dt,
						pmobilityData,
						pcoordinat);                
		} // bNewz
	} // ( (bNewCourse) | (bNewz) | (bNewPitch) )
	else
	{
		moveStraight (dt,pcoordinat);
	}		
	return true;                    
} // moveWithControl


bool moveWithControl_od(
						float dt,
                   		MobilityData * pmobilityData,						
						Control * pcontrol,
						ObjectData * pobjectData
					)
{
	bool bNewz, bNewCourse, bNewPitch;
	float dopAngle, dopdt;
	
	if (pcontrol->v > pmobilityData->vMax) pcontrol->v = pmobilityData->vMax;
	if (pcontrol->v < pmobilityData->vMin) pcontrol->v = pmobilityData->vMin;	 
	if (pcontrol->z > pmobilityData->zMax) pcontrol->z = pmobilityData->zMax;
	if (pcontrol->z < pmobilityData->zMin) pcontrol->z = pmobilityData->zMin;	 

	if ( (pcontrol->bNewv) & ( fabs(pcontrol->v - pobjectData->v) > 0 ) )
	      moveReCountV_od (pcontrol->v, 
	                    !pmobilityData->bConstCirk,
	                    pmobilityData->maxGravitation,
	                    pmobilityData->maxGravitation,
	                    dt,
	                    pmobilityData,
	                    pobjectData );
	bNewz      =  ( (pcontrol->bNewz) & (fabs (pcontrol->z - pobjectData->z) >1 ) );
	bNewPitch  =  ( (pcontrol->bNewPitch) & (cos (pcontrol->pitch - pobjectData->pitch) < cos (M_PI/180)) ) ;
	bNewCourse =  ( (pcontrol->bNewCourse) & (cos (pcontrol->course - pobjectData->course) < cos (M_PI/180)) );
	if ((!bNewz) &(pcontrol->bNewz)) {pobjectData->z = pcontrol->z;pobjectData->pitch = 0;}
	if ((!bNewCourse) &(pcontrol->bNewCourse)) pobjectData->course = pcontrol->course;
	if ((!bNewz)& (pcontrol->bNewPitch)) pobjectData->pitch = pcontrol->pitch;				
	
	if ( (bNewCourse) | (bNewz) | (bNewPitch) )
	{ 
		if (bNewCourse)
		{
			dopAngle = pcontrol->course - pobjectData->course;
			if (pmobilityData->wh > 0.001) dopdt = fabs(dopAngle/pmobilityData->wh);
				else dopdt = 10*dt;
			if ((bNewPitch) | (bNewz) )
			{
			        moveHturn_od (dopAngle, 
		    	               minf(dt/2, dopdt),
		        	           pmobilityData,
		            	       pobjectData);
					dt = dt - minf(dt/2, dopdt);		            	       
			}        	       
				else
			{	 moveHturn_od (dopAngle, 
		        	           dt,
		            	       pmobilityData,
		                	   pobjectData);
		         dt =0;
			}            	   
		} // bNewCourse
		if (bNewPitch)
		{
			dopAngle = pcontrol->pitch - pobjectData->pitch;
			moveVturn_od (dopAngle,
			           dt,
			           pmobilityData,
			           pobjectData);
			dt = 0;           
		} // bNewPitch		
		if (bNewz)
		{
			moveAutoTaxiUp_od(
						pcontrol->z,
						dt,
						pmobilityData,
						pobjectData);                
		} // bNewz
	} // ( (bNewCourse) | (bNewz) | (bNewPitch) )
	else
	{
		moveStraight_od (dt,pobjectData);
	}		
	return true;                    
} // moveWithControl_od



float countMeetingTime_od(// расчет точки встречи в случае, когда оружие движется равномерно прямолинейно
                      ObjectData weapon,// координаты и скорость оружия (время на разворот игнорируется)
                      ObjectData target,  // координаты и параметры движения цели
                      Coordinat * meetPoint)
{                      
    float dist, qTarget, timeMeet;
    float root1, root2;
    bool b;
    meetPoint->x = target.x;
    meetPoint->y = target.y;
    meetPoint->z = target.z;
    dist = sqrtf(powf(weapon.x-target.x,2)+powf(weapon.y-target.y,2)+
                 powf(weapon.z-target.z,2));
 	qTarget = getPeleng(weapon.y-target.y*cosf(target.pitch),
 	                    weapon.x-target.x*cosf(target.pitch))-target.course;
 	b  = root(powf(weapon.v,2)-powf(target.v,2),
 	          2*target.v*cosf(qTarget)*dist,
 	          powf(dist,2),
 	          &root1,&root2);
    if (b & ((root1>=0) |(root2>=0)) )
    {
   		timeMeet = maxf(root1,root2);
   		meetPoint->x = target.x+timeMeet*target.v*cosf(target.course)*cosf(target.pitch);
   		meetPoint->y = target.y+timeMeet*target.v*sinf(target.course)*cosf(target.pitch);
   		meetPoint->z = target.z+timeMeet*target.v*sinf(target.pitch);
   		return timeMeet;
	}
	return 86400.0;
}

int getVisualMarksByMinDistance(
				int size,
				VisualMark  pvisualMarks[]
				)
{
	int i, iMin = -1;
	float minD = 10e6;
	for (i = 0; i < size; ++i) 
	{	if ((pvisualMarks[i].iname >=0) &
		    (pvisualMarks[i].distance < minD ))
		   { minD = pvisualMarks[i].distance;
		   	 iMin = i;
		   }
	}
	return iMin;					
}	// 	getVisualMarksByMinDistance



void movingCoordinats(int sizeCoordinats, 		// размер массива
            MobilityData * pmobilities[],		// массив контекстов
            Control  * pcontrols[],			// массив управляющих данных             
            ObjectData  * pobjectDatas[],		// массив координат
            float dt) 							// прошедшее время              
{
	int i;//
	bool b;
	for (i = 0; i < sizeCoordinats; ++i) 
	{
		if (pobjectDatas[i]->iname < 0) continue;
		b = moveWithControl_od(dt, 
		                pmobilities[i],
		                pcontrols[i],
		                pobjectDatas[i]
		                );
	} 
		
} // movingCoordinats

 
   	
   
   
   

