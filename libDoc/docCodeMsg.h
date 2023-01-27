#ifndef DOCCODEMSG_H_
#define DOCCODEMSG_H_

#define COUNT_DI 100 
 

#define DI_FORMULAR_TRASSA 0 // типа DocFormularTrassa
#define DI_IP3_Puma 1		// типа DocIP3_Puma
#define DI_OURSHIPGEO 2		// типа DocOurShipGeo
#define DI_TIME 3			// типа DocTime

#define DI_Dev67Alarm 4 // типа DocDev67Alarm;
#define DI_Dev67Tgt 5 // типа DocDev67Tgt; // цель обнаружена 67 изделием + отожденствлена РЛК
#define DI_DeleteZapros 6 // типа Doc67Zapros; // удаление запроса из очереди запросов
#define DI_SwitchOff67 7 // типа Doc67Zapros; // выключение запросчика+сброс цели с опознавания 
#define DI_NewZapros67 8 //типа Doc67Zapros новый запрос по цели 
#define DI_HandGO 9 // типа DocHandGO назначение признака ГП
#define DI_Zapros67 10 //типа Doc67Zapros запрос в ПА режиме по цели
#define DI_Result_5P27 11 //типа DocHandGo результат ГО: присвоение признака ГП
#define DI_Result_5P20 12 //типа DocDev67Tgt ответ по цели 

#define DI_TaskTarget 20 // DocTaskTarget;// задание цели
#define DI_NullShootChannel 21 // типа DocNullShootChannel  сброс канала
#define DI_ResetShootChannel 22// типа DocResetShootChannel; // сброс канала

#define DI_SendRli 30// типа DocSendRli; // выдача в канал РЛИ
#define DI_SendRedut 31// типа DocSendRedut; // выдача в канал Редута
#define DI_RVVRedut 32 // типа DocRVVRedut; // запрос о сопровождении с повышенной точностью
#define DI_OPERATOR_DIRECTIVE 33 // типа DocOPERATOR_DIRECTIVE_STRUCT2,3; TS=1 
// команда на ЗАЗ, ПА, СБР, ПРиор, РТЦ, Высоты, Мирный, Порога для НЦ
#define DI_ANSWER_BOS_OPERATOR 34// типа DocANSWER_BOS_OPERATOR_STRUCT2,3 ,TS=10 
// квитанция на DI_OPERATOR_DIRECTIVE
#define DI_ADAPTATION_CONTROL 35 // типа DocCODOGRAMM_ADAPTATION_CONTROL_STRUCT; ts=3 
// команда на Режимы, скорости, частоты, Дкгк Двару по кругу, ЛЧМ, МДФА
#define DI_ANSWER_ADAPTATION_CONTROL 36// типа DocANSWER_ADAPTATION_CONTROL_STRUCT; ts=17 
// квитанция на DI_ADAPTATION_CONTROL
#define DI_STATE_BOS 37// типа DocCODOGRAMM_GENERAL_MODES_STRUCT; ts=81 
#define DI_MAINDOC 38//типа DocMainDoc
#define DI_PAV_OK 39 // типа DocPAV_OK_STRUCT; ts=51
#define DI_CODOGRAMM_ADAPTATION_CONTROL_PROTECT 40 // типа DocCODOGRAMM_ADAPTATION_CONTROL_PROTECT_STRUCT_f4; ts=22
#define DI_ANSWER_SPZ_ADAPTATION_CONTROL 41 // типа DocANSWER_SPZ_ADAPTATION_CONTROL_STRUCT_f4; ts=23
#define DI_CODOGRAMM_SECTOR_CANCEL_LOCATOR 42 // типа DocCODOGRAMM_SECTOR_CANCEL_LOCATOR_STRUCT_f4; ts=4
#define DI_ANSWER_NZI_ADAPTATION_CONTROL 43 // типа DocANSWER_NZI_ADAPTATION_CONTROL_STRUCT_f4; ts=19
#define DI_OURSHIPGEOEX 44		// типа DocOurShipGeoEx
#define DI_ZaprosSkol67 45 		//типа Doc67ZaprosSkol запрос по сколу
#define DI_COMMANDX_STROB_CODE 46// типа DocCommandX_Strob команды по поиск в заданном стробе
#define DI_COMMANDX_CHANGEPRI_CODE 47// типа DocCommandX_ChangePri команда на изменение приоритета
#define DI_RESULT_COMMANDX_CODE 48 // типа DocResult_CommandX ответ на команду 
#define DI_EMS_SU 49 // типа DocEMS_SU 2000
#define DI_EMS_FURKE 50// типа DocEMS_FURKE 2001
#define DI_TARGETX_CODE 51 // типа DocTargetX_m

#define DI_TARGETTELEVIK 71 // типа  DocTargetTelevik
#define DI_TARGETSERVAL 72 // типа  DocTargetServal
#define DI_STATESERVAL 73 // типа  DocStateServal
#define DI_STATEGLOBALTELEVIK 74 // типа  DocStateGlobalTelevik

#define DI_TARGETPRISTAVKA 75 // типа  DocTargetPristavka
#define DI_STATEPRISTAVKA 76 // типа  DocStatePristavka
#define DI_STATEGLOBALISTRA 77 // типа  DocStateGlobalIstra
#define DI_TARGETISTRA 78 // типа  DocTargetTelevik
#define DI_TaskTargetAll 99 // типа  DocTaskTargetAll
 
#endif /*DOCCODEMSG_H_*/