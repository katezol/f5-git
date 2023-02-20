#include <furke2Doc.h>
#include <docCodeMsg.h>

extern unsigned char _START_DOC;
extern struct timespec _timeStartDoc;

int sendDocFormularTrassa
    (byte * filter_DI, FORMULAR_TRASSA_STRUCT ft,
    		bool b3K, word time3k96)
{
	DocFormularTrassa docFt;	
	if (filter_DI[DI_FORMULAR_TRASSA] ==0) return -1;
	memcpy(&docFt,&ft,sizeof(FORMULAR_TRASSA_STRUCT));
	docFt.skoH = time3k96;
	docFt.skoVh = b3K;
	docFt.nType = 16; // признак записи в skoVh наличия привязки к времени 3К96, 
					// в skoH - времени 3К96	
	sendDocMsg(DI_FORMULAR_TRASSA, &docFt);
	return 1;
}

int sendDocIP3_Puma
	(byte * filter_DI, int ind, IP3_Puma * ip3_Puma)
{
	DocIP3_Puma docIP3Puma;
	static IP3_Puma buff_ip3[4];
	if (filter_DI[DI_IP3_Puma] ==0) return -1;
	if (_START_DOC == 0) return -1;
	if (ip3_Puma->nPuma ==0)
	{// нулевые пакеты записываем только первый раз 	
		if (memcmp(&buff_ip3[ind],ip3_Puma,sizeof(IP3_Puma))==0) return -1;
	}			
	memcpy(&buff_ip3[ind], ip3_Puma, sizeof(IP3_Puma)); // копируем 
	docIP3Puma.indPuma = ind;
	/*printf("&(docIP3Puma.nPuma)=%d &docIP3Puma=%d sizeof(IP3_Puma)=%d sizeof(docIP3Puma)=%d",
			&(docIP3Puma.nPuma),
			&(docIP3Puma),
			sizeof(IP3_Puma),
			sizeof(docIP3Puma));
	*/		
	memcpy(&(docIP3Puma.nPuma),ip3_Puma,sizeof(IP3_Puma));		
	sendDocMsg( DI_IP3_Puma, &docIP3Puma);
	return 1;
}

int sendDocOurShipGeo
	(OurShipGeo OurShipGeo)
{
	DocOurShipGeo docOurShipGeo;
	memcpy (&docOurShipGeo,&OurShipGeo,sizeof(OurShipGeo));
	sendDocMsg(DI_OURSHIPGEO,&docOurShipGeo);
	return 1;
}
int sendDocTime
	(unsigned int timeSigma,unsigned int timeBOS,word time3K96)
{
	DocTime docTime;
	struct timespec curr;
	double diffTime;
	
	memset(&docTime,0,sizeof(docTime));
	clock_gettime( CLOCK_REALTIME, &curr) ;// получаем текущее время
	diffTime = (curr.tv_sec - _timeStartDoc.tv_sec) * 1.0 
				+ (double)( curr.tv_nsec - _timeStartDoc.tv_nsec )
              / (double)1000000000L;		
	docTime.currentTime = diffTime;
	docTime.timeSigma = timeSigma;
	docTime.timeBOS = timeBOS;
	docTime.time3K96 = time3K96;
	sendDocMsg(DI_TIME,&docTime);
	return 1;
}

int sendDocDev67Alarm
	(byte * filter_DI, Dev67Alarm_t al)
{
	DocDev67Alarm docDev67Alarm;
	if (filter_DI[DI_Dev67Alarm] ==0) return -1;
	docDev67Alarm.pel = al.pel;	
	sendDocMsg(DI_Dev67Alarm, &docDev67Alarm);
	return 1;
}


int sendDocDev67Tgt
	(byte * filter_DI, Dev67Tgt_t tgt, word line, word mode, word numberRlk)
{
	DocDev67Tgt docDev67Tgt;
	if (filter_DI[DI_Dev67Tgt] ==0) return -1;
	docDev67Tgt.B = tgt.B;
	docDev67Tgt.PN = tgt.PN;	
	docDev67Tgt.dist = tgt.dist;	
	docDev67Tgt.num = tgt.num;	
	docDev67Tgt.pel = tgt.pel;
	docDev67Tgt.mode = mode;	
	docDev67Tgt.numberRLK = numberRlk;	
	docDev67Tgt.line = line;
	sendDocMsg( DI_Dev67Tgt, &docDev67Tgt);
	return 1;
}

int sendDoc67Zapros// типа Doc67Zapros
	(byte * filter_DI, SR_target sr_target)
{
	Doc67Zapros doc67Zapros;
	if (filter_DI[DI_Zapros67] ==0) return -1;
	doc67Zapros.numberRlk = sr_target.tracking_number;
	doc67Zapros.numberX = sr_target.number_X;
	doc67Zapros.numberSfera = sr_target.number_Sfera;
	doc67Zapros.sourceGo.b3K96 	= (sr_target.number_3K96>0);
	doc67Zapros.sourceGo.bX 	= (sr_target.number_X>0);
	doc67Zapros.sourceGo.b5P27 	= (sr_target.number_BOS>0);
	doc67Zapros.sourceGo.bMTK 	= (sr_target.number_MTK>0);
	doc67Zapros.sourceGo.bSigma = (sr_target.number_sigma>0);
	doc67Zapros.sourceGo.bSfera= (sr_target.number_Sfera>0);
	sendDocMsg( DI_Zapros67, &doc67Zapros);
	return 1;
}

int sendDocNewZapros67// типа Doc67Zapros
	(byte * filter_DI,SR_target sr_target)
{
	Doc67Zapros doc67Zapros;
	if (filter_DI[DI_NewZapros67] ==0) return -1;
	memset(&doc67Zapros,0,sizeof(doc67Zapros));
	doc67Zapros.numberRlk = sr_target.tracking_number;
	doc67Zapros.numberX = sr_target.number_X;
	doc67Zapros.numberSfera = sr_target.number_Sfera;
	doc67Zapros.sourceGo.b3K96 	= (sr_target.number_3K96>0);
	doc67Zapros.sourceGo.b5P20 	= (sr_target.number_5P20>0);
	doc67Zapros.sourceGo.b5P27 	= (sr_target.number_BOS>0);
	doc67Zapros.sourceGo.bMTK 	= (sr_target.number_MTK>0);
	doc67Zapros.sourceGo.bSigma = (sr_target.number_sigma>0);
	doc67Zapros.sourceGo.bX 	= (sr_target.number_X>0);
	doc67Zapros.sourceGo.bSfera 	= (sr_target.number_Sfera>0);
	sendDocMsg( DI_NewZapros67, &doc67Zapros);
	return 1;
}

int sendDocSwitchOff67// типа Doc67Zapros
	(byte * filter_DI, word numberRlk, word numberX, word numberSfera)
{
	Doc67Zapros doc67Zapros;
	if (filter_DI[DI_SwitchOff67] ==0) return -1;
	memset(&doc67Zapros,0,sizeof(doc67Zapros));
	doc67Zapros.numberRlk = numberRlk;
	doc67Zapros.numberX = numberX;
	doc67Zapros.numberSfera = numberSfera;
	sendDocMsg(DI_SwitchOff67, &doc67Zapros);
	return 1;
}

int sendDocDeleteZapros// типа Doc67Zapros
	(byte * filter_DI, SR_target sr_target,word parameter)// word numberRlk, word number5P20, word parameter,SourceGo sourceGo)
{
	Doc67Zapros doc67Zapros;
	if (filter_DI[DI_DeleteZapros] ==0) return -1;
	memset(&doc67Zapros,0,sizeof(doc67Zapros));
	doc67Zapros.numberRlk = sr_target.tracking_number;
	doc67Zapros.numberX = sr_target.number_X;
	doc67Zapros.numberSfera = sr_target.number_Sfera;
	doc67Zapros.parameter = parameter;
	doc67Zapros.sourceGo.b3K96 	= (sr_target.number_3K96>0);
	doc67Zapros.sourceGo.b5P20 	= (sr_target.number_5P20>0);
	doc67Zapros.sourceGo.b5P27 	= (sr_target.number_BOS>0);
	doc67Zapros.sourceGo.bMTK 	= (sr_target.number_MTK>0);
	doc67Zapros.sourceGo.bSigma = (sr_target.number_sigma>0);
	doc67Zapros.sourceGo.bSfera = (sr_target.number_Sfera>0);
	doc67Zapros.sourceGo.bX = (sr_target.number_X>0);
	sendDocMsg( DI_DeleteZapros, &doc67Zapros);
	return 1;
}

int sendDocHandGO
	(byte * filter_DI, word numberRlk, word own, word bX)
{
	DocHandGO docHandGO;
	if (filter_DI[DI_HandGO] ==0) return -1;
	docHandGO.numberRlk = numberRlk;
	docHandGO.own  = own;
	docHandGO.bX  = bX;
	sendDocMsg(DI_HandGO, &docHandGO);
	return 1;
}

int sendDocResult_5P27// типа DocHandGO
	(byte * filter_DI, word numberRlk, word own)
{
	DocHandGO docHandGO;
	if (filter_DI[DI_Result_5P27] ==0) return -1;
	docHandGO.numberRlk = numberRlk;
	docHandGO.own  = own;
	sendDocMsg(DI_Result_5P27, &docHandGO);
	return 1;
}

int sendDocResult_5P20// типа DocDev67Tgt
	(byte * filter_DI, Dev67Tgt_t tgt, 
	 word line, word mode, word number5P20)
{
	DocDev67Tgt docDev67Tgt;
	if (filter_DI[DI_Result_5P20] ==0) return -1;
	docDev67Tgt.B = tgt.B;
	docDev67Tgt.PN = tgt.PN;	
	docDev67Tgt.dist = tgt.dist;	
	docDev67Tgt.num = tgt.num;	
	docDev67Tgt.pel = tgt.pel;
	docDev67Tgt.mode = mode;	
	docDev67Tgt.numberRLK = number5P20;	
	docDev67Tgt.line = line;
	sendDocMsg(DI_Result_5P20, &docDev67Tgt);
	return 1;
}


int sendDocTaskTarget//DocTaskTarget;
	(byte * filter_DI, word numberRlk, word numberShootChannel)
{
	DocTaskTarget docTaskTarget;
	if (filter_DI[DI_TaskTarget] ==0) return -1;
	docTaskTarget.numberRlk = numberRlk;
	docTaskTarget.numberShootChannel  = numberShootChannel;
	sendDocMsg( DI_TaskTarget, &docTaskTarget);
	return 1;
}	

int sendDocTaskTargetAll//DocTaskTargetAll;
	(byte * filter_DI, word numberEco,word numberX,word numberFurke, word numberShootChannel)
{
	DocTaskTargetAll docTaskTargetAll;
	if (filter_DI[DI_TaskTargetAll] ==0) return -1;
	docTaskTargetAll.numberFurke = numberFurke;
	docTaskTargetAll.numberX  = numberX;
	docTaskTargetAll.numberEco  = numberEco;
	docTaskTargetAll.numberShootChannel  = numberShootChannel;
	sendDocMsg( DI_TaskTargetAll, &docTaskTargetAll);
	return 1;
}	

int sendDocNullShootChannel//DocNullShootChannel;
	(byte * filter_DI, word numberShootChannel, word parameter)
{
	DocNullShootChannel docNullShootChannel;
	if (filter_DI[DI_NullShootChannel] ==0) return -1;
	docNullShootChannel.parameter = parameter;
	docNullShootChannel.numberShootChannel  = numberShootChannel;
	docNullShootChannel.parameter = parameter;  
	sendDocMsg( DI_NullShootChannel, &docNullShootChannel);
	return 1;
}	

int sendDocResetShootChannel//DocResetShootChannel
	(byte * filter_DI,  word numberShootChannel, word parameter)
{
	DocResetShootChannel docResetShootChannel;
	if (filter_DI[DI_ResetShootChannel] ==0) return -1;
	docResetShootChannel.parameter = parameter;
	docResetShootChannel.numberShootChannel  = numberShootChannel;
	docResetShootChannel.parameter = parameter;
	sendDocMsg( DI_ResetShootChannel, &docResetShootChannel);
	return 1;
}

int sendDocSendRli// типа DocSendRli
	(byte * filter_DI,  word numberRlk)
{
	DocSendRli docSendRli;
	if (filter_DI[DI_SendRli] ==0) return -1;
	docSendRli.nSource = numberRlk;
	sendDocMsg( DI_SendRli, &docSendRli);
	return 1;
}

int sendDocSendRedut (byte * filter_DI)// типа DocSendRedut
{
	DocSendRedut docSendRedut;
	if (filter_DI[DI_SendRedut] ==0) return -1;
	sendDocMsg( DI_SendRedut, &docSendRedut);
	return 1;
}
int sendDocRVVRedut// DocRVVRedut
	(byte * filter_DI, word numberRlk, word numberShootChannel,word RVV)
{
	DocRVVRedut docRVVRedut;
	if (filter_DI[DI_RVVRedut] ==0) return -1;
	docRVVRedut.RVV = RVV;
	docRVVRedut.numberRlk = numberRlk;
	docRVVRedut.numberShootChannel = numberShootChannel;	
	sendDocMsg( DI_RVVRedut, &docRVVRedut);
	return 1;
}


int sendDocOPERATOR_DIRECTIVE// DocOPERATOR_DIRECTIVE_STRUCT2
	(byte * filter_DI, OPERATOR_DIRECTIVE_STRUCT od){
	DocOPERATOR_DIRECTIVE_STRUCT2 docod;	
	if (filter_DI[DI_OPERATOR_DIRECTIVE] ==0) return -1;
	memcpy(&docod,&od,sizeof(OPERATOR_DIRECTIVE_STRUCT2));
	sendDocMsg(DI_OPERATOR_DIRECTIVE, &docod);
	return 1;	
	
}
int sendDocANSWER_BOS_OPERATOR// DocANSWER_BOS_OPERATOR_STRUCT2
	(byte * filter_DI, ANSWER_BOS_OPERATOR_STRUCT answer_od){
	DocANSWER_BOS_OPERATOR_STRUCT2 docanswer_od;	
	if (filter_DI[DI_ANSWER_BOS_OPERATOR] ==0) return -1;
	memcpy(&docanswer_od,&answer_od,sizeof(ANSWER_BOS_OPERATOR_STRUCT));
	sendDocMsg(DI_ANSWER_BOS_OPERATOR, &docanswer_od);
	return 1;	
}
int sendDocADAPTATION_CONTROL// DocCODOGRAMM_ADAPTATION_CONTROL_STRUCT
	(byte * filter_DI, CODOGRAMM_ADAPTATION_CONTROL_STRUCT ac){
	DocCODOGRAMM_ADAPTATION_CONTROL_STRUCT docac;	
	if (filter_DI[DI_ADAPTATION_CONTROL] ==0) return -1;
	memcpy(&docac,&ac,sizeof(CODOGRAMM_ADAPTATION_CONTROL_STRUCT));
	sendDocMsg(DI_ADAPTATION_CONTROL, &docac);
	return 1;	
}
int sendDocANSWER_ADAPTATION_CONTROL// DocANSWER_ADAPTATION_CONTROL_STRUCT
	(byte * filter_DI, ANSWER_ADAPTATION_CONTROL_STRUCT answer_ac){
	DocANSWER_ADAPTATION_CONTROL_STRUCT docanwer_ac;	
	if (filter_DI[DI_ADAPTATION_CONTROL] ==0) return -1;
	memcpy(&docanwer_ac,&answer_ac,sizeof(ANSWER_ADAPTATION_CONTROL_STRUCT));
	sendDocMsg(DI_ADAPTATION_CONTROL, &docanwer_ac);
	return 1;	
}
int sendSTATE_BOS// DocCODOGRAMM_GENERAL_MODES_STRUCT
	(byte * filter_DI, CODOGRAMM_GENERAL_MODES_STRUCT state_bos)
{
	DocCODOGRAMM_GENERAL_MODES_STRUCT doc_state_bos;
	if (filter_DI[DI_STATE_BOS] ==0) return -1;
	memcpy(&doc_state_bos,&state_bos,sizeof(state_bos));
	sendDocMsg( DI_STATE_BOS, &doc_state_bos);
	return 1;
}

int sendMainDoc// DocMainDoc
	(byte * filter_DI, MainDoc mainDoc)
{
	DocMainDoc doc_main_doc;
	if (filter_DI[DI_MAINDOC] ==0) return -1;
	memcpy(&doc_main_doc,&mainDoc,sizeof(mainDoc));
	sendDocMsg( DI_MAINDOC, &doc_main_doc);
	return 1;
}

int sendPAV_OK// DocPAV_OK_STRUCT
	(byte * filter_DI, PAV_OK_STRUCT pav_ok)
{
	DocPAV_OK_STRUCT docPAV_OK_STRUCT;
	if (filter_DI[DI_PAV_OK] ==0) return -1;
	memcpy(&docPAV_OK_STRUCT,&pav_ok,sizeof(pav_ok));
	sendDocMsg( DI_PAV_OK, &docPAV_OK_STRUCT);
	return 1;
}
int sendDoc67ZaprosSkol// типа Doc67ZaprosSkol
	(byte * filter_DI,SkolToGo skolToGo)
{
	Doc67ZaprosSkol doc67ZaprosSkol;
	if (filter_DI[DI_ZaprosSkol67] ==0) return -1;
	memset(&doc67ZaprosSkol,0,sizeof(doc67ZaprosSkol));
	doc67ZaprosSkol.lineSkol = skolToGo.lineSkol;
	doc67ZaprosSkol.modeSkol = skolToGo.modeSkol;
	doc67ZaprosSkol.pelengSkol = skolToGo.pelengSkol;
	doc67ZaprosSkol.distanceSkol = skolToGo.distanceSkol;
	sendDocMsg( DI_ZaprosSkol67, &doc67ZaprosSkol);
	return 1;
}

int sendDocCommandX_Strob// DocCommandX_Strob
	(byte * filter_DI, CommandX_Strob commandX_Strob)
{
	DocCommandX_Strob doccommandX_Strob;
	if (filter_DI[DI_COMMANDX_STROB_CODE] ==0) return -1;
	memset(&doccommandX_Strob,0,sizeof(doccommandX_Strob));
	memcpy(&doccommandX_Strob,&commandX_Strob,sizeof(doccommandX_Strob));
	sendDocMsg( DI_COMMANDX_STROB_CODE, &doccommandX_Strob);
}
int sendDocCommandX_ChangePri// DocCommandX_ChangePri
	(byte * filter_DI, CommandX_ChangePri commandX_ChangePri)
{
	DocCommandX_ChangePri docCommandX_ChangePri;
	if (filter_DI[DI_COMMANDX_CHANGEPRI_CODE] ==0) return -1;
	memset(&docCommandX_ChangePri,0,sizeof(docCommandX_ChangePri));
	memcpy(&docCommandX_ChangePri,&commandX_ChangePri,sizeof(CommandX_ChangePri));
	sendDocMsg( DI_COMMANDX_CHANGEPRI_CODE, &docCommandX_ChangePri);
}
int sendDocResult_CommandX// DocResult_CommandX
	(byte * filter_DI, Result_CommandX result_CommandX)
{
	DocResult_CommandX docResult_CommandX;
	if (filter_DI[DI_RESULT_COMMANDX_CODE] ==0) return -1;
	memset(&docResult_CommandX,0,sizeof(docResult_CommandX));
	memcpy(&docResult_CommandX,&result_CommandX,sizeof(Result_CommandX));
	sendDocMsg( DI_RESULT_COMMANDX_CODE, &docResult_CommandX);

}
int sendDocEMS_SU// DocEMS_SU
	(byte * filter_DI, EMS_SU ems_SU)
{
	DocEMS_SU docEMS_SU;
	if (filter_DI[DI_EMS_SU] ==0) return -1;
	memset(&docEMS_SU,0,sizeof(docEMS_SU));
	memcpy(&docEMS_SU,&ems_SU,sizeof(EMS_SU));
	sendDocMsg( DI_EMS_SU, &docEMS_SU);

}
int sendDocEMS_FURKE// DocEMS_FURKE
	(byte * filter_DI, EMS_FURKE ems_FURKE)
{
	DocEMS_FURKE docEMS_FURKE;
	if (filter_DI[DI_EMS_FURKE] ==0) return -1;
	memset(&docEMS_FURKE,0,sizeof(docEMS_FURKE));
	memcpy(&docEMS_FURKE,&ems_FURKE,sizeof(EMS_FURKE));
	sendDocMsg( DI_EMS_FURKE, &docEMS_FURKE);
}
int sendDocTargetX_m// DocTargetX_m
	(byte * filter_DI, TargetX_m targetX_m)
{
	DocTargetX_m docTargetX_m;
	if (filter_DI[DI_TARGETX_CODE] ==0) return -1;
	memset(&docTargetX_m,0,sizeof(docTargetX_m));
	memcpy(&docTargetX_m,&targetX_m,sizeof(TargetX_m));
	sendDocMsg( DI_TARGETX_CODE, &docTargetX_m);
}
