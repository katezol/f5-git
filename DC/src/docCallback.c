/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "global.h"
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"


int _startDoc;

// Переход в режим настроек
int docSetup( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	// Установка флагов
		PtWidgetToFront(ABW_paneSetup);
	PtWidgetToFront(ABW_MenuSetup);
	return( Pt_CONTINUE );
}


int
docClose( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidgetToFront(ABW_paneSel);
	if(USE_PANE_IN_PANESEL){
		PtWidgetToFront(ABW_PtPane_LogFiles);
	}
	PtWidgetToFront(ABW_MenuSel);
	return( Pt_CONTINUE );
}


int
docStart( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	if(_startDoc){	// Остановка воспроизведения
		_startDoc = 0;
		// Разблокировка
		PtSetResource(AbGetABW(ABN_docSlideTime),Pt_ARG_FLAGS,0, Pt_TRUE);
		PtSetResource(AbGetABW(ABN_docSlidePos),Pt_ARG_FLAGS, Pt_HIGHLIGHTED, Pt_TRUE);
		PtSetResource(AbGetABW(ABN_docSetup),Pt_ARG_FLAGS, Pt_HIGHLIGHTED|Pt_SELECTABLE, Pt_TRUE);
		PtSetResource(AbGetABW(ABN_closeLog),Pt_ARG_FLAGS, Pt_HIGHLIGHTED|Pt_SELECTABLE, Pt_TRUE);
	}else{			// Запуск воспроизведени
		_startDoc = 1;
		// Блокировка
		PtSetResource(AbGetABW(ABN_docSlideTime),Pt_ARG_FLAGS, Pt_GHOST|Pt_BLOCKED, Pt_TRUE);
		PtSetResource(AbGetABW(ABN_docSlidePos),Pt_ARG_FLAGS, Pt_GHOST|Pt_BLOCKED|Pt_HIGHLIGHTED, Pt_TRUE);
		PtSetResource(AbGetABW(ABN_docSetup),Pt_ARG_FLAGS, Pt_GHOST|Pt_BLOCKED, Pt_TRUE);
		PtSetResource(AbGetABW(ABN_closeLog),Pt_ARG_FLAGS, Pt_GHOST|Pt_BLOCKED, Pt_TRUE);
		// Выбор настроек
	}
	
	return( Pt_CONTINUE );
}


int
selTable( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/*for(i=0; i<10; i++)
		if(widget==AbGetABW(ABN_docTableRow1+i))
		{
			PtSetResource(AbGetABW(ABN_docTableMsg1+i), Pt_ARG_FILL_COLOR, 0xBEFFBE, 0);
			PtSetResource(AbGetABW(ABN_docTableTime1+i), Pt_ARG_FILL_COLOR, 0xBEFFBE, 0);
		}else{
			PtSetResource(AbGetABW(ABN_docTableMsg1+i), Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
			PtSetResource(AbGetABW(ABN_docTableTime1+i), Pt_ARG_FILL_COLOR, Pg_WHITE, 0);
		}
*/
	return( Pt_CONTINUE );

}

