#ifndef MOBILITY_H_
#define MOBILITY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <commonTypes.h>




 /* прямолинейное движение
  *  @param dt прошедшее время
  *  @param pcoordinat указатель на координаты
  * */
 void 	moveStraight	(float dt,				// прошедшее время
 						Coordinat * pcoordinat
                   		);	
 /* прямолинейное движение 
  *  @param dt прошедшее время
  *  @param pobjectData указатель на координаты
  * */
 void 	moveStraight_od	(float dt,				// прошедшее время
 						ObjectData * pobjectData
                   		);	
 

/* маневр в горизонтальной плоскости
  *  @param dAngle угол поворота
  *  @param dt прошедшее время
  *  @param pmobilityData указатель на контекст
  *  @param pcoordinat указатель на кординаты 
  *  @return закончен ли маневр	
  * */
  bool moveHturn	(	float dAngle, 				// угол поворота
                   		float dt, 					// прошедшее время
                   		MobilityData * pmobilityData,//
                   		Coordinat * pcoordinat		// 	 		
					);
/* маневр в горизонтальной плоскости
  *  @param dAngle угол поворота
  *  @param dt прошедшее время
  *  @param pmobilityData указатель на контекст
  *  @param pobjectData указатель на кординаты 
  *  @return закончен ли маневр	
  * */
  bool moveHturn_od	(	float dAngle, 				// угол поворота
                   		float dt, 					// прошедшее время
                   		MobilityData * pmobilityData,//
                   		ObjectData * pobjectData		// 	 		
					);	
						
					
/* маневр в вертикальной плоскости
  *  @param dAngle угол поворота
  *  @param dt прошедшее время
  *  @param pmobilityData указатель на контекст
  *  @param pcoordinat указатель на кординаты 
  *  @return закончен ли маневр	
  * */
  bool moveVturn	(	float dAngle, 				// угол поворота
                   		float dt, 					// прошедшее время
                   		MobilityData * pmobilityData,//
                   		Coordinat * pcoordinat		// 	 		
					);
/* маневр в вертикальной плоскости
  *  @param dAngle угол поворота
  *  @param dt прошедшее время
  *  @param pmobilityData указатель на контекст
  *  @param pobjectData указатель на кординаты 
  *  @return закончен ли маневр	
  * */
  bool moveVturn_od	(	float dAngle, 				// угол поворота
                   		float dt, 					// прошедшее время
                   		MobilityData * pmobilityData,//
                   		ObjectData * pobjectData		// 	 		
					);	
						



/* вычисление высоты изменения направления циркуляции
  *  @param hWanted желаемая высота
  *  @param pmobilityData указатель на контекст
  *  @param pcoordinat указатель на кординаты 
  *  @return высота изменения направления циркуляции	
  * */
/*float moveCountFlyMarshDown( // расчет высоты изменения направления циркуляции
                    	float hWanted,         	// желаемая высота
                   		MobilityData * pmobilityData,//
                   		Coordinat * pcoordinat		// 	 		
      							);              	
*/
/* вычисление времени на выполнение маневра AutoTaxiUp
  *  @param hWanted желаемая высота
  *  @param pmobilityData указатель на контекст
  *  @param pcoordinat указатель на кординаты 
  *  @param ptFirstCirculation время на первую циркуляцию
  *  @param ptLineMoving время движения по прямой, если макс.диферент не достигается, то=0
  *  @param ptSecondCirculation время на вторую циркуляцию
  *  @return общее время, необходимое на выполнение маневра AutoTaxiUp	
  * */
/*float moveCountTimeTaxiUp(  // расчет времени на выполнение маневра AutoTaxiUp
                    	float hWanted,         	// желаемая высота
                   		MobilityData * pmobilityData,//
                   		Coordinat * pcoordinat,		//                   		
                        float * ptFirstCirculation,  // время на первую циркуляцию
                        float * ptLineMoving,           // время движения по прямой, если макс.диферент не достигается, то=0
                        float * ptSecondCirculation);   // время на вторую циркуляцию

*/
/* расчет выхода на заданную высоту с нулевым углом наклона
  *  @param hWanted желаемая высота
  *  @param hFlyMarshDown высота перегиба 
  *  @param dt прошедшее время
  *  @param pmobilityData указатель на контекст
  *  @param pcoordinat указатель на кординаты 
  *  @return 	
  * */
/*bool moveTaxiUp		(
                    	float hWanted,         	// желаемая высота
                    	float hFlyMarshDown,   	// высота изменения направления циркуляции
                    	float dt,				// прошедшее время
                   		MobilityData * pmobilityData,//
                   		Coordinat * pcoordinat		//                   		
					);
*/
/* расчет автоматного выхода на заданную высоту с нулевым углом наклона
  *  @param hWanted желаемая высота
  *  @param dt прошедшее время
  *  @param pmobilityData указатель на контекст
  *  @param pcoordinat указатель на кординаты 
  *  @return true, если маневр закончен	
  * */
bool moveAutoTaxiUp		( 
                    	float hWanted,         	// желаемая высота
                    	float dt,				// прошедшее время                    	
                   		MobilityData * pmobilityData,//
                   		Coordinat * pcoordinat		//                   		
                    	);                    	
/* расчет автоматного выхода на заданную высоту с нулевым углом наклона
  *  @param hWanted желаемая высота
  *  @param dt прошедшее время
  *  @param pmobilityData указатель на контекст
  *  @param pobjectData указатель на кординаты 
  *  @return true, если маневр закончен	
  * */
bool moveAutoTaxiUp_od		( 
                    	float hWanted,         	// желаемая высота
                    	float dt,				// прошедшее время                    	
                   		MobilityData * pmobilityData,//
                   		ObjectData * pobjectData		//                   		
                    	);                    	


/* пересчет скорости
  *  @param vWanted желаемая скорость
  *  @param bChangeRCirk пересчитывать ли радиусы циркуляции
  *  @param accSpeed уcкорение на наборе скорости 
  *  @param accTurn ускорение на поворотах
  *  @param dt прошедшее время
  *  @param pmobilityData указатель на контекст
  *  @param pcoordinat указатель на кординаты 
  *  @return true, 	
  * */
bool moveReCountV		(
						float vWanted,         // нужная скорость
						float bChangeRCirk,             // менять ли радиус циркуляции
                  		float accSpeed,              // ускорение для набора.потери скорости
                  		float accTurn,
                  		float dt,
                   		MobilityData * pmobilityData,//
                   		Coordinat * pcoordinat		//                   		
						);
/* пересчет скорости
  *  @param vWanted желаемая скорость
  *  @param bChangeRCirk пересчитывать ли радиусы циркуляции
  *  @param accSpeed уcкорение на наборе скорости 
  *  @param accTurn ускорение на поворотах
  *  @param dt прошедшее время
  *  @param pmobilityData указатель на контекст
  *  @param pobjectData указатель на кординаты 
  *  @return true, 	
  * */
bool moveReCountV_od		(
						float vWanted,         // нужная скорость
						float bChangeRCirk,             // менять ли радиус циркуляции
                  		float accSpeed,              // ускорение для набора.потери скорости
                  		float accTurn,
                  		float dt,
                   		MobilityData * pmobilityData,//
                   		ObjectData * pobjectData		//                   		
						);
						
						
/* пересчет скорости
  *  @param dt  прошедшее время
  *  @param pmobilityData указатель на контекст
  *  @param pcontrol указатель на управляющие данные
  *  @param pcoordinat указатель на кординаты 
  *  @return true, 	
  * */
bool moveWithControl(
						float dt,
                   		MobilityData * pmobilityData,						
						Control * pcontrol,
						Coordinat * pcoordinat
					);	
/* пересчет скорости
  *  @param dt  прошедшее время
  *  @param pmobilityData указатель на контекст
  *  @param pcontrol указатель на управляющие данные
  *  @param pobjectData указатель на кординаты* 
  *  @return true, 	
  * */
bool moveWithControl_od(
						float dt,
                   		MobilityData * pmobilityData,						
						Control * pcontrol,
						ObjectData * pobjectData
					);	
					
float countMeetingTime_od(// расчет точки встречи в случае, когда оружие движется равномерно прямолинейно
                      ObjectData weapon,// координаты и скорость оружия (время на разворот игнорируется)
                      ObjectData target,  // координаты и параметры движения цели
                      Coordinat * meetPoint);
					
											
/* движение объектов в соответствии и управляющими воздействиями
  *  @param sizeCoordinats размер массивов
  *  @param pmobilities  массив указателелей на контекст
  *  @param pcontrols массив указателей на управление 
  *  @param pobjectDatas массив указателей на координаты
  *  @param dt прошедшее время
  *  @return закончен ли маневр	
  * */
void movingCoordinats(int sizeCoordinats, 		// размер массива
            MobilityData * pmobilities[],		// массив контекстов
            Control  * pcontrols[],				// массив управляющих данных             
            ObjectData  * pobjectDatas[],			// массив координат
            float dt); 							// прошедшее время
					

int getVisualMarksByMinDistance(
				int size,
				VisualMark  pvisualMarks[]
				);



#ifdef __cplusplus
}
#endif

#endif /*MOBILITY_H_*/
