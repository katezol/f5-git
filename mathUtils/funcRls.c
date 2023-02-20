//#include <stdlib.h>
#include <mathutils.h>
#include <math.h>

#include <commonTypes.h>

float getMaxRlsDistance (         
             float h1,
             float h2)
{
	if ((h1 >=0) & (h2 >=0))
	  return ( 4120.0*(sqrtf(h1)+sqrtf(h2)) );
	  else return 0.0; 
}	              
/* Ретранслятор целей-вилит все!
 */
int getVisualMarksRetranslator(
            int sizeCoordinats,
            ObjectData  * pobjectDatas[],		// массив координат
            Coordinat  * pourShip,
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
}	
            


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
            )
{
	int i, size;
	float dopRr, deltaAngle;
	float distance, peleng, distanceHor, um;
	Point3d leftP;
	
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
		if (bOnlyEnemy & (pobjectDatas[i]->own == owOur)) continue;
		dopRr = maxRr;
		if (isAir(pobjectDatas[i]->objType))
		{ 	if (!bAirTarget) continue;
			dopRr = getMaxRlsDistance (pobjectDatas[i]->z, zAntenna);	
		}
		if (isSurface(pobjectDatas[i]->objType))
		{ 	if (!bSurfTarget) continue;
			dopRr = getMaxRlsDistance ((float)15.0, //*getShipHeight
			                           zAntenna);	
		}
		if (isUnderwater(pobjectDatas[i]->objType))
		{ 	if (!bUnderTarget) continue;
			dopRr = getMaxRlsDistance (pobjectDatas[i]->z+5.0,//getShipHeight
			                           zAntenna);	
		}
		if (dopRr < maxRr) maxRr = dopRr;
		distance = sqrtf ( powf(pobjectDatas[i]->y-pourShip->y,2) + powf(pobjectDatas[i]->x-pourShip->x,2) +
	                             powf(pobjectDatas[i]->z-pourShip->z,2) );
		if ((distance > dopRr) | (distance < minRr)) continue;	                             
		peleng   = getPeleng(pobjectDatas[i]->y - pourShip->y, pobjectDatas[i]->x - pourShip->x);
		if (isSurface(pobjectDatas[i]->objType))
		{
           leftP.x = pobjectDatas[i]->x-pobjectDatas[i]->length*cosf(pobjectDatas[i]->course)/2-
                                        pobjectDatas[i]->width *sinf(pobjectDatas[i]->course)/2;
           leftP.y = pobjectDatas[i]->y-pobjectDatas[i]->length*sinf(pobjectDatas[i]->course)/2+
                                        pobjectDatas[i]->width *cosf(pobjectDatas[i]->course)/2;
                                        
		   deltaAngle = fabsf( dopi(peleng - getPeleng(leftP.y-pourShip->y,leftP.x-pourShip->x)));
		   if (fabsf(dopi(peleng - bissektHor-pourShip->course))>
		                angleScanHor+deltaAngle ) continue;
		} // isSurface
		else 
		{
			if (! isAngleInSector (peleng, bissektHor - angleScanHor,bissektHor + angleScanHor ))
			  continue;
		}	
	    distanceHor= hypotf (pobjectDatas[i]->y - pourShip->y, pobjectDatas[i]->x - pourShip->x);		
		um         = getPeleng(pobjectDatas[i]->z - pourShip->z, distanceHor);
		if (fabsf(dopi(um - bissektVer)) > angleScanVer ) continue; 
		
/*		errno = EOK;
		pvisualMarks[size] = (VisualMark *) malloc(sizeof(VisualMark));
		if ( (errno != EOK) | (pvisualMarks[size]) == NULL ) return size;
*/
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
}	            
