#ifndef __V4L2__H_
#define __V4L2__H_
#include <A20Common.h>
#include <linux/videodev2.h>
/****************************************************************
*******V4L2璋冪敤鎺ュ彛**鏁版嵁缁撴瀯***杩斿洖鍊�***************
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
**鍔熻兘鎻忚堪:鎵撳紑V4L2璁惧骞朵笖鍒濆鍖栬澶�
**鍙傛暟:
**杈撳叆:name v4l2鐨勮澶囧悕绉�(璺緞+鍚嶇О)
**杈撳叆:devnum v4l2鐨勮澶囧簭鍙�
**杩斿洖鍊�:
**V4L2_DEV_SUCCESS 	鎵撳紑uvc璁惧鎴愬姛
**V4L2_DEV_NOFIX	璁惧涓嶆槸uvc璁惧
**V4L2_DEV_MMFAIL	鏄犲皠鍐呭瓨澶辫触
**V4L2_DEV_STREAMONFAIL	鍚姩閲囬泦澶辫触
**鍚﹀垯涓哄け璐ワ紝璇ヨ澶囧皢鏃犳硶浣跨敤
*********************************************************************/
int V4l2_OpenDev(unsigned char* name,int cm,void** CamHandle,int with,int high,int rawfmt);

/*******************************************************************
**鍔熻兘鎻忚堪:鍏抽棴V4L2璁惧骞朵笖鍘诲垵濮嬪寲璁惧
**鍙傛暟:
**杈撳叆:devnum v4l2鐨勮澶囧簭鍙�
**杩斿洖鍊�:
**V4L2_DEV_SUCCESS 	鍏抽棴璁惧鎴愬姛
*********************************************************************/
int V4l2_CloseDev(void* CamHandle);

/*******************************************************************
**鍔熻兘鎻忚堪:浠嶸4L2璁惧鑾峰彇涓�甯у浘鍍�
**鍙傛暟:
**杈撳叆:devnum v4l2鐨勮澶囧簭鍙�
**杈撳叆:buff 杩斿洖鏁版嵁灏嗕繚瀛樹竴甯у浘鍍忕殑鍐呭瓨鍦板潃锛屾鍦板潃涓嶅彲閲婃斁鍜屾洿鏀�
**杈撳叆:lenght 杩斿洖鏁版嵁鐨勯暱搴�
**杩斿洖鍊�:
**V4L2_DEV_SUCCESS 			鑾峰彇鍥惧儚鎴愬姛
**V4L2_DEV_READERROR		鑾峰彇浠庨槦鍒楄幏鍙栫紦鍐插尯澶辫触
*********************************************************************/
int GetPrivewFrame(void* CamHandle ,unsigned char* yuv,int CamFmt);

/*******************************************************************
**鍔熻兘鎻忚堪:娓呴櫎鎵�鏈塚4l2璁惧鐨勭紦鍐插抚
**鍙傛暟:
**杈撳叆:NULL
**杩斿洖鍊�:
**V4L2_DEV_SUCCESS			鎴愬姛缁撴潫
*********************************************************************/
int ClearAllFrame(void* CamHandle);

/*******************************************************************
**鍔熻兘鎻忚堪:鍒濆鍖朧4L2璁惧璧勬簮
**鍙傛暟:
**杈撳叆:NULL
**杩斿洖鍊�:
**V4L2_DEV_SUCCESS			鎴愬姛缁撴潫
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
