#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <pthread.h>
#include <linux/videodev2.h>

#include "ioV4l2.h"

static int Table_fv1[256] = {-180, -179, -177, -176, -174, -173, -172, -170, -169, -167, -166, -165,
                             -163, -162, -160, -159, -158, -156, -155, -153, -152, -151, -149, -148,
                             -146, -145, -144, -142, -141, -139, -138, -137, -135, -134, -132, -131,
                             -130, -128, -127, -125, -124, -123, -121, -120, -118, -117, -115, -114,
                             -113, -111, -110, -108, -107, -106, -104, -103, -101, -100, -99, -97,
                             -96, -94, -93, -92, -90, -89, -87, -86, -85, -83, -82, -80, -79, -78,
                             -76, -75, -73, -72, -71, -69, -68, -66, -65, -64, -62, -61, -59, -58,
                             -57, -55, -54, -52, -51, -50, -48, -47, -45, -44, -43, -41, -40, -38,
                             -37, -36, -34, -33, -31, -30, -29, -27, -26, -24, -23, -22, -20, -19,
                             -17, -16, -15, -13, -12, -10, -9, -8, -6, -5, -3, -2, 0, 1, 2, 4, 5, 7,
                             8, 9, 11, 12, 14, 15, 16, 18, 19, 21, 22, 23, 25, 26, 28, 29, 30, 32,
                             33, 35, 36, 37, 39, 40, 42, 43, 44, 46, 47, 49, 50, 51, 53, 54, 56, 57,
                             58, 60, 61, 63, 64, 65, 67, 68, 70, 71, 72, 74, 75, 77, 78, 79, 81, 82,
                             84, 85, 86, 88, 89, 91, 92, 93, 95, 96, 98, 99, 100, 102, 103, 105,
                             106, 107, 109, 110, 112, 113, 114, 116, 117, 119, 120, 122, 123, 124,
                             126, 127, 129, 130, 131, 133, 134, 136, 137, 138, 140, 141, 143, 144,
                             145, 147, 148, 150, 151, 152, 154, 155, 157, 158, 159, 161, 162, 164,
                             165, 166, 168, 169, 171, 172, 173, 175, 176, 178};
static int Table_fv2[256] = {-92, -91, -91, -90, -89, -88, -88, -87, -86, -86, -85, -84, -83, -83,
                             -82, -81, -81, -80, -79, -78, -78, -77, -76, -76, -75, -74, -73, -73,
                             -72, -71, -71, -70, -69, -68, -68, -67, -66, -66, -65, -64, -63, -63,
                             -62, -61, -61, -60, -59, -58, -58, -57, -56, -56, -55, -54, -53, -53,
                             -52, -51, -51, -50, -49, -48, -48, -47, -46, -46, -45, -44, -43, -43,
                             -42, -41, -41, -40, -39, -38, -38, -37, -36, -36, -35, -34, -33, -33,
                             -32, -31, -31, -30, -29, -28, -28, -27, -26, -26, -25, -24, -23, -23,
                             -22, -21, -21, -20, -19, -18, -18, -17, -16, -16, -15, -14, -13, -13,
                             -12, -11, -11, -10, -9, -8, -8, -7, -6, -6, -5, -4, -3, -3, -2, -1, 0,
                             0, 1, 2, 2, 3, 4, 5, 5, 6, 7, 7, 8, 9, 10, 10, 11, 12, 12, 13, 14, 15,
                             15, 16, 17, 17, 18, 19, 20, 20, 21, 22, 22, 23, 24, 25, 25, 26, 27, 27,
                             28, 29, 30, 30, 31, 32, 32, 33, 34, 35, 35, 36, 37, 37, 38, 39, 40, 40,
                             41, 42, 42, 43, 44, 45, 45, 46, 47, 47, 48, 49, 50, 50, 51, 52, 52, 53,
                             54, 55, 55, 56, 57, 57, 58, 59, 60, 60, 61, 62, 62, 63, 64, 65, 65, 66,
                             67, 67, 68, 69, 70, 70, 71, 72, 72, 73, 74, 75, 75, 76, 77, 77, 78, 79,
                             80, 80, 81, 82, 82, 83, 84, 85, 85, 86, 87, 87, 88, 89, 90, 90};
static int Table_fu1[256] = {-44, -44, -44, -43, -43, -43, -42, -42, -42, -41, -41, -41, -40, -40,
                             -40, -39, -39, -39, -38, -38, -38, -37, -37, -37, -36, -36, -36, -35,
                             -35, -35, -34, -34, -33, -33, -33, -32, -32, -32, -31, -31, -31, -30,
                             -30, -30, -29, -29, -29, -28, -28, -28, -27, -27, -27, -26, -26, -26,
                             -25, -25, -25, -24, -24, -24, -23, -23, -22, -22, -22, -21, -21, -21,
                             -20, -20, -20, -19, -19, -19, -18, -18, -18, -17, -17, -17, -16, -16,
                             -16, -15, -15, -15, -14, -14, -14, -13, -13, -13, -12, -12, -11, -11,
                             -11, -10, -10, -10, -9, -9, -9, -8, -8, -8, -7, -7, -7, -6, -6, -6, -5,
                             -5, -5, -4, -4, -4, -3, -3, -3, -2, -2, -2, -1, -1, 0, 0, 0, 1, 1, 1,
                             2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9,
                             10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 15, 16,
                             16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 22, 22,
                             22, 23, 23, 23, 24, 24, 24, 25, 25, 25, 26, 26, 26, 27, 27, 27, 28, 28,
                             28, 29, 29, 29, 30, 30, 30, 31, 31, 31, 32, 32, 33, 33, 33, 34, 34, 34,
                             35, 35, 35, 36, 36, 36, 37, 37, 37, 38, 38, 38, 39, 39, 39, 40, 40, 40,
                             41, 41, 41, 42, 42, 42, 43, 43};
static int Table_fu2[256] = {-227, -226, -224, -222, -220, -219, -217, -215, -213, -212, -210, -208,
                             -206, -204, -203, -201, -199, -197, -196, -194, -192, -190, -188, -187,
                             -185, -183, -181, -180, -178, -176, -174, -173, -171, -169, -167, -165,
                             -164, -162, -160, -158, -157, -155, -153, -151, -149, -148, -146, -144,
                             -142, -141, -139, -137, -135, -134, -132, -130, -128, -126, -125, -123,
                             -121, -119, -118, -116, -114, -112, -110, -109, -107, -105, -103, -102,
                             -100, -98, -96, -94, -93, -91, -89, -87, -86, -84, -82, -80, -79, -77,
                             -75, -73, -71, -70, -68, -66, -64, -63, -61, -59, -57, -55, -54, -52,
                             -50, -48, -47, -45, -43, -41, -40, -38, -36, -34, -32, -31, -29, -27,
                             -25, -24, -22, -20, -18, -16, -15, -13, -11, -9, -8, -6, -4, -2, 0, 1,
                             3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 21, 23, 24, 26, 28, 30, 31, 33, 35,
                             37, 39, 40, 42, 44, 46, 47, 49, 51, 53, 54, 56, 58, 60, 62, 63, 65, 67,
                             69, 70, 72, 74, 76, 78, 79, 81, 83, 85, 86, 88, 90, 92, 93, 95, 97, 99,
                             101, 102, 104, 106, 108, 109, 111, 113, 115, 117, 118, 120, 122, 124,
                             125, 127, 129, 131, 133, 134, 136, 138, 140, 141, 143, 145, 147, 148,
                             150, 152, 154, 156, 157, 159, 161, 163, 164, 166, 168, 170, 172, 173,
                             175, 177, 179, 180, 182, 184, 186, 187, 189, 191, 193, 195, 196, 198,
                             200, 202, 203, 205, 207, 209, 211, 212, 214, 216, 218, 219, 221, 223,
                             225};

int tim_subtract(struct timeval *result, struct timeval *x, struct timeval *y)
{
	if ( x->tv_sec > y->tv_sec )
	{
		return	-1;
	}
	if ((x->tv_sec==y->tv_sec) && (x->tv_usec>y->tv_usec))
	{
		return	-1;
	}
	result->tv_sec = ( y->tv_sec-x->tv_sec );
	result->tv_usec = ( y->tv_usec-x->tv_usec );
	if (result->tv_usec<0)
	{
		result->tv_sec--;
		result->tv_usec+=1000000;
	}
	return	0;
}

int raw10toraw8(u8 *pSrc,u8 *pDst,int w,int h)
{
        int i,j;
        u8 *p = NULL;
        u8 byte0,byte1;
        for(i = 0;i < h;i++)
        {
                p = pSrc + i * w * 2;
                for(j = 0;j < w;j++)
                {
                        byte0 = *(p++);
                        byte1 = *(p++);
                        *(pDst++) = (byte0 + ((byte1 & 0x03) << 8)) >> 2;
                }
        }
        return 0;
}

int YV12ToBGR24_Table(unsigned char *pYUV, unsigned char *pBGR32, int width, int height) {
    if (width < 1 || height < 1 || pYUV == NULL || pBGR32 == NULL) {
        return -1;
    }
    const long len = width * height;
    unsigned char *yData = pYUV;
    int k = 0;
    int bgr[8] = {0};
    int strip = width << 1;
    int yIdx, uIdx, vIdx, idx, y2Idex;
    int rdif, invgdif, bdif;
    int tag = width >> 1;
    int offset = 0;
    int i = 0;
    int j = 0;
    idx = 0;


    for (i = 0; i < height; i++) {

        offset = strip * i;

        for (j = strip - 1; j > 0; j -= 4) {

            yIdx = offset + (j - strip - 1);
            y2Idex = offset + (j - strip - 1) - 2;
            vIdx = offset + (j - strip - 1) - 3;
            uIdx = offset + (j - strip - 1) - 1; //blue

            rdif = Table_fv1[yData[vIdx]];
            invgdif = Table_fu1[yData[uIdx]] + Table_fv2[yData[vIdx]];
            bdif = Table_fu2[yData[uIdx]];

            bgr[0] = yData[yIdx] + rdif;        //red
            bgr[1] = yData[yIdx] - invgdif;        //geed
            bgr[2] = (yData[yIdx] + bdif);        //b
            bgr[3] = 0xFF;                        //alpa

            bgr[4] = yData[y2Idex] + rdif;        //rad
            bgr[5] = yData[y2Idex] - invgdif;    //geed
            bgr[6] = (yData[y2Idex] + bdif);    //b
            bgr[7] = 0xFF;                        //alpa

            for (k = 0; k < 8; k++) {
                if (bgr[k] >= 0 && bgr[k] <= 255) {
                    pBGR32[idx++] = bgr[k];
                }
                else {
                    pBGR32[idx++] = (bgr[k] < 0) ? 0 : 255;
                }
            }
        }
    }
    return 0;
}

void yuyv_to_rgb(unsigned char* yuv,unsigned char* rgb)
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
	DBG("yuyv_to_rgb start\n");
    float rt0 = 0, gt0 = 0, bt0 = 0, rt1 = 0, gt1 = 0, bt1 = 0;

    for(i = 0; i <= (IMAGE_WIDTH * IMAGE_HEIGHT) / 2 ;i++)
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

		*r0 = (unsigned char)bt0;
		*g0 = (unsigned char)gt0;
		*b0 = (unsigned char)rt0;

		*r1 = (unsigned char)bt1;
		*g1 = (unsigned char)gt1;
		*b1 = (unsigned char)rt1;

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
	DBG("yuyv_to_rgb end\n");
}

int rgb_to_bmp(unsigned char* pdata,  const char *pFileName, int width, int height)
{
    int ret = 0;
	FILE *bmp_fd = NULL;

    //分别为rgb数据，要保存的bmp文件名
    int size = width*height*3*sizeof(char); // 每个像素点3个字节
    // 位图第一部分，文件信息
    BMPFILEHEADER_T bfh;

	 bmp_fd = fopen(pFileName, "wb");
	if (NULL == bmp_fd)
	{
		ret = -3;
		return ret;
	}

	//bfh.bfType = (unsigned short)0x4d42;  //bm
	unsigned short bfType=0x4d42;
    bfh.bfSize = size  // data size
        + sizeof( BMPFILEHEADER_T ) // first section size
        + sizeof( BMPINFOHEADER_T ) // second section size
        ;
	//DBG("sizeof( BMPFILEHEADER_T )== %ld,sizeof( BMPINFOHEADER_T )=%ld\n", sizeof( BMPFILEHEADER_T ),sizeof( BMPINFOHEADER_T ));
    bfh.bfReserved1 = 0; // reserved
    bfh.bfReserved2 = 0; // reserved
    bfh.bfOffBits = sizeof( BMPFILEHEADER_T )+ sizeof( BMPINFOHEADER_T );//真正的数据的位置
    //DBG("bmp_head== %ld\n", bfh.bfOffBits);
    // 位图第二部分，数据信息
    BMPINFOHEADER_T bih;
    bih.biSize = sizeof(BMPINFOHEADER_T);
    bih.biWidth = width;
    bih.biHeight = -height;//BMP图片从最后一个点开始扫描，显示时图片是倒着的，所以用-height，这样图片就正了
    bih.biPlanes = 1;//为1，不用改
    bih.biBitCount = 24;
    bih.biCompression = 0;//不压缩
    bih.biSizeImage = size;

    bih.biXPelsPerMeter = 0;//像素每米

    bih.biYPelsPerMeter = 0;
    bih.biClrUsed = 0;//已用过的颜色，为0,与bitcount相同
    bih.biClrImportant = 0;//每个像素都重要

    fwrite(&bfType,sizeof(bfType),1,bmp_fd);
    fwrite( &bfh, 6, 1, bmp_fd);
    fwrite(&bfh.bfReserved2, sizeof(bfh.bfReserved2), 1, bmp_fd);
    fwrite(&bfh.bfOffBits, sizeof(bfh.bfOffBits), 1, bmp_fd);
    fwrite(&bih, sizeof(BMPINFOHEADER_T), 1, bmp_fd);

    fwrite(pdata, size, 1, bmp_fd);
	if (NULL != bmp_fd)
    {
        fclose(bmp_fd);
        bmp_fd = NULL;
    }
	return ret;
}
int rgb_to_bmp1(unsigned char* pdata,  unsigned char *bmp, int width, int height)
{
    int ret = 0;
	FILE *bmp_fd = NULL;

    //分别为rgb数据，要保存的bmp文件名
    int size = width*height*3*sizeof(char); // 每个像素点3个字节
    // 位图第一部分，文件信息
    BMPFILEHEADER_T bfh;

	if (NULL == bmp)
	{
		ret = -3;
		return ret;
	}

	//bfh.bfType = (unsigned short)0x4d42;  //bm
	unsigned short bfType=0x4d42;
    bfh.bfSize = size  // data size
        + sizeof( BMPFILEHEADER_T ) // first section size
        + sizeof( BMPINFOHEADER_T ) // second section size
        ;
	//DBG("sizeof( BMPFILEHEADER_T )== %ld,sizeof( BMPINFOHEADER_T )=%ld\n", sizeof( BMPFILEHEADER_T ),sizeof( BMPINFOHEADER_T ));
    bfh.bfReserved1 = 0; // reserved
    bfh.bfReserved2 = 0; // reserved
    bfh.bfOffBits = sizeof( BMPFILEHEADER_T )+ sizeof( BMPINFOHEADER_T );//真正的数据的位置
    //DBG("bmp_head== %ld\n", bfh.bfOffBits);
    // 位图第二部分，数据信息
    BMPINFOHEADER_T bih;
    bih.biSize = sizeof(BMPINFOHEADER_T);
    bih.biWidth = width;
    bih.biHeight = -height;//BMP图片从最后一个点开始扫描，显示时图片是倒着的，所以用-height，这样图片就正了
    bih.biPlanes = 1;//为1，不用改
    bih.biBitCount = 24;
    bih.biCompression = 0;//不压缩
    bih.biSizeImage = size;

    bih.biXPelsPerMeter = 0;//像素每米

    bih.biYPelsPerMeter = 0;
    bih.biClrUsed = 0;//已用过的颜色，为0,与bitcount相同
    bih.biClrImportant = 0;//每个像素都重要

    memcpy(bmp,&bfType,sizeof(bfType));
    memcpy(bmp+sizeof(bfType),&bfh,6);
    memcpy(bmp+sizeof(bfType)+6,&bfh.bfReserved2,sizeof(bfh.bfReserved2));
    memcpy(bmp+sizeof(bfType)+6+sizeof(bfh.bfReserved2),&bfh.bfOffBits,sizeof(bfh.bfOffBits));
    memcpy(bmp+sizeof(bfType)+6+sizeof(bfh.bfReserved2)+sizeof(bfh.bfOffBits),&bih,sizeof(BMPINFOHEADER_T));
    memcpy(bmp+sizeof(bfType)+6+sizeof(bfh.bfReserved2)+sizeof(bfh.bfOffBits)+sizeof(BMPINFOHEADER_T),&pdata,size);
	return ret;
}
int BuildImage(unsigned char* bmp,int w ,int h)
{
   int i = 0;
   char color = 0;
   char patte[1024] = {0};
   int pos = 0;
   char heard[54] = {0x42,0x4d,0x30,0x0C,0x01,0,0,0,0,0,0x36,4,0,0,0x28,0,\
                 0,0,0xF5,0,0,0,0x46,0,0,0,0x01,0,0x18,0,0,0,\
                 0,0,0xF8,0x0b,0x01,0,0x12,0x0b,0,0,0x12,0x0b,0,0,0,0,\
                 0,0,0,0,0,0};
   int size = w*h*3;
   int allsize = size +1080;

   heard[2] = allsize&0xFF;
   heard[3] = (allsize>>8)&0xFF;
   heard[4] = (allsize>>16)&0xFF;
   heard[5] = (allsize>>24)&0xFF;

   heard[18] = w&0xFF;
   heard[19] = (w>>8)&0xFF;
   heard[20] = (w>>16)&0xFF;
   heard[21] = (w>>24)&0xFF;

   heard[22] = h&0xFF;
   heard[23] = (h>>8)&0xFF;
   heard[24] = (h>>16)&0xFF;
   heard[25] = (h>>24)&0xFF;

   allsize -=1078;
   heard[34] = allsize&0xFF;
   heard[35] = (allsize>>8)&0xFF;
   heard[36] = (allsize>>16)&0xFF;
   heard[37] = (allsize>>24)&0xFF;

   for(i=0;i<1024;i+=4)
   {
       patte[pos++] = color;
       patte[pos++] = color;
       patte[pos++] = color;
       patte[pos++] = 0;
       color++;
   }
    memcpy(bmp,heard,54);
    memcpy(bmp+54,patte,1024);
}

int saveRawToBmp(uint8_t *pBuf, int32_t width, int32_t height, const char *pFileName)
{
    char heard[54] = { \
            0x42,0x4d,0x30,0x0C,0x01,0x00,0x00,0x00,0x00,0x00, \
            0x36,0x04,0x00,0x00,0x28,0x00,0x00,0x00,0xF5,0x00, \
            0x00,0x00,0x46,0x00,0x00,0x00,0x01,0x00,0x08,0x00, \
            0x00,0x00,0x00,0x00,0xF8,0x0b,0x01,0x00,0x12,0x0b, \
            0x00,0x00,0x12,0x0b,0x00,0x00,0x00,0x00,0x00,0x00, \
            0x00,0x00,0x00,0x00 };
    char patte[1024] = {0};
    char end[2] = {0, 0};
    char color = 0;
    int32_t w = width;
    int32_t h = height;
    int32_t size = w * h;
    int32_t allsize = size + 1080;
    int32_t pos = 0;
    int32_t idx = 0;
    int32_t ret = 0;
    FILE *pFp = NULL;

    do
    {
        if ((NULL == pBuf) || (NULL == pFileName))
        {
            ret = -1;
            break;
        }
        if ((width < 0) || (height < 0))
        {
            ret = -2;
            break;
        }

        heard[2] = allsize & 0xFF;
        heard[3] = (allsize >> 8) & 0xFF;
        heard[4] = (allsize >> 16) & 0xFF;
        heard[5] = (allsize >> 24) & 0xFF;

        heard[18] = w & 0xFF;
        heard[19] = (w >> 8) & 0xFF;
        heard[20] = (w >> 16) & 0xFF;
        heard[21] = (w >> 24) & 0xFF;

        heard[22] = h & 0xFF;
        heard[23] = (h >> 8) & 0xFF;
        heard[24] = (h >> 16)  & 0xFF;
        heard[25] = (h >> 24) & 0xFF;

        allsize -= 1078;
        heard[34] = allsize & 0xFF;
        heard[35] = (allsize >> 8) & 0xFF;
        heard[36] = (allsize >>  16) & 0xFF;
        heard[37] = (allsize >> 24) &  0xFF;

        for (idx = 0; idx < 1024; idx += 4)
        {
            patte[pos++] = color;
            patte[pos++] = color;
            patte[pos++] = color;
            patte[pos++] = 0;
            color++;
        }

        //====== Write raw data ======
        pFp = fopen(pFileName, "wb");
        if (NULL == pFp)
        {
            ret = -3;
            break;
        }

        fwrite(heard, 1, sizeof(heard), pFp);
        fwrite(patte, 1, sizeof(patte), pFp);
#if 1
        for (idx = height - 1; idx >= 0; idx--)
        {
            fwrite(&pBuf[idx * width], sizeof(uint8_t) * width, 1, pFp);
        }
#else
        fwrite(buf, 1, size, pFp);
#endif
        fwrite(end, 1, sizeof(end), pFp);

        fflush(pFp);
        if (0 != fsync(fileno(pFp)))
        {
            ret = -4;
            break;
        }
    } while (0);

    if (NULL != pFp)
    {
        fclose(pFp);
        pFp = NULL;
    }

    return ret;
}

int cam_open(int id)
{
	if (id == 0)
	{
		uvc1_fd = open(VIDEO_DEVICE1, O_RDWR);
		if (uvc1_fd >= 0) return 0;
		else return -1;
	}else if (id == 1)
	{
		uvc2_fd = open(VIDEO_DEVICE2, O_RDWR);
		if (uvc2_fd >= 0) return 0;
		else return -1;
	}
}

int cam_close(int id)
{
	if (0 == id)
		close(uvc1_fd);
	else if (1 == id)
		close(uvc2_fd);
    return 0;
}

int cam_select(int id,int index)
{
    int ret;

    int input = index;
	//struct v4l2_input inp;
	//inp.index = 0;
	//inp.type = V4L2_INPUT_TYPE_CAMERA;
	if (0 == id)
		ret = ioctl(uvc1_fd, VIDIOC_S_INPUT, &input);
	else if (1 == id)
		ret = ioctl(uvc2_fd, VIDIOC_S_INPUT, &input);
    return ret;
}

int cam_init(int id)
{
    int i;
    int ret;

	struct v4l2_fmtdesc fmtdesc;
	fmtdesc.index=0;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	do{
		if (0 == id)
			ret=ioctl(uvc1_fd, VIDIOC_ENUM_FMT, &fmtdesc);
		else if (1 == id)
			ret=ioctl(uvc2_fd, VIDIOC_ENUM_FMT, &fmtdesc);
		DBG("index %d, format 0x%x ", fmtdesc.index,  fmtdesc.pixelformat);
		fmtdesc.index++;
	}while(0==ret);

	/*struct v4l2_streamparm stream_parm;
	memset(&stream_parm, 0, sizeof(struct v4l2_streamparm));
	stream_parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (0 == id)
		ret = ioctl(uvc1_fd, VIDIOC_G_PARM, &stream_parm);
    else if (1 == id)
		ret = ioctl(uvc2_fd, VIDIOC_G_PARM, &stream_parm);
	DBG("befor:framerate: numerator = %d,denominator = %d\n",stream_parm.parm.capture.timeperframe.numerator,stream_parm.parm.capture.timeperframe.denominator);

	struct v4l2_streamparm Stream_Parm;
	memset(&Stream_Parm, 0, sizeof(struct v4l2_streamparm));
	Stream_Parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	Stream_Parm.parm.capture.timeperframe.denominator =30;
	Stream_Parm.parm.capture.timeperframe.numerator = 1;
	if (0 == id)
		ret = ioctl(uvc1_fd, VIDIOC_S_PARM, &Stream_Parm);
    else if (1 == id)
		ret = ioctl(uvc2_fd, VIDIOC_S_PARM, &Stream_Parm);

	memset(&stream_parm, 0, sizeof(struct v4l2_streamparm));
	stream_parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (0 == id)
		ret = ioctl(uvc1_fd, VIDIOC_G_PARM, &stream_parm);
    else if (1 == id)
		ret = ioctl(uvc2_fd, VIDIOC_G_PARM, &stream_parm);
	DBG("after:framerate: numerator = %d,denominator = %d\n",stream_parm.parm.capture.timeperframe.numerator,stream_parm.parm.capture.timeperframe.denominator);
	*/

    struct v4l2_format format;
    memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //format.fmt.pix.pixelformat = V4L2_PIX_FMT_SBGGR8;
	//format.fmt.pix.pixelformat = V4L2_PIX_FMT_SGRBG10;
	//format.fmt.pix.pixelformat = V4L2_PIX_FMT_SGRBG8;
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	//format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV422P;
	//format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	//format.fmt.pix.pixelformat = V4L2_PIX_FMT_YVYU;
	//format.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
	//format.fmt.pix.pixelformat = V4L2_PIX_FMT_VYUY;
	//format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
	//format.fmt.pix.pixelformat = V4L2_PIX_FMT_NV16;
	//format.fmt.pix.pixelformat = V4L2_PIX_FMT_NV12;
	//format.fmt.pix.pixelformat = V4L2_PIX_FMT_HM12;
	//format.fmt.pix.pixelformat = V4L2_PIX_FMT_H264;
    format.fmt.pix.width = IMAGE_WIDTH;
    format.fmt.pix.height = IMAGE_HEIGHT;
	if (0 == id)
		ret=ioctl(uvc1_fd, VIDIOC_TRY_FMT, &format);
	else if (1 == id)
		ret=ioctl(uvc2_fd, VIDIOC_TRY_FMT, &format);
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_TRY_FMT) failed %d(%s)", errno, strerror(errno));
        return ret;
    }

    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (0 == id)
		ret=ioctl(uvc1_fd, VIDIOC_S_FMT, &format);
	else if (1 == id)
		ret=ioctl(uvc2_fd, VIDIOC_S_FMT, &format);
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_S_FMT) failed %d(%s)", errno, strerror(errno));
        return ret;
    }

	if (0 == id)
	{
		if(-1 == ioctl(uvc1_fd, VIDIOC_G_FMT, &format)){//得到图片格式
			perror("set format failed!");
			return -1;
		}
	}else if (1 == id)
	{
		if(-1 == ioctl(uvc2_fd, VIDIOC_G_FMT, &format)){//得到图片格式
			perror("set format failed!");
			return -1;
		}
	}

	printf("fmt.type:\t\t%d\n",format.type);
	printf("pix.pixelformat:\t%c%c%c%c\n", \
			format.fmt.pix.pixelformat & 0xFF,\
			(format.fmt.pix.pixelformat >> 8) & 0xFF, \
			(format.fmt.pix.pixelformat >> 16) & 0xFF,\
			(format.fmt.pix.pixelformat >> 24) & 0xFF);
	printf("pix.width:\t\t%d\n",format.fmt.pix.width);
	printf("pix.height:\t\t%d\n",format.fmt.pix.height);
	printf("pix.field:\t\t%d\n",format.fmt.pix.field);

    struct v4l2_requestbuffers req;
    req.count = BUFFER_COUNT;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
	if (0 == id)
		ret = ioctl(uvc1_fd, VIDIOC_REQBUFS, &req);
	else if (1 == id)
		ret = ioctl(uvc2_fd, VIDIOC_REQBUFS, &req);
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_REQBUFS) failed %d(%s)", errno, strerror(errno));
        return ret;
    }
    DBG("req.count: %d", req.count);
    if (req.count < BUFFER_COUNT)
    {
        DBG("request buffer failed");
        return ret;
    }

    struct v4l2_buffer buffer;
    memset(&buffer, 0, sizeof(buffer));
    buffer.type = req.type;
    buffer.memory = V4L2_MEMORY_MMAP;
    for (i=0; i<req.count; i++)
    {
        buffer.index = i;
		if (0 == id)
			ret = ioctl (uvc1_fd, VIDIOC_QUERYBUF, &buffer);
		else if (1 == id)
			ret = ioctl (uvc2_fd, VIDIOC_QUERYBUF, &buffer);
        if (ret != 0)
        {
            DBG("ioctl(VIDIOC_QUERYBUF) failed %d(%s)", errno, strerror(errno));
            return ret;
        }
        DBG("buffer.length: %d", buffer.length);
        DBG("buffer.m.offset: %d", buffer.m.offset);
		if (0 == id)
		{
			video_buffer_ptr1[i] = (u8*) mmap(NULL, buffer.length, PROT_READ|PROT_WRITE, MAP_SHARED, uvc1_fd, buffer.m.offset);
			if (video_buffer_ptr1[i] == MAP_FAILED)
			{
				DBG("mmap() failed %d(%s)", errno, strerror(errno));
				return -1;
			}
		}else if (1 == id)
		{
			video_buffer_ptr2[i] = (u8*) mmap(NULL, buffer.length, PROT_READ|PROT_WRITE, MAP_SHARED, uvc2_fd, buffer.m.offset);
			if (video_buffer_ptr2[i] == MAP_FAILED)
			{
				DBG("mmap() failed %d(%s)", errno, strerror(errno));
				return -1;
			}

		}

        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory = V4L2_MEMORY_MMAP;
        buffer.index = i;
		if (0 == id)
			ret = ioctl(uvc1_fd, VIDIOC_QBUF, &buffer);
		else if (1 == id)
			ret = ioctl(uvc2_fd, VIDIOC_QBUF, &buffer);
        if (ret != 0)
        {
            DBG("ioctl(VIDIOC_QBUF) failed %d(%s)", errno, strerror(errno));
            return ret;
        }
    }

    int buffer_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (0 == id)
		ret = ioctl(uvc1_fd, VIDIOC_STREAMON, &buffer_type);
	else if (1 == id)
		ret = ioctl(uvc2_fd, VIDIOC_STREAMON, &buffer_type);
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_STREAMON) failed %d(%s)", errno, strerror(errno));
        return ret;
    }

    DBG("cam init done.");

    return 0;
}

int cam_get_image(int id,u8* out_buffer, int out_buffer_size)
{
    int ret;
    int i = 0;
    struct v4l2_buffer buffer;
    u8 buftmp[IMAGE_SIZE] = {0};
    //u8 *tmp = NULL;
    //tmp = malloc(640*480*3);
    //memset(tmp,0x00,640*480*3);
    DBG("cam_get_image start\n");
    //BuildImage(out_buffer,640,480);
    memset(&buffer, 0, sizeof(buffer));
    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer.memory = V4L2_MEMORY_MMAP;
    //buffer.index = BUFFER_COUNT;
	if (0 == id)
		ret = ioctl(uvc1_fd, VIDIOC_DQBUF, &buffer);
	else if (1 == id)
		ret = ioctl(uvc2_fd, VIDIOC_DQBUF, &buffer);
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_DQBUF) failed %d(%s)", errno, strerror(errno));
        return ret;
    }

    if (buffer.index < 0 || buffer.index >= BUFFER_COUNT)
    {
        DBG("invalid buffer index: %d", buffer.index);
        ret = -100;
		return ret;
    }

	if (0 == id)
	{
		DBG("dequeue done, index: %d", buffer.index);
		memcpy(out_buffer, video_buffer_ptr1[buffer.index], IMAGE_SIZE);
		DBG("copy done.");

		ret = ioctl(uvc1_fd, VIDIOC_QBUF, &buffer);
		if (ret != 0)
		{
			DBG("ioctl(VIDIOC_QBUF) failed %d(%s)", errno, strerror(errno));
			return ret;
		}
	}else if (1 == id)
	{
		DBG("dequeue done, index: %d", buffer.index);
		memcpy(out_buffer, video_buffer_ptr2[buffer.index], IMAGE_SIZE);
		DBG("copy done.");

		ret = ioctl(uvc2_fd, VIDIOC_QBUF, &buffer);
		if (ret != 0)
		{
			DBG("ioctl(VIDIOC_QBUF) failed %d(%s)", errno, strerror(errno));
			return ret;
		}
	}
    DBG("enqueue done.");
	//yuyv_to_rgb(buftmp,tmp);
	//memcpy(out_buffer+1078,tmp,640*480*3);
	//free(tmp);
    return 0;
}
