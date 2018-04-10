#include <jni.h>
#include <sys/select.h>
#include <sys/time.h>
#include <dlfcn.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdarg.h>
#include <fcntl.h>
#include<pthread.h>

#include "debug.h"
#include "serial.h"

pthread_mutex_t send_mutex;

#define TPDET_PATH		"/sys/class/gpio_sw/PI16/data"

JNIEXPORT jint JNICALL Java_com_sykean_Vein_openPort(JNIEnv *env, jclass jc,jstring path,jint baudrate)
{
	jboolean iscopy;

	const char* path_sv = (*env)->GetStringUTFChars(env,path, &iscopy);
	pthread_mutex_init(&send_mutex, NULL);
	jint ret = openPort(path_sv,8,'N',baudrate,1);
	LOGI("Java_com_sykean_Vein_openPort %s ret=%d",path_sv,ret);
	(*env)->ReleaseStringUTFChars(env,path,path_sv);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_closePort(JNIEnv *env, jclass jc)
{
    jint ret = -1;
    pthread_mutex_destroy(&send_mutex);
    ret = closePort();
    LOGE("Java_com_sykean_Vein_closePort ret = %d",ret);
    return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_getFingerTpDet(JNIEnv *env, jclass jc)
{
    jint ret = -1;
    jint state = -1;
    jint tFd = open(TPDET_PATH,O_RDWR);
    if(tFd < 0)
    {
    	LOGE("open %s fail",TPDET_PATH);
    	return ERROR;
    }

    ret = read(tFd,&state,1);
    if(ret < 0)
	{
		LOGE("read %s fail",TPDET_PATH);
		return ERROR;
	}

    LOGE("Java_com_sykean_Vein_getFingerTpDet state = %d",state);
    close(tFd);
    return state;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_openDev(JNIEnv *env,jclass jc,jshort para)
{
	jint ret = -1;
	pthread_mutex_lock(&send_mutex);
	ret = Cmd_openDev(para);
	pthread_mutex_unlock(&send_mutex);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_closeDev(JNIEnv *env,jclass jc)
{
	jint ret = -1;

	pthread_mutex_lock(&send_mutex);
	ret = Cmd_closeDev();
	pthread_mutex_unlock(&send_mutex);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_setLedState(JNIEnv *env,jclass jc,jshort color,jshort flush)
{
	jint ret = -1;

	pthread_mutex_lock(&send_mutex);
	ret = Cmd_setLedState(color,flush);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_getButtonState(JNIEnv *env,jclass jc)
{
	jint ret = -1;

	pthread_mutex_lock(&send_mutex);
	ret = Cmd_getButtonState();
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_rollStep(JNIEnv *env,jclass jc,jbyte curModel)
{
	jint ret = -1;

	pthread_mutex_lock(&send_mutex);
	ret = Cmd_rollStep(curModel);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_rollFinish(JNIEnv *env,jclass jc,jint id,jbyteArray modelId)
{
	jint ret = -1;

	unsigned char* model_id_tmp = NULL;
	pthread_mutex_lock(&send_mutex);
	model_id_tmp = (*env)->GetByteArrayElements(env, modelId, 0);
	ret = Cmd_rollFinish(id,model_id_tmp);

	(*env)->ReleaseByteArrayElements(env, modelId, model_id_tmp, 0);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_identy(JNIEnv *env,jclass jc,jbyteArray modelId)
{
	jint ret = -1;

	unsigned char* model_id_tmp = NULL;
	pthread_mutex_lock(&send_mutex);
	model_id_tmp = (*env)->GetByteArrayElements(env, modelId, 0);
	ret = Cmd_identy(model_id_tmp);

	(*env)->ReleaseByteArrayElements(env, modelId, model_id_tmp, 0);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_getIDExist(JNIEnv *env,jclass jc,jint id,jbyteArray count)
{
	jint ret = -1;

	unsigned char* count_tmp = NULL;
	pthread_mutex_lock(&send_mutex);
	count_tmp = (*env)->GetByteArrayElements(env, count, 0);
	ret = Cmd_getIDExist(id,count_tmp);

	(*env)->ReleaseByteArrayElements(env, count, count_tmp, 0);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_cancelWait(JNIEnv *env,jclass jc)
{
	jint ret = -1;

	pthread_mutex_lock(&send_mutex);
	ret = Cmd_cancelWait();
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_clearUser(JNIEnv *env,jclass jc)
{
	jint ret = -1;

	pthread_mutex_lock(&send_mutex);
	ret = Cmd_clearUser();
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_setUsbMode(JNIEnv *env,jclass jc,jbyte mode)
{
	jint ret = -1;

	pthread_mutex_lock(&send_mutex);
	ret = Cmd_setUsbMode(mode);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_getEmptyId(JNIEnv *env,jclass jc,jbyteArray newId)
{
	jint ret = -1;

	unsigned char* new_id_tmp = NULL;
	pthread_mutex_lock(&send_mutex);
	new_id_tmp = (*env)->GetByteArrayElements(env, newId, 0);
	ret = Cmd_getEmptyId(new_id_tmp);

	(*env)->ReleaseByteArrayElements(env, newId, new_id_tmp, 0);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_delUser(JNIEnv *env,jclass jc,jint modelId)
{
	jint ret = -1;

	pthread_mutex_lock(&send_mutex);
	ret = Cmd_delUser(modelId);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_setBaudRate(JNIEnv *env,jclass jc,jbyte speed)
{
	jint ret = -1;

	pthread_mutex_lock(&send_mutex);
	ret = Cmd_setBaudRate(speed);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_setTimeout(JNIEnv *env,jclass jc,jbyte timeout)
{
	jint ret = -1;

	pthread_mutex_lock(&send_mutex);
	ret = Cmd_setTimeOut(timeout);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_getUserNum(JNIEnv *env,jclass jc,jbyteArray userCount,jbyteArray modelCount)
{
	jint ret = -1;

	unsigned char* user_count_tmp = NULL;
	unsigned char* model_count_tmp = NULL;
	pthread_mutex_lock(&send_mutex);
	user_count_tmp = (*env)->GetByteArrayElements(env, userCount, 0);
	model_count_tmp = (*env)->GetByteArrayElements(env, modelCount, 0);
	ret = Cmd_getUserNum(user_count_tmp,model_count_tmp);

	(*env)->ReleaseByteArrayElements(env, userCount, user_count_tmp, 0);
	(*env)->ReleaseByteArrayElements(env, modelCount, model_count_tmp, 0);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_reset(JNIEnv *env,jclass jc)
{
	jint ret = -1;

	pthread_mutex_lock(&send_mutex);
	ret = Cmd_reset();
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_getSystemInfo(JNIEnv *env,jclass jc,jbyteArray systemInfo)
{
	jint ret = -1;

	unsigned char* info_tmp = NULL;
	pthread_mutex_lock(&send_mutex);
	info_tmp = (*env)->GetByteArrayElements(env, systemInfo, 0);
	ret = Cmd_getSysInfo(info_tmp);

	(*env)->ReleaseByteArrayElements(env, systemInfo, info_tmp, 0);
	pthread_mutex_unlock(&send_mutex);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_uploadModel(JNIEnv *env,jclass jc,jint modelId,jbyteArray data,jint dataSize)
{
	jint ret = -1;

	unsigned char* pData = NULL;
	pthread_mutex_lock(&send_mutex);
	pData = (*env)->GetByteArrayElements(env, data, 0);
	ret = Cmd_upLoadModel(pData,dataSize,modelId);

	(*env)->ReleaseByteArrayElements(env, data, pData, 0);
	pthread_mutex_unlock(&send_mutex);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Vein_downloadModel(JNIEnv *env,jclass jc,jint packSize,/*jint modelSize,*/jint modelId,jbyteArray fw,jintArray outSize)
{
	jint ret = -1;

	unsigned char* pFw = NULL;
	jint *pOutSize = NULL;
	pthread_mutex_lock(&send_mutex);
	pFw = (*env)->GetByteArrayElements(env, fw, 0);
	pOutSize = (*env)->GetIntArrayElements(env, outSize, 0);
	ret = Cmd_downLoadModel(packSize,modelId,/*modelSize,*/pFw,pOutSize);
	LOGE("pOutSize = %d",*pOutSize);
	(*env)->ReleaseByteArrayElements(env, fw, pFw, 0);
	(*env)->ReleaseIntArrayElements(env, outSize, pOutSize, 0);
	pthread_mutex_unlock(&send_mutex);
	return ret;
}

JNIEXPORT jbyte JNICALL Java_com_sykean_Vein_getImage(JNIEnv *env,jclass jc,jshort num)
{
	jbyte ret = -1;

	pthread_mutex_lock(&send_mutex);
	ret = Cmd_grabImage(num);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}
