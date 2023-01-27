#include "read_write.h"
#include "global.h"
#include <fcntl.h>

enum {
	idImitatorRedutOn=70,// управление включением имитатора Редут, типа bool
	idZoneZos,		// зона Зос, типа ZoneZos, от РМО2 к PORT_IMREDUT
	/***********************************************************/
	/***********************************************************/    		
	idIP1_3k96,  // ИП1 в Редут типа IP1_3k96 
	idIP2_3k96,  // ИП2 в Редут типа IP2_3k96 (RlsTarget[NRLSCHANNELS]) цели Фурке + ЦР
                              
	/************** формуляры МТК */
	idIP1_Mtk,	// ИП1 в МТК типа IP1_Mtk- управляющие команды
	idIP2_Mtk,	// ИП2 в МТК типа IP2_Mtk (ResultGoMtk[6])-результаты запроса ГО

	/************** формуляры 5П-10 */
	idIP1_Puma,	// ИП1 в Пуму типа IP1_Puma- управляющие команды	
	idIPTarget_Puma,	//77 ИП с целями из Пумы типа	IPTarget_Puma
	/************** формуляры Сигма */

	/************** формуляры Блокировки */

	/************** формуляры TK25 */
	idIP1_TK25,
	idIP2_TK25,
	/************** формуляры РМО-2 */
	// из РМО-2 в УС
	idIP1_Rmo2, // от РМО-2	типа IP1_Rmo2 80
	idIP1_Rmo2_vAntenna, // от РМО-2	типа byte 30: 0xC, 6: 0xF, 3:0x7, stop:0x1, avar: 0x18
	idIP1_Rmo2_IP5_3K96, // от РМО-2	типа IP1_Rmo2_IP5_3K96
	idIP1_Rmo2_IP23_Puma, // от РМО-2	типа IP1_Rmo2_IP23_Puma,
	idIP1_Rmo2_IP1_Mtk, // от РМО-2	типа IP1_Rmo2_IP1_Mtk
	idIP1_Rmo2_IP1_Puma, //85 от РМО-2	типа IP1_Rmo2_IP1_Puma
	idIP1_Rmo2_SourceTarget, // от РМО-2	типа IP1_Rmo2_SourceTarget и обратно  типа IP1_Rmo2_SourceTarget
	idNewTarget_Rmo2, // от РМО-2	типа NewTarget_Rmo2
	idPvoCommand_Rmo2,		// посылка команд для ПВО типа PvoCommand_RMO2
	idRmo2Working, //89 от РМО-2	пустой, для проверки работы РМО2
	// от УС  на РМО-2
	idZonePuma,			// типа Zone,  от логики ПВО на РМО-2
	idWorkSector,		// SectorWorkLock[NSHOOTCOMPLEX]от логики ПВО на РМО-2
	idGoControl,//// команда управления ГО типа GoControl
	idPvoResult_PA,			// результат ЦР типа Plan_CR в полуавтоматическом режиме 
	id67_Alarm,//ответ 67 или тревога типа Dev67Alarm_t
	id67_Target,//95 ответ 67 или тревога типа Dev67Tgt_t
	idIP1_Rmo2_UpravBos, // от РМО-2 типа IP1_Rmo2_UpravBos
	idChartSrvOurShipGeo,// типа 2*OurShipGeo на CHARTSRV
	idCuMtk,	// типа 2*CuMtk на лоу
	idSaveLog,  // типа byte, номер файла для записи лога
	idRecordSettings,//100 типа RecordSettings 
	idResetLog,	// команда на удаление текущего файла лога
	idIP4_TK25
} IDMessages; 

// Поток с переоткрытием портов 
void* readThread(void* data){
	OurShipGeo tmpGeo1, tmpGeo2;
	
	unsigned char id;
	int _testConnect=-1;
	int fd = -1;
	unsigned char buff[100];
	while(1){
		if(_testConnect==-1){
			_testConnect = 1;
			fd = open(port_ChartServer, O_RDONLY);
			if(fd==-1){
				//printf("Connection port chart server error...\n");
				_testConnect = -1;
				delay(100);
			}
		}else{
			
			// В этот канал пишется только координаты корабля поэтому первый байт сообщения просто считываем 
			_testConnect = read(fd, &id,1);
			if(id!=idChartSrvOurShipGeo){
				delay(100); 				
				continue;
			}
			_testConnect += read(fd, buff, sizeof(ourShipGeo)*2);
			if(_testConnect==1+2*sizeof(ourShipGeo)){
				memcpy(&ourShipGeo, buff, sizeof(ourShipGeo));
				memcpy(&ourShipGeo2, buff+sizeof(ourShipGeo), sizeof(ourShipGeo));	
			}
			delay(10);
		}
		delay(10);
	}
}

