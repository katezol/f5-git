#ifndef ABONENTS_H_
#define ABONENTS_H_
/* содержит идентификаторы сообщений - передаются в первом байте сообщения
 * содержит адреса портов
 * */
 typedef struct
{
	char name[40];
	char path[40];
} Param;	
// идентификаторы  сообщения - передаются в первом байте сообщения
enum  {
    idImitatorRedutOn=70,// управление включением имитатора Редут, типа bool
    idZoneZos,		// 71зона Зос, типа ZoneZos, от РМО2 к PORT_IMREDUT
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
	idIP4_TK25_first100, // типа 100*mTK25_Ip4Tgt_t
	idImitTarget,// RlsTarget
	idUUS3_state, // SHeader+Uus3State
	idKKSState,  // SHeader+KKSState
	idCOMMANDX_STROB_CODE, // char+ CommandX_Strob_m
	idCOMMANDX_CHANGEPRI_CODE,// char+ CommandX_ChangePri
	idRESULT_COMMANDX_CODE, // char+Result_CommandX
	idAutoGo67,//типа int, 1=AUTO 0=PA +  SHeader.nType = idAutoGo67// не сделано
	idMode67,//110 типа int, 1=1 режим 2=2 режим 4-4 режим +  SHeader.nType = idMode67// не сделано
	idChangeAutoBos,//только  SHeader.nType = idChangeAutoBos// не сделано
	idChangeRubejGo,// типа int + SHeader.nType = idChangeRubejGo// не сделано
	idSkolToGo,//типа SkolToGo+nType
	idNerasr67,//типа Dev67Tgt_NerasrnType =idNerasr67
	idSkolSectorRLSX, //SkolSectorRLSX + n
	idDopoiskRLSX,//SkolSectorRLSX + n
	idCU_To_Mtk,// типа IP1_Rmo2_IP1_Mtk на логику
	idOptika_MObn,// типа SP_Optika_MObn на логику
	idOptika_MSopr,// типа SP_Optika_MSopr на логику
	idip1_Sfera,
	idip2_Sfera,
	idSferaUstForSektor,// типа SferaUstForSektor
	idPvoResult_PA_ALL,			// результат ЦР типа Plan_CR_ALL в полуавтоматическом режиме 
	idIP4_TK25_last, // типа 155*mTK25_Ip4Tgt_t
	idChangeAutoX,//только  SHeader.nType = idChangeAutoX//
} IDMessages;
 
char
	
	port_bid[40],// для отправки BosImitatorData c имитатора БОС 
	port_rmo2[40],//
	port_rmo2_low[40],//
	port_UUS1[40],
	port_BIsettings[40],// порт имитатора БОС для передачи настроек
	pvoMap_path[40],		// дорога к драйверу pvoMap
	pvoMapMain_path[40],		// дорога к драйверу pvoMapMain
	extraPvoMap_path[40], 	// дорога к драйверу extraPvoMapMain
	extraPvoMapMain_path[40], 	// дорога к драйверу extraPvoMapMain
	sr_targets_port[40], 	// дорога к драйверу sr_targets
	targetsX_portMain[40],
	targetsX_portLow[40],
	targetsAll_portMain[40],
	targetsAll_portLow[40],
	USADDR[20], //адрес УС связи с РМО1
    ipISGKPosn[40],
    ipISGKPres[40],
    
    port_pav_RS[40],//порт ПАВ RS-422(232)
    port_Blockage[40], // порт Блокировки RS-422 
    port_bosPel[40],// БОС пеленг RS-422
    port_MDVV[40],//порт МДВВ, RS-232    
    port_67inf[40],// 67 прибор инф RS-422
    port_MR231[40],// МР231 RS-422
    port_MR2313[40],//МР231-3 RS-422
// порты для MSL
	port_kksState[40],		// Состояния каналов ККС
	port_chartState[40],		// Состояние готовности MSL
	port_mc1[40],				// Состояние клиента 1...
	port_mc2[40],				// Состояние клиента 2...
	port_mc3[40],				// Состояние клиента 3...
	
	port_3K96[40],				// дорога к драйверу 3К96-3
	port_5P10[40],
	port_RLI[40],				// дорога к драйверу RLI
	port_Sigma[40],				// дорога к драйверу SigmaUpr
	port_Chardash[40],		    // дорога к драйверу Чардаш
	port_MTK[40],		    // дорога к драйверу MTK
	port_Sfera[40],		    // дорога к драйверу Sfera
	port_TK25[40],	    // дорога к драйверу TK25
	port_5P20[40],		    // дорога к драйверу 5P20
	port_MR123[40],		    // дорога к драйверу MR123
	port_3m87[40],		    // дорога к драйверу 3m87
	port_Gnome[40],
	US_IP_ADDR[40],
	
// порты для MVL
	path_mapPath[40],	  // Путь к разобраным картам
	logFileName[50], // лог УСА
    logSimpleNavName[50], // лог simpleNav
    logPath[50],	//путь для логов
	logPVOMain[50],	//лог для pvo_main
	path_flash[40];	  // Путь к флешке
unsigned char
    bFlushReceiveBosPeleng, // очищать буфер после каждого успешного приема боспеленга 
    bLogSimpleNav,//регистрироватьбоспеленг в файл  logSimpleNavName
    bBidLog,//регистрировать БосИмитатор  logBid
    bImitBos,// признак работы по имитированной обстановке всегда, при загрузке
    bImitTarget,// признак  имитированной цели, при загрузке
    bLoadRLI, // признак необходимости запуска канала РЛИ
    bLoadChardash, // признак необходимости запуска канала Chzrdash
    bLoadSigmaUpr, // признак необходимости запуска Сигма УПР
    bLoad3K96, // признак необходимости запуска Сигма УПР
	bLoad5P10, // признак необходимости запуска 5P10
	bLoadMTK, // признак необходимости запуска MTK
	bLoadSfera, // признак необходимости запуска Sfera
	bLoadTK25, // признак необходимости запуска TK25
	bLoad5P20, // признак необходимости запуска 5P20
	bLoad67, // признак необходимости запуска 67inf
	bLoadMR, // признак необходимости запуска MR231, MR231-3
	bLoadGnome,
	bLoad_MR123, // признак необходимости запуска MR123
	bLoad3m87, // признак необходимости запуска 3m87
	bLoadBlok,// признак необходимости запуска Blok
	mdvvType,// тип МДВВ, 1-2014год
	shipType,//1-1001, 0-остальные
	mdvvWithPui,//1-МДВВ, состыкованное с ПУИ
	bTrainPvoPermit,//признак разрешения режима PVOSTATE_TRAIN
	bResetAutoGo, // признак отключения bAutoGo
	bCuEco;// признак решения задачи по ЕЦО

int server_RMO1_PORT_NUM, //0x1212
    server_BOS_PORT_NUM,  //0x1211
    rmo2_PORT_NUM,
    rmo2low_PORT_NUM,
    uus3_PORT_NUM,
    suOsn_PORT_NUM,
    suRes_PORT_NUM;
   

//номера физических каналов устройств, читается из config_Bt 
int iBt_3K96,  // 1-6, 21-26 - манчестеры первого, второго блоков.
    iBt_5P10,  // 7, 27 - одинарные Ethernet первого, второго блоков.
    iBt_MTK, // 8-9, 28-29 - двлйные Ethernet первого, второго блоков.
    iBt_TK25, // 10-12, 30-32 - RS-422 первого, второго блоков.
    iBt_5P20,// 13, 33 - RS-232 из Технологии-2 первого, второго блоков.
    iBt_secondary,
    iBt_5P23,
    iBt_chardash_1,
    iBt_chardash_2,

    iBt_3K96_Imit,  // 0-5, - манчестеры первого, 10-15 - второго блоков.
    iBt_5P10_Imit,
    iBt_MTK_Imit,
    iBt_TK25_Imit, 
    iBt_5P20_Imit,
    iBt_chardash_Imit;



// идентификаторы устройств, связанных с УС
#define I3K96 1
#define I5P10 2
#define IMTK 3
#define ITK25 4
#define I5P20 5

#define IBOS 7
#define IRMO1 8
#define IRMO2 9

#define IBLOCKAGE 10
#define IBOSPEL 11
#define IPAV 12

#define IMDVV 13
#define IGNOME 24

#define ISECONDARY 21
#define I5P23 22
#define ICZARDAS 23
#define ISFERA 24

#define ISIGMA 28
#define IKKS 29

#define I67INF 30
#define IMR231 31
#define IMR2313 32

#define I5P10_S 51
#define I5P20_S 52
#define ISIGMA_S 53
// идентификаторы имитаторов устройств, про которые надо знать, что это имитаторы
#define I_IMIT_3K96 101
#define I_IMIT_5P10 102
#define I_IMIT_MTK 103
#define I_IMIT_TK25 104
#define I_IMIT_5P20 105
#define I_IMIT_CZARDAS 123
	
void definePorts(char *path);	// считывание портов из файла /home/furke2/abonents2

				
#endif /*ABONENTS_H_*/
