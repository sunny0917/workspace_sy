#include <jni.h>
#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<assert.h>
#include "debug.h"

#define FRAMELEN	8
#define END		0xFF
char send[FRAMELEN];

JNIEXPORT jint JNICALL Java_com_sykean_IRUart_openPort(JNIEnv *env, jclass jc,jstring path,jint baudrate)
{
	jboolean iscopy;

	const char* path_sv = (*env)->GetStringUTFChars(env,path, &iscopy);
	LOGI("Java_com_sykean_IRUart_openPort %s",path_sv);
	jint ret = openPort(path_sv,8,'N',baudrate,1);
	LOGI("Java_com_sykean_IRUart_openPort ret=%d",ret);
	(*env)->ReleaseStringUTFChars(env,path,path_sv);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_IRUart_closePort(JNIEnv *env, jclass jc)
{
    int ret = -1;
    LOGE("Java_com_sykean_IRUart_closePort");
    ret = closePort();
    LOGE("Java_com_sykean_IRUart_closePort ret = %d",ret);
    return ret;
}

int makeFrame(char sn,char cmd,char* parm,int size)
{
	int len = 0;
	int i = 0;

	memset(send,0,FRAMELEN);
	send[len++] = sn;
	send[len++]	= cmd;
	for(;i < size;i++)
	{
		send[len++]	= parm[i];
	}
	send[len++]	= END;
	send[len++]	= END;
	return len;
}

JNIEXPORT jint JNICALL Java_com_sykean_IRUart_sendData(JNIEnv *env, jclass jc,jbyte sn,jbyte cmd,jbyteArray parm)
{
	int ret = -1;
	int len = 0;
	LOGE("Java_com_sykean_IRUart_sendData");
	jbyte * parm_tmp = (jbyte*)(*env)->GetByteArrayElements(env,parm, 0);
    jsize  size = (*env)->GetArrayLength(env,parm);

	len = makeFrame(sn,cmd,(char*)parm_tmp,(int)size);
	LOGE("sendData len = %d",len);
	ret = writePort(send,len);
	if(ret < 0)
	{
		LOGE("sendData failed ret = %d",ret);
		return ret;
	}
    (*env)->ReleaseByteArrayElements(env, parm,parm_tmp, 0);
	LOGE("Java_com_sykean_IRUart_sendData ret = %d",ret);
    return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_IRUart_recvData(JNIEnv *env, jclass jc,jbyteArray recv_data)
{
	int ret = -1;
	int len = 0;
    unsigned char* recv_data_tmp = NULL;
    //unsigned int* recv_len_tmp = NULL;
	LOGE("Java_com_sykean_IRUart_recvData");

    recv_data_tmp = (*env)->GetByteArrayElements(env, recv_data, 0);
    //recv_len_tmp = (*env)->GetIntArrayElements(env, recv_len, 0);

    ret = readPort(recv_data_tmp,FRAMELEN);
    LOGE("return(%d)\n",ret);

	(*env)->ReleaseByteArrayElements(env, recv_data, recv_data_tmp, 0);
	//(*env)->ReleaseIntArrayElements(env, recv_len, recv_len_tmp, 0);
	LOGE("Java_com_sykean_IRUart_recvData ret = %d",ret);
    return ret;
}
