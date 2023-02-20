#include <math.h>
#include <mathutils.h>
#include <commonTypes.h>

 void func_moveStraight(float dt,				// прошедшее время
                        float * px,				// указатель на координаты 
                   		float * py,				// указатель на координаты
                   		float * pz,				// указатель на координаты
				   		float v,				// текущая скорость
				   		float course,			// текущий курс
				   		float pitch	) 			// текущий угол наклона траекттории	 			 		 
{
 	*px = *px + v*dt*cosf(course)*cosf(pitch);
 	*py = *py +v*dt*sinf(course)*cosf(pitch);
 	*pz = *pz +v*dt*sinf(pitch);
 } // func_moveStraight
 
  bool func_moveHturn(	float dAngle, 				// угол поворота
                   		float dt, 					// прошедшее время
                   		float wh,
                   		float rCirkHor,
                   		int * pnapravH,
                   		float * px,				// указатель на координаты 
                   		float * py,				// указатель на координаты
                   		float * pz,				// указатель на координаты
				   		float v,				// текущая скорость
				   		float * pcourse,		// текущий курс
				   		float pitch)		    // текущий угол наклона траекттории
{
	float dopDt, alfa;
	Point3d centrCirk; 
	if ((dt == 0) | (wh == 0)) return false;
    if (wh > 0) 
            { if (sinf(dAngle) > 0) *pnapravH = 1; else *pnapravH = -1;}
     else { if (sinf(dAngle) > 0)  *pnapravH = -1; else *pnapravH = 1; };
      
   	if (fabsf(dopi(dAngle))>0) 
   	{ if (fabsf (wh) > 0.0001 ) 
   		     dopDt = fabsf (dopi(dAngle)/wh);
   		 else dopDt = 10.0*dt;
   	}
  	else  dopDt = 0.0;

  	centrCirk.x = *px + rCirkHor*cosf(*pcourse+M_PI/2*(*pnapravH))* cosf(pitch);
  	centrCirk.y = *py + rCirkHor*sinf(*pcourse+M_PI/2*(*pnapravH))* cosf(pitch);
  	alfa = *pcourse - M_PI/2*(*pnapravH);
  
  	if (dopDt > dt)
  	{
    	*px = centrCirk.x + rCirkHor*cosf(alfa+(*pnapravH)*wh*dt)* cosf(pitch);
    	*py = centrCirk.y + rCirkHor*sinf(alfa+(*pnapravH)*wh*dt)* cosf(pitch);
    	*pcourse = do2pi(*pcourse + (*pnapravH)*wh*dt);
    	*pz      = *pz + v*dt*sinf(pitch);
    	return false ; // поворот не закончен
  	}  
  	else
  	{
    	*px = centrCirk.x + rCirkHor*cosf(alfa + (*pnapravH)*wh*dopDt)* cosf(pitch);
    	*py = centrCirk.y + rCirkHor*sinf(alfa + (*pnapravH)*wh*dopDt)* cosf(pitch);
    	*pcourse = do2pi(*pcourse+dAngle);
    	*px = *px + v*(dt-dopDt)*cosf(*pcourse)*cosf(pitch);
    	*py = *py + v*(dt-dopDt)*sinf(*pcourse)*cosf(pitch);
    	*pz = *pz + v*dt*sinf(pitch);    	
    	return true; // поворот закончен
  	} 
} // func_moveHTurn     

 bool func_moveVturn(	float dAngle, 				// угол поворота
                   		float dt, 					// прошедшее время
                   		float wv,
                   		float rCirkVer,
                   		int * pnapravV,
                   		float * px,				// указатель на координаты 
                   		float * py,				// указатель на координаты
                   		float * pz,				// указатель на координаты
				   		float v,				// текущая скорость
				   		float course,			// текущий курс
				   		float * ppitch) 		// текущий угол наклона траектории	 			 		 
{
	float dopDt, alfa;
	Point3d centrCirk;
    if ((dt == 0) | (wv == 0)) return false;
    if (sinf(dAngle) > 0) *pnapravV = 1;  else *pnapravV = -1;
    
    
   	if (fabsf(dopi(dAngle))>0) 
   	{ if (fabsf (wv) > 0.0001 ) 
   		     dopDt = fabsf(dopi(dAngle)/wv);
   		 else dopDt = 10*dt;
   	}
  	else  dopDt = 0;

  	centrCirk.z  = *pz + rCirkVer * sinf(*ppitch + (*pnapravV)*M_PI/2);
  	centrCirk.x  = *px + rCirkVer * cosf(course)*cosf(*ppitch + (*pnapravV)*M_PI/2);
  	centrCirk.y  = *py + rCirkVer * sinf(course)*cosf(*ppitch + (*pnapravV)*M_PI/2);  
  	alfa         = *ppitch - M_PI/2*(*pnapravV);
  	if (dopDt > dt)
  	{
    	*pz     = centrCirk.z + rCirkVer*sinf((*pnapravV)*wv*dt+alfa);
    	*px     = centrCirk.x + rCirkVer*cosf(course)*cosf((*pnapravV)*wv*dt+alfa);
    	*py     = centrCirk.y + rCirkVer*sinf(course)*cosf((*pnapravV)*wv*dt+alfa);    
    	*ppitch = dopi(*ppitch + (*pnapravV)*wv*dt);
    	return false; // поворот не завершен
  	}
  	else
  	{
    	*pz     = centrCirk.z + rCirkVer*sinf((*pnapravV)*wv*dopDt+alfa);
    	*px     = centrCirk.x + rCirkVer*cosf(course)*cosf((*pnapravV)*wv*dopDt+alfa);
    	*py     = centrCirk.y + rCirkVer*sinf(course)*cosf((*pnapravV)*wv*dopDt+alfa);    
    	*ppitch = dopi(*ppitch+dAngle);
    	*pz     = *pz + v*sinf(*ppitch)*(dt-dopDt);
    	*px     = *px + v*(dt-dopDt)*cosf(course)*cosf(*ppitch);
    	*py     = *py + v*(dt-dopDt)*sinf(course)*cosf(*ppitch);
    	return true; // поворот завершен    	
  	}  
		
} //func_moveVturn


float func_moveCountFlyMarshDown( // высота изменения направления циркуляции
                    	float hWanted,         	// желаемая высота
                        float pitchMax,         // максимальный диферент
                        float rCirkVer,         // радиус циркуляции
                        float z, 				// текущая высота
                        float pitch,
                        int * pnapravV) 	
{                        
	float hPitchMax;     // высота на максимальном диференте
    float h0;     		// высота на нулевом      диференте
    
    
    if (hWanted > z) *pnapravV =1; else *pnapravV = -1;
    h0         = z + (*pnapravV) * rCirkVer*(cosf(pitch)-1);
    hPitchMax  = z + (*pnapravV) * rCirkVer*(cosf(pitch)-cosf(pitchMax));
    if ((*pnapravV) * hPitchMax > (*pnapravV) * (h0 + hWanted)/2) return (h0+hWanted)/2;
       else return hWanted -(hPitchMax-h0);
}  //func_moveCountFlyMarshDown


float func_moveCountTimeTaxiUp(  // расчет времени на выполнение маневра AutoTaxiUp
                    	float hWanted,         	// желаемая высота
                        float pitchMax,         // максимальный диферент
                        float rCirkVer,         // радиус циркуляции
                        float z, 				// текущая высота
                        float v, 				// текущая скорость                        
                        float pitch, 			// текущая высота
                        float * ptFirstCirculation,  // время на первую циркуляцию
                        float * ptLineMoving,           // время движения по прямой, если макс.диферент не достигается, то=0
                        float * ptSecondCirculation)   // время на вторую циркуляцию
{
	float hPitchMax; 	// высота на максимальном диференте
    float h0;     		// высота на нулевом      диференте
    float hFlyMarshDown;// высота изменения диферента
    float wv;     		// угловая скорость циркуляции
    int napravV;
    
  	if (hWanted > z) napravV = 1; else napravV = -1;
    h0     = z + napravV * rCirkVer*(cosf(pitch)-1);
    hPitchMax = z + napravV * rCirkVer*(cosf(pitch)-cosf(pitchMax));
    if ((rCirkVer < 0.01) | (v<=0.01)) 
    {
        *ptFirstCirculation  =0;
    	*ptLineMoving        =0;
    	*ptSecondCirculation =0;
    	return 0;
    }	
  	wv = v / rCirkVer;
    
    if (napravV * hPitchMax > napravV*(h0+hWanted)/2)
    {
      	hFlyMarshDown        = (h0+hWanted)/2 ;
      	*ptFirstCirculation  = (napravV*acosf(cosf(pitch)-(hFlyMarshDown-z)/(napravV*rCirkVer))-pitch)/
                                   (napravV*wv);
      	*ptLineMoving        = 0;
      	*ptSecondCirculation = napravV*acosf((hFlyMarshDown-hWanted)/(napravV*rCirkVer)+1)/
                                   (napravV*wv);
    }
    else
    {
      	*ptFirstCirculation = fabsf(dopi(pitch-napravV*pitchMax))/wv;
      	hFlyMarshDown       = hWanted-(hPitchMax-h0);
      	*ptLineMoving       = fabsf((hPitchMax-hFlyMarshDown)/(v*sinf(pitchMax)));
      	*ptSecondCirculation= fabsf(dopi(pitchMax))/wv;
    };
    return *ptFirstCirculation + *ptLineMoving + *ptSecondCirculation;
} //func_moveCountTimeTaxiUp


bool func_moveTaxiUp(
                    	float hWanted,         	// желаемая высота
                    	float hFlyMarshDown,   	// высота изменения направления циркуляции
                    	float dt,				// прошедшее время                    	
                        float pitchMax,         // максимальный диферент
                        float rCirkVer,         // радиус циркуляции
                   		int * pnapravV,			// направление цируляции	                        
                   		float * px,				// указатель на координаты 
                   		float * py,				// указатель на координаты
                   		float * pz,				// указатель на координаты
				   		float v,				// текущая скорость
				   		float course,			// текущий курс
                        float * ppitch) 	    // текущий угол наклона траектории
{
	bool bSubEscort; 
    int iUp;
    float tFirstCirculation, tLineMoving, tSecondCirculation;
    float doprCirkVer;
    float dopWv;
    bool b;
    b = false;
    if (rCirkVer <0.01) return b;
    dopWv = v/rCirkVer;
   	if (hFlyMarshDown >= hWanted) iUp = -1; else iUp = 1;
    if (*pz > hFlyMarshDown) bSubEscort = iUp > 0; 
                       else  bSubEscort = iUp < 0;
    if (!bSubEscort)
    { 
	 	if (iUp*(*pz) < iUp*hFlyMarshDown)
	 	{
          	func_moveCountTimeTaxiUp(
                                hWanted ,
                                pitchMax,
                                rCirkVer,
                                *pz,
                                v,
                                *ppitch,
                                &tFirstCirculation,
                                &tLineMoving,
                                &tSecondCirculation);
           	if ((tFirstCirculation + tLineMoving <= 0) | (tFirstCirculation+tLineMoving >= dt))
            	func_moveVturn(iUp*pitchMax-(*ppitch),dt,
            	               dopWv,rCirkVer,pnapravV,px,py,pz,v,course,ppitch);
            else
            {
            	func_moveVturn(iUp*pitchMax-(*ppitch),tFirstCirculation+tLineMoving,
            	                 dopWv,rCirkVer,pnapravV,px,py,pz,v,course,ppitch);
                dt = dt - tFirstCirculation-tLineMoving;
                *pnapravV = -iUp;
                bSubEscort = true;
            }   
	 	}
        else
        {
        	*pnapravV = -iUp;
            bSubEscort = true;
        }    
    }  // (!bSubEscort)
    if (bSubEscort) 
	{
		if ( fabs((*pz - hWanted)< 0.00001)) doprCirkVer = rCirkVer;
		else if (cosf(*ppitch) != 1)  doprCirkVer = fabs((*pz - hWanted)/(1-cosf(*ppitch)));   // временно, чтобы выйти на заданную глубину
             else doprCirkVer = rCirkVer;  // с нулевым диферентом
        dopWv = v / doprCirkVer;                                       
        if ( func_moveVturn(0-(*ppitch),dt,dopWv,doprCirkVer,pnapravV,px,py,pz,v,course,ppitch) |
             (iUp*(*pz) >= iUp*hWanted))  { *ppitch = 0; b = true; };
	}
	return b;	
} // func_moveTaxiUp

bool func_moveAutoTaxiUp( 
                    	float hWanted,         	// желаемая высота
                    	float * phWantedOld,   	// указатель на желаемую высоту на предыдущем цикле                    	
                    	float * phFlyMarshDown, // указатель на высоту изменения направления циркуляции
                    	float dt,				// прошедшее время                    	
                        float pitchMax,         // максимальный диферент
                        float rCirkVer,         // радиус циркуляции
                   		int * pnapravV,			// направление цируляции	                        
                   		float * px,				// указатель на координаты 
                   		float * py,				// указатель на координаты
                   		float * pz,				// указатель на координаты
				   		float v,				// текущая скорость
				   		float course,			// текущий курс
                        float * ppitch) 	    // текущий угол наклона траектрории
{                        
    if (hWanted   != *phWantedOld) 
   		*phFlyMarshDown = func_moveCountFlyMarshDown(hWanted,pitchMax,rCirkVer,*pz,*ppitch,pnapravV);
    *phWantedOld = hWanted;
    if (func_moveTaxiUp(hWanted,*phFlyMarshDown,dt,pitchMax,rCirkVer,
                       pnapravV,px,py,pz,v,course,ppitch)) return true;
    return false;
} // func_moveAutoTaxiUp

float func_CountRCirk(
						float vCurr,
						float accTurn)
{						
   if ( fabs(accTurn) <0.001) accTurn = 0.001;
   return ( vCurr * vCurr / accTurn );
} //func_CountRCirk  


bool func_moveReCountV(
						float vWanted,         // нужная скорость
						float bChangeRCirk,             // менять ли радиус циркуляции
                  		float accSpeed,              // ускорение для набора.потери скорости
                  		float accTurn,
                  		float dt,
                  		float * pv,
             			float * prCirkHor,
             			float * pwh,
             			float * prCirkVer,
             			float * pwv)
{             			
	int iUp;
	bool b;
  	if ((*pv) < vWanted) iUp = 1; else iUp = -1;
  	accSpeed = fabs(accSpeed) * iUp;
  	if (iUp*(*pv + iUp*dt*accSpeed) >= iUp*vWanted)
  		{ 	*pv = vWanted;  	b = true;  	}	   
    else
    	{  	*pv  = *pv + accSpeed * dt;	b  = false;  }
    if ( bChangeRCirk)
    	{	*prCirkVer  = func_CountRCirk(*pv , accTurn);
        	*prCirkHor  = *prCirkVer;
        	*pwv        = *pv / *prCirkVer;
        	*pwh     	= *pwv;
    	}
    return b;
} // func_moveReCountV

          
 
 
   	
   
   
   

