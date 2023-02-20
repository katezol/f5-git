
#ifndef MANCH_H_
#define MANCH_H_


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <time.h>


int BcTest(int fd);

//////////////////////
//        BC        //
//////////////////////
typedef struct{
	unsigned short rt_fault	:1; 
	unsigned short get_rule :1;
	unsigned short sub_fault:1;
	unsigned short sub_busy	:1;
	unsigned short grp_cmd	:1;
	unsigned short pad		:3;
	unsigned short sc_req 	:1;//service request
	unsigned short aw_tag	:1;// 
	unsigned short err_msg	:1;//
	unsigned short rt_addr	:5;//
}BcAnswerWord_t;

typedef struct{
	unsigned short	err_word		:1;
	unsigned short	err_sync_pulse	:1;
	unsigned short	err_size		:1;
	unsigned short	err_addr		:1;
	unsigned short	pad				:2;
	unsigned short	repeat_msg		:1;
	unsigned short	flag_in_aw		:1;
	unsigned short	err_control		:1;
	unsigned short	no_aw			:1;
	unsigned short	err_format		:1;
	unsigned short	set_aw_mask_bit	:1;
	unsigned short	error			:1;
	unsigned short	channel			:1;
	unsigned short	start_cmd		:1;
	unsigned short	end_cmd			:1;
}BcStateWord_t;

typedef struct{
	unsigned short	rt_fault	:1;//19
	unsigned short 	sub_fault	:1;//17
	unsigned short 	sub_busy	:1;//16
	unsigned short 	group_cmd	:1;//15
	unsigned short 	sc_req 		:1;	//11 service request
	unsigned short 	err_msg		:1;	//9
	unsigned short	pad			:10;
}BcErrMask_t;


//  file descriptor:  Success.
// -1:	An error occurred (errno is set).
int BcOpen(const char* dev);
// For all below functions returns:
//  0:  Success. 
// -1:	An error occurred (errno is set).  
int	BcClose(int fd);
int	BcSetErrMask(int fd, BcErrMask_t* mask);
#define MCH_CANNEL_PRIMARY 0
#define MCH_CANNEL_RESERVE 1
int	BcSetChannel(int fd, unsigned short ch);
#define BC_BROADCAST_ADDR		(0x1F)
int	BcSetAddress(int fd, unsigned short addr);
#define BC_SUBADDR_SET_PKS(sa)  ((sa & 0x1F) | 0x10)
int	BcSetSubAddress(int fd, unsigned short subaddr);
int BcCmd(int fd, unsigned short cmd);
int BcCmdWD(int fd, unsigned short cmd, unsigned short* data);
int BcCmdRD(int fd, unsigned short cmd, unsigned short* data);
int BcWrite(int fd, void* data, unsigned short size);
int BcRead(int fd, void* data, unsigned short size);
int BcGetAW(int fd, BcAnswerWord_t* aw);
int BcGetSW(int fd, BcStateWord_t* sw);
//////////////////////
//     BC FRAME     //
//////////////////////
// BcFrame_t : point to memory, it allocate from BcFrameCreate(),
//  memory map (16-bit):
//  map[0] total size
//  map[1] cmd type, see BC_FRAME_CMD_TYPE_*
//  map[2] cmd size
//  map[3] cmd type, see BC_FRAME_CMD_TYPE_*
//  map[4] cmd size
//  etc...
typedef unsigned short* BcFrame_t;
BcFrame_t BcFrameCreate();
BcFrame_t BcFrameDestroy(BcFrame_t cc);
// For all functions returns:
//  0:  Success. 
// -1:	An error occurred (errno is set).  
int BcFrameReset(BcFrame_t cc);
int BcFrameCmd(BcFrame_t cc, unsigned short addr, unsigned short cmd);
int BcFrameCmdWD(BcFrame_t cc, unsigned short addr, unsigned short cmd, unsigned short* data);
int BcFrameWrite(BcFrame_t cc, unsigned short addr, unsigned short subaddr, void* data, unsigned short size);
int BcFrameDelay(BcFrame_t cc, unsigned long delay); // delay in [mcs], min = 10[mcs], max = 0xFFFF * 0xA[mcs]
int BcFrameStart(int fd, BcFrame_t cc);

//////////////////////
//        RT        //
//////////////////////
// RT event type:
// Error
#define MR_RT_EVENT_ERROR		0
// BC send cmd
#define MR_RT_EVENT_CMD 		1
// BC send cmd and read data word
#define MR_RT_EVENT_CMDWD 		2
// BC send cmd and write data word
#define MR_RT_EVENT_CMDRD 		3
// BC read from subaddress
#define MR_RT_EVENT_WDATA 		4
// BC write to subaddress
#define MR_RT_EVENT_RDATA 		5
// TimeOut
#define MR_RT_EVENT_TIMEOUT		6

typedef  struct{
	unsigned short naw	:1; // нет ОС (формат ОУ-ОУ, принемающее ОУ)
	unsigned short st	:1; // ошибка самоконтроля
	unsigned short ill	:1; // недопустимая команда
	unsigned short busy	:1; // занятость ОУ при приеме команды
	unsigned short bof	:1; // переполнение кольцевого буфера
	unsigned short ic	:1; // невозможная команда
	unsigned short isc	:1; // ошибка 2-о команды (формат ОУ-ОУ, принемающее ОУ)
	unsigned short iwc	:1; // неправильное количество слов
	unsigned short is	:1; // неправильный тип синхроимпульса
	unsigned short iw	:1; // неправильное слово
}RtEventErr_t;

typedef struct{
	unsigned short 	type;		// event type
	unsigned short 	channel;	// prim or res
	unsigned short 	cw;			// command word from BC
	unsigned short 	sw;			// RT state word
	RtEventErr_t	err;		// if type = ERROR err is available
	unsigned long	time;		// if type = TIMEOUT time[mks] is available
	unsigned short 	cmd;		// if type = CMD_* cmd is available
	unsigned short 	data;		// if type = CMD_R data is available
	unsigned short 	subaddr;    // if type = WRDATA data is available
	unsigned short 	size;		// if type = WRDATA size is available
}RtEvent_t;
// For all functions returns:
//  0:  Success. 
// -1:	An error occurred (errno is set).  
int RtOpen(const char* dev);
int	RtClose(int fd);
int	RtSetAddress(int fd, unsigned short addr);
#define MR_RT_ALLOW_CMD_ALL		0xFFFF
int RtAllowCmd(int fd, unsigned short cmd);
int RtSetTimeOut(int fd, unsigned long delay);// delay in [mcs], min = 10[mcs], max = 0xFFFF * 0xA[mcs]
int RtWriteCmdData(int fd, unsigned short cmd, unsigned short data);
int RtAllowSubAddress(int fd, unsigned short subaddr, unsigned short read_size, unsigned short write_size);
int RtAllowSubAddressNoShift(int fd, unsigned short subaddr);
int	RtSetSubAddress(int fd, unsigned short subaddr);
int RtEnable(int fd);
// next fuctions use only after RtEnable()
// max size 64[byte]
int RtWrite(int fd, void* data, unsigned short size);
int RtRead(int fd, void* data, unsigned short size);
int RtWaitEvent(int fd, RtEvent_t* event);

#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /*MANCH_H_*/
