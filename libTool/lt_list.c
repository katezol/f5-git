#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "ltool.h"

List_t* ltListGetNext(List_t* it){
	return(it->next);
}	

List_t* ltListGetPrew(List_t* it){
	if(NULL == it->prev)
		errno = ENODATA;
	return(it->prev);
}	

List_t* ltListAddAfter(List_t* it, void *data, int size){
	List_t* this = NULL;

	this = malloc(sizeof(List_t));
	if(NULL == this)
		return(NULL);
	this->data 	= malloc(size);
	memcpy(this->data,data,size);
	if(NULL == data)
		return(NULL);
	if(NULL == it){
		this->prev 	= NULL;
		this->next 	= NULL;
	}
	else{
		this->prev = it;
		this->next = it->next;
		if(NULL != this->next)
			this->next->prev = this;
		it->next = this;
			
	}	
	if(NULL == this)
		errno = ENODATA;
	return(this);
}

List_t* ltListAddBefore(List_t* it, void *data, int size){
	List_t* this = NULL;

	this = malloc(sizeof(List_t));
	if(NULL == this)
		return(NULL);
	this->data 	= malloc(size);
	memcpy(this->data,data,size);
	if(NULL == data)
		return(NULL);
	if(NULL == it){
		this->prev 	= NULL;
		this->next 	= NULL;
	}
	else{
		this->next = it;
		this->prev = it->prev;
		if(NULL != this->prev)
			this->prev->next = this;
		it->prev = this;
	}	
	if(NULL == this)
		errno = ENODATA;
	return(this);
}

List_t* ltListGetFirst(List_t* it){
	if(NULL == it)
		return(NULL);
	while(NULL != it->prev)	
		it = it->prev;
	if(NULL == it)
		errno = ENODATA;	
	return(it);	
}

List_t* ltListGetEnd(List_t* it){
	if(NULL == it)
		return(NULL);
	while(NULL != it->next)	
		it = it->next;
	if(NULL == it)
		errno = ENODATA;	
	return(it);	
}

List_t* ltListRemove(List_t* it){
	if(NULL != it){
		List_t* next = it->next;
		List_t* prev = it->prev;
		free(it->data);
		free(it);
		if(NULL != prev)
			prev->next = next;	
		if(NULL != next){
			next->prev = prev;
			return(next);
		}
		if(NULL == prev)
			errno = ENODATA;
		return(prev);	
	}	
	else{
		errno = ENODATA;
		return(NULL);
	}	
}

