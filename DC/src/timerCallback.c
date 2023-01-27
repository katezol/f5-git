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
timer( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/*static int counter = 0;
	printf("Timer ...\n");
	if(startDoc==0)
		return(Pt_CONTINUE);
	counter ++;
	if(counter%playSpeed==0)
		counter = 0;
	else
		return(Pt_CONTINUE);
	printf("Timer ...\n");*/

	return( Pt_CONTINUE );
}

