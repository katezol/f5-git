#ifndef __MATHUTILS_H
#define __MATHUTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <commonTypes.h>

/**
 * @file mathutils.h
 * @short математические вычисления
 */
/*#  define PI 3.141596
/* нормализует угол 0..2*PI
 * @param value угол в радианах
 */
float do2pi(float value);

/* нормализует угол -PI..PI
 * @param value угол в радианах
 */
float dopi(float value);           

/* нормализует угол 0..360
 * @param value угол в градусах
 */
float do360(float value);   

/* нормализует угол -180..180
 * @param value угол в градусах
 */
float do180(float value);    

/* вычисляет пеленг, возвращает 0..2*PI
 * пеленг отсчитывается от оси x по часовой стрелке
 * @param deltaX x-x0
 * @param deltaY y-y0
 */
float getPeleng(float deltaY, float deltaX);

/* вычисление курсового параметра */
float countCourseParameter(float peleng, float course, float distanceHor);

/* нахождение angle в секторе anglebegin..angleEnd */
bool isAngleInSector(float angle, float angleBegin, float angleEnd);

/* нахождение точки в круге радиусом rad*/
bool isPointInRad(float x, float y, float rad);

/* поворот точки на угол angle, отсчитываемый по часовой*/
void rotatePoint(float * x, float * y, float angle);

/*решение квадратного уравнения, возвращает true, если есть решение */
bool root(float a, float b, float c, // коэффициенты уравнения
          float * root1, float * root2);
          
float minf(float value1, float value2);

float maxf(float value1, float value2);

//нахождение полярных координат точки из прямоугольных
PolarPoint3d rect2polar(float x, float y, float z);
PolarPoint3d rect2polarPoint(Point3d rect);

//нахождение двумерных полярных координат точки из двумерных прямоугольных
PolarPoint2d rect2polar2d(float x, float y);
PolarPoint2d rect2polarPoint2d(Point2d rect);

//вычисление прямоугольных координат точки из полярных
Point3d polar2rect(float distance, float peleng, float elevation);
Point3d polar2rectPoint(PolarPoint3d polar);

//вычисляет новые координаты точки, двигавшейся из точки point со скоростью speed в течение времени time
Point3d recalcPoint(const Point3d *point, const Point3d *speed, double time);

//вычисляет новые полярные координаты точки pt. diff - хранит направление и величину
//вектора скорости точки в полярной СК
void recalcPolarPoint(PolarPoint3d *pt, const PolarPoint3d *diff, double time);

//вычисляет новые прямоугольные координаты точки point. v - вектор скорости,
//time - время, в течение которого точка перемещалась со скоростью v.
//void recalcPoint(Point3d *point, const Point3d v, double time);

#ifdef __cplusplus
}
#endif

#endif // __MATHUTILS_H
