/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "global.h"
#include "abimport.h"
#include "proto.h"

extern int readCount;
extern char logName[100][100];

int
doneRename( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
	int i;
	char *strNewName, *strOldName;
	char pathOld[50], pathNew[50];
	memset(&pathOld[0], 0, 50);
	memset(&pathNew[0], 0, 50);

//	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtGetResource(ABW_txtFileName, Pt_ARG_TEXT_STRING, &strNewName, 0);
	for (i = 0; i < readCount; ++i)
	{
//		if (strcmp(strNewName, logName[i]) == 0)
		if (strcmp(strNewName, selectedListStuct.item) == 0)
		{
			PtWidgetToFront(AbGetABW(ABN_paneRenameErr));
			return( Pt_CONTINUE );
		}
	}

	PtGetResource(ABW_fileName, Pt_ARG_TEXT_STRING, &strOldName, 0);

	strcat(pathOld, docPath);
	strcat(pathOld, "/");
	strcat(pathOld, strOldName);
	strcat(pathNew, docPath);
	strcat(pathNew, "/");
	strcat(pathNew, strNewName);

	i = rename(pathOld, pathNew);

	refresh();
	PtWidgetToBack(AbGetABW(ABN_paneRename));

	return( Pt_CONTINUE );

	}

