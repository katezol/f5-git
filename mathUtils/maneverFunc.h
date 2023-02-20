#ifndef MANEVERFUNC_H_
#define MANEVERFUNC_H_

/* содержит функции маневров в горизонтальной и вертикальной плоскостях */

 /* прямолинейное движение
  *  @param dt прошедшее время
  *  @param px указатель на координату
  *  @param py указатель на координату
  *  @param pz указатель на координату
  *  @param v текущая скорость
  *  @param course текущий курс
  *  @param pitch текущий угол наклона траекттории
  * */
 void func_moveStraight(float dt,				// прошедшее время
                        float * px,				// указатель на координаты 
                   		float * py,				// указатель на координаты
                   		float * pz,				// указатель на координаты
				   		float v,				// текущая скорость
				   		float course,			// текущий курс
				   		float pitch				// текущий угол наклона траекттории	 			 		 
                   );	
 
 /* маневр в горизонтальной плоскости
  *  @param dAngle угол поворота
  *  @param dt прошедшее время
  *  @param wh
  *  @param rCirkHor
  *  @param pnapravH,
  *  @param px указатель на координату
  *  @param py указатель на координату
  *  @param pz указатель на координату
  *  @param pv указатель на скорость
  *  @param pcourse указатель на курс
  *  @param ppitch указатель на угол наклона траекттории
  *  @return закончен ли маневр	
  * */
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
				   		float pitch);		    // текущий угол наклона траекттории	 			 		 

/* маневр в вертикальной плоскости
  *  @param dAngle угол поворота
  *  @param dt прошедшее время
  *  @param wv
  *  @param rCirkVer
  *  @param pnapravV,
  *  @param px указатель на координату
  *  @param py указатель на координату
  *  @param pz указатель на координату
  *  @param v скорость
  *  @param course курс
  *  @param ppitch указатель на угол наклона траекттории
  *  @return закончен ли маневр	
  * */
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
				   		float * ppitch);		// текущий угол наклона траектории	 			 		 



/* вычисление высоты изменения направления циркуляции
  *  @param hWanted желаемая высота
  *  @param pitchMax максимальный угол наклона 
  *  @param rCirkVer радиус циркуляции
  *  @param z высота
  *  @param pitch угол наклона траекттории
  *  @return высота изменения направления циркуляции	
  * */
float func_moveCountFlyMarshDown( // высота изменения направления циркуляции
                    	float hWanted,         	// желаемая высота
                        float pitchMax,         // максимальный диферент
                        float rCirkVer,         // радиус циркуляции
                        float z, 				// текущая высота
                        float pitch,
                        int * pnapravV); 			

/* вычисление времени на выполнение маневра AutoTaxiUp
  *  @param hWanted желаемая высота
  *  @param pitchMax максимальный угол наклона 
  *  @param rCirkVer радиус циркуляции
  *  @param z высота
  *  @param v скорость 
  *  @param pitch угол наклона траектории
  *  @param ptFirstCirculation время на первую циркуляцию
  *  @param ptLineMoving время движения по прямой, если макс.диферент не достигается, то=0
  *  @param ptSecondCirculation время на вторую циркуляцию
  *  @return общее время, необходимое на выполнение маневра AutoTaxiUp	
  * */
float func_moveCountTimeTaxiUp(  // расчет времени на выполнение маневра AutoTaxiUp
                    	float hWanted,         	// желаемая высота
                        float pitchMax,         // максимальный диферент
                        float rCirkVer,         // радиус циркуляции
                        float z, 				// текущая высота
                        float v, 				// текущая скорость                        
                        float pitch, 			// текущая высота
                        float * ptFirstCirculation,  // время на первую циркуляцию
                        float * ptLineMoving,           // время движения по прямой, если макс.диферент не достигается, то=0
                        float * ptSecondCirculation);   // время на вторую циркуляцию


/* расчет выхода на заданную высоту с нулевым углом наклона
  *  @param hWanted желаемая высота
  *  @param hFlyMarshDown высота перегиба 
  *  @param dt прошедшее время   
  *  @param pitchMax максимальный угол наклона 
  *  @param rCirkVer радиус циркуляции
  *  @param pnapravV,
  *  @param px указатель на координату
  *  @param py указатель на координату
  *  @param pz указатель на координату
  *  @param v скорость
  *  @param course курс
  *  @param ppitch указатель на угол наклона траекттории
  *  @return 	
  * */
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
                        float * ppitch); 	    // текущий угол наклона траектории


/* расчет автоматного выхода на заданную высоту с нулевым углом наклона
  *  @param hWanted желаемая высота
  *  @param phWantedOld указатель на желаемую высота на предыдущем цикле 
  *  @param phFlyMarshDown указатель на высоту перегиба 
  *  @param dt прошедшее время
  *  @param pitchMax максимальный угол наклона  
  *  @param rCirkVer радиус циркуляции   
  *  @param pnapravV,
  *  @param px указатель на координату
  *  @param py указатель на координату
  *  @param pz указатель на координату
  *  @param v скорость
  *  @param course курс
  *  @param ppitch указатель на угол наклона траекттории
  *  @return true, если маневр закончен	
  * */
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
                        float * ppitch); 	    // текущий угол наклона траектрории

/* расчет радиуса поворота 
  *  @param vСurr текущая скорость
  *  @param accTurn располагаемая перегрузка
  *  @return радиус поворота 	
  * */
float func_CountRCirk(
						float vCcurr,
						float accTurn);


/* пересчет скорости
  *  @param vWanted желаемая скорость
  *  @param bChangeRCirk пересчитывать ли радиусы циркуляции
  *  @param accSpeed уcкорение на наборе скорости 
  *  @param accTurn ускорение на поворотах
  *  @param dt прошедшее время
  *  @param prCirkVer радиус циркуляции
  *  @param pwv угловая скорость
  *  @param prCirkHor радиус циркуляции
  *  @param pwh угловая скорость   
  *  @return true, 	
  * */
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
             			float * pwv);


#endif /*MANEVERFUNC_H_*/
