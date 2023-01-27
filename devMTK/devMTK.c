#include <mathutils.h>
#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <devctl.h>
#include <pthread.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <semaphore.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define THREAD_POOL_PARAM_T dispatch_context_t
#include <sys/iofunc.h>
#include <sys/neutrino.h>
#include <sys/resmgr.h>
#include <sys/dispatch.h>
#include <sys/dcmd_chr.h>
#include <sys/netmgr.h>



#include 	"lmanch.h"
#include	"lMTK201M.h"
#include	"ltool.h"
#include	"lconv.h"


unsigned short _MRT_ADDRESS	=	1;

#define _DEV_ADDRESS_LEN  32
char _DEV_ADDRESS[_DEV_ADDRESS_LEN] =	"/dev/manch0";



/// channel ctrl ///
#define _CH_NO			0
#define _CH_MANCH_PRIM	3
#define _CH_MANCH_RES	4
#define _CH_STATE_ON	1
#define _CH_STATE_OFF	0

typedef struct{
	unsigned short manch1;
	unsigned short manch2;
}ChannCtrl_t;

int chInit(ChannCtrl_t* chtl);
int chSetOn(ChannCtrl_t* chtl, int chann );
int chSetOff(ChannCtrl_t* chtl, int chann );
int chIsOff(ChannCtrl_t* chtl, int chann );
int chIsActive(ChannCtrl_t* chtl, int chann );
int checkManchChann(int fd, int chann);
int chan2ch(int ch);
int checkCH(int fd,ChannCtrl_t* chtl,int* cur, int* res);
int checkPriCh(int fd, ChannCtrl_t* chtl,int* cur);
static ChannCtrl_t  chctl; //global variable, used in threads !!!
///  --- ///


typedef struct{
	sem_t  sip3;
	sem_t  sip4;
}ReadBlockSem_t; 
unsigned short CsIp3 = 0;
unsigned short CsIp4 = 0;
unsigned short GetCsIp(char* ip, int size);
int bsem_post(sem_t *sem);

/// threads ///
#define _SWAP_INTERVAL_ns 	100000000
#define _SWAP_PULSE_CODE   (_PULSE_CODE_MINAVAIL +1)

void*  th_manch( void*  arg );
///  --- ///

/// raw data ///
#define IP_MAP IpMapMTK_t
typedef struct{
	pthread_rwlock_t 	rwl;
	IP_MAP*				map;
	ReadBlockSem_t		sip;
}IpRaw_t;

static IpRaw_t IpRaw; //global variable, used in threads !!!
int ipRawInit(IpRaw_t *ip);

//#define _IP1_NS		110
int RawToMip1(mMTK_Ip1_t *ip, IpRaw_t* ipraw);
int RawToMip2(mMTK_Ip2_t *ip, IpRaw_t* ipraw, int shift);
int Mip3ToRaw(mMTK_Ip3_t *ip, IpRaw_t* ipraw);
int Mip4ToRaw(mMTK_Ip4_t *ip, IpRaw_t* ipraw, int shift);
///  --- ///
int StateToRaw(unsigned short ch,unsigned short state,IpRaw_t* ipraw);
/// resmgr ///
#define _DEV_NAME 	_MTK_DEV_NAME
#define _POOL_VAL 	5

// this variables defined as global, because it need for resmgr
static 	resmgr_connect_funcs_t  Connect_funcs;
static 	resmgr_io_funcs_t       Io_funcs;
static	iofunc_attr_t			Attr;
static struct timespec lastTimeKU;
static float lastKU; // радианы
extern float do2pi(float value);

int io_open (resmgr_context_t *ctp, io_open_t *msg, iofunc_attr_t* atr, void *extra);
int io_read (resmgr_context_t *ctp, io_read_t  *msg, RESMGR_OCB_T *ocb);
int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);
int io_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb);
int io_nolock(resmgr_context_t *ctp, void* msg, RESMGR_OCB_T *ocb); 
int io_unblock( resmgr_context_t* ctp, io_pulse_t* msg, RESMGR_OCB_T *ocb );
// we are redefine lock_ocb() to our io_nolock() because resmgr by default are bloked OCB in write,read end etc. functions and we 
// can not call next write,read end etc. functions, until not exit from previos functions in multi threads res mgr. 
///  --- ///

int main(int argc, char *argv[]) {
	int c_opt = 0;
	thread_pool_attr_t  pool_attr;
	resmgr_attr_t       rattr;
	dispatch_t          *dpp = NULL;
    thread_pool_t      	*tpp = NULL;
    int 				pathID = 0;
	
	ltDebugOff();
	while( ( c_opt = getopt( argc, argv, "v:d:a:" ) )
      != -1 ) {
      switch( c_opt ) {
        case 'v': {
        	int dlevel = min(atoi(optarg),LT_PDB_LEVEL_3);
        	dlevel = max(dlevel,LT_PDB_LEVEL_0);
    		ltDebugSetLevel(dlevel);    	
        } break;
        case 'd':
        	strncpy(_DEV_ADDRESS,optarg,_DEV_ADDRESS_LEN);
        break;
        case 'a':
        	_MRT_ADDRESS =	atoi(optarg);
        break;	
		default: 
			return (EXIT_FAILURE);
		break;        
      }
    }
	
	ltDebugMsg("start [%s] device[%s] address[%d]\n", argv[0],_DEV_ADDRESS,_MRT_ADDRESS);
    signal( SIGPIPE , SIG_IGN  );// if write() return EPIPE, a SIGPIPE signal is also sent to the process. by default exit, we set it to IGNORE 
	
	
	/// init ///
	chInit(&chctl);
	if(EOK != ipRawInit(&IpRaw)){
	    ltDebugErr("couldn't IP_RAW init: %s\n",strerror (errno));
        return (EXIT_FAILURE);
	}

	
	
	/// threads start ///
	if(1){
		pthread_attr_t attr;
   		pthread_attr_init( &attr );
   		pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
   		if(EOK != pthread_create( NULL, &attr, &th_manch, NULL )){
   			ltDebugErr("pthread_create %s\n", strerror (errno));
   			return (EXIT_FAILURE);
   		}
	}
	
	/// resmgr start ///
    dpp = dispatch_create ();
    if (dpp == NULL) {
        ltDebugErr("couldn't dispatch_create: %s\n",strerror (errno));
        return (EXIT_FAILURE);
    }
    memset (&rattr, 0, sizeof (rattr)); 
    rattr.nparts_max = 1;
	rattr.msg_max_size = 2048;

    iofunc_func_init (_RESMGR_CONNECT_NFUNCS, &Connect_funcs, _RESMGR_IO_NFUNCS, &Io_funcs);
    Connect_funcs.open 	= io_open;
    Io_funcs.read 		= io_read;
    Io_funcs.write 		= io_write;
    Io_funcs.devctl 	= io_devctl;
    Io_funcs.lock_ocb 	= io_nolock;
    Io_funcs.unblock	= io_unblock;
	
	iofunc_attr_init ( &Attr, S_IFCHR | 0666, NULL, NULL);
	pathID = resmgr_attach (dpp, &rattr, _DEV_NAME,_FTYPE_ANY, 0, &Connect_funcs, &Io_funcs, &Attr);
	if (pathID == -1) {
	    ltDebugErr("couldn't attach pathname: %s\n", strerror (errno));
	    return (EXIT_FAILURE);
	}
	memset(&pool_attr, 0, sizeof pool_attr);
	pool_attr.handle = dpp;
	pool_attr.context_alloc = dispatch_context_alloc;
	pool_attr.block_func 	= dispatch_block;
	pool_attr.unblock_func 	= dispatch_unblock;
	pool_attr.handler_func 	= dispatch_handler;
	pool_attr.context_free 	= dispatch_context_free;
	pool_attr.lo_water 	= _POOL_VAL;
	pool_attr.hi_water 	= _POOL_VAL;
	pool_attr.increment = 1;
	pool_attr.maximum 	= _POOL_VAL;

	tpp = thread_pool_create(&pool_attr, POOL_FLAG_EXIT_SELF);
	if(NULL == tpp){
    	ltDebugErr("Unable to initialize thread pool\n");
    	return (EXIT_FAILURE);
	}
	thread_pool_start(tpp);
	ltDebugMsg("exit [%s]\n",argv[0]);
	return (EXIT_SUCCESS);
}

int io_open (resmgr_context_t *ctp, io_open_t *msg, iofunc_attr_t* attr, void *extra){
	ltDebugMsg ("io_open [%d]\n",ctp->id);
    return (iofunc_open_default (ctp, msg, attr, extra));
}

int io_read (resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb){
    int status;
	int size = msg->i.nbytes;
	int offset = ocb->offset;	
	int err = 0;
	ltDebugMsg("io read\n");
 	if ((status = iofunc_read_verify(ctp, msg, ocb, NULL)) != EOK)
        return (status);
    if (msg->i.xtype & _IO_XTYPE_MASK != _IO_XTYPE_NONE) 
        return(ENOSYS);
    if (msg->i.nbytes <= 0){
    	ltDebugWarn("ioread: get read 0 byte\n");
    	_IO_SET_READ_NBYTES (ctp, 0);
		return (_RESMGR_NPARTS (0));
    }
  //  if(ocb->ioflag != (O_RDONLY + 1 ))// why +1 ?, see help on iofunc_ocb_t
    //	return (EACCES);

	if(size > (sizeof(IP_MAP) - offset)){
		ltDebugErr("io read size [%d] more then\n",size,(sizeof(IP_MAP) - offset));
		return(ENOMEM);
	}	
	err = pthread_rwlock_rdlock(&(IpRaw.rwl));
	if(EOK != err)
		return(err);
	MsgReply(ctp->rcvid, size, (char*)(IpRaw.map) + offset, size);	
	err = pthread_rwlock_unlock(&(IpRaw.rwl));
	if(EOK != err)
		return(err);
	ltDebugMsg("io read size [%d]\n",size);
	clock_gettime( CLOCK_REALTIME, &lastTimeKU) ;// получаем текущее время
	
	lastKU = IpRaw.map->ip1.kuAnt*M_PI/(1<<15); // в радианах
	// TODO здесь запомнить время 
	_IO_SET_READ_NBYTES (ctp, size);
	ocb->attr->flags |= IOFUNC_ATTR_ATIME;
	return (_RESMGR_NOREPLY);
}

int io_write (resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb){
    int status;
	int size = msg->i.nbytes;
	int offset = ocb->offset;
	int err = 0;
    if ((status = iofunc_write_verify(ctp, msg, ocb, NULL)) != EOK)
        return (status);
    if (msg->i.xtype & _IO_XTYPE_MASK != _IO_XTYPE_NONE) 
        return(ENOSYS);
    if (msg->i.nbytes <= 0){
    	ltDebugWarn("iowrite: write 0 byte\n");
    	_IO_SET_WRITE_NBYTES (ctp, 0);
		return (_RESMGR_NPARTS (0));
    }
    // warning read and write must used with different file descriptor 
    //if(ocb->ioflag != (O_WRONLY + 1 ))// why +1 ?, see help on iofunc_ocb_t
    	//return (EACCES);
 	
	if(size > (sizeof(IP_MAP) - offset)){
		ltDebugErr("io write size [%d] more then[%d]\n",size, (sizeof(IP_MAP) - offset));
		return(ENOMEM);
	}	
	err = pthread_rwlock_wrlock(&(IpRaw.rwl));
	if(EOK != err)
		return(err);
	if(size != resmgr_msgread( ctp, (char*)(IpRaw.map) + offset, size, sizeof(msg->i))){
		ltDebugErr("resmgr_msgread %s\n", strerror (errno));
		return(errno);
	}
	err = pthread_rwlock_unlock(&(IpRaw.rwl));
	if(EOK != err)
		return(err);
	ltDebugMsg("io write size [%d]\n",size);
	_IO_SET_WRITE_NBYTES (ctp, size);
	ocb->attr->flags |= IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME;
	return (_RESMGR_NPARTS (0));
}

int io_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb){
	int err = 0;
	int sts = iofunc_devctl_default(ctp,msg,ocb);
	ltDebugMsg("io devctl\n");
	if( _RESMGR_DEFAULT != sts )
		return(sts);
	switch(msg->i.dcmd){
		case MMTK_GET_IP3 :{ 
			sem_wait(&(IpRaw.sip.sip3));
			err = pthread_rwlock_rdlock(&(IpRaw.rwl));
			if(EOK != err)
				return(err);
			memcpy(_DEVCTL_DATA(msg->i), &(IpRaw.map->ip3), sizeof(mMTK_Ip3_t));
			memset(&(msg->o),0,sizeof(msg->o));	
			msg->o.nbytes = sizeof(mMTK_Ip3_t);
			resmgr_msgwrite (ctp, &(msg->o), sizeof(msg->o)+ msg->o.nbytes, 0);
			err = pthread_rwlock_unlock(&(IpRaw.rwl));
			if(EOK != err)
				return(err);
			return(EOK);
		}break;		
		case MMTK_GET_IP4 :{ 
			sem_wait(&(IpRaw.sip.sip4));
			err = pthread_rwlock_rdlock(&(IpRaw.rwl));
			if(EOK != err)
				return(err);
			memcpy(_DEVCTL_DATA(msg->i), IpRaw.map->ip4tgt, MTK_TARGET_NUM * sizeof(mMTK_Ip4Tgt_t));
			memset(&(msg->o),0,sizeof(msg->o));	
			msg->o.nbytes = MTK_TARGET_NUM* sizeof(mMTK_Ip4Tgt_t);
			resmgr_msgwrite (ctp, &(msg->o), sizeof(msg->o)+ msg->o.nbytes, 0);
			err = pthread_rwlock_unlock(&(IpRaw.rwl));
			if(EOK != err)
				return(err);
			return(EOK);
		}break;		
		default : 	
			ltDebugWarn("UNKNOWN DEVCTL COMMAND [0x%x]\n",msg->i.dcmd);
			return(ENOSYS); 
			break;
	}	
	SETIOV(ctp->iov,&(msg->o),sizeof(msg->o)+ msg->o.nbytes);
	return(_RESMGR_NPARTS(1));
}

int io_nolock(resmgr_context_t *ctp, void* msg, RESMGR_OCB_T *ocb){
	return(EOK);
}
int io_unblock( resmgr_context_t * ctp, io_pulse_t* msg, RESMGR_OCB_T * ocb  ){
	int status = 0;	
	if ((status = iofunc_unblock_default(ctp, msg, ocb)) != _RESMGR_DEFAULT)
        return (status);
    ltDebugWarn ("io_unblock [%d]\n",ctp->id);
    sem_post(&(IpRaw.sip.sip3));
    sem_post(&(IpRaw.sip.sip4));
	return(EOK);
}
/// ---- ///
int ipRawInit(IpRaw_t *ip){
	ip->map = malloc(sizeof(IP_MAP));
	memset(ip->map,0,sizeof(IP_MAP));
	if(-1 == sem_init(&ip->sip.sip3, 0, 1)){ 
   		ltDebugErr ("sem init: %s\n",strerror (errno));
		return (errno);
	}
	if(-1 == sem_init(&ip->sip.sip4, 0, 1)){ 
   		ltDebugErr ("sem init: %s\n",strerror (errno));
		return (errno);
	}

	return(pthread_rwlock_init(&(ip->rwl),NULL ));
}
/// ---- ///

float mtkdo2pi(float value)
{
	float result = value;
	while (result >= 2*M_PI) result = result-2*M_PI;
    while (result < 0)     result = result+2*M_PI;
    return result;
}


void*  th_manch( void*  arg ){
	//int chid = *((int*)arg);
	struct _pulse   pulse;
	mMTK_Ip1_t	ip1;
	mMTK_Ip2_t	ip2;
	mMTK_Ip3_t	ip3;
	mMTK_Ip4_t 	ip4;

	//unsigned short bsync_init 	= 0xC;
	//unsigned short bsync_end 	= 0xE;
	//const int cmd_sync 	= 0x11;
	
	int 	check_cnt = 0;
	int mfd = 0;
	//const int addr = 0x16;
	const int addr 	= _MRT_ADDRESS;
	int cur_chann 	= _CH_MANCH_PRIM;
	int res_chann 	= _CH_MANCH_RES;
	char* path 		= _DEV_ADDRESS;
	//////////////
	/// pulse timer create ///
	int                 chid_swap; 	//to get timer pulse in threads, for start swap 
	timer_t             swap_timer_id;  //
	struct sigevent         event;
	struct itimerspec       itime;
	struct timespec curr;
	double ct;
	float speed, rotation;
	
	BcFrame_t frame;
	frame = BcFrameCreate();
	
	//clock_gettime( CLOCK_REALTIME, &lastTimeKU);
	memset(&lastTimeKU,0,sizeof(lastTimeKU));
	
	StateToRaw( cur_chann, _CH_STATE_OFF, &IpRaw);
	StateToRaw( res_chann, _CH_STATE_OFF, &IpRaw);
	
	chid_swap = ChannelCreate(0);
	if(-1 == chid_swap){
		ltDebugErr("ChannelCreate %s\n", strerror (errno));
		return (NULL);
	}	
	event.sigev_notify = SIGEV_PULSE;
	event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0, chid_swap, _NTO_SIDE_CHANNEL, 0);
	if(-1 == event.sigev_coid){
		ltDebugErr("ConnectAttach %s\n", strerror (errno));
		return (NULL);
	}
	event.sigev_priority = getprio(0);
	event.sigev_code = _SWAP_PULSE_CODE;
	if(-1 == timer_create(CLOCK_REALTIME, &event, &swap_timer_id)){
		ltDebugErr("timer_create %s\n", strerror (errno));
		return (NULL);
	}
	
	while(1){
		unsigned short mch_err_cnt = 0;
	//StateToRaw( cur_chann, M3K96_MCH_STATE_OFF, &IpRaw);
	//StateToRaw( cur_chann, M3K96_MCH_STATE_OFF, &IpRaw);
		itime.it_value.tv_sec 		= 0;
		itime.it_value.tv_nsec 		= 0; 
		itime.it_interval.tv_sec 	= 0;
		itime.it_interval.tv_nsec 	= 0; 
		if(-1 == timer_settime(swap_timer_id, 0, &itime, NULL)){
			ltDebugErr("timer_create %s\n", strerror (errno));
			continue;
		}
		delay(500); // if open is error then wait 500ms and try again
		memset(&ip1,0,sizeof(ip1));
		memset(&ip2,0,sizeof(ip2));
		memset(&ip3,0,sizeof(ip3));
		memset(&ip4,0,sizeof(ip4));
		
		ltDebugMsg("start manch\n");
		printf( "bc open %s\n", path );
		ltDebugMsg("manch: open[%s]\n",path);
		mfd = BcOpen(path);
		if(-1 == mfd){
				ltDebugErr("manch: open() %s\n", strerror (errno));
				chSetOff(&chctl, _CH_MANCH_PRIM);
				continue;	
		}		
	    if (EOK != BcSetAddress(mfd,addr)){
	    		ltDebugErr("mrSetAddr() %s\n", strerror( errno ) );
	    		continue;
		}	
		chSetOn(&chctl,_CH_MANCH_PRIM );
		chSetOn(&chctl,_CH_MANCH_RES );
		itime.it_value.tv_sec 		= 1;
		itime.it_value.tv_nsec 		= _SWAP_INTERVAL_ns; 
		itime.it_interval.tv_sec 	= 0;
		itime.it_interval.tv_nsec 	= _SWAP_INTERVAL_ns; 
		if(-1 == timer_settime(swap_timer_id, 0, &itime, NULL)){
			ltDebugErr("timer_create %s\n", strerror (errno));
			continue;
		}
		while(1){
			int subaddr 	= 1;
			int sub_delay 	= 5000; //[mcs]
			int i = 0;
	//		int i = 0;
			if(chIsOff(&chctl,cur_chann ))
				StateToRaw( cur_chann, _CH_STATE_OFF, &IpRaw);
			else	
				StateToRaw( cur_chann, _CH_STATE_ON, &IpRaw);
			
			if(chIsOff(&chctl,res_chann ))
				StateToRaw( res_chann, _CH_STATE_OFF, &IpRaw);
			else	
				StateToRaw( res_chann, _CH_STATE_ON, &IpRaw);	
			// reserve timer pule
			
			
			
			
			
			if(-1 == MsgReceivePulse( chid_swap, &pulse, sizeof(pulse),NULL )){
				ltDebugErr("%s\n", strerror (errno));
				chSetOff(&chctl,cur_chann );
				chSetOff(&chctl,res_chann );
				continue;
			}
			if (_SWAP_PULSE_CODE != pulse.code){
				ltDebugWarn("manch: Unknown pulse code\n");
				chSetOff(&chctl,cur_chann );
				chSetOff(&chctl,res_chann );
				mch_err_cnt++;
				continue;
			}
			
			// if primary channel is OFF check reserve cannel
			//if(chIsOff(&chctl,cur_chann ))
				//check_cnt = 0;
			if( !checkPriCh(mfd, &chctl,&cur_chann))
				check_cnt = 0;
	
			// check reserve channel every 10 tic
			if(!(check_cnt%10)){ // swap interval = 0.1s check iterval = 1s ==> chech%10 by swap
				if(!checkCH(mfd, &chctl, &cur_chann, &res_chann))
					continue;
			}	
	    		
			// set channel		
	   		if(EOK != BcSetChannel(mfd, chan2ch(cur_chann)) ){
	   			ltDebugErr("manch: bcSetCannel() %s\n", strerror (errno));
	   			chSetOff(&chctl,cur_chann );
	   			mch_err_cnt++;
	    		continue;
	    	}
	   		
	   		ltDebugMsg("PRI=%d  RES=%d \n",IpRaw.map->state.mch_pri_state,IpRaw.map->state.mch_res_state);
	    	//////////////////////////////////////////////////////////////////////////////////////////////////

	   		delay (10);
	   		// write IP 1 2
			subaddr = 8;
			BcFrameReset(frame);
			
			RawToMip1(&ip1,&IpRaw);
			
			ip1.activeChannel = 1*(cur_chann==_CH_MANCH_PRIM)+2*(cur_chann==_CH_MANCH_RES);
			clock_gettime( CLOCK_REALTIME, &curr) ;// получаем текущее время
			ct = (curr.tv_sec-lastTimeKU.tv_sec)*1.0+(curr.tv_nsec-lastTimeKU.tv_nsec)*1.0/1000000000;
			if ((ct<0.0) | (ct>5.0)) ct = 0.0;
			
			switch (ip1.speedAnt) {
					case 0:	speed = 0; break;
					case 1:	speed = 3; break;
					case 2:	speed = 6; break;
					case 4:	speed = 30;	break;
					default: speed = 0; break;
				}
			//speed = 30;
			rotation = mtkdo2pi(lastKU+ct*speed*M_PI/30);	
			ip1.kuAnt =  (unsigned short)rotation/M_PI*(1<<15);
			ltDebugMsg("lastKU = %0.2f speed =%0.0f ip1.kuAnt = %0.2f %d\n",lastKU,speed,rotation,ip1.kuAnt);
			// TODO сюда надо дописать экстраполяцию вращения антенны
			ip1.numberIP2 =0;
			for(i=0; i < MTK_TARGET_NUM; i+= 4) if (ip2.tgt[i].nMTK >0) ip1.numberIP2 ++;
	   		if (EOK != BcSetSubAddress(mfd,BC_SUBADDR_SET_PKS(subaddr))){
	 //  		if (EOK != BcSetSubAddress(mfd,subaddr)){
	   			ltDebugErr( "ch[%d] bcSetSubAddr() %s\n", cur_chann, strerror( errno ) );
	   			chSetOff(&chctl,cur_chann );
	   			continue;
			}			
	   		
	   		
	   		if(EOK != BcWrite(mfd,&ip1,sizeof(ip1))){
							ltDebugErr("ch[%d] write ip1() %s\n", cur_chann,strerror( errno ) );
							chSetOff(&chctl,cur_chann );
							continue;
						}	
			// сюда ip2
			for(i=0; i < MTK_TARGET_NUM; i+= 4)
			{
				subaddr++;
				if (EOK != BcSetSubAddress(mfd,BC_SUBADDR_SET_PKS(subaddr))){
					ltDebugErr( "ch[%d] bcSetSubAddr() %s\n", cur_chann, strerror( errno ) );
					chSetOff(&chctl,cur_chann );
					continue;
				}		
				RawToMip2(&ip2,&IpRaw,i);
				if(EOK != BcWrite(mfd,&ip2.tgt[i],4*sizeof(ip2.tgt[i]))){
							ltDebugErr("ch[%d] write ip2() %s\n", cur_chann,strerror( errno ) );
							chSetOff(&chctl,cur_chann );
							continue;
						}	
			}
			for (i = 0; i < MTK_TARGET_NUM; ++i) {
				if (ip2.tgt[i].process >0) 
					ltDebugMsg("ResG0[%i]:nMTK=%d process=%d GO=%d t1=%d t2=%d\n",
							i,ip2.tgt[i].nMTK,ip2.tgt[i].process,ip2.tgt[i].GO,ip2.tgt[i].t1,ip2.tgt[i].t2); 
				if (ip2.tgt[i].process ==2) 
					ltDebugMsg("!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				
			}
			
			
			delay(20);
	   		
	   		// read IP 3
			subaddr = 1;
//			if (EOK != BcSetSubAddress(mfd,subaddr)){
				if (EOK != BcSetSubAddress(mfd,BC_SUBADDR_SET_PKS(subaddr))){
   				ltDebugErr( "bcSetSubAddr() %s\n", strerror( errno ) );
   				chSetOff(&chctl,cur_chann );
   				mch_err_cnt++;
   				continue;
			}
			if(EOK != BcRead(mfd,&ip3,sizeof(ip3))){
				ltDebugErr("manch: read() %s\n", strerror( errno ) );
				chSetOff(&chctl,cur_chann );
				mch_err_cnt++;
				continue;
			}
			Mip3ToRaw(&ip3,&IpRaw);
			
			// read IP 4
			subaddr = 4;
//			for(i=0; i < min(ip3.numberIP4, MTK_TARGET_NUM); i+= _MTK_IP4_TGT_MAX_NUM){
			for(i=0; i < MTK_TARGET_NUM; i+= _MTK_IP4_TGT_MAX_NUM){
				if (EOK != BcSetSubAddress(mfd,BC_SUBADDR_SET_PKS(subaddr))){
	   				ltDebugErr( "bcSetSubAddr() %s\n", strerror( errno ) );
	   				chSetOff(&chctl,cur_chann );
	   				mch_err_cnt++;
	   				continue;
				}
				if(EOK != BcRead(mfd,&ip4,sizeof(ip4))){
					ltDebugErr("manch: read() %s\n", strerror( errno ) );
					chSetOff(&chctl,cur_chann );
					mch_err_cnt++;
					continue;
				}
				Mip4ToRaw(&ip4,&IpRaw,i);
				subaddr ++;
				
			}
	   		
			
		} //while
	}//while	
	ltDebugMsg("manch: exit\n");
	return(NULL);
}
/// ---- ///
int checkPriCh(int fd, ChannCtrl_t* chtl,int* cur){
   	if(checkManchChann(fd,*cur)){
		chSetOn(chtl,*cur );
		ltDebugMsg("manch: cur chann chesk OK\n");
		return(1);
	}	
	else{		
		chSetOff(chtl,*cur );
		ltDebugErr("manch: cur chann chesk ERROR\n");
	}	
	return(0);
}

int checkCH(int fd, ChannCtrl_t* chtl,int* cur, int* res){
   	if(checkManchChann(fd,*res)){
		chSetOn(chtl,*res );
		ltDebugMsg("manch: res chann chesk OK\n");
	}	
	else{		
		chSetOff(chtl,*res );
		ltDebugWarn("manch: res chann chesk ERROR\n");
	}	
	// if primary channel is OFF try to ON reserve channel
	if(chIsOff(chtl, *cur)){
		ltDebugWarn("manch: cur chann ERROR \n");
		if(chIsOff(chtl,*res )){
			ltDebugWarn("manch: all chann ERROR \n");
			return(0);
		}	
		else{
			int tt 	= *cur;	
			*cur 	= *res;
			*res 	= tt;
			ltDebugWarn("manch swap channel \n");
		}	
	}	
	return(1);
}

int chan2ch(int ch){
	if(_CH_MANCH_RES == ch)
		return(MCH_CANNEL_RESERVE);
	return(MCH_CANNEL_PRIMARY);
}

int RawToMip1(mMTK_Ip1_t *ip,IpRaw_t* ipraw){
	if(EOK != pthread_rwlock_rdlock(&(ipraw->rwl)))
		return(0);
	memcpy(ip, &(ipraw->map->ip1), sizeof(mMTK_Ip1_t));
	if(EOK != pthread_rwlock_unlock(&(ipraw->rwl)))
		return(0);
	return(1);	
}	

int RawToMip2(mMTK_Ip2_t *ip,IpRaw_t* ipraw,int shift){
	if(EOK != pthread_rwlock_rdlock(&(ipraw->rwl)))
		return(0);
	memcpy(&ip->tgt[shift], &(ipraw->map->ip2.tgt[shift]), 4*sizeof(mMTK_Ip2ResGoTgt_t));
	if(EOK != pthread_rwlock_unlock(&(ipraw->rwl)))
		return(0);
	return(1);	
}	
int Mip3ToRaw(mMTK_Ip3_t *ip, IpRaw_t* ipraw){
	unsigned short csip = 0;
	if(EOK != pthread_rwlock_wrlock(&(ipraw->rwl)))
		return(0);
	memcpy(&(ipraw->map->ip3), ip, sizeof(mMTK_Ip3_t));
	/////////////////////////////
	csip = GetCsIp((char*)(&(ipraw->map->ip3)), sizeof(mMTK_Ip3_t));
	if(csip != CsIp3){
		bsem_post(&(ipraw->sip.sip3));
		CsIp3 = csip;		
	}
	/////////////////////////////	
	memset(ipraw->map->ip4tgt,0,MTK_TARGET_NUM*sizeof(mMTK_Ip4Tgt_t) );	
	if(EOK != pthread_rwlock_unlock(&(ipraw->rwl)))
		return(0);
	return(1);	
}		

int Mip4ToRaw(mMTK_Ip4_t *ip, IpRaw_t* ipraw, int shift){
	unsigned short csip = 0;
	if(EOK != pthread_rwlock_wrlock(&(ipraw->rwl)))
		return(0);
	memcpy( &(ipraw->map->ip4tgt[shift]), ip, sizeof(mMTK_Ip4_t));
	/////////////////////////////
	csip = GetCsIp((char*)(&(ipraw->map->ip4tgt)), MTK_TARGET_NUM* sizeof(mMTK_Ip4Tgt_t));
	if(csip != CsIp4){
		bsem_post(&(ipraw->sip.sip4));
		CsIp4 = csip;		
	}
	/////////////////////////////	
	if(EOK != pthread_rwlock_unlock(&(ipraw->rwl)))
		return(0);
	return(1);	
}

	
int chInit(ChannCtrl_t* chtl){
	chtl->manch1 	= LT_OFF;
	chtl->manch2 	= LT_OFF;
	return(1);
}

int chSetOn(ChannCtrl_t* chtl, int chann ){
	switch(chann){
 		case _CH_MANCH_PRIM:	chtl->manch1 	= LT_ON; break;
 		case _CH_MANCH_RES:	chtl->manch2 	= LT_ON; break;
 		default:				break;
	}
	return(1);
}

int chSetOff(ChannCtrl_t* chtl, int chann ){
	switch(chann){
 		case _CH_MANCH_PRIM:	chtl->manch1 	= LT_OFF; break;
 		case _CH_MANCH_RES:	chtl->manch2 	= LT_OFF; break;
 		default:				break;
	}
	return(1);
}

int chIsOff(ChannCtrl_t* chtl, int chann ){
	switch(chann){
 		case _CH_MANCH_PRIM:	return(LT_OFF == chtl->manch1); break;
 		case _CH_MANCH_RES:	return(LT_OFF == chtl->manch2); break;
 		default:				break;
	}
	return(0);
}

int checkManchChann(int fd, int chann){
	const int cmd_check = 0x2;
	if(EOK != BcSetChannel(fd, chan2ch(chann)) ){
   		ltDebugErr("manch: bcSetCannel() %s\n", strerror (errno));
   		return(0);
	}
	if (EOK != BcCmd(fd,cmd_check)){
   		ltDebugErr("bcCmd() %s\n", strerror( errno ) );
   		return(0);
	}
	else
		return(1);		
}

int chIsActive(ChannCtrl_t* chtl, int chann ){
	if(!chIsOff(chtl,_CH_MANCH_PRIM)){ 
		if(_CH_MANCH_PRIM == chann){
			ltDebugMsg("is active MANCH_PRIM\n");
			return(1);
		}
		return(0);	
	}	
	if(!chIsOff(chtl,_CH_MANCH_RES)){ 
		if(_CH_MANCH_RES == chann){
			ltDebugMsg("is active MANCH_RES\n");
			return(1);
		}	
		return(0);
	}	
	return(0);
}



int StateToRaw(unsigned short ch,unsigned short state,IpRaw_t* ipraw){
	if(EOK != pthread_rwlock_wrlock(&(ipraw->rwl)))
		return(0);
	switch(ch){
 		case _CH_MANCH_PRIM: ipraw->map->state.mch_pri_state = state; break;
 		case _CH_MANCH_RES: ipraw->map->state.mch_res_state = state; break;
 		default:				break;
	}
	if(EOK != pthread_rwlock_unlock(&(ipraw->rwl)))
		return(0);
	return(1);	
}		


unsigned short GetCsIp(char* data, int size){
	int i=0;
	unsigned short cs = 0;
	for(i=0;i<size;i++)
		cs ^= data[i];
	return(cs);	
}

int bsem_post(sem_t *sem){
	int value = 0;
	if(-1 == sem_getvalue(sem, &value) )
		return(-1);
	if(!value)
		return(sem_post(sem));
	return(0);		
}
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
