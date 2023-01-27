#ifndef FILTER_H_
#define FILTER_H_

#define MAX_ARR_NUMB 300
// Индекс в конце структуры смотреть в docCodeMsg.h, слишком большие
// оригинальные названия 



struct filterStr0{
	unsigned char show;
	int SV;
	int TC;
	unsigned char pr3k96;
	unsigned char nSource[MAX_ARR_NUMB];
};

struct filterStr1{
	unsigned char show;
	unsigned char nPuma[MAX_ARR_NUMB];
	unsigned char nRls[MAX_ARR_NUMB];
	int prn;
	int objType;
};

struct filterSendRli{
	unsigned char show;
	unsigned char nSource[MAX_ARR_NUMB];
};
struct filterStrX{
	unsigned char show;
	unsigned int nX[MAX_ARR_NUMB];
};
struct filterStrTelevik{
	unsigned char show;
	unsigned char nTelevik[100];
};

struct filterStrAll{
	unsigned char show;
};

extern struct filterStr0 filterStr0;
extern struct filterStr1 filterStr1;

extern struct filterSendRli fSendRli;
extern struct filterStrX filterStrX;
extern struct filterStrTelevik filterStrTelevik;

extern struct filterStrAll filterStr[400];

int filterGlobal(unsigned char type);
int filterFormularTrassaStruct2(void* data);
int filterDocIP3Puma(void* data);
int filterSenRli(void* data);

#endif /*FILTER_H_*/
