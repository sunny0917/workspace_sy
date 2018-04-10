#ifndef __IO_V4L2_H__
#define __IO_V4L2_H__

#include "debug.h"
#define VIDEO_DEVICE1 "/dev/video0"
#define VIDEO_DEVICE2 "/dev/video2"


#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480
#define IMAGE_SIZE (IMAGE_WIDTH * IMAGE_HEIGHT * 2)
#define BUFFER_COUNT 5

typedef long LONG;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef int int32_t;

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

int uvc1_fd = -1;
int uvc2_fd = -1;

struct v4l2_buffer video_buffer[BUFFER_COUNT];
u8* video_buffer_ptr[BUFFER_COUNT];
u8* video_buffer_ptr1[BUFFER_COUNT];
u8* video_buffer_ptr2[BUFFER_COUNT];
u8 buf[IMAGE_SIZE];
u8 uvc1buf[IMAGE_SIZE];
u8 uvc2buf[IMAGE_SIZE];
u8 raw8buf[IMAGE_SIZE/2];
u8 rgbbuf[IMAGE_WIDTH * IMAGE_HEIGHT  * 3];
u8 rgbbuf1[IMAGE_WIDTH * IMAGE_HEIGHT  * 3];
u8 rgbbuf2[IMAGE_WIDTH * IMAGE_HEIGHT  * 3];

#endif
