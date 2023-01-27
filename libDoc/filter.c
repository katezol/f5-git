#include "filter.h"

#include "docStructs.h"

struct filterStr0 filterStr0;
struct filterStr1 filterStr1;
struct filterSendRli fSendRli;
struct filterStrX filterStrX;

struct filterStrAll filterStr[400];

int filterGlobal(unsigned char type){
	if(filterStr[type].show)
		return 1;
	else 
		return 0;
}

int filterSenRli(void* data){
	DocSendRli* p = (DocSendRli*)data;
	if(fSendRli.show==0){
		return 0;
	}else{
		if(fSendRli.nSource[p->nSource]==0)
						return 0;
	}
	return 1;
}

int filterFormularTrassaStruct2(void* data){
	DocFormularTrassa* p = (DocFormularTrassa*)data;
	if(filterStr0.show==0){
		return 0;
	}else{
		// Сортировка по номеру цели
		if(filterStr0.nSource[p->nSource]==0)
				return 0;

		if(p->nType==16)
			if(filterStr0.pr3k96)
				if(!p->skoVh)
					return 0;
		// Сортировка по Типу цели
		if(filterStr0.TC!=1){
			switch(filterStr0.TC){
			case 2: if(p->TC!=0)
						return 0;
				break;
			case 3:	if(p->TC!=1)
						return 0;
				break;
			case 4: if(p->TC!=8)
						return 0;
				break;
			case 5: if(p->TC!=9)
						return 0;
				break;
			case 6: if(p->TC!=10)
						return 0;
				break;
			case 7: if(p->TC!=11)
						return 0;
				break;
			case 8: if(p->TC!=12)
						return 0;
				break;
			case 9: if(p->TC!=13)
						return 0;
				break;
			case 10: if(p->TC!=15)
						return 0;
				break;
			}
		}
		// Сортировка по Гос Принадлежности
		if(filterStr0.SV!=1){
			switch(filterStr0.SV){
			case 2: if(p->SV!=0)
						return 0;
				break;
			case 3: if(p->SV!=1)
						return 0;
				break;
			case 4: if(p->SV!=2)
						return 0;
				break;
			}
		}
	}		
	return 1;
}

int filterTargetXStruct(void* data){
	DocTargetX_m* p = (DocTargetX_m*)data;
	int i, count;
	if(filterStrX.show==0)
	{
		return 0;
	}else
	{
		count =0;
		for (i = 0; i < 300; ++i) 
		{
			if(filterStrX.nX[i] == p->nn) 
				return 1;
			count = count+filterStrX.nX[i];
		}
		// Сортировка по номеру цели
	}	
	if (count >0) return 0; else return 1; 
}

int filterTargetTelevikStruct(void* data){
	DocTargetTelevik* p = (DocTargetTelevik*)data;
	if(filterStrX.show==0){
		return 0;
	}else{
		// Сортировка по номеру цели
		if(filterStrX.nX[p->nSource]==0)
				return 0;
		}

	return 1;
}

int filterDocIP3Puma(void* data){
	DocIP3_Puma* p = (DocIP3_Puma*)data;
	if(filterStr1.show==0){
		return 0;
	}else{
		// Сортировка по номеру цели РЛС
		if(filterStr1.nRls[p->nRls]==0)
			return 0;
		// Сортировка по номеру цели Пума
		if(filterStr1.nPuma[p->nPuma]==0)
			return 0;
		// Сортировка по типу цели
		if(filterStr1.objType!=1){
			switch(filterStr1.objType){
			case 2: if(p->objType!=0)
						return 0;
				break;
			case 3: if(p->objType!=1)
						return 0;
				break;
			case 4: if(p->objType!=3||p->objType!=4)
						return 0;
				break;
			case 5: if(p->objType!=11)
						return 0;
				break;
			case 6: if(p->objType!=12)
						return 0;
				break;
			case 7: if(p->objType!=13)
						return 0;
				break;
			case 8: if(p->objType!=14)
						return 0;
				break;
			case 9: if(p->objType!=15)
						return 0;
				break;
			}
		}
		// Сортировка по ГП
		if(filterStr1.prn!=1){
			switch(filterStr1.prn){
			case 2: if(p->prn!=0)
						return 0;
				break;
			case 3: if(p->prn!=1)
						return 0;
				break;
			case 4: if(p->prn!=2)
						return 0;
				break;
			}
		}
	}
	return 1;
}
