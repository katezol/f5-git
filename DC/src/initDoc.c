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

/* Application Options string */
const char ApOptions[] =
	AB_OPTIONS ""; /* Add your options in the "" */
// AB_OPTIONS "c:"; /* Add your options in the "" */

char docPath[100];
int bParamPD, bParamVr, shipType;
void readSettings(char * _strPath)
{
  	char str_buf[100];
  	char path[100];
    FILE* f;
    int i;
  	int ports = 0;
  	Param params[100];

  	strcpy(path, _strPath);
  	strcat(path, "/DC.ini");
  	printf("fullpath=%s\n",path);

  	f = fopen( path, "r" );
  	if( f == NULL )
  	{
  		printf("DC.ini opening FAILED docPath=%s\n",docPath);
  		return;
  	}
    memset(&params,0,sizeof(params));
    for (i = 0; i < 100; ++i)
    {
        if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) break;
      	sscanf(str_buf, "%s %s \n", params[ports].name,params[ports].path);
      	++ports;
    } // for i
    fclose( f );

    for (i = 0; i < ports; ++i)
    {
    	if ( strncmp("logPath",params[i].name,strlen("logPath")) == 0)
  			strcpy(docPath,params[i].path);
    	if ( strncmp("bParamPD",params[i].name,strlen("bParamPD")) == 0)
    		bParamPD = strtoul( params[i].path, NULL, 10 );
    	if ( strncmp("bParamVr",params[i].name,strlen("bParamVr")) == 0)
    		bParamVr = strtoul( params[i].path, NULL, 10 );
    	if ( strncmp("shipType",params[i].name,strlen("shipType")) == 0)
    		shipType = strtoul( params[i].path, NULL, 10 );
    	
    } // for i

}; // definePorts

int initDoc( int argc, char *argv[] ){
//
//	  int opt;
//	    while ( ( opt = getopt( argc, argv, ApOptions ) ) != -1 ){
//	        switch ( opt ) {
//				case 'c' :
//					printf("conf file %s\n", optarg);
//					break;
//	        }
//		}


	// use -s key to set photon dev !!!!!!

	if(argc>1){
		strcpy(pathPhoton, argv[1]);
	}else{
		strcpy(pathPhoton, "");
	}
	/* eliminate 'unreferenced' warnings */
//	argc = argc, argv = argv;
	readSettings((char *)dirname(argv[0])); // чтение настроек из DC.ini,
	// DC.ini должен находиться в рабочем дир, а в дебуге - в /temp
	
	return( Pt_CONTINUE );

	}

