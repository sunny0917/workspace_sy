#ifndef __PROTOL_ZHY_H
#define __PROTOL_ZHY_H
#include<sys/time.h>
#include<pthread.h>

pthread_mutex_t queue_cs;
pthread_mutex_t queue_start;
pthread_cond_t queue_cv;
pthread_mutex_t get_lock_mux;

#pragma pack (1)

typedef  unsigned char   md_u8;
typedef  unsigned short  md_u16;
typedef  unsigned int  md_u32;

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))


#define   	CMD_PACKET_SIZE         525
#define 	INFO_SIZE				512
#define 	FRAMETAG				0x55
#define		ETX						0x33
#define 	USERID_LEN				24
#define 	FRAMELEN				3
#define		INFOINDEX				11
#define		CARDIDLEN				4


#pragma pack (1)
//协议包包头，固定长度
typedef struct _md_dmpHdr_{
	md_u8		FrameTag;				//帧头
	md_u16		FrameLen;				//帧长
	md_u16		Pack;					//包号
	md_u16		MaxPack;				//最大包数
}md_dmpHdr;

typedef  struct  _md_normal_cmd_pkt_
{
	md_dmpHdr	hdr;
	md_u8		CmdType;
	md_u8		Cmd;
	md_u16		Length;
	//md_u8		Info[INFO_SIZE];
	md_u8		BBC;
	md_u8		Etx;				//帧结束符
}md_normal_cmd_pkt;
#pragma pack ()

//命令类型
enum {
	CMD_CONTROL	=	0x01			//控制命令
};

//命令
enum {
	CMD_START			= 0x41,			//启动信号
	CMD_START_IDENTIFY	= 0x42,			//开始识别
	CMD_START_REGISTER	= 0x43,			//开始注册
	CMD_DEL_MODEL		= 0x44,   		//删除模板
	CMD_READ_USERID		= 0x45,			//读取用户ID
	CMD_CANCEL			= 0x46,			//取消当前动作
	CMD_GET_STATE		= 0x47,         //获取当前设备状态
	CMD_GET_IRIS_FOR_ID	= 0x48,			//获取ID对应的虹膜
	CMD_ISSUED_IRIS_FOR_ID	= 0x49,   		//下发对应ID的模板
	CMD_ISSUED_APP		= 0x4A,			//下发应用程序
	CMD_CARD_IDENTIFY	= 0x4B,         //切换成卡识别模式
	CMD_READ_M1			= 0x4C,			//读M1卡卡号
	CMD_START_CREATE_IRIS_MODEL	= 0x4D,	//开始产生虹膜模板
	CMD_GET_REALTIME_MODEL = 0x4E,	//获取实时模板
	CMD_CAPTUTE			= 0x4F,			//捕获图像
	CMD_STOP_CREATE_IRIS_MODEL = 0x51,	//结束产生虹膜模板
	CMD_RES_PACK		= 0x53,			//应答握手包
	CMD_VERIFY_ERR_HS_PACK = 0x54,		//校验错误握手包
	CMD_ISSUED_PROFILE	= 0x55,			//下发对应注册识别配置
	CMD_GET_FEEDBACK_INFO = 0x56,		//返回虹膜过程反馈信息
};

enum{
	PR_OK,                              //成功
	//  PR_FAIL,
	PR_DEV_NOT_FIND,				//没有找到可用设备
	PR_DEV_NOT_OPEN,				//设备无法打开
	PR_DEV_OPENED,					//设备已经打开
	PR_QUALITY_LOW,                 //指静脉质量太差
	PR_DIFF_FINGER,					//不同手指
	PR_FINGER_ROLLED,				//手指已经注册
	PR_WRONG_PARA,					//参数错误
	PR_TIME_OUT,					//超时退出
	PR_PKT_ERROR,					//包检验出错
	PR_ID_EXIST,                    //注册的id已经存在
	PR_DATA_ERROR,					//数据检验出错
	PR_EMPTY_ID,					//空id
	PR_NO_FRAME,					//没有采图
	PR_PRESS_FINGER,			    //按下手指
	PR_RELEAE_FINGER,				//松开手指
	PR_WRONG=0xff					//未知错误
};

enum{RTXBUF = 1024};

unsigned char             m_rxbuf[2*RTXBUF];
unsigned char             m_txbuf[RTXBUF];

#define  BOOL     int

#endif
