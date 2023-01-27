/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

/* Standard headers */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
/* Local headers */

#include "global.h"
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"
#include <errno.h>

#include "docCodeMsg.h"
#include "docStructs.h"
#include "filter.h"
#include "math.h"

#define NEW_FRAME_STRUCT


///todo - move to libDoc (docStructs.h)

#define DOC_TMP_FILE_PATH			"/tmp/tmp.log"
#define DOC_TMP_COMPLETE_FILE_PATH	"/tmp/tmp_complete.log"

#define Pt_ITEM_TRUE Pt_SELECTABLE|Pt_HIGHLIGHTED
#define Pt_ITEM_FALSE Pt_HIGHLIGHTED|Pt_GHOST


typedef struct DOC_FILE_STRUCT_S{
	char filePath[200];
	uint32_t msgCount;
	uint32_t startTime;
	uint32_t stopTime;
}DocFileStruct_t;

int docMsgCount(const int fd);
int docGetStartStopTime(DocFileStruct_t *docFile);

///todo - move to libDoc   END

PtListCallback_t 	selectedListStuct;
PtListCallback_t 	selectedMergeListStuct;



struct{
	volatile int 		fd;
	uint32_t			pos; //filtered pos (showFrame)
	DocFileStruct_t		docFileStruct;
}logFileStr;

enum enumBool {eFalse=0, eTrue=1};
enum enumSplitMergeButtonState {eNone, eMerge, eSplit};
enum enumSplitMergeButtonState	curSplitMergeButtonState = eNone;

void changeBlockState(PtWidget_t *widget, enum enumBool blockFlag);
void refresh();


//struct fileList fileList[12];
//int posSellect;
int curPos;
int fileSellect;
int readCount;
//char logName[100][100];
long int curPosFile=0;

int showFrameCount = 0;
unsigned int *showFrame;

int readLogFileCount=0;
#ifdef NEW_FRAME_STRUCT
	doc_frame_t	*DFrame;
#else
	struct frame* frame;
	int logFile;
	unsigned int logFilePos=0;
	int msgCount = 0;
#endif

char playViewDoc = 0;


char _sliderViewActive = 0;

#define bPLAY_STOP		0 // 0 - не идет проигрывание
#define bPLAY_PLAY		1 // 1 - проигрывание продолжается
#define bPLAY_NEW_PLAY	2 // 2 - новое проигрывание

// проигрывание документирования
char bPlay = bPLAY_STOP;

unsigned int pauseTime; // Время паузы
unsigned int startPlayTime;

extern int bParamPD, bParamVr, shipType;
float viewDo2pi(float value);

// Клавиатура
struct keyboardStruct{
	char keyboardNumb[10];
	int keyboard_MIN;
	int keyboard_MAX;
	PtWidget_t* parentKeyWidget;
}keyboardStruct;
//------------

// TimeScroll
struct timeScroll{
	PtWidget_t* wgt;
	unsigned int scrollMin;
	unsigned int scrollMax;
	unsigned int timeFrame[300];
	unsigned int area;
	double perCount;
	int timeStart;
	int timeEnd;
}timeScroll;

PtBlockedList_t *paneBlock;


void setScrollViewLabels(unsigned int _timeStart, unsigned int _timeEnd){

#ifndef NEW_FRAME_STRUCT
	char str[100];

	sprintf(str,"%d:%d:%d.%d",
			_timeStart/360000, (_timeStart/6000)%60,
					(_timeStart/100)%60,  (_timeStart%100)/10);
	PtSetResource(ABW_labelViewStartTime, Pt_ARG_TEXT_STRING, str, 0);
	
	
	sprintf(str,"%d:%d:%d.%d",
				_timeEnd/360000, (_timeEnd/6000)%60,
						(_timeEnd/100)%60,  (_timeEnd%100)/10);
	PtSetResource(ABW_labelViewEndTime, Pt_ARG_TEXT_STRING, str, 0);
#else

	char str[100];

	timeConverter(str, _timeStart, NULL);
	PtSetResource(ABW_labelViewStartTime, Pt_ARG_TEXT_STRING, str, 0);

	timeConverter(str, _timeEnd, NULL);
	PtSetResource(ABW_labelViewEndTime, Pt_ARG_TEXT_STRING, str, 0);
#endif
}

#ifndef NEW_FRAME_STRUCT

int getFramePosFromTime(unsigned int _time){
	int i;
	int testTime;


	//printf("showFrameCount = %d\n", showFrameCount);
	for(i=0; i<showFrameCount; i++){
		memcpy(&testTime, frame[showFrame[i]].data, sizeof(int));
		//printf("testTime = %d\n", testTime);
		if(testTime>=_time)
			break;
	}

	if(i>0)
		i--;
	return i;
}

#else

int getFramePosFromTime(unsigned int _time){
	int i;
	int testTime;

	if(_time <= logFileStr.docFileStruct.startTime){
		return 0;
	}else if(_time >= logFileStr.docFileStruct.stopTime){
		return showFrameCount;
	}else{
		int left = 0;
		int right = showFrameCount;
		int mid = 0;

		while ((right-left) > 20){
			mid = left + (right - left) / 2;

			if (_time == DFrame[showFrame[mid]].time){
				break;
			}else{
				if (DFrame[showFrame[mid]].time > _time){
					right = mid - 1;
				}else{
					left = mid + 1;
				}
			}
		}
		for(i=left; i < right; i++){
			testTime = DFrame[showFrame[i]].time;
			if(DFrame[showFrame[i]].time >= _time){
				if(DFrame[showFrame[i]].time == _time){
					break;
				}else{
					if(i>0){
						i--;
					}
					break;
				}
			}
		}
	}

	return i;
}
#endif

// Установка позиции скролл в панели отрисовки
void setScrollView(unsigned int curTime){
	unsigned int t1 = timeScroll.timeEnd - timeScroll.timeStart;
	double perc = (double)t1/100.0;
	t1 = curTime - timeScroll.timeStart;
	t1 = (double)t1/perc;
	PtSetResource(ABW_sliderView, Pt_ARG_GAUGE_VALUE, t1,0);
}

#ifndef NEW_FRAME_STRUCT
// Установка значений timeScroll
void setTimeScroll(){
	unsigned long *size;
	char str[256];
	timeScroll.wgt = ABW_docSlideTime;
	if(showFrameCount!=0){
		memcpy(&timeScroll.timeStart, frame[showFrame[0]].data, sizeof(int));
		memcpy(&timeScroll.timeEnd,
				frame[showFrame[showFrameCount-1]].data, sizeof(int));
		PtGetResource(ABW_docSlideTime, Pt_ARG_GAUGE_MINIMUM, &size, 0);
		timeScroll.scrollMin = *size;
		PtGetResource(ABW_docSlideTime, Pt_ARG_GAUGE_MAXIMUM, &size, 0);
		timeScroll.scrollMax = *size;
		timeScroll.area = timeScroll.scrollMax - timeScroll.scrollMin;
		timeScroll.perCount = (timeScroll.timeEnd - timeScroll.timeStart);
		timeScroll.perCount /= (double)timeScroll.area;
		memset(timeScroll.timeFrame, 0, 300*sizeof(int));
		// Время начала вермя конца
		//tm = localtime(&timeScroll.timeStart);
		sprintf(str,"%d:%d:%d.%d",
				timeScroll.timeStart/360000, (timeScroll.timeStart/6000)%60,
				(timeScroll.timeStart/100)%60, (timeScroll.timeStart%100)/10);
		PtSetResource(ABW_docTimeStart, Pt_ARG_TEXT_STRING, str, 0);
		//
		sprintf(str,"%d:%d:%d.%d",
				timeScroll.timeEnd/360000, (timeScroll.timeEnd/6000)%60,
				(timeScroll.timeEnd/100)%60,  (timeScroll.timeEnd%100)/10);
		PtSetResource(ABW_docTimeEnd, Pt_ARG_TEXT_STRING, str, 0);
		setScrollViewLabels(timeScroll.timeStart, timeScroll.timeEnd);
	}else{
		PtSetResource(ABW_docTimeStart, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_docTimeEnd, Pt_ARG_TEXT_STRING, "", 0);
	}
}
#else
// Установка значений timeScroll
void setTimeScroll(){
	unsigned long *size;
	char str[256];
	timeScroll.wgt = ABW_docSlideTime;
	if(showFrameCount!=0){
		timeScroll.timeStart = DFrame[showFrame[0]].time;
		timeScroll.timeEnd = DFrame[showFrame[showFrameCount-1]].time;

		PtGetResource(ABW_docSlideTime, Pt_ARG_GAUGE_MINIMUM, &size, 0);
		timeScroll.scrollMin = *size;
		PtGetResource(ABW_docSlideTime, Pt_ARG_GAUGE_MAXIMUM, &size, 0);
		timeScroll.scrollMax = *size;
		timeScroll.area = timeScroll.scrollMax - timeScroll.scrollMin;
		timeScroll.perCount = (timeScroll.timeEnd - timeScroll.timeStart);
		timeScroll.perCount /= (double)timeScroll.area;
		memset(timeScroll.timeFrame, 0, 300*sizeof(int));
		// Время начала вермя конца
		//tm = localtime(&timeScroll.timeStart);

		timeConverter(str, timeScroll.timeStart, NULL);
		PtSetResource(ABW_docTimeStart, Pt_ARG_TEXT_STRING, str, 0);
		//
		timeConverter(str, timeScroll.timeEnd, NULL);
		PtSetResource(ABW_docTimeEnd, Pt_ARG_TEXT_STRING, str, 0);
		setScrollViewLabels(timeScroll.timeStart, timeScroll.timeEnd);
	}else{
		PtSetResource(ABW_docTimeStart, Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(ABW_docTimeEnd, Pt_ARG_TEXT_STRING, "", 0);
	}
}
#endif

// Считывание фильтра
void readFilter(void){
	int i;
	unsigned int *flags;
	unsigned short *sel;
	char **items;
	
	// str0
	PtGetResource(ABW_filterShow0, Pt_ARG_FLAGS, &flags, 0);
	filterStr0.show = (*flags)&Pt_SET?1:0;
	PtGetResource(ABW_filterShow01, Pt_ARG_FLAGS, &flags, 0);
	filterStr0.pr3k96 = (*flags)&Pt_SET?1:0;
	PtGetResource(ABW_filterTc0, Pt_ARG_CBOX_SEL_ITEM, &sel, 0);
	filterStr0.TC = *sel;
	PtGetResource(ABW_filterGp0, Pt_ARG_CBOX_SEL_ITEM, &sel, 0);
	filterStr0.SV = *sel;
	
	PtGetResource(ABW_filterNt0, Pt_ARG_LIST_ITEM_COUNT , &sel, 0);
	if(*sel==0)
		memset(&filterStr0.nSource, 1, sizeof(unsigned char)*MAX_ARR_NUMB);
	else{
		memset(&filterStr0.nSource, 0, sizeof(unsigned char)*MAX_ARR_NUMB);
		PtGetResource(ABW_filterNt0, Pt_ARG_ITEMS , &items, 0);
		for(i=0; i<*sel; i++)
			filterStr0.nSource[atoi(items[i])] = 1;
	}
	
	//str1
	PtGetResource(ABW_filterShow1, Pt_ARG_FLAGS, &flags, 0);
	filterStr1.show = (*flags)&Pt_SET?1:0;
	PtGetResource(ABW_filterNrls1, Pt_ARG_LIST_ITEM_COUNT , &sel, 0);
	if(*sel==0)
		memset(&filterStr1.nRls, 1, sizeof(unsigned char)*MAX_ARR_NUMB);
	else{
		memset(&filterStr1.nRls, 0, sizeof(unsigned char)*MAX_ARR_NUMB);
		PtGetResource(ABW_filterNrls1, Pt_ARG_ITEMS , &items, 0);
		for(i=0; i<*sel; i++)
			filterStr1.nRls[atoi(items[i])] = 1;
	}

	PtGetResource(ABW_filterNpuma1, Pt_ARG_LIST_ITEM_COUNT , &sel, 0);
	if(*sel==0)
		memset(&filterStr1.nPuma, 1, sizeof(unsigned char)*MAX_ARR_NUMB);
	else{
		memset(&filterStr1.nPuma, 0, sizeof(unsigned char)*MAX_ARR_NUMB);
		PtGetResource(ABW_filterNpuma1, Pt_ARG_ITEMS , &items, 0);
		for(i=0; i<*sel; i++)
			filterStr1.nPuma[atoi(items[i])] = 1;
	}
	
	PtGetResource(ABW_filterGp1, Pt_ARG_CBOX_SEL_ITEM, &sel, Pt_TRUE);
	filterStr1.prn =*sel;
	PtGetResource(ABW_filterTc1, Pt_ARG_CBOX_SEL_ITEM, &sel, Pt_TRUE);
	filterStr1.objType =*sel;
	// str2
	PtGetResource(ABW_filterShow2, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_OURSHIPGEO].show = (*flags)&Pt_SET?1:0;
	// str3
	PtGetResource(ABW_filterShow3, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_TIME].show = (*flags)&Pt_SET?1:0;
	// str4
	PtGetResource(ABW_filterShow4, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_Dev67Alarm].show = (*flags)&Pt_SET?1:0;
	// str5
	PtGetResource(ABW_filterShow5, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_Dev67Tgt].show = (*flags)&Pt_SET?1:0;
	// str6
	PtGetResource(ABW_filterShow6, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_DeleteZapros].show = (*flags)&Pt_SET?1:0;
	// str7
	PtGetResource(ABW_filterShow7, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_SwitchOff67].show = (*flags)&Pt_SET?1:0;
	// str8
	PtGetResource(ABW_filterShow8, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_NewZapros67].show = (*flags)&Pt_SET?1:0;
	// str9
	PtGetResource(ABW_filterShow9, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_HandGO].show = (*flags)&Pt_SET?1:0;
	// str10
	PtGetResource(ABW_filterShow10, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_Zapros67].show = (*flags)&Pt_SET?1:0;
	// str11
	PtGetResource(ABW_filterShow11, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_Result_5P27].show = (*flags)&Pt_SET?1:0;
	// str12
	PtGetResource(ABW_filterShow12, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_Result_5P20].show = (*flags)&Pt_SET?1:0;
	// str13
	PtGetResource(ABW_filterShow13, Pt_ARG_FLAGS, &flags, 0);
	
	filterStr[DI_TaskTarget].show = (*flags)&Pt_SET?1:0;
	filterStr[DI_TaskTargetAll].show = filterStr[DI_TaskTarget].show;
	// str14
	PtGetResource(ABW_filterShow14, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_NullShootChannel].show = (*flags)&Pt_SET?1:0;
	// str15
	PtGetResource(ABW_filterShow15, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_ResetShootChannel].show = (*flags)&Pt_SET?1:0;
	//str16
	PtGetResource(ABW_filterShow16, Pt_ARG_FLAGS, &flags, 0);
	fSendRli.show = (*flags)&Pt_SET?1:0;
	
	PtGetResource(ABW_filterSendRli, Pt_ARG_LIST_ITEM_COUNT , &sel, 0);
	if(*sel==0)
		memset(&fSendRli.nSource, 1, sizeof(unsigned char)*MAX_ARR_NUMB);
	else{
		memset(&fSendRli.nSource, 0, sizeof(unsigned char)*MAX_ARR_NUMB);
		PtGetResource(ABW_filterSendRli, Pt_ARG_ITEMS , &items, 0);
		for(i=0; i<*sel; i++)
			fSendRli.nSource[atoi(items[i])] = 1;
	}
	//str17
	PtGetResource(ABW_filterShow17, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_SendRedut].show = (*flags)&Pt_SET?1:0;
	//str18
	PtGetResource(ABW_filterShow18, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_OPERATOR_DIRECTIVE].show = (*flags)&Pt_SET?1:0;
	//str19
	PtGetResource(ABW_filterShow19, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_ANSWER_BOS_OPERATOR].show = (*flags)&Pt_SET?1:0;
	//str20
	PtGetResource(ABW_filterShow20, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_ADAPTATION_CONTROL].show = (*flags)&Pt_SET?1:0;
	//str21
	PtGetResource(ABW_filterShow21, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_ANSWER_ADAPTATION_CONTROL].show = (*flags)&Pt_SET?1:0;
	//str22
	PtGetResource(ABW_filterShow22, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_STATE_BOS].show = (*flags)&Pt_SET?1:0;
	//str23
	PtGetResource(ABW_filterShow23, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_MAINDOC].show = (*flags)&Pt_SET?1:0;
	filterStr[DI_PAV_OK].show = (*flags)&Pt_SET?1:0;

	PtGetResource(ABW_filterX, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_COMMANDX_STROB_CODE].show = (*flags)&Pt_SET?1:0;
	filterStr[DI_COMMANDX_CHANGEPRI_CODE].show = (*flags)&Pt_SET?1:0;
	filterStr[DI_RESULT_COMMANDX_CODE].show = (*flags)&Pt_SET?1:0;
	filterStr[DI_EMS_SU].show = (*flags)&Pt_SET?1:0;
	filterStr[DI_EMS_FURKE].show = (*flags)&Pt_SET?1:0;
	
	//str1
	PtGetResource(ABW_filter_TargetX, Pt_ARG_FLAGS, &flags, 0);
	filterStrX.show = (*flags)&Pt_SET?1:0;
	PtGetResource(ABW_filterNtX, Pt_ARG_LIST_ITEM_COUNT , &sel, 0);
	if(*sel==0)
		memset(&filterStrX.nX, 0, sizeof(unsigned int)*MAX_ARR_NUMB);
	else{
		memset(&filterStrX.nX, 0, sizeof(unsigned int)*MAX_ARR_NUMB);
		PtGetResource(ABW_filterNtX, Pt_ARG_ITEMS , &items, 0);
		for(i=0; i<*sel; i++)
			//filterStrX.nX[atoi(items[i])] = 1;
			filterStrX.nX[i] = atoi(items[i]);
	}
	
	
	PtGetResource(ABW_filter_TargetServal, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_TARGETSERVAL].show = (*flags)&Pt_SET?1:0;
	
	PtGetResource(ABW_filter_StateServal, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_STATESERVAL].show = (*flags)&Pt_SET?1:0;
	
	PtGetResource(ABW_filter_StateTelevik, Pt_ARG_FLAGS, &flags, 0);
	filterStr[DI_STATEGLOBALTELEVIK].show = (*flags)&Pt_SET?1:0;

	//	PtGetResource(ABW_filter_TargetX, Pt_ARG_FLAGS, &flags, 0);
//	filterStr[DI_TARGETX_CODE].show = (*flags)&Pt_SET?1:0;
	
}

#ifdef NEW_FRAME_STRUCT
//// Вывод фрейма
void printFrame(int position){
	int dataTime = 0;
	int timePos;
	int i,j;
	//unsigned char typeMsg;
	char str[256];
	//char tempStr[256];
	for(i = position, j = 0; j < READ_COUNT && i < showFrameCount; i++){
		memset(str,0, sizeof(str));//strcpy(str,"");
		parseDocMsg(DFrame[showFrame[i]].type, DFrame[showFrame[i]].data, str,bParamPD,bParamVr,shipType);

		//strcpy(str, getStrMsg(typeMsg, 0, tempStr));

		PtSetResource(AbGetABW(ABN_docTableMsg1+j), Pt_ARG_TEXT_STRING, str, 0);
		// Вывод времени
		dataTime = DFrame[showFrame[i]].time;
		timeConverter(str, dataTime, NULL);
		PtSetResource(AbGetABW(ABN_docTableTime1+j), Pt_ARG_TEXT_STRING, str, 0);
		j++;
	}
	for(i = j; i < READ_COUNT; i++){
		PtSetResource(AbGetABW(ABN_docTableMsg1+i),	Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(AbGetABW(ABN_docTableTime1+i), Pt_ARG_TEXT_STRING, "", 0);
	}

	// Время первой строки в списке
	if(showFrameCount!=0){
		dataTime = DFrame[showFrame[position]].time;
		timeConverter(str, dataTime, NULL);
		PtSetResource(ABW_docCurTime, Pt_ARG_TEXT_STRING, str, 0);
	}else{
		PtSetResource(ABW_docCurTime, Pt_ARG_TEXT_STRING, "", 0);
	}
	if(timeScroll.perCount!=0)
		timePos = (double)(dataTime - timeScroll.timeStart)/timeScroll.perCount;
	else
		timePos = 0;

	PtSetResource(timeScroll.wgt, Pt_ARG_GAUGE_VALUE, timePos,0);
}


#else
// Вывод фрейма
void printFrame(int position){
	int dataTime;
	int timePos;
	int i,j;
	//unsigned char typeMsg;
	char str[256];
	//char tempStr[256];
	for(i=position, j=0; j<READ_COUNT&i<showFrameCount; i++){
		strcpy(str,"");
		//typeMsg = frame[showFrame[i]].data[4];
		parseDocMsg(frame[showFrame[i]].data[4], &frame[showFrame[i]].data[5], str,bParamPD,bParamVr,shipType);
				
		
		//strcpy(str, getStrMsg(typeMsg, 0, tempStr));
		
		PtSetResource(AbGetABW(ABN_docTableMsg1+j),
								Pt_ARG_TEXT_STRING, str, 0);
		// Вывод времени
		memcpy(&dataTime,frame[showFrame[i]].data, sizeof(int));
		sprintf(str,"%d:%d:%d.%d",
					dataTime/360000, (dataTime/6000)%60, 
					(dataTime/100)%60, (dataTime%100)/10);
		PtSetResource(AbGetABW(ABN_docTableTime1+j),
									Pt_ARG_TEXT_STRING, str, 0);
		j++;	
	}
	for(i=j; i<READ_COUNT; i++){
		PtSetResource(AbGetABW(ABN_docTableMsg1+i),
											Pt_ARG_TEXT_STRING, "", 0);
		PtSetResource(AbGetABW(ABN_docTableTime1+i),
											Pt_ARG_TEXT_STRING, "", 0);
	}
	
	// Время первой строки в списке
	if(showFrameCount!=0){
		memcpy(&dataTime,frame[showFrame[position]].data, sizeof(int));
		sprintf(str,"%d:%d:%d.%d",
					dataTime/360000, (dataTime/6000)%60,
					(dataTime/100)%60, (dataTime%100)/10);
		PtSetResource(ABW_docCurTime, Pt_ARG_TEXT_STRING, str, 0);
	}else{
		PtSetResource(ABW_docCurTime, Pt_ARG_TEXT_STRING, "", 0);
	}
	//memcpy(&tempTime, frame[showFrame[*pos]].data, sizeof(int));
	if(timeScroll.perCount!=0)
		timePos = (double)(dataTime - timeScroll.timeStart)/timeScroll.perCount;
	else
		timePos = 0;
	
	PtSetResource(timeScroll.wgt, Pt_ARG_GAUGE_VALUE, timePos,0);
}
#endif


#ifndef NEW_FRAME_STRUCT
// Фильтрация фреймов
void filterFrame(struct frame* frame, unsigned int countFrame){
	int i,j,complete;
	int timePos;
	int tempTime;
	unsigned int timeSigmaCurr;
	PtSetResource((PtWidget_t *)ABW_PtProgressFilter, Pt_ARG_GAUGE_VALUE , 0, 0);
	PtWidgetToFront(ABW_PtPaneFilter);
	timeSigmaCurr =0;
	for(i=0,j=0; i<countFrame; i++){
		//if(frame[i].data[4]==DI_TIME)
		//{timeSigmaCurr = frame[i].data[4]}
		if(frame[i].data[4]==DI_FORMULAR_TRASSA){
			if(filterFormularTrassaStruct2(&frame[i].data[5])==1){
				frame[i].show = 1;
				showFrame[j] = i;
				j++;
			}else{
				frame[i].show = 0;
			}
		}else if(frame[i].data[4]==DI_IP3_Puma){
			if(filterDocIP3Puma(&frame[i].data[5])==1){
				frame[i].show = 1;
				showFrame[j] = i;
				j++;
			}else{
				frame[i].show = 0;
			}
		}else if(frame[i].data[4]==DI_SendRli){
			if(filterSenRli(&frame[i].data[5])==1){
				frame[i].show = 1;
				showFrame[j] = i;
				j++;
			}else{
				frame[i].show = 0;
			}
		}else if(frame[i].data[4]==DI_TARGETX_CODE){
			if(filterTargetXStruct(&frame[i].data[5])==1){
				frame[i].show = 1;
				showFrame[j] = i;
				j++;
			}else{
				frame[i].show = 0;
			}
		}else if(frame[i].data[4]==DI_TARGETTELEVIK){
			if(filterTargetTelevikStruct(&frame[i].data[5])==1){
				frame[i].show = 1;
				showFrame[j] = i;
				j++;
			}else{
				frame[i].show = 0;
			}
		}else if(frame[i].data[4]==DI_TARGETPRISTAVKA){ // телевик с приставкой -используется один фильтр
			if(filterTargetTelevikStruct(&frame[i].data[5])==1){
				frame[i].show = 1;
				showFrame[j] = i;
				j++;
			}else{
				frame[i].show = 0;
			}


		}else{
			if(filterGlobal(frame[i].data[4])){
				frame[i].show = 1;
				showFrame[j] = i;
				j++;
			}else{
				frame[i].show = 0;
			}
		}
		frame[i].show = 0;
		if(i%1000==0){
			complete = (float)i/(float)readLogFileCount*100.0;
			PtSetResource((PtWidget_t *)ABW_PtProgressFilter, Pt_ARG_GAUGE_VALUE , complete, 0);
			PtFlush();
		}
	}
	showFrameCount = j;
	// установка скроллинга времени
	setTimeScroll();

	for(i=0; i<showFrameCount; i++){
		memcpy(&tempTime, frame[showFrame[i]].data, sizeof(int));

		if(timeScroll.perCount!=0)
			timePos = (double)(tempTime - timeScroll.timeStart)/timeScroll.perCount;
		else
			timePos = 0;

		if(timeScroll.timeFrame[timePos]==0)
				timeScroll.timeFrame[timePos] = i;
	}

	//printf("timeFrame\n");
	timePos = 0;
	for(i=0; i<300; i++){
		if(timeScroll.timeFrame[i]!=0)
			timePos = timeScroll.timeFrame[i];
		if(timeScroll.timeFrame[i]==0&&timePos!=0)
			timeScroll.timeFrame[i] = timePos;
	}

	PtSetResource(timeScroll.wgt, Pt_ARG_GAUGE_VALUE, 0,0);

	PtSetResource((PtWidget_t *)AbGetABW(ABN_docSlidePos), Pt_ARG_MAXIMUM , showFrameCount+14-READ_COUNT, 0);
	PtSetResource((PtWidget_t *)AbGetABW(ABN_docSlidePos), Pt_ARG_SCROLL_POSITION , 0, 0);

}
#else

// Фильтрация фреймов
void filterFrame(doc_frame_t* frame, unsigned int countFrame){
	int i,j,complete;
	int timePos;
	int tempTime;
//	unsigned int timeSigmaCurr;
	PtSetResource((PtWidget_t *)ABW_PtProgressFilter, Pt_ARG_GAUGE_VALUE , 0, 0);
	PtWidgetToFront(ABW_PtPaneFilter);
//	timeSigmaCurr =0;
	for(i=0,j=0; i<countFrame; i++){
		//if(frame[i].data[4]==DI_TIME)
		//{timeSigmaCurr = frame[i].data[4]}
		if(frame[i].type == DI_FORMULAR_TRASSA){
			if(filterFormularTrassaStruct2(frame[i].data)==1){
				showFrame[j] = i;
				j++; 
			}
		}else if(frame[i].type == DI_IP3_Puma){
			if(filterDocIP3Puma(frame[i].data)==1){
				showFrame[j] = i;
				j++;
			}
		}else if(frame[i].type == DI_SendRli){
			if(filterSenRli(frame[i].data)==1){
				showFrame[j] = i;
				j++;
			}
		}else if(frame[i].type == DI_TARGETX_CODE){
			if(filterTargetXStruct(frame[i].data)==1){
				showFrame[j] = i;
				j++;
			}
		}else if(frame[i].type == DI_TARGETTELEVIK){
			if(filterTargetTelevikStruct(frame[i].data)==1){
				showFrame[j] = i;
				j++;
			}
		}else if(frame[i].type == DI_TARGETPRISTAVKA){ // телевик с приставкой -используется один фильтр
			if(filterTargetTelevikStruct(frame[i].data)==1){
				showFrame[j] = i;
				j++;
			}
		}else{
			if(filterGlobal(frame[i].type)){
				showFrame[j] = i;
				j++;
			}
		}
		if(i%1000==0){
			complete = (float)i/(float)readLogFileCount*100.0;
			PtSetResource((PtWidget_t *)ABW_PtProgressFilter, Pt_ARG_GAUGE_VALUE , complete, 0);
			PtFlush();
		}
	}
	showFrameCount = j;	
	// установка скроллинга времени
	setTimeScroll();
	
	for(i=0; i<showFrameCount; i++){
		tempTime = frame[showFrame[i]].time;

		if(timeScroll.perCount!=0)
			timePos = (double)(tempTime - timeScroll.timeStart)/timeScroll.perCount;
		else
			timePos = 0;

		if(timeScroll.timeFrame[timePos]==0)
				timeScroll.timeFrame[timePos] = i;
	}
	
	//printf("timeFrame\n");
	timePos = 0;
	for(i=0; i<300; i++){
		if(timeScroll.timeFrame[i]!=0)
			timePos = timeScroll.timeFrame[i];
		if(timeScroll.timeFrame[i]==0&&timePos!=0)
			timeScroll.timeFrame[i] = timePos;
	}
	
	PtSetResource(timeScroll.wgt, Pt_ARG_GAUGE_VALUE, 0,0);
	
	PtSetResource((PtWidget_t *)AbGetABW(ABN_docSlidePos), Pt_ARG_MAXIMUM , showFrameCount+14-READ_COUNT, 0);
	PtSetResource((PtWidget_t *)AbGetABW(ABN_docSlidePos), Pt_ARG_SCROLL_POSITION , 0, 0);
			
}
#endif

#ifdef NEW_FRAME_STRUCT
void freeMem(){
	int i=0;
	if(DFrame != NULL){
		for(;i < logFileStr.docFileStruct.msgCount; i++){
			free(DFrame->data);
			DFrame->data = NULL;
		}
		free(DFrame);
		DFrame = NULL;
	}
	if(showFrame != NULL){
		free(showFrame);
		showFrame = NULL;
	}
}
// Прочтение документа
void readList(int fd){
	unsigned int test_lastTime, test_curTime,doptime;
	int i, k,size, complete;
//	int nCorrected;
	unsigned char sch;
	unsigned int curDataTime, lastDataTime;
	long int seekPosDif = 0;
	PtWidgetToFront(AbGetABW( ABN_PtPaneLoadLog ));
	PtSetResource((PtWidget_t *)AbGetABW( ABN_PtProgressLoadLog ), Pt_ARG_GAUGE_VALUE , 0, 0);
	lastDataTime = 0;
	test_lastTime =0;
//	nCorrected =0;

	///////////////////
	DOC_SIZE_t msgSize = 0;
//	uint8_t buff[200];
	uint32_t msgBuffSize = 0;
	/////////////////

	for(i=0; i < logFileStr.docFileStruct.msgCount; i++){
		size = read(fd, &DFrame[i].size, sizeof(DOC_SIZE_t));
		seekPosDif+=size;
		size = read(fd, &DFrame[i].time, sizeof(DOC_TIME_t));
		seekPosDif+=size;
		size = read(fd, &DFrame[i].type, sizeof(DOC_TYPE_t));
		seekPosDif+=size;

		msgBuffSize = DFrame[i].size - sizeof(DOC_TIME_t) - sizeof(DOC_TYPE_t);

		DFrame[i].data = (uint8_t *)calloc( msgBuffSize, sizeof(uint8_t) );
		if(DFrame[i].data != NULL){
			size = read(fd, DFrame[i].data, msgBuffSize);
		}else{
			printf("Can't allocate memory for frame[%d].data!\n", i);
			fflush(stdout);
			i=i-1;
			break;
		}
		seekPosDif+=size;

		doptime = curDataTime;
		curDataTime = DFrame[i].time;
		if (curDataTime <= 0)
		{
			printf("correct Time 33idn Pos=%d i=%d size=%d time=%d timeCorr=%d \n",
							seekPosDif,i,DFrame[i].size,curDataTime,doptime);
			curDataTime = doptime;
			DFrame[i].time = curDataTime;
		}
		if ((curDataTime < doptime) && (curDataTime > doptime-10) && (i>0)){
			printf("correct Time 55 Pos=%d i=%d size=%d type=%d time=%d timeCorr=%d prevsize=%d prevTyp=%d\n",
					seekPosDif,i,DFrame[i].size,DFrame[i].type,curDataTime,doptime,
					DFrame[i-1].size,DFrame[i-1].type);
					curDataTime = doptime;
					DFrame[i].time = curDataTime;
				}
		test_curTime = (unsigned int) (curDataTime/100.0);

		if(((test_curTime < test_lastTime) && (curDataTime/100 < lastDataTime/100))
				&& lastDataTime/100!=0){
			logFileStr.docFileStruct.msgCount = i;
			printf("break idn Pos=%d i=%d size=%d time=%d  timePrev=%d \n",
					seekPosDif,i,DFrame[i].size,curDataTime,lastDataTime);
//			for (k = 0; k < frame[i].size-1; ++k)  printf ("%d ",frame[i].data[k]);
//			printf("\n");
			printf("break idn Pos=%d i=%d size=%d\n",seekPosDif,i-1,DFrame[i-1].size);
//			for (k = 0; k < frame[i-1].size-1; ++k)  printf ("%d ",frame[i-1].data[k]);
			printf("\n");
			i=i-1;
			break;
		}

		lastDataTime = curDataTime;
		test_lastTime = test_curTime;

		size = lseek(fd, 1L, SEEK_CUR);
		if(size==-1){
			printf("break 22idn Pos=%d i=%d size=%d time=%d \n",seekPosDif,i,DFrame[i].size,curDataTime);
			break;
		}
		seekPosDif+=1;
		if(i%1000==0){
			complete = (float)i / (float)logFileStr.docFileStruct.msgCount * 100.0;
			PtSetResource((PtWidget_t *)ABW_PtProgressLoadLog, Pt_ARG_GAUGE_VALUE , complete, 0);
			PtFlush();
		}
	}



	lseek(fd, -seekPosDif, SEEK_CUR);
	readLogFileCount = i;
	printf("readLogFileCount=%d SEEK_CUR=%d\n",readLogFileCount, SEEK_CUR);
}

#else
// Прочтение документа
void readList(int fd){
	unsigned int test_lastTime, test_curTime,doptime;
	int i, k,size, complete,nCorrected;
	unsigned char sch;
	unsigned int curDataTime, lastDataTime;
	long int seekPosDif = 0;
	PtWidgetToFront(AbGetABW( ABN_PtPaneLoadLog ));
	PtSetResource((PtWidget_t *)AbGetABW( ABN_PtProgressLoadLog ), Pt_ARG_GAUGE_VALUE , 0, 0);
	lastDataTime = 0;
	test_lastTime =0;
	nCorrected =0;
	for(i=0; i<msgCount; i++){
//		size = read(fd, &frame[i].size, 1);
		size= read(fd, &sch, 1);
		frame[i].size = sch;
		if(size!=1){
			printf("didnt read to end i=%d\n",i);
			break;
			
		}
		seekPosDif+=size;
		
		size = read(fd, frame[i].data, frame[i].size);
		if(size==-1){
			break;
		}
		seekPosDif+=size;
		frame[i].data[frame[i].size]='\0'; 
		doptime= curDataTime;
		memcpy(&curDataTime, frame[i].data, sizeof(int));
		if (curDataTime <= 0)
		{
			printf("correct Time 33idn Pos=%d i=%d size=%d time=%d timeCorr=%d \n",
							seekPosDif,i,frame[i].size,curDataTime,doptime);
			curDataTime = doptime;			
			memcpy(frame[i].data,&curDataTime,sizeof(int));
		}
		if ((curDataTime < doptime)&(curDataTime > doptime-10)&(i>0))
				{
			printf("correct Time 55 Pos=%d i=%d size=%d type=%d time=%d timeCorr=%d prevsize=%d prevTyp=%d\n",
					seekPosDif,i,frame[i].size,frame[i].data[4],curDataTime,doptime,
					frame[i-1].size,frame[i-1].data[4]);
					curDataTime = doptime;			
					memcpy(frame[i].data,&curDataTime,sizeof(int));
				}
		test_curTime = rint(curDataTime/100.0);
		test_curTime = (unsigned int) (curDataTime/100.0);
	//	printf("OK idn Pos=%d i=%d size=%d time=%d  timePrev=%d \n",
		//							seekPosDif,i,frame[i].size,curDataTime,lastDataTime);
	//	for (k = 0; k < frame[i].size; ++k)  printf ("%d ",frame[i].data[k]);
	//						printf("\n");			
				
				if(((test_curTime<test_lastTime)&&(curDataTime/100<lastDataTime/100))
						&&lastDataTime/100!=0){
					msgCount = i;
					printf("break idn Pos=%d i=%d size=%d time=%d  timePrev=%d \n",
							seekPosDif,i,frame[i].size,curDataTime,lastDataTime);
					for (k = 0; k < frame[i].size-1; ++k)  printf ("%d ",frame[i].data[k]);
					printf("\n");	
					printf("break idn Pos=%d i=%d size=%d\n",
							seekPosDif,i-1,frame[i-1].size);
					for (k = 0; k < frame[i-1].size-1; ++k)  printf ("%d ",frame[i-1].data[k]);
					
					printf("\n");	
					i=i-1;
					break;
				}
				
				lastDataTime = curDataTime;
				test_lastTime = test_curTime;
		
		size = lseek(fd, 1L, SEEK_CUR);
		if(size==-1){
			printf("break 22idn Pos=%d i=%d size=%d time=%d \n",seekPosDif,i,frame[i].size,curDataTime);
			break;
		}
		seekPosDif+=1;
		if(i%1000==0){
			complete = (float)i/(float)msgCount*100.0;
			PtSetResource((PtWidget_t *)ABW_PtProgressLoadLog, Pt_ARG_GAUGE_VALUE , complete, 0);
			PtFlush();
		}
	}	
	

	
	lseek(fd, -seekPosDif, SEEK_CUR);
	readLogFileCount = i;
	printf("readLogFileCount=%d SEEK_CUR=%d\n",readLogFileCount, SEEK_CUR);
}
#endif




//void preLoadFile(int state){
//	int i;
//	int size;
//	if (state==0)
//		logFilePos = 0;
//	for(i=0, readLogFileCount=0; i<READ_COUNT; readLogFileCount++,i++){
//		size = read(logFile, &frame[i].size, 1);
//		if(size==-1)
//			break;
//		size = read(logFile, frame[i].data, frame[i].size);
//		//frame[i].data[size]='\0'; // TEMP!!!!!!!!!!!!
//
//		if(size==-1)
//			break;
//		size = read(logFile, &frame[i].size, 1);
//		if(size==-1)
//			break;
//	}
//}

// Проверка типа файла
// отображаются только файлы с расширением .log
int testLogFilesType(char *path){
	int len = strlen(path);

	if(strncmp(path, "doc", 3)){
		return 0;
	}
	if(!strncmp(path+(len - strlen(".log")), ".log", 4)){
		return 1;
	}else{
		return 0;
	}
}

void cpFileToTemp(char* str){
	char tmpStr[256]="";
	sprintf(tmpStr, "cp %s %s", str, TMP_LOG_PATH);
	system(tmpStr);
}
// Начать просмотр документа
int
startView( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	char path[100];
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	//loadFileView();
//	PtSetResource(AbGetABW(ABN_docName), Pt_ARG_TEXT_STRING, logName[posSellect], 0);
//	PtSetResource(AbGetABW(ABN_docName), Pt_ARG_TEXT_STRING, selectedListStuct.item, 0);

	PtSetResource(AbGetABW(ABN_butSaveTxt1),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);
	PtSetResource(AbGetABW(ABN_butSaveTxt0),Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);

	char *name;
	PtGetResource(AbGetABW(ABN_fileName), Pt_ARG_TEXT_STRING, &name, 0);
	PtSetResource(AbGetABW(ABN_docName), Pt_ARG_TEXT_STRING, name, 0);

	sprintf(path,"%s/%s",docPath,name);
//	sprintf(path,"%s/%s",docPath,selectedListStuct.item);

//	strcpy(path, docPath);
//	strcat(path, "/");
//	strcat(path, logName[posSellect]);
//
	cpFileToTemp(path);

#ifdef NEW_FRAME_STRUCT
	logFileStr.fd = open(TMP_LOG_PATH, O_RDONLY);
	if(logFileStr.fd != -1){
		strcpy(logFileStr.docFileStruct.filePath, TMP_LOG_PATH);
		docGetStartStopTime(&logFileStr.docFileStruct);
		logFileStr.pos = 0;

		printf("\nRead logFile in func %s\n", __FUNCTION__);
		printf("path: %s \n", logFileStr.docFileStruct.filePath);
		printf("msgCount: %d \n", logFileStr.docFileStruct.msgCount);
		printf("startTime: %d \n", logFileStr.docFileStruct.startTime);
		printf("stopTime: %d \n\n", logFileStr.docFileStruct.stopTime);
		if ( (showFrame = calloc(logFileStr.docFileStruct.msgCount + 1, sizeof(uint32_t))) == NULL){
			printf("Error calloc showFrame %d\n", __LINE__);
			perror(strerror(errno));
			close(logFileStr.fd);
			freeMem();
			return( Pt_CONTINUE );
		}else if( (DFrame = calloc(logFileStr.docFileStruct.msgCount + 1, sizeof(doc_frame_t))) == NULL){
			printf("Error calloc frame %d\n", __LINE__);
			perror(strerror(errno));
			close(logFileStr.fd);
			freeMem();
			return( Pt_CONTINUE );
		}

		readList(logFileStr.fd);
		readFilter();
		filterFrame(DFrame, readLogFileCount);
		printFrame(0);
		PtSetResource((PtWidget_t *)AbGetABW(ABN_docSlidePos), Pt_ARG_MIN_SLIDER_SIZE, 40, 0);
		PtWidgetToFront(ABW_MenuDoc);
		PtWidgetToFront(ABW_paneDoc);
		close(logFileStr.fd);
	}

#else
	logFile = open(TMP_LOG_PATH, O_RDONLY);

	if(logFile!=-1){
		logFilePos=0;
		msgCount = docMsgCount(logFile);
		printf("Msg count = %d \n", msgCount);

		readList(logFile);
		readFilter();
		filterFrame(frame, readLogFileCount);
		PtSetResource((PtWidget_t *)AbGetABW(ABN_docSlidePos), Pt_ARG_MIN_SLIDER_SIZE, 40, 0);
		printFrame(0);
		PtWidgetToFront(ABW_MenuDoc);
		PtWidgetToFront(ABW_paneDoc);
	}
#endif



	return( Pt_CONTINUE );
}


int
pDFDell( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
//	char path[100];
//	unsigned long    *flags;
//	unsigned long	blockFlags;
//	char *name;
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;


	if(fileSellect == 1){
		DocFileStruct_t 	selectedDocFile;
		char *name;
		PtGetResource(AbGetABW(ABN_fileName), Pt_ARG_TEXT_STRING, &name, 0);
		sprintf(selectedDocFile.filePath, "%s/%s", docPath, name);
//		sprintf(selectedDocFile.filePath, "%s/%s", docPath, selectedListStuct.item);

		if(remove(selectedDocFile.filePath) != 0){
			printf("Delete file: %s \t failed!\n", selectedDocFile.filePath);
		}
		refreshFile(widget, NULL, NULL);

	}

//	if(posSellect!=-1){
//		if(fileSellect==1){
//			PtGetResource(AbGetABW(ABN_fileName), Pt_ARG_TEXT_STRING, &name, 0);
//			if(!strcmp(name, fileList[posSellect].name)){
//				PtSetResource(AbGetABW(ABN_startView),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);
//				//PtSetResource(AbGetABW(ABN_fileSellect),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);
//				PtSetResource( AbGetABW(ABN_fileDelete),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);
//				PtSetResource(AbGetABW(ABN_fileState), Pt_ARG_TEXT_STRING, "Файл архива\nне выбран", 0);
//				PtSetResource(AbGetABW(ABN_fileState),Pt_ARG_FLAGS, Pt_GHOST, Pt_TRUE);
//				PtSetResource(AbGetABW(ABN_fileName), Pt_ARG_TEXT_STRING, "", 0);
//			}
//		}
//		fileSellect = -1;
//		posSellect = -1;
//		strcpy(path, docPath);
//		strcat(path, "/");
//		strcat(path, name);
//		if (remove(path) != 0)
//		{
//			printf("Delete file: %s\n", path);
//			perror("Delete failed");
//		}
//		refreshFile(widget, NULL, NULL);
//		// Удаление из файла
//	}
	changeBlockState(ABW_MenuSel, eFalse);
	changeBlockState(ABW_paneSel, eFalse);
//
//	PtGetResource(AbGetABW(ABN_MenuSel), Pt_ARG_FLAGS, &flags, 0);
//	blockFlags = (*flags)&(~Pt_BLOCKED);
//	PtSetResource(AbGetABW(ABN_MenuSel), Pt_ARG_FLAGS, blockFlags, Pt_TRUE);
//	PtGetResource(AbGetABW(ABN_paneSel), Pt_ARG_FLAGS, &flags, 0);
//	blockFlags = (*flags)&(~Pt_BLOCKED);
//	PtSetResource(AbGetABW(ABN_paneSel), Pt_ARG_FLAGS, blockFlags, Pt_TRUE);
	PtWidgetToFront(AbGetABW(ABN_paneSel));
	if(USE_PANE_IN_PANESEL){
		PtWidgetToFront(ABW_PtPane_LogFiles);
	}
	return( Pt_CONTINUE );
}


int
pDFCancel( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
//	unsigned long    *flags;
//	unsigned long	blockFlags;
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	changeBlockState(ABW_MenuSel, eFalse);
	changeBlockState(ABW_paneSel, eFalse);
//	PtGetResource(AbGetABW(ABN_MenuSel), Pt_ARG_FLAGS, &flags, 0);
//	blockFlags = (*flags)&(~Pt_BLOCKED);
//	PtSetResource(AbGetABW(ABN_MenuSel), Pt_ARG_FLAGS, blockFlags, Pt_TRUE);
//	PtGetResource(AbGetABW(ABN_paneSel), Pt_ARG_FLAGS, &flags, 0);
//	blockFlags = (*flags)&(~Pt_BLOCKED);
//	PtSetResource(AbGetABW(ABN_paneSel), Pt_ARG_FLAGS, blockFlags, Pt_TRUE);

#ifdef NEW_FRAME_STRUCT
	if (logFileStr.fd != -1){
		close(logFileStr.fd);
	}
#else
	if(logFile!=-1)
		close(logFile);
#endif
	PtWidgetToFront(ABW_paneSel);
	if(USE_PANE_IN_PANESEL){
		PtWidgetToFront(ABW_PtPane_LogFiles);
	}
	
	return( Pt_CONTINUE );
}

// Выход из программы
int
Exit( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
#ifdef NEW_FRAME_STRUCT
	freeMem();
#endif
	PtExit(0);
	return( Pt_CONTINUE );
}

// Пролистывание документации
int
scrollLog( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){
	unsigned int *pos;

	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtGetResource((PtWidget_t *)AbGetABW(ABN_docSlidePos), Pt_ARG_SCROLL_POSITION , &pos, 0);
#ifdef NEW_FRAME_STRUCT
	logFileStr.pos = *pos;
	printFrame(logFileStr.pos);
#else
	logFilePos = *pos;
	printFrame(logFilePos);
#endif


	return( Pt_CONTINUE );
}


// Задать фильтрацию
int
okSetup( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	readFilter();
	filterFrame(DFrame, readLogFileCount);
	printFrame(0);
	
	PtWidgetToFront(ABW_MenuDoc);
	PtWidgetToFront(ABW_paneDoc);
	
	return( Pt_CONTINUE );
}


// Кнопки задать номер для фильтрации
int
numb( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	unsigned short *sel;
	char **items;
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	keyboardStruct.keyboard_MAX = 999;
	keyboardStruct.keyboard_MIN = 0;
	strcpy(keyboardStruct.keyboardNumb, "");
	PtSetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, "", 0);
	if(widget==(PtWidget_t*)ABW_setNt0){
		keyboardStruct.parentKeyWidget = (PtWidget_t*)ABW_filterNt0;
		PtGetResource(ABW_filterNt0, Pt_ARG_LIST_ITEM_COUNT , &sel, 0);
		if(*sel==0)
			PtListDeleteAllItems(ABW_keyboardList);
		else{
			PtListDeleteAllItems(ABW_keyboardList);
			PtGetResource(ABW_filterNt0, Pt_ARG_ITEMS , &items, 0);
			PtListAddItems(ABW_keyboardList, (const char**)items, *sel, 0);
		}
	}else if(widget==(PtWidget_t*)ABW_setNrls1){
		keyboardStruct.parentKeyWidget = (PtWidget_t*)ABW_filterNrls1;
		PtGetResource(ABW_filterNrls1, Pt_ARG_LIST_ITEM_COUNT , &sel, 0);
		if(*sel==0)
			PtListDeleteAllItems(ABW_keyboardList);
		else{
			PtListDeleteAllItems(ABW_keyboardList);
			PtGetResource(ABW_filterNrls1, Pt_ARG_ITEMS , &items, 0);
			PtListAddItems(ABW_keyboardList, (const char**)items, *sel, 0);
		}
	}else if(widget==(PtWidget_t*)ABW_setNpuma1){
		keyboardStruct.parentKeyWidget = (PtWidget_t*)ABW_filterNpuma1;
		PtGetResource(ABW_filterNpuma1, Pt_ARG_LIST_ITEM_COUNT , &sel, 0);
		if(*sel==0)
			PtListDeleteAllItems(ABW_keyboardList);
		else{
			PtListDeleteAllItems(ABW_keyboardList);
			PtGetResource(ABW_filterNpuma1, Pt_ARG_ITEMS , &items, 0);
			PtListAddItems(ABW_keyboardList, (const char**)items, *sel, 0);
		}
	}else if(widget==(PtWidget_t*)ABW_setSendRli){
		keyboardStruct.parentKeyWidget = (PtWidget_t*)ABW_filterSendRli;
		PtGetResource(ABW_filterSendRli, Pt_ARG_LIST_ITEM_COUNT , &sel, 0);
		if(*sel==0)
			PtListDeleteAllItems(ABW_keyboardList);
		else{
			PtListDeleteAllItems(ABW_keyboardList);
			PtGetResource(ABW_filterSendRli, Pt_ARG_ITEMS , &items, 0);
			PtListAddItems(ABW_keyboardList, (const char**)items, *sel, 0);
		}
	}else if(widget==(PtWidget_t*)ABW_setNtX){
		keyboardStruct.parentKeyWidget = (PtWidget_t*)ABW_filterNtX;
		PtGetResource(ABW_filterNtX, Pt_ARG_LIST_ITEM_COUNT , &sel, 0);
		if(*sel==0)
			PtListDeleteAllItems(ABW_keyboardList);
		else{
			PtListDeleteAllItems(ABW_keyboardList);
			PtGetResource(ABW_filterNtX, Pt_ARG_ITEMS , &items, 0);
			PtListAddItems(ABW_keyboardList, (const char**)items, *sel, 0);
	}
	}
	PtWidgetToFront(ABW_PtKeyboard);
	return( Pt_CONTINUE );
}


// Обработка нажатия кнопок клавиатуры
int
pressKeyboard( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	unsigned short *sel;
	unsigned short selCount;
	char **items;
	char *str;
	char tempNumb[100]="";
	int n;
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	if(widget==(PtWidget_t*)ABW_key0){
		PtGetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, &str, 0);
		strcpy(tempNumb, str);
		if(strcmp(tempNumb, "0")){
			strcat(tempNumb,"0");
			n = atoi(tempNumb);
			if(n<=keyboardStruct.keyboard_MAX&&n>=keyboardStruct.keyboard_MIN)
				PtSetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, tempNumb, 0);
		}
	}else if(widget==(PtWidget_t*)ABW_key1){
		PtGetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, &str, 0);
		strcpy(tempNumb, str);
		strcat(tempNumb,"1");
		n = atoi(tempNumb);
		if(n<keyboardStruct.keyboard_MAX&&n>keyboardStruct.keyboard_MIN)
						PtSetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, tempNumb, 0);
	}else if(widget==(PtWidget_t*)ABW_key2){
		PtGetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, &str, 0);
		strcpy(tempNumb, str);
		strcat(tempNumb,"2");
		n = atoi(tempNumb);
		if(n<keyboardStruct.keyboard_MAX&&n>keyboardStruct.keyboard_MIN)
						PtSetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, tempNumb, 0);
	}else if(widget==(PtWidget_t*)ABW_key3){
		PtGetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, &str, 0);
		strcpy(tempNumb, str);
		strcat(tempNumb,"3");
		n = atoi(tempNumb);
		if(n<keyboardStruct.keyboard_MAX&&n>keyboardStruct.keyboard_MIN)
						PtSetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, tempNumb, 0);
	}else if(widget==(PtWidget_t*)ABW_key4){
		PtGetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, &str, 0);
		strcpy(tempNumb, str);
		strcat(tempNumb,"4");
		n = atoi(tempNumb);
		if(n<keyboardStruct.keyboard_MAX&&n>keyboardStruct.keyboard_MIN)
						PtSetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, tempNumb, 0);
	}else if(widget==(PtWidget_t*)ABW_key5){
		PtGetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, &str, 0);
		strcpy(tempNumb, str);
		strcat(tempNumb,"5");
		n = atoi(tempNumb);
		if(n<keyboardStruct.keyboard_MAX&&n>keyboardStruct.keyboard_MIN)
						PtSetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, tempNumb, 0);
	}else if(widget==(PtWidget_t*)ABW_key6){
		PtGetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, &str, 0);
		strcpy(tempNumb, str);
		strcat(tempNumb,"6");
		n = atoi(tempNumb);
		if(n<keyboardStruct.keyboard_MAX&&n>keyboardStruct.keyboard_MIN)
						PtSetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, tempNumb, 0);
	}else if(widget==(PtWidget_t*)ABW_key7){
		PtGetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, &str, 0);
		strcpy(tempNumb, str);
		strcat(tempNumb,"7");
		n = atoi(tempNumb);
		if(n<keyboardStruct.keyboard_MAX&&n>keyboardStruct.keyboard_MIN)
						PtSetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, tempNumb, 0);
	}else if(widget==(PtWidget_t*)ABW_key8){
		PtGetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, &str, 0);
		strcpy(tempNumb, str);
		strcat(tempNumb,"8");
		n = atoi(tempNumb);
		if(n<keyboardStruct.keyboard_MAX&&n>keyboardStruct.keyboard_MIN)
						PtSetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, tempNumb, 0);
	}else if(widget==(PtWidget_t*)ABW_key9){
		PtGetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, &str, 0);
		strcpy(tempNumb, str);
		strcat(tempNumb,"9");
		n = atoi(tempNumb);
		if(n<keyboardStruct.keyboard_MAX&&n>keyboardStruct.keyboard_MIN)
						PtSetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, tempNumb, 0);
	}else if(widget==(PtWidget_t*)ABW_keyOk){
		//копирование всего в структуру
		PtGetResource(ABW_keyboardList, Pt_ARG_LIST_ITEM_COUNT , &sel, 0);
		if(*sel==0){
			PtListDeleteAllItems(keyboardStruct.parentKeyWidget);
			PtSetResource(keyboardStruct.parentKeyWidget, Pt_ARG_TEXT_STRING, "Все", 0);
		}else{
			PtListDeleteAllItems(keyboardStruct.parentKeyWidget);
			PtGetResource(ABW_keyboardList, Pt_ARG_ITEMS , &items, 0);
			PtListAddItems(keyboardStruct.parentKeyWidget, (const char**)items, *sel, 0);
			PtSetResource(keyboardStruct.parentKeyWidget, Pt_ARG_TEXT_STRING, "#", 0);
		}
		PtWidgetToBack(ABW_PtKeyboard);
		return( Pt_CONTINUE );
	}else if(widget==(PtWidget_t*)ABW_keyCancel){
		PtWidgetToBack(ABW_PtKeyboard);
		return( Pt_CONTINUE );
	}else if(widget==(PtWidget_t*)ABW_keyReset){
		PtListDeleteAllItems(ABW_keyboardList);
	}else if(widget==(PtWidget_t*)ABW_keyDel){
		PtGetResource(ABW_keyboardList, Pt_ARG_SELECTION_INDEXES, &selCount, 0);
		if(selCount>0){
			PtGetResource(ABW_keyboardList, Pt_ARG_SELECTION_INDEXES, &sel, 0);
			PtListRemovePositions(ABW_keyboardList, sel, 1);
		}
	}else if(widget==(PtWidget_t*)ABW_keyAdd){
		if(strcmp(keyboardStruct.keyboardNumb,"")){
			PtGetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, &str, 0);
			PtListAddItems(ABW_keyboardList,(const char**)&str, 1, 1);
			strcpy(keyboardStruct.keyboardNumb, "");
			PtSetResource(ABW_keyboardNumb, Pt_ARG_TEXT_STRING, "", 0);
		}
		return(Pt_CONTINUE);
	}
	strcpy(keyboardStruct.keyboardNumb, tempNumb);
	return( Pt_CONTINUE );
}


int
timeScrollMove( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	unsigned long *size;
	unsigned int pos;
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	PtGetResource(ABW_docSlideTime, Pt_ARG_GAUGE_VALUE, &size, 0);
	pos = timeScroll.timeFrame[*size];
	
	PtSetResource((PtWidget_t *)AbGetABW(ABN_docSlidePos), Pt_ARG_SCROLL_POSITION , pos, 0);
#ifdef NEW_FRAME_STRUCT
	logFileStr.pos = pos;
	printFrame(logFileStr.pos);
#else
	logFilePos = pos;
	printFrame(logFilePos);
#endif

	return( Pt_CONTINUE );
}


int
docPlay( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	if(showFrameCount<1)
		return( Pt_CONTINUE );
#ifdef NEW_FRAME_STRUCT
	startPlayTime = DFrame[showFrame[logFileStr.pos]].time;
#else
	memcpy(&startPlayTime, frame[showFrame[logFilePos]].data, sizeof(int));
#endif
	if(startPlayTime!=pauseTime){
		bPlay = 2;
		pauseTime = 0;
	}else{
		bPlay = 1;
	}
	PtSetResource( AbGetABW(ABN_docSetup),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);
	PtSetResource( AbGetABW(ABN_closeLog),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);
	return( Pt_CONTINUE );
}


int
docPause( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
#ifdef NEW_FRAME_STRUCT
	pauseTime = DFrame[showFrame[logFileStr.pos]].time;
#else
	memcpy(&pauseTime, frame[showFrame[logFilePos]].data, sizeof(int));
#endif

	bPlay = 0;
	PtSetResource( AbGetABW(ABN_docSetup),Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);
	PtSetResource( AbGetABW(ABN_closeLog),Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);
	return( Pt_CONTINUE );
}

void add67toDraw(DocDev67Tgt *p){
	pointDraw67.draw[pointDraw67.curPos] = 1;
	pointDraw67.timeDraw[pointDraw67.curPos] = time(NULL);
	memcpy(&pointDraw67.dev67Tgt[pointDraw67.curPos], p, sizeof(DocDev67Tgt));
	
	pointDraw67.curPos ++;
	if(pointDraw67.curPos>99)
		pointDraw67.curPos = 0;
	if(pointDraw67.count<100)
		pointDraw67.count++;
}

void addOurShipCourse(DocOurShipGeo *p){
	courseShipView = p->course;
}

void addPointToArray(DocFormularTrassa *p){
	float distance; 
	float peleng;
	float x,y,z;
	//int arrPos = 0;
	//struct pointCoord pointCoord[1001];
	
	if(!p->ShP){
		x = (float)(signed short)p->X*5.0;
		y = (float)(signed short)p->Y*5.0;
		z = (float)p->H;
		distance = sqrtf ( powf(x,2) + powf(y,2) + powf(z,2) );
		peleng = docDo2pi(atan2f(y, x));
		
		pointTrassa[p->nSource].draw = 1;
		pointTrassa[p->nSource].nSource = p->nSource;
		pointTrassa[p->nSource].x = x;
		pointTrassa[p->nSource].y = y;
		pointTrassa[p->nSource].z = z;
		pointTrassa[p->nSource].distance = distance;
		pointTrassa[p->nSource].peleng = peleng;
		pointTrassa[p->nSource].objType = p->TC;
		pointTrassa[p->nSource].timeDraw = time(NULL);
		pointTrassa[p->nSource].vx 		= docDecodeValue(p->Vx, 5.0, 1, 10);
		pointTrassa[p->nSource].vy 		= docDecodeValue(p->Vy, 5.0, 1, 10);
		pointTrassa[p->nSource].vz 		= docDecodeValue(p->Vh, 5.0, 1, 10);
		pointTrassa[p->nSource].course = docDo2pi(atan2f(pointTrassa[p->nSource].vy, pointTrassa[p->nSource].vx));
		pointTrassa[p->nSource].vr = sqrtf ( powf(pointTrassa[p->nSource].vx,2) + 
				powf(pointTrassa[p->nSource].vy,2) +
				powf(pointTrassa[p->nSource].vz,2) );
		pointTrassa[p->nSource].own = p->SV;
		
		
		if((pointArray.curPos)>999)
			pointArray.curPos = 0;
		
		pointArray.pointCoord[pointArray.curPos].distance = distance;
		pointArray.pointCoord[pointArray.curPos].peleng = peleng;
		pointArray.pointCoord[pointArray.curPos].pr_own = p->SV;
		
		if(pointArray.count<1000)
			pointArray.count++;
		pointArray.curPos++;

		
	}
	
}


int getTargetTelevikByNDoc (int nTelevik)
{
	int i;
	if (nTelevik<=0) return -1;
	for (i = 0; i < 100; ++i) 
		if (pointTelevik[i].nSource ==nTelevik) return i;
	return -1;
}

int getFreeTelevikDoc (void)
{
	int i;
	for (i = 0; i < 100; ++i) 
		if (pointTelevik[i].nSource ==0) return i;
	return -1;
};



void addPointToArrayFromTelevik(DocTargetTelevik *p){
	float distance; 
	float peleng;
	int i;
	//int arrPos = 0;
	//struct pointCoord pointCoord[1001];
	
		distance = sqrtf ( powf(p->x,2) + powf(p->y,2) + powf(p->h,2) );
		peleng = docDo2pi(atan2f(p->y, p->x));
		
		i = getTargetTelevikByNDoc(p->nSource);
		if (i==-1) i = getFreeTelevikDoc();
		if (i==-1) return;
		pointTelevik[i].draw = 1;
		pointTelevik[i].nSource = p->nSource;
		pointTelevik[i].x = p->x;
		pointTelevik[i].y = p->y;
		pointTelevik[i].z = p->h;
		pointTelevik[i].distance = distance;
		pointTelevik[i].peleng = peleng;
		pointTelevik[i].objType = p->typeTarget;
		pointTelevik[i].timeDraw = time(NULL);
		pointTelevik[i].vx 		= p->vx;
		pointTelevik[i].vy 		= p->vy;
		pointTelevik[i].vz 		= p->vh;
		pointTelevik[i].course = docDo2pi(atan2f(pointTelevik[i].vy, pointTelevik[i].vx));
		pointTelevik[i].vr = sqrtf ( powf(pointTelevik[i].vx,2) + powf(pointTelevik[i].vy,2) +	powf(pointTelevik[i].vz,2) );
		pointTelevik[i].own = 0;
		
		
		if((pointArray.curPos)>999)
			pointArray.curPos = 0;
		
		pointArray.pointCoord[pointArray.curPos].distance = distance;
		pointArray.pointCoord[pointArray.curPos].peleng = peleng;
		pointArray.pointCoord[pointArray.curPos].pr_own = 0;
		
		if(pointArray.count<1000)
			pointArray.count++;
		pointArray.curPos++;
}
void addPointToArrayFromPristavka(DocTargetPristavka *p){
	float distance; 
	float peleng;
	int i;
	//int arrPos = 0;
	//struct pointCoord pointCoord[1001];
	
		distance = p->distance;
		peleng = docDo2pi(p->peleng);
		i = p->nSource-1; 
		if ((i < 0) | (i>=200)) return;
		
		pointTrassa[i].draw = 1;
		pointTrassa[i].nSource = p->nSource;
		pointTrassa[i].distance = p->distance;
		pointTrassa[i].peleng = p->peleng;
		pointTrassa[i].objType = otPlane;
		pointTrassa[i].timeDraw = time(NULL);
		pointTrassa[i].course = p->course;
		pointTrassa[i].vr = p->speed;
		
		
		if((pointArray.curPos)>999)
			pointArray.curPos = 0;
		
		pointArray.pointCoord[pointArray.curPos].distance = distance;
		pointArray.pointCoord[pointArray.curPos].peleng = peleng;
		pointArray.pointCoord[pointArray.curPos].pr_own = 0;
		
		if(pointArray.count<1000)
			pointArray.count++;
		pointArray.curPos++;
}

void addPointToArrayFromServal(DocTargetServal *p){
	float distance; 
	float peleng;
	int i;
	//int arrPos = 0;
	//struct pointCoord pointCoord[1001];
	
		if ((p->bdistance ==0) | (p-> rrc != 1)) return;
		distance = (p->bdistance ==1) * p->distance;
		
		pointServal.draw = 1;
		pointServal.nServal = p->nServal;
		pointServal.nTelevik = p->nTelevik;
		pointServal.AC = (p-> rrc == 2);
		pointServal.bFire = (p-> fire);
		if (p->typeTarget ==1) pointServal.objType = otShip; else
			if (p->typeTarget ==8) pointServal.objType = otMissileCommon; else
				if (p->typeTarget ==10) pointServal.objType = otPlane; else
					if (p->typeTarget ==11) pointServal.objType = otPkr; else
						if (p->typeTarget ==12) pointServal.objType = otPrr; else
							if (p->typeTarget ==13) pointServal.objType = otHelicopter; 
		else pointServal.objType = otMissileCommon;
		pointServal.nTelevik = p->nTelevik;
		pointServal.distance = p->distance;
		pointServal.peleng = p->ku; // TODO
		pointServal.course = docDo2pi(atan2f(p->vy, p->vx));
		
		if((pointArray.curPos)>999)
			pointArray.curPos = 0;
		
		pointArray.pointCoord[pointArray.curPos].distance = p->distance;
		pointArray.pointCoord[pointArray.curPos].peleng = p->ku;p->ku; // TODO
		pointArray.pointCoord[pointArray.curPos].pr_own = 1;
		
		if(pointArray.count<1000)
			pointArray.count++;
		pointArray.curPos++;
}

int getTargetXByNxDoc (int nX)
{
	int i;
	if (nX <=0) return -1;
	for (i = 0; i < 300; ++i) 
		if (pointTrassa_X[i].nX ==nX) return i;
	return -1;
}


int getFreeNxDoc (void)
{
	int i;
	for (i = 0; i < 300; ++i) 
		if (pointTrassa_X[i].nX ==0) return i;
	return -1;
};




void addPointToArrayFromX(DocTargetX_m *p){

	int ind = -1;
	float Vx,Vy;
	time_t doptime;
	//struct pointCoord pointCoord[1001];
	if (p->SKS == 0) return;
	ind = getTargetXByNxDoc(p->nn);
	if (ind==-1) ind = getFreeNxDoc();
	else
	{
		doptime = time(NULL);
		if (doptime - pointTrassa_X[ind].timeDraw <0.1) 
			return;
	}
	if (ind==-1) return;
		pointTrassa_X[ind].draw = 1;
	    pointTrassa_X[ind].nX= p->nn;
	    pointTrassa_X[ind].objType= DocconvSuTypeTargetToObjType(p->typeTarget);
	    pointTrassa_X[ind].timeDraw = time(NULL);
		pointTrassa_X[ind].distance = ((unsigned long)(p->distance_hi <<16|p->distance_low ))*0.05;
		pointTrassa_X[ind].peleng = ((unsigned long)(p->peleng_hi <<16|p->peleng_low ))/pow(2,31)*M_PI;
		pointTrassa_X[ind].own = p->GO;

		if (p->SKS == 1) // если вместо Vx, Vx ВИР и ВИП - игнорируем их
  	  {	  
  		  Vx =  p->bVx* ((signed short)(p->Vx))*0.25; 	  
  		  Vy =  p->bVy*((signed short)(p->Vy))*0.25;
  		  pointTrassa_X[ind].course =  viewDo2pi((atan2f(Vy,Vx)));
  	  }

  	  if((pointArray.curPos)>999)
			pointArray.curPos = 0;
		
		//pointArray.pointCoord[pointArray.curPos].distance = pointTrassa[ind].distance;
		//pointArray.pointCoord[pointArray.curPos].peleng = pointTrassa[ind].peleng;
		//pointArray.pointCoord[pointArray.curPos].pr_own = pointTrassa[ind].own;
		
		if(pointArray.count<1000)
			pointArray.count++;
		pointArray.curPos++;
}


#ifdef NEW_FRAME_STRUCT
int
playTimer( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){
	int time1,time2;
	int j, i;
	int count=0;
	char str[256];
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	//printf("_sliderViewActive = %d\n", _sliderViewActive);
	
	if(_sliderViewActive)
		return( Pt_CONTINUE );
	
	if(bPlay==2){ // Опереции при новом просмотре
		printf("Reset message\n");
		// Проигрывание 100 прошлых секунд
		// Очистка экрана
		pointArray.count = 0;
		pointArray.curPos = 0;
		for(i=0; i<200; i++){
			pointTrassa[i].draw = 0;
		}
		for(i=0; i<300; i++){
			pointTrassa_X[i].draw = 0;
		}
		for(i=0; i<100; i++){
			pointTelevik[i].draw = 0;
		}
		pointServal.draw = 0 ;
		pointDraw67.count = 0;
		pointDraw67.curPos = 0;		
		
		bPlay=1;
		time1 = DFrame[showFrame[logFileStr.pos]].time;
		time2 = time1;
		// ищем фраймы на 100 секунд раньше которые были
		for(j = logFileStr.pos; j > 1; j--){
			time2 = DFrame[showFrame[logFileStr.pos]].time;
			if(time2 < time1-100000)
				break;
		}
		for(i=j; i < logFileStr.pos; i++){
			//  Быстрый ПРОГОН ПРОШЛЫХ 100 секунд
			// send frame[showFrame[i]].data
			if(DFrame[showFrame[i]].type == DI_FORMULAR_TRASSA)
					addPointToArray(DFrame[showFrame[i]].data);
			if(DFrame[showFrame[i]].type == DI_TARGETTELEVIK)
					addPointToArray(DFrame[showFrame[i]].data);
			if(DFrame[showFrame[i]].type == DI_TARGETPRISTAVKA)
					addPointToArray(DFrame[showFrame[i]].data);
			if(DFrame[showFrame[i]].type == DI_TARGETX_CODE)
					addPointToArrayFromX(DFrame[showFrame[i]].data);
		}
		//printf("Fast send : %d\n", i);
	}
	if(bPlay==1){
		//printf("Send Message\n");
		time1 = DFrame[showFrame[logFileStr.pos]].time;
		time2 = time1;
		// Send controlTimeId
		if(logFileStr.pos < showFrameCount-1){
			count = 0;
			if(time1 <= startPlayTime)
			while(time2 < time1+100){ // выбираются только те сообщения
				
				// Отметки трасс
				if(DFrame[showFrame[logFileStr.pos]].type == DI_FORMULAR_TRASSA)
					addPointToArray(DFrame[showFrame[logFileStr.pos]].data);
				if(DFrame[showFrame[logFileStr.pos]].type == DI_TARGETTELEVIK)
					addPointToArrayFromTelevik(DFrame[showFrame[logFileStr.pos]].data);
				if(DFrame[showFrame[logFileStr.pos]].type == DI_TARGETPRISTAVKA)
					addPointToArrayFromPristavka(DFrame[showFrame[logFileStr.pos]].data);
				if(DFrame[showFrame[logFileStr.pos]].type == DI_TARGETSERVAL)
					addPointToArrayFromServal(DFrame[showFrame[logFileStr.pos]].data);
				if(DFrame[showFrame[logFileStr.pos]].type == DI_TARGETX_CODE)
					addPointToArrayFromX(DFrame[showFrame[logFileStr.pos]].data);

				// Отметки изделия 67
				if(DFrame[showFrame[logFileStr.pos]].type == DI_Dev67Tgt)
					add67toDraw(DFrame[showFrame[logFileStr.pos]].data);
				// Курс корбля
				if(DFrame[showFrame[logFileStr.pos]].type == DI_OURSHIPGEO)
					addOurShipCourse(DFrame[showFrame[logFileStr.pos]].data);
				
				logFileStr.pos++;
				count++;
				if(logFileStr.pos >= showFrameCount){
					logFileStr.pos --;
					bPlay=0;
					PtSetResource( AbGetABW(ABN_docSetup),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);
					PtSetResource( AbGetABW(ABN_closeLog),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);
					return(Pt_CONTINUE);
				}
				if(playViewDoc)
					PtDamageWidget(ABW_paneDraw);
				else
					printFrame(logFileStr.pos);
				
				time2 = DFrame[showFrame[logFileStr.pos]].time;
			}
			startPlayTime+=100;
			timeConverter(str, startPlayTime, NULL);
//			sprintf(str,"%d:%d:%d.%d",
//						startPlayTime/360000, (startPlayTime/6000)%60,
//						(startPlayTime/100)%60, (startPlayTime%100)/10);
			setScrollView(startPlayTime);
			
			PtSetResource(ABW_docCurTime, Pt_ARG_TEXT_STRING, str, 0);
			//printf("count = %d\n", count);
		}else {
			bPlay=0;
			return(Pt_CONTINUE);
		}
	}
	return( Pt_CONTINUE );
}
#else
int
playTimer( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){
	int time1,time2;
	int j, i;
	int count=0;
	char str[256];
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	//printf("_sliderViewActive = %d\n", _sliderViewActive);

	if(_sliderViewActive)
		return( Pt_CONTINUE );

	if(bPlay==2){ // Опереции при новом просмотре
		printf("Reset message\n");
		// Проигрывание 100 прошлых секунд
		// Очистка экрана
		pointArray.count = 0;
		pointArray.curPos = 0;
		for(i=0; i<200; i++){
			pointTrassa[i].draw = 0;
		}
		for(i=0; i<300; i++){
			pointTrassa_X[i].draw = 0;
		}
		for(i=0; i<100; i++){
			pointTelevik[i].draw = 0;
		}
		pointServal.draw = 0 ;
		pointDraw67.count = 0;
		pointDraw67.curPos = 0;

		bPlay=1;
		memcpy(&time1, frame[showFrame[logFilePos]].data, sizeof(int));
		time2 = time1;
		// ищем фраймы на 100 секунд раньше которые были

		for(j=logFilePos; j>1; j--){
			memcpy(&time2, frame[showFrame[j]].data, sizeof(int));
			if(time2 < time1-100000)
				break;
		}
		for(i=j; i<logFilePos; i++){
			//  Быстрый ПРОГОН ПРОШЛЫХ 100 секунд
			// send frame[showFrame[i]].data
			if(frame[showFrame[i]].data[4] == DI_FORMULAR_TRASSA)
					addPointToArray(&frame[showFrame[i]].data[5]);
			if(frame[showFrame[i]].data[4] == DI_TARGETTELEVIK)
					addPointToArray(&frame[showFrame[i]].data[5]);
			if(frame[showFrame[i]].data[4] == DI_TARGETPRISTAVKA)
					addPointToArray(&frame[showFrame[i]].data[5]);
			if(frame[showFrame[i]].data[4] == DI_TARGETX_CODE)
					addPointToArrayFromX(&frame[showFrame[i]].data[5]);
		}
		//printf("Fast send : %d\n", i);
	}
	if(bPlay==1){
		//printf("Send Message\n");
		memcpy(&time1, frame[showFrame[logFilePos]].data, sizeof(int));
		time2 = time1;
		// Send controlTimeId
		if(logFilePos< showFrameCount-1){
			count = 0;
			if(time1<=startPlayTime)
			while(time2<time1+100){ // выбираются только те сообщения

				// Отметки трасс
				if(frame[showFrame[logFilePos]].data[4] == DI_FORMULAR_TRASSA)
					addPointToArray(&frame[showFrame[logFilePos]].data[5]);
				if(frame[showFrame[logFilePos]].data[4] == DI_TARGETTELEVIK)
					addPointToArrayFromTelevik(&frame[showFrame[logFilePos]].data[5]);
				if(frame[showFrame[logFilePos]].data[4] == DI_TARGETPRISTAVKA)
					addPointToArrayFromPristavka(&frame[showFrame[logFilePos]].data[5]);
				if(frame[showFrame[logFilePos]].data[4] == DI_TARGETSERVAL)
					addPointToArrayFromServal(&frame[showFrame[logFilePos]].data[5]);
				if(frame[showFrame[logFilePos]].data[4] == DI_TARGETX_CODE)
					addPointToArrayFromX(&frame[showFrame[logFilePos]].data[5]);

				// Отметки изделия 67
				if(frame[showFrame[logFilePos]].data[4] == DI_Dev67Tgt)
					add67toDraw(&frame[showFrame[logFilePos]].data[5]);
				// Курс корбля
				if(frame[showFrame[logFilePos]].data[4] == DI_OURSHIPGEO)
					addOurShipCourse(&frame[showFrame[logFilePos]].data[5]);

				logFilePos++;
				count++;
				if(logFilePos>=showFrameCount){
					logFilePos --;
					bPlay=0;
					PtSetResource( AbGetABW(ABN_docSetup),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);
					PtSetResource( AbGetABW(ABN_closeLog),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);
					return(Pt_CONTINUE);
				}
				if(playViewDoc)
					PtDamageWidget(ABW_paneDraw);
				else
					printFrame(logFilePos);
				memcpy(&time2, frame[showFrame[logFilePos]].data, sizeof(int));
				//memcpy(&startPlayTime, frame[showFrame[logFilePos]].data, sizeof(int));

			}
			startPlayTime+=100;
			sprintf(str,"%d:%d:%d.%d",
						startPlayTime/360000, (startPlayTime/6000)%60,
						(startPlayTime/100)%60, (startPlayTime%100)/10);

			setScrollView(startPlayTime);

			PtSetResource(ABW_docCurTime, Pt_ARG_TEXT_STRING, str, 0);
			//printf("count = %d\n", count);
		}else {
			bPlay=0;
			return(Pt_CONTINUE);
		}
	}
	return( Pt_CONTINUE );
}
#endif



void cpFileName(char* dst, char *src){
	int i;
	int len = strlen(src);
	
	for(i=0; i<len; i++)
		if(src[i]=='.')
			break;
	
	strncpy(dst, src, i);
	printf("i=%d\n", i);
}

void createFileTxtPath(char* buff){
	char tmpStr[256]="";
	
	strcpy(buff, TMP_DIR_PATH);
//	cpFileName(tmpStr, logName[posSellect]);
//	cpFileName(tmpStr, selectedListStuct.item);
	char *name;
	PtGetResource(AbGetABW(ABN_fileName), Pt_ARG_TEXT_STRING, &name, 0);
	cpFileName(tmpStr, name);
	strcat(buff, tmpStr);
	strcat(buff, "_UTF8.txt");
}

int
saveTxt( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	int i;
	FILE *fp = NULL;
	char str[1000];
	
	memset(str, 0, 1000);
	strcpy(str, "");
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	createFileTxtPath(str);
	printf("docTxtName = %s\n", str);
	fp = fopen(str, "w");
	if(fp==NULL)
		return 0;
		
	for(i=0; i<showFrameCount; i++){
//		parseDocMsg2Txt(frame[showFrame[i]].data, str, bParamPD, bParamVr,shipType);
#ifdef NEW_FRAME_STRUCT
		parseDocMsg2Txt(DFrame[showFrame[i]].time, DFrame[showFrame[i]].type, DFrame[showFrame[i]].data, str, bParamPD, bParamVr,shipType);
#else
		uint32_t time;
		memcpy(&time, frame[showFrame[i]].data, sizeof(time));
		parseDocMsg2Txt(time, frame[showFrame[i]].data[4], &frame[showFrame[i]].data[5], str, bParamPD, bParamVr,shipType);
#endif
		//printf("%s\n", str);
		fprintf(fp, "%s\n", str);
	}
	
	
	fclose(fp);
	
	return( Pt_CONTINUE );
}


int
openTxt( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char str[300];
	char openPath[300];
		
	memset(openPath, 0, 300);
	memset(str, 0, 300);
	strcpy(str, "");
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	createFileTxtPath(str);
	//TODO UBRAT!!! PARAMETRI V KONFIG!!!!
	sprintf(openPath, "/usr/photon/bin/ped -x0 -y0 -h1024 -w1280 %s &", str);
	printf("openPath = %s\n", openPath);
	system(openPath);

	return( Pt_CONTINUE );
}


int
closeLog( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{

	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	char str[100];
	sprintf(str,"rm %s", TMP_LOG_PATH);
	system(str);
#ifdef NEW_FRAME_STRUCT
	freeMem();
#endif
	PtWidgetToFront(ABW_paneSel);
	if(USE_PANE_IN_PANESEL){
		PtWidgetToFront(ABW_PtPane_LogFiles);
	}
	PtWidgetToFront(ABW_MenuSel);
	return( Pt_CONTINUE );

	}


int
viewOpen( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{

	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	PtSetResource(AbGetABW(ABN_butSaveTxt1),Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);
	PtSetResource(AbGetABW(ABN_butSaveTxt0),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);
	playViewDoc = 1;
	PtWidgetToBack(ABW_paneDoc);
	PtWidgetToFront(ABW_paneViewDraw);
	return( Pt_CONTINUE );

	}


int
docOpen( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	PtSetResource(AbGetABW(ABN_butSaveTxt1),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);
	PtSetResource(AbGetABW(ABN_butSaveTxt0),Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);
	playViewDoc = 0;
	PtWidgetToFront(ABW_paneDoc);
	return( Pt_CONTINUE );

}


int
armSliderView( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	printf("Arm\n");
	_sliderViewActive = 1;
	return( Pt_CONTINUE );
}


int
disarmSliderView( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	_sliderViewActive = 0;
	bPlay = 2;
	return( Pt_CONTINUE );
}


int
moveSliderView( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){
	char str[100];
	unsigned int timeScrollView;
	unsigned long *pos;
	double perc;
	bPlay = 0;
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtGetResource(ABW_sliderView, Pt_ARG_GAUGE_VALUE, &pos, 0);
	perc =  (double)(timeScroll.timeEnd-timeScroll.timeStart)/100.0;
	timeScrollView = perc*(*pos) + timeScroll.timeStart;
#ifdef NEW_FRAME_STRUCT
	logFileStr.pos =  getFramePosFromTime(timeScrollView);
#else
	logFilePos =  getFramePosFromTime(timeScrollView);
#endif
	timeConverter(str, timeScrollView, NULL);

//	sprintf(str,"%d:%d:%d.%d",
//			timeScrollView/360000, (timeScrollView/6000)%60,
//							(timeScrollView/100)%60, (timeScrollView%100)/10);
				
	//			setScrollView(startPlayTime);
				
	PtSetResource(ABW_docCurTime, Pt_ARG_TEXT_STRING, str, 0);
	
	//printf("logFilePos = %d\n", logFilePos);
	return( Pt_CONTINUE );

}

void addSymbolToRenameFilename(char *_str, char _symbol)
{
	char temp[36];
	char ext[5] = {'.','l','o','g',0};
	strncpy(temp, _str, strlen(_str)-4);
	temp[strlen(_str)-4] = 0;
	strncat(temp, &_symbol, 1);
	strcat(temp, ext);
	PtSetResource(ABW_txtFileName, Pt_ARG_TEXT_STRING, temp, 0);
}

int
keyPressed( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char *str;
	char temp[100];
	char ext[] = ".log";
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtGetResource(ABW_txtFileName, Pt_ARG_TEXT_STRING, &str, 0);
	if (strlen(str) > 70)
	{
		strncpy(&temp[0], str, strlen(str)-5);
		strcpy(&temp[strlen(str)-5], ext);
		PtSetResource(ABW_txtFileName, Pt_ARG_TEXT_STRING, &temp, 0);
		return( Pt_CONTINUE );
	}

	if (widget == ABW_btnKeyBackspace)
	{
		if (strlen(str) > 30)
		{
			strncpy(&temp[0], str, strlen(str)-5);
			strcpy(&temp[strlen(str)-5], ext);
			PtSetResource(ABW_txtFileName, Pt_ARG_TEXT_STRING, &temp, 0);
		}
		else
		{
			str = "doc.log";
			PtSetResource(ABW_txtFileName, Pt_ARG_TEXT_STRING, str, 0);
		}
	}
	if (widget == ABW_btnKey0) addSymbolToRenameFilename(str, '0');
	if (widget == ABW_btnKey1) addSymbolToRenameFilename(str, '1');
	if (widget == ABW_btnKey2) addSymbolToRenameFilename(str, '2');
	if (widget == ABW_btnKey3) addSymbolToRenameFilename(str, '3');
	if (widget == ABW_btnKey4) addSymbolToRenameFilename(str, '4');
	if (widget == ABW_btnKey5) addSymbolToRenameFilename(str, '5');
	if (widget == ABW_btnKey6) addSymbolToRenameFilename(str, '6');
	if (widget == ABW_btnKey7) addSymbolToRenameFilename(str, '7');
	if (widget == ABW_btnKey8) addSymbolToRenameFilename(str, '8');
	if (widget == ABW_btnKey9) addSymbolToRenameFilename(str, '9');
	if (widget == ABW_btnKeyQ) addSymbolToRenameFilename(str, 'q');
	if (widget == ABW_btnKeyW) addSymbolToRenameFilename(str, 'w');
	if (widget == ABW_btnKeyE) addSymbolToRenameFilename(str, 'e');
	if (widget == ABW_btnKeyR) addSymbolToRenameFilename(str, 'r');
	if (widget == ABW_btnKeyT) addSymbolToRenameFilename(str, 't');
	if (widget == ABW_btnKeyY) addSymbolToRenameFilename(str, 'y');
	if (widget == ABW_btnKeyU) addSymbolToRenameFilename(str, 'u');
	if (widget == ABW_btnKeyI) addSymbolToRenameFilename(str, 'i');
	if (widget == ABW_btnKeyO) addSymbolToRenameFilename(str, 'o');
	if (widget == ABW_btnKeyP) addSymbolToRenameFilename(str, 'p');
	if (widget == ABW_btnKeyA) addSymbolToRenameFilename(str, 'a');
	if (widget == ABW_btnKeyS) addSymbolToRenameFilename(str, 's');
	if (widget == ABW_btnKeyD) addSymbolToRenameFilename(str, 'd');
	if (widget == ABW_btnKeyF) addSymbolToRenameFilename(str, 'f');
	if (widget == ABW_btnKeyG) addSymbolToRenameFilename(str, 'g');
	if (widget == ABW_btnKeyH) addSymbolToRenameFilename(str, 'h');
	if (widget == ABW_btnKeyJ) addSymbolToRenameFilename(str, 'j');
	if (widget == ABW_btnKeyK) addSymbolToRenameFilename(str, 'k');
	if (widget == ABW_btnKeyL) addSymbolToRenameFilename(str, 'l');
	if (widget == ABW_btnKeyZ) addSymbolToRenameFilename(str, 'z');
	if (widget == ABW_btnKeyX) addSymbolToRenameFilename(str, 'x');
	if (widget == ABW_btnKeyC) addSymbolToRenameFilename(str, 'c');
	if (widget == ABW_btnKeyV) addSymbolToRenameFilename(str, 'v');
	if (widget == ABW_btnKeyB) addSymbolToRenameFilename(str, 'b');
	if (widget == ABW_btnKeyN) addSymbolToRenameFilename(str, 'n');
	if (widget == ABW_btnKeyM) addSymbolToRenameFilename(str, 'm');
	if (widget == ABW_btnKeyUnderline) addSymbolToRenameFilename(str, '_');
	if (widget == ABW_btnKeyPoint) addSymbolToRenameFilename(str, '.');
	return( Pt_CONTINUE );
}

int
renameErrOK( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtWidgetToBack(AbGetABW(ABN_paneRenameErr));
	return( Pt_CONTINUE );
}


//////////////////////////////////////////////////////////////////
////////////////////   new selPane		//////////////////////////
//////////////////////////////////////////////////////////////////

//todo move to libDoc START

void timeConverter(char* buffStr, DOC_TIME_t rawTime, uint16_t opts){
	DOC_TIME_t tTime;

	if(opts & DOC_TIME_CONVERTER_OPT_NO_MS){
		tTime = rawTime;
	}else{
		tTime = rawTime/100;
	}

	if(opts & DOC_TIME_CONVERTER_OPT_FORMAT_ADD_DMY){
		strftime(buffStr, 21 ,"%d-%m-%Y  %H:%M:%S",	localtime(&tTime));
	}else{
		strftime(buffStr, 9 ,"%H:%M:%S",	localtime(&tTime));
	}

	if( !(opts & DOC_TIME_CONVERTER_OPT_NO_MS) ){
		if( !(opts & DOC_TIME_CONVERTER_OPT_FORMAT_NO_MS) ){
			char mSecStr[4];
			sprintf(mSecStr,".%d", (rawTime%100)/10);
			strcat(buffStr, mSecStr);
		}
	}
	if(opts & DOC_TIME_CONVERTER_OPT_FORMAT_ADD_ENDL){
		strcat(buffStr, "\n");
	}
}

int docMsgCount(const int fd){
	DOC_MSG_COUNT_t msgCount = 0;
	if(fd){
		long int curPos = 0;

		curPos = tell(fd);
		lseek(fd, -sizeof(msgCount), SEEK_END);
		read(fd, &msgCount, sizeof(msgCount));
		lseek(fd, curPos, SEEK_SET);
	}
	return msgCount;
}

int docGetStartStopTime(DocFileStruct_t *docFile){
	int fd = -1;
	off_t offSet = 0;
	DOC_SIZE_t msgSize = 0;
	DOC_TIME_t time = 0;
	DOC_MSG_COUNT_t msgCount = 0;
//	const uint8_t docPreambula[] = DOC_PREAMBULA;


	fd = open(docFile->filePath, O_RDONLY);
	if(fd == -1){
		return errno;
	}
//	if( checkVerDocFile(docFile->filePath) == EOK){
//		offSet = sizeof(docPreambula) + DOC_PROJECT_SIZE + DOC_BOS_ID_SIZE;
//		lseek(fd, offSet , SEEK_SET);
//	}

	lseek(fd, sizeof(msgSize) + offSet, SEEK_SET);
	read(fd, &docFile->startTime, sizeof(time));

	lseek(fd, -( sizeof(msgCount) + sizeof(msgSize) ), SEEK_END);
	read(fd, &msgSize, sizeof(msgSize));
	read(fd, &docFile->msgCount, sizeof(msgCount));

	lseek(fd, -( sizeof(msgCount)+ sizeof(msgSize) + msgSize ), SEEK_END);
	read(fd, &docFile->stopTime, sizeof(time));

	close(fd);
	return EOK;
}


int docMergeFiles(const char *firstFilePath, const char *secondFilePath){
	int fd1 = -1;
	int fd2 = -1;
	int sizeBuff = 0;
	char buff[1000];
//	DOC_BOS_ID_t BosID_1 = 0;
//	DOC_PROJECT_t ProjID_1 = 0;
//	DOC_BOS_ID_t BosID_2 = 0;
//	DOC_PROJECT_t ProjID_2 = 0;

	DOC_MSG_COUNT_t	sumMsgCount = 0;

//	if( checkVerDocFile(firstFilePath) == EOK){
//		if(	docGetProjBosID(firstFilePath, &ProjID_1, &BosID_1) != EOK){
//			return (EOK+1);
//		}
//		if( checkVerDocFile(secondFilePath) == EOK){
//			if(	docGetProjBosID(firstFilePath, &ProjID_2, &BosID_2) != EOK){
//				return (EOK+1);
//			}
//
//			if((ProjID_1 != ProjID_2) || (BosID_1 != BosID_2)){
//				return (EOK+1);
//			}
//		}
//	}

	fd1 = open(firstFilePath, O_RDWR);
	if(fd1 == -1){
		fprintf(stderr, "%s: %s %s \n", __FUNCTION__, firstFilePath, strerror(errno));
		return errno;
	}

	fd2 = open(secondFilePath, O_RDWR);
	if(fd2 == -1){
		fprintf(stderr, "%s: %s %s \n", __FUNCTION__, firstFilePath, strerror(errno));
		return errno;
	}

	sumMsgCount = docMsgCount(fd1) + docMsgCount(fd2);


	lseek(fd1, -sizeof(DOC_MSG_COUNT_t), SEEK_END);
//	if( checkVerDocFile(secondFilePath) == EOK){
//		const uint8_t docPreambula[] = DOC_PREAMBULA;
//		lseek(fd2, sizeof(docPreambula) + DOC_PROJECT_SIZE + DOC_BOS_ID_SIZE , SEEK_SET);
//	}

	while( (sizeBuff=read(fd2, buff, sizeof(buff))) ){
		write(fd1,buff, sizeBuff);
	}

	lseek(fd1, -sizeof(DOC_MSG_COUNT_t), SEEK_END);
	write(fd1, &sumMsgCount, sizeof(sumMsgCount));

	close(fd1);
	close(fd2);

	sprintf(buff, "mv %s %s", firstFilePath, secondFilePath);
	system(buff);

	return EOK;
}

int docSplitFilesReOpenTmpFile( const char *tmpFilePath, const char *srcFilePath, int *fd, const int oflag, uint32_t fileTime, DOC_MSG_COUNT_t *MsgCount){
	char buff[300];
	char *pLog = NULL;
	static char parth[4] = "_p0";

	write(*fd, MsgCount, sizeof(*MsgCount));
	close(*fd);

	memset(buff, 0, sizeof(buff));

	pLog = strstr(srcFilePath,".log");

	sprintf(buff, "mv %s ", tmpFilePath);
	strncat(buff, srcFilePath,  strlen(srcFilePath) - strlen(pLog) );

	strcat(buff, parth);
	strcat(buff, ".log");

	system(buff);

	*MsgCount = 0;
	if(oflag){
		parth[2]++;
		return open(tmpFilePath, oflag);
	}else{
		parth[2] = '0';
		return EOK;
	}
	return -1;
}

int docSplitFiles(const char *filePath, const uint32_t startSplitTime, const uint32_t stopSplitTime){
	int fd = -1;
	int fdTemp = -1;
	enum fileParth{eFirst,eMidle,eLast};
	char buff[1000];
	DOC_MSG_COUNT_t splitMsgCount = 0;
	int 	 i = 0;
	char tmpFileName[50] = "/tmp/tmpFile";
	enum fileParth curFilePath;
//	char bNewFormat = 0;
//	DOC_BOS_ID_t BosID = COMPLEX_BOS_ID_SMALL;
//	DOC_PROJECT_t ProjID = DOC_PROJ_ID;

	DOC_SIZE_t msgSize = 0;
	DOC_TIME_t *msgTime = NULL;

	DocFileStruct_t	docFile;

	if( (startSplitTime > stopSplitTime) && (stopSplitTime != 0) ){
		return EBADMSG;
	}

	memset(&docFile, 0, sizeof(docFile));
	strcpy(docFile.filePath, filePath);

	docGetStartStopTime(&docFile);

//	if( checkVerDocFile(docFile.filePath) == EOK){
//		bNewFormat = 1;
//		if(	docGetProjBosID(docFile.filePath, &ProjID, &BosID) != EOK){
//			return (EOK+1);
//		}
//		setDocVar(ProjID, BosID);
//	}

	if( (( docFile.startTime > startSplitTime) || (startSplitTime > docFile.stopTime)) 		||
		((stopSplitTime > 0) && (stopSplitTime > docFile.stopTime))	){
		return EBADMSG;
	}

	fd = open(filePath, O_RDONLY);
	if(fd == -1){
		fprintf(stderr, "%s: %s %s \n", __FUNCTION__, filePath, strerror(errno));
		return errno;
	}

	curFilePath = eFirst;

	fdTemp = open(tmpFileName, O_CREAT | O_TRUNC | O_WRONLY);
	if(fdTemp == -1){
		fprintf(stderr, "%s: %s %s \n", __FUNCTION__, tmpFileName, strerror(errno));
		return errno;
	}

//	if(bNewFormat){
//		writePreambulaPrjIdBosTypeToDocFile(fdTemp, ProjID, BosID);
//	}

	for(i = 0; i < docFile.msgCount; i++){
		read(fd, &msgSize, sizeof(msgSize));
		read(fd, buff, (msgSize + sizeof(msgSize)) );
		msgTime = (DOC_TIME_t *)buff;

		switch (curFilePath) {
			case eFirst:
				if(*msgTime > startSplitTime){
					if(stopSplitTime != 0){
						curFilePath = eMidle;
					}else{
						curFilePath = eLast;
					}

					fdTemp = docSplitFilesReOpenTmpFile(tmpFileName, docFile.filePath, &fdTemp, O_CREAT | O_TRUNC | O_WRONLY, startSplitTime, &splitMsgCount);
					if(fdTemp == -1){
						fprintf(stderr, "%s: %s %s \n", __FUNCTION__, tmpFileName, strerror(errno));
						return errno;
//					}else{
//						if(bNewFormat){
//							writePreambulaPrjIdBosTypeToDocFile(fdTemp, ProjID, BosID);
//						}
					}
				}
				break;
			case eMidle:
				if(*msgTime > stopSplitTime){
					curFilePath = eLast;

					fdTemp = docSplitFilesReOpenTmpFile(tmpFileName, docFile.filePath, &fdTemp, O_CREAT | O_TRUNC | O_WRONLY, stopSplitTime, &splitMsgCount);
					if(fdTemp == -1){
						fprintf(stderr, "%s: %s %s \n", __FUNCTION__, tmpFileName, strerror(errno));
						return errno;
//					}else{
//						if(bNewFormat){
//							writePreambulaPrjIdBosTypeToDocFile(fdTemp, ProjID, BosID);
//						}
					}
				}
				break;
			case eLast:				break;
		}
		++splitMsgCount;
		write(fdTemp, &msgSize, sizeof(msgSize));
		write(fdTemp, buff, (msgSize + sizeof( msgSize )));
	}

	docSplitFilesReOpenTmpFile(tmpFileName, docFile.filePath, &fdTemp, 0, docFile.stopTime, &splitMsgCount);

	return EOK;
}

//todo move to libDoc END


void changeBlockState(PtWidget_t *widget, enum enumBool blockFlag){
	unsigned long    *flags;
	unsigned long	blockFlags;

	PtGetResource(widget, Pt_ARG_FLAGS, &flags, 0);
	if(blockFlag){
		blockFlags = (*flags)|Pt_BLOCKED;
	}else{
		blockFlags = (*flags)&(~Pt_BLOCKED);
	}
	PtSetResource(widget, Pt_ARG_FLAGS, blockFlags, Pt_TRUE);
}


// Send a Phantom release event to a widget.
int Phantom( PtWidget_t *widget, PhEvent_t *event ){
//	PhPointerEvent_t *event_data;
//	if (cbinfo->event->type != Ph_EV_BUT_RELEASE) {
//		printf ("Это не событие Ph_EV_BUT_RELEASE\n");
//	}
//	else {
//		printf ("Это событие Ph_EV_BUT_RELEASE\n");
//		event_data = (PhPointerEvent_t *) PhGetData (cbinfo->event);
//		if (event_data->key_mods & Pk_KM_Shift ) printf (" Shift\n");
//		if (event_data->key_mods & Pk_KM_Ctrl ) printf (" Ctrl\n");
//		if (event_data->key_mods & Pk_KM_Alt ) printf (" Alt\n");
//	}



  struct{
    PhEvent_t event;
    PhRect_t  rect;
    PhPointerEvent_t pevent;
  } new_event;

  memset( &new_event, 0 ,sizeof( new_event ) );
  memset( &new_event.rect, -1 ,sizeof( new_event.rect ) );

  if( event ) {
    new_event.event = *event;
  }

  new_event.event.processing_flags = Ph_FAKE_EVENT;
  new_event.event.type = Ph_EV_BUT_PRESS;
  new_event.pevent.click_count = 1;
  new_event.pevent.buttons = Ph_BUTTON_SELECT;
  new_event.event.num_rects = 1;

  PtSendEventToWidget( widget, (PhEvent_t *) &new_event);

  new_event.event.processing_flags = Ph_FAKE_EVENT;
  new_event.event.type = Ph_EV_BUT_RELEASE;
  new_event.event.subtype = Ph_EV_RELEASE_REAL;
  new_event.pevent.click_count = 1;
  new_event.pevent.buttons = Ph_BUTTON_SELECT;
  new_event.event.num_rects = 1;

  PtSendEventToWidget( widget, (PhEvent_t *) &new_event);

  return 0;
}



int compareLogFiles( const void* op1, const void* op2 ){
    const char **p1 = (const char **) op1;
    const char **p2 = (const char **) op2;

    return( strcmp( *p1, *p2 ) );
}

// Загрузка списка док файлов
void loadLogFiles (char *path){
	int i;
	DIR* dirp;
	struct dirent* direntp;
	char **items;
    int n = 0;

	dirp = opendir(path);
	if( dirp == NULL ) {
		 perror( "can't open dir\n" );
	 }else{
		 for(i=-1;;){
			 direntp = readdir( dirp );
	         if( direntp == NULL ) break;
	         if(testLogFilesType(direntp->d_name)){
	        	 i++;
	         }
	     }
		 n = i + 1;
		 items = (char **)malloc( n * sizeof(char *));
		 if(items == NULL){
			 printf("Not enough memory. (%s  : %d)\n",__FUNCTION__, __LINE__);
			 return;
		 }
		 rewinddir( dirp );
		 printf("in dir %s - %d log files\n", path, i);

		 for(i=-1;;){
			 direntp = readdir( dirp );
	         if( direntp == NULL ){
	        	 break;
	         }
	         if(testLogFilesType(direntp->d_name)){
	        	 i++;
	        	 items[i] = (char *)malloc( strlen(direntp->d_name) + 1);
	    		 if(items[i] == NULL){
	    			 printf("Not enough memory. (%s  : %d)\n",__FUNCTION__, __LINE__);
	    			 return;
	    		 }
	    		 strcpy(items[i], direntp->d_name);
	         }
	     }

 	     qsort( items, n, sizeof(char *),  compareLogFiles );

		 PtListAddItems(ABW_PtList_LogFiles, (const char **)items, n, 0);
		 for (i = 0; i < n; i++){
			 if(items[i] != NULL){
				free(items[i]);
			 }
		 }
		 free(items);
	 }
}

char 	**logFilesItems = NULL;
int		logFilesQty = 0;
int 	curTopLogLabelsPos=0;
int 	curTopLogMergeLabelsPos=0;
int		logMergeFilesQty = 0;
char 	**logMergeFilesItems = NULL;
PtWidget_t *selectedLogLabel = NULL;
PtWidget_t *selectedLogMergeLabel = NULL;
int qtyVisibleLogFiles = 15;
int qtyVisibleLogMergeFiles = 5;



void clearLogLabels(){
	char str[100];
	sprintf(str,"  ");
	int i=0;
	for(i = 0; i < qtyVisibleLogFiles; i++){
		PtSetResource(AbGetABW(ABN_PtLabel_PaneLofFiles_str_1 + i),Pt_ARG_TEXT_STRING, str, 0);
	}

}
void clearLogMergeLabels(){
	char str[100];
	sprintf(str,"  ");
	int i=0;
	for(i = 0; i < qtyVisibleLogMergeFiles; i++){
		PtSetResource(AbGetABW(ABN_PtLabel_PaneLogFilesToMerge_str_1 + i),Pt_ARG_TEXT_STRING, str, 0);
	}

}
void loadLogFilesToLabelsInPane(char *path){
	int i;
	DIR* dirp;
	struct dirent* direntp;
    int n = 0;

	dirp = opendir(path);
	if( dirp == NULL ) {
		 perror( "can't open dir\n" );
	 }else{
		 for(i=-1;;){
			 direntp = readdir( dirp );
	         if( direntp == NULL ) break;
	         if(testLogFilesType(direntp->d_name)){
	        	 i++;
	         }
	     }
		 n = i + 1;
		 if(logFilesItems != NULL){
			 for (i = 0; i < logFilesQty; i++){
				 if(logFilesItems[i] != NULL){
					free(logFilesItems[i]);
					logFilesItems[i] = NULL;
				 }
			 }
			 free(logFilesItems);
			 logFilesItems = NULL;
		 }
		 logFilesQty = n;
		 logFilesItems = (char **)calloc( n , sizeof(char *));
//		 logFilesItems = (char **)malloc( n * sizeof(char *));
		 if(logFilesItems == NULL){
			 printf("Not enough memory. (%s  : %d)\n",__FUNCTION__, __LINE__);
			 return;
		 }
		 rewinddir( dirp );
		 printf("in dir %s - %d log files\n", path, i);

		 for(i=-1;;){
			 direntp = readdir( dirp );
	         if( direntp == NULL ){
	        	 break;
	         }
	         if(testLogFilesType(direntp->d_name)){
	        	 i++;
//	        	 logFilesItems[i] = (char *)malloc( strlen(direntp->d_name) + 1);
	        	 logFilesItems[i] = (char *)calloc( strlen(direntp->d_name) + 1, sizeof(char));

	    		 if(logFilesItems[i] == NULL){
	    			 printf("Not enough memory. (%s  : %d)\n",__FUNCTION__, __LINE__);
	    			 return;
	    		 }
	    		 strcpy(logFilesItems[i], direntp->d_name);
	         }
	     }

 	     qsort( logFilesItems, n, sizeof(char *),  compareLogFiles );
// 	     for(i = 0; i < n; i++){
// 			printf("%s pos[%d] str[%s]\n", __FUNCTION__, i, logFilesItems[i]);
// 	     }


 		for(i = 0; i < qtyVisibleLogFiles && i < logFilesQty; i++){
 			PtSetResource(AbGetABW(ABN_PtLabel_PaneLofFiles_str_1 + i),Pt_ARG_TEXT_STRING, logFilesItems[i], 0);
 			PtSetResource(AbGetABW(ABN_PtLabel_PaneLofFiles_str_1 + i),Pt_ARG_FLAGS, Pt_SELECTABLE, Pt_TRUE);

 		}
 		curTopLogLabelsPos = 0;
		PtSetResource(ABW_PtScrollbar_PaneLogFiles,Pt_ARG_MAXIMUM, logFilesQty, 0);

	 }
}
void refresh(){
	if(USE_PANE_IN_PANESEL){
		if(selectedLogLabel != NULL){
			qtyVisibleLogFiles = 15;
			PtSetResource(selectedLogLabel,Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
			selectedLogLabel = NULL;
			PhDim_t dim;
			dim.w = 35;
			dim.h = 455;
			PtSetResource(ABW_PtScrollbar_PaneLogFiles,Pt_ARG_DIM, &dim, 0);
			PtSetResource(ABW_PtScrollbar_PaneLogFiles,Pt_ARG_SLIDER_SIZE, qtyVisibleLogFiles, 0);

		}
 	    clearLogLabels();
		loadLogFilesToLabelsInPane(docPath);
	}else{
		PtListDeleteAllItems(ABW_PtList_LogFiles);
		loadLogFiles(docPath);
	}

	PtSetResource( AbGetABW(ABN_fileDelete),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);

	PtSetResource( AbGetABW(ABN_fileMerge),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);
	PtSetResource( AbGetABW(ABN_fileSplit),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);
	PtSetResource( AbGetABW(ABN_fileRename),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);

	PtSetResource(AbGetABW(ABN_fileState), Pt_ARG_TEXT_STRING, "Файл архива\nне выбран", 0);
	PtSetResource(AbGetABW(ABN_fileState),Pt_ARG_FLAGS, Pt_GHOST, Pt_TRUE);
	PtSetResource(AbGetABW(ABN_fileName), Pt_ARG_TEXT_STRING, "", 0);


	if(USE_PANE_IN_PANESEL){
		PtWidgetToFront(ABW_PtPane_LogFiles);
	}else{
		PtSetResource( ABW_PtList_LogFiles,Pt_ARG_VISIBLE_COUNT, 12, 0);
		PtWidgetToFront(ABW_PtList_LogFiles);
	}
	curSplitMergeButtonState = eNone;
	PtSetResource( AbGetABW(ABN_fileMergeSplitApply),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);

	fileSellect = -1;
}

int splitScrollMoved( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char timeStr[20];
	PtScrollbarCallback_t  *cbi = (PtScrollbarCallback_t *)cbinfo->cbdata;
	strftime(timeStr, sizeof(timeStr) ,"%H:%M:%S",	localtime((time_t*)&cbi->position));
	PtSetResource(ABW_PtLabel_Split_TimeCurent, Pt_ARG_TEXT_STRING, timeStr, 0);
	PtSetResource( AbGetABW(ABN_fileMergeSplitApply),Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);

	return( Pt_CONTINUE );
}


int selToMerge( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){
	/* eliminate 'unreferenced' warnings */
//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	memcpy(&selectedMergeListStuct, (PtListCallback_t *)cbinfo->cbdata, sizeof(selectedMergeListStuct));
	PtSetResource( AbGetABW(ABN_fileMergeSplitApply),Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);

	return( Pt_CONTINUE );
}


int refreshFile( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){
	// Загрузка списка файлов
	refresh();
	return( Pt_CONTINUE );
}

int fileSplit( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )	{
	DocFileStruct_t 	docFile;
	char timeStr[20];
	time_t tTime;


	char *name;
	PtGetResource(AbGetABW(ABN_fileName), Pt_ARG_TEXT_STRING, &name, 0);
//	sprintf(docFile.filePath, "%s/%s", docPath,  selectedListStuct.item);
	sprintf(docFile.filePath, "%s/%s", docPath,  name);

	docGetStartStopTime(&docFile);

	if(USE_PANE_IN_PANESEL){
		PhDim_t dim;
		dim.w = 35;
		dim.h = 300;
		qtyVisibleLogFiles = 10;
		PtSetResource(ABW_PtScrollbar_PaneLogFiles,Pt_ARG_DIM, &dim, 0);
		PtSetResource(ABW_PtScrollbar_PaneLogFiles,Pt_ARG_SLIDER_SIZE, qtyVisibleLogFiles, 0);

	}else{
		PtSetResource( ABW_PtList_LogFiles,Pt_ARG_VISIBLE_COUNT, 7, 0);
	}
	PtWidgetToFront(ABW_PtPane_Split);

	curSplitMergeButtonState = eSplit;

	tTime = docFile.startTime/100;
	strftime(timeStr, 20 ,"%H:%M:%S",	localtime(&tTime));
	PtSetResource(ABW_PtLabel_Split_TimeStart, Pt_ARG_TEXT_STRING, timeStr, 0);
	tTime = docFile.stopTime/100;
	strftime(timeStr, 20 ,"%H:%M:%S",	localtime(&tTime));
	PtSetResource(ABW_PtLabel_Split_TimeEnd, Pt_ARG_TEXT_STRING, timeStr, 0);
	PtSetResource(ABW_PtLabel_Split_TimeCurent, Pt_ARG_TEXT_STRING, timeStr, 0);

	PtSetResource( ABW_PtScrollbar_Split,Pt_ARG_MINIMUM, docFile.startTime/100, 0);
	PtSetResource( ABW_PtScrollbar_Split,Pt_ARG_MAXIMUM, docFile.stopTime/100, 0);
	PtSetResource( ABW_PtScrollbar_Split,Pt_ARG_SCROLL_POSITION, docFile.stopTime/100, 0);

	return( Pt_CONTINUE );
}


int fileMerge( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )	{
	PtArg_t args[1];
	short *num, i;
	char **items = NULL;
	DocFileStruct_t 	selectedDocFile;
	DocFileStruct_t 	docFile;
	const char *item;

	curSplitMergeButtonState = eMerge;

	if(USE_PANE_IN_PANESEL){
		PhDim_t dim;
		dim.w = 35;
		dim.h = 300;
		qtyVisibleLogFiles = 10;
		PtSetResource(ABW_PtScrollbar_PaneLogFiles,Pt_ARG_DIM, &dim, 0);
		PtSetResource(ABW_PtScrollbar_PaneLogFiles,Pt_ARG_SLIDER_SIZE, qtyVisibleLogFiles, 0);

		PtWidgetToFront(ABW_PtPane_LogFilesToMerge);
		clearLogMergeLabels();
		if(logMergeFilesItems != NULL){
			// DONT FREE ELLEMENTS!!!! - it will be free in logFilesItems
//			for (i = 0; i < logMergeFilesQty; i++){
//				if(logMergeFilesItems[i] != NULL){
//					free(logMergeFilesItems[i]);
//					logMergeFilesItems[i] = NULL;
//				}
//			}
			free(logMergeFilesItems);
			logMergeFilesItems = NULL;
		}
		curTopLogMergeLabelsPos=0;
		logMergeFilesQty = 0;
	}else{
		PtSetResource( ABW_PtList_LogFiles,Pt_ARG_VISIBLE_COUNT, 7, 0);
		PtWidgetToFront(ABW_PtList_LogFilesToMerge);
		PtListDeleteAllItems(ABW_PtList_LogFilesToMerge);
		PtSetArg(&args[0], Pt_ARG_ITEMS, &items, &num);
		PtGetResources(ABW_PtList_LogFiles, 1, args);
	}


	char *name;
	PtGetResource(AbGetABW(ABN_fileName), Pt_ARG_TEXT_STRING, &name, 0);
	sprintf(selectedDocFile.filePath, "%s/%s", docPath,  name);
//	sprintf(selectedDocFile.filePath, "%s/%s", docPath,  selectedListStuct.item);
	docGetStartStopTime(&selectedDocFile);

	printf("selectedDocFile s:%d  e:%d\n", selectedDocFile.startTime, selectedDocFile.stopTime);

	if(USE_PANE_IN_PANESEL){
		for (i = 0; i < logFilesQty; i++){
			if (strcmp(logFilesItems[i], name)==0){
				continue;
			}
			sprintf(docFile.filePath, "%s/%s", docPath,  logFilesItems[i]);
			docGetStartStopTime(&docFile);

//			printf("Item %d: %s \t s:%d  e:%d\n", i, logFilesItems[i], docFile.startTime, docFile.stopTime);

			if(docFile.stopTime == 0){
				continue;
			}
			if( ((selectedDocFile.startTime - docFile.stopTime) < 1000) ||
				((docFile.startTime - selectedDocFile.stopTime) < 1000)){
				logMergeFilesQty++;
//				 item = logFilesItems[i];
//				 PtListAddItems(ABW_PtList_LogFilesToMerge, &item, 1, 0);
			}
		}
		if( logMergeFilesQty ){
//			logMergeFilesItems = (char **)malloc( logMergeFilesQty + 1 * sizeof(char *));
			logMergeFilesItems = (char **)calloc( logMergeFilesQty + 1, sizeof(char *));
			if(logMergeFilesItems == NULL){
				printf("Not enough memory. (%s  : %d)\n",__FUNCTION__, __LINE__);
				logMergeFilesQty = 0;
				return( Pt_CONTINUE );
			 }
			 int k = 0;
			 for (i = 0; i < logFilesQty; i++){
				if (strcmp(logFilesItems[i], name)==0){
					continue;
				}
				sprintf(docFile.filePath, "%s/%s", docPath,  logFilesItems[i]);
				docGetStartStopTime(&docFile);

//				printf(" ! Item %d: %s \t s:%d  e:%d\n", i, logFilesItems[i], docFile.startTime, docFile.stopTime);

				if(docFile.stopTime == 0){
					continue;
				}
				if( ((selectedDocFile.startTime - docFile.stopTime) < 100) ||
					((docFile.startTime - selectedDocFile.stopTime) < 100)){
					if(k < logMergeFilesQty){
						printf("%s  ", logFilesItems[i]);
						logMergeFilesItems[k] = logFilesItems[i];
						printf("%s\n  ", logMergeFilesItems[k]);
						k++;
					}else{
						break;
					}
				}
			}
			for(i = 0; i < qtyVisibleLogMergeFiles && i < logMergeFilesQty; i++){
				PtSetResource(AbGetABW(ABN_PtLabel_PaneLogFilesToMerge_str_1 + i),Pt_ARG_TEXT_STRING, logMergeFilesItems[i], 0);
				PtSetResource(AbGetABW(ABN_PtLabel_PaneLogFilesToMerge_str_1 + i),Pt_ARG_FLAGS, Pt_SELECTABLE, Pt_TRUE);
			}
			curTopLogMergeLabelsPos = 0;
			PtSetResource(ABW_PtScrollbar_PaneLogFilesToMerge,Pt_ARG_MAXIMUM, logMergeFilesQty, 0);

		}


	}else{
		for (i = 0; i < *num; i++){

			sprintf(docFile.filePath, "%s/%s", docPath,  items[i]);
			docGetStartStopTime(&docFile);

			printf("Item %d: %s \t s:%d  e:%d\n", i, items[i], docFile.startTime, docFile.stopTime);

			if(docFile.stopTime == 0){
				continue;
			}
			if( ((selectedDocFile.startTime - docFile.stopTime) < 100) ||
				((docFile.startTime - selectedDocFile.stopTime) < 100)){
				 item = items[i];
				 PtListAddItems(ABW_PtList_LogFilesToMerge, &item, 1, 0);
			}
		}
	}




	return( Pt_CONTINUE );

}


int fileSplitMergeApply( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){

	switch (curSplitMergeButtonState) {
		case eMerge:
			if(1){
				DocFileStruct_t 	selectedDocFile;
				DocFileStruct_t 	selectedMergeDocFile;

				char *name;
				PtGetResource(selectedLogLabel, Pt_ARG_TEXT_STRING, &name, 0);
				sprintf(selectedDocFile.filePath, "%s/%s", docPath, name);
//				sprintf(selectedDocFile.filePath, "%s/%s", docPath, selectedListStuct.item);
				docGetStartStopTime(&selectedDocFile);

				PtGetResource(selectedLogMergeLabel, Pt_ARG_TEXT_STRING, &name, 0);
				sprintf(selectedMergeDocFile.filePath, "%s/%s", docPath, name);
//				sprintf(selectedMergeDocFile.filePath, "%s/%s", docPath, selectedMergeListStuct.item);
				docGetStartStopTime(&selectedMergeDocFile);

				if( ((selectedDocFile.startTime - selectedMergeDocFile.stopTime) < 100) ){
					docMergeFiles(selectedMergeDocFile.filePath, selectedDocFile.filePath);
				}else if((selectedMergeDocFile.startTime - selectedDocFile.stopTime) < 100){
					docMergeFiles(selectedDocFile.filePath, selectedMergeDocFile.filePath);
				}
			}
			break;
		case eSplit:
			if(1){
				long *pos;
				char path[100];

				PtGetResource(ABW_PtScrollbar_Split, Pt_ARG_SCROLL_POSITION, &pos, 0);

				char *name;
				PtGetResource(AbGetABW(ABN_fileName), Pt_ARG_TEXT_STRING, &name, 0);
				sprintf(path, "%s/%s", docPath,  name);
//				sprintf(path, "%s/%s", docPath, selectedListStuct.item);
				docSplitFiles(path, *pos*100, 0);
			}
			break;
		default:
			break;
	}

	refresh();

	return( Pt_CONTINUE );
}





int delDoc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){

	char *name;
	char string[100] = "Удалить ";
	PtGetResource(AbGetABW(ABN_fileName), Pt_ARG_TEXT_STRING, &name, 0);
	strcat(string, name);
	strcat(string, " ?");
	PtSetResource(AbGetABW(ABN_DelFileNameLb), Pt_ARG_TEXT_STRING, &string, 0);
	PtWidgetToFront(AbGetABW(ABN_paneDelFile));
	changeBlockState(ABW_MenuSel, eTrue);
	changeBlockState(ABW_paneSel, eTrue);

	return( Pt_CONTINUE );

}


int pDFRename( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){
	char *str;

	PtGetResource(ABW_fileName, Pt_ARG_TEXT_STRING, &str, 0);
	PtSetResource(ABW_startView, Pt_ARG_FLAGS, Pt_HIGHLIGHTED|Pt_GHOST, Pt_TRUE);
	PtSetResource(ABW_txtFileName, Pt_ARG_TEXT_STRING, str, 0);
	PtWidgetToFront(AbGetABW(ABN_paneRename));
	return( Pt_CONTINUE );
}


int pDFCopy( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){
	return( Pt_CONTINUE );
}


int refreshFileList( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )	{

	refresh();
	return( Pt_CONTINUE );
}


int selArch( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){
	memcpy(&selectedListStuct, (PtListCallback_t *)cbinfo->cbdata, sizeof(selectedListStuct));

	PtSetResource(AbGetABW(ABN_startView),Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);
	PtSetResource(AbGetABW(ABN_fileState), Pt_ARG_TEXT_STRING, "Выбран файл:", 0);
	PtSetResource(AbGetABW(ABN_fileState),Pt_ARG_FLAGS, 0, Pt_TRUE);
	PtSetResource(AbGetABW(ABN_fileName), Pt_ARG_TEXT_STRING, selectedListStuct.item, 0);

	fileSellect = 1;
	PtSetResource( AbGetABW(ABN_fileDelete),Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);
	PtSetResource( AbGetABW(ABN_fileMerge),Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);
	PtSetResource( AbGetABW(ABN_fileSplit),Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);
	PtSetResource(ABW_fileRename, Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);

	PtSetResource( AbGetABW(ABN_fileMergeSplitApply),Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);

	if(curSplitMergeButtonState == eSplit){
		Phantom(ABW_fileSplit, NULL);
	}else if(curSplitMergeButtonState == eMerge){
		Phantom(ABW_fileMerge, NULL);
	}

	return( Pt_CONTINUE );

}


		///PANE SECTION ///////

int paneLogFilesScrollMoved( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtScrollbarCallback_t  *cbi = (PtScrollbarCallback_t *)cbinfo->cbdata;
	int pos = cbi->position;
//	printf("pos=%d\n",pos);
	int i;
	for(i = 0; i < qtyVisibleLogFiles; i++){
		if(i+pos < logFilesQty){
//			printf("%s pos[%d] i+pos[%d] str[%s]\n", __FUNCTION__, pos, i+pos, logFilesItems[i+pos]);
			PtSetResource(AbGetABW(ABN_PtLabel_PaneLofFiles_str_1 + i),Pt_ARG_TEXT_STRING, logFilesItems[i+pos], 0);
		}else{
			PtSetResource(AbGetABW(ABN_PtLabel_PaneLofFiles_str_1 + i), Pt_ARG_TEXT_STRING, " ", 0);
		}

	}
	curTopLogLabelsPos = pos;
//	printf("%s curTopLogLabelsPos[%d]\n", __FUNCTION__, curTopLogLabelsPos);

	if(selectedLogLabel != NULL){
		PtSetResource(selectedLogLabel,Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
	}


	return( Pt_CONTINUE );

}


int paneLogFilesToMergeScrollMoved( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtScrollbarCallback_t  *cbi = (PtScrollbarCallback_t *)cbinfo->cbdata;
	int pos = cbi->position;
	int i;
	for(i = 0; i < qtyVisibleLogMergeFiles; i++){
		if(i+pos < logMergeFilesQty){
//			printf("%s pos[%d] i+pos[%d] str[%s]\n", __FUNCTION__, pos, i+pos, logMergeFilesItems[i+pos]);
			PtSetResource(AbGetABW(ABN_PtLabel_PaneLogFilesToMerge_str_1 + i),Pt_ARG_TEXT_STRING, logMergeFilesItems[i+pos], 0);
		}else{
			PtSetResource(AbGetABW(ABN_PtLabel_PaneLogFilesToMerge_str_1 + i), Pt_ARG_TEXT_STRING, " ", 0);
		}

	}
	curTopLogMergeLabelsPos = pos;
//	printf("%s curTopLogMergeLabelsPos[%d]\n", __FUNCTION__, curTopLogMergeLabelsPos);

	if(selectedLogMergeLabel != NULL){
		PtSetResource(selectedLogMergeLabel,Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
	}
	return( Pt_CONTINUE );
}


int paneSelArch( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char *item;
	if(selectedLogLabel != NULL){
		PtSetResource(selectedLogLabel,Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
	}
	selectedLogLabel = widget;
	PtSetResource( selectedLogLabel, Pt_ARG_FILL_COLOR, Pg_GREEN, 0 );

	PtGetResource(selectedLogLabel, Pt_ARG_TEXT_STRING, &item, 0);

	PtSetResource(ABW_startView,Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);
	PtSetResource(ABW_fileState, Pt_ARG_TEXT_STRING, "Выбран файл:", 0);
	PtSetResource(ABW_fileState,Pt_ARG_FLAGS, 0, Pt_TRUE);
	PtSetResource(ABW_fileName, Pt_ARG_TEXT_STRING, item, 0);

	fileSellect = 1;
	PtSetResource( ABW_fileDelete,Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);
	PtSetResource( ABW_fileMerge,Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);
	PtSetResource( ABW_fileSplit,Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);
	PtSetResource(ABW_fileRename, Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);

	PtSetResource( ABW_fileMergeSplitApply,Pt_ARG_FLAGS, Pt_ITEM_FALSE, Pt_TRUE);

	if(curSplitMergeButtonState == eSplit){
		Phantom(ABW_fileSplit, NULL);
	}else if(curSplitMergeButtonState == eMerge){
		Phantom(ABW_fileMerge, NULL);
	}

	return( Pt_CONTINUE );
}


int paneLogFilesToMergeSelToMerge( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	if(selectedLogMergeLabel != NULL){
		PtSetResource(selectedLogMergeLabel,Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
	}
	selectedLogMergeLabel = widget;
	PtSetResource( selectedLogMergeLabel, Pt_ARG_FILL_COLOR, Pg_GREEN, 0 );
	PtSetResource( AbGetABW(ABN_fileMergeSplitApply),Pt_ARG_FLAGS, Pt_ITEM_TRUE, Pt_TRUE);

	return( Pt_CONTINUE );
}
