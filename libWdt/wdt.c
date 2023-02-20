#include "lwdt.h"

/// Widget Set
int setWidget(PtWidget_t* widget){
	return(PtSetResource( widget, Pt_ARG_FLAGS, Pt_TRUE, Pt_SET ));
}
int unsetWidget(PtWidget_t* widget){
	return(PtSetResource( widget, Pt_ARG_FLAGS, Pt_FALSE, Pt_SET ));
}
int setsWidget(PtWidget_t* widget,int state){
	if(state)
		setWidget(widget);
	else 	
		unsetWidget(widget);
	return(state);	
}
int isWidgetSet(PtWidget_t *widget){
	unsigned long    *flags;
	PtGetResource( widget,Pt_ARG_FLAGS, &flags, 0);
	if((*flags) & Pt_SET )
		return(1);
	return(0);	
}

/// Widget Block
int blockWidget(PtWidget_t* widget){
	PtSetResource( widget, Pt_ARG_FLAGS, Pt_TRUE, Pt_BLOCKED );
	return(PtSetResource( widget, Pt_ARG_FLAGS, Pt_TRUE, Pt_GHOST ));
}

int unblockWidget(PtWidget_t* widget){
	PtSetResource( widget, Pt_ARG_FLAGS, Pt_FALSE, Pt_BLOCKED );
	return(PtSetResource( widget, Pt_ARG_FLAGS, Pt_FALSE, Pt_GHOST ));
}

int blocksWidget(PtWidget_t* widget,int state){
	if(state)
		blockWidget(widget);
	else 	
		unblockWidget(widget);
	return(state);	
}

int isWidgetBlock(PtWidget_t *widget){
	unsigned long    *flags;
	PtGetResource( widget,Pt_ARG_FLAGS, &flags, 0);
	if((*flags) & Pt_BLOCKED )
		return(1);
	return(0);	
}

/// Widget Text
int setWidgetText(PtWidget_t* widget,char* txt){
	return(PtSetResource( widget, Pt_ARG_TEXT_STRING, txt, 0 ));
}
char* getWidgetText(PtWidget_t* widget){
	char * txt = NULL;
	PtGetResource( widget, Pt_ARG_TEXT_STRING, &txt, 0 );
	return(txt);
}

/// Int Num
long int getIntNumValue(PtWidget_t* widget){
	//return(((PtNumericIntegerWidget_t*)widget)->value);
	long int *value;
	PtGetResource (widget, Pt_ARG_NUMERIC_VALUE, &value, 0);
	return(*value);
}

long int setIntNumValue(PtWidget_t* widget,long value){
	PtSetResource( widget, Pt_ARG_NUMERIC_VALUE, value, 0 );
	return(value);
}

/// Float Num
double getFloatNumValue(PtWidget_t* widget){
	double *value;
	PtGetResource (widget, Pt_ARG_NUMERIC_VALUE, &value, 0);
	return(*value);
}
double setFloatNumValue(PtWidget_t* widget, double value){
	PtSetResource (widget, Pt_ARG_NUMERIC_VALUE,&value, 0);
	return(value);
}

//Color
int setWidgetFillColor(PtWidget_t* widget,PgColor_t color){
	return(PtSetResource( widget, Pt_ARG_FILL_COLOR, color, 0 ));
}

// Timer
int timerEnable(PtWidget_t* timer,unsigned long value){
	PtArg_t args[2];
	int arg_cnt = 0;
	PtSetArg( &args[arg_cnt++], Pt_ARG_TIMER_INITIAL , value, 0 );
	PtSetArg( &args[arg_cnt++], Pt_ARG_TIMER_REPEAT , value, 0 );
	PtSetResources(timer,arg_cnt,args);
	return(1);
}
int timerOneTic(PtWidget_t* timer,unsigned long value){
	PtArg_t args[2];
	int arg_cnt = 0;
	PtSetArg( &args[arg_cnt++], Pt_ARG_TIMER_INITIAL , value, 0 );
	PtSetResources(timer,arg_cnt,args);
	return(1);
}

int timerDisable(PtWidget_t* timer){
	PtArg_t args[2];	
	int arg_cnt = 0;
	PtSetArg( &args[arg_cnt++], Pt_ARG_TIMER_INITIAL ,0, 0 );
	PtSetArg( &args[arg_cnt++], Pt_ARG_TIMER_REPEAT , 0, 0 );
	PtSetResources(timer,arg_cnt,args);
	return(1);
} 
// ComboBox
unsigned short getSelectItem(PtWidget_t* widget){
	unsigned short *item = NULL;
	PtGetResource( widget, Pt_ARG_CBOX_SEL_ITEM, &item, 0 );
	return(*item);
}	
unsigned short setSelectItem(PtWidget_t* widget,int item){
	PtSetResource( widget, Pt_ARG_CBOX_SEL_ITEM, item, 0 );
	return(item);
}	



