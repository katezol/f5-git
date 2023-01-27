#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <abonents.h>



void definePorts(char * path)
{
	char str_buf[100];
    FILE* f;
    int i;
	int nPorts = 0;
	Param params[100];
	//str_buf=(dirname (arg[0]));

	f = fopen( path, "r" );    
	if( f == NULL )  return;
	 //  f = fopen( "/home/furke5/abonents5", "r" );    if( f == NULL )  return;
    memset(&params,0,sizeof(params));    
    for (i = 0; i < 100; ++i) 
    {
        if (fgets(str_buf, sizeof(str_buf), f) ==  NULL) break;    	
    	sscanf(str_buf, "%s %s \n", params[nPorts].name,params[nPorts].path);
    	++nPorts;    	
   	} // for i
   	fclose( f );
   	
   	mdvvWithPui = 0;
   	mdvvType = 0;
   	shipType = 0;
   	bTrainPvoPermit = 0;
   	bResetAutoGo = 0;
   	bCuEco = 0;
   	
   	
    for (i = 0; i < nPorts; ++i) 
    {
    	// Порты для MSL
    	
    	if ( strncmp("kksState_port",params[i].name,strlen("kksState_port")) == 0) 
    		strcpy(port_kksState,params[i].path);
    	if ( strncmp("ChartServer_port",params[i].name,strlen("ChartServer_port")) == 0) 
    		strcpy(port_chartState,params[i].path);
    	if ( strncmp("mc1_port",params[i].name,strlen("mc1_port")) == 0) 
    		strcpy(port_mc1,params[i].path);
    	if ( strncmp("mc2_port",params[i].name,strlen("mc2_port")) == 0) 
    		strcpy(port_mc2,params[i].path);
    	if ( strncmp("mc3_port",params[i].name,strlen("mc3_port")) == 0) 
    		strcpy(port_mc3,params[i].path);
    	if ( strncmp("3K96-3_port",params[i].name,strlen("3K96-3")) == 0) 
    		strcpy(port_3K96,params[i].path);
    	if ( strncmp("5P10_port",params[i].name,strlen("5P10_port")) == 0) 
    		strcpy(port_5P10,params[i].path);
    	if ( strncmp("US_IP_ADDR",params[i].name,strlen("US_IP_ADDR")) == 0)
    		strcpy(US_IP_ADDR,params[i].path);
       	if ( strncmp("RLI_port",params[i].name,strlen("RLI_port")) == 0) 
    		strcpy(port_RLI,params[i].path);  
    	if ( strncmp("Sigma_port",params[i].name,strlen("Sigma_port")) == 0) 
    	    		strcpy(port_Sigma,params[i].path);  
    	if ( strncmp("Chardash_port",params[i].name,strlen("Chardash_port")) == 0) 
    	    		strcpy(port_Chardash,params[i].path);  
    	if ( strncmp("MTK_port",params[i].name,strlen("MTK_port")) == 0) 
    	    		strcpy(port_MTK,params[i].path);  
       	if ( strncmp("Sfera_port",params[i].name,strlen("Sfera_port")) == 0) 
        	    		strcpy(port_Sfera,params[i].path);  
    	if ( strncmp("TK25_port",params[i].name,strlen("TK25_port")) == 0) 
    	      	strcpy(port_TK25,params[i].path);  
    	if ( strncmp("port_Gnome",params[i].name,strlen("port_Gnome")) == 0) 
    	      	strcpy(port_Gnome,params[i].path);  
    	if ( strncmp("MR123_port",params[i].name,strlen("MR123_port")) == 0) 
        	      	strcpy(port_MR123,params[i].path);      	
       	if ( strncmp("3m87_port",params[i].name,strlen("3m87_port")) == 0) 
        	      	strcpy(port_3m87,params[i].path);      	
       	if ( strncmp("5P20_port",params[i].name,strlen("5P20_port")) == 0) 
        	      	strcpy(port_5P20,params[i].path);      	
    	if ( strncmp("pvoMap_port",params[i].name,strlen("pvoMap_port")) == 0) 
    		strcpy(pvoMap_path,params[i].path);
    	if ( strncmp("pvoMapMain_port",params[i].name,strlen("pvoMapMain_port")) == 0) 
    		strcpy(pvoMapMain_path,params[i].path);
    	if ( strncmp("extraPvoMap_port",params[i].name,strlen("extraPvoMap_port")) == 0) 
    		strcpy(extraPvoMap_path,params[i].path);
    	if ( strncmp("extraPvoMapMain_port",params[i].name,strlen("extraPvoMapMain_port")) == 0) 
    		strcpy(extraPvoMapMain_path,params[i].path);
    	if ( strncmp("sr_Targets_port",params[i].name,strlen("sr_Targets_port")) == 0) 
    		strcpy(sr_targets_port,params[i].path); 
    	if ( strncmp("targetsX_portMain",params[i].name,strlen("targetsX_portMain")) == 0) 
    		strcpy(targetsX_portMain,params[i].path); 
    	if ( strncmp("targetsX_portLow",params[i].name,strlen("targetsX_portLow")) == 0) 
    		strcpy(targetsX_portLow,params[i].path); 
    	if ( strncmp("targetsAll_portMain",params[i].name,strlen("targetsAll_portMain")) == 0) 
    		strcpy(targetsAll_portMain,params[i].path); 
    	if ( strncmp("targetsAll_portLow",params[i].name,strlen("targetsAll_portLow")) == 0) 
    		strcpy(targetsAll_portLow,params[i].path); 
    
 
    	    	// Порты для MVL
    	if ( strncmp("path_mapPath",params[i].name,strlen("path_mapPath")) == 0) 
    		strcpy(path_mapPath,params[i].path);
    	if ( strncmp("path_flash",params[i].name,strlen("path_flash")) == 0) 
    		strcpy(path_flash,params[i].path);
    	
    	//*******************************

		if ( strncmp("bid_port",params[i].name,strlen("bid_port")) == 0) 
				      strcpy(port_bid,params[i].path);
		if ( strncmp("rmo2_port",params[i].name,strlen("rmo2_port")) == 0) 
		     strcpy(port_rmo2,params[i].path);
		if ( strncmp("rmo2_low_port",params[i].name,strlen("rmo2_low_port")) == 0) 
		     strcpy(port_rmo2_low,params[i].path);
		if ( strncmp("port_UUS1",params[i].name,strlen("port_UUS1")) == 0) 
		     strcpy(port_UUS1,params[i].path);
		if ( strncmp("BIsettings_port",params[i].name,strlen("BIsettings_port")) == 0) 
		     strcpy(port_BIsettings,params[i].path);
		if ( strncmp("USADDR",params[i].name,strlen("USADDR")) == 0)
		     strcpy(USADDR,params[i].path);
		if ( strncmp("ipISGKPres",params[i].name,strlen("ipISGKPres")) == 0) 
				     strcpy(ipISGKPres,params[i].path);
		if ( strncmp("ipISGKPosn",params[i].name,strlen("ipISGKPosn")) == 0) 
				     strcpy(ipISGKPosn,params[i].path);
		if ( strncmp("pavRs_port",params[i].name,strlen("pavRs_port")) == 0) 
		     strcpy(port_pav_RS,params[i].path);
		if ( strncmp("blockage_port",params[i].name,strlen("blockage_port")) == 0) 
		     strcpy(port_Blockage,params[i].path);
		if ( strncmp("bosPel_port",params[i].name,strlen("bosPel_port")) == 0) 
		     strcpy(port_bosPel,params[i].path);
		if ( strncmp("MDVV_port",params[i].name,strlen("MDVV_port")) == 0) 
		     strcpy(port_MDVV,params[i].path);
		if ( strncmp("67inf_port",params[i].name,strlen("67inf_port")) == 0) 
		     strcpy(port_67inf,params[i].path);
		if ( strncmp("MR231_port",params[i].name,strlen("MR231_port")) == 0) 
		     strcpy(port_MR231,params[i].path);
		if ( strncmp("MR2313_port",params[i].name,strlen("MR2313_port")) == 0) 
		     strcpy(port_MR2313,params[i].path);
		
		if ( strncmp("server_RMO1_PORT_NUM",params[i].name,strlen("server_RMO1_PORT_NUM")) == 0)
		       server_RMO1_PORT_NUM = strtoul( params[i].path, NULL, 16 );
		if ( strncmp("server_BOS_PORT_NUM",params[i].name,strlen("server_BOS_PORT_NUM")) == 0)
		       server_BOS_PORT_NUM = strtoul( params[i].path, NULL, 16 );
		if ( strncmp("rmo2_PORT_NUM",params[i].name,strlen("rmo2_PORT_NUM")) == 0)
		       rmo2_PORT_NUM = strtoul( params[i].path, NULL, 16 );
		if ( strncmp("rmo2low_PORT_NUM",params[i].name,strlen("rmo2low_PORT_NUM")) == 0)
		       rmo2low_PORT_NUM = strtoul( params[i].path, NULL, 16 );
		if ( strncmp("uus3_PORT_NUM",params[i].name,strlen("uus3_PORT_NUM")) == 0)
			uus3_PORT_NUM = strtoul( params[i].path, NULL, 16 );
		if ( strncmp("suOsn_PORT_NUM",params[i].name,strlen("suOsn_PORT_NUM")) == 0)
			suOsn_PORT_NUM = strtoul( params[i].path, NULL, 16 );
		if ( strncmp("suRes_PORT_NUM",params[i].name,strlen("suRes_PORT_NUM")) == 0)
			suRes_PORT_NUM = strtoul( params[i].path, NULL, 16 );

	
		if ( strncmp("logPath",params[i].name,strlen("logPath")) == 0)
			strcpy( logPath, params[i].path);
		if ( strncmp("logFileName",params[i].name,strlen("logFileName")) == 0)
			strcpy( logFileName, params[i].path);
		if ( strncmp("logPVOMain",params[i].name,strlen("logPVOMain")) == 0)
			strcpy( logPVOMain, params[i].path);
		if ( strncmp("logSimpleNavName",params[i].name,strlen("logSimpleNavName")) == 0)
					strcpy( logSimpleNavName, params[i].path);
		if ( strncmp("bFlushReceiveBosPeleng",params[i].name,strlen("bFlushReceiveBosPeleng")) == 0)
					bFlushReceiveBosPeleng = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bLogSimpleNav",params[i].name,strlen("bLogSimpleNav")) == 0)
					bLogSimpleNav = strtoul( params[i].path, NULL, 10 );
		
		if ( strncmp("bImitBos",params[i].name,strlen("bImitBos")) == 0)
			bImitBos = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bImitTarget",params[i].name,strlen("bImitTarget")) == 0)
			bImitTarget = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bBidLog",params[i].name,strlen("bBidLog")) == 0)
			bBidLog = strtoul( params[i].path, NULL, 10 );		
		if ( strncmp("bLoadRLI",params[i].name,strlen("bLoadRLI")) == 0)
			bLoadRLI = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bLoadSigmaUpr",params[i].name,strlen("bLoadSigmaUpr")) == 0)
			bLoadSigmaUpr = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bLoad3K96",params[i].name,strlen("bLoad3K96")) == 0)
			bLoad3K96 = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bLoad5P10",params[i].name,strlen("bLoad5P10")) == 0)
			bLoad5P10 = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bLoadMTK",params[i].name,strlen("bLoadMTK")) == 0)
			bLoadMTK = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bLoadSfera",params[i].name,strlen("bLoadSfera")) == 0)
			bLoadSfera = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bLoadTK25",params[i].name,strlen("bLoadTK25")) == 0)
			bLoadTK25 = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bLoad5P20",params[i].name,strlen("bLoad5P20")) == 0)
			bLoad5P20 = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bLoadGnome",params[i].name,strlen("bLoadGnome")) == 0)
			bLoadGnome = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bLoad67",params[i].name,strlen("bLoad67")) == 0)
			bLoad67 = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bLoadMR",params[i].name,strlen("bLoadMR")) == 0)
			bLoadMR = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bLoad_MR123",params[i].name,strlen("bLoad_MR123")) == 0)
			bLoad_MR123 = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bLoad3m87",params[i].name,strlen("bLoad3m87")) == 0)
			bLoad3m87 = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bLoadBlok",params[i].name,strlen("bLoadBlok")) == 0)
			bLoadBlok = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bLoadChardash",params[i].name,strlen("bLoadChardash")) == 0)
			bLoadChardash = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("mdvvType",params[i].name,strlen("mdvvType")) == 0)
			mdvvType = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("shipType",params[i].name,strlen("shipType")) == 0)
			shipType = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("mdvvWithPui",params[i].name,strlen("mdvvWithPui")) == 0)
			mdvvWithPui = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bTrainPvoPermit",params[i].name,strlen("bTrainPvoPermit")) == 0)
			bTrainPvoPermit = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bResetAutoGo",params[i].name,strlen("bResetAutoGo")) == 0)
			bResetAutoGo = strtoul( params[i].path, NULL, 10 );
		if ( strncmp("bCuEco",params[i].name,strlen("bCuEco")) == 0)
			bCuEco = strtoul( params[i].path, NULL, 10 );
		

   	} // for i
   	
} // definePorts

