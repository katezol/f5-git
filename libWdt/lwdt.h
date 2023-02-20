#include <Pt.h>

#ifndef WDT_H_
#define WDT_H_

/// Widget Set
int setWidget(PtWidget_t* widget);
int unsetWidget(PtWidget_t* widget);
int setsWidget(PtWidget_t* widget,int state);
int isWidgetSet(PtWidget_t *widget);

/// Widget Block
int blockWidget(PtWidget_t* widget);
int unblockWidget(PtWidget_t* widget);
int blocksWidget(PtWidget_t* widget,int state);
int isWidgetBlock(PtWidget_t *widget);


/// Widget Text
int		setWidgetText(PtWidget_t* widget,char* txt);
char* 	getWidgetText(PtWidget_t* widget);

/// Int Num
long int getIntNumValue(PtWidget_t* widget);
long int setIntNumValue(PtWidget_t* widget,long value);
/// Float Num
double getFloatNumValue(PtWidget_t* widget);
double setFloatNumValue(PtWidget_t* widget, double value);

// Color
int setWidgetFillColor(PtWidget_t* widget,PgColor_t color);

// Timer
int timerEnable(PtWidget_t* timer,unsigned long value);
int timerOneTic(PtWidget_t* timer,unsigned long value);
int timerDisable(PtWidget_t* timer);

// ComboBox
unsigned short getSelectItem(PtWidget_t* widget);
unsigned short setSelectItem(PtWidget_t* widget,int item);
#endif /*WDT_H_*/
