#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#include "docStructs.h"
#include "docCodeMsg.h"
extern int bParamPD, bParamVr;

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
}DocCountTP;

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
 } DocRlsTarget;
 
 void data2strOurShipGeo(DocOurShipGeo* p, char* str);
 void data2strTime(DocTime* p, char* str);
 void data2strDoc67Zapros(int type, Doc67Zapros* p, char *str);
 void data2strDoc67ZaprosSkol(int type, Doc67ZaprosSkol* p, char *str);
 void data2strDev67tgt(DocDev67Tgt* p, char *str);
 void data2strDeleteZapros();
 void data2strSwitchOff67();
 void data2strNewZapros67();
 void data2strHandGo();
 void data2strZapros67();
 void data2strTaskTarget(DocTaskTarget* p, char *str, int shipType);
 void data2strTaskTargetAll(DocTaskTargetAll* p, char *str, int shipType);
 void data2strNullShootChannel(DocNullShootChannel* p, char *str, int shipType);
 void data2strResetShootChannel(DocResetShootChannel* p, char *str, int shipType);
 void data2strSendRli(DocSendRli* p, char *str);
 void data2strRVVRedut(DocRVVRedut* p, char *str, int shipType);
 void data2strANSWER_BOS_OPERATOR(DocANSWER_BOS_OPERATOR_STRUCT3* p, char *str);
 void data2strOPERATOR_DIRECTIVE(DocOPERATOR_DIRECTIVE_STRUCT2* p, char *str);
 void data2strADAPTATION_CONTROL(DocCODOGRAMM_ADAPTATION_CONTROL_STRUCT* p, char *str);
 void data2strANSWER_ADAPTATION_CONTROL(DocANSWER_ADAPTATION_CONTROL_STRUCT* p, char *str);
 void data2strSTATE_BOS(DocCODOGRAMM_GENERAL_MODES_STRUCT* p, char* str);
 void data2strMainDoc(DocMainDoc* p, char* str);
 void data2strPAV_OK(DocPAV_OK_STRUCT* p, char* str);


 
 
 
 float docDo2pi(float value)
 {
 	float result = value;
 	while (result >= 2*M_PI) result = result-2*M_PI;
     while (result < 0)     result = result+2*M_PI;
     return result;
 }

 float docDopi(float value)           
 {
 	float result = value;
     while (result > M_PI) result = result - 2*M_PI;
     while (result <-M_PI) result = result + 2*M_PI;
 	return result;
 }
 
 float docDo180(float value)    
 {
 	float result = value;
     while (result > 180) result = result - 360;
     while (result <-180) result = result + 360;
 	return result;
 }

 double docDecodeValue(unsigned short wrd , double LOB_worth, int sign, int size){
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
 
 void time2Str(unsigned int _time, char *buff){
	char tmpStr[100];
	unsigned int t;
	//Часы
	t=_time/360000;
	if(t<10)
		strcat(buff, "0");
	sprintf(tmpStr, "%d:", t);
	strcat(buff, tmpStr);
	//Минуты
	t=(_time/6000)%60;
	if(t<10)
		strcat(buff, "0");
	sprintf(tmpStr, "%d:", t);
	strcat(buff, tmpStr);
	//Секунды
	t=(_time/100)%60;
	if(t<10)
		strcat(buff, "0");
	sprintf(tmpStr, "%d.", t);
	strcat(buff, tmpStr);
	//Мс
	t=_time%100;
	if(t<10)
		strcat(buff, "0");
	sprintf(tmpStr, "%d", t);
	strcat(buff, tmpStr);
	strcat(buff, " \t| ");
}
 
void parseDocMsg2Txt(uint32_t time, uint8_t type, void *pMsg, char* buff,int bParamPD,int bParamVr, int shipType){

// void parseDocMsg2Txt(void *p, char* buff, int bParamPD,int bParamVr, int shipType){
	char str[1000];
	char tempBuff[800];
	unsigned int _time;
	unsigned char msgCode;
	strcpy(str,"");
	strcpy(tempBuff,"");


	// Заносим время в строку
	_time = time;
//	memcpy(&_time, p, sizeof(_time));
	time2Str(_time, str);
	
	// Заносим данные пакета
//	memcpy(&msgCode, (byte*)p+4, sizeof(char));
	msgCode = type;
	
	strcat(str, getStrMsg(msgCode, 0, tempBuff));
	strcat(str, " ");
	
	switch (msgCode) {
		case DI_FORMULAR_TRASSA: //strcat(str, "27_Цель");
			data2strFormularTrassa((byte*)pMsg, bParamPD, bParamVr, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_IP3_Puma: //strcat(str, "10_Цель");
			data2strIP3_Puma((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_OURSHIPGEO: //strcat(str, "ШД, К свой");
			data2strOurShipGeo((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_TIME: 
			data2strTime((byte*)pMsg, tempBuff);//strcat(str, "время");
			strcat(str, tempBuff);
			break;
		case DI_Dev67Tgt: 
			data2strDev67tgt((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_DeleteZapros: 
			data2strDoc67Zapros(DI_DeleteZapros,(byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_ZaprosSkol67: 
			data2strDoc67ZaprosSkol(DI_ZaprosSkol67, (byte*)pMsg, tempBuff);
			strcat(str, tempBuff);			
		case DI_SwitchOff67:
			data2strDoc67Zapros(DI_NewZapros67,(byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_NewZapros67: //strcat(str, "ГО: запрос по цели");
			data2strDoc67Zapros(DI_NewZapros67,(byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_HandGO: 
			data2strHandGo((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_Zapros67: //strcat(str, "ГО: произвести в автомате");
			data2strDoc67Zapros(DI_NewZapros67, (byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_TaskTarget: 
			data2strTaskTarget((byte*)pMsg, tempBuff, shipType);
			strcat(str, tempBuff);
			break;
		case DI_TaskTargetAll: 
			data2strTaskTargetAll((byte*)pMsg, tempBuff, shipType);
			strcat(str, tempBuff);
			break;
		case DI_NullShootChannel: 
			data2strNullShootChannel((byte*)pMsg, tempBuff, shipType);
			strcat(str, tempBuff);
			break;
		case DI_ResetShootChannel:
			data2strResetShootChannel((byte*)pMsg, tempBuff, shipType);
			strcat(str, tempBuff);
			break;
		case DI_Result_5P27:
			break;
		case DI_Result_5P20:
			data2strDev67tgt((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);			
			break;
		case DI_SendRli:
			data2strSendRli((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_SendRedut:
			break;
		case DI_RVVRedut:
			data2strRVVRedut((byte*)pMsg, tempBuff, shipType);
			strcat(str, tempBuff);
			break;
			
	/*	case DI_OPERATOR_DIRECTIVE:
			strcpy(buff, getStrMsg(msgCode, 0, str));
			strcat(buff, " ");
			data2strOPERATOR_DIRECTIVE((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_ANSWER_BOS_OPERATOR:
			strcpy(buff, getStrMsg(msgCode, 0, str));
			strcat(buff, " ");
			data2strANSWER_OPERATOR_DIRECTIVE((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_ADAPTATION_CONTROL:
			strcpy(buff, getStrMsg(msgCode, 0, str));
			strcat(buff, " ");
			data2strADAPTATION_CONTROL((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_ANSWER_ADAPTATION_CONTROL:
			strcpy(buff, getStrMsg(msgCode, 0, str));
			strcat(buff, " ");
			data2strANSWER_ADAPTATION_CONTROL((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
*/			
		case DI_STATE_BOS:
			data2strSTATE_BOS((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_MAINDOC:
			data2strMainDoc((byte*)pMsg,tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_PAV_OK:
			data2strPAV_OK((byte*)pMsg,tempBuff);
			strcat(str, tempBuff);
			break;			
		case DI_OURSHIPGEOEX:
			strcat(str, tempBuff);
			break;
		case DI_COMMANDX_STROB_CODE:
			data2viewCommandX_Strob((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_COMMANDX_CHANGEPRI_CODE:
			data2viewCommandX_ChangePri((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_RESULT_COMMANDX_CODE:
			data2viewResult_CommandX((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_EMS_SU:
			data2viewEMS_SU((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_EMS_FURKE:
			data2viewEMS_FURKE((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_TARGETX_CODE:
			data2viewTargetX_m((byte*)pMsg, tempBuff,0);
			strcat(str, tempBuff);
			break;
		case DI_TARGETTELEVIK:
			data2viewTargetTelevik((byte*)pMsg, bParamPD, bParamVr,tempBuff);
			strcat(str, tempBuff);
			break;
		case DI_TARGETSERVAL:
			data2viewTargetServal((byte*)pMsg, bParamPD, bParamVr,tempBuff);
			strcat(str, tempBuff);
			break;			
		case DI_STATESERVAL:
			data2viewStateServal((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;			
		case DI_STATEGLOBALTELEVIK:
			data2viewStateGlobalTelevik((byte*)pMsg, tempBuff);
			strcat(str, tempBuff);
			break;			
		default: 
			strcat(str, " ");
			break;
	}
			
	//
	strcpy(buff, str);
}

void data2strFormularTrassa(DocFormularTrassa* p, int bPD, int bVr, char *str){
	DocRlsTarget docRlsTarget;
	char tmpStr[800];
	unsigned short hex=0;
	
	strcpy(str, " ");
	
	docRlsTarget.bNew = p->NEW;
	docRlsTarget.bKt = p->bKT;
	docRlsTarget.bPrc = p->PRC;
	
	
	memset(&docRlsTarget, 0, sizeof(docRlsTarget));

	if (p->NEW)
		sprintf(tmpStr, "Нт=%d ! ", p->nSource);
	else
		sprintf(tmpStr, "Нт=%d   ", p->nSource);

	strcat(str, tmpStr);
		

	if(p->ShP){
		docRlsTarget.peleng = (float)p->X*M_PI/(1<<15);
		docRlsTarget.distance = 250000.0;
		docRlsTarget.um = (float)(signed short)p->H*M_PI/(1<<16);
		docRlsTarget.vx = docDecodeValue(p->Vx, 5.0, 1, 10)/1000.0;
		docRlsTarget.vy = 0;
		docRlsTarget.vz = docDecodeValue(p->Vh, 5.0, 1, 10)/1000.0;
		docRlsTarget.course      = 0;
		docRlsTarget.vr          = sqrtf ( powf(docRlsTarget.vx,2) + powf(docRlsTarget.vy,2) +
		            							powf(docRlsTarget.vz,2) );
		docRlsTarget.x = docRlsTarget.distance
									*cosf(docRlsTarget.peleng)
									*cosf(docRlsTarget.um);
		docRlsTarget.y = docRlsTarget.distance
									*sinf(docRlsTarget.peleng)
									*cosf(docRlsTarget.um);
		docRlsTarget.z = docRlsTarget.distance*sinf(docRlsTarget.um);
			
			
		docRlsTarget.skoX  = 0;
		docRlsTarget.skoY  = 0;
		docRlsTarget.skoZ  = 0;
		docRlsTarget.skoVx = 0;
		docRlsTarget.skoVy = 0;
		//docRlsTarget.skoVz = 0;
	}else{
		docRlsTarget.x 		= (float)(signed short)p->X*5.0;
		docRlsTarget.y 		= (float)(signed short)p->Y*5.0;
		docRlsTarget.z 		= (float)p->H;
		docRlsTarget.vx 		= docDecodeValue(p->Vx, 5.0, 1, 10);
		docRlsTarget.vy 		= docDecodeValue(p->Vy, 5.0, 1, 10);
		docRlsTarget.vz 		= docDecodeValue(p->Vh, 5.0, 1, 10);
		docRlsTarget.skoX	= (float)p->skoX;
		docRlsTarget.skoY	= (float)p->skoY;
		docRlsTarget.skoZ	= (float)p->skoH;
		docRlsTarget.skoVx 	= (float)p->skoVx*5.0;
		docRlsTarget.skoVy 	= (float)p->skoVy*5.0;
		//docRlsTarget.skoVz 	= (float)p->skoVh*5.0;
		docRlsTarget.distance    = sqrtf ( powf(docRlsTarget.x,2) + powf(docRlsTarget.y,2) +
		                                               powf(docRlsTarget.z,2) );
		docRlsTarget.distanceHor = hypotf (docRlsTarget.x, docRlsTarget.y);
		docRlsTarget.um         	= docDopi(atan2f(docRlsTarget.z, docRlsTarget.distanceHor));
		docRlsTarget.peleng     	= docDo2pi(atan2f(docRlsTarget.y, docRlsTarget.x));
		docRlsTarget.course      = docDo2pi(atan2f(docRlsTarget.vy, docRlsTarget.vx));
		docRlsTarget.vr          = sqrtf ( powf(docRlsTarget.vx,2) + powf(docRlsTarget.vy,2) +
		            							powf(docRlsTarget.vz,2) );

	}
	/*
	sprintf(tmpStr,"x=%0.2f y=%0.2f П=%0.1f Д=%0.2f В=%0.0f УМ=%0.1f ",
						docRlsTarget.x/1000.0,
						docRlsTarget.y/1000.0,
						docRlsTarget.peleng/M_PI*180.0,
						docRlsTarget.distance/1000,
						docRlsTarget.z,
						docRlsTarget.um/M_PI*180.0);
	*/
	if (bPD)
		sprintf(tmpStr,"ШП=%d П=%0.1f Д=%0.2f В=%0.0f Ум=%0.1f ",
			                p->ShP,
			                docRlsTarget.peleng/M_PI*180.0,
							docRlsTarget.distance/1000,
							docRlsTarget.z,
							docRlsTarget.um/M_PI*180.0);
	else
		sprintf(tmpStr,"ШП=%d x =%0.2f y=%0.2f В=%0.0f ",
				                p->ShP,
				                docRlsTarget.x/1000.0,
								docRlsTarget.y/1000.0,
								docRlsTarget.z);
	strcat(str, tmpStr);
	if (bVr) sprintf(tmpStr,"V=%0.1f К=%0.0f", docRlsTarget.vr, docRlsTarget.course/M_PI*180.0);
		else sprintf(tmpStr,"Vx=%0.1f Vy=%0.1f Vh=%0.1f",
					 docRlsTarget.vx,
					 docRlsTarget.vy,
					 docRlsTarget.vz);
	
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
				strcat(str, "Тип=Н ");
				break;
			case 8:
				strcat(str, "Тип=ВН ");
				break;
			case 9:
				strcat(str, "Тип=Л ");
				break;
			case 10:
				strcat(str, "Тип=С ");
				break;
			case 11:
				strcat(str, "Тип=ПКР ");
				break;
			case 12:
				strcat(str, "Тип=ПРР ");
				break;
			case 13:
				strcat(str, "Тип=В ");
				break;
			case 15:
				strcat(str, "Тип=ВПЦ ");
				break;
			default:
				strcat(str, "Тип=Неопр ");
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
	//
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

void data2strIP3_Puma(DocIP3_Puma* p, char *str){
	float speed1;
	DocCountTP docCountTP;
	char tmpStr[800];
	
	strcpy(str, " ");
	memset(&docCountTP,0,sizeof(DocCountTP));
	if ((p->nPuma !=0)|(p->nRls !=0))
		if(p->skk==0){
			docCountTP.x = p->x/1000.0;
			docCountTP.y = p->y/1000.0;
			docCountTP.z = p->h;
			docCountTP.dist = sqrtf(p->x/1000.0*p->x/1000.0+
								p->y/1000.0*p->y/1000.0+
								p->h/1000.0*p->h/1000.0);
			docCountTP.distHor = hypot(p->x, p->y);
			docCountTP.pel = docDo2pi(atan2f(p->y,p->x))/M_PI*180.0;
			docCountTP.um=docDopi(atan2f(p->h, docCountTP.distHor));
			docCountTP.distHor = docCountTP.distHor/1000;
			docCountTP.course = docDo2pi(atan2f(p->vy, p->vx));
			speed1 = p->vx*p->vx+p->vy*p->vy+p->bVh*p->bVh;
			docCountTP.speed = sqrtf(speed1);
		}else{
			docCountTP.x = p->x;
			docCountTP.y = p->y;
			docCountTP.dist = 150.0;
			docCountTP.pel  = docDo2pi(p->x)/M_PI*180.0;
			docCountTP.um  = docDopi(p->y)/M_PI*180.0;
			docCountTP.z = docCountTP.dist*sin(docCountTP.um);
			docCountTP.course = docDo2pi(atan2f(p->vy, p->vx))/M_PI*180.0;
			speed1 = p->vx*p->vx+p->vy*p->vy+p->bVh*p->bVh;
			docCountTP.speed = sqrtf(speed1);
		}

	sprintf(tmpStr, "нРЛС=%d нПума=%d x=%0.2f y=%0.2f П=%0.1f Д=%0.2f В=%0.0f УМ=%0.1f K=%0.0f V=%0.0f",
			p->nRls,
			p->nPuma,
			docCountTP.x,
			docCountTP.y,
			docCountTP.pel,
			docCountTP.dist,
			docCountTP.z,
			docCountTP.um,
			docCountTP.course,
			docCountTP.speed
			);
	
	strcat(str, tmpStr);

	sprintf(tmpStr, " rrc=%d ", p->rrc);
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

void data2strOurShipGeo(DocOurShipGeo* p, char* str){
	char tmpStr[800];
	float fi, lambda;

	strcpy(str, " ");
	sprintf(tmpStr,"К = %d° ", (int)rint(p->course*180.0/M_PI));
	strcat(str, tmpStr);
	
	fi = docDo180(p->fi);
	sprintf( tmpStr,"    Ш=%d°%d'%d",(int) floor(fabs(fi)),
	                      (int) fmod(fabs(fi)*60,60.0),
	                      (int) fmod(fabs(fi)*60*60,60.0));
	strcat(str, tmpStr);
	if(fi > 0) strcat(str," CШ"); else strcat(str," ЮШ");
	
	lambda = docDo180(p->lambda);
	sprintf( tmpStr,"     Д=%d°%d'%d",(int)floor(fabs(lambda)),
		 	                (int) fmod(fabs(lambda)*60,60.0),
		 	                (int) fmod(fabs(lambda)*60*60,60.0));
	strcat(str, tmpStr);
	if 	(lambda > 0) strcat(str," ВД"); else strcat(str," ЗД");
}

void data2strTime(DocTime* p, char* str){
	char tmpStr[800];
	struct tm* restime;
	unsigned int timesigma; 
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

void data2strDoc67Zapros(int type, Doc67Zapros* p, char *str){
	char tmpStr[100];
	strcpy(str,"");
	
	if(p->numberRlk!=0){
		sprintf(tmpStr," нРЛК=%d ", p->numberRlk);
		strcat(str, tmpStr);
	}
//	if(p->number5P20!=0){
//		sprintf(tmpStr, " н5П20=%d", p->number5P20);
//		strcat(str, tmpStr);
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


void data2strDoc67ZaprosSkol(int type, Doc67ZaprosSkol* p, char *str){
char tmpStr[100]; // DI_ZaprosSkol67
	sprintf(tmpStr," пеленг=%0.1f° дальность=%0.0f км ",
			        p->pelengSkol*180.0/M_PI,p->distanceSkol/1000.0);
	strcat(str, tmpStr);
	if (p->lineSkol == 1) strcat(str, " Л:корабль"); else strcat(str, " Л:самолет");
	if (p->modeSkol == 4) strcat(str, ", Реж 4");
}


void data2strDev67tgt(DocDev67Tgt* p, char *str){
	char tmpStr[100];
	
	strcpy(str, " ");
	
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
	if(p->line==4&&p->PN==1){
		sprintf(tmpStr, "нц=%d ", p->num);
		strcat(str, tmpStr);
	}
	
	if(p->B)
		strcat(str, "Б ");

}

void data2strHandGo(DocHandGO* p, char *str){
	char tmpStr[100];
	
	strcpy(str, "");
	if (p->bX ==0)
	sprintf(tmpStr, "нРЛК=%d ", p->numberRlk);
	else sprintf(tmpStr, "нX=%d ", p->numberRlk);
	strcat(str, tmpStr);
	
	switch(p->own){
	case 1: strcat(str, "ГП=С");
		break;
	case 2: strcat(str, "ГП=Ч");
		break;
	default: strcat(str, "ГП=Н");
		break;
	}
}

void data2strTaskTarget(DocTaskTarget* p, char *str,int shipType){
	char tmpStr[100];
	
	strcpy(str, " СК=");
	strcat(str, getStrNumberShootChannel(p->numberShootChannel, shipType));
	
	sprintf(tmpStr, " нРЛК=%d", p->numberRlk);
	strcat(str, tmpStr);
}
void data2strTaskTargetAll(DocTaskTargetAll* p, char *str,int shipType){
	char tmpStr[100];
	
	strcpy(str, " СК=");
	strcat(str, getStrNumberShootChannel(p->numberShootChannel, shipType));
	
	if (p->numberEco>0)
	{	
		sprintf(tmpStr, " нЕЦО=%d", p->numberEco);
		strcat(str, tmpStr);
	}	
	if (p->numberFurke>0)
	{	
		sprintf(tmpStr, " нФ=%d", p->numberFurke);
		strcat(str, tmpStr);
	}	
	if (p->numberX>0)
	{	
		sprintf(tmpStr, " нХ=%d", p->numberX );
		strcat(str, tmpStr);
	}	
}

void data2strNullShootChannel(DocNullShootChannel* p, char *str, int shipType){
	char tmpStr[100];
	
	strcpy(str, " ");
	strcat(str, getStrNumberShootChannel(p->numberShootChannel, shipType));
	strcat(str, " ");
	
	getStrMsg(DI_NullShootChannel, p->parameter, tmpStr);
	strcat(str, tmpStr);
}

void data2strResetShootChannel(DocResetShootChannel* p, char *str, int shipType){
	char tmpStr[100];
	
	strcpy(str, " ");
	strcat(str, getStrNumberShootChannel(p->numberShootChannel, shipType));
	strcat(str, " ");
	
	getStrMsg(DI_ResetShootChannel, p->parameter, tmpStr);
	strcat(str, tmpStr);
}

void data2strSendRli(DocSendRli* p, char *str){
	char tmpStr[100];
	
	strcpy(str, " ");
	sprintf(tmpStr, "НТ=%d", p->nSource);
	strcat(str,tmpStr);
}


void data2strRVVRedut(DocRVVRedut* p, char *str, int shipType){
	char tmpStr[100];
	sprintf(str, " нРЛК=%d СК=", p->numberRlk);
	strcat(str, getStrNumberShootChannel(p->numberShootChannel, shipType));
	getStrMsg(DI_RVVRedut, p->RVV, tmpStr);
	strcat(tmpStr, str);
}
/*

void data2strOPERATOR_DIRECTIVE(DocOPERATOR_DIRECTIVE_STRUCT2* p, char *str){
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

void data2strANSWER_BOS_OPERATOR(DocANSWER_BOS_OPERATOR_STRUCT3* p, char *str){
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
		sprintf(tmpStr,"Юстировочная поправка П=%dмин Д=%dм УМ=%dмин",p2->B1,p2->R1,p2->B2;
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

void data2strADAPTATION_CONTROL(DocCODOGRAMM_ADAPTATION_CONTROL_STRUCT* p, char *str){
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
	


void data2strANSWER_ADAPTATION_CONTROL(DocANSWER_ADAPTATION_CONTROL_STRUCT* p, char *str){
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
*/


void data2strSTATE_BOS(DocCODOGRAMM_GENERAL_MODES_STRUCT* p, char* str){
	char tmpStr[1000]="";
	
	strcpy(str, " ");
	sprintf(tmpStr, "нРЛК=%d ", 5);
	strcat(str, tmpStr);
}




void data2strMainDoc(DocMainDoc* p, char* str){
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

void data2strPAV_OK(DocPAV_OK_STRUCT* p, char* str){
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

