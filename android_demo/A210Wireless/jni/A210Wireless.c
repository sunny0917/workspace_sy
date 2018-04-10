#include <jni.h>
#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<assert.h>
#include "debug.h"

#define PATHSLEEP	"/sys/class/gpio_sw/PB7/data"
#define BUFSIZE	9
#define START	0x55
#define LENGTH	0x09
#define TYPE	0x00
#define ID		0x00
#define PORT	0x00
#define CMD		0x63
#define END		0xAA
char send[BUFSIZE];

JNIEXPORT jint JNICALL Java_com_sykean_a210wireless_A210Wireless_openPort(JNIEnv *env, jclass jc,jstring path,jint baudrate)
{
	jboolean iscopy;

	const char* path_sv = (*env)->GetStringUTFChars(env,path, &iscopy);
	LOGI("Java_com_sykean_a210wireless_A210Wireless_openPort %s",path_sv);
	jint ret = openPort(path_sv,8,'N',baudrate,1);
	LOGI("Java_com_sykean_a210wireless_A210Wireless_openPort ret=%d",ret);
	(*env)->ReleaseStringUTFChars(env,path,path_sv);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_a210wireless_A210Wireless_closePort(JNIEnv *env, jclass jc)
{
    int ret = -1;
    LOGE("Java_com_sykean_a210wireless_A210Wireless_closePort");
    ret = closePort();
    LOGE("Java_com_sykean_a210wireless_A210Wireless_closePort ret = %d",ret);
    return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_a210wireless_A210Wireless_enterSleep(JNIEnv *env, jclass jc,jint enable)
{
    int ret = -1;
    int sFp = -1;
    int value = 0;
    LOGE("Java_com_sykean_a210wireless_A210Wireless_enterSleep");
    value = enable;
    sFp = open(PATHSLEEP,O_RDWR);
    if(sFp < 0)
    {
        LOGE("open enter sleep file failed,sFp = %d",sFp);
        return ret;
    }
    ret = write(sFp,&enable,1);
    if(ret < 0)
    {
    	LOGE("write sleep file failed,ret = %d",ret);
    	return ret;
    }
    LOGE("Java_com_sykean_a210wireless_A210Wireless_enterSleep ret = %d",ret);
    return ret;
}

int makeFrame(char type,char id,char port,char cmd,char parm)
{
	int len = 0;
	int i = 1;
	char crc = 0x00;
	memset(send,0,BUFSIZE);
	send[len++] = START;
	send[len++]	= LENGTH;
	send[len++]	= type;
	send[len++]	= id;
	send[len++]	= port;
	send[len++]	= cmd;
	send[len++]	= parm;

	for(;i < len;i++)
	{
		crc += send[i];
	}
	send[len++]	= crc & 0xff;
	send[len++]	= END;
	return len;
}

JNIEXPORT jint JNICALL Java_com_sykean_a210wireless_A210Wireless_sendData(JNIEnv *env, jclass jc,jbyte type,jbyte id,jbyte port,jbyte cmd,jbyte parm)
{
	int ret = -1;
	int len = 0;
	LOGE("Java_com_sykean_a210wireless_A210Wireless_sendData");
	//jbyte * send_data_tmp = (jbyte*)(*env)->GetByteArrayElements(env,send_data, 0);
    //jsize  size = (*env)->GetArrayLength(env,send_data);
	len = makeFrame(type,id,port,cmd,parm);
	LOGE("sendData len = %d",len);
	ret = writePort(send,len);
	if(ret < 0)
	{
		LOGE("sendData failed ret = %d",ret);
		return ret;
	}
    //(*env)->ReleaseByteArrayElements(env, send_data,send_data_tmp, 0);
	LOGE("Java_com_sykean_a210wireless_A210Wireless_sendData ret = %d",ret);
    return ret;
}
