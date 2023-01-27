#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fcntl.h"

#include "docStructs.h"
#include "docCodeMsg.h"


unsigned int msgDocCount = 0;

FILE* fp = NULL;
unsigned char _START_DOC = 0;
unsigned char _SavingDOC = 0;
struct timespec _timeStartDoc;

int createDoc(void){
	msgDocCount = 0;	
	fp = fopen("/tmp/tmp.log", "wb");
	if(fp!=NULL)
		return 1;
	else 
		return 0;
}

void startDoc(struct timespec timeStartDoc){
	_START_DOC = 1;
	memcpy(&_timeStartDoc,&timeStartDoc,sizeof(timeStartDoc));
}

void stopDoc(void){
	_START_DOC = 0;
}

int testFile(char *s){
	int tempid;
	tempid = open(s, O_RDWR | O_CREAT | O_TRUNC  );
	if (tempid<=0)
	{	
		return -1;
	}	
	else close(tempid);
	return 1;
}

int saveDoc(char* savePath, char* saveDir2){
	char path[256], s[200];
	_START_DOC = 0;
	_SavingDOC = 1;
	
		
	if(fp!=NULL){
		fwrite(&msgDocCount, sizeof(msgDocCount), 1, fp);
		fclose(fp);
	}

	if(savePath!=NULL){
		strcpy(path, "mv ");
		strcat(path, "/tmp/tmp.log ");
		strcat(path, savePath);
	}else{
		strcpy(path, "mv /tmp/tmp.log ");
		strcpy(savePath," /tmp/tmp_complete.log");
		strcat(path, savePath);
	}
	system(path);
	
	if(saveDir2!=NULL)
	{	
		sprintf(s,"%s/1.log",saveDir2);
		if (testFile(s)==1)
		{	
			strcpy(path, "cp ");
			strcat(path, savePath);
			strcat(path, " ");
			strcat(path, saveDir2);
			system(path);
			strcpy(path, "rm ");
			strcat(path, savePath);
			system(path);
			printf(path);
		}	
	}	
	_SavingDOC = 0;
	createDoc();	
	return 1;
}

void closeDoc(void){
	if(fp!=NULL)
		fclose(fp);
}


int getTypeSize(int type){
	switch(type){
	case DI_FORMULAR_TRASSA: return sizeof(DocFormularTrassa);
		break;
	case DI_IP3_Puma: return sizeof(DocIP3_Puma);
		break;
	case DI_OURSHIPGEO: return sizeof(DocOurShipGeo);
		break;
	case DI_TIME: return sizeof(DocTime);
		break;
	case DI_Dev67Alarm: return sizeof(DocDev67Alarm);
		break;
	case DI_Dev67Tgt: return sizeof(DocDev67Tgt);
			break;
	case DI_DeleteZapros: return sizeof(Doc67Zapros);
			break;
	case DI_SwitchOff67: return sizeof(Doc67Zapros);
			break;
	case DI_NewZapros67: return sizeof(Doc67Zapros);
			break;
	case DI_HandGO: return sizeof(DocHandGO);
			break;
	case DI_Zapros67: return sizeof(Doc67Zapros);
			break;
	case DI_ZaprosSkol67: return sizeof(Doc67ZaprosSkol);
			break;
	case DI_TaskTarget: return sizeof(DocTaskTarget);
			break;
	case DI_NullShootChannel: return sizeof(DocNullShootChannel);
			break;
	case DI_ResetShootChannel: return sizeof(DocResetShootChannel);
			break;
	case DI_Result_5P27: return sizeof(DocHandGO);
			break;
	case DI_Result_5P20: return sizeof(DocDev67Tgt);

	case DI_SendRli: return sizeof(DocSendRli);
			break;
	case DI_SendRedut: return sizeof(DocSendRedut);
			break;
	case DI_RVVRedut: return sizeof(DocRVVRedut);
			break;
	case DI_OPERATOR_DIRECTIVE: return sizeof(DocOPERATOR_DIRECTIVE_STRUCT2);
			break;
	case DI_ANSWER_BOS_OPERATOR: return sizeof(DocANSWER_BOS_OPERATOR_STRUCT2);
			break;
	case DI_ADAPTATION_CONTROL: return sizeof(DocCODOGRAMM_ADAPTATION_CONTROL_STRUCT);
			break;
	case DI_ANSWER_ADAPTATION_CONTROL: return sizeof(DocANSWER_ADAPTATION_CONTROL_STRUCT);
			break;
	case DI_STATE_BOS: return sizeof(DocCODOGRAMM_GENERAL_MODES_STRUCT);
			break;
	case DI_MAINDOC: return sizeof(DocMainDoc);
			break;
	case DI_PAV_OK: return sizeof(DocPAV_OK_STRUCT);
			break;
	case DI_OURSHIPGEOEX: return sizeof(DocOurShipGeo);
			break;
	case DI_COMMANDX_STROB_CODE: return sizeof(DocCommandX_Strob);
			break;
	case DI_COMMANDX_CHANGEPRI_CODE: return sizeof(DocCommandX_ChangePri);
			break;
	case DI_RESULT_COMMANDX_CODE: return sizeof(DocResult_CommandX);
			break;
	case DI_EMS_SU: return sizeof(DocEMS_SU);
			break;
	case DI_EMS_FURKE: return sizeof(DocEMS_FURKE);
			break;
	case DI_TARGETX_CODE: return sizeof(DocTargetX_m);
			break;
	case DI_TARGETTELEVIK: return sizeof(DocTargetTelevik);
			break;
	case DI_TARGETSERVAL: return sizeof(DocTargetServal);
			break;
	case DI_STATESERVAL: return sizeof(DocStateServal);
			break;
	case DI_STATEGLOBALTELEVIK: return sizeof(DocStateGlobalTelevik);
			break;
	case DI_TARGETPRISTAVKA: return sizeof(DocTargetPristavka);
			break;
	case DI_STATEPRISTAVKA: return sizeof(DocStatePristavka);
			break;
	case DI_STATEGLOBALISTRA: return sizeof(DocStateGlobalIstra);
			break;
	case DI_TaskTargetAll: return sizeof(DocTaskTargetAll);
			break;
			
	default:return 0;
		break;
	}
}


void sendDocMsg(unsigned char type, void *p){
	unsigned char buff[1000];
	unsigned char size=0;
	int sz = sizeof(int);
	unsigned int intTime;
	struct timespec curr;
	double diffTime;
	
	if(!_START_DOC)
		return;
	clock_gettime( CLOCK_REALTIME, &curr) ;// получаем текущее время
	diffTime = (curr.tv_sec - _timeStartDoc.tv_sec) * 1.0 
				+ (double)( curr.tv_nsec - _timeStartDoc.tv_nsec )
              / (double)1000000000L;		
	intTime = (unsigned int)(diffTime*100.0);

	size = getTypeSize(type);
	
	buff[0] = size+1+sz;
	memcpy(&buff[1],&intTime,sizeof(int));
	buff[sz+1] = type;
	memcpy(&buff[sz+2], p, size);
	buff[size+2+sz] = size+1+sz;
	size =  size+3+sz;
	
	if(fp!=NULL){
		fwrite(buff, 1,size, fp);
		//printf("type=%d size=%d\n",type,size);
		msgDocCount++;
	}else{// Пробуем переоткрыть файл
		createDoc();
	}
}



void parseDocMsg(unsigned char type, void *p, char* buff,int bParamPD,int bParamVr, int shipType){
	char tmpStr[1000];
	char str[1000];
	strcpy(tmpStr,"");
	strcpy(str,"");
	if(type==DI_FORMULAR_TRASSA){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewFormularTrassa(p, tmpStr);
		strcat(buff, tmpStr);
	}else if(type== DI_IP3_Puma){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewIP3_Puma(p, tmpStr);
		strcat(buff, tmpStr);
	}else if(type==DI_DeleteZapros){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewDoc67Zapros(DI_DeleteZapros, p, tmpStr);
		strcat(buff, tmpStr);
	}else if(type==DI_Result_5P27){
		strcpy(buff, getStrMsg(type, 0, str));
	}else if(type==DI_Result_5P20){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewDev67tgt(p, tmpStr);
		strcat(buff, tmpStr);
	}else if(type==DI_OURSHIPGEO){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewOurShipGeo(p, tmpStr);
		strcat(buff, tmpStr);
	}else if(type==DI_TIME){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");		
		data2viewTime(p, tmpStr);
		strcat(buff, tmpStr);
	}else if(type==DI_Dev67Tgt){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewDev67tgt(p, tmpStr);
		strcat(buff, tmpStr);
	}else if(type==DI_SwitchOff67){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewDoc67Zapros(DI_DeleteZapros, p, tmpStr);
		strcat(buff, tmpStr);
	}else if(type==DI_NewZapros67){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewDoc67Zapros(DI_DeleteZapros, p, tmpStr);
		strcat(buff, tmpStr);
	}else if(type==DI_HandGO){
		strcpy(buff, getStrMsg(type, 0, str));
	}else if(type==DI_Zapros67){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewDoc67Zapros(DI_DeleteZapros, p, tmpStr);
		strcat(buff, tmpStr);
	}else if(type==DI_ZaprosSkol67){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2strDoc67ZaprosSkol(DI_ZaprosSkol67, p, tmpStr);
		strcat(buff, tmpStr);
	}else if(type==DI_TaskTarget){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");		
		data2strTaskTarget((byte*)p, str, shipType);
		strcat(buff, str);
	}else if(type==DI_TaskTargetAll){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");		
		data2strTaskTargetAll((byte*)p, str, shipType);
		strcat(buff, str);
	}else if(type==DI_NullShootChannel){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");		
		data2strNullShootChannel((byte*)p, str, shipType);
		strcat(buff, str);
	}else if(type==DI_ResetShootChannel){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");		
		data2strResetShootChannel((byte*)p, str, shipType);
		strcat(buff, str);
	}else if(type==DI_SendRli){
		DocSendRli* sp = (DocSendRli*)p;
		strcpy(buff, getStrMsg(type, sp->nSource, str));
		strcat(buff, " ");
		strcat(buff, str);
	}else if(type==DI_SendRedut){
		strcpy(buff, getStrMsg(type, 0, str));
	}else if(type==DI_RVVRedut){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");		
		data2strRVVRedut((byte*)p+5, str, shipType);
		strcat(buff, str);
		
	}else if(type==DI_OPERATOR_DIRECTIVE){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewOPERATOR_DIRECTIVE(p, tmpStr);
		strcat(buff, tmpStr);		
	}else if(type==DI_ANSWER_BOS_OPERATOR){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewANSWER_BOS_OPERATOR(p, tmpStr);
		strcat(buff, tmpStr);	
	}else if(type==DI_ADAPTATION_CONTROL){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewADAPTATION_CONTROL(p, tmpStr);
		strcat(buff, tmpStr);		
	}else if(type==DI_ANSWER_ADAPTATION_CONTROL){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewANSWER_ADAPTATION_CONTROL(p, tmpStr);
		strcat(buff, tmpStr);		
	}else if(type==DI_STATE_BOS){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewSTATE_BOS(p, tmpStr);
		strcat(buff, tmpStr);		
	}else if(type==DI_MAINDOC){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewMainDoc(p, tmpStr);
		strcat(buff, tmpStr);		
	}else if(type==DI_PAV_OK){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewPAV_OK(p, tmpStr);
		strcat(buff, tmpStr);		
	}else if(type==DI_COMMANDX_STROB_CODE){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewCommandX_Strob(p, tmpStr);
		strcat(buff, tmpStr);		
	}else if(type==DI_COMMANDX_CHANGEPRI_CODE){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewCommandX_ChangePri(p, tmpStr);
		strcat(buff, tmpStr);		
	}else if(type==DI_RESULT_COMMANDX_CODE){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewResult_CommandX(p, tmpStr);
		strcat(buff, tmpStr);		
	}else if(type==DI_EMS_SU){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewEMS_SU(p, tmpStr);
		strcat(buff, tmpStr);		
	}else if(type==DI_EMS_FURKE){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewEMS_FURKE(p, tmpStr);
		strcat(buff, tmpStr);		
	}else if(type==DI_TARGETX_CODE){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewTargetX_m(p, tmpStr,0);
		strcat(buff, tmpStr);
	}else if(type==DI_TARGETTELEVIK){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewTargetTelevik(p, bParamPD, bParamVr,tmpStr);
		strcat(buff, tmpStr);
	}else if(type==DI_TARGETSERVAL){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewTargetServal(p, bParamPD, bParamVr,tmpStr);
		strcat(buff, tmpStr);		
	}else if(type==DI_STATESERVAL){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewStateServal(p, tmpStr);
		strcat(buff, tmpStr);		
	}else if(type==DI_STATEGLOBALTELEVIK){
		strcpy(buff, getStrMsg(type, 0, str));
		strcat(buff, " ");
		data2viewStateGlobalTelevik(p, tmpStr);
		strcat(buff, tmpStr);		
	}else{
		strcpy(buff, getStrMsg(type, 0, str));
	}
	
	 
}

char * getStrMsg(word msgCode, word parameter,char * str_parameter)
{
	char buff[10];
	strcpy(str_parameter,"");
	switch (msgCode) {
		case DI_FORMULAR_TRASSA: 
			return "27_Ц";
			break;
		case DI_IP3_Puma: return "10_Цель"; break;
		case DI_OURSHIPGEO: return "ШД, К свой"; break;
		case DI_TIME: return "время"; break;
		case DI_Dev67Tgt: return "67_Цель";break;
		case DI_DeleteZapros:
			 switch (parameter) 
			 {
				case 1: strcpy(str_parameter,"Отсутствуют запросы от источников"); break;
				case 2: strcpy(str_parameter,"Цель потеряна"); break;
				case 3: strcpy(str_parameter,"Превышено время обновления"); break;		
				case 4: strcpy(str_parameter,"Цель отвечает: своя"); break;
				case 5: strcpy(str_parameter,"Цель не отвечает: чужая"); break;		
				default: strcpy(str_parameter,"Оператор"); break;
			 }
			return "ГО: удаление запроса"; break;
		case DI_SwitchOff67: return "ГО: отключение 67"; break;
		case DI_NewZapros67: return "ГО: запрос по цели"; break;
		case DI_HandGO: return "ГО: назначение ГП"; break;
		case DI_Zapros67: return "ГО: произвести в автомате"; break;
		case DI_Result_5P27: return "ГО: присвоение ГП"; break;
		case DI_Result_5P20: return "ГО: по цели 5П20"; break;
		case DI_TaskTarget: return "ЦУ: выдано ЦУ"; break;
		case DI_TaskTargetAll: return "ЦУ: выдано ЦУ"; break;
		case DI_NullShootChannel: 
			 switch (parameter) 
			 {
				case 1: strcpy(str_parameter,"Неготовность стрельбового комплекса"); break;
				case 2: strcpy(str_parameter,"Истекло время выдачи ЦУ"); break;
				case 3: strcpy(str_parameter,"Истекло время сброса ЦУ"); break;		
				case 4: strcpy(str_parameter,"Истекло время на прием ЦУ"); break;
				case 5: strcpy(str_parameter,"Принята ОТМЕНА"); break;		
				default: strcpy(str_parameter,"Оператор"); break;
			}
			return "ЦУ: очистка СК";
		break;
		case DI_ResetShootChannel:
			switch (parameter) 
			{
				case 1: strcpy(str_parameter,"Цель потеряна"); break;
				case 2: strcpy(str_parameter,"Принят ОТКАЗ"); break;
				case 3: strcpy(str_parameter,"Обн более опасная цель"); break;
				default: strcpy(str_parameter,"Оператор"); break;
			}
			return "ЦУ: сброс ЦУ";
		break;
		case DI_SendRli:
			itoa(parameter,str_parameter,10);
			return "РЛИ: выдача"; 
		break;
		case DI_SendRedut: return "3К96: обновление"; break;
		case DI_RVVRedut:
			switch (parameter) 
						 {
							case 0: return "Приоритетная цель:";break;
							case 1: return "Отмена приоритета:"; break;
							default: return "Приоритет?:";break;
						 }
		case DI_OPERATOR_DIRECTIVE: return "ДирОператора:"; break;
		case DI_ANSWER_BOS_OPERATOR: return "КвитОператору:"; break;
		case DI_ADAPTATION_CONTROL: return "Дир АДУ:"; break;
		case DI_ANSWER_ADAPTATION_CONTROL: return "Квит АДУ:"; break;
		case DI_STATE_BOS: return "БОС:"; break;
		case DI_MAINDOC: return "Реж:"; break;
		case DI_PAV_OK: return "ПАВ:"; break;
		case DI_COMMANDX_STROB_CODE: return "СтробвХ:"; break;
		case DI_COMMANDX_CHANGEPRI_CODE: return "ИзиПриор:"; break;
		case DI_RESULT_COMMANDX_CODE: return "ПАВ:"; break;
		case DI_EMS_SU: return "ЕМС от СУ:"; break;
		case DI_EMS_FURKE: return "Отработка ЭМС:"; break;
		case DI_TARGETX_CODE: return "Х_цель:"; break;
		case DI_TARGETTELEVIK: return "T_цель:"; break;
		case DI_TARGETSERVAL: return "C_цель:"; break;
		case DI_STATESERVAL: return "Сервал:"; break;
		case DI_STATEGLOBALTELEVIK: return "Телевик:"; break;
		default: 
			return "27_Ц";
		break;
	};
}



char * getStrNumberShootChannel(word numberShootChannel, int shipType)// строковое значение номера стрельбового канала
{
	switch (numberShootChannel) 
	{
		case 1: if (shipType == 99) return("Сервал");
				else return("А-190"); break;
		case 2: return("АК-630"); break;
		case 3: if (shipType == 1) return("3М-87");
					else return("3К96 канал 1"); 
				break;
		case 4: return("3К96 канал 2"); break;
		case 5: return("3К96 канал 3"); break;
		case 6: return("3К96 канал 4"); break;
		case 7: return("3К96 канал 5"); break;
		case 8: return("3К96 канал 6"); break;
		default : return ("А-190"); break;
	}
}
