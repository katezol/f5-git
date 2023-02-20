#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mathutils.h>
	  
float do2pi(float value)
{
	float result = value;
	while (result >= 2*M_PI) result = result-2*M_PI;
    while (result < 0)     result = result+2*M_PI;
    return result;
}

float dopi(float value)           
{
	float result = value;
    while (result > M_PI) result = result - 2*M_PI;
    while (result <-M_PI) result = result + 2*M_PI;
	return result;
}

float do360(float value)   
{
	float result = value;
	while (result >= 360) result = result - 360;
    while (result < 0)     result = result + 360;
    return result;
}

float do180(float value)    
{
	float result = value;
    while (result > 180) result = result - 360;
    while (result <-180) result = result + 360;
	return result;
}

float getPeleng(float deltaY, float deltaX)
{
    return do2pi(atan2f(deltaY,deltaX));
}	

/* вычисление курсового параметра */
float countCourseParameter(float peleng, float course, float distanceHor)
{
	float qt;
	qt = dopi(peleng+M_PI-course);
    return (fabs(distanceHor * sin(qt)));
}

/* нахождение angle в секторе anglebegin..angleEnd */
bool isAngleInSector(float angle, float angleBegin, float angleEnd)
{
	if (fabs(dopi(angleBegin-angleEnd)) < 0.0001) return true;
	return ( (do2pi(angle - angleBegin) <= do2pi (angleEnd - angleBegin)) &
             (do2pi(angleEnd - angle) <= do2pi (angleEnd - angleBegin)) );	
}

/* нахождение точки в круге радиусом rad*/
bool isPointInRad(float x, float y, float rad)
{
 if (fabs(rad)>0)
   return (x*x+y*y<=rad*rad);
   else return false;  	
}

/* поворот точки на угол angle, отсчитываемый по часовой*/
void rotatePoint(float * x, float * y, float angle)
{
 	float xres,yres;
 	xres = (*x)*cos(angle) - (*y)*sin(angle);
 	yres = (*x)*sin(angle) + (*y)*cos(angle);
 	*x = xres;
 	*y = yres;  	
}	

/*решение квадратного уравнения, возвращает true, если есть решение */
bool root(float a, float b, float c, // коэффициенты уравнения
          float * root1, float * root2)
{          
	float rab;
	*root1 = 0;
	*root2 = 0;
	rab = powf(b,2)-4*a*c;
    if (rab<0) return false; // нет решений
    if (a == 0)
    {  if (b == 0)  return false; // решение - любое число
       *root1 = -c/b;      // линейное уравнение
       *root2 = *root1;     //
    } else   
    {   *root1 = (-b + sqrtf(rab))/(2*a);      // квадратное уравнение
        *root2 = (-b - sqrtf(rab))/(2*a);      //
    }
    return true;
}

float minf(float value1, float value2)
{
	if (value1 < value2) return value1; else return value2;
}

float maxf(float value1, float value2)
{
	if (value1 < value2) return value2; else return value1;
}

//вычисление полярных координат точки из прямоугольных
PolarPoint3d rect2polar(float x, float y, float z)
{
	PolarPoint3d point;
	
	point.distance = sqrt(x*x + y*y + z*z);
	point.peleng = do2pi(atan2f(y, x));
	point.elevation = dopi( atan2f(z, sqrt(x*x + y*y)) );
	
	return point;
}
PolarPoint3d rect2polarPoint(Point3d rect)
{	
	return rect2polar(rect.x, rect.y, rect.z);
}
PolarPoint2d rect2polar2d(float x, float y)
{
	PolarPoint2d point;
	
	point.distance = sqrt(x*x + y*y);
	point.peleng = do2pi(atan2f(y, x));
	
	return point;
}
PolarPoint2d rect2polarPoint2d(Point2d rect)
{
	return rect2polar2d(rect.x, rect.y);
}
//вычисление прямоугольных координат точки из полярных
Point3d polar2rect(float distance, float peleng, float elevation)
{
	Point3d rect;
	float horDist;
	
	horDist = distance * cos(elevation);
	
	rect.x = horDist * cos(peleng);
	rect.y = horDist * sin(peleng);
	rect.z = distance * sin(elevation);
	
	return rect;
}
Point3d polar2rectPoint(PolarPoint3d polar)
{	
	return polar2rect(polar.distance, polar.peleng, polar.elevation);
}
//пересчет координат точки pt, движущейся прямолинейно с вектором скорости diff.
Point3d recalcPoint(const Point3d *point, const Point3d *speed, double time)
{
	Point3d result;
	
	result.x = point->x + speed->x*time;
	result.y = point->y + speed->y*time;
	result.z = point->z + speed->z*time;
	
	return result;
}
void recalcPolarPoint(PolarPoint3d *pt, const PolarPoint3d *diff, double time)
{
	Point3d ptRect, diffRect;
	
	ptRect = polar2rectPoint(*pt);
	diffRect = polar2rectPoint(*diff);
	
	ptRect.x += diffRect.x * time;
	ptRect.y += diffRect.y * time;
	ptRect.z += diffRect.z * time;
	
	*pt = rect2polarPoint(ptRect);
}        





