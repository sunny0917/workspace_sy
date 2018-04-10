#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "protol.h"
#include "debug.h"


extern void processCmd(int cmd, int wp, int lp);

#define MAX_TIEMOUT (5*60)
#define HEART_TM        15

#define  tfalse   false
#define  ttrue     true
#if 0
int calcSum(unsigned char *pkt)
{
	int i = 0;
	int sum=pkt[6]+pkt[7]+pkt[8];
	int len=pkt[7]+((int)pkt[8])<<8;
	for(i=0;i<len-2;i++)
	{
		LOGE("sum = 0x%04x,pkt[%d]=0x%02x",sum,i,pkt[i+9]);
		sum += pkt[i+9];
	}
	LOGE("calcSum sum = %d",sum);
	sum = sum&0xffff;
	return sum;
}
#else
void calcSum(void *pkt)
{
	int i;
	md_dmpHdr *hdr=(md_dmpHdr*)pkt;
	int sum=0;//hdr->type+hdr->length;
	unsigned char *data=(unsigned char*)(hdr+1);
	for(i=-3;i<hdr->length-2;i++)
	{
		sum += data[i];
	}
	sum = sum&0xffff;
	LOGE("check sum = %d\n",sum);
	memcpy(&data[hdr->length-2],&sum,sizeof(short));
}
#endif

int packData(int cmd, void *data, int len)
{
	//memset(m_txbuf,0x00,RTXBUF);
	md_normal_cmd_pkt *hdr=(md_normal_cmd_pkt*)m_txbuf;
	hdr->hdr.type=CMD_PKT;
	//hdr->hdr.length=sizeof(md_normal_cmd_pkt)-sizeof(md_dmpHdr);
	//hdr->hdr.seq = 0x0000;
	hdr->hdr.seq = cmd;
	hdr->hdr.sig=0xef01;
	hdr->hdr.adr=0xffff;
	hdr->cmd=cmd;
	memcpy(m_txbuf+sizeof(md_dmpHdr)+1, data, len);
	hdr->hdr.length=sizeof(md_normal_cmd_pkt)-sizeof(md_dmpHdr);
	calcSum(hdr);
	return sizeof(md_normal_cmd_pkt);
}

int packDataPara(int cmd, int wp, int lp)
{
	//memset(m_txbuf,0x00,RTXBUF);
	md_normal_cmd_pkt *hdr=(md_normal_cmd_pkt*)m_txbuf;
	hdr->hdr.type=CMD_PKT;
	//hdr->hdr.seq = 0x0000;
	hdr->hdr.seq = cmd;
	hdr->cmd=cmd;
	hdr->hdr.sig=0xef01;
	hdr->hdr.adr=0xffff;
	hdr->wp=wp;
	hdr->lp=lp;
	hdr->hdr.length=sizeof(md_normal_cmd_pkt)-sizeof(md_dmpHdr);
	calcSum(hdr);
	return sizeof(md_normal_cmd_pkt);
}

void connectionStart(int mode)
{
	packDataPara(CMD_DEV_OPEN,mode,0);
}
void connectionStop()
{
	packDataPara(CMD_DEV_CLOSE,0,0);
}

void setLedState(int color,int flush)
{
	packDataPara(CMD_SET_LED,color,flush);
}

void getButtonState()
{
	packDataPara(CMD_GET_BUTTON_STATE,0,0);
}

void enrollStep(unsigned char step)
{
	packData(CMD_ROLL_STEP,&step,1);
}

void enrollFinish(int id)
{
	packData(CMD_ROLL_STOP,&id,4);
}

void identy()
{
	packDataPara(CMD_IDENTY_USER,0,0);
}

void getIDExist(int id)
{
	packData(CMD_GET_ID,&id,4);
}

void cancelWait()
{
	packDataPara(CMD_CANCEL_WAIT,0,0);
}

void delAllUser()
{
	packDataPara(CMD_DEL_ALL,0,0);
}

void setUsbMode(unsigned char mode)
{
	packData(CMD_SET_USB_MODE,&mode,1);
}

void getEmptyId()
{
	packDataPara(CMD_GET_NEW_ID,0,0);
}

void delUser(int id)
{
	packData(CMD_DEL_MODEL,&id,4);
}

void setBaudRate(unsigned char baudRate)
{
	packData(CMD_SET_BAUD_RATE,&baudRate,1);
}

void setTimeOut(unsigned char t)
{
	packData(CMD_SET_TIMEOUT,&t,1);
}

void getUserNum()
{
	packData(CMD_GET_USER_INFO,0,0);
}

void reset()
{
	packData(CMD_RESET,0,0);
}

void getSystemInfo()
{
	packDataPara(CMD_GET_SYS_INFO,0,0);
}

void writeModelStart(int dataSize,int index)
{
	unsigned char out[9];
	int i=0;
	out[i++]=DATA_MODEL;
	//int dataSize=3352;
	memcpy(out+i,&dataSize,4);
	i += 4;
	memcpy(out+i,&index,4);
	packData(CMD_WRITE_START,out,sizeof(out));
}
int writeData(int adr,void *data, int len,BOOL bEof)
{
	md_dmpHdr *hdr=(md_dmpHdr*)m_txbuf;
	if(!bEof)
	   hdr->type=DATA_PKT;
	else
		hdr->type = EOF_PKT;
	hdr->length=len+2;
	//hdr->seq = 0x0000;
	hdr->seq = CMD_WRITE_DATA;
	hdr->sig=0xef01;
	hdr->adr=adr;

	unsigned char *pBuff=(unsigned char*)(m_txbuf+sizeof(md_dmpHdr));

	memcpy(m_txbuf+sizeof(md_dmpHdr), data, len);

	calcSum(hdr);

	return sizeof(md_dmpHdr)+hdr->length;
}
void readModelStart(int index, int packSize)
{
	unsigned char out[9];
	int i=0;
	out[i++]=DATA_MODEL;

	memcpy(out+i,&packSize,4);
	i += 4;
	memcpy(out+i,&index,4);
	packData(CMD_READ_START,out,sizeof(out));
}
void readData(int adr,int index,int size)
{
	md_dmpHdr *hdr=(md_dmpHdr*)m_txbuf;
	hdr->type=CMD_PKT;
	hdr->length=sizeof(md_normal_cmd_pkt)-sizeof(md_dmpHdr);;
	//hdr->seq = 0x0000;
	hdr->seq = CMD_READ_DATA;
	hdr->sig=0xef01;
	hdr->adr=adr;

	unsigned char *pBuff=(unsigned char*)(m_txbuf+sizeof(md_dmpHdr));
	pBuff[0]=CMD_READ_DATA;
	pBuff[1] = DATA_MODEL;

	memcpy(&pBuff[2],&size,4);
	memcpy(&pBuff[6],&index,4);

	calcSum((unsigned char*)hdr);
}

void grabImage(unsigned short seq)
{
	packData(CMD_GRAB_IMAGE,&seq,2);
}
