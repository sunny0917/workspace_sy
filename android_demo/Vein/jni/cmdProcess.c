#include <stdlib.h>

#include "protol.h"
#include "serial.h"
#include "debug.h"

const  int arBaud[]={50,75,110,134,150,200,300,600,1200,1800,2400,4800,9600,19200,38400,57600,115200,230400,460800,500000,
	576000,921600,1000000,1152000,1500000,2000000,2500000,3000000,3500000,4000000};

int  readPkt(unsigned char *buf, int  len/*, int timeout*/)
{
	int ret = -1;
	int sz=sizeof(md_dmpHdr);
	int total=0;
	int sum = 0,i = 0,calSum = 0;

	ret = readPort(&buf[total],sz - total);
	if (ret < 0)
	{
		LOGE("readPkt readPort error,ret = %d",ret);
		return 0;
	}

	total +=  sz;

	if((buf[0] != 0x01) && (buf[1] != 0xEF))
	{
		LOGE("readPkt startCode is not 0xef01,is 0x%02x02x",buf[1],buf[0]);
		return 0;
	}

	md_dmpHdr  *hdr=(md_dmpHdr*)buf;
	sz = hdr->length;

	ret = readPort(&buf[total],sz);
	if (ret < 0)
	{
		LOGE("readPkt readPort error,ret = %d",ret);
		return 0;
	}
	total += sz;
#if 1
	LOGI("recv:");
	for (i = 0;i<total;i++)
	{
		LOGI("%02x ",buf[i]);
	}
	LOGI("\n");
#endif
	for (i=sizeof(md_dmpHdr)-3;i<total-2;i++)
	{
		sum += buf[i];
	}

	sum = sum & 0xffff;
	calSum = (buf[total -2 ] & 0xFF) | ((buf[total - 1] & 0xFF) << 8);
	if (sum != calSum/**(unsigned short*)(buf+sizeof(md_dmpHdr)+hdr->length-2)*/)
	{
		LOGE("data check error,please check uart");
		return 0;
	}

	return total;
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

	if (len>500)
	{
		int ret=0;
		int chuk=128;
		int ds=chuk;
		do
		{
			ds=min(len-ret,chuk);
			ret += writePort((unsigned char*)sbuf+ret,ds);
			//sleep(1);
		} while (ret<len);
		return ret;
	}
	else
	{
		return writePort((unsigned char*)sbuf,len);

	}
}

int Cmd_openDev(int mode)
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;

	connectionStart(mode);
	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));

	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_openDev success");
			return SUCCESS;
		}
		else
		{
			result = res->pData[0];
			LOGE("Cmd_openDev fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_closeDev()
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	connectionStop();

	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_closeDev success");
			return SUCCESS;
		}
		else
		{
			result = res->pData[0];
			LOGE("Cmd_closeDev fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_setLedState(int color,int flush)
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	setLedState(color,flush);

	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));

	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("setLEDState success");
			return SUCCESS;
		}
		else
		{
			result = res->pData[0];
			LOGE("setLEDState fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_getButtonState()
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	getButtonState();

	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));

	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_getButtonState success");
			result = res->pData[1];
			return result;
		}
		else
		{
			result = res->pData[0];
			LOGE("Cmd_getButtonState fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_rollStep(unsigned char step)
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	md_dmpHdr *hdr=(md_dmpHdr*)rvData;
	enrollStep(step);
	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));

	if (readPkt(rvData,sizeof(rvData)))
	{
		result = res->pData[0];
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_rollStep success");
			return result;
		}
		else
		{
			LOGE("Cmd_rollStep fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_rollFinish(int id,unsigned char *modelId)
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	md_dmpHdr  *hdr=(md_dmpHdr*)rvData;
	enrollFinish(id);

	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_rollFinish success");
			//*modelId = res->pData[1] || (res->pData[2] << 8) || (res->pData[3] << 16) || (res->pData[4] << 24);
			memcpy(modelId,res->pData+1,4);

			return SUCCESS;
		}
		else
		{
			LOGE("Cmd_rollFinish fail");
			result = res->pData[0];
			return result;
		}
	}
	return ERROR;
}

int Cmd_identy(int *modelId)
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	identy();

	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));

	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_identy success");
			//*modelId = res->pData[1] || (res->pData[2] << 8) || (res->pData[3] << 14) || (res->pData[4] << 24);
			memcpy(modelId,res->pData+1,4);
			return SUCCESS;
		}
		else
		{
			result = res->pData[0];
			LOGE("Cmd_identy fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_getIDExist(int id,int *count)
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	getIDExist(id);

	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));

	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_getIDExist success");
			*count = res->pData[1];
			return SUCCESS;
		}
		else
		{
			result = res->pData[0];
			LOGE("Cmd_getIDExist fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_cancelWait()
{

	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	md_dmpHdr  *hdr=(md_dmpHdr*)rvData;
	cancelWait();
	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_cancelWait success");
			return SUCCESS;
		}
		else
		{
			result = res->pData[0];
			LOGE("Cmd_cancelWait fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_clearUser()
{

	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	delAllUser();
	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_clearUser success");
			return SUCCESS;
		}
		else
		{
			result = res->pData[0];
			LOGE("Cmd_clearUser fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_setUsbMode(unsigned char mode)
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	setUsbMode(mode);
	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_setUsbMode success");
			return SUCCESS;
		}
		else
		{
			result = res->pData[0];
			LOGE("Cmd_setUsbMode fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_getEmptyId(unsigned char* emptyId)
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	getEmptyId();
	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			memcpy(emptyId,res->pData+1,4);
			LOGE("Cmd_getEmptyId success");
			return SUCCESS;
		}
		else
		{
			result = res->pData[0];
			LOGE("Cmd_getEmptyId fail");
			return result;
		}
	}
	return 0;
}

int Cmd_delUser(int id)
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	delUser(id);
	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_delUser success");
			return SUCCESS;
		}
		else
		{
			result = res->pData[0];
			LOGE("Cmd_delUser fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_setBaudRate(unsigned char baud)
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	setBaudRate(baud);
	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_setBaudRate success");
			return SUCCESS;
		}
		else
		{
			result = res->pData[0];
			LOGE("Cmd_setBaudRate fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_setTimeOut(unsigned char tick)
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	setTimeOut(tick);
	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_setTimeOut success");
			return SUCCESS;
		}
		else
		{
			result = res->pData[0];
			LOGE("Cmd_setTimeOut fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_getUserNum(unsigned char *userNum,unsigned char *modelNum)
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	getUserNum();
	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_getUserNum success");
			memcpy(userNum,res->pData+1,4);
			memcpy(modelNum,res->pData+5,4);
			return SUCCESS;
		}
		else
		{
			result = res->pData[0];
			LOGE("Cmd_getUserNum fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_reset()
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	reset();
	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_reset success");
			return SUCCESS;
		}
		else
		{
			result = res->pData[0];
			LOGE("Cmd_reset fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_getSysInfo(unsigned char* content)
{
	int i = 0,j = 1;
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	getSystemInfo();
	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_getSysInfo success");
			content[i++] = res->pData[j++];	//main version
			content[i++] = res->pData[j++];	//sub version
			content[i++] = res->pData[j++];	//device id
			content[i++] = res->pData[j++];	//baud rate
			memcpy(&content[i],res->pData + j,2);	//level
			i += 2;
			j += 2;
			content[i++] = res->pData[j++];	//time out
			content[i++] = res->pData[j++];	//check dup
			content[i++] = res->pData[j++];	//check same finger
			content[i++] = res->pData[j++];	//usb mode
			content[i++] = res->pData[j++];	//read error
			return SUCCESS;
		}
		else
		{
			result = res->pData[0];
			LOGE("Cmd_getSysInfo fail");
			return result;
		}
	}
	return ERROR;
}

int Cmd_upLoadModel(unsigned char *data,int dataSize,int index)
{
	int ret = 0;
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	writeModelStart(dataSize,index);
	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
	sleep(1);

	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_upLoadModel start success");
			int pos=0;
			int chuk=512;
			int size=0;

			do
			{
				size=min(3352-pos,chuk);
				int  bEof = 0;
				if (pos + size == dataSize)
				{
					bEof = 1;
				}

				int sz = writeData(pos,data+pos,size,bEof);
				ret = sendData(m_txbuf,sz);

				if (readPkt(rvData,sizeof(rvData)))
				{
					if (res->pData[0] == PR_OK)
					{
						pos += size;
						LOGE("write data pos = %d\n",pos);
					}else{
						result = res->pData[0];
						LOGE("Cmd_upLoadModel fail");
						return result;
					}
				}
				else
				{
					LOGE("drop data size %d\n",pos);

				}
			}while(pos < dataSize);

			if(pos == dataSize)
			{
				LOGE("upload success");
				return SUCCESS;
			}
		}else{
			result = res->pData[0];
			LOGE("Cmd_upLoadModel start fail");
			return result;
		}
	}

	return ERROR;
}

int Cmd_downLoadModel(int packSize,int index,/*int modelSize,*/unsigned char *pOutData,int *pOutSize)
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	int chuk=128;
	int dataSize = 0;
	readModelStart(index,packSize);
	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
	if (readPkt(rvData,sizeof(rvData)))//响应包有模板大小信息
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_downLoadModel start success");
			int pos=0;
			memcpy(&dataSize,res->pData+1,4);
			do
			{
				readData(pos,index,dataSize);
				sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
				if (readPkt(rvData,sizeof(rvData)))
				{
					if (res->hdr.type == DATA_PKT )
					{
						memcpy(pOutData+pos,res->pData,res->hdr.length-2);
						pos += res->hdr.length-2;
						//pos =res->hdr.adr += res->hdr.length-2;
					}
					else if (res->hdr.type == EOF_PKT)
					{
						memcpy(pOutData+pos,res->pData,res->hdr.length-2);
						pos += res->hdr.length-2;
						//pos =res->hdr.adr += res->hdr.length-2;
						break;
					}
					else if (res->hdr.type == ACK_PKT)
					{
						if (res->pData[0] == PR_WRONG_PARA)
						{
							LOGE("para error");
						}
					}
				}
			}while(pos < dataSize);

			*pOutSize = pos;

			return SUCCESS;
		}else{
			result = res->pData[0];
			LOGE("Cmd_downLoadModel start fail");
			return result;
		}
	}

	return ERROR;
}

unsigned char Cmd_grabImage(unsigned short seq)
{
	unsigned char rvData[512];
	unsigned char result = 0;
	md_normal_res_pkt *res=(md_normal_res_pkt*)rvData;
	LOGE("====seq=%d",seq);
	grabImage(seq);
	sendData(m_txbuf,sizeof(md_normal_cmd_pkt));
	if (readPkt(rvData,sizeof(rvData)))
	{
		if (res->pData[0] == PR_OK)
		{
			LOGE("Cmd_grabImage success");
			return SUCCESS;
		}
		else
		{
			LOGE("Cmd_grabImage fail");
			result = res->pData[0];
			return result;
		}
	}
	return ERROR;
}
