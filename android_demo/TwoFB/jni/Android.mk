LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := TwoFB
LOCAL_SRC_FILES := TwoFB.c \
					fb.c \
					tofb.c \
					bmp.c \
					rgb_convert.c

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
