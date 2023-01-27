/* Import (extern) header for application - AppBuilder 2.03  */

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApWidget_t AbWidgets[ 236 ];


#ifdef __cplusplus
extern "C" {
#endif
int initDoc( int argc, char **argv );
int setup( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int pDFDell( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int pDFCancel( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int docSetup( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int scrollLog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int Exit( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int cancel( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int okSetup( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int pressKeyboard( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int numb( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int timeScrollMove( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int docPlay( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int docPause( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int playTimer( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int saveTxt( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int openTxt( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int closeLog( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
void drawPane( PtWidget_t *widget, PhTile_t *damage ) 

;
int viewOpen( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int docOpen( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int armView( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int clearView( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int viewScaleUp( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int viewScaleDown( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int faster( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int slower( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int armSliderView( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int disarmSliderView( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int moveSliderView( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int startView( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int keyPressed( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int cancelRename( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int doneRename( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int renameErrOK( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int refreshFileList( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int splitScrollMoved( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int selToMerge( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int selArch( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int delDoc( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int refreshFile( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int pDFRename( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int pDFCopy( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int fileSplit( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int fileMerge( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int fileSplitMergeApply( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int paneLogFilesScrollMoved( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int paneLogFilesToMergeScrollMoved( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int paneSelArch( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int paneLogFilesToMergeSelToMerge( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif
