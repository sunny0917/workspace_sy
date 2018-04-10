#ifndef __V4L2__H_
#define __V4L2__H_
#include <A20Common.h>
/****************************************************************
*******V4L2调用接口**数据结构***返回值***************
*****************************************************************/
#define V4L2_DEV_FAIL				-1
#define V4L2_DEV_SUCCESS			0
#define V4L2_DEV_NOFIX				1
#define V4L2_DEV_MMFAIL				2
#define V4L2_DEV_STREAMOFFFAIL		3
#define V4L2_DEV_STREAMONFAIL		4
#define V4L2_DEV_QBUFFAIL			5
#define V4L2_DEV_READERROR			6
#define V4L2_DEV_READFINISHFAIL		7
#define V4L2_DEV_BUSY				8

#define CAMERAINDEXLEFT				0
#define CAMERAINDEXRIGHT			1
#define CAMERAINDEXALL				2

#define CAP_WITH					1920
#define CAP_HIGHT					1080
#define CAP_PIXTH					4
#define MMP_BUF_MAX				    3

#define FACE_W						640
#define FACE_H						480
#define FACE_SIZE					FACE_W*FACE_H//1228800//307200

#define TMPMOUDLE					512
#define TMPBMP						360*360
#define TMPNUM						4

#define ERRORMODE		-1
#define ERROR			-2
#define TURNONBLUE		-3
#define TURNOFFBLUE		-4
#define ADJUSTDIST		-5
#define LOOKATMIRR		-6
#define CLOSETO			-7
#define OPENEYE			-8
#define GAZE			-9
#define LOOKAT			-10
#define REGISTEROK		-11
#define REGISTERFAIL	-12
#define RECOGNIZEOK		-13
#define RECOGNIZEFAIL	-14
#define REPEAT			-15
#define ERRORKEY		-98
#define REBOOTSYSTEM	-99

#define LEFTLIGHT		0
#define RIGHTLIGHT		1
#define REDLIGHT		2
#define GREENLIGHT		3
#define BLUELIGHT		4

#define RIGHTDEV		"/sys/class/gpio_sw/PG2/data"
#define LEFTDEV		    "/sys/class/gpio_sw/PG1/data"

#define REDDEV		    "/sys/class/gpio_sw/PL09/data"
#define GREENDEV		"/sys/class/gpio_sw/PL11/data"
#define BLUEDEV			"/sys/class/gpio_sw/PL08/data"

#define LIGHTON		 	1
#define LIGHTOFF	    0

#define IRISCAMERA		0
#define FACECAMERA		1
#define USBCAMERA		2

/*******************************************************************
**功能描述:打开V4L2设备并且初始化设备
**参数:
**输入:name v4l2的设备名称(路径+名称)
**输入:devnum v4l2的设备序号
**返回值:
**V4L2_DEV_SUCCESS 	打开uvc设备成功
**V4L2_DEV_NOFIX	设备不是uvc设备
**V4L2_DEV_MMFAIL	映射内存失败
**V4L2_DEV_STREAMONFAIL	启动采集失败
**否则为失败，该设备将无法使用
*********************************************************************/
int V4l2_OpenDev(unsigned char* name,int cm,void** CamHandle,int with,int high,int rawfmt);

/*******************************************************************
**功能描述:关闭V4L2设备并且去初始化设备
**参数:
**输入:devnum v4l2的设备序号
**返回值:
**V4L2_DEV_SUCCESS 	关闭设备成功
*********************************************************************/
int V4l2_CloseDev(void* CamHandle);

/*******************************************************************
**功能描述:从V4L2设备获取一帧图像
**参数:
**输入:devnum v4l2的设备序号
**输入:buff 返回数据将保存一帧图像的内存地址，此地址不可释放和更改
**输入:lenght 返回数据的长度
**返回值:
**V4L2_DEV_SUCCESS 			获取图像成功
**V4L2_DEV_READERROR		获取从队列获取缓冲区失败
*********************************************************************/
int GetPrivewFrame(void* CamHandle ,unsigned char* yuv,int CamFmt);

/*******************************************************************
**功能描述:清除所有V4l2设备的缓冲帧
**参数:
**输入:NULL
**返回值:
**V4L2_DEV_SUCCESS			成功结束
*********************************************************************/
int ClearAllFrame(void* CamHandle);

/*******************************************************************
**功能描述:初始化V4L2设备资源
**参数:
**输入:NULL
**返回值:
**V4L2_DEV_SUCCESS			成功结束
*********************************************************************/
int V4l2_InitDev();
int camera_is_read_ready(int devnum);
int V4l2_SetGain(int adc);
int GetDisValue(void);
int OpenIndraredLight(int mode,int value);
int CloseIndraredLight(int mode,int value);
int OpenIndicatLight(int mode);
int CloseIndicatLight(int mode);
void LedModeSet(int mode);
int InitDisTask(int module);
int ExitDisTask(void);
int YV12ToBGR24_Table(unsigned char* pYUV,unsigned char* pBGR32,int width,int height);
#endif
