#ifndef __DEBUG_H__
#define __DEBUG_H__


#include <android/log.h>
//#define DEBUG

#define u8 unsigned char
#define uint8_t unsigned char

#define LOG_TAG	"SY_CAM"
#ifdef DEBUG

#define  LOGD(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define DBG(fmt, args...) LOGD("%s:%d, " fmt, __FUNCTION__, __LINE__, ##args);
#else
#define  LOGD(...)  do {} while (0)
#define DBG(fmt, args...) do {} while (0)
#endif
#define ASSERT(b) \
do \
{ \
    if (!(b)) \
    { \
        LOGD("error on %s:%d", __FUNCTION__, __LINE__); \
        return 0; \
    } \
} while (0)
#endif
