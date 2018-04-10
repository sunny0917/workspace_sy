#ifndef __COMMON__H__
#define __COMMON__H__
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <stdarg.h>

//#include <linux/videodev.h>
#include <linux/kernel.h>
#include <linux/videodev2.h>
#include <stdlib.h>
#include <android/log.h>
typedef enum Camera_pf_level_e
{
	IRIS_PF_ERROR = 1,		/* error conditions    */
	IRIS_PF_WARNING,     	/* warning information */
	IRIS_PF_DEBUG,			/* debug-level         */
	IRIS_PF_INFO ,       	/* always print        */
} TP_PF_LEVEL_E;

#define IRIS_TRACE_LEVEL IRIS_PF_INFO

#define IRIS_DEBUG_LEVEL
#ifdef IRIS_DEBUG_LEVEL
#define LOG_TAG	"IrisLib"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define IRIS_PRINTF( level, fmt, args... ) \
    do { \
        if (level <= IRIS_TRACE_LEVEL)\
        {\
            if(level == IRIS_PF_INFO)\
            {\
            	LOGI("[IRIS]%s().",__FUNCTION__); \
            	LOGI( fmt, ## args ); \
            }\
            else if(level == IRIS_PF_ERROR)\
            {\
            	LOGE("[IRIS_ERR]%s().",__FUNCTION__); \
            	LOGE( fmt, ## args ); \
            }\
            else if(level == IRIS_PF_WARNING)\
            {\
            	LOGI("[IRIS_WARN]%s().",__FUNCTION__); \
            	LOGI( fmt, ## args ); \
            }\
            else if(level == IRIS_PF_DEBUG)\
            {\
            	LOGI("[IRIS_DBG]%s().",__FUNCTION__); \
            	LOGI( fmt, ## args ); \
            }\
        } \
    } while (0)
#else
    #define IRIS_PRINTF( level, fmt, args... ) do{}while(0)
#endif

#endif
