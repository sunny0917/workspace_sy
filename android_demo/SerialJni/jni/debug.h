#ifndef __DEBUG_H__
#define __DEBUG_H__
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include<pthread.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <termios.h>
#include <sys/types.h>
#include <jni.h>

#include<assert.h>
#include <android/log.h>

typedef enum debug_level_e
{
	DEBUG_ERROR = 1,		/* error conditions    */
	DEBUG_WARNING,     	/* warning information */
	DEBUG_DEBUG,			/* debug-level         */
	DEBUG_INFO ,       	/* always print        */
} DEBUG_LEVEL_E;

#define TRACE_LEVEL DEBUG_INFO

#define DEBUG_LEVEL
#ifdef DEBUG_LEVEL
#define LOG_TAG	"SERIAL"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define DEBUG_PRINTF( level, fmt, args... ) \
    do { \
        if (level <= TRACE_LEVEL)\
        {\
            if(level == DEBUG_INFO)\
            {\
            	LOGI("[INFO]%s().",__FUNCTION__); \
            	LOGI( fmt, ## args ); \
            }\
            else if(level == DEBUG_ERROR)\
            {\
            	LOGE("[ERR]%s().",__FUNCTION__); \
            	LOGE( fmt, ## args ); \
            }\
            else if(level == DEBUG_WARNING)\
            {\
            	LOGI("[WARN]%s().",__FUNCTION__); \
            	LOGI( fmt, ## args ); \
            }\
            else if(level == DEBUG_DEBUG)\
            {\
            	LOGI("[DBG]%s().",__FUNCTION__); \
            	LOGI( fmt, ## args ); \
            }\
        } \
    } while (0)
#else
    #define DEBUG_PRINTF( level, fmt, args... ) do{}while(0)
#endif

#endif
