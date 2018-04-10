#include <stdlib.h>

#include "proctol.h"
#include "serial.h"
#include "debug.h"

const  int arBaud[]={50,75,110,134,150,200,300,600,1200,1800,2400,4800,9600,19200,38400,57600,115200,230400,460800,500000,
	576000,921600,1000000,1152000,1500000,2000000,2500000,3000000,3500000,4000000};

int  readPkt(unsigned char *buf, unsigned short *len/*, int timeout*/)
{
	int ret = -1;
	int sz = FRAMELEN;//sizeof(md_dmpHdr);
	int total = 0;
	int sum = 0,i = 0,calSum = 0;

	ret = readPort(&buf[total],sz - total);
	if (ret < 0)
	{
		LOGE("[%s]:readPort error,ret = %d",__func__,ret);
		return ERROR;
	}

	total +=  sz;

	if((buf[0] != FRAMETAG))
	{
		LOGE("[%s]:FrameTag is not 0x55,is 0x%02x",__func__,buf[0]);
		return ERROR;
	}

	md_dmpHdr  *hdr=(md_dmpHdr*)buf;
	sz = hdr->FrameLen;

	ret = readPort(&buf[total],sz);
	if (ret < 0)
	{
		LOGE("[%s]:readPort data error,ret = %d",__func__,ret);
		return ERROR;
	}
	total += sz;
#if 1
	LOGI("[%s]:recv:",__func__);
	for (i = 0;i < total;i++)
	{
		LOGI("[%s]:%02x ",__func__,buf[i]);
	}
	LOGI("\n");
#endif
	for (i = 0;i < total - 2;i++)
	{
		sum ^= buf[i];
	}

	sum = ~sum;
	calSum = buf[total -2 ] & 0xFF;
	if (sum != calSum)
	{
		LOGE("[%s]:data check error,please check uart",__func__);
		return ERROR;
	}

	*len = total;
	return 0;
}

int readMultiPkt(unsigned char *data,unsigned int *dataLen)
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned int sendLen = 0;
	unsigned short rvLen = 0;
	unsigned short infoLen = 0;
	unsigned char result = 0;
	unsigned short pack = 0,maxPack = 0;
	unsigned int pos = 0;

	while(!readPkt(rvData,&rvLen))
	{
		pack = rvData[3] << 8 | rvData[4];
		maxPack = rvData[5] << 8 | rvData[6];
		infoLen = rvData[9] << 8 | rvData[10];

		memcpy(data + pos,&rvData[INFOINDEX],infoLen);
		pos += infoLen;

		if(pack < maxPack)
		{
			memset(rvData,0,sizeof(rvData));
			sendLen = ack();
			ret = sendData(m_txbuf,sendLen);
			if (ret < 0)
			{
				LOGE("[%s]:sendData ack error\n",__func__);
				return ERROR;
			}
			continue;
		}else if(pack == maxPack){
			*dataLen = pos;
			return SUCCESS;
		}
	}

	return ERROR;
}

int sendData(void *sbuf, int len)
{
	int i = 0;
	unsigned char *buf=(unsigned char*)sbuf;
	md_normal_cmd_pkt *cmd=(md_normal_cmd_pkt*)buf;

#if 1
	LOGI("send:");
	for (i=0;i<len;i++)
	{
		LOGI("%02x ",buf[i]);
	}
	LOGI("\n");
#endif

	if (len > 500)
	{
		int ret=0;
		int chuk=128;
		int ds = chuk;
		do
		{
			ds = min(len-ret,chuk);
			ret += writePort((unsigned char*)sbuf+ret,ds);
			//sleep(1);
		} while (ret < len);
		return ret;
	}
	else
	{
		return writePort((unsigned char*)sbuf,len);

	}
}

int sendMutliPkt(unsigned char cmd,unsigned char *data,unsigned int dataLen)
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned int sendLen = 0;
	unsigned short rvLen = 0;
	unsigned int pos = 0;
	unsigned short len = 0;
	unsigned short pack = 0,maxPack = 0;
	unsigned char count = 1;

	if(dataLen <= INFO_SIZE)
	{
		sendLen = downloadData(cmd,data,dataLen,pack,maxPack);
		ret = sendData(m_txbuf,sendLen);
		if(ret != sendLen)
		{
			LOGI("[%s][%d]:sendData fail\n",__func__,__LINE__);
			return ERROR;
		}

		if(!readPkt(rvData,&rvLen))
		{
			if(rvData[8] == cmd)
			{
				return rvData[INFOINDEX];
			}
		}
	}else{
		if(dataLen % INFO_SIZE == 0)
		{
			if(dataLen)
			{
				maxPack = dataLen/INFO_SIZE - 1;
			}else{
				maxPack = 0;
			}
		}else{
			maxPack = dataLen/INFO_SIZE;
		}

		for(;pack <= maxPack;pack++)
		{
		reSend:
			len = min(dataLen - pos,INFO_SIZE);
			sendLen = downloadData(cmd,data + pos,len,pack,maxPack);
			ret = sendData(m_txbuf,sendLen);
			if(ret != dataLen)
			{
				LOGI("[%s][%d]:sendData fail\n",__func__,__LINE__);
				return ERROR;
			}

			if(!readPkt(rvData,&rvLen))
			{
				if((rvData[8] == CMD_RES_PACK) && (rvData[INFOINDEX] == 0))
				{
					count = 1;
					pos	+= dataLen;
					continue;
				}else if(rvData[8] == cmd)
				{
					return rvData[INFOINDEX];
				}
			}else{
				if(count == 3)
				{
					LOGE("[%s][%d]:sendData no resp\n",__func__,__LINE__);
					return ERROR;
				}
				count++;
				goto reSend;
			}
		}
	}

	return ERROR;
}

int Cmd_startSignal(unsigned char *version)
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned short rvLen = 0;
	unsigned int dataLen = 0;
	unsigned char result = 0;

	dataLen = startSignal();
	ret = sendData(m_txbuf,dataLen);
	if(ret != dataLen)
	{
		LOGI("[%s]:sendData fail\n",__func__);
		return ERROR;
	}

	if (!readPkt(rvData,&rvLen))
	{
		memcpy(version,&rvData[INFOINDEX],4);
		return SUCCESS;
	}
	return ERROR;
}

int Cmd_startIdentify(unsigned char *userId,unsigned char *res)
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned short rvLen = 0;
	unsigned int dataLen = 0;
	unsigned char result = 0;

	dataLen = startIdentify(userId);
	ret = sendData(m_txbuf,dataLen);
	if(ret != dataLen)
	{
		LOGI("[%s]:sendData fail\n",__func__);
		return ERROR;
	}

	if (!readPkt(rvData,&rvLen))
	{
		memcpy(res,&rvData[INFOINDEX],24);
		return SUCCESS;
	}
	return ERROR;
}

unsigned char Cmd_startRegister(unsigned char *userId)
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned short rvLen = 0;
	unsigned int dataLen = 0;
	unsigned char result = 0;

	dataLen = startRegister(userId);
	ret = sendData(m_txbuf,dataLen);
	if(ret != dataLen)
	{
		LOGI("[%s]:sendData fail\n",__func__);
		return ERROR;
	}

	if (!readPkt(rvData,&rvLen))
	{
		return rvData[INFOINDEX];
	}
	return ERROR;
}

int Cmd_delModel(unsigned char *userId)
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned short rvLen = 0;
	unsigned int dataLen = 0;
	unsigned char result = 0;

	dataLen = delModel(userId);
	ret = sendData(m_txbuf,dataLen);
	if(ret != dataLen)
	{
		LOGI("[%s]:sendData fail\n",__func__);
		return ERROR;
	}

	if (!readPkt(rvData,&rvLen))
	{
		return rvData[INFOINDEX];
	}
	return ERROR;
}

int Cmd_getLocalUser(unsigned char *userId,unsigned char *resData,unsigned int *resLen)
{
	int ret = 0;
	unsigned int dataLen = 0;

	dataLen = getLocalUser(userId);
	ret = sendData(m_txbuf,dataLen);
	if(ret != dataLen)
	{
		LOGI("[%s]:sendData fail\n",__func__);
		return ERROR;
	}

	if(!readMultiPkt(resData,resLen))
	{
		return SUCCESS;
	}

	return ERROR;
}

int Cmd_cancelAction()
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned int dataLen = 0;
	unsigned short rvLen = 0;

	dataLen = cancelAction();
	ret = sendData(m_txbuf,dataLen);
	if(ret != dataLen)
	{
		LOGI("[%s]:sendData fail\n",__func__);
		return ERROR;
	}

	if (!readPkt(rvData,&rvLen))
	{
		return SUCCESS;
	}
	return ERROR;
}

int Cmd_getDevState(int *modelId)
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned int dataLen = 0;
	unsigned short rvLen = 0;

	dataLen = getDevState();
	ret = sendData(m_txbuf,dataLen);
	if(ret != dataLen)
	{
		LOGI("[%s]:sendData fail\n",__func__);
		return ERROR;
	}

	if (!readPkt(rvData,&rvLen))
	{
		return rvData[INFOINDEX];
	}
	return ERROR;
}

int Cmd_getModel(unsigned char *userId,unsigned char *resData,unsigned int *resLen)
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned int dataLen = 0;
	unsigned short rvLen = 0;

	dataLen = getModel(userId);
	ret = sendData(m_txbuf,dataLen);
	if(ret != dataLen)
	{
		LOGI("[%s]:sendData fail\n",__func__);
		return ERROR;
	}

	if (!readMultiPkt(resData,resLen))
	{
		return SUCCESS;
	}
	return ERROR;
}

int Cmd_downloadModel(unsigned char *iris,unsigned int irisLen)
{
	return sendMutliPkt(CMD_ISSUED_IRIS_FOR_ID,iris,irisLen);
}

int Cmd_upgradeApp(unsigned char *appData,unsigned int appLen)
{
	return sendMutliPkt(CMD_ISSUED_APP,appData,appLen);
}

int Cmd_switchIdentifyMode(unsigned char mode)
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned int dataLen = 0;
	unsigned short rvLen = 0;

	dataLen = switchIdentifyMode(mode);
	ret = sendData(m_txbuf,dataLen);
	if(ret != dataLen)
	{
		LOGI("[%s]:sendData fail\n",__func__);
		return ERROR;
	}

	if (!readPkt(rvData,&rvLen))
	{
		return rvData[INFOINDEX];
	}
	return ERROR;
}

int Cmd_getCardId(unsigned char* cardId)
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned int dataLen = 0;
	unsigned short rvLen = 0;

	dataLen = getCardId();
	ret = sendData(m_txbuf,dataLen);
	if(ret != dataLen)
	{
		LOGI("[%s]:sendData fail\n",__func__);
		return ERROR;
	}

	if (!readPkt(rvData,&rvLen))
	{
		memcpy(cardId,&rvData[INFOINDEX],CARDIDLEN);
		return SUCCESS;
	}
	return ERROR;
}

int Cmd_capture(unsigned char *image,unsigned int *imageLen)
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned int dataLen = 0;
	unsigned short rvLen = 0;

	dataLen = capture();
	ret = sendData(m_txbuf,dataLen);
	if(ret != dataLen)
	{
		LOGI("[%s]:sendData fail\n",__func__);
		return ERROR;
	}

	if(!readMultiPkt(image,imageLen))
	{
		return SUCCESS;
	}

	return ERROR;
}

int Cmd_downloadProfile(unsigned char rZhd,unsigned char rQxd,unsigned char rXsd,unsigned char vZhd,unsigned char vQxd,unsigned char vXsd)
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned int dataLen = 0;
	unsigned short rvLen = 0;
	unsigned char profile[6] = {0};

	profile[0] = rZhd;
	profile[1] = rQxd;
	profile[2] = rXsd;
	profile[3] = vZhd;
	profile[4] = vQxd;
	profile[5] = vXsd;

	dataLen = downloadProfile(profile);
	ret = sendData(m_txbuf,dataLen);
	if(ret != dataLen)
	{
		LOGI("[%s]:sendData fail\n",__func__);
		return ERROR;
	}

	if (!readPkt(rvData,&rvLen))
	{
		return rvData[INFOINDEX];
	}
	return ERROR;
}

int Cmd_startCreateModel()
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned int dataLen = 0;
	unsigned short rvLen = 0;

	dataLen = startCreateModel();
	ret = sendData(m_txbuf,dataLen);
	if(ret != dataLen)
	{
		LOGI("[%s]:sendData fail\n",__func__);
		return ERROR;
	}

	if (!readPkt(rvData,&rvLen))
	{
		return rvData[INFOINDEX];
	}
	return ERROR;
}

int Cmd_getRealtimeModel(unsigned char *userNum,unsigned char *modelNum)
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned int dataLen = 0;
	unsigned short rvLen = 0;

	dataLen = startCreateModel();
	ret = sendData(m_txbuf,dataLen);
	if(ret != dataLen)
	{
		LOGI("[%s]:sendData fail\n",__func__);
		return ERROR;
	}
	return ERROR;
}

int Cmd_stopCreateModel()
{
	int ret = 0;
	unsigned char rvData[525];
	unsigned int dataLen = 0;
	unsigned short rvLen = 0;

	dataLen = stopCreateModel();
	ret = sendData(m_txbuf,dataLen);
	if(ret != dataLen)
	{
		LOGI("[%s]:sendData fail\n",__func__);
		return ERROR;
	}
	return ERROR;
}
