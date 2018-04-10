#include <jni.h>
#include <stdio.h>
#include "debug.h"
#include "hyidcard.h"

JNIEXPORT jint JNICALL Java_com_sykean_IDCard_openPort(JNIEnv *env, jclass jc,jstring path,jint baudrate)
{
	jboolean iscopy;

	const char* path_sv = (*env)->GetStringUTFChars(env,path, &iscopy);

	jint ret = openPort(path_sv,8,'N',baudrate,1);
	LOGI("Java_com_sykean_Vein_openPort %s ret=%d",path_sv,ret);
	(*env)->ReleaseStringUTFChars(env,path,path_sv);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_IDCard_closePort(JNIEnv *env, jclass jc)
{
    jint ret = -1;
    ret = closePort();
    LOGE("Java_com_sykean_Vein_closePort ret = %d",ret);
    return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_IDCard_findCard(JNIEnv *env, jclass jc,jbyteArray recvData)
{
	jint ret = -1;

	unsigned char* recvData_tmp = NULL;
	recvData_tmp = (*env)->GetByteArrayElements(env, recvData, 0);

	ret = findCard(recvData_tmp);

	(*env)->ReleaseByteArrayElements(env, recvData, recvData_tmp, 0);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_IDCard_selectCard(JNIEnv *env, jclass jc,jbyteArray recvData)
{
	jint ret = -1;

	unsigned char* recv_tmp = NULL;
	recv_tmp = (*env)->GetByteArrayElements(env, recvData, 0);

	ret = selectCard(recv_tmp);

	(*env)->ReleaseByteArrayElements(env, recvData, recv_tmp, 0);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_IDCard_readCard(JNIEnv *env, jclass jc,jbyteArray txtInfo,jbyteArray image)
{
	jint ret = -1;

	unsigned char* txt_tmp = NULL;
	unsigned char* image_tmp = NULL;
	txt_tmp = (*env)->GetByteArrayElements(env, txtInfo, 0);
	image_tmp = (*env)->GetByteArrayElements(env, image, 0);

	ret = readCard(txt_tmp,image_tmp);

	(*env)->ReleaseByteArrayElements(env, txtInfo, txt_tmp, 0);
	(*env)->ReleaseByteArrayElements(env, image, image_tmp, 0);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_IDCard_oneKeyReadCard(JNIEnv *env, jclass jc,jbyteArray txtInfo,jbyteArray image)
{
	jint ret = -1;

	unsigned char* txt_tmp = NULL;
	unsigned char* image_tmp = NULL;
	txt_tmp = (*env)->GetByteArrayElements(env, txtInfo, 0);
	image_tmp = (*env)->GetByteArrayElements(env, image, 0);

	ret = findCard(txt_tmp);
	ret = selectCard(txt_tmp);
	ret = readCard(txt_tmp,image_tmp);

	(*env)->ReleaseByteArrayElements(env, txtInfo, txt_tmp, 0);
	(*env)->ReleaseByteArrayElements(env, image, image_tmp, 0);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_IDCard_getText(JNIEnv *env, jclass jc)
{
    jint ret = -1;
    struct idtext* textinfo = NULL;

    ret = IDCard_get();
    if(ret < 0)
	{
    	LOGE("getText error");
    	return -1;
	}
	else
	{
		textinfo = IDCard_getText();
		LOGE("name:%s\n",textinfo->name);
		LOGE("sex:%s\n",textinfo->sex);
		LOGE("nation:%s\n",textinfo->nation);
		LOGE("startdate:%s\n",textinfo->startdate);
		LOGE("addr:%s\n",textinfo->addr);
		LOGE("number:%s\n",textinfo->number);
		LOGE("police:%s\n",textinfo->police);
		LOGE("enddate:%s\n",textinfo->enddate);
		LOGE("other:%s\n",textinfo->other);
	}
    LOGE("Java_com_sykean_IDCard_getText ret = %d",ret);
    return ret;
}
