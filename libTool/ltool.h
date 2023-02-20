#ifndef LTOOL_H_
#define LTOOL_H_


///////////////////////
///		Debug		///
///////////////////////
#define LT_OFF		0
#define LT_ON		1
#define LT_ERROR	2
//0- debug off
//1- only errors
//2- errors and warnings
//3- errors, warnings ang messages
#define LT_PDB_LEVEL_0		0
#define LT_PDB_LEVEL_1		1
#define LT_PDB_LEVEL_2		2
#define LT_PDB_LEVEL_3		3

// ltDebugOn() set max level (LT_PDB_LEVEL_3) 
int ltDebugOn();
int ltDebugOff();
int ltDebugGetLevel();
int ltDebugSetLevel(int level);
int ltDebugTimeMarkOn();
int ltDebugTimeMarkOff();
int ltDebugPidMarkOn();
int ltDebugPidMarkOff();
int ltDebugTidMarkOn();
int ltDebugTidMarkOff();

inline int ltDebugErr(  const char*  format, ... );
inline int ltDebugWarn( const char*  format, ... );
inline int ltDebugMsg(  const char*  format, ... );
//int ltDebugLog(  const char*  format, ... );
///////////////////////////
///		checksum		///
///////////////////////////
// use [Sh]-suffix, if check shift from MSB (Most significant bit)
char ltCheckSumB(char* data, int size);
char ltCheckSumBSh(char* data, int size);

unsigned short ltCheckSumW(unsigned short* data, int size);
unsigned short ltCheckSumWSh(unsigned short* data, int size);

///////////////////////////
///		random  		///
///////////////////////////
// generate random value in range [min,max]
int ltRandi(int min, int max);
float ltRandf(float min,float max);

///////////////////////////
///		line list  		///
///////////////////////////
struct LIST_T{
	struct LIST_T*	prev;
	struct LIST_T*  next;
	void*   data;
};
typedef struct LIST_T List_t;

List_t* ltListGetPrew(List_t* it);
List_t* ltListGetNext(List_t* it);
List_t* ltListAddBefore(List_t* it, void *data, int size);
List_t* ltListAddAfter(List_t* it, void *data, int size);
List_t* ltListGetFirst(List_t* it);
List_t* ltListGetEnd(List_t* it);
List_t* ltListRemove(List_t* it);

///////////////////////////
///		resd config		///
///////////////////////////
// 1 Success.
// 0 Not find
#define LT_CFG_PATH_MAX	128
int ltGetcfg(char* str, char* var, char* val);

/////////////////////////
#endif /*PDB_H_*/
