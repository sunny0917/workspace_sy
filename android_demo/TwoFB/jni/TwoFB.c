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

JNIEXPORT jint JNICALL Java_com_sykean_TwoFB_tofb(JNIEnv *env,jclass jc,jstring path,jbyteArray bmpImage)
{
	jint ret = -1;
	jboolean iscopy;

	unsigned char* bmp_tmp = NULL;

	const char* path_sv = (*env)->GetStringUTFChars(env,path, &iscopy);

	bmp_tmp = (*env)->GetByteArrayElements(env, bmpImage, 0);
	//ret = Cmd_rollFinish(id,bmp_tmp);
	ret = file_to_fb(path_sv);
	(*env)->ReleaseByteArrayElements(env, bmpImage, bmp_tmp, 0);

	(*env)->ReleaseStringUTFChars(env,path,path_sv);

	return ret;
}
