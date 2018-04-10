#include <time.h>
#include <math.h>
#include <A20V4l2.h>
#include <dirent.h>
#include <pthread.h>  
#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>

#define JNI_PORTING_DEBUG
#ifdef JNI_PORTING_DEBUG
#define JNI_DEBUG IRIS_PRINTF
#else
#define JNI_DEBUG
#endif

#define VERSION				0x11004001
//version   0x11004001  0x10000000 linux platforms 0x01000000 Bigsize 0x00004000  main version 0x00000001  sub version 
#define test__
//#define __STUDIO_OPEN_

#define __APP_PATH__			"/sdcard/sykean"
#define __SAVE_PATH__			"/sdcard/sykean/irisimage"
#define __SAVE_STUDIO_PATH__	"/sdcard/sykean/irisstudio"
#define __SAVE_LIST_PATH__		"/sdcard/sykean/irisimagelist"
#define __SAVE_NOT_PATH__		"/sdcard/sykean/irisimagefail"
#define __SAVE_REGIST_PATH__	"/sdcard/sykean/irisregister"
#define __APP_LOG_FILE__		"/sdcard/sykean/logiris"
#define __APP_LOG_FILE_BACK__	"/sdcard/sykean/logiris_back"
#define __APP_INFO_FILE__		"/sdcard/sykean/information"


#define SOURCEWITH	1920
#define SOURCEHIGH	1080
#define DEPTH		1
#define SOURCESIZE	SOURCEWITH*SOURCEHIGH

#define SOURCEWITH2	1920
#define SOURCEHIGH2	1080
#define SOURCESIZE2	SOURCEWITH2*SOURCEHIGH2

int LogFlag = 0;

void* CameraHandle[3] = {0};

int CameraWith[3] = {1920,1920,640};//30 350 130 
int CameraHigh[3] = {1080,1080,480};//30 350 130 
int CameraDepth[3] = {1,1,2};//30 350 130 
int CameraFmt[3] = {3,2,1};

#define EXPRI	680
int irisshut = EXPRI;
int faceshut = EXPRI;
int CameraShutter[3] = {EXPRI,EXPRI,1};

int keyfail = 1;

int debugflag = 0;
FILE* logdebug = NULL;
pthread_mutex_t mutex_logdebug;

/****************鍐呴儴鎺ュ彛**************搴撳唴璋冪敤*******/
/**D*****姣旇緝鏃堕棿宸嚱鏁�*******/
int com_subtract(struct timeval *result, struct timeval *x, struct timeval *y)
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

/**D*****鏄剧ず鏃堕棿宸嚱鏁�*******/
void showtime(struct timeval* tvpre)
{
	struct timeval diff;
	struct timeval tvafter;
	
	gettimeofday(&tvafter, NULL);
	int ret = com_subtract(&diff,tvpre,&tvafter);
	return;
}

/**D*****淇濆瓨鏃ュ織鍒版枃浠跺嚱鏁�****/
int LOG(char *pszFormat, ...)
{
	struct timeval	tv;
	char filename[512] = {0};
    #if 1
 	if(LogFlag == 0)
 	{
		return 0;
	}
	 time_t now;
	 int nByteWrite = 0;
	 va_list pArgList;
	 int currpos;
	 pthread_mutex_lock(&mutex_logdebug);
 
	 if(logdebug == NULL)
	 {
		 pthread_mutex_unlock(&mutex_logdebug);
		 return -1;
	 }
	 currpos = ftell(logdebug);
	if(currpos > 0xA00000)
	{
		printf("log is full delive start currpos=%d\n",currpos);
		fclose(logdebug);
		sprintf(filename,"mv %s %s",__APP_LOG_FILE__,__APP_LOG_FILE_BACK__);
		system(filename);
		logdebug = fopen(__APP_LOG_FILE__,"w");
		if(logdebug == NULL)
		{
		    pthread_mutex_unlock(&mutex_logdebug);
		    return -1;
		}
	}	 
	 va_start(pArgList, pszFormat);
	 fprintf(logdebug,"[IrisDeBug]");
	
	 time(&now);
	 struct tm *t_now = localtime(&now);
	 gettimeofday(&tv, NULL);
	 fprintf(logdebug,"[%04d-%02d-%02d %02d:%02d:%02d:%06d]",
			 t_now->tm_year+1900,t_now->tm_mon+1,t_now->tm_mday,t_now->tm_hour,t_now->tm_min,t_now->tm_sec,tv.tv_usec);
	 nByteWrite = vfprintf(logdebug, pszFormat, pArgList);
	 va_end(pArgList);
	 fflush(logdebug);
	 fsync(logdebug);
	 pthread_mutex_unlock(&mutex_logdebug);
 
	 return nByteWrite;
     #endif
 }

int SaveImage(char* filename,unsigned char* bmp,int w ,int h,int position)
{
   int i = 0;
   char color = 0;
   char end[2] = {0,0};
   char patte[1024] = {0};
   int pos = 0;
   char file[400] = {0};
   time_t   now;
   struct timeval	tv;
   struct   tm     *timenow;
   FILE *outfile;
   char heard[54] = {0x42,0x4d,0x30,0x0C,0x01,0,0,0,0,0,0x36,4,0,0,0x28,0,\
                 0,0,0xF5,0,0,0,0x46,0,0,0,0x01,0,8,0,0,0,\
                 0,0,0xF8,0x0b,0x01,0,0x12,0x0b,0,0,0x12,0x0b,0,0,0,0,\
                 0,0,0,0,0,0};
   int size = w*h;
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
   time(&now);
   timenow = localtime(&now);
   gettimeofday(&tv, NULL);
   sprintf(file,"%s/%d__%04d-%02d-%02d_%02d-%02d-%02d_%06d.bmp",filename,position,timenow->tm_year+1900,timenow->tm_mon+1,timenow->tm_mday,timenow->tm_hour,timenow->tm_min,timenow->tm_sec,tv.tv_usec);
   outfile = fopen( file, "w" );
   if(outfile!=NULL){
       fwrite(heard,54,1,outfile);
       fwrite(patte,1024,1,outfile);
       fwrite(bmp,size,1,outfile);
       fwrite(end,2,1,outfile);
       fclose(outfile);
   }
}

#ifdef __DeBug_L_
 int is_dir_exist(const char *dir_path)
 {
 	DIR *dirp;
	 if(dir_path == NULL)
		 return 0;
	 dirp = opendir(dir_path);
	 if(dirp == NULL)
	 {
		return 0;
	 }
	 closedir(dirp);
	 return 1;
 }
 
int show_debuginfo()
{
	LOG("show debuginfo:\n");
	LOG("totalpic:%d\n",information.total_pic);
	LOG("L        total:%d\n",information.total_L);
	LOG("         buildok:%d\n",information.build_L_success);
	LOG("         buildfail:%d\n",information.build_L_fail);
	LOG("         matchok:%d\n",information.match_L_success);
	LOG("         matchfail:%d\n",information.match_L_fail);
	LOG("R        total:%d\n",information.total_R);
	LOG("         buildok:%d\n",information.build_R_success);
	LOG("         buildfail:%d\n",information.build_R_fail);
	LOG("         matchok:%d\n",information.match_R_success);
	LOG("         matchfail:%d\n",information.match_R_fail);
	LOG("fail becase match:%d\n",information.match_fail);
	LOG("fail becase build:%d\n",information.build_fail);
	LOG("show end\n");
	save_debuginfo();
	return 0;
}

int read_debuginfo()
{
	FILE* fp = NULL;
	fp = fopen(__APP_INFO_FILE__,"rb");
	if(fp == NULL)
	{
		LOG("open infomation fail\n");
		memset(&information,0,sizeof(struct Debug_Info));
		return 0;
	}
	if(fread(&information,sizeof(struct Debug_Info),1,fp)!=1)
	{
		LOG("fread infomation error\n");
	}
	fclose(fp);
	return 0;
}

int save_debuginfo()
{
	FILE* fp = NULL;
	fp = fopen(__APP_INFO_FILE__,"wb");
	if(fp == NULL)
	{
		LOG("open infomation fail");
		return 0;
	}
	if(fwrite(&information,sizeof(struct Debug_Info),1,fp)!=1)
	{
		LOG("fwrite infomation error");
	}
	fclose(fp);
	return 0;
}

#endif

/**D*****鍒濆鍖栨棩蹇楀嚱鏁�******/
void init_debug()
{
	 char tmp[40] = {0};
	 logdebug =  fopen(__APP_LOG_FILE__,"a+");
	 if(logdebug == NULL)
	 {
		 printf("this is wrong ...!\n");
		 return;
	 }
	 pthread_mutex_init(&mutex_logdebug,NULL);
	 JNI_DEBUG(IRIS_PF_DEBUG,"init_debug finish\n");
	 return;
 }
  
/**D*****閲婃斁鏃ュ織鍑芥暟*******/
void release_debug()
 {
	 char tmp[40] = {0};
	 if(logdebug!=NULL)
	 {
		 fclose(logdebug);
		 pthread_mutex_destroy(&mutex_logdebug);
		 logdebug = NULL;
	 }
	 return;
 }

/******************瀵瑰鎺ュ彛***************渚涚敤鎴蜂娇鐢�********/
/********鍒濆鍖栬櫣鑶滃簱***********/
JNIEXPORT jint JNICALL Java_com_sykean_SyA83_ServerInit(JNIEnv *env, jobject obj,jint module)
{
	int ret = 0;
	FILE* fp = NULL;
	char camname[200] = {0};
	init_debug();
	ret = 0;
	if(ret != 0)
	{
		LOG("Lib  fail\n");
		keyfail = 1;
		release_debug();
		return ERRORKEY;
	}
	LOG("lib  success\n");
	keyfail = 0;

	CloseIndraredLight(LEFTLIGHT,0);
	CloseIndraredLight(RIGHTLIGHT,0);
    
	CloseIndicatLight(REDLIGHT);
	CloseIndicatLight(GREENLIGHT);
	CloseIndicatLight(BLUELIGHT);
	CameraHandle[0] = NULL;
	CameraHandle[1] = NULL;
	CameraHandle[2] = NULL;
	
	CameraShutter[0] = EXPRI;
	CameraShutter[1] = EXPRI;
	CameraShutter[2] = 0;
	
	LOG("Iris_ServerInit success\n");
	JNI_DEBUG(IRIS_PF_DEBUG,"Iris_ServerInit success\n");
	return VERSION;
}

/********鍘诲垵濮嬪寲铏硅啘鏈嶅姟***********/
JNIEXPORT jint JNICALL Java_com_sykean_SyA83_ServerRelease(JNIEnv *env, jobject obj)
{
	FILE* fp = NULL;
	release_debug();
	return 0;
} 
/*********鎵撳紑瀵瑰簲鐨勭孩澶栫伅***********/
JNIEXPORT jint JNICALL Java_com_sykean_SyA83_TurnOffLed(JNIEnv *env, jobject obj,jint index,jint onoff,jint value)
{
	int Light = 0;
	if(keyfail==1)
	{
		return ERRORKEY;
	}
	switch(index)
	{
		case 0:
			Light = (value==0)?310:value;
			if(onoff == 1)
			{
				OpenIndraredLight(index,Light);
			}
			else
			{
				CloseIndraredLight(index,Light);
			}
			break;
		case 1:
			Light = (value==0)?310:value;
			if(onoff == 1)
			{
				OpenIndraredLight(index,Light);
			}
			else
			{
				CloseIndraredLight(index,Light);
			}
		case 2:
		case 3:
		case 4:
			if(onoff == 1)
			{
				OpenIndicatLight(index);
			}
			else
			{
				CloseIndicatLight(index);
			}
			break;
		default:
			break;
	}
	return 0;
}

/********璁剧疆鎽勫儚澶寸殑澧炵泭*********/
JNIEXPORT jint JNICALL Java_com_sykean_SyA83_VideoGain(JNIEnv *env, jobject obj,jint index,jint adc)
{
	if(keyfail==1)
	{
		return ERRORKEY;
	}
	if(CameraHandle[index] == NULL||index == USBCAMERA)
	{
		return ERROR;
	}
#if 1
	switch(adc)
	{
		case 1://X1
		case 2://X2
		case 3://X3
		case 4://X4
		case 5://X5
		case 6://X6
		case 7://X7
		case 8://X8
			return V4l2_SetParams((void*)CameraHandle[index],index,adc);
		default:
			break;
	}
#endif
	return -1;
}

/********璁剧疆鎽勫儚澶寸殑鏇濆厜鏃堕棿*********/
JNIEXPORT jint JNICALL Java_com_sykean_SyA83_VideoShutter(JNIEnv *env, jobject obj,jint index,jint shutter)
{
	if(keyfail==1)
	{
		return ERRORKEY;
	}
	if(CameraHandle[index] == NULL||index == USBCAMERA)
	{
		return ERROR;
	}
	return V4l2_SetParams((void*)CameraHandle[index],4,shutter);
}

/********璁剧疆鎽勫儚澶寸殑姘村钩闀滃儚妯″紡*********/
JNIEXPORT jint JNICALL Java_com_sykean_SyA83_VideoHflip(JNIEnv *env, jobject obj,jint index,jint hflip)
{
	if(keyfail==1)
	{
		return ERRORKEY;
	}
	if(CameraHandle[index] == NULL||index == USBCAMERA)
	{
		return ERROR;
	}
	switch(hflip)
	{
		case 0:
		case 1:
			return V4l2_SetParams((void*)CameraHandle[index],2,hflip);
		default:
			break;
	}
	return -1;
}

/********璁剧疆鎽勫儚澶寸殑鍨傜洿缈昏浆*********/
JNIEXPORT jint JNICALL Java_com_sykean_SyA83_VideoVflip(JNIEnv *env, jobject obj,jint index,jint vflip)
{
	if(keyfail==1)
	{
		return ERRORKEY;
	}
	if(CameraHandle[index] == NULL||index == USBCAMERA)
	{
		return ERROR;
	}
	switch(vflip)
	{
		case 0:
		case 1:
			return V4l2_SetParams((void*)CameraHandle[index],3,vflip);
		default:
			break;
	}
	return -1;
}
/********鎵撳紑铏硅啘鎽勫儚澶�*********/
JNIEXPORT jint JNICALL Java_com_sykean_SyA83_OpenIrisCamera(JNIEnv *env, jobject obj)
{
	char camname[200] = {0};
	if(keyfail==1)
	{
		return ERRORKEY;
	}
	if(CameraHandle[IRISCAMERA] != NULL)
	{
		return ERROR;
	}
	sprintf(camname,"/dev/video%d",IRISCAMERA);
    JNI_DEBUG(IRIS_PF_DEBUG,"try open  %s\n",camname);
	if(V4l2_OpenDev(camname,IRISCAMERA,(void*)&CameraHandle[IRISCAMERA],CameraWith[IRISCAMERA],CameraHigh[IRISCAMERA],CameraFmt[IRISCAMERA]) != V4L2_DEV_SUCCESS)
	{
		LOG("open /dev/video%d fail\n",IRISCAMERA);
        JNI_DEBUG(IRIS_PF_DEBUG,"open /dev/video%d fail\n",IRISCAMERA);
		return ERROR;
	}

	return 0;
}

/********浠庤櫣鑶滄憚鍍忓ご鑾峰彇鏁版嵁甯�*********/
JNIEXPORT jint JNICALL Java_com_sykean_SyA83_GetIrisFrame(JNIEnv *env, jobject obj,jbyteArray frame)
{
	int framelen = 0;
    int ret = 0;
	unsigned char* raw = NULL;
	if(keyfail==1)
	{
		return ERRORKEY;
	}
	if(CameraHandle[IRISCAMERA]==NULL)
	{
		return ERROR;
	}
	framelen = (*env)->GetArrayLength(env, frame);
    if(framelen < SOURCESIZE)
	{
		return ERROR;
	}
    raw = (*env)->GetByteArrayElements(env, frame, 0);
	
	int timeoutframe = 0;
    do{
        ret = GetPrivewFrame((void*)CameraHandle[IRISCAMERA],raw,CameraFmt[IRISCAMERA]);
        if(ret == 0)
        {
            (*env)->ReleaseByteArrayElements(env, frame, raw, 0);
            return 0;
        }
        if(ret == -2)
        {
            LOG("camera is not start\n");
            (*env)->ReleaseByteArrayElements(env, frame, raw, 0);
            return ERROR;
        }
        LOG("Get Iris frame fail\n");
		timeoutframe++;
		if(timeoutframe > 20)
		{
			LOG("usb camera have same error to check it\n");
			(*env)->ReleaseByteArrayElements(env, frame, raw, 0);
			return REBOOTSYSTEM;
		}
		usleep(20000);
    }while(ret != 0);
	(*env)->ReleaseByteArrayElements(env, frame, raw, 0);
	return 0;	
	
}

/********鍏抽棴铏硅啘鎽勫儚澶�*********/
JNIEXPORT jint JNICALL Java_com_sykean_SyA83_CloseIrisCamera(JNIEnv *env, jobject obj)
{
	if(keyfail==1)
	{
		return ERRORKEY;
	}
	if(CameraHandle[IRISCAMERA]!=NULL)
	{
		V4l2_CloseDev((void*)CameraHandle[IRISCAMERA]);
		CameraHandle[IRISCAMERA] = NULL;
	}
	return 0;
}

/********澶勭悊鏄剧ず鍥惧儚***********/
JNIEXPORT jint JNICALL Java_com_sykean_SyA83_RatioImage
(JNIEnv *env, jobject obj,jbyteArray Frame,jobject bmp,jint index)
{
	int i,j,offset;
	int pos = 0;
	unsigned char tmp = 0;	
    jint framelen = 0;
    unsigned char* frame = NULL;
    unsigned char* bitmap = NULL;

	if(keyfail==1)
	{
		return ERRORKEY;
	}
	
    framelen = (*env)->GetArrayLength(env, Frame);
    if(framelen < 640*480)
    {
        LOG("get len deferent :%d\n",framelen);
        return ERROR;
    }
    frame = (*env)->GetByteArrayElements(env, Frame, 0);

    if (AndroidBitmap_lockPixels(env, bmp, (void **)&bitmap) < 0)
    {
		LOG("can lock pixels\n");
        (*env)->ReleaseByteArrayElements(env, Frame, frame, 0);
		return ERROR;	
	}
	offset = 0;
	switch(index)
	{
		case 0://水平翻转裁剪
			offset -= SOURCEWITH*4;
			for(i = 0;i < SOURCEHIGH;i+=4)
			{
				offset += SOURCEWITH*4;
				for(j=SOURCEWITH-1; j >= 0; j -=4)
                {
                    tmp = frame[offset+j];
                    bitmap[pos++] = tmp;
                    bitmap[pos++] = tmp;
                    bitmap[pos++] = tmp;
                    bitmap[pos++] = 0xFF;
                }
			}
			break;
		case 1://正常裁剪
			offset -= SOURCEWITH*4;
			for(i = 0;i < SOURCEHIGH;i+=4)
			{
				offset += SOURCEWITH*4;
				for(j=0; j <= SOURCEWITH-1; j +=4)
				{
					tmp = frame[offset+j];
					bitmap[pos++] = tmp;
					bitmap[pos++] = tmp;
					bitmap[pos++] = tmp;
					bitmap[pos++] = 0xFF;
				}
			}
			break;
		case 2://垂直翻转裁剪
			for(i = SOURCEHIGH - 4;i >= 0;i -= 4)
			{
				offset = i*SOURCEWITH;
				for(j=0; j <= SOURCEWITH-1; j +=4)
				{
					tmp = frame[offset+j];
					bitmap[pos++] = tmp;
					bitmap[pos++] = tmp;
					bitmap[pos++] = tmp;
					bitmap[pos++] = 0xFF;
				}
			}
			break;
		default:
			break;
    }
	AndroidBitmap_unlockPixels(env, bmp);
	(*env)->ReleaseByteArrayElements(env, Frame, frame, 0);
    return 0;
}

JNIEXPORT jint JNICALL Java_com_sykean_SyA83_ToBmp
  (JNIEnv *env, jobject obj,jbyteArray Frame,jint Camindex)
{
	int pos,i;
	int size = 0;
	int lensrc = 0;
	unsigned char* frame = NULL;
	lensrc = (*env)->GetArrayLength(env, Frame);
	size = CameraWith[Camindex]*CameraHigh[Camindex]*CameraDepth[Camindex];
	if(lensrc < size)
	{
		return ERROR;
	}
	frame = (*env)->GetByteArrayElements(env, Frame, 0);
	SaveImage(__APP_PATH__,frame,CameraWith[Camindex],CameraHigh[Camindex],Camindex);
    (*env)->ReleaseByteArrayElements(env, Frame, frame, 0);
    return 0;
}
