/*
 * lt_cfg.c
 *
 *  Created on: 30.09.2011
 *      Author: drd
 */
#include <stdlib.h>
#include <string.h>

#include "ltool.h"

char* cutchar(char* str, char* ch);

int ltGetcfg(char* str, char* var, char* val){
	char* _tp;
	char* _var;
	char* _val;
	char* delims = { "=" };
	*var = '\0';
	*val = '\0';
	_tp = strpbrk(str,"\r\n");
	if(NULL != _tp)
		*_tp = '\0';
	cutchar(str," \t");
	if('#' == *str)
		return(0);
	_var = strtok( str, delims );
	if(NULL != _var){
		_val = strtok( NULL, delims );
		if(NULL != _val){
			strncpy(var, _var, LT_CFG_PATH_MAX);
			strncpy(val, _val, LT_CFG_PATH_MAX);
			return(1);
		}
	}
	return(0);
}


char* cutchar(char* str, char* ch){
	char* p;
    char* buff = strdup( str );
    *str = '\0';
    p = strtok( buff, ch );
    while( p != NULL ) {
      strcat(str,p);
      p = strtok( NULL, ch );
    }
    free(buff);
    return(str);
}

