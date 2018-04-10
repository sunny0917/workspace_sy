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
//Э�����ͷ���̶�����
typedef struct _md_dmpHdr_{
	md_u16     sig;				//��ͷ
	md_u16     seq;				//���
	md_u16     adr;				//��ַ
	md_u8	   type;			//������
	md_u16	   length;			//���ĳ���
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


//������
enum {
	CMD_PKT=1,						//�����
	DATA_PKT=0x02,					//���ݰ�
	ACK_PKT=0x07,					//�ظ���
	EOF_PKT=0x08					//���ݽ�����
};
//������
enum {
	CMD_DEV_OPEN,           //�豸��
	CMD_DEV_CLOSE,          //�豸�ر�
	CMD_SET_LED,            //�豸led��
	CMD_GET_BUTTON_STATE,   //����Ƭ״̬
	CMD_ROLL_STEP,          //��ģ����
	CMD_ROLL_STOP,          //��ģ���
	CMD_ROLL_STATE,         //����
	CMD_IDENTY_USER,        //��֤
	//CMD_GET_MODEL_INDEXS,   //��ȡ����ģ��id
	CMD_DEL_MODEL = 0x09,          //ɾ��ָ��ģ��
	CMD_DEL_ALL,            //ɾ������ģ��

	//CMD_PRINT_IMAGE,
	//CMD_RELEASE_FINGER,
	//CMD_REPRESS_FINGER,

	CMD_SET_TIMEOUT = 0x0E,            //������ʱ
	CMD_SET_DUPCHECK,			//�Ƿ�����ظ�ע��
	CMD_SET_SAMEFINGERCHECK,	//��ģ�Ƿ���ͬһ����ָ
	CMD_SET_BAUD_RATE,			//���ò�����
	CMD_GET_ID,				   //�鿴ָ��ģ���Ƿ�Ϊ��
	CMD_CANCEL_WAIT,			//ȡ���ȴ�
	CMD_GRAB_IMAGE,				//��ȡͼ��
	//CMD_SET_BACK_LIGHT,			//���ú����
	//CMD_SET_PASSWORD,			//��������
	CMD_SET_DEVICEID = 0x17,			//�����豸���
	CMD_REBOOT,					//����
	CMD_SET_SECURITY_LEVEL,			//
	CMD_GET_NEW_ID,
	//CMD_DEVICE_INIT,			//�豸��ʼ��
	CMD_RESET = 0x1C,					//�����豸Ϊ��ʼ״̬
	CMD_GET_SYS_INFO,			//��ȡϵͳ��Ϣ
	CMD_GET_USER_INFO,			//��ȡģ����Ϣ
	CMD_READ_START,				//��ȡ��Ϣ
	CMD_WRITE_START,			//д����Ϣ
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
	PR_OK,                              //�ɹ�
	//  PR_FAIL,
	PR_DEV_NOT_FIND,				//û���ҵ������豸
	PR_DEV_NOT_OPEN,				//�豸�޷���
	PR_DEV_OPENED,					//�豸�Ѿ���
	PR_QUALITY_LOW,                 //ָ��������̫��
	PR_DIFF_FINGER,					//��ͬ��ָ
	PR_FINGER_ROLLED,				//��ָ�Ѿ�ע��
	PR_WRONG_PARA,					//��������
	PR_TIME_OUT,					//��ʱ�˳�
	PR_PKT_ERROR,					//���������
	PR_ID_EXIST,                    //ע���id�Ѿ�����
	PR_DATA_ERROR,					//���ݼ������
	PR_EMPTY_ID,					//��id
	PR_NO_FRAME,					//û�в�ͼ
	PR_PRESS_FINGER,			    //������ָ
	PR_RELEAE_FINGER,				//�ɿ���ָ
	PR_WRONG=0xff					//δ֪����
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
