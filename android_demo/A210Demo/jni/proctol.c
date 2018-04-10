#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proctol.h"
#include "debug.h"

void calcBBC(void *pkt)
{
	int i;
	md_dmpHdr *hdr = (md_dmpHdr*)pkt;
	int BBC = 0;
	unsigned char *data = (unsigned char*)(hdr);
	for(i = 0;i< hdr->FrameLen + 1;i++)
	{
		BBC ^= data[i];
	}
	BBC = ~BBC;
	LOGI("[%s]:check BBC = %d\n",__func__,BBC);
	memcpy(&data[hdr->FrameLen + 1],&BBC,sizeof(unsigned char));
}

int pacelData(unsigned char cmd,unsigned char cmdType,unsigned char *info,unsigned short infoLen,unsigned short pack,unsigned short maxPack)
{
	md_normal_cmd_pkt *hdr=(md_normal_cmd_pkt*)m_txbuf;
	hdr->hdr.FrameTag = FRAMETAG;
	hdr->hdr.FrameLen = infoLen + 10;
	hdr->hdr.Pack = pack;
	hdr->hdr.MaxPack = maxPack;
	hdr->CmdType = cmdType;
	hdr->Cmd = cmd;
	hdr->Length = infoLen;
	memcpy(m_txbuf+sizeof(md_dmpHdr) + 4,info,infoLen);
	calcBBC(hdr);
	hdr->Etx = ETX;
	return sizeof(md_normal_cmd_pkt) + infoLen;
}

int startSignal()
{
	return pacelData(CMD_START,CMD_CONTROL,NULL,0,0,0);
}

int startIdentify(unsigned char *userId)
{
	return pacelData(CMD_START_IDENTIFY,CMD_CONTROL,userId,USERID_LEN,0,0);
}

int startRegister(unsigned char *userId)
{
	return pacelData(CMD_START_REGISTER,CMD_CONTROL,userId,USERID_LEN,0,0);
}

int delModel(unsigned char *userId)
{
	return pacelData(CMD_DEL_MODEL,CMD_CONTROL,userId,USERID_LEN,0,0);
}

int getLocalUser(unsigned char *userId)
{
	return pacelData(CMD_READ_USERID,CMD_CONTROL,userId,USERID_LEN,0,0);
}

int cancelAction()
{
	return pacelData(CMD_CANCEL,CMD_CONTROL,NULL,USERID_LEN,0,0);
}

int getDevState()
{
	return pacelData(CMD_GET_STATE,CMD_CONTROL,NULL,USERID_LEN,0,0);
}

int ack()
{
	unsigned char success = 0x00;
	return pacelData(CMD_RES_PACK,CMD_CONTROL,&success,0x01,0,0);
}

int getModel(unsigned char *userId)
{
	return pacelData(CMD_GET_IRIS_FOR_ID,CMD_CONTROL,userId,USERID_LEN,0,0);
}

int downloadModel(unsigned char *info,unsigned short infoLen,unsigned short pack,unsigned short maxPack)//info = userId + model
{
	return pacelData(CMD_ISSUED_IRIS_FOR_ID,CMD_CONTROL,info,infoLen,pack,maxPack);
}

int upgradeApp(unsigned char *info,unsigned short infoLen,unsigned short pack,unsigned short maxPack)
{
	return pacelData(CMD_ISSUED_APP,CMD_CONTROL,info,infoLen,pack,maxPack);
}

int downloadData(unsigned char cmd,unsigned char *info,unsigned short infoLen,unsigned short pack,unsigned short maxPack)
{
	switch(cmd)
	{
	case CMD_ISSUED_IRIS_FOR_ID:
		return pacelData(CMD_ISSUED_IRIS_FOR_ID,CMD_CONTROL,info,infoLen,pack,maxPack);
	case CMD_ISSUED_APP:
		return pacelData(CMD_ISSUED_APP,CMD_CONTROL,info,infoLen,pack,maxPack);
	default:
		return 0;
	}
}

int switchIdentifyMode(unsigned char mode)	//mode = 0x01:��ʶ��ģʽ	mode = 0x02:����ʶ��ģʽ
{
	return pacelData(CMD_CARD_IDENTIFY,CMD_CONTROL,&mode,0x01,0,0);
}

int getCardId()
{
	return pacelData(CMD_READ_M1,CMD_CONTROL,NULL,0,0,0);
}

int capture()
{
	return pacelData(CMD_CAPTUTE,CMD_CONTROL,NULL,0,0,0);
}

/**
 *info��6���ֽڣ�
 *ǰ�����ֽ�ע���������ź϶�(Ĭ��50)��������(Ĭ��60)��б�Ӷ�(Ĭ��50)	ȡֵ��Χ40~90�����·�FF FF FF����ʹ��Ĭ��ֵ
 *�������ֽ�ʶ���������ź϶�(Ĭ��50)��������(Ĭ��40)��б�Ӷ�(Ĭ��50)
 *���ú�������Ч���ϵ縴λ��ָ�Ĭ��ֵ������ע�����ʱ�����Ⱦ�����������ֻ�����ź϶Ⱥ�б�Ӷȣ�ֵԽС��ͼ������Ҫ��Խ�ͣ�ע���Խ���ף�
 *ֵԽ�󣬶�ͼ������Ҫ��Խ�ߣ�ע���Խ���ѣ�ʶ����ͬ����������ֻ���ڸ����˺���ע�������²ŵ���������ʹ��ʱ�벻Ҫ����
 */
int downloadProfile(unsigned char *info)
{
	return pacelData(CMD_ISSUED_PROFILE,CMD_CONTROL,info,0x06,0,0);
}

int startCreateModel()
{
	return pacelData(CMD_START_CREATE_IRIS_MODEL,CMD_CONTROL,NULL,0,0,0);
}

int getRealtimeModel()
{
	return pacelData(CMD_GET_REALTIME_MODEL,CMD_CONTROL,NULL,0,0,0);
}

int stopCreateModel()
{
	return pacelData(CMD_STOP_CREATE_IRIS_MODEL,CMD_CONTROL,NULL,0,0,0);
}
