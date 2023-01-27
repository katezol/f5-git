#ifndef UIT_H_
#define UIT_H_
/*
 * файл с реализацией управления объектами 
 * */
#include <tempForModeling.h>

typedef enum {
	binit,         // инициализация независимая или с привязкой
	bcontrol,      // управляющая команда
    bchangePosition,  // мнгновенное изменение координат объекта (перепрыгнуть в точку)
    bchangePdc,  // мгновенное изменение параметров движения объекта        
    bkill
} TypeCommandUit;	
 
typedef struct {
	double time;	          	// время команды
	TypeCommandUit type;  		// тип команды
	short ind;	          		// признак инициализации ind <0
	short indPriv;	  		    // индекс объекта привязки для binit, bcontrol, bchangePosition, bchangePdc	
	short indTarget;	  		// индекс объекта для bcontrol, bchangePdc
	bool  bMeetPoint;	        // прицеливания в ТВ для bcontrol, bchangePdc			
	float x;   
	float y;       // if binit, width = (short) bNewCourse, для помехопостановщика - мощность излучения
	short bNewz;   // if binit or binitLike, objType = (ObjType) bNewz
	               // if (bControl с привязкой),
	               //  0 - высота не задается, 1-признак передачи задаваемой высоты,
	               // -1 - признак поворота на высоту привязки
	short bNewv;   // if binit, own     = (Own) bNewv 
	bool  bNewCourse; 
   	bool bNewPitch;
	float z;
	float v;
	float course;
	float pitch;	
} CommandUit;

typedef enum {
	errNoError,
	errWrongValue,
	errTooManyObjects,
	errTooManyCommands
} ErrorReadCommandUit;


#define NCOMMANDUIT 1000 // число команд максимальное

CommandUit commandUit[NCOMMANDUIT];  // массив команд
int nCommandUitCurr, nVarPvoWork; // число команд в массиве commandUit, номер проигрываемого варианта 
double centerfi, centerlambda; // центр карты

void deleteTarget(int i); // удаление объекта

/* преобразование файла с траекториями в массив команд commandUit,
 * определяет nCommandUitCurr, timeModeling, indexOurShip 
  *  @param fileName имя файла 
  *  @return количество команд	
  * */
int 
 	readCommandUit(char * fileName);
 	
/* реализация команд из массива команд commandUit
  *  @param oldTime старое текущее время
  *  @param currTime текущее время 
  *  подает команды на инициализацию, управление или удаление объекта
 * */
void fill_control_uit(double oldTime, double currTime);

/* вызывается при обнулении времени моделирования
 * */
void fill_new_uit(void);



#endif /*UIT_H_*/
