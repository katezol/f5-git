#include <math.h>

#include "docViewParser.h"
#include "docStructs.h"



typedef struct 
{
	float x;
	float y;
	float z;	
	float pel;
	float dist;
	float distHor;
	float um;
	float course;
	float speed;
}ViewCountTP;

typedef struct 
{
  float x;									// координата x, м или пеленг, если bPassive
  
  float y;									// координата y, м
  float z;									// координата z, м или ум, если bPassive
  float vx;				
  // проекция скорости на x или скорость изменения пеленга, м/с
  float vy;									// проекция скорости на y, м/с
  float vz;									// проекция скорости на z или скорость изменения ум, м/с
  float skoX;
  float skoY;
  float skoZ;
  float skoVx;
  float skoVy;
  //float skoVz;
  bool bNew;									// признак завязки новой трассы
  bool bKt;									// признак привязки КТ к трассе (0 - ДНО)
  bool bPrc;									// признак приоритетной цели
  byte pad0;

  /************ эти данные считаются отдельно ******************/
  
  float distance;								// дистанция наклонная, м
  float distanceHor;							// дистанция горизонтальная, м
  float peleng;								// пеленг, рад
  float um;    								// УМ, рад
  float course;								// курс цели, рад
  float vr;									// м/с радиальная скорость
  /***************************************************************/
 }ViewRlsTarget;
 
extern int DocconvSuTypeTargetToObjType(int typeTarget);
extern int convRead_DocTargetX(DocTargetX_f * targetX_f, DocTargetX_m * targetX_m);

 float viewDo2pi(float value)
 {
 	float result = value;
 	while (result >= 2*M_PI) result = result-2*M_PI;
     while (result < 0)     result = result+2*M_PI;
     return result;
 }

 float viewDopi(float value)           
 {
 	float result = value;
     while (result > M_PI) result = result - 2*M_PI;
     while (result <-M_PI) result = result + 2*M_PI;
 	return result;
 }
 
 float viewDo180(float value)    
 {
 	float result = value;
     while (result > 180) result = result - 360;
     while (result <-180) result = result + 360;
 	return result;
 }
 float viewDo360(float value)    
 {
 	float result = value;
     while (result > 360) result = result - 360;
     while (result <0) result = result + 360;
 	return result;
 }

 double viewDecodeValue(unsigned short wrd , double LOB_worth, int sign, int size){
	 float value = 0.0f;
	  	int counter;
	  	bool negative = 0;
	  	
	  	if(LOB_worth == 0.0)
	  		return 0;
	  	
	  	if(sign)
	  	{
	  		counter = size-2;
	  		if(wrd>>size-1)	negative = 1;
	  	}
	  	else
	  		counter = size-1;
	  	
	  	if(negative)
	  		wrd = ~(wrd-0x1);
	  	
	  	for(; counter>=0; --counter)
	  	{
	  		value += ( wrd & 0x1<<counter ) * LOB_worth;
	  	}
	  	if(negative)
	  		value *= -1;
	  	
	  	return value;
 }
 
 

 
void data2viewFormularTrassa(DocFormularTrassa* p, char *str){
	ViewRlsTarget viewRlsTarget;
	char tmpStr[800];
	
	strcpy(str, " ");
	
	viewRlsTarget.bNew = p->NEW;
	viewRlsTarget.bKt = p->bKT;
	viewRlsTarget.bPrc = p->PRC;
	
	
	memset(&viewRlsTarget, 0, sizeof(viewRlsTarget));

	if (p->NEW)
		sprintf(tmpStr, "Нт=%d ! ", p->nSource);
	else
		sprintf(tmpStr, "Нт=%d   ", p->nSource);

	strcat(str, tmpStr);

		
	if(p->ShP){
		viewRlsTarget.peleng = (float)p->X*M_PI/(1<<15);
		viewRlsTarget.distance = 250000.0;
		viewRlsTarget.um = (float)(signed short)p->H*M_PI/(1<<16);
		viewRlsTarget.vx = viewDecodeValue(p->Vx, 5.0, 1, 10)/1000.0;
		viewRlsTarget.vy = 0;		
		viewRlsTarget.vz = viewDecodeValue(p->Vh, 5.0, 1, 10)/1000.0;
		viewRlsTarget.course      = 0;
		viewRlsTarget.vr          = sqrtf ( powf(viewRlsTarget.vx,2) + powf(viewRlsTarget.vy,2) +
		            							powf(viewRlsTarget.vz,2) );
		viewRlsTarget.x = viewRlsTarget.distance
									*cosf(viewRlsTarget.peleng)
									*cosf(viewRlsTarget.um);
		viewRlsTarget.y = viewRlsTarget.distance
									*sinf(viewRlsTarget.peleng)
									*cosf(viewRlsTarget.um);
		viewRlsTarget.z = viewRlsTarget.distance*sinf(viewRlsTarget.um);
			
			
		viewRlsTarget.skoX  = 0;
		viewRlsTarget.skoY  = 0;
		viewRlsTarget.skoZ  = 0;
		viewRlsTarget.skoVx = 0;
		viewRlsTarget.skoVy = 0;
		//viewRlsTarget.skoVz = 0;
	}else{
		viewRlsTarget.x 		= (float)(signed short)p->X*5.0;
		viewRlsTarget.y 		= (float)(signed short)p->Y*5.0;
		viewRlsTarget.z 		= (float)p->H;
		viewRlsTarget.vx 		= viewDecodeValue(p->Vx, 5.0, 1, 10);
		viewRlsTarget.vy 		= viewDecodeValue(p->Vy, 5.0, 1, 10);
		viewRlsTarget.vz 		= viewDecodeValue(p->Vh, 5.0, 1, 10);
		viewRlsTarget.skoX	= (float)p->skoX;
		viewRlsTarget.skoY	= (float)p->skoY;
		viewRlsTarget.skoZ	= (float)p->skoH;
		viewRlsTarget.skoVx 	= (float)p->skoVx*5.0;
		viewRlsTarget.skoVy 	= (float)p->skoVy*5.0;
		//viewRlsTarget.skoVz 	= (float)p->skoVh*5.0;
		viewRlsTarget.distance    = sqrtf ( powf(viewRlsTarget.x,2) + powf(viewRlsTarget.y,2) +
		                                               powf(viewRlsTarget.z,2) );
		viewRlsTarget.distanceHor = hypotf (viewRlsTarget.x, viewRlsTarget.y);
		viewRlsTarget.um         	= viewDopi(atan2f(viewRlsTarget.z, viewRlsTarget.distanceHor));
		viewRlsTarget.peleng     	= viewDo2pi(atan2f(viewRlsTarget.y, viewRlsTarget.x));
		viewRlsTarget.course      = viewDo2pi(atan2f(viewRlsTarget.vy, viewRlsTarget.vx));
		viewRlsTarget.vr          = sqrtf ( powf(viewRlsTarget.vx,2) + powf(viewRlsTarget.vy,2) +
		            							powf(viewRlsTarget.vz,2) );

	}
	
	sprintf(tmpStr,"П=%0.1f Д=%0.2f В=%0.0f УМ=%0.1f ",
						viewRlsTarget.peleng/M_PI*180.0,
						viewRlsTarget.distance/1000,
						viewRlsTarget.z,
						viewRlsTarget.um/M_PI*180.0);
	strcat(str, tmpStr);

	if (p->bKT)
		strcat(str, " КТ=1 ");
	else
		strcat(str, " КТ=0 ");

	sprintf(tmpStr, "ПНС=%d ", p->PNS);
	strcat(str, tmpStr);

	sprintf(tmpStr,"CБР=%d ", p->SBR);
	strcat(str, tmpStr);

	if (p->PRC)
		strcat(str, " ПЦ=1 ");
	else
		strcat(str, " ПЦ=0 ");
	
	// Тип цели
	switch(p->TC){
			case 1:
				strcat(str, "Т=Н ");
				break;
			case 8:
				strcat(str, "Т=ВН ");
				break;
			case 9:
				strcat(str, "Т=Л ");
				break;
			case 10:
				strcat(str, "Т=С ");
				break;
			case 11:
				strcat(str, "Т=ПКР ");
				break;
			case 12:
				strcat(str, "Т=ПРР ");
				break;
			case 13:
				strcat(str, "Т=В ");
				break;
			case 15:
				strcat(str, "Т=ВПЦ ");
				break;
			default:
				strcat(str, "Т=Неопр ");
				break;
			}
			//ГП
			switch(p->SV){
			case 0:       strcat(str, "ГП=Н ");
				break;
			case 1: strcat(str, "ГП=Ч ");
				break;
			case 2: strcat(str, "ГП=С ");
				break;
			}

			if(p->nType==16){
				if (p->skoH == 0.0)
				{
					sprintf(tmpStr, "РедХ");
				}
				else
				{
					sprintf(tmpStr, "Ред=%d", (int)p->skoVh);
				}
				strcat(str, tmpStr);
			}
}


void data2viewIP3_Puma(DocIP3_Puma* p, char *str){
	float speed1;
	ViewCountTP viewCountTP;
	char tmpStr[800];
	
	strcpy(str, " ");
	memset(&viewCountTP,0,sizeof(ViewCountTP));
		
	if ((p->nPuma !=0)|(p->nRls !=0))
		if(p->skk==0){
			viewCountTP.x = p->x/1000.0;
			viewCountTP.y = p->y/1000.0;
			viewCountTP.z = p->h;
			viewCountTP.dist = sqrtf(p->x/1000.0*p->x/1000.0+
								p->y/1000.0*p->y/1000.0+
								p->h/1000.0*p->h/1000.0);
			viewCountTP.distHor = hypot(p->x, p->y);
			viewCountTP.pel = viewDo2pi(atan2f(p->y,p->x))/M_PI*180.0;
			viewCountTP.um=viewDopi(atan2f(p->h, viewCountTP.distHor));
			viewCountTP.distHor = viewCountTP.distHor/1000;
			viewCountTP.course = viewDo2pi(atan2f(p->vy, p->vx));
			speed1 = p->vx*p->vx+p->vy*p->vy+p->bVh*p->bVh;
			viewCountTP.speed = sqrtf(speed1);
		}else{
			viewCountTP.x = p->x;
			viewCountTP.y = p->y;
			viewCountTP.dist = 150.0;
			viewCountTP.pel  = viewDo2pi(p->x)/M_PI*180.0;		
			viewCountTP.um  = viewDopi(p->y)/M_PI*180.0;
			viewCountTP.z = viewCountTP.dist*sin(viewCountTP.um);
			viewCountTP.course = viewDo2pi(atan2f(p->vy, p->vx));
			speed1 = p->vx*p->vx+p->vy*p->vy+p->bVh*p->bVh;
			viewCountTP.speed = sqrtf(speed1);
		}

	sprintf(tmpStr, "нРЛС=%d нПума=%d П=%0.1f Д=%0.2f В=%0.0f УМ=%0.1f ",
			p->nRls,
			p->nPuma,
			viewCountTP.pel,
			viewCountTP.dist,
			viewCountTP.z,
			viewCountTP.um);
	
	strcat(str, tmpStr);

	sprintf(tmpStr, "rrc=%d ", p->rrc);
	strcat(str, tmpStr);
	
	switch(p->objType){
	case 1:
		strcat(str, " Тип=ВЦ ");
		break;
	case 3:
		strcat(str, " Тип=МЦ(НЦ) ");
		break;
	case 4:
		strcat(str, " Тип=МЦ(НЦ) ");
		break;
	case 11:
		strcat(str, " Тип=В ");
		break;
	case 12:
		strcat(str, " Тип=C ");
		break;
	case 13:
		strcat(str, " Тип=ПРР ");
		break;
	case 14:
		strcat(str, " Тип=ПКР ");
		break;
	case 15:
		strcat(str, " Тип=ВПЦ ");
		break;
	default:
		strcat(str, " Тип=Неопр ");
		break;
	}
}

void data2viewOurShipGeo(DocOurShipGeo* p, char* str){
	char tmpStr[800];
	float fi, lambda;

	strcpy(str, " ");
	sprintf(tmpStr,"К = %d° ", (int)rint(p->course*180.0/M_PI));
	strcat(str, tmpStr);
	
	fi = viewDo180(p->fi);
	sprintf( tmpStr,"    Ш=%d°%d'%d",(int) floor(fabs(fi)),
	                      (int) fmod(fabs(fi)*60,60.0),
	                      (int) fmod(fabs(fi)*60*60,60.0));
	strcat(str, tmpStr);
	if(fi > 0) strcat(str," CШ"); else strcat(str," ЮШ");
	
	lambda = viewDo180(p->lambda);
	sprintf( tmpStr,"     Д=%d°%d'%d",(int)floor(fabs(lambda)),
		 	                (int) fmod(fabs(lambda)*60,60.0),
		 	                (int) fmod(fabs(lambda)*60*60,60.0));
	strcat(str, tmpStr);
	if 	(lambda > 0) strcat(str," ВД"); else strcat(str," ЗД");
}




void data2viewDoc67Zapros(int type, Doc67Zapros* p, char *str){
	char tmpStr[100];
	strcpy(str,"");
	
	if(p->numberRlk!=0){
		sprintf(tmpStr," нРЛК=%d ", p->numberRlk);
		strcat(str, tmpStr);
	}
//	if(p->number5P20!=0){
//		sprintf(tmpStr, " н5П20=%d", p->number5P20);
//		strcat(str, tmpStr);//
//	}

	if(p->numberX!=0){
		sprintf(tmpStr, " нX=%d", p->numberX);
		strcat(str, tmpStr);
	}
	if(p->numberSfera!=0){
		sprintf(tmpStr, " нСфера=%d", p->numberSfera);
		strcat(str, tmpStr);
	}
	
	if(p->sourceGo.b5P27){
		strcat(str, " И=5П27 ");
	}else if(p->sourceGo.b3K96){
		strcat(str, " И=3К96 ");
	}else if(p->sourceGo.bSigma){
		strcat(str, " И=Сигма ");
	}else if(p->sourceGo.b5P20){
		strcat(str, " И=5П20 ");
	}else if(p->sourceGo.bMTK){
		strcat(str, " И=МТК ");
	}
	getStrMsg(type, p->parameter, tmpStr);
	strcat(str, tmpStr);
}

void data2viewDev67tgt(DocDev67Tgt* p, char *str){
	char tmpStr[1000]="";
	
	strcpy(str, " ");
	
/*	sprintf(tmpStr, "B=%d, PN=%d, d=%f, l=%d, m=%d, n=%d, nR=%d, p=%f", 
	p->B,
	p->PN,
	p->dist,
	p->line,
	p->mode,
	p->num,
	p->numberRLK,
	p->pel);
	strcat(str, tmpStr);
	
	return ;
	*/
	sprintf(tmpStr, "нРЛК=%d ", p->numberRLK);
	strcat(str, tmpStr);
	sprintf(tmpStr, "П=%0.1f ", p->pel);
	strcat(str, tmpStr);
	sprintf(tmpStr, "Д=%0.1f ", p->dist);
	strcat(str, tmpStr);
	sprintf(tmpStr, "Линия=%d ", p->line);
	strcat(str, tmpStr);
	
	switch(p->mode){
	case 1: strcat(str, "Корабль ");
		break;
	case 2: strcat(str, "Самолет ");
		break;
	default: 
		break;
	}
	if(p->num!=0){
		sprintf(tmpStr, "нц=%d ", p->num);
		strcat(str, tmpStr);
	}
	
	if(p->B)
		strcat(str, "Б ");
}


void data2viewTime(DocTime* p, char* str){
	unsigned int timesigma;
	char tmpStr[800];
	struct tm* restime;
	time_t gg;
	strcpy(str, "");
	timesigma = p->timeSigma;
	restime = gmtime(& (timesigma) );
	gg = mktime(restime);
	restime = gmtime( &gg );
	strftime( tmpStr, 100, "Сигмы: %d %b %Y  %H:%M:%S",restime); 
	strcat(str, tmpStr);
	sprintf(tmpStr, " время 3К96(%d) ", p->time3K96);
	strcat(str, tmpStr);
}


void data2viewOPERATOR_DIRECTIVE(DocOPERATOR_DIRECTIVE_STRUCT2* p, char *str){
	char tmpStr[800];
	int code;
	DocOPERATOR_DIRECTIVE_STRUCT3 * p3;
	
	strcpy(str, " ");
	code = p->NTC+p->PC+p->YST+p->NZZ+p->OZZ+p->ONC+p->VA1; // дочитываем признак команды	
	if (code==7){ //расшифровка согласно DocOPERATOR_DIRECTIVE_STRUCT3
		p3 = (DocOPERATOR_DIRECTIVE_STRUCT3 *) p;
		if (p3->PRM) 
		{
			sprintf(tmpStr,"МИР=%d ",p3->MIR);
			strcat(str, tmpStr);
		}
		if (p3->PRNC) 
		{
			sprintf(tmpStr,"ПорогНЦ=%d° ",p3->NC);
			strcat(str, tmpStr);
		}  
		if (p3->PRH) 
		{
			sprintf(tmpStr,"Цели Нц=%d назначена Н=%dм° ",p3->NT3, p3->H);
			strcat(str, tmpStr);
		}  
		return;
	} // (code==7)
	code += p->OPC+p->SBR+p->TRC; // дочитываем признак команды
	if (code !=1){
		strcpy(str, " некорректная команда");		//д.б.только один признак задан или все
		return;
	}	
	if (p->NTC) 
	{
		sprintf(tmpStr,"Цели Нц=%d назначен Тип=%d",p->NT3,p->NTC);
		strcat(str, tmpStr);
	}
	if (p->PC) 
	{
		sprintf(tmpStr,"Цели Нц=%d назначен Приоритет",p->NT3);
		strcat(str, tmpStr);
	}
	if (p->OPC) 
	{
		sprintf(tmpStr,"Цели Нц=%d Отменен приоритет",p->NT3);
		strcat(str, tmpStr);
	}
	if (p->YST) 
	{
		sprintf(tmpStr,"Юстировочная поправка П=%dмин Д=%dм УМ=%dмин",p->B1,p->R1,p->Eps);
		strcat(str, tmpStr);
	}
	if (p->NZZ) 
	{
		sprintf(tmpStr,"Назначена ЗАЗ №%d П=%0.0f°..%0.0f°  Д=%0.3fкм.. %0.3fкм",
				        p->NT3,p->B1*180/pow(2,15),p->B2*180/pow(2,15), p->R1*2.5/1000, p->R2*2.5/1000);
		strcat(str, tmpStr);
	}
	if (p->OZZ) 
	{
		sprintf(tmpStr,"Отменена ЗАЗ №%d",p->NT3);
		strcat(str, tmpStr);
	}
	if (p->ONC) 
	{
		sprintf(tmpStr,"ПА по надводной цели П=%0.0f° Д=%0.2fкм УМ=%0.1f°",
									    p->B1*180/pow(2,15),p->R1*2.5/1000,p->Eps*180/pow(2,16)); 
		strcat(str, tmpStr);
	}
	if (p->VA1) 
	{
		if (p->PA== 1) sprintf(tmpStr,"Первый ввод П=%0.0f° Д=%0.2fкм УМ=%0.1f°",
				                        p->B1*180/pow(2,15),p->R1*2.5/1000,p->Eps*180/pow(2,16)); else
			if (p->PA == 2) sprintf(tmpStr,"Корректура Нц=%d П=%0.0f° Д=%0.2f УМ=%0.1f°",
			                     p->NT3,p->B1*180/pow(2,15),p->R1*2.5/1000,p->Eps*180/pow(2,16)); else
				if (p->PA == 3) sprintf(tmpStr,"Изменение типа сопровождения Нц=%d",p->NT3); 
				else sprintf(tmpStr," ");
		strcat(str, tmpStr);		
	}
	if (p->SBR) 
	{
		sprintf(tmpStr,"Сброс с сопровождения Нц=%d",p->NT3);
		strcat(str, tmpStr);
	}
	if (p->TRC) 
	{
		sprintf(tmpStr,"Назначен РТЦ по цели Нц=%d",p->NT3);
		strcat(str, tmpStr);
	}

	
	
}

void data2viewANSWER_BOS_OPERATOR(DocANSWER_BOS_OPERATOR_STRUCT3* p, char *str){
	char tmpStr[800];
	int code;
	DocANSWER_BOS_OPERATOR_STRUCT2 * p2;
	
	strcpy(str, " ");
	if (p->typeMes==127) //расшифровка согласно DocOPERATOR_DIRECTIVE_STRUCT3
	{	
		if (p->errorCode >0) 
		{ 	sprintf(tmpStr,"ошибка в директиве оператора=%d",p->errorCode);
			return;
		}
		if (p->PRM) 
		{
			if (p->MIR==1) sprintf(tmpStr,"Назначен МИР"); else sprintf(tmpStr,"Отменен МИР");  
			strcat(str, tmpStr);
		}
		if (p->PRNC) 
		{
			sprintf(tmpStr,"установлен порог по НЦ=%d° ",p->NC);
			strcat(str, tmpStr);
		}  
		if (p->PRH) 
		{
			sprintf(tmpStr,"Цели Нц=%d назначена Н=%dм° ",p->NT3, p->H);
			strcat(str, tmpStr);
		}  
		return;
	} // (typeMes == 127
	p2 = (DocANSWER_BOS_OPERATOR_STRUCT2 *) p;
	if (p2->error >0) 
	{ 	sprintf(tmpStr,"ошибка в директиве оператора");
		strcat(str, tmpStr);
		return;
	}
	if (p2->refuseVA1 >0) 
	{ 	sprintf(tmpStr,"отказ от корректуры оператора");
		strcat(str, tmpStr);	
		return;
	}
	
	code = p2->PC + p2->YST + p2->NZZ+ + p2->OZZ + p2->OPC; // считываем признак команды
	if (code !=1)
	{
		strcpy(str, "некорректная команда");		//д.б.только один признак задан
		return;
	}	
	if (p2->PC) 
	{
		sprintf(tmpStr,"Цели Нц=%d назначен Приоритет",p2->NT3);
		strcat(str, tmpStr);
	}
	if (p2->OPC) 
	{
		sprintf(tmpStr,"Цели Нц=%d Отменен приоритет",p2->NT3);
		strcat(str, tmpStr);
	}
	if (p2->YST) 
	{
		sprintf(tmpStr,"Юстировочная поправка П=%dмин Д=%dм УМ=%dмин",p2->B1,p2->R1,p2->B2);
		strcat(str, tmpStr);
	}
	if (p2->NZZ) 
	{
		sprintf(tmpStr,"Назначена ЗАЗ №%d П=%0.0f°..%0.0f°  Д=%0.3fкм.. %%0.3fкм",
				        p2->NT3,p2->B1*180/pow(2,15),p2->B2*180/pow(2,15), p2->R1*2.5/1000, p2->R2*2.5/1000);
		strcat(str, tmpStr);
	}
	if (p2->OZZ) 
	{
		sprintf(tmpStr,"Отменена ЗАЗ №%d",p->NT3);
		strcat(str, tmpStr);
	}
}

void data2viewADAPTATION_CONTROL(DocCODOGRAMM_ADAPTATION_CONTROL_STRUCT* p, char *str){
	char tmpStr[800];
	int code;
	if (p->bBR) {sprintf(tmpStr, "БР "); strcat(str, tmpStr);}
	if (p->bDR) {sprintf(tmpStr, "ДР "); strcat(str, tmpStr);}
	if (p->bRR) {sprintf(tmpStr, "РР "); strcat(str, tmpStr);}
	if (p->bISH) {sprintf(tmpStr, "ИСХ "); strcat(str, tmpStr);}
	if (p->bVRSH) {code = 60*p->VR60 + 30*p->VR30 + 6*p->VR6 + 3*p->VR3; sprintf(tmpStr, "V=%d ",code); strcat(str, tmpStr);}
	if (p->bRO) { sprintf(tmpStr, "Режим=%d ",p->NRO); strcat(str, tmpStr);}
	if (p->bLCHM) {if (p->LCHM) sprintf(tmpStr, "ЛЧМ полож"); else sprintf(tmpStr, "ЛЧМ полож"); strcat(str, tmpStr);}
	if (p->bRVF)	{ 	sprintf(tmpStr,"Назначена частота F%d",p->NCHRV);}
	if (p->bRCH)	{ 	sprintf(tmpStr,"Заданы разрешенные частоты F=%d",p->f);	}
	if (p->bACBF) { 	sprintf(tmpStr,"Назначен ACBF");}
	if (p->bADBF) { 	sprintf(tmpStr,"Назначен ADBF");}
	if (p->bDkgk) { 	sprintf(tmpStr,"Назначена по кругу Дкгк = %dм",  p->Dkgk * 10);}
	if (p->bDvaru) { 	sprintf(tmpStr,"Назначена по кругу Двару = %dм",  p->Dvaru * 10);	}
	if (p->bMDFA) 	{ 	sprintf(tmpStr,"Назначен МДФА%d",  p->MDFA+1); }// 0=МДФА1 1=МДФА2	

} //data2viewADAPTATION_CONTROL
	


void data2viewANSWER_ADAPTATION_CONTROL(DocANSWER_ADAPTATION_CONTROL_STRUCT* p, char *str){
	char tmpStr[800];
	int code;
	if (p->bBR) 	{ sprintf(tmpStr, "БР "); strcat(str, tmpStr);}
	if (p->bDR) 	{ sprintf(tmpStr, "ДР "); strcat(str, tmpStr);}
	if (p->bRR) 	{ sprintf(tmpStr, "РР "); strcat(str, tmpStr);}
	if (p->bISCH) 	{ sprintf(tmpStr, "ИСХ "); strcat(str, tmpStr);}
	if (p->bVRSH) 	{ code = 60*p->VR60 + 30*p->VR30 + 6*p->VR6 + 3*p->VR3; sprintf(tmpStr, "V=%d ",code); strcat(str, tmpStr);}
	if (p->bRO) 	{ sprintf(tmpStr, "Режим=%d ",p->NRO); strcat(str, tmpStr);}
	if (p->bLCHM) 	{if (p->LCHM) sprintf(tmpStr, "ЛЧМ полож"); else sprintf(tmpStr, "ЛЧМ полож"); strcat(str, tmpStr);}
	if (p->bRVF)	{ sprintf(tmpStr,"Назначена частота F%d",p->NCHRV);}
	if (p->bRCH)	{ sprintf(tmpStr,"Заданы разрешенные частоты F=%ld",p->f);	}
	if (p->bACBF) 	{ sprintf(tmpStr,"Назначен ACBF");}
	if (p->bADBF) 	{ sprintf(tmpStr,"Назначен ADBF");}
	if (p->bDkgk) 	{ sprintf(tmpStr,"Назначена по кругу Дкгк = %dм",  p->Dkgk* 10);}
	if (p->bDvaru) 	{ sprintf(tmpStr,"Назначена по кругу Двару = %dм",  p->Dvaru * 10);	}
	if (p->bMDFA) 	{ sprintf(tmpStr,"Назначен МДФА%d",  p->MDFA+1); }// 0=МДФА1 1=МДФА2	
	
} //ANSWER_ADAPTATION_CONTROL



void data2viewSTATE_BOS(DocCODOGRAMM_GENERAL_MODES_STRUCT* p, char* str){
	char tmpStr[1000]="";
	
	strcpy(str, " ");
	sprintf(tmpStr, "нРЛК=%d ", 5);
	strcat(str, tmpStr);
}




void data2viewMainDoc(DocMainDoc* p, char* str){
	char tmpStr[1000]="";
	strcpy(str, " ");
	if (p->bCombatI ==1) sprintf(tmpStr, "Боевой_И "); 
	 else sprintf(tmpStr, "Боевой ");
	strcat(str, tmpStr);
	if (p->stateAutonom ==0) sprintf(tmpStr, "ПВО_ЦР "); 
	 else if (p->stateAutonom ==1) sprintf(tmpStr, "ПВО_РР ");
	 	else sprintf(tmpStr, "ПВО_АР ");
	strcat(str, tmpStr);
	if (p->bAutoCr ==0) sprintf(tmpStr, "ЦР_руч "); 
	 else sprintf(tmpStr, "ЦР_авт ");
	strcat(str, tmpStr);
	sprintf(tmpStr, "СК=%d ",p->vAntenna);
	strcat(str, tmpStr);	
	if (p->bHigh ==1) strcat(str, "Изл=1 ");
	 else strcat(str, "Изл=0 ");
	switch (p->RO) {
				case 1:  sprintf(tmpStr, "РО-1" ); break;
				case 3:  sprintf(tmpStr, "РО-дпла "); break;
				case 5:  sprintf(tmpStr, "РО-2 " ); break;
				case 10: sprintf(tmpStr, "РО-32 ");break;
				case 11: sprintf(tmpStr, "РО-31 "); break;
				case 14: sprintf(tmpStr, "РО-КИ "); break;
				case 15: sprintf(tmpStr, "РО-4 "); break;
				default: sprintf(tmpStr, "РО-0 "); break;
			}
	strcat(str, tmpStr);
	if (p->bHigh ==1) strcat(str, "Изл=1 ");
	 else strcat(str, "Изл=0 ");
	
	if (p->b67 ==1) strcat(str, "67=1 "); 
	 else strcat(str, "67=0 ");
	if (p->bAutoGo ==1) strcat(str, "ГО=АВТ "); 
	 else strcat(str, "ГО=ПА ");
	sprintf(tmpStr, "БОСупр=%d ", p->bBosUprConnected);
	strcat(str, tmpStr);
	sprintf(tmpStr, "БОСнав=%d ", p->bBosNavConnected);
	strcat(str, tmpStr);
	sprintf(tmpStr, "Пав=%d ", p->bPavConnected);
	strcat(str, tmpStr);
}

void data2viewPAV_OK(DocPAV_OK_STRUCT* p, char* str){
	char tmpStr[1000]=""; 
	int speed;
	strcpy(str, " ");
	speed = 60*(p->bRotarySpeed_60==1)+30*(p->bRotarySpeed_30==1)+
		     6*(p->bRotarySpeed_6==1)+3*(p->bRotarySpeed_3==1); 
	sprintf(tmpStr, "СК=%d ", speed);
	strcat(str, tmpStr);
	if (p->bAvar ==1) strcat(str, "Авар ");
	if (p->bPVK ==1) strcat(str, "ПВК ");
	if (p->bIK ==1) strcat(str, "ИК ");
	if (p->bReady ==1) strcat(str, "Готов ");
	if (p->bErrOper ==1) strcat(str, "ОшОп ");
	if (p->bHand ==1) strcat(str, "Ручн ");
	if (p->bBlVrash ==1) strcat(str, "БлВращ ");
	if (p->bOtkCp ==1) strcat(str, "ОткЦП ");
	if (p->bSKO ==1) strcat(str, "СбойКО ");
	if (p->bPBU ==1) strcat(str, "БлУпр ");
	if (p->bOtkBuPav ==1) strcat(str, "ОткБУ ");
	if (p->bOtkMehPav ==1) strcat(str, "ОткМех ");
	if (p->bVklGrupB ==1) strcat(str, "ВклГрупВ ");
	if (p->bOtk13 ==1) strcat(str, "ОткВ1..3 ");
	if (p->bVklVum ==1) strcat(str, "Вкл27ВУМ ");
	if (p->bOtk47 ==1) strcat(str, "ОткВ1..3 ");
	if (p->bOtkVent ==1) strcat(str, "ОткВентКонт ");
	if (p->bVklPav ==1) strcat(str, "Вкл27ВипПАВ ");
	if (p->bVklBos ==1) strcat(str, "ВклБОС ");
	if (p->bVklVip ==1) strcat(str, "ВклВИП ");
	if (p->bOhlVum1 ==1) strcat(str, "ОхлВУМ1 ");
	if (p->bOhlVum2 ==1) strcat(str, "ОхлВУМ2 ");
}


void data2viewCommandX_Strob(DocCommandX_Strob* p, char* str)
{
	strcpy(str, " ");
	sprintf(str, "Нз=%d П=%0.1f° СтрП=%0.1f° Д=%0.2fкм СтрД=%0.2fкм УМ=%0.1f° СтрУМ=%0.1f°",
			 p->iRequest, 
			(float)(p->peleng*180.0/pow(2,15)),
			(float)(p->strobPel*180.0/pow(2,15)),
			(float)(p->distance/100.0),
			(float)(p->strobDist/100.0),
			(float)(p->um*180.0/pow(2,15)),
			(float)(p->strobUm*180.0/pow(2,15)));
}	

void data2viewCommandX_ChangePri(DocCommandX_ChangePri* p, char* str)
{
	char tmpStr[20]=""; 
	sprintf(str," Hх=%d Нз=%d",p->iRequest,p->nX);
	if (p->PR ==0) strcpy(tmpStr,"команда: отмена ЦУ"); else
		if (p->PR ==2) strcpy(tmpStr,"команда: АС"); else
			if (p->PR ==3) strcpy(tmpStr,"команда: ПС");
	strcat(str, tmpStr);	
}
void data2viewResult_CommandX(DocResult_CommandX* p, char* str)
{
	char tmpStr[100]="";
	int i;
	if (p->iRequest ==0)
	{
		sprintf(str, "Х_цель Нх=%d сброс цели", p->nX[0]);
		return;
	}
	sprintf(str, "Нз=%d", p->iRequest);
	if (p->RO ==0)
	{
		if (p->error==1) strcpy(tmpStr,"запрос отклонен: нет связи СУ с РЛС-Х"); else
			if (p->error==2) strcpy(tmpStr,"запрос отклонен: неипр РЛС-Х"); else
				if (p->error==3) strcpy(tmpStr,"запрос отклонен: отсутствие тех.возможности"); else
					if (p->error==3) strcpy(tmpStr,"запрос отклонен: нарушение приоритета, невозможность исполнения"); else
						strcpy(tmpStr,"запрос отклонен");
		strcat(str, tmpStr);
	}
	if (p->RO ==1)
	{
		sprintf(tmpStr,"захвачено %d целей: ",p->numberTargets);
		strcat(str, tmpStr);
		for (i = 0; i < p->numberTargets; ++i) 
		{
			sprintf(tmpStr," %d ",p->nX[i]);
			strcat(str, tmpStr);
		}
		strcat(str, tmpStr);
	}
	if (p->RO ==2)
	{
		sprintf(str,"Цель Нх=%d сброшена с сопровождения в РЛС-Х", p->nX[0]);
	}
	if (p->RO ==3)
	{
		sprintf(str,"Цель Нх=%d отсутствует в РЛС-Х", p->nX[0]);
	}
}

void data2viewEMS_SU(DocEMS_SU* p, char* str)
{
	char tmpStr[100]="";
	int i;
	
	strcpy(str,"РазрЧастоты=");
    for (i = 0; i < 16; ++i)
    {
        if ( ((unsigned int)((p->RTrr1 >> i) &0x1) == 1))
        	strcpy(tmpStr,"1"); else strcpy(tmpStr,"0");
    }    
    strcat(str,tmpStr);
    for (i = 16; i < 32; ++i)
    {
        if ( ((unsigned int)((p->RTrr2 >> i-16) &0x1) == 1))
        	strcpy(tmpStr,"1"); else strcpy(tmpStr,"0");
    }    
    strcat(str,tmpStr);
	strcat(str,"РекЧастоты=");
    for (i = 0; i < 16; ++i)
    {
        if ( ((unsigned int)((p->RTrk1 >> i) &0x1) == 1))
        	strcpy(tmpStr,"1"); else strcpy(tmpStr,"0");
    }    
    strcat(str,tmpStr);
    
    for (i = 16; i < 32; ++i)
    {
        if ( ((unsigned int)((p->RTrk2 >> i-16) &0x1) == 1))
        	strcpy(tmpStr,"1"); else strcpy(tmpStr,"0");
    }    
    strcat(str,tmpStr);
    if ((p->NSzi+p->OSzi!=0))
    {
    	sprintf(tmpStr," Сектор: %0.1f°..%0.1f°",p->NSzi*0.1,p->OSzi*0.1 );
    	strcat(str,tmpStr);
    }
    if ((p->HiOff==1))
    {
    	sprintf(tmpStr," Отключить излучение!");
    	strcat(str,tmpStr);
    }
}

void data2viewEMS_FURKE(DocEMS_FURKE* p, char* str)
{
	char tmpStr[100]="";
	int i;
	
	strcpy(str,"Команда подтверждения");
}


void data2viewTargetX_m(DocTargetX_m* p, char* str,unsigned char bHEth)
{
	DocTargetX_f targetX_f;
	char tmpStr[800];
	float course,speed, h,workX,workY;
	double deltah;
	convRead_DocTargetX(&targetX_f,p);
	sprintf(str, " Нх=%d",targetX_f.nn);
	if (targetX_f.bnonImit==0) strcat(str,"И"); 
	if (targetX_f.pri==0) strcat(str,"Пр"); 
	course = atan2f(targetX_f.Vy,targetX_f.Vx)*180.0/M_PI;
	course = viewDo360(course);
	speed = hypotf(targetX_f.Vy,targetX_f.Vx);
	speed = hypotf(speed,targetX_f.Vz);
	workX = targetX_f.distance *cosf(targetX_f.peleng) *cosf (targetX_f.um);
	workY = targetX_f.distance *sinf(targetX_f.peleng) *cosf (targetX_f.um);
	h= targetX_f.distance * sinf(targetX_f.um);
	sprintf(tmpStr, " П=%0.1f° Д=%0.2fкм X=%0.2fкм Y=%0.2fкм УМ=%0.2f° H=%0.0f",
			targetX_f.peleng*180.0/M_PI,
			targetX_f.distance/1000,
			workX/1000,
			workY/1000,
			targetX_f.um*180.0/M_PI,h);
	strcat(str,tmpStr);

	deltah = (targetX_f.distance*targetX_f.distance)/ (2*6370000.0);
	h = h+ (float)deltah;
	sprintf(tmpStr, " Hг=%0.0f",h);
	strcat(str,tmpStr);

	sprintf(tmpStr, " К=%0.0f° V=%0.0fм/с Vxyz=%0.0f %0.0f %0.0f СКО: ПДУVxVyVz=%0.2f° %0.0fм %0.2f° %0.0f %0.0f %0.0f",
			course,speed,
			targetX_f.Vx,targetX_f.Vy,targetX_f.Vz,
			targetX_f.skoPeleng*180.0/M_PI,
			targetX_f.skoDistance,
			targetX_f.skoUm*180.0/M_PI,
			targetX_f.skoVx,
			targetX_f.skoVy,
			targetX_f.skoVz);
	strcat(str,tmpStr);
	
	
	if (targetX_f.GO ==0) strcpy(tmpStr," ГО:Н "); else
        if (targetX_f.GO ==1) strcpy(tmpStr," ГО:Ч "); else
            if (targetX_f.GO ==2) strcpy(tmpStr," ГО:С "); else  sprintf(tmpStr," ГО:%d ",targetX_f.GO);  
	strcat(str,tmpStr);
	switch(targetX_f.typeTarget){
			case 1:
				strcat(str, "Т=НЦ ");
				break;
			case 2:
				strcat(str, "Т=НЦ ");
				break;
			case 8:
				strcat(str, "Т=ВЦ ");
				break;
			case 9:
				strcat(str, "Т=Л ");
				break;
			case 10:
				strcat(str, "Т=Сам ");
				break;
			case 11:
				strcat(str, "Т=ПКР ");
				break;						
			case 12:
				strcat(str, "Т=ПРР ");
				break;
			case 13:
				strcat(str, "Т=Верт ");
				break;				
			case 15:
				strcat(str, "Т=ВПЦ ");
				break;				
			default:
				strcat(str, "Т=неопр");
				break;
			}
}

void data2viewTargetTelevik(DocTargetTelevik* p, int bParamPD, int  bParamVr, char* str)
{
	char tmpStr[800];
	
	float course,speed, peleng, distHor, distance, um;
	sprintf(str, " Нт=%d",p->nSource);
	if (p->bPriority==1) strcat(str,"Пр"); 
	speed = hypotf(p->vy,p->vx);
	speed = hypotf(p->vh,speed);
	distHor = hypotf(p->y,p->x);
	distance = hypotf(p->h,distHor);
	course = viewDo180((atan2f(p->vy,p->vx))*180.0/M_PI);
	um = viewDo180((atan2f(p->h,distHor))*180.0/M_PI);
	peleng = viewDo360((atan2f(p->y,p->x))*180.0/M_PI);

	sprintf(tmpStr, " t=%d",	p->t);
	strcat(str,tmpStr);

	
	sprintf(tmpStr, " П=%0.1f° Д=%0.2fкм  УМ=%0.2f° H=%0.0f",
			peleng,
			distance/1000,
			um,p->h);
	strcat(str,tmpStr);
	if (bParamVr ==1)  sprintf(tmpStr, " К=%0.0f° V=%0.1fм/с",	course,speed);
	else sprintf(tmpStr, " Vx=%0.0f Vy=%0.0f Vh=%0.0f",	p->vx, p->vy, p->vh);
	strcat(str,tmpStr);
	
	//sprintf(tmpStr, " П=%0.1f° Д=%0.2fкм  УМ=%0.2f° H=%0.0f Vx=%0.2f° Vy=%0.2f Vh=%0.2f",
	//		peleng,
	//		distance/1000,
		//	um,p->h,p->vx,p->vy,p->vh);
//	sprintf(tmpStr, " П=%0.1f° Д=%0.2fкм  УМ=%0.2f° H=%0.0f Vx=%0.2f° Vy=%0.2f Vh=%0.2f",
//			peleng,
//			distance/1000,
//			um,p->h,p->vx,p->vy,p->vh);
	switch(p->typeTarget){
			case 1:
				strcat(str, "Т=НЦ");
				break;
			case 8:
				strcat(str, "Т=Вмал");
				break;
			case 10:
				strcat(str, "Т=САМ");
			case 11:
				strcat(str, "Т=ПКР");
				break;
			case 12:
				strcat(str, "Т=ПРР");
				break;
			case 13:
				strcat(str, "Т=Врт");
				break;
			default:
				strcat(str, "Т=Внеопр");
				break;
			}
	if (p->bReset ==1) strcat(str, "Сбр+");
	if (p->bPNS ==1) strcat(str, " ПНС+");
	if (p->bPriority ==1) strcat(str, " Приор+");
	if (p->bNew ==1) strcat(str, " Нов+");
}


void data2viewTargetServal(DocTargetServal* p, int bParamPD, int  bParamVr, char* str)
{
	char tmpStr[800];
	float course,speed, peleng, distance, um;
	sprintf(str, " Нc=%d",p->nServal);
	if (p->nTelevik>0)
	 {sprintf(tmpStr, " Нт=%d",p->nTelevik); strcat(str,tmpStr);}
	if (p->rrc==1)strcat(str, " по_ЦУ"); 
	if (p->rrc==2)strcat(str, " АС");
	if (p->fire==1)strcat(str, " ВО");
	if (p->ppc==1)strcat(str, " поражена");
	if (p->dcu==1)strcat(str, " ЦУ принимается"); else
	if (p->dcu==2)strcat(str, " ЦУ принято"); else
	if (p->dcu==3)strcat(str, " Отказ ЦНЗ"); else
	if (p->dcu==4)strcat(str, " Отказ неде"); else
	if (p->dcu==1)strcat(str, " Отмена ЦУ принята"); else strcat(str, " нет"); 

	if (p->bvx + p->bvy >0)
	{	
		course = atan2f((p->bvy==1)*p->vy,(p->bvy==1)*p->vx);
		course = viewDo2pi(course)*180.0/M_PI;
		speed = hypotf((p->bvy==1)*p->vy,(p->bvy==1)*p->vy);
		speed = hypotf((p->bvh==1)*p->vh,speed);
	} else {course = 0.0;speed = 0.0;}	
	distance = (p->bdistance ==1)*p->distance;
	peleng = (p->bku ==1)*p->ku*180.0/M_PI;
	um = (p->bum ==1)*p->um*180.0/M_PI;
	

	sprintf(tmpStr, " KU=%0.1f° Д=%0.2fкм  УМ=%0.1f°",
			peleng,
			distance/1000,
			um);
	strcat(str,tmpStr);
	if (bParamVr ==1)  sprintf(tmpStr, " К=%0.0f° V=%0.1fм/с",	course,speed);
	else sprintf(tmpStr, " Vx=%0.0f Vy=%0.0f Vh=%0.0f",	p->vx, p->vy, p->vh);
	strcat(str,tmpStr);

	switch(p->typeTarget){
			case 1:
				strcat(str, " Т=НЦ ");
				break;
			case 8:
				strcat(str, " Т=Мал ");
				break;
			case 10:
				strcat(str, " Т=САМ ");
				break;
			case 11:
				strcat(str, " Т=ПКР ");
				break;
			case 12:
				strcat(str, " Т=ПРР ");
				break;
			case 13:
				strcat(str, " Т=Врт ");
				break;
			default:
				strcat(str, " Т=Внеопр");
				break;
			}
}


void data2viewStateServal(DocStateServal* p, char* str)
{
	char tmpStr[800];
	unsigned char bSuitable;
	int i;
	
	bSuitable = (p->stateComplex >0) & (p->error==0);

	if (p->error==1) strcpy(str, " Неиcп,"); else strcpy(str, "");
	if (p->stateComplex==1)strcat(str, " автоном"); else
		if (p->stateComplex==2)strcat(str, " совм"); else strcat(str, " нет работы");
	if (bSuitable ==0) return;
	for (i = 0; i < 2; ++i) 
	{
		bSuitable = (p->modul[i].error ==0) & (p->modul[i].stateComplex >0);
		sprintf(tmpStr, " №%d",i+1); strcat(str,tmpStr);
		if (p->modul[i].bBM ==1) strcat(str,"_БМ:"); else strcat(str,"_О:");
		if (p->modul[i].error==1) strcat(str, " Неиcп,");
		if (p->modul[i].stateComplex==1)strcat(str, " ГС"); else
			if (p->modul[i].stateComplex==2)strcat(str, " нет упр"); else 
				if (p->modul[i].stateComplex==3)strcat(str, " автон"); else 
					if (p->modul[i].stateComplex==4)strcat(str, " ведущий"); else 
						if (p->modul[i].stateComplex==5)strcat(str, " ведомый"); else 
				strcat(str, " нет ГС");
		if (bSuitable ==0) continue;
		//if (p->modul[i].xVynos != 0.0)sprintf(tmpStr, " Хв=%0.0f",p->modul[i].xVynos); strcat(str,tmpStr);
		//if (p->modul[i].yVynos != 0.0)sprintf(tmpStr, " Yв=%0.0f",p->modul[i].yVynos); strcat(str,tmpStr);
		//if (p->modul[i].hVynos != 0.0)sprintf(tmpStr, " Hв=%0.0f",p->modul[i].hVynos); strcat(str,tmpStr);
		sprintf(tmpStr, " УГН=%0.1f°  УВН=%0.1f°",p->modul[i].courseAngle*180.0/M_PI,p->modul[i].um*180.0/M_PI); 
		strcat(str,tmpStr);
		if (p->modul[i].nServal >0) { sprintf(tmpStr, " Нц=%d",p->modul[i].nServal); strcat(str,tmpStr);}
		if (p->modul[i].AC ==1) strcat(str," АС"); 
		if (p->modul[i].bBM ==1)
		{	
			if (p->modul[i].readyAU ==1) strcat(str," АУ"); 
			if (p->modul[i].CS ==0) strcat(str," ЦС"); 
			if (p->modul[i].fire ==1) strcat(str," ВО"); 
		}	
	}
}

void data2viewStateGlobalTelevik(DocStateServal* p, char* str)
{
/*	char tmpStr[800];
	float course,speed, peleng, distance, um;
	sprintf(str, " Нc=%d",p->nServal);
	if (p->nTelevik==1) {sprintf(tmpStr, " Нт=%d",p->nTelevik); strcat(str,tmpStr);}
	if (p->rrc==1)strcat(str, " AC"); else
	if (p->rrc==2)strcat(str, " срыв");
	if (p->fire==1)strcat(str, " ВО");
	if (p->ppc==1)strcat(str, " поражена");
	if (p->dcu==1)strcat(str, " ЦУ принимается"); else
	if (p->dcu==2)strcat(str, " ЦУ принято"); else
	if (p->dcu==3)strcat(str, " Отказ ЦНЗ"); else
	if (p->dcu==4)strcat(str, " Отказ неде"); else
	if (p->dcu==1)strcat(str, " Отмена ЦУ принята");

	if (p->bvx + p->bvy >0)
	{	
		course = atan2f((p->bvy==1)*p->vy,(p->bvy==1)*p->vx);
		course = viewDo2pi(course)*180.0/M_PI;
		speed = hypotf((p->bvy==1)*p->vy,(p->bvy==1)*p->vy);
		speed = hypotf((p->bvh==1)*p->vh,speed);
	} else {course = 0.0;speed = 0.0;}	
	distance = (p->bdistance ==1)*p->distance;
	peleng = (p->bpeleng ==1)*p->peleng*180.0/M_PI;
	um = (p->bum ==1)*p->um*180.0/M_PI;

	sprintf(tmpStr, " П=%0.1f° Д=%0.2fкм  УМ=%0.1f° К=%0.0f° V=%0.1fм/с",
			peleng,
			distance/1000,
			um,course,speed);
	strcat(str,tmpStr);
	switch(p->typeTarget){
			case 1:
				strcat(str, "Т=НЦ ");
				break;
			case 8:
				strcat(str, "Т=Мал ");
				break;
			case 10:
				strcat(str, "Т=САМ ");
			case 11:
				strcat(str, "Т=ПКР ");
				break;
			case 12:
				strcat(str, "Т=ПРР ");
				break;
			case 13:
				strcat(str, "Т=Врт ");
				break;
			default:
				strcat(str, "Т=Внеопр");
				break;
			}
*/			
}


void data2viewTargetPristavka(DocTargetPristavka* p, int bParamPD, int  bParamVr, char* str)
{
	char tmpStr[800];
	float course,speed, peleng, distance, um;
	sprintf(str, " Нc=%d",p->nSource);
	if (p->bNew ==1)  { sprintf(tmpStr, "!"); strcat(str,tmpStr);}
	if (p->bPriority ==1)  { sprintf(tmpStr, "ПРЦ"); strcat(str,tmpStr);}
	sprintf(tmpStr, " ПНС=%d",	p->bPNS); strcat(str,tmpStr);
	sprintf(tmpStr, " СБР=%d",	p->bReset); strcat(str,tmpStr);
	sprintf(tmpStr, " П=%0.1f° Д=%0.2fкм К=%0.1f° С=%0.2fм/c ",
			p->peleng,
			p->distance/1000,
			p->course,
			p->speed);
	strcat(str,tmpStr);
}


void data2viewStatePristavka(DocStatePristavka* p, char* str)
{
	char tmpStr[800];
	unsigned char bSuitable;
	int i;
	
	bSuitable = (p->stateComplex >0);
	if (p->stateComplex==1)strcpy(str, "боевой"); else
		if (p->stateComplex==2)strcpy(str, "дежурный"); else strcpy(str, "нет работы");
	if (bSuitable ==0) return;
	sprintf(tmpStr, " ПП=%d",p->pp); strcat(str,tmpStr);
	sprintf(tmpStr, " ИЗЛ=%d",p->high); strcat(str,tmpStr);
	sprintf(tmpStr, " РО=%d",p->RO); strcat(str,tmpStr);
	sprintf(tmpStr, " П=%0.1f..%0.1f, УМ=%0.1f..%0.1f, Д=%0.1f..%0.1f ",
			p->sector.pelengMin*180.0/M_PI,p->sector.pelengMax*180.0/M_PI,
			p->sector.umMin*180.0/M_PI,p->sector.umMax*180.0/M_PI,
			p->sector.distanceMin*180.0/M_PI,p->sector.distanceMin*180.0/M_PI);
	strcat(str,tmpStr);
}

void data2viewStateGlobalIstra(DocStateGlobalIstra* p, char* str)
{
}


