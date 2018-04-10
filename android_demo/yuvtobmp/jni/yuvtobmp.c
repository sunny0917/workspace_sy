#include <jni.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <pthread.h>
#include<android/bitmap.h>

#include <android/log.h>

#define LOG_TAG "yuvtobmp"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGDebug(str)   __android_log_write(ANDROID_LOG_DEBUG,LOG_TAG,str)

#define  LOGD(...)  do {printf(__VA_ARGS__);printf("\n");} while (0)
#define DBG(fmt, args...) LOGD("%s:%d, " fmt, __FUNCTION__, __LINE__, ##args);

typedef int LONG;
typedef unsigned int DWORD;
typedef unsigned short WORD;

typedef struct {
        //WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
} BMPFILEHEADER_T;

typedef struct{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BMPINFOHEADER_T;

void yuyv_to_rgb(unsigned char* yuv,unsigned char* rgb,int width, int height)
{
    unsigned int i;
    unsigned char* y0 = yuv + 0;
    unsigned char* u0 = yuv + 1;
    unsigned char* y1 = yuv + 2;
    unsigned char* v0 = yuv + 3;

    unsigned  char* r0 = rgb + 0;
    unsigned  char* g0 = rgb + 1;
    unsigned  char* b0 = rgb + 2;
    unsigned  char* r1 = rgb + 3;
    unsigned  char* g1 = rgb + 4;
    unsigned  char* b1 = rgb + 5;
    LOGE("yuyv_to_rgb start\n");
    float rt0 = 0, gt0 = 0, bt0 = 0, rt1 = 0, gt1 = 0, bt1 = 0;

    for(i = 0; i <= (width * height) / 2 ;i++)
    {
        bt0 = 1.164 * (*y0 - 16) + 2.018 * (*u0 - 128);
        gt0 = 1.164 * (*y0 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128);
        rt0 = 1.164 * (*y0 - 16) + 1.596 * (*v0 - 128);

    	bt1 = 1.164 * (*y1 - 16) + 2.018 * (*u0 - 128);
        gt1 = 1.164 * (*y1 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128);
        rt1 = 1.164 * (*y1 - 16) + 1.596 * (*v0 - 128);

       	if(rt0 > 250)  	rt0 = 255;
		if(rt0 < 0)    	rt0 = 0;

		if(gt0 > 250) 	gt0 = 255;
		if(gt0 < 0)	gt0 = 0;

		if(bt0 > 250)	bt0 = 255;
		if(bt0 < 0)	bt0 = 0;

		if(rt1 > 250)	rt1 = 255;
		if(rt1 < 0)	rt1 = 0;

		if(gt1 > 250)	gt1 = 255;
		if(gt1 < 0)	gt1 = 0;

		if(bt1 > 250)	bt1 = 255;
		if(bt1 < 0)	bt1 = 0;

		*r0 = (unsigned char)rt0;
		*g0 = (unsigned char)gt0;
		*b0 = (unsigned char)bt0;

		*r1 = (unsigned char)rt1;
		*g1 = (unsigned char)gt1;
		*b1 = (unsigned char)bt1;

        yuv = yuv + 4;
        rgb = rgb + 6;
        if(yuv == NULL)
          break;

        y0 = yuv;
        u0 = yuv + 1;
        y1 = yuv + 2;
        v0 = yuv + 3;

        r0 = rgb + 0;
        g0 = rgb + 1;
        b0 = rgb + 2;
        r1 = rgb + 3;
        g1 = rgb + 4;
        b1 = rgb + 5;
    }
    LOGE("yuyv_to_rgb end\n");
}

int NV21_T_ARGB(unsigned int width , unsigned int height , unsigned char *yuyv , int *rgb)
{
	const int nv_start = width * height ;
    unsigned int  i, j, index = 0, rgb_index = 0;
    unsigned char y, u, v;
    int r, g, b, nv_index = 0;


    for(i = 0; i <  height ; i++)
    {
        for(j = 0; j < width; j ++){
            nv_index = i / 2  * width + j - j % 2;

            y = yuyv[rgb_index];
            v = yuyv[nv_start + nv_index ];
            u = yuyv[nv_start + nv_index + 1];


            //r = y + (140 * (v-128))/100;  //r
            //g = y - (34 * (u-128))/100 - (71 * (v-128))/100; //g
            //b = y + (177 * (u-128))/100; //b
            //r = y + 1.402 * (v-128);  //r
			//g = y - 0.34414 * (u-128) - 0.71414 * (v-128); //g
			//b = y + 1.772 * (u-128); //b

			r = (int)((y&0xff) + 1.4075 * ((v&0xff)-128));
			g = (int)((y&0xff) - 0.3455 * ((u&0xff)-128) - 0.7169*((v&0xff)-128));
			b = (int)((y&0xff) + 1.779 * ((u&0xff)-128));

            if(r > 255)   r = 255;
            if(g > 255)   g = 255;
            if(b > 255)   b = 255;
            if(r < 0)     r = 0;
            if(g < 0)     g = 0;
            if(b < 0)     b = 0;

            /*index = rgb_index % width + (height - i - 1) * width;
            rgb[index * 3+0] = r;
            rgb[index * 3+1] = g;
            rgb[index * 3+2] = b;*/
            rgb[index++] = (0 << 24) | (r << 16) | (g << 8) | b;
            rgb_index++;
        }
    }
    return 0;
}

/********´¦ÀíÏÔÊ¾Í¼Ïñ***********/
JNIEXPORT jint JNICALL Java_com_sykean_yuv_RatioImage(JNIEnv *env,jobject obj, jbyteArray Frame, jobject bmp, jint width, jint height) {
    int i, j, offset;
    int pos = 0;
    unsigned char tmp = 0;
    jint framelen = 0;
    unsigned char *frame = NULL;
    int *bitmap = NULL;
    unsigned char *rgb = NULL;

    framelen = (*env)->GetArrayLength(env, Frame);

    LOGE("framelen = %d,w= %d,h = %d",framelen,width,height);
    frame = (*env)->GetByteArrayElements(env, Frame, 0);
    if (AndroidBitmap_lockPixels(env, bmp, (void **)&bitmap) < 0) {
    	LOGE("can lock pixels\n");
        (*env)->ReleaseByteArrayElements(env, Frame, frame, 0);
        return -1;
    }
    NV21_T_ARGB(width,height,frame,(int *)bitmap);

    AndroidBitmap_unlockPixels(env, bmp);
    (*env)->ReleaseByteArrayElements(env, Frame, frame, 0);
    return 0;
}
