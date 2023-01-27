#include <math.h>
#include "docStructs.h"
#include <string.h>


int DocconvSuTypeTargetToObjType(int typeTarget)
{
	int result;   
	// двойное преобразование!!
	switch (typeTarget)
	    {
	    case 0      : result = 0; break;
	    // воздушные
	    case 1             : result = 8; break;
	    case 2        : result = 8; break;
	    case 3       : result = 8; break;
	    case 4       : result = 8; break;
	    case 5          : result = 11; break;
	    case 6             : result = 11; break;
	    case 7             : result = 12; break;
	    case 8             : result = 8; break;
	    case 9           : result = 13; break;
	    case 10           : result = 10; break;
	    case 11     : result = 10; break;
	    case 12          : result = 10; break;
	    case 13         : result = 10; break;
	    case 14        : result = 10; break;
	    case 15            : result = 10; break;
	    case 16        : result = 10; break;
	    case 17  : result = 10; break;
	    case 18      : result = 10; break;
	    // надводные
	    case 50             : result = 1; break;
	    case 51        : result = 1; break;
	    case 52          : result = 1; break;
	    case 53    : result = 1; break;
	    case 54      : result = 1; break;
	    case 55     : result = 1; break;
	    case 56      : result = 1; break;
	    case 57         : result = 1; break;
	    case 58       : result = 1; break;
	    case 59        : result = 1; break;
	    case 60: result = 1; break;
	    case 61: result = 1; break;
	    case 62     : result = 1; break;
	    case 63        : result = 1; break;
	    case 64    : result = 1; break;
	    case 65     : result = 1; break;
	    case 66     : result = 1; break;
	    case 67 : result = 1; break;
	    case 68      : result = 1; break;
	    case 69             : result = 1; break;
	    case 70             : result = 1; break;
	    case 71            : result = 1; break;
	    case 72             : result = 1; break;
	    case 73             : result = 1; break;
	    case 74             : result = 1; break;

	    // подводные
	    case 100  : result = 0; break;
	    case 101 : result = 0; break;
	    case 102 : result = 0; break;
	    case 103          : result = 0; break;
	    case 104 : result = 0; break;
	    case 105  : result = 0; break;
	    case 106  : result = 0; break;

	    // береговые
	    case 150              : result = 0; break;
	    case 151               : result = 0; break;

	    // специальные
	    case 200          : result = 0; break;
	    case 201              : result = 1; break;
	    case 202         : result = 0; break;
	    case 203           : result = 0; break;
	    case 204    : result = 0; break;
	    case 205           : result = 0; break;
	    case 206      : result = 0; break;
	    case 207       : result = 0; break;
	    case 208             : result = 0; break;
	    case 209       : result = 0; break;
	    case 210            : result = 0; break;
	    case 211        : result = 0; break;
	    case 212       : result = 0; break;
	    case 213       : result = 0; break;
	    case 214     : result = 0; break;
	    case 240          : result = 1; break;
	    case 241       : result = 0; break;
	    case 242              : result = 8; break;
	    case 243          : result = 0; 0;
	    case 244          : result = 0; break;
	    case 245   : result = 0; break;
	    default:  result = 0; break;

	    }
	//ТЦ - тип цели 0-неопр, 1-надв, 8-возд неопр, 9-ловушка, 10-самолет,
	        						// 11-ПКР, 12-ПРР, 13-ВЕРТ, 15-ВПЦ;
	
	return result;
}

int convRead_DocTargetX(DocTargetX_f * targetX_f, DocTargetX_m * targetX_m)
      {
		  int dopSek;
		  float course,speed;
		  memset(targetX_f,0,sizeof(DocTargetX_f));
    	  targetX_f->nn = targetX_m->nn;
    	  targetX_f->sSource = targetX_m->sSource;
    	  targetX_f->SKK = targetX_m->SKK;
    	  targetX_f->SKS = targetX_m->SKS;
    	  targetX_f->bMonevrHor = targetX_m->bMonevrHor;
    	  targetX_f->bMonevrV = targetX_m->bMonevrV;
    	  targetX_f->bMonevrH = targetX_m->bMonevrH;
    	  dopSek = targetX_m->timeSu_mks % 1000000;
    	  targetX_f->timeSu_d = targetX_m->timeSu_s*1.0+dopSek/1000000;
    	  targetX_f->bPeleng = targetX_m->bPeleng;
    	  targetX_f->bDistance = targetX_m->bDistance;
    	  targetX_f->bUm = targetX_m->bUm;
    	  targetX_f->bVx = targetX_m->bVx;
    	  targetX_f->bVy = targetX_m->bVy;
    	  targetX_f->bVz = targetX_m->bVz;
    	  targetX_f->bskoPeleng = targetX_m->bskoPeleng;
    	  targetX_f->bskoDistance = targetX_m->bskoDistance;
    	  targetX_f->bskoUm = targetX_m->bskoUm;
    	  targetX_f->bskoVx = targetX_m->bskoVx;
    	  targetX_f->bskoVy = targetX_m->bskoVy;
    	  targetX_f->bskoVz = targetX_m->bskoVz;
    	  targetX_f->peleng = ((unsigned long)(targetX_m->peleng_hi <<16|targetX_m->peleng_low ))/pow(2,31)*M_PI; 	  
    	  targetX_f->um = targetX_f->bUm *
    	  				((signed long)(targetX_m->um_hi <<16|targetX_m->um_low ))/pow(2,31)*M_PI; 	  
    	  targetX_f->distance = ((unsigned long)(targetX_m->distance_hi <<16|targetX_m->distance_low ))*0.05; 	  
    	  targetX_f->skoPeleng = ((unsigned long)(targetX_m->skoPeleng_hi <<16|targetX_m->skoPeleng_low ))/pow(2,32)*M_PI; 	  
    	  targetX_f->skoUm = ((unsigned long)(targetX_m->skoUm_hi <<16|targetX_m->skoUm_low ))/pow(2,32)*M_PI; 	  
    	  targetX_f->skoDistance = ((unsigned long)(targetX_m->skoDistance))*0.5; 	  
    	  if (targetX_f->SKS == 1) // если вместо Vx, Vx ВИР и ВИП - игнорируем их
    	  {	  
    		  targetX_f->Vx =  (float)(targetX_m->bVx* ((signed short)(targetX_m->Vx))*0.25); 	  
    		  targetX_f->Vy =  (float)(targetX_m->bVy*((signed short)(targetX_m->Vy))*0.25); 	  
    		  targetX_f->Vz =  (float)(targetX_m->bVz*((signed short)(targetX_m->Vz))*0.25); 	  
    		  targetX_f->skoVx = (targetX_m->bskoVx)*((unsigned short)(targetX_m->skoVx))*0.25; 	  
    		  targetX_f->skoVy = (targetX_m->bskoVy)*((unsigned short)(targetX_m->skoVy))*0.25; 	  
    		  targetX_f->skoVz = (targetX_m->bskoVz)*((unsigned short)(targetX_m->skoVz))*0.25; 	
    	  }
    	  if (targetX_f->SKS == 0) // 
    	  {	  
    		 //vx=вип, vz=вир, vy=сиум (не исп)
    		  targetX_f->Vx =  (float)targetX_m->bVx* ((signed short)(targetX_m->Vx))*0.01*M_PI/180; //вип переводим в рад/c
    		  targetX_f->Vz =  (float)targetX_m->bVz*((signed short)(targetX_m->Vz))*0.5; //вир переводим в м/c
    		  course = targetX_f->peleng+atan2f(targetX_f->distance*targetX_f->Vx,targetX_f->Vz);
    		  speed = sqrt(pow(targetX_f->Vz,2)+pow(targetX_f->distance*targetX_f->Vx,2));
    		  targetX_f->Vx =  speed*cos(course);
    		  targetX_f->Vy =  speed*sin(course);
    		  targetX_f->Vz =  0; 	  
    	  }
    	  targetX_f->bnonImit = targetX_m->bnonImit;
    	  targetX_f->GO = targetX_m->GO;
    	  targetX_f->bComplex = targetX_m->bComplex;
    	  targetX_f->typeTarget = DocconvSuTypeTargetToObjType(targetX_m->typeTarget); // требует преобразования
    	  if ((targetX_f->typeTarget==1) | (targetX_f->typeTarget==2)) targetX_f->um =0;
    	  targetX_f->pri = targetX_m->pri;
    	  return 1;
      }
