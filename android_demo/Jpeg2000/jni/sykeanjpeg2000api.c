#include <jni.h>
#include <sys/select.h>
#include <sys/time.h>
#include <dlfcn.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdarg.h>

#include "./sykean_include/sykean_jpeg.h"


JNIEXPORT jint JNICALL Java_com_sykean_SykeanJpeg2000_allocSize(JNIEnv *env, jclass jc,jint width, jint height, jint cmprsSizeByte)
{
	jint ret = sykean_jpeg_alloc_size(width,height,cmprsSizeByte);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_SykeanJpeg2000_jpegCompress(JNIEnv *env, jclass jc,jbyteArray *pRawBuf, jint width, jint height,jbyteArray *pOutBuf, jintArray *pOutSize, jint cmprsSizeByte)
{
    int ret = -1;
    unsigned char* pRaw = NULL;
    char* pOut = NULL;
    jint* pOutLen = NULL;

    pRaw = (*env)->GetByteArrayElements(env, pRawBuf, 0);
    pOut = (*env)->GetByteArrayElements(env, pOutBuf, 0);
    pOutLen = (*env)->GetIntArrayElements(env, pOutSize, 0);
    ret = sykean_jpeg_compress(pRaw,width,height,pOut,pOutLen,cmprsSizeByte);

    (*env)->ReleaseByteArrayElements(env, pRawBuf, pRaw, 0);
    (*env)->ReleaseByteArrayElements(env, pOutBuf, pOut, 0);
    (*env)->ReleaseIntArrayElements(env, pOutSize, pOutLen, 0);
    return ret;
}
