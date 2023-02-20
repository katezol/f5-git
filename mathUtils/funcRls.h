#ifndef FUNCRLS_H_
#define FUNCRLS_H_

#include <commonTypes.h>

/* вычисление надгоризонтонтной видимости между двумя точками
  *  @param h1 высота первой точки
  *  @param h2 высота второй точки
  *  @return дальность видимости в метрах	
  * */
float getMaxRlsDistance (         
             float h1,
             float h2);
             
/* вычисление видимых объектов
  *  @param sizeCoordinats
  *  @param pobjectDatas указатель на кординаты
  *  @param pourShip указатель на кординаты носителя антенны
  *  @param zAntenna  высота антенны над уровнем моря (не суммируется с pourShip->z)
  *  @param bissektHor центр угла сканирования (от севера)
  *  @param angleScanHor пол-сектора в горизонтальной оси 
  *  @param bissektVer центр угла сканирования в верт.плоскости (от плоскости горизонта)
  *  @param angleScanVer пол-сектора в вертикальной оси
  *  @param minRr минимальный радиус видимости
  *  @param maxRr максимальный радиус видимости
  *  @param bSurfTarget видим ли надводные цели
  *  @param bUnderTarget видим ли подводные цели
  *  @param bAirTarget видим ли воздушные цели
  *  @param bOnlyEnemy сопровождать только врага
  *  @param pvisualMarks[] указатель на видимые отметки      
  *  @return количество видимых отметок	
  * */
int getVisualMarks(
           int sizeCoordinats,
            ObjectData  * pobjectDatas[],		// массив координат
            Coordinat  * pourShip,
            float zAntenna,
            float bissektHor,
            float angleScanHor,
            float bissektVer,
            float angleScanVer,
            float minRr,
            float maxRr,
            bool bSurfTarget,
            bool bUnderTarget,
            bool bAirTarget,
            bool bOnlyEnemy,
            int nVisualMarks,            
            VisualMark visualMarks[]
            );
/* Ретранслятор целей-видит все!
 */
int getVisualMarksRetranslator(
            int sizeCoordinats,
            ObjectData  * pobjectDatas[],		// массив координат
            Coordinat  * pourShip,
            int nVisualMarks,
            VisualMark visualMarks[]
            );
            

            				

#endif /*FUNCRLS_H_*/
