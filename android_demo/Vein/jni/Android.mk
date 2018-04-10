LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := SykeanFingerVein
LOCAL_SRC_FILES := protol.c	\
					serial.c \
					cmdProcess.c \
					sykeanFingerVein.c

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)
