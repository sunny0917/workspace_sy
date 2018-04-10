LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := serial
LOCAL_SRC_FILES := serial.c

LOCAL_LDLIBS    := -lm -llog
include $(BUILD_SHARED_LIBRARY)
