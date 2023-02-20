#ifndef FURKE2DOC_H_
#define FURKE2DOC_H_

#include <formular.h>
#include <messages.h>
#include <docStructs.h>
#include <stateRec.h>

int sendDocFormularTrassa
    (byte * filter_DI, FORMULAR_TRASSA_STRUCT ft, 
    		       bool b3K, word time3k96);
int sendDocIP3_Puma
	(byte * filter_DI, int ind, IP3_Puma * ip3_Puma);
int sendDocOurShipGeo
	(OurShipGeo OurShipGeo);
int sendDocTime
	(unsigned int timeSigma,unsigned int timeBOS,word time3K96);

int sendDocDev67Alarm
	(byte * filter_DI, Dev67Alarm_t al);

int sendDocDev67Tgt
	(byte * filter_DI, Dev67Tgt_t tgt, word line, word mode, word numberRlk);

int sendDoc67Zapros// типа Doc67Zapros
	(byte * filter_DI, SR_target sr_target);
int sendDocNewZapros67// типа Doc67Zapros
	(byte * filter_DI,SR_target sr_target);
int sendDocSwitchOff67// типа Doc67Zapros
	(byte * filter_DI, word numberRlk, word numberX, word numberSfera);
int sendDocDeleteZapros// типа Doc67Zapros
	(byte * filter_DI, SR_target sr_target,word parameter);
int sendDocHandGO// типа DocHandGO
	(byte * filter_DI, word numberRlk, word own, word bX);
int sendDocResult_5P27// типа DocHandGO
	(byte * filter_DI, word numberRlk, word own);
int sendDocResult_5P20// типа DocDev67Tgt
	(byte * filter_DI, Dev67Tgt_t tgt, 
	 word line, word mode, word number5P20);

int sendDocTaskTarget//DocTaskTarget;
	(byte * filter_DI, word numberRlk, word numberShootChannel);
int sendDocNullShootChannel//DocNullShootChannel;
	(byte * filter_DI, word numberShootChannel, word parameter);
int sendDocResetShootChannel//DocResetShootChannel
	(byte * filter_DI, word numberShootChannel, word parameter);

int sendDocSendRli// SendRli
	(byte * filter_DI, word numberRlk);
int sendDocSendRedut// SendRedut
	(byte * filter_DI);
int sendDocRVVRedut// DocRVVRedut
	(byte * filter_DI, word numberRlk, word numberShootChannel,word RVV);
int sendDocOPERATOR_DIRECTIVE// DocOPERATOR_DIRECTIVE_STRUCT2
	(byte * filter_DI, OPERATOR_DIRECTIVE_STRUCT od);
int sendDocANSWER_BOS_OPERATOR// DocANSWER_BOS_OPERATOR_STRUCT2
	(byte * filter_DI, ANSWER_BOS_OPERATOR_STRUCT answer_od);
int sendDocADAPTATION_CONTROL// CODOGRAMM_ADAPTATION_CONTROL
	(byte * filter_DI, CODOGRAMM_ADAPTATION_CONTROL_STRUCT ac);
int sendDocANSWER_ADAPTATION_CONTROL// ANSWER_ADAPTATION_CONTROL
	(byte * filter_DI, ANSWER_ADAPTATION_CONTROL_STRUCT answer_ac);
int sendSTATE_BOS// DocCODOGRAMM_GENERAL_MODES_STRUCT
	(byte * filter_DI, CODOGRAMM_GENERAL_MODES_STRUCT state_bos);
int sendMainDoc// DocMainDoc
	(byte * filter_DI, MainDoc mainDoc);
int sendPAV_OK// DocPAV_OK_STRUCT
	(byte * filter_DI, PAV_OK_STRUCT pav_ok);
int sendDoc67ZaprosSkol// типа Doc67ZaprosSkol
	(byte * filter_DI,SkolToGo skolToGo);

int sendDocCommandX_Strob// DocCommandX_Strob
	(byte * filter_DI, CommandX_Strob commandX_Strob);
int sendDocCommandX_ChangePri// DocCommandX_ChangePri
	(byte * filter_DI, CommandX_ChangePri commandX_ChangePri);
int sendDocResult_CommandX// DocResult_CommandX
	(byte * filter_DI, Result_CommandX result_CommandX);
int sendDocEMS_SU// DocEMS_SU
	(byte * filter_DI, EMS_SU ems_SU);
int sendDocEMS_FURKE// DocEMS_FURKE
	(byte * filter_DI, EMS_FURKE ems_FURKE);
int sendDocTargetX_m// DocTargetX_m
	(byte * filter_DI, TargetX_m targetX_m);
int sendDocTaskTargetAll//DocTaskTargetAll;
	(byte * filter_DI, word numberEco,word numberX,word numberFurke, word numberShootChannel);





#endif /*FURKE2DOC_H_*/
