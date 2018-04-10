LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := yuvtobmp
LOCAL_SRC_FILES := yuvtobmp.c

LOCAL_LDLIBS    := -ljnigraphics -llog

LOCAL_MULTILIB := 32
include $(BUILD_SHARED_LIBRARY)
