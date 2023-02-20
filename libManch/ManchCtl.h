#ifndef MANCHCTL_H_
#define MANCHCTL_H_

#define BC_RAM_SIZE				0x0400

/////////////////////////
//     BC FRAME DEF    //
/////////////////////////
// BC_FRAME MEM HEAD
#define BC_FRAME_MEM_ADDR_TOTAL_SIZE		0x0
#define BC_FRAME_MEM_ADDR_CMD_CNT			0x1
#define BC_FRAME_MEM_HEAD_LEN				0x2

#define BC_FRAME_MEM_OFFSET_SIZE			0x0
#define BC_FRAME_MEM_OFFSET_TYPE			0x1

// BC_FRAME MEM DATA
#define BC_FRAME_MEM_OFFSET_DATA_ADDR		0x2
#define BC_FRAME_MEM_OFFSET_DATA_SUBADDR	0x3
#define BC_FRAME_MEM_OFFSET_DATA_SIZE		0x4
#define BC_FRAME_MEM_OFFSET_DATA_DATA		0x5

#define BC_FRAME_MEM_OFFSET_CMD_ADDR		0x2
#define BC_FRAME_MEM_OFFSET_CMD_CMD			0x3
#define BC_FRAME_MEM_OFFSET_CMD_SUBADDR		0x3
#define BC_FRAME_MEM_OFFSET_CMD_DATA		0x4

#define BC_FRAME_MEM_OFFSET_DELAY			0x2

#define BC_FRAME_CMD_TYPE_RULE				0x1
#define BC_FRAME_CMD_TYPE_RULE_WD			0x2
#define BC_FRAME_CMD_TYPE_RULE_RD			0x3
#define BC_FRAME_CMD_TYPE_DATA_WD			0x4
#define BC_FRAME_CMD_TYPE_DATA_RD			0x5
#define BC_FRAME_CMD_TYPE_DELAY				0x6

////////////////////////
//      CTRL DEF      //
////////////////////////
#define MR_CMD_TEST_		0
#define MR_CMD_TEST					__DIOT( _DCMD_MISC,  MR_CMD_TEST_ + 0, 0  )

//////////////////////
//        BC        //
//////////////////////
#define MR_CMD_BC 					(MR_CMD_TEST_ + 10)
#define MR_CMD_BC_CONFIG			__DIOT( _DCMD_MISC,  MR_CMD_BC + 0, 0  )
#define MR_CMD_BC_SET_ERR_MASK		__DIOT( _DCMD_MISC,  MR_CMD_BC + 1, BcErrMask_t  )
#define MR_CMD_BC_SET_CHANNEL		__DIOT( _DCMD_MISC,  MR_CMD_BC + 2, unsigned short  )
#define MR_CMD_BC_SET_ADDRESS		__DIOT( _DCMD_MISC,  MR_CMD_BC + 3, unsigned short  )
#define MR_CMD_BC_SET_SUBADDRESS	__DIOT( _DCMD_MISC,  MR_CMD_BC + 4, unsigned short  )
#define MR_CMD_BC_CMD_RULE			__DIOT( _DCMD_MISC,  MR_CMD_BC + 5, unsigned short  )
#define MR_CMD_BC_CMD_RULE_WD		__DIOT( _DCMD_MISC,  MR_CMD_BC + 6, unsigned short  )
#define MR_CMD_BC_CMD_RULE_RD		__DIOTF(_DCMD_MISC,  MR_CMD_BC + 7, unsigned short  )
#define MR_CMD_BC_CMD_WRITE			__DIOT( _DCMD_MISC,  MR_CMD_BC + 8, 0  )
#define MR_CMD_BC_CMD_READ			__DIOTF( _DCMD_MISC, MR_CMD_BC + 9, 0  )
#define MR_CMD_BC_GET_AW			__DIOF( _DCMD_MISC,  MR_CMD_BC + 10,0  )
#define MR_CMD_BC_GET_SW			__DIOF( _DCMD_MISC,  MR_CMD_BC + 11,0  )
#define MR_CMD_BC_FRAME				__DIOT( _DCMD_MISC,  MR_CMD_BC + 12, unsigned short  )

//////////////////////
//        RT        //
//////////////////////
#define MR_CMD_RT 					(MR_CMD_BC + 15)
#define MR_CMD_RT_CONFIG			__DIOT( _DCMD_MISC,  MR_CMD_RT + 0, 0  )
#define MR_CMD_RT_SET_ADDRESS		__DIOT( _DCMD_MISC,  MR_CMD_RT + 1, unsigned short )
#define MR_CMD_RT_ALLOW_CMD			__DIOT( _DCMD_MISC,  MR_CMD_RT + 2, unsigned short )
#define MR_CMD_RT_ALLOW_SA			__DIOT( _DCMD_MISC,  MR_CMD_RT + 3, unsigned short )
#define MR_CMD_RT_ENABLE			__DIOT( _DCMD_MISC,  MR_CMD_RT + 4, 0  )
#define MR_CMD_RT_WAIT_EVENT		__DIOF( _DCMD_MISC,  MR_CMD_RT + 5, RtEvent_t)
#define MR_CMD_RT_CMD_DATA			__DIOT( _DCMD_MISC,  MR_CMD_RT + 6, unsigned short )
#define MR_CMD_RT_WRITE_TO_SA		__DIOT( _DCMD_MISC,  MR_CMD_RT + 7, 0  )
#define MR_CMD_RT_READ_FROM_SA		__DIOTF( _DCMD_MISC, MR_CMD_RT + 8, 0  )
#define MR_CMD_RT_SET_SUBADDRESS	__DIOT( _DCMD_MISC,  MR_CMD_RT + 9, unsigned short )
#define MR_CMD_RT_ADD_DELAY			__DIOT( _DCMD_MISC,  MR_CMD_RT + 10, unsigned long )
#define MR_CMD_RT_ALLOW_SA_NOSHIFT	__DIOT( _DCMD_MISC,  MR_CMD_RT + 11, unsigned short )
#endif /*MANCHCTL_H_*/
