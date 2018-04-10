LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := A210Wireless
LOCAL_SRC_FILES := A210Wireless.c \
					serial.c


LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
  
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)
