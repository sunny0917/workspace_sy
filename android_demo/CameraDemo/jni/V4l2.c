#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <assert.h>

#include "debug.h"
static int count = 0;
JNIEXPORT jint JNICALL Java_com_sykean_camerademo_V4l2_InitCamera(JNIEnv *env, jobject obj,jint id)
{
	int ret;

	ret = cam_open(id);
	if (ret != 0)
		return ret;

	ret = cam_select(id,0);
	if (ret != 0)
		return ret;

	ret = cam_init(id);
	if (ret != 0)
		return ret;

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_camerademo_V4l2_GetFrame(JNIEnv *env, jobject obj,jint id,jbyteArray frame)
{
	int framelen = 0;
    int ret = 0;
	unsigned char* raw = NULL;

	framelen = (*env)->GetArrayLength(env, frame);
    raw = (*env)->GetByteArrayElements(env, frame, 0);
    DBG("framelen = %d\n",framelen);
	int timeoutframe = 0;
    do{
        ret = cam_get_image(id,raw,framelen);
        DBG("ret = %d,count = %d\n",ret,++count);
        if(ret == 0)
        {
            (*env)->ReleaseByteArrayElements(env, frame, raw, 0);
            return 0;
        }
    }while(ret != 0);
	(*env)->ReleaseByteArrayElements(env, frame, raw, 0);
	return 0;

}

JNIEXPORT jint JNICALL Java_com_sykean_camerademo_V4l2_CloseCamera(JNIEnv *env, jobject obj,jint id)
{
	int ret;

	ret = cam_close(id);

	return ret;
}
