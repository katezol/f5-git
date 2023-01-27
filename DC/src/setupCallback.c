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



int
ok( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	unsigned long    *flags;

	PtWidgetToFront(ABW_paneDoc);
	PtWidgetToFront(ABW_MenuDoc);

	return( Pt_CONTINUE );
}


int
cancel( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	PtWidgetToFront(ABW_paneDoc);
	PtWidgetToFront(ABW_MenuDoc);

	return( Pt_CONTINUE );
}


int
ex( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{

	return( Pt_CONTINUE );

	}

