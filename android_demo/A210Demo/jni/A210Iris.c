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
#include "proctol.h"

pthread_mutex_t send_mutex;

JNIEXPORT jint JNICALL Java_com_sykean_Iris_openPort(JNIEnv *env, jclass jc,jstring path,jint baudrate)
{
	jboolean iscopy;

	const char* path_sv = (*env)->GetStringUTFChars(env,path, &iscopy);

	pthread_mutex_init(&send_mutex, NULL);
	pthread_mutex_init(&queue_cs, NULL);
	pthread_mutex_init(&queue_start, NULL);
	pthread_cond_init(&queue_cv, NULL);

	jint ret = openPort(path_sv,8,'N',baudrate,1);
	LOGI("Java_com_sykean_Iris_openPort %s ret=%d",path_sv,ret);
	(*env)->ReleaseStringUTFChars(env,path,path_sv);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_closePort(JNIEnv *env, jclass jc)
{
    jint ret = -1;

    pthread_mutex_destroy(&send_mutex);
    pthread_mutex_destroy(&queue_cs);
    pthread_mutex_destroy(&queue_start);
    pthread_cond_destroy(&queue_cv);

    ret = closePort();
    LOGI("Java_com_sykean_Iris_closePort ret = %d",ret);
    return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_recvData(JNIEnv *env, jclass jc,jbyteArray recvData,jintArray recvLen)
{
	jint ret = -1;
	unsigned char* recvDate_tmp = NULL;
	unsigned int * recvLen_tmp = NULL;

	pthread_mutex_lock(&send_mutex);
	recvDate_tmp = (*env)->GetByteArrayElements(env, recvData, 0);
	recvLen_tmp = (*env)->GetIntArrayElements(env, recvLen, 0);

	ret = readMultiPkt(recvDate_tmp);

	if(recvDate_tmp[8] == CMD_RES_PACK)
	{
		pthread_mutex_lock(&queue_cs);
		pthread_cond_signal(&queue_cv);
		pthread_mutex_unlock(&queue_cs);
	}

	(*env)->ReleaseByteArrayElements(env, recvData, recvDate_tmp, 0);
	(*env)->ReleaseIntArrayElements(env, recvLen, recvLen_tmp, 0);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_startSignal(JNIEnv *env, jclass jc)
{
	jint ret = -1;
	pthread_mutex_lock(&send_mutex);

	ret = Cmd_startSignal();

	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_startIdentify(JNIEnv *env, jclass jc,jbyteArray userId)
{
	jint ret = -1;
	unsigned char* userId_tmp = NULL;

	pthread_mutex_lock(&send_mutex);
	userId_tmp = (*env)->GetByteArrayElements(env, userId, 0);

	ret = Cmd_startIdentify(userId_tmp);

	(*env)->ReleaseByteArrayElements(env, userId, userId_tmp, 0);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_startRegister(JNIEnv *env, jclass jc,jbyteArray userId)
{
	jint ret = -1;
	unsigned char* userId_tmp = NULL;

	pthread_mutex_lock(&send_mutex);
	userId_tmp = (*env)->GetByteArrayElements(env, userId, 0);

	ret = Cmd_startRegister(userId_tmp);

	(*env)->ReleaseByteArrayElements(env, userId, userId_tmp, 0);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_delModel(JNIEnv *env, jclass jc,jbyteArray userId)
{
	jint ret = -1;
	unsigned char* userId_tmp = NULL;

	pthread_mutex_lock(&send_mutex);
	userId_tmp = (*env)->GetByteArrayElements(env, userId, 0);

	ret = Cmd_delModel(userId_tmp);

	(*env)->ReleaseByteArrayElements(env, userId, userId_tmp, 0);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_getLocalUser(JNIEnv *env, jclass jc,jbyteArray userId)
{
	jint ret = -1;
	unsigned char* userId_tmp = NULL;

	pthread_mutex_lock(&send_mutex);
	userId_tmp = (*env)->GetByteArrayElements(env, userId, 0);

	ret = Cmd_getLocalUser(userId_tmp);

	(*env)->ReleaseByteArrayElements(env, userId, userId_tmp, 0);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_cancelAction(JNIEnv *env, jclass jc)
{
	jint ret = -1;
	pthread_mutex_lock(&send_mutex);

	ret = Cmd_cancelAction();

	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_getDevState(JNIEnv *env, jclass jc)
{
	jint ret = -1;
	pthread_mutex_lock(&send_mutex);

	ret = Cmd_getDevState();

	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_getModel(JNIEnv *env, jclass jc,jbyteArray userId)
{
	jint ret = -1;
	unsigned char* userId_tmp = NULL;

	pthread_mutex_lock(&send_mutex);
	userId_tmp = (*env)->GetByteArrayElements(env, userId, 0);

	ret = Cmd_getModel(userId_tmp);

	(*env)->ReleaseByteArrayElements(env, userId, userId_tmp, 0);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_downloadModel(JNIEnv *env, jclass jc,jbyteArray iris,jint irisLen)
{
	jint ret = -1;
	unsigned char* iris_tmp = NULL;

	pthread_mutex_lock(&send_mutex);
	iris_tmp = (*env)->GetByteArrayElements(env, iris, 0);

	ret = Cmd_downloadModel(iris_tmp,irisLen);

	(*env)->ReleaseByteArrayElements(env, iris, iris_tmp, 0);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_upgradeApp(JNIEnv *env, jclass jc,jbyteArray appData,jint appLen)
{
	jint ret = -1;
	unsigned char* appData_tmp = NULL;

	pthread_mutex_lock(&send_mutex);
	appData_tmp = (*env)->GetByteArrayElements(env, appData, 0);

	ret = Cmd_upgradeApp(appData_tmp,appLen);

	(*env)->ReleaseByteArrayElements(env, appData, appData_tmp, 0);
	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_switchIdentifyMode(JNIEnv *env, jclass jc,jbyte mode)
{
	jint ret = -1;
	pthread_mutex_lock(&send_mutex);

	ret = Cmd_switchIdentifyMode(mode);

	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_getCardId(JNIEnv *env, jclass jc)
{
	jint ret = -1;
	pthread_mutex_lock(&send_mutex);

	ret = Cmd_getCardId();

	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_capture(JNIEnv *env, jclass jc)
{
	jint ret = -1;
	pthread_mutex_lock(&send_mutex);

	ret = Cmd_capture();

	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_downloadProfile(JNIEnv *env, jclass jc,jbyte rZhd,jbyte rQxd,jbyte rXsd,jbyte vZhd,jbyte vQxd,jbyte vXsd)
{
	jint ret = -1;
	pthread_mutex_lock(&send_mutex);

	ret = Cmd_downloadProfile(rZhd,rQxd,rXsd,vZhd,vQxd,vXsd);

	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_startCreateModel(JNIEnv *env, jclass jc)
{
	jint ret = -1;
	pthread_mutex_lock(&send_mutex);

	ret = Cmd_startCreateModel();

	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_getRealtimeModel(JNIEnv *env, jclass jc)
{
	jint ret = -1;
	pthread_mutex_lock(&send_mutex);

	ret = Cmd_getRealtimeModel();

	pthread_mutex_unlock(&send_mutex);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_Iris_stopCreateModel(JNIEnv *env, jclass jc)
{
	jint ret = -1;
	pthread_mutex_lock(&send_mutex);

	ret = Cmd_stopCreateModel();

	pthread_mutex_unlock(&send_mutex);

	return ret;
}
