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

struct message* message;
struct setupFlags setupFlags;
int playSpeed;
char Helvetica18[MAX_FONT_TAG];



char pathPhoton[100]="";

int
setup( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	
	if(strcmp(pathPhoton, ""))
		PhAttach(pathPhoton, NULL);

	
	PfGenerateFontName("Helvetica", 0, 9, Helvetica18);
	
	playSpeed = 10;
	
	pointArray.count = 0;
	pointArray.curPos = 0;
	memset(pointTrassa, 0, sizeof(pointTrassa));
	
	pointDraw67.count = 0;
	pointDraw67.curPos = 0;
	
	// Инициализация настроек проигрывания 
	setupFlags.f1[0] = Pt_TRUE;
	setupFlags.f2[0] = Pt_TRUE;
	setupFlags.f2[1] = Pt_TRUE;
	setupFlags.f2[2] = Pt_TRUE;
	setupFlags.f3[0] = Pt_TRUE;
	setupFlags.f3[1] = Pt_TRUE;
	setupFlags.f4[0] = Pt_TRUE;
	setupFlags.f4[1] = Pt_TRUE;
	setupFlags.f4[2] = Pt_TRUE;
	setupFlags.f4[3] = Pt_TRUE;
	setupFlags.f4[4] = Pt_TRUE;
	setupFlags.f4[5] = Pt_TRUE;
	setupFlags.f5[0] = Pt_TRUE;
	setupFlags.f5[1] = Pt_TRUE;
	setupFlags.f5[2] = Pt_TRUE;
	setupFlags.f5[3] = Pt_TRUE;
	setupFlags.f5[4] = Pt_TRUE;
	setupFlags.f5[5] = Pt_TRUE;
	setupFlags.f5[6] = Pt_TRUE;
	setupFlags.f5[7] = Pt_TRUE;
	setupFlags.f5[8] = Pt_TRUE;



	return( Pt_CONTINUE );
}

