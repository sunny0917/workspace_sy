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
//Э�����ͷ���̶�����
typedef struct _md_dmpHdr_{
	md_u8		FrameTag;				//֡ͷ
	md_u16		FrameLen;				//֡��
	md_u16		Pack;					//����
	md_u16		MaxPack;				//������
}md_dmpHdr;

typedef  struct  _md_normal_cmd_pkt_
{
	md_dmpHdr	hdr;
	md_u8		CmdType;
	md_u8		Cmd;
	md_u16		Length;
	//md_u8		Info[INFO_SIZE];
	md_u8		BBC;
	md_u8		Etx;				//֡������
}md_normal_cmd_pkt;
#pragma pack ()

//��������
enum {
	CMD_CONTROL	=	0x01			//��������
};

//����
enum {
	CMD_START			= 0x41,			//�����ź�
	CMD_START_IDENTIFY	= 0x42,			//��ʼʶ��
	CMD_START_REGISTER	= 0x43,			//��ʼע��
	CMD_DEL_MODEL		= 0x44,   		//ɾ��ģ��
	CMD_READ_USERID		= 0x45,			//��ȡ�û�ID
	CMD_CANCEL			= 0x46,			//ȡ����ǰ����
	CMD_GET_STATE		= 0x47,         //��ȡ��ǰ�豸״̬
	CMD_GET_IRIS_FOR_ID	= 0x48,			//��ȡID��Ӧ�ĺ�Ĥ
	CMD_ISSUED_IRIS_FOR_ID	= 0x49,   		//�·���ӦID��ģ��
	CMD_ISSUED_APP		= 0x4A,			//�·�Ӧ�ó���
	CMD_CARD_IDENTIFY	= 0x4B,         //�л��ɿ�ʶ��ģʽ
	CMD_READ_M1			= 0x4C,			//��M1������
	CMD_START_CREATE_IRIS_MODEL	= 0x4D,	//��ʼ������Ĥģ��
	CMD_GET_REALTIME_MODEL = 0x4E,	//��ȡʵʱģ��
	CMD_CAPTUTE			= 0x4F,			//����ͼ��
	CMD_STOP_CREATE_IRIS_MODEL = 0x51,	//����������Ĥģ��
	CMD_RES_PACK		= 0x53,			//Ӧ�����ְ�
	CMD_VERIFY_ERR_HS_PACK = 0x54,		//У��������ְ�
	CMD_ISSUED_PROFILE	= 0x55,			//�·���Ӧע��ʶ������
	CMD_GET_FEEDBACK_INFO = 0x56,		//���غ�Ĥ���̷�����Ϣ
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

enum{RTXBUF = 1024};

unsigned char             m_rxbuf[2*RTXBUF];
unsigned char             m_txbuf[RTXBUF];

#define  BOOL     int

#endif
