LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := IDCard
LOCAL_SRC_FILES := IDCard.c \
					hyidcard.c \
					serial.c
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
