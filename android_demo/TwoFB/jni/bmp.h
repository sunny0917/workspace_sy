/********************************************************************
	created:	2017/09/13
	filename: 	bmp.h
	author:

	purpose:
*********************************************************************/

#ifndef _bmp_h__
#define _bmp_h__

#define ROOT_PATH "/mnt/sdcard"
#define stricmp strcasecmp


enum {
	eRGB_MIN = 0,
	eRGB_565,
	eRGB_888,
	eRGBX_8888,

	eRGB_AUTO,//如果没有设置输出图片的格式，则自动选择与输入匹配的格式
	eRGB_MAX
};

#define UpAlign4(n) (((n) + 3) & ~3)
#define UpAlign8(n) (((n) + 7) & ~7)

#define BMP_HEAD_SIZE 14

static const char * g_rgbstr[] =
{
	"",
	"rgb565",
	"rgb888",
	"rgbx8888",
};

static const int g_rgbbpp[] =
{
	0, 16, 24, 32,
};

enum {
	eACTION_MIN = -1,
	eBMP_RGB565_TO_FB0_RGB565,
	eBMP_RGB565_TO_FB0_RGB888,
	eBMP_RGB888_TO_FB0_RGB565,
	eBMP_RGB888_TO_FB0_RGB888,

	eACTION_MAX,

	eBMP_TO_BMP,//WIN32下测试用
};

//-------------------------------------------------------------------
/*
　　位图文件的组成
　　结构名称 符 号
　　位图文件头 (bitmap-file header) BITMAPFILEHEADER bmfh
　　位图信息头 (bitmap-information header) BITMAPINFOHEADER bmih
　　彩色表　(color table) RGBQUAD aColors[]
　　图象数据阵列字节 BYTE aBitmapBits[]
*/
typedef struct bmp_header
{
	short twobyte			;//两个字节，用来保证下面成员紧凑排列，这两个字符不能写到文件中
	//14B
	char bfType[2]			;//!文件的类型,该值必需是0x4D42，也就是字符'BM'
	unsigned int bfSize		;//!说明文件的大小，用字节为单位
	unsigned int bfReserved1;//保留，必须设置为0
	unsigned int bfOffBits	;//!说明从文件头开始到实际的图象数据之间的字节的偏移量，这里为14B+sizeof(BMPINFO)
}BMPHEADER;

typedef struct //tagBMPINFO
{
	//40B
	unsigned int biSize			;//!BMPINFO结构所需要的字数
	int biWidth					;//!图象的宽度，以象素为单位
	int biHeight				;//!图象的宽度，以象素为单位,如果该值是正数，说明图像是倒向的，如果该值是负数，则是正向的
	unsigned short biPlanes		;//!目标设备说明位面数，其值将总是被设为1
	unsigned short biBitCount	;//!比特数/象素，其值为1、4、8、16、24、或32
	unsigned int biCompression	;//说明图象数据压缩的类型
#define BI_RGB        0L	//没有压缩
#define BI_RLE8       1L	//每个象素8比特的RLE压缩编码，压缩格式由2字节组成（重复象素计数和颜色索引）；
#define BI_RLE4       2L	//每个象素4比特的RLE压缩编码，压缩格式由2字节组成
#define BI_BITFIELDS  3L	//每个象素的比特由指定的掩码决定。
	unsigned int biSizeImage	;//图象的大小，以字节为单位。当用BI_RGB格式时，可设置为0
	int biXPelsPerMeter			;//水平分辨率，用象素/米表示
	int biYPelsPerMeter			;//垂直分辨率，用象素/米表示
	unsigned int biClrUsed		;//位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用所有调色板项）
	unsigned int biClrImportant	;//对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。
}BMPINFO;

typedef struct
{
	BMPINFO i;
}RGB888INFO;

typedef struct
{
	BMPINFO i;
	unsigned int rgb[4];
}RGB565INFO;

typedef struct
{
	BMPINFO i;
	unsigned int rgb[4];
}RGBX8888INFO;

int save_bmp(const char * path, int w, int h, void * pdata, int bpp);
int screen_shot(const char * path);
int shot(int argc, char * argv[]);

struct tagBMP_Read;
typedef struct tagBMP_Read BMP_READ;

BMP_READ * bmp_open(const char * path);
void bmp_close(BMP_READ * bmp);
int bmp_width(BMP_READ * bmp);
int bmp_height(BMP_READ * bmp);
int bmp_bpp(BMP_READ * bmp);
void * bmp_data(BMP_READ * bmp);
int bmp_forward(BMP_READ * bmp);

typedef struct tagImageInfo {
	int bpp;
	int width;
	int height;
}ImageInfo;
int bmp_info(const char * path, ImageInfo * imageinfo);

//拷贝数据
void rgb_copy(const void * psrc, void * pdst, int sw, int sh, int dw, int dh, int bpp);
void rbg565_copy(const void * psrc, void * pdst, int sw, int sh, int dw, int dh);
void rbg888_copy(const void * psrc, void * pdst, int sw, int sh, int dw, int dh);
void rbgx8888_copy(const void * psrc, void * pdst, int sw, int sh, int dw, int dh);

//行数据翻转
void line_reversal(void * pdata, int w, int h, int bpp);
void rgb565_line_reversal(void * p565, int w, int h);
void rgb888_line_reversal(void * p888, int w, int h);
void rgbx8888_line_reversal(void * p8888, int w, int h);

//转换
typedef void * (* RGB_CONVERT_FUN)(const void * psrc, int w, int h);
RGB_CONVERT_FUN get_convert_func(int frombpp, int tobpp);

//file to fb
void * rgb565_to_bgra8888_buffer(const void * psrc, int w, int h);
void * bgr888_to_bgra8888_buffer(const void * psrc, int w, int h);
void * abgr8888_to_bgra8888_buffer(const void * psrc, int w, int h);


int bmp_to_screen(const char * path);

int tofb(int argc, char * argv[]);
//-------------------------------------------------------------------
#endif // #ifndef _bmp_h__
