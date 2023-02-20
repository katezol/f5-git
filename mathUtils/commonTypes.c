#include <commonTypes.h>
#include <string.h>

 /* цель воздушная?*/
 bool isAir(ObjType objType)
 {
 	return ( (objType>=otAir) & (objType<=otTrap) );	  
 }; 	
   /* цель надводная?*/
 bool isSurface(ObjType objType)
 {
    return ((objType == otSurface) | (objType == otShip)) ;
 };
 
   /* цель надводный корабль?*/   
 bool isShip(ObjType objType)
{
	return ( objType == otShip); 
}

   /* цель какая-нибудь ракета?*/   
  bool isRocket(ObjType objType)
 {
 	 return ((objType == otMissileCommon) |(objType == otPkr) |
 	         (objType == otPrr));
 }	  
 
   /* цель подводная?*/
 bool isUnderwater(ObjType objType)
 {
    return (false);
 };
 
 char * getStrObjType(ObjType objType)
 {
	switch (objType) {
		case otSurface: return "надводная";
    		    break;		
		case otShip: return "корабль";
    		    break;
		case otAir: return "воздушная";
    		    break;				
		case otPlane: return "самолет";
    		    break;    		    
		case otLft: return "низколетящая";
    		    break;    		    
		case otFalseTarget: return "ложная";
    		    break;    		    
		case otMissileCommon: return "ракета";
    		    break;
		case otPkr: return "ПКР";
    		    break;    		        		    
		case otPrr: return "ПРР";
    		    break;    		        		    
		case otHelicopter: return "вертолет";
    		    break;
		case otPn: return "помехоноситель";
    		    break;    		        		    
		case otSat: return "ВПЦ";
    		    break;    		        		    
		case otTrap: return "ловушка";
			    break;    		        		    
		default: return "самолет";
    		    break;    		    
			}
 };
 
 
void setTypicalMobilityDataFromObjType (
              ObjType objType, 
              MobilityData * pmobilityData)
{
		float vav;
		memset(pmobilityData, sizeof(MobilityData),0);
		pmobilityData->maxGravitation = 0.02;
		pmobilityData->zMax = 0.0;
		pmobilityData->zMin = 0.0;
  	 	pmobilityData->vMax = 25.0;
		pmobilityData->vMin = 0.0;
	    vav = 5.0;		
		pmobilityData->pitchMax = 0.0;
		if (isAir(objType))
		{
			pmobilityData->maxGravitation = 5.0 * 9.8;
			pmobilityData->zMax = 25000.0;
			pmobilityData->zMin = 0.0;
  	 		pmobilityData->vMax = 750.0;
			pmobilityData->vMin = 0.0;
			pmobilityData->pitchMax = 60*3.1416/180;
			vav = (pmobilityData->vMax +	pmobilityData->vMin) / 2;
		}
		pmobilityData->rCirkHor = vav * vav / pmobilityData->maxGravitation;
		pmobilityData->rCirkVer = pmobilityData->rCirkHor;
		pmobilityData->wh       = pmobilityData->vMax / pmobilityData->rCirkHor;
		pmobilityData->wv       = pmobilityData->wh;
}	  


            
 
 
   
  
   	
   
   
   

