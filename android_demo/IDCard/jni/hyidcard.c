#include <stdlib.h>

#include "hyidcard.h"
#include "serial.h"
#include "debug.h"

#define UTF8_ONE_START      (0xOOO1)
#define UTF8_ONE_END        (0x007F)
#define UTF8_TWO_START      (0x0080)
#define UTF8_TWO_END        (0x07FF)
#define UTF8_THREE_START    (0x0800)
#define UTF8_THREE_END      (0xFFFF)


char szIDLastMessage[1024] = {0};

char photoinfo[1024] = {0};
char textinfo[256] = {0};
int photolen = 0;
int textlen = 0;

struct idtext textinfomation = {0};

int IDCard_Init(char *path_sv, int baudrate)
{
    int ret = -1;

    if(openPort(path_sv,8,'N',baudrate,1) == -1)
    {
    	LOGE("open port error");
        return -1;
    }

    return 0;
}

int IDCard_GetLastError(char *szMessage)
{
    strcpy(szMessage, szIDLastMessage);
    return 0;
}

int IDCard_Rlease()
{
	closePort();
    return 0;
}

int calc_check_sum(unsigned char *pkg, int len)
{
    int check_sum = 0;
    int i = 0;
    for (i = 0; i < len; i++)
    {
        check_sum ^= pkg[i];
    }
    return check_sum;
}

void show_data_id(unsigned char *data, int len)
{
    int i = 0;
    for(i = 0; i < len; i++)
    {
        LOGI("%02X", data[i]);
    }
}


typedef unsigned long   UTF32;  /* at least 32 bits */
typedef unsigned short  UTF16;  /* at least 16 bits */
typedef unsigned char   UTF8;   /* typically 8 bits */
typedef unsigned int    INT;
 
/*
0000 - 007F  0xxxxxxx
0080 - 07FF 110xxxxx 10xxxxxx
0800 - FFFF 1110xxxx 10xxxxxx 10xxxxxx
*/
 
#define UTF8_ONE_START      (0xOOO1)
#define UTF8_ONE_END        (0x007F)
#define UTF8_TWO_START      (0x0080)
#define UTF8_TWO_END        (0x07FF)
#define UTF8_THREE_START    (0x0800)
#define UTF8_THREE_END      (0xFFFF)
 
void UTF16ToUTF8(unsigned short* pUTF16Start, unsigned short* pUTF16End, unsigned char* pUTF8Start, unsigned char* pUTF8End)
{
    unsigned short* pTempUTF16 = pUTF16Start;
    unsigned char* pTempUTF8 = pUTF8Start;
 
    while (pTempUTF16 < pUTF16End)
    {
        if (*pTempUTF16 <= UTF8_ONE_END
            && pTempUTF8 + 1 < pUTF8End)
        {
            //0000 - 007F  0xxxxxxx
            *pTempUTF8++ = (unsigned char)*pTempUTF16;
        }
        else if(*pTempUTF16 >= UTF8_TWO_START && *pTempUTF16 <= UTF8_TWO_END
            && pTempUTF8 + 2 < pUTF8End) 
        {
            //0080 - 07FF 110xxxxx 10xxxxxx
            *pTempUTF8++ = (*pTempUTF16 >> 6) | 0xC0;
            *pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
        }
        else if(*pTempUTF16 >= UTF8_THREE_START && *pTempUTF16 <= UTF8_THREE_END
            && pTempUTF8 + 3 < pUTF8End)
        {
            //0800 - FFFF 1110xxxx 10xxxxxx 10xxxxxx
            *pTempUTF8++ = (*pTempUTF16 >> 12) | 0xE0;
            *pTempUTF8++ = ((*pTempUTF16 >> 6) & 0x3F) | 0x80;
            *pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
        }
        else
        {
            break;
        }
        pTempUTF16++;
    }
    *pTempUTF8 = 0;
}


int IDCard_get()
{
	
	memset(photoinfo,0,1024);
	memset(textinfo,0,256);
	memset(&textinfomation,0,sizeof(struct idtext));

    unsigned char pkg[10] = {0xAA, 0xAA, 0xAA, 0x96, 0x69, 0x00, 0x03, 0x20, 0x01};
    char *p = pkg;
    p += 5;
    pkg[9] = calc_check_sum(p, 4) & 0xff;
    //show_data_id(pkg, 10);

    int nLen = 10;
    if(writePort(pkg, nLen) != nLen)
    {
        LOGE("find card,send error");
        //return -1;
    }
    char szRecv[1024] = {0};
    int nRecvLen = 7;
    //printf("begin serial_recv  \n");
    if(readPort(szRecv, nRecvLen) != nRecvLen)
    {
        LOGE("find card,recv error1_1");
        //return -1;
    }
    //printf("end serial_recv 1 \n");
    //show_data_id((unsigned char*)szRecv, nRecvLen);

    nRecvLen = szRecv[5];
    nRecvLen = (nRecvLen << 8) | szRecv[6];
    //printf("begin serial_recv 2 nRecvLen = %d\n", nRecvLen);
    if(readPort(szRecv+7, nRecvLen) != nRecvLen)
    {
        LOGE("find card,recv error1_2");
        //return -1;
    }
    //printf("end serial_recv 2 \n");
    //show_data_id((unsigned char*)szRecv, nRecvLen+7);

    if (szRecv[7] != 0x00 || szRecv[8] != 0x00 || szRecv[9] != 0x9F)
    {
        //printf("%02x \n", szRecv[7]);
        //printf("%02x \n", szRecv[8]);
        //printf("%02x \n", szRecv[9]);

        LOGE("find card,data format error");
        //return -1;
    }


    unsigned char pkg1[10] = {0xAA, 0xAA, 0xAA, 0x96, 0x69, 0x00, 0x03, 0x20, 0x02};
    char *p1 = pkg1;
    p1 += 5;
    pkg1[9] = calc_check_sum(p1, 4) & 0xff;
    //show_data_id(pkg1, 10);

    int nLen1 = 10;
    if(writePort(pkg1, nLen1) != nLen1)
    {
        LOGE("select card,send error");
        //return -1;
    }
    char szRecv1[1024] = {0};
    int nRecvLen1 = 7;
    //printf("begin serial_recv 3 \n");
    if(readPort(szRecv1, nRecvLen1) != nRecvLen1)
    {
        LOGE("select card,recv error2_1");
        //return -1;
    }
    //printf("end serial_recv 3 \n");
    //show_data_id((unsigned char*)szRecv1, nRecvLen1);

    nRecvLen1 = szRecv1[5];
    nRecvLen1 = (nRecvLen1 << 8) | szRecv1[6];
    //printf("begin serial_recv 4 nRecvLen1 = %d\n", nRecvLen1);
    if(readPort(szRecv1+7, nRecvLen1) != nRecvLen1)
    {
        LOGE("select card,recv error2_2");
        //return -1;
    }
    //printf("end serial_recv 4 \n");
    //show_data_id((unsigned char*)szRecv1, nRecvLen1+7);

    if (szRecv1[7] != 0x00 || szRecv1[8] != 0x00 || szRecv1[9] != 0x90)
    {
        //printf("%02x \n", szRecv1[7]);
        //printf("%02x \n", szRecv1[8]);
        //printf("%02x \n", szRecv1[9]);

        LOGE("select card,data format error");
        //return -1;
    }

    unsigned char pkg2[10] = {0xAA, 0xAA, 0xAA, 0x96, 0x69, 0x00, 0x03, 0x30, 0x01};
    char *p2 = pkg2;
    p2 += 5;
    pkg2[9] = calc_check_sum(p2, 4) & 0xff;
    //show_data_id(pkg2, 10);

    int nLen2 = 10;
    if(writePort(pkg2, nLen2) != nLen2)
    {
        LOGE("read id card,send error");
        return -1;
    }
    char szRecv2[1536] = {0};
    int nRecvLen2 = 7;
    //printf("begin serial_recv 5 \n");
    if(readPort(szRecv2, nRecvLen2) != nRecvLen2)
    {
        LOGE("read id card,recv error3_1");
        return -1;
    }
    //printf("end serial_recv 5 \n");
    //show_data_id((unsigned char*)szRecv2, nRecvLen2);

    nRecvLen2 = 7;
    //printf("begin serial_recv 6 \n");
    if(readPort(szRecv2+7, nRecvLen2) != nRecvLen2)
    {
        LOGE("read id card,recv error3_2");
        return -1;
    }
    //printf("end serial_recv 6 \n");
    //show_data_id((unsigned char*)szRecv2, nRecvLen2+7);
	
    if (szRecv2[7] != 0x00 || szRecv2[8] != 0x00 || szRecv2[9] != 0x90)
    {
        //printf("%02x \n", szRecv2[7]);
        //printf("%02x \n", szRecv2[8]);
        //printf("%02x \n", szRecv2[9]);
		
    	LOGE("read id card,data format error");
        return -1;
    }

	textlen = szRecv2[10]<<8|szRecv2[11];
	photolen = szRecv2[12]<<8|szRecv2[13];
    //printf("begin serial_recv 6 \n");
	//show_data_id((unsigned char*)szRecv2+7, 7);
    if(readPort(szRecv2+14, textlen) != textlen)
    {
        LOGE("read id card image,recv error3_1");
        return -1;
    }
	memcpy(textinfo,szRecv2+14,textlen);

	//show_data_id((unsigned char*)szRecv2+14, textlen);
	if(readPort(szRecv2+14+textlen, photolen) != photolen)
    {
        LOGE("read id card image,recv error3_2");
        return -1;
    }
	memcpy(photoinfo,szRecv2+14+textlen,photolen);
	//printf("begin serial_recv 7\n");
	//show_data_id((unsigned char*)szRecv2+14+textlen, photolen);
    return 0;
}

int findCard(unsigned char *recv)
{
	int ret = -1;

	unsigned char pkg[10] = {0xAA, 0xAA, 0xAA, 0x96, 0x69, 0x00, 0x03, 0x20, 0x01};
	pkg[9] = calc_check_sum(&pkg[5], 4) & 0xff;

	int nLen = 10;
	if(writePort(pkg, nLen) != nLen)
	{
		LOGE("find card,send error");
		return -1;
	}

	unsigned char recvFind[1024] = {0};
	ret = readPkg(recvFind);
	if(ret <= 0)
	{
		LOGE("find card error");
		return -2;
	}
	LOGE("SW1 = 0x%02x,SW2 = 0x%02x,SW3 = 0x%02x",recvFind[7],recvFind[8],recvFind[9]);

	memcpy(recv,recvFind+7,ret-7);
	return ret;
}

int selectCard(unsigned char *recv)
{
	int ret = -1;

	unsigned char pkg[10] = {0xAA, 0xAA, 0xAA, 0x96, 0x69, 0x00, 0x03, 0x20, 0x02};
	pkg[9] = calc_check_sum(&pkg[5], 4) & 0xff;

	int nLen = 10;
	if(writePort(pkg, nLen) != nLen)
	{
		LOGE("find card,send error");
		return -1;
	}

	unsigned char recvFind[1024] = {0};
	ret = readPkg(recvFind);
	if(ret <= 0)
	{
		LOGE("find card error");
		return -2;
	}
	LOGE("SW1 = 0x%02x,SW2 = 0x%02x,SW3 = 0x%02x",recvFind[7],recvFind[8],recvFind[9]);
	memcpy(recv,recvFind+7,ret-7);
	return ret;
}

int readCard(unsigned char *txt,unsigned char *image)
{
	int ret = -1;

	unsigned char pkg[10] = {0xAA, 0xAA, 0xAA, 0x96, 0x69, 0x00, 0x03, 0x30, 0x01};
	pkg[9] = calc_check_sum(&pkg[5], 4) & 0xff;

	int nLen = 10;
	if(writePort(pkg, nLen) != nLen)
	{
		LOGE("find card,send error");
		return -1;
	}

	unsigned char recvFind[1295] = {0};
	ret = readPkg(recvFind);
	if(ret <= 0)
	{
		LOGE("find card error");
		return -2;
	}
	LOGE("SW1 = 0x%02x,SW2 = 0x%02x,SW3 = 0x%02x",recvFind[7],recvFind[8],recvFind[9]);

	memcpy(txt,recvFind+14,256);
	memcpy(image,recvFind+14+256,1024);

	memcpy(textinfo,txt,256);
	struct idtext* idtext = IDCard_getText();
	memcpy(txt,(void *)idtext,256);
	//unsigned char buf[256] = {0};
	//UTF16ToUTF8(textinfo, &textinfo[255], buf, &buf[255]);
	//LOGE("%s",buf);
	return ret;
}

struct idtext* IDCard_getText()
{
	int i = 0;
	int position = 0;//name
	
	UTF16ToUTF8(textinfo, &textinfo[29], textinfomation.name, &textinfomation.name[30]);
	//LOGE("name:%s",textinfomation.name);
	//position = 0;//sex
	//for(i = 30;i<32;i+=2){
	//	textinfomation.sex[position++] = textinfo[i];
	//}
	UTF16ToUTF8(textinfo+30, &textinfo[31], textinfomation.sex, &textinfomation.sex[2]);
	//LOGE("sex:%s\n",textinfomation.sex);
	
	//position = 0;//nation
	//for(i = 32;i<36;i+=2){
	//	textinfomation.nation[position++] = textinfo[i];
	//}
	
	UTF16ToUTF8(textinfo+32, &textinfo[35], textinfomation.nation, &textinfomation.nation[4]);
	
	//LOGE("nation:%s\n",textinfomation.nation);
	
	//position = 0;//startdate
	//for(i = 36;i<52;i+=2){
	//	textinfomation.startdate[position++] = textinfo[i];
	//}
	UTF16ToUTF8(textinfo+36, &textinfo[51], textinfomation.startdate, &textinfomation.startdate[16]);
	//LOGE("startdate:%s\n",textinfomation.startdate);
	//position = 0;//addr
	UTF16ToUTF8(textinfo+52, &textinfo[121], textinfomation.addr, &textinfomation.addr[70]);
	//LOGE("addr:%s\n",textinfomation.addr);
	//position = 0;//number

	//for(i = 122;i<158;i+=2){
	//	textinfomation.number[position++] = textinfo[i];
	//}
	UTF16ToUTF8(textinfo+122, &textinfo[157], textinfomation.number, &textinfomation.number[36]);
	//LOGE("number:%s\n",textinfomation.number);
	
	//position = 0;//police

	UTF16ToUTF8(textinfo+158, &textinfo[187], textinfomation.police, &textinfomation.police[31]);
	//LOGE("police:%s\n",textinfomation.police);
	
	//position = 0;//enddate
	//for(i = 188;i<220;i+=2){
	//	textinfomation.enddate[position++] = textinfo[i];
	//}
	UTF16ToUTF8(textinfo+188, &textinfo[219], textinfomation.enddate, &textinfomation.enddate[17]);
	//LOGE("enddate:%s\n",textinfomation.enddate);
	return &textinfomation;
}

char* IDCard_getPhoto()
{
	return &photoinfo;
}
