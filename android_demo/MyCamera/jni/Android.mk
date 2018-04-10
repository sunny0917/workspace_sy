LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := MyCamera
LOCAL_SRC_FILES := MyCamera.c

LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)
