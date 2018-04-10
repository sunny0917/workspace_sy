LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := A210Iris
LOCAL_SRC_FILES := A210Iris.c \
					serial.c \
					proctol.c \
					cmdProcess2.c \
					#cmdProcess.c \

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)
