#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <process.h>
#include <pthread.h>


#include "ltool.h"

#define BILLION  1000000000L

#define _LT_DBG_MSG_MSG 		"-------: "
#define _LT_DBG_MSG_WARNING 	"Warning: "
#define _LT_DBG_MSG_ERROR 		"Error  : "

#define _LT_PDB_ON			(LT_PDB_LEVEL_3)
#define _LT_PDB_OFF  		(LT_PDB_LEVEL_0)

static int _dbg_level 		= _LT_PDB_ON;
static int _dbg_time_mark 	= _LT_PDB_ON;
static int _dbg_pid_mark 	= _LT_PDB_OFF;
static int _dbg_tid_mark 	= _LT_PDB_ON;

int ltDebugGetLevel(){
	return(_dbg_level);
}

int ltDebugSetLevel(int level){
	if(level > _LT_PDB_ON)
		level = _LT_PDB_ON;
	if(level < _LT_PDB_OFF)	
		level = _LT_PDB_OFF;
	_dbg_level = level;
	return(level);	
}

int ltDebugOn(){
	_dbg_level = _LT_PDB_ON;
	return(_dbg_level);
}

int ltDebugOff(){
	_dbg_level = _LT_PDB_OFF;
	return(_dbg_level);
}

int ltDebugTimeMarkOn(){
	_dbg_time_mark = _LT_PDB_ON;
	return(_dbg_time_mark);
}

int ltDebugTimeMarkOff(){
	_dbg_time_mark = _LT_PDB_OFF;
	return(_dbg_time_mark);
}

int ltDebugPidMarkOn(){
	_dbg_pid_mark = _LT_PDB_ON;
	return(_dbg_pid_mark);
}

int ltDebugPidMarkOff(){
	_dbg_pid_mark = _LT_PDB_OFF;
	return(_dbg_pid_mark);
}

int ltDebugTidMarkOn(){
	_dbg_tid_mark = _LT_PDB_ON;
	return(_dbg_tid_mark);
}

int ltDebugTidMarkOff(){
	_dbg_tid_mark = _LT_PDB_OFF;
	return(_dbg_tid_mark);
}



inline int ltDebugErr( const char* format, ... ){
	if(LT_PDB_LEVEL_1 <= _dbg_level){
    	va_list arglist;
    	if(_dbg_pid_mark)
    		printf("[p%d]",getpid());
    	if(_dbg_tid_mark)
    		printf("[t%d]",pthread_self());
		if(_dbg_time_mark){
			struct timespec start;
	   		if( -1 == clock_gettime( CLOCK_REALTIME, &start) )
	   			return(0);
	   		printf("<%f>\t",(start.tv_sec + (double)(start.tv_nsec ) / (double)BILLION));
		}
    	printf(_LT_DBG_MSG_ERROR);
    	va_start( arglist, format );
    	vprintf( format, arglist );
    	va_end( arglist );
    	fflush( stdout );
    	return(1);
    }	
    return(0);
}

inline int ltDebugWarn( const char* format, ... ){
	if(LT_PDB_LEVEL_2 <= _dbg_level){
    	va_list arglist;
    	if(_dbg_pid_mark)
			printf("[p%d]",getpid());
		if(_dbg_tid_mark)
			printf("[t%d]",pthread_self());
    	if(_dbg_time_mark){
			struct timespec start;
	   		if( -1 == clock_gettime( CLOCK_REALTIME, &start) )
	   			return(0);
	   		printf("<%f>\t",(start.tv_sec + (double)(start.tv_nsec ) / (double)BILLION));
		}
    	printf(_LT_DBG_MSG_WARNING);
    	va_start( arglist, format );
    	vprintf( format, arglist );
    	va_end( arglist );
    	fflush( stdout );
    	return(1);
    }
    return(0);	
}

inline int ltDebugMsg( const char* format, ... ){
	if(LT_PDB_LEVEL_3 <= _dbg_level){
    	va_list arglist;
    	if(_dbg_pid_mark)
			printf("[p%d]",getpid());
		if(_dbg_tid_mark)
			printf("[t%d]",pthread_self());
    	if(_dbg_time_mark){
			struct timespec start;
	   		if( -1 == clock_gettime( CLOCK_REALTIME, &start) )
	   			return(0);
	   		printf("<%f>\t",(start.tv_sec + (double)(start.tv_nsec ) / (double)BILLION));
		}
    	printf(_LT_DBG_MSG_MSG);
    	va_start( arglist, format );
    	vprintf( format, arglist );
    	va_end( arglist );
    	fflush( stdout );
    	return(1);
    }
    return(0);
}

/*
int ltDebugLog( const char* format, ... ){
    	va_list arglist;
    	struct timespec start;
   		if( -1 == clock_gettime( CLOCK_REALTIME, &start) )
   			return(0);
   		printf("<%f>",(start.tv_sec + (double)(start.tv_nsec ) / (double)BILLION));
//    	printf(_LT_DBG_MSG_WARNING);
    	va_start( arglist, format );
    	vprintf( format, arglist );
    	va_end( arglist );
    	fflush( stdout );
    	return(1);
}
*/
