#ifndef __PROTOL_ZHY_H
#define __PROTOL_ZHY_H
#include<sys/time.h>


#pragma pack (1)

typedef  unsigned char   md_u8;
typedef  unsigned short  md_u16;
typedef  unsigned int  md_u32;

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))


#define   CMD_PACKET_SIZE          24

#pragma pack (1)
//协议包包头，固定长度
typedef struct _md_dmpHdr_{
	md_u16     sig;				//包头
	md_u16     seq;				//序号
	md_u16     adr;				//地址
	md_u8	   type;			//包类型
	md_u16	   length;			//包的长度
}md_dmpHdr;

typedef  struct  _md_normal_cmd_pkt_
{
	md_dmpHdr      hdr;
	md_u8		   cmd;
	md_u16		   wp;
	md_u16		   lp;
	md_u8          reserver[CMD_PACKET_SIZE-sizeof(md_dmpHdr)-7];
	md_u16		   sum;
}md_normal_cmd_pkt;

typedef  struct  _md_normal_res_pkt_
{
	md_dmpHdr      hdr;
	md_u8          pData[CMD_PACKET_SIZE-sizeof(md_dmpHdr)-2];
	md_u16		   sum;
}md_normal_res_pkt;


#pragma pack ()


//包类型
enum {
	CMD_PKT=1,						//命令包
	DATA_PKT=0x02,					//数据包
	ACK_PKT=0x07,					//回复包
	EOF_PKT=0x08					//数据结束包
};
//包命令
enum {
	CMD_DEV_OPEN,           //设备打开
	CMD_DEV_CLOSE,          //设备关闭
	CMD_SET_LED,            //设备led灯
	CMD_GET_BUTTON_STATE,   //触摸片状态
	CMD_ROLL_STEP,          //建模步骤
	CMD_ROLL_STOP,          //建模完成
	CMD_ROLL_STATE,         //保留
	CMD_IDENTY_USER,        //认证
	//CMD_GET_MODEL_INDEXS,   //获取所有模板id
	CMD_DEL_MODEL = 0x09,          //删除指定模板
	CMD_DEL_ALL,            //删除所有模板

	//CMD_PRINT_IMAGE,
	//CMD_RELEASE_FINGER,
	//CMD_REPRESS_FINGER,

	CMD_SET_TIMEOUT = 0x0E,            //设置延时
	CMD_SET_DUPCHECK,			//是否可以重复注册
	CMD_SET_SAMEFINGERCHECK,	//建模是否检查同一个手指
	CMD_SET_BAUD_RATE,			//设置波特率
	CMD_GET_ID,				   //查看指定模板是否为空
	CMD_CANCEL_WAIT,			//取消等待
	CMD_GRAB_IMAGE,				//获取图像
	//CMD_SET_BACK_LIGHT,			//设置红外灯
	//CMD_SET_PASSWORD,			//设置密码
	CMD_SET_DEVICEID = 0x17,			//设置设备编号
	CMD_REBOOT,					//重启
	CMD_SET_SECURITY_LEVEL,			//
	CMD_GET_NEW_ID,
	//CMD_DEVICE_INIT,			//设备初始化
	CMD_RESET = 0x1C,					//重置设备为初始状态
	CMD_GET_SYS_INFO,			//获取系统信息
	CMD_GET_USER_INFO,			//获取模板信息
	CMD_READ_START,				//读取信息
	CMD_WRITE_START,			//写入信息
	CMD_READ_DATA,
	CMD_WRITE_DATA,

	//CMD_SET_SESSOR,
	//CMD_GET_SESSOR,
	//CMD_IMAGE_WRITE_COMPLETE,
	//CMD_UPGRADE_SYSTEM,
	//CMD_UPGRADE_SYSTEM_COMPLETE,
	CMD_SET_USB_MODE = 0x28,




	CMD_RAW_BEGIN=128,
	CMD_RAW_END,
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

enum{ONLINE_MODE,OFFLINE_MODE};

enum{DATA_MODEL=1,  DATA_APP_MODULE, DATA_LOG};

enum{RTXBUF = 1024};

unsigned char             m_rxbuf[2*RTXBUF];
unsigned char             m_txbuf[RTXBUF];

#define  PACKET_SIZE    267
#define  CMD_PACKET_SIZE     24
#pragma pack (1)
typedef struct _zhy_dmpHdr_{
	unsigned short     sig;
	unsigned short     seq;
	unsigned short     adr;
	unsigned char  type;
	unsigned short length;
} zhy_dmpHdr;

typedef struct _zhy_packet_common_cmd_{
	zhy_dmpHdr     hdr;
	unsigned char pData[CMD_PACKET_SIZE-sizeof(zhy_dmpHdr)-2];
	unsigned       short  sum;
} zhy_packet_common_cmd;




typedef struct  _zhy_raw_data_{
	zhy_dmpHdr         hdr;
	unsigned  char     buf[PACKET_SIZE-sizeof(zhy_dmpHdr)];
} zhy_raw_data;


#pragma pack ()


#define  BOOL     int


void protol_init(void);
BOOL protol_rxData(unsigned char *buf, int len);
void protol_ackClient(zhy_packet_common_cmd *req,int result, unsigned char **buf, int *len);
void protol_ackClient_with_para(zhy_packet_common_cmd *req,int result,  void* para, int paraLen,unsigned char**buf, int *len);
void protol_indexUsers(short *index,int count,unsigned char **buf, int *len);
void protol_calcSum(void *hdr);
BOOL protol_parsePacket(zhy_dmpHdr *hdr);
BOOL protol_parse(unsigned char *buf, int len);
BOOL protol_checkSum(void *phdr);

zhy_packet_common_cmd *GetPacket(void);
void    addPacket(zhy_packet_common_cmd *cmd);


#endif
