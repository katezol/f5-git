#ifndef DOCVIEWPARSER_H_
#define DOCVIEWPARSER_H_

#include "docStructs.h"

void data2viewFormularTrassa(DocFormularTrassa* p, char *str);
void data2viewIP3_Puma(DocIP3_Puma* p, char *str);
void data2viewOurShipGeo(DocOurShipGeo* p, char* str);
void data2viewDoc67Zapros(int type, Doc67Zapros* p, char *str);
void data2viewDev67tgt(DocDev67Tgt* p, char *str);
void data2viewTime(DocTime* p, char* str);
void data2viewANSWER_BOS_OPERATOR(DocANSWER_BOS_OPERATOR_STRUCT3* p, char *str);
void data2viewOPERATOR_DIRECTIVE(DocOPERATOR_DIRECTIVE_STRUCT2* p, char *str);
void data2viewADAPTATION_CONTROL(DocCODOGRAMM_ADAPTATION_CONTROL_STRUCT* p, char *str);
void data2viewANSWER_ADAPTATION_CONTROL(DocANSWER_ADAPTATION_CONTROL_STRUCT* p, char *str);
void data2viewSTATE_BOS(DocCODOGRAMM_GENERAL_MODES_STRUCT* p, char* str);
void data2viewMainDoc(DocMainDoc* p, char* str);
void data2viewPAV_OK(DocPAV_OK_STRUCT* p, char* str);
void data2viewCommandX_Strob(DocCommandX_Strob* p, char* str);
void data2viewCommandX_ChangePri(DocCommandX_ChangePri* p, char* str);
void data2viewResult_CommandX(DocResult_CommandX* p, char* str);
void data2viewEMS_SU(DocEMS_SU* p, char* str);
void data2viewEMS_FURKE(DocEMS_FURKE* p, char* str);
void data2viewTargetX_m(DocTargetX_m* p, char* str, unsigned char bHEth);
void data2viewTargetTelevik(DocTargetTelevik* p, int bParamPD, int  bParamVr,char* str);
void data2viewTargetServal(DocTargetServal* p, int bParamPD, int  bParamVr, char* str);

#endif /*DOCVIEWPARSER_H_*/
